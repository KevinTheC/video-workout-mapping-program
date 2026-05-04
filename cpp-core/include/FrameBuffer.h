#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <cstdlib>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <memory>
#include <algorithm>
#define FRAME_SIZE 99
class FrameUpdateObserver{
    public:
        virtual ~FrameUpdateObserver() {}
        virtual void onFrameUpdate(uintptr_t handle) = 0;
};
struct JointState{
    float flexion;
    float adduction;
    float rotation;
};
struct BodySide {
    JointState shoulder;
    JointState elbow;
    JointState hip;
    JointState knee;
    JointState ankle;
    JointState wrist;
    float shoulderHipDiff;
    float shoulderEarDiff;
    float femur;
    float torso;
};
struct BodyState{
    BodySide left;
    BodySide right;
};
class FrameBuffer{
    public:
        FrameBuffer();
        bool shutdown();
        bool initialize(size_t maxFrames);
        bool submitFrame(float* bufferBegin, size_t numFloats);
        size_t const getFrameCount();
        BodyState getState(size_t index);
        BodyState getState();
        void assignFrameUpdateObserver(FrameUpdateObserver* frameUpdateObserver);
        bool destroyFrameUpdateObserver();
        enum JointOffset {
            LeftEar = 7,
            RightEar,
            LeftShoulder = 11,
            RightShoulder,
            LeftElbow,
            RightElbow,
            LeftWrist,
            RightWrist,
            LeftPinky,
            RightPinky,
            LeftIndex,
            RightIndex,
            LeftThumb,
            RightThumb,
            LeftHip,
            RightHip,
            LeftKnee,
            RightKnee,
            LeftAnkle,
            RightAnkle,
            LeftHeel,
            RightHeel,
            LeftIndexToe,
            RightIndexToe
        };
    private:
        size_t maxFrames;
        std::vector<float> buffer;
        size_t nextFrame;
        FrameUpdateObserver* frameUpdateObserver = nullptr;
};