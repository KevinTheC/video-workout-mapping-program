#pragma once
#include <cstdint>
#include "FrameBuffer.h"
extern FrameBuffer fb;
int testIncrement(int value);
bool initializeBuffer(size_t maxFrames);
bool submitFrame(float* bufferBegin, size_t size);
bool shutdown();
void registerListener(FrameUpdateObserver* fuo);
void deleteListener();
void setResistanceOrigin(uint8_t* bufferBegin, bool isBilateral, float x1, float y1, float x2, float y2);