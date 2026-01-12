#include "FrameBuffer.h"
#include <iostream>
bool FrameBuffer::shutdown() {
    if (this->maxFrames == 0)
        return false;
    this->buffer = std::vector<uint8_t>(0);
    this->maxFrames = 0;
    this->nextFrame = 0;
    return true;
};
bool FrameBuffer::initialize(const FrameFormat& format, size_t maxFrames) {
    if (this->maxFrames != 0)
        return false;
    this->format = format;
    this->buffer = std::vector<uint8_t>();
    this->buffer.reserve(format.getFrameSize() * maxFrames);
    this->maxFrames = maxFrames;
    this->nextFrame = 0;
    return true;
};
bool FrameBuffer::submitFrame(uint8_t* bufferBegin, size_t length) {
    //these could potentially be exceptions instead
    if (this->maxFrames == 0) {
        return false;
    }
    if (this->buffer.capacity() < this->buffer.size() + length) {
        return false;
    }
    if (length != this->format.getFrameSize()) {
        return false;
    }
    std::copy(bufferBegin, bufferBegin + length, this->buffer.begin() + nextFrame);
    nextFrame += length;
    return true;
};
FrameBuffer::FrameBuffer() : format(FrameFormat{0, 0, FrameFormat::Format::RGB}){
    this->maxFrames = 0;
    this->buffer = std::vector<uint8_t>(0);
    this->nextFrame = 0;
};