#include "FrameBuffer.h"
bool FrameBuffer::shutdown() {
    if (buffer == nullptr)
        return false;
    delete[] buffer;
    buffer = nullptr;
    next = nullptr;
    maxFrames = 0;
    return true;
};
bool FrameBuffer::initialize(FrameFormat format, size_t maxFrames) {
    if (buffer != nullptr)
        return false;
    this->format = format;
    this->buffer = new uint8_t[format.getFrameSize() * maxFrames];
    this->next = buffer;
    this->maxFrames = maxFrames;
    return true;
};
bool FrameBuffer::submitFrame(uint8_t* bufferBegin, size_t length) {
    //these could potentially be exceptions instead
    if (buffer == nullptr)
        return false;
    size_t used = next - buffer;
    if (length + used > maxFrames) {
        return false;
    }
    if (length != format.getFrameSize()) {
        return false;
    }
    std::copy(bufferBegin, bufferBegin + length, next);
    next += length;
    return true;
};
FrameBuffer::FrameBuffer(){
    this->format = FrameFormat{0, 0, FrameFormat::Format::RGB};
    this->maxFrames = 0;
    this->buffer = nullptr;
    this->next = nullptr;
};
FrameBuffer::~FrameBuffer(){
    if (buffer != nullptr)
        delete[] buffer;
};