#pragma once
#include <cstdint>
#include "FrameBuffer.h"
extern FrameBuffer fb;
int testIncrement(int value);
bool initializeBuffer(size_t maxFrames);
bool submitFrame(float* bufferBegin, size_t numFloats);
// bool submitFrame(float* bufferBegin, size_t width, size_t height, int64_t timestamp);
void updateState(float* kotlinBufferAddress, std::vector<size_t> requestedUpdateIndexes, size_t stateIndex);
bool shutdown();
void registerListener(FrameUpdateObserver* fuo);
void deleteListener();
void setResistanceOrigin(uint8_t* bufferBegin, bool isBilateral, float x1, float y1, float x2, float y2);

const BodyState& tempMethod();