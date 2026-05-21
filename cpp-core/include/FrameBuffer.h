#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <cstdlib>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <memory>
#include <algorithm>
#include "BodyState.h"
#include "Logger.h"
#define FRAME_SIZE 99
#define TEMPORARY_FLOAT 0.0f
class FrameUpdateObserver{
    public:
        virtual ~FrameUpdateObserver() {}
        virtual void onFrameUpdate(uintptr_t handle) = 0;
};
class FrameBuffer{
    public:
        FrameBuffer();
        bool shutdown();
        bool initialize(size_t maxFrames);
        bool submitFrame(float* bufferBegin, const size_t numFloats);
        size_t const getFrameCount();
        const BodyState& getState(size_t index);
        const BodyState& getState();
        void assignFrameUpdateObserver(FrameUpdateObserver* frameUpdateObserver);
        bool destroyFrameUpdateObserver();
        void updateState(float* kotlinBufferAddress, std::vector<size_t> requestedUpdateIndexes, size_t stateIndex);
        void updateState(float* kotlinBufferAddress, std::vector<size_t> requestedUpdateIndexes);
    private:
        size_t maxFrames;
        std::vector<float> buffer;
        std::vector<const BodyState> stateBuffer;
        size_t nextFrame;
        PoseTree poseTree;
        FrameUpdateObserver* frameUpdateObserver = nullptr;
        void fixZDepth(const size_t frameIndex);
        const BodyState createState(size_t index);
};