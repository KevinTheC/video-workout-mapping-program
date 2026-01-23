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
bool FrameBuffer::submitFrame(float* bufferBegin, size_t count) {
    //these could potentially be exceptions instead
    if (this->maxFrames == 0) {
        return false;
    }
    if (count != FRAME_SIZE) {
        return false;
    }
    if (this->buffer.capacity() < nextFrame + count) {
        return false;
    }
    std::copy(bufferBegin, bufferBegin + count, buffer.begin() + nextFrame);
    nextFrame += count;
    return true;
};
FrameBuffer::FrameBuffer() {
    this->maxFrames = 0;
    this->buffer = std::vector<float>(0);
    this->nextFrame = 0;
};