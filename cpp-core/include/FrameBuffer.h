#pragma once
#include <cstdlib>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <memory>
#include <algorithm>
#define FRAME_SIZE 132
class FrameBuffer{
    public:
        FrameBuffer();
        bool shutdown();
        bool initialize(size_t maxFrames);
        bool submitFrame(float* bufferBegin, size_t count);
    private:
        size_t maxFrames;
        std::vector<float> buffer;
        size_t nextFrame;
};