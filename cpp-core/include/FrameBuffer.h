#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <cstdlib>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include "BodyState.h"
#define FRAME_SIZE 99
class FrameUpdateObserver{
    public:
        virtual ~FrameUpdateObserver() {}
        virtual void onFrameUpdate(uintptr_t handle) = 0;
};
class FrameBuffer{
    public:
        struct Rotations {
    float omega; // Azimuth (Rotation around Y)
    float theta; // Elevation (Angle from Y-axis)
};

Rotations getUpVectorRotations(glm::vec3 dir) {

    dir = glm::normalize(dir);
    float theta = std::acos(dir[1]); 
    float omega = std::atan2(dir[2], dir[0]);

    return {omega, theta};
};

        FrameBuffer();
        bool shutdown();
        bool initialize(size_t maxFrames);
        bool submitFrame(float* bufferBegin, const size_t numFloats);
        size_t const getFrameCount();
        BodyState getState(size_t index);
        BodyState getState();
        void assignFrameUpdateObserver(FrameUpdateObserver* frameUpdateObserver);
        bool destroyFrameUpdateObserver();
    private:
        size_t maxFrames;
        std::vector<float> buffer;
        size_t nextFrame;
        PoseTree poseTree;
        FrameUpdateObserver* frameUpdateObserver = nullptr;
        void fixZDepth(const size_t frameIndex);
};