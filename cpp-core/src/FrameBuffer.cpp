#include "FrameBuffer.h"
#include <iostream>
//this really shouldn't be inlined
inline glm::vec3 getLandmark(std::vector<float>& buffer, size_t jointIndex, size_t frameIndex) {
    return glm::make_vec3(std::addressof(*(
            //frame_size is in bytes
            buffer.begin() + (frameIndex * FRAME_SIZE) + (jointIndex * 3)
                                         )));
};

bool FrameBuffer::shutdown() {
    if (this->maxFrames == 0)
        return false;
    this->buffer = std::vector<float>(0);
    this->maxFrames = 0;
    this->nextFrame = 0;
    return true;
};
bool FrameBuffer::initialize(size_t maxFrames) {
    if (this->maxFrames != 0)
        return false;
    this->buffer = std::vector<float>();
    this->buffer.reserve(maxFrames * FRAME_SIZE);
    this->maxFrames = maxFrames;
    this->nextFrame = 0;
    return true;
};
bool FrameBuffer::submitFrame(float* bufferBegin, const size_t numFloats) {
    //these could potentially be exceptions instead
    if (this->maxFrames == 0) {
        return false;
    }
    if (numFloats != FRAME_SIZE) {
        return false;
    }
    if (this->buffer.capacity() < nextFrame + numFloats) {
        return false;
    }
    std::copy(bufferBegin, bufferBegin + numFloats, buffer.begin() + nextFrame);
    //reorganizing the copy might increase speed
    //fixZDepth(nextFrame / FRAME_SIZE);
    nextFrame += numFloats;
    if (frameUpdateObserver) {
        this->frameUpdateObserver->onFrameUpdate((uintptr_t)this);
    }
    return true;
};
void FrameBuffer::fixZDepth(const size_t frameIndex){
    //this is my solution to Mediapose's z depth issue (where its slightly inaccurate). The goal is to use 2d space coordinates at calculate the length of limbs based on that.
    //knowing the common ratios between the human body, we can determine which bone is the least "foreshortened", aka most accurate in a 2d only space.
    //this method won't work if all bones in the body are somehow at angled from the camera's POV, which I can fix later if need be.
    size_t samples[7] = {0,1,3,4,12,14,15};
    //these values are the indexes of the shoulder-shoulder, shoulder-hip, shoulder-elbow, elbow-wrist
    size_t best = 0;
    float actualLength = 0;
    for (size_t i = 0; i < sizeof(samples) / sizeof(size_t); ++i) {
        glm::vec3 one = getLandmark(buffer, poseTree.edges[samples[i]].origin, frameIndex);
        glm::vec3 two = getLandmark(buffer, poseTree.edges[samples[i]].destination, frameIndex);
        float distance = sqrt((one[0] - two[0]) * (one[0] - two[0]) + (one[1] - two[1]) * (one[1] - two[1])) / poseTree.edges[samples[i]].calibratedWeight;
        if (distance > actualLength) {
            best = i;
            actualLength = distance;
        }
    }
    //Now that we have the most accurate bone, we can use this to calculate the expect pixel difference for each bone.
    //using this, we can spread out from the shoulders, fixing the Z depth.
    //We will use mediapipe's generated Z-depths to determine if the landmark is forward or backward from the expected one.

    //multiply the best actual bone by the expected ratio for each bone.
    //left shoulder will be our root node. 
    float newValues[33];
    for (size_t i = 0; i < sizeof(newValues) / sizeof(float); i++)
        newValues[i] = buffer[(frameIndex * FRAME_SIZE) + (i * 3)];
    for (const PoseEdge& edge : poseTree.edges) {
        glm::vec3 origin = getLandmark(buffer, edge.origin, frameIndex);
        glm::vec3 destination = getLandmark(buffer, edge.destination, frameIndex);
        float x_mag = origin[0] - destination[0];
        float y_mag = origin[1] - destination[1];
        float total_mag = actualLength * edge.calibratedWeight;
        total_mag = total_mag * total_mag - x_mag * x_mag - y_mag * y_mag;
        if (total_mag < 0.0f)
            total_mag = 0.0f;
        float z_mag = std::sqrt(total_mag);
        
        //flip z_mag if need be 
        if (buffer[(frameIndex * FRAME_SIZE) + (edge.origin * 3)] > buffer[(frameIndex * FRAME_SIZE) + (edge.destination * 3)]) {
            z_mag = z_mag * -1.0f;
        }
        newValues[edge.destination] = newValues[edge.origin] + z_mag;
    }
    //rewrite to my buffer
    for (size_t i = 0; i < sizeof(newValues) / sizeof(float); i++)
        buffer[(frameIndex * FRAME_SIZE) + (i * 3)] = newValues[i];
};
FrameBuffer::FrameBuffer() {
    this->maxFrames = 0;
    this->buffer = std::vector<float>(0);
    this->nextFrame = 0;
    this->frameUpdateObserver = nullptr;
};

size_t const FrameBuffer::getFrameCount(){
    return nextFrame / FRAME_SIZE;
};
void FrameBuffer::assignFrameUpdateObserver(FrameUpdateObserver* frameUpdateObserver) {
    this->frameUpdateObserver = frameUpdateObserver;
};
bool FrameBuffer::destroyFrameUpdateObserver() {
    if (!this->frameUpdateObserver)
        return false;
    delete this->frameUpdateObserver;
    this->frameUpdateObserver = nullptr;
    return true;
};
const BodyState FrameBuffer::getState(size_t index) {
    if (index + 1 > getFrameCount())
        throw std::out_of_range("Tried retrieving state from a non-existant frame");
    BodyState returnState;
    //this will definitely need to be changed to builder pattern, i dont know if compiler will be able to optimize this garbage return statement bs
    //we can use a value to invert all the values for L/R
    for (size_t leftToRight = 0; leftToRight < 2; ++leftToRight){
        BodySide side = leftToRight ? returnState.right : returnState.left;
        glm::vec3 shoulderToShoulder = 
            getLandmark(buffer, JointOffset::LeftShoulder + (1 * leftToRight), index) -
            getLandmark(buffer, JointOffset::RightShoulder + (-1 * leftToRight), index);
        glm::vec3 shoulderToHip = 
            getLandmark(buffer, JointOffset::LeftShoulder + leftToRight, index) -
            getLandmark(buffer, JointOffset::LeftHip + leftToRight, index);
        glm::vec3 hipToKnee = 
            getLandmark(buffer, JointOffset::LeftHip + leftToRight, index) -
            getLandmark(buffer, JointOffset::LeftKnee + leftToRight, index);
        glm::vec3 earToShoulder =
            getLandmark(buffer, JointOffset::LeftEar + leftToRight, index) -
            getLandmark(buffer, JointOffset::LeftShoulder + leftToRight, index);
        float basis = glm::length(shoulderToShoulder);
        side.torso = glm::length(shoulderToHip) / basis;
        side.shoulderEarDiff = glm::length(earToShoulder) / basis;
        side.femur = glm::length(hipToKnee) / basis;

        shoulderToShoulder = glm::normalize(shoulderToShoulder);
        shoulderToHip = glm::normalize(shoulderToHip);
        hipToKnee = glm::normalize(hipToKnee);
        earToShoulder = glm::normalize(earToShoulder);

        glm::vec3 elbowToHand = glm::normalize(getLandmark(buffer, JointOffset::LeftElbow + leftToRight, index) - getLandmark(buffer, JointOffset::LeftWrist + leftToRight, index));
        glm::vec3 shoulderToElbow = glm::normalize(getLandmark(buffer, JointOffset::LeftShoulder + leftToRight, index) - getLandmark(buffer, JointOffset::LeftElbow + leftToRight, index));
        glm::vec3 hipToHip = glm::normalize(getLandmark(buffer, JointOffset::LeftHip + (1 * leftToRight), index) - getLandmark(buffer, JointOffset::RightHip + (-1 * leftToRight), index));
        glm::vec3 kneeToAnkle = glm::normalize(getLandmark(buffer, JointOffset::LeftKnee + leftToRight, index) - getLandmark(buffer, JointOffset::LeftAnkle + leftToRight, index));
        glm::vec3 ankleToFoot = glm::normalize(getLandmark(buffer, JointOffset::LeftAnkle + leftToRight, index) - getLandmark(buffer, JointOffset::LeftIndexToe + leftToRight, index));
        glm::vec3 shoulderLocalXTransverse = glm::normalize(shoulderToShoulder - (glm::dot(shoulderToShoulder, shoulderToElbow)) * shoulderToElbow);
        glm::vec3 hipLocalXTransverse = glm::normalize(hipToHip - (glm::dot(hipToHip, hipToKnee)) * hipToKnee);
        glm::vec3 kneeLocalXTransverse = glm::normalize(hipToHip - (glm::dot(hipToHip, ankleToFoot)) * ankleToFoot);




        //a lot of these cross products will need to be checked to make sure they make sense for both right and left
        //they are definitely wrong, left hand rule
        //also can definitely strip some normalization spam
        //ALSO CHANGE ELBOW REVERSE ELBOW VERY IMPORTANT
        side.shoulder = JointState{
            std::atan2f(
                glm::dot(glm::normalize(shoulderToElbow), shoulderToHip),
                glm::dot(glm::normalize(shoulderToElbow), glm::normalize(glm::cross(shoulderToShoulder, shoulderToHip)))
            ),
            std::atan2f(
                glm::dot(glm::normalize(shoulderToElbow), shoulderToHip),
                glm::dot(glm::normalize(shoulderToElbow), shoulderToShoulder)
            ),
            std::atan2f(
                glm::dot(glm::normalize(elbowToHand), shoulderLocalXTransverse),
                glm::dot(glm::normalize(elbowToHand), glm::normalize(glm::cross(shoulderLocalXTransverse, shoulderToElbow)))
            )
        };
        if (glm::dot(elbowToHand, shoulderToElbow) > 0.95f)
        {
            side.elbow = JointState{
                    constexpr(180.0f / 180.0f * M_PI), NAN, TEMPORARY_FLOAT};
        }
        else
        {
            side.elbow = JointState{
                std::atan2f(
                    glm::dot(glm::normalize(elbowToHand), shoulderToElbow),
                    glm::dot(glm::normalize(elbowToHand), glm::cross(shoulderToElbow, glm::cross(elbowToHand, shoulderToElbow)))
                ),
                NAN, TEMPORARY_FLOAT};
        }
        side.hip = JointState{
            std::atan2f(
                glm::dot(glm::normalize(hipToKnee), shoulderToHip),
                glm::dot(glm::normalize(hipToKnee), glm::normalize(glm::cross(hipToHip, -1.0f * shoulderToHip)))
            ),
            std::atan2f(
                glm::dot(glm::normalize(hipToKnee), hipToHip),
                glm::dot(glm::normalize(hipToKnee), glm::normalize(glm::cross(hipToHip, -1.0f * shoulderToHip)))
            ),
            std::atan2f(
                glm::dot(glm::normalize(kneeToAnkle), hipLocalXTransverse),
                glm::dot(glm::normalize(kneeToAnkle), glm::normalize(glm::cross(hipLocalXTransverse, hipToKnee)))
            )
        };
        if (glm::dot(kneeToAnkle, hipToKnee) > 0.95f) {
            side.knee = JointState{
                std::atan2f(
                    glm::dot(glm::normalize(kneeToAnkle), hipToKnee),
                    glm::dot(glm::normalize(kneeToAnkle), glm::cross(hipToKnee, glm::cross(kneeToAnkle, hipToKnee)))
                ),
                NAN, 0.0f
            };
        }
        else 
        {
            side.knee = JointState{
                std::atan2f(
                    glm::dot(glm::normalize(kneeToAnkle), hipToKnee),
                    glm::dot(glm::normalize(kneeToAnkle), glm::cross(hipToKnee, glm::cross(kneeToAnkle, hipToKnee)))
                ),
                NAN, 
                std::atan2f(
                    glm::dot(glm::normalize(ankleToFoot), kneeLocalXTransverse),
                    glm::dot(glm::normalize(ankleToFoot), glm::normalize(glm::cross(kneeLocalXTransverse, kneeToAnkle)))
                )
            };
        }
        side.ankle = JointState{
            std::atan2f(
                glm::dot(glm::normalize(ankleToFoot), kneeToAnkle),
                glm::dot(glm::normalize(ankleToFoot), glm::cross(hipToKnee, glm::cross(ankleToFoot, kneeToAnkle)))
            ), NAN, TEMPORARY_FLOAT//foot inversion, need toes
        };
        side.wrist = JointState{
            TEMPORARY_FLOAT, NAN, TEMPORARY_FLOAT
        };
    }
    return returnState;
};
const BodyState FrameBuffer::getState() {
    if (getFrameCount() < 1)
        throw std::out_of_range("No frames exist to get the state from");
    return getState(getFrameCount() - 1);
};