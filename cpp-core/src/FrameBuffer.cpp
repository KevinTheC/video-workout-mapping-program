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
BodyState FrameBuffer::getState(size_t index) {
    if (index + 1 > getFrameCount())
        throw std::out_of_range("Tried retrieving state from a non-existant frame");
    //this will definitely need to be changed to builder pattern, i dont know if compiler will be able to optimize this garbage return statement bs
    glm::vec3 elbowToHand = 
        getLandmark(buffer, JointOffset::LeftElbow, index) -
        getLandmark(buffer, JointOffset::LeftWrist, index);
    glm::vec3 elbowToShoulder =
        getLandmark(buffer, JointOffset::LeftElbow, index) -
        getLandmark(buffer, JointOffset::LeftShoulder, index);
    glm::vec3 shoulderToShoulder = 
        getLandmark(buffer, JointOffset::LeftShoulder, index) -
        getLandmark(buffer, JointOffset::RightShoulder, index);
    glm::vec3 shoulderToHip = 
        getLandmark(buffer, JointOffset::LeftShoulder, index) -
        getLandmark(buffer, JointOffset::LeftHip, index);
    glm::vec3 hipToKnee = 
        getLandmark(buffer, JointOffset::LeftHip, index) -
        getLandmark(buffer, JointOffset::LeftKnee, index);
    glm::vec3 hipToHip = 
        getLandmark(buffer, JointOffset::LeftHip, index) -
        getLandmark(buffer, JointOffset::RightHip, index);
    glm::vec3 kneeToAnkle = 
        getLandmark(buffer, JointOffset::LeftKnee, index) -
        getLandmark(buffer, JointOffset::LeftAnkle, index);
    glm::vec3 ankleToFoot =
        getLandmark(buffer, JointOffset::LeftAnkle, index) -
        getLandmark(buffer, JointOffset::LeftIndexToe, index);
    glm::vec3 shoulderLocalXTransverse = 
        glm::normalize(shoulderToShoulder - (glm::dot(shoulderToShoulder, elbowToShoulder)) * elbowToShoulder);
    glm::vec3 hipLocalXTransverse = 
        glm::normalize(hipToHip - (glm::dot(hipToHip, hipToKnee)) * hipToKnee);
    glm::vec3 kneeLocalXTransverse =
        glm::normalize(hipToHip - (glm::dot(hipToHip, ankleToFoot)) * ankleToFoot);
    //a lot of these cross products will need to be checked to make sure they make sense for both right and left
    //they are definitely wrong, left hand rule
    //also, 0.0f is placeholders while NAN is indeterminate
    //I think the arctan method is probably more effective than doing dot product, and makes more sense. maybe thats why the wrist joint wasn't working too
    //also can definitely strip some normalization spam
    float basis = glm::length(shoulderToShoulder);
    return BodyState{
        BodySide{
            JointState{//shoulder joint
                glm::dot(glm::normalize(elbowToShoulder), glm::normalize(glm::cross(shoulderToShoulder, shoulderToHip)))
                glm::dot(glm::normalize(elbowToShoulder), glm::normalize(shoulderToShoulder)),
                std::atan2f(
                    glm::dot(glm::normalize(elbowToHand), shoulderLocalXTransverse),
                    glm::dot(glm::normalize(elbowToHand), glm::normalize(glm::cross(shoulderLocalXTransverse, elbowToShoulder)))
                )},
            JointState{glm::dot(glm::normalize(elbowToHand), glm::normalize(elbowToShoulder)), NAN, 0.0f},//elbow joint
            JointState{
                //flexion = pulling knees up, front vector. shoulder to knee vector is probably better for determining hip position
                glm::dot(glm::normalize(hipToKnee), glm::normalize(glm::cross(hipToHip, shoulderToHip))),
                //adduction
                glm::dot(glm::normalize(hipToKnee), glm::normalize(hipToHip)),
                std::atan2f(
                    glm::dot(glm::normalize(kneeToAnkle), hipLocalXTransverse),
                    glm::dot(glm::normalize(kneeToAnkle), glm::normalize(glm::cross(hipLocalXTransverse, hipToKnee)))
                )},//hip joint
            JointState{
                glm::dot(glm::normalize(hipToKnee), glm::normalize(kneeToAnkle)), NAN, 
                std::atan2f(
                    glm::dot(glm::normalize(ankleToFoot), kneeLocalXTransverse),
                    glm::dot(glm::normalize(ankleToFoot), glm::normalize(glm::cross(kneeLocalXTransverse, kneeToAnkle)))
                )//clamp this, knee rotation doesn't occur below 10degrees of flexion
            },//knee joint
            JointState{
                glm::dot(ankleToFoot, kneeToAnkle), NAN, 0.0f//foot inversion, need toes
            },//ankle joint
            JointState{
                0.0f, NAN, 0.0f
            },//wrist, just skip
            glm::length(shoulderToHip) / basis,
            glm::length(getLandmark(buffer, JointOffset::LeftEar, index) - getLandmark(buffer, JointOffset::LeftShoulder, index)) / basis,
            glm::length(hipToKnee) / basis,
        }
    };
};
BodyState FrameBuffer::getState() {
    if (getFrameCount() < 1)
        throw std::out_of_range("No frames exist to get the state from");
    return getState(getFrameCount() - 1);
};