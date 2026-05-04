#include "FrameBuffer.h"
#include <iostream>
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
bool FrameBuffer::submitFrame(float* bufferBegin, size_t numFloats) {
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
    //this is probably an issue, nothings being written
    std::copy(bufferBegin, bufferBegin + numFloats, buffer.begin() + nextFrame);
    nextFrame += numFloats;
    if (frameUpdateObserver) {
        this->frameUpdateObserver->onFrameUpdate((uintptr_t)this);
    }
    return true;
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
inline glm::vec3 getLandmark(std::vector<float>& buffer, size_t jointIndex, size_t frameIndex) {
    return glm::make_vec3(std::addressof(*(
            //frame_size is in bytes
            buffer.begin() + (frameIndex * FRAME_SIZE) + (jointIndex * 3)
                                         )));
};
BodyState FrameBuffer::getState(size_t index) {
    if (index + 1 > getFrameCount())
        throw std::out_of_range("Tried retrieving state from a non-existant frame");
    //HOW ABOUT WE START WITH JUST ELBOW FLEXION!
    glm::vec3 elbowToHand = 
        getLandmark(buffer, JointOffset::LeftElbow, index) -
        getLandmark(buffer, JointOffset::LeftWrist, index);
    glm::vec3 elbowToShoulder =
        getLandmark(buffer, JointOffset::LeftElbow, index) -
        getLandmark(buffer, JointOffset::LeftShoulder, index);
    auto LW = getLandmark(buffer, JointOffset::LeftWrist, index);
    auto LE = getLandmark(buffer, JointOffset::LeftElbow, index);
    auto LS = getLandmark(buffer, JointOffset::LeftShoulder, index);
    auto LP = getLandmark(buffer, JointOffset::LeftPinky, index);
    auto LI = getLandmark(buffer, JointOffset::LeftIndex, index);
    return BodyState{BodySide{JointState{}, JointState{
        glm::dot(glm::normalize(elbowToHand), glm::normalize(elbowToShoulder)), glm::dot(glm::normalize(glm::cross(glm::normalize(elbowToHand), glm::normalize(elbowToShoulder))), glm::normalize(LP - LI))
    }, JointState{LE[2], LP[2], LI[2]}
}};
};
BodyState FrameBuffer::getState() {
    if (getFrameCount() < 1)
        throw std::out_of_range("No frames exist to get the state from");
    return getState(getFrameCount() - 1);
};