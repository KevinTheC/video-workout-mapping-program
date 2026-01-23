#pragma once
#include <cstdint>
#include "FrameBuffer.h"
int testIncrement(int value){
    return value + 2;
};
bool initializeBuffer(size_t maxFrames){
    return true;
};
bool submitFrame(float* bufferBegin, size_t size){
    return true;
};
bool shutdown(){
    return true;
};
bool registerListener(){
    return true;
};
void setResistanceOrigin(uint8_t* bufferBegin, bool isBilateral, float x1, float y1, float x2, float y2){

};