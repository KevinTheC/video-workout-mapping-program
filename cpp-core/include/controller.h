#pragma once
#include <cstdint>
#include "FrameBuffer.h"
int testIncrement(int value){
    return value + 2;
};
bool initializeBuffer(FrameFormat format, size_t maxFrames){
    return true;
};
bool submitFrame(uint8_t* bufferBegin, size_t length){
    return true;
};
bool shutdown(){
    return true;
};
bool registerListener(){
    return true;
};