#pragma once
#include <stdint.h>
struct FrameFormat{
    uint8_t width;
    uint8_t height;
    enum Format{
        YUV420,
        RGB,
        RGBA
    };
    Format format;
};
int testIncrement(int value){
    return value + 2;
};
bool initializeBuffer(int maxFrames){
    return true;
};
bool submitFrame(uint8_t* bufferBegin, size_t length, FrameFormat format){
    return true;
};
bool shutdown(){
    return true;
};
bool registerListener(){
    return true;
};