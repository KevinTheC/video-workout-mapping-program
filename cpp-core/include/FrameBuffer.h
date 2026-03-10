#pragma once
#include <cstdlib>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <memory>
#include <algorithm>
#define FRAME_SIZE 99
struct FrameUpdateData{
    float angle;
};
class FrameUpdateObserver{
    public:
        virtual ~FrameUpdateObserver() {}
        virtual void onFrameUpdate(FrameUpdateData fud) = 0;
};
class FrameBuffer{
    public:
        FrameBuffer();
        bool shutdown();
        bool initialize(size_t maxFrames);
        bool submitFrame(float* bufferBegin, size_t count);
        size_t const getFrameCount();
        enum JointOffset {
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
        void assignFrameUpdateObserver(FrameUpdateObserver* frameUpdateObserver);
        bool destroyFrameUpdateObserver();
    private:
        size_t maxFrames;
        std::vector<float> buffer;
        size_t nextFrame;
        FrameUpdateObserver* frameUpdateObserver = nullptr;
};