#pragma once
#include <type_traits>
#include <cstdint>
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
    float torso;
    float shoulderEarDiff;
    float femur;
};
struct BodyState{
    //
    BodySide left;
    BodySide right;
};
static_assert(sizeof(BodyState) == ((sizeof(float) * 3 * 6) + (sizeof(float) * 3)) * 2, 
    "CRITICAL: BodyState size must be an exact multiple of float size, FrameBuffer::updateState relies on this. Make sure you look at FrameBuffer::updateState before changing state size");
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
struct PoseEdge{
    size_t origin;
    size_t destination;
    float calibratedWeight;
};
struct PoseTree{
    //don't need breadth first search if I just hardcode it, order shouldn't change
    PoseEdge edges[23] = {
        {JointOffset::LeftShoulder, JointOffset::RightShoulder, 1.0f},
        {JointOffset::LeftShoulder, JointOffset::LeftElbow, 0.53f}, //humerus
        {JointOffset::LeftShoulder, JointOffset::LeftEar, 0.53f},
        {JointOffset::LeftShoulder, JointOffset::LeftHip, 1.06f}, //torso
        {JointOffset::LeftElbow, JointOffset::LeftWrist, 0.63f}, //forearm
        {JointOffset::LeftWrist, JointOffset::LeftPinky, 0.28f},
        {JointOffset::LeftWrist, JointOffset::LeftIndex, 0.19f},
        {JointOffset::LeftWrist, JointOffset::LeftThumb, 0.13f},
        {JointOffset::LeftHip, JointOffset::LeftKnee, 1.06f},
        {JointOffset::LeftKnee, JointOffset::LeftAnkle, 0.94f},
        {JointOffset::LeftAnkle, JointOffset::LeftHeel, 0.188f},
        {JointOffset::LeftHeel, JointOffset::LeftIndexToe, 0.313f},
        
        {JointOffset::RightShoulder, JointOffset::RightElbow, 0.53f}, //humerus
        {JointOffset::RightShoulder, JointOffset::RightEar, 0.53f},
        {JointOffset::RightShoulder, JointOffset::RightHip, 1.06f}, //torso
        {JointOffset::RightElbow, JointOffset::RightWrist, 0.63f}, //forearm
        {JointOffset::RightWrist, JointOffset::RightPinky, 0.28f},
        {JointOffset::RightWrist, JointOffset::RightIndex, 0.19f},
        {JointOffset::RightWrist, JointOffset::RightThumb, 0.13f},
        {JointOffset::RightHip, JointOffset::RightKnee, 1.06f},
        {JointOffset::RightKnee, JointOffset::RightAnkle, 0.94f},
        {JointOffset::RightAnkle, JointOffset::RightHeel, 0.188f},
        {JointOffset::RightHeel, JointOffset::RightIndexToe, 0.313f},
    };
};