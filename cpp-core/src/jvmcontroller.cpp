#include "jvmcontroller.h"
FrameBuffer fb;
int testIncrement(int value){
    return value + 2;
};
bool initializeBuffer(size_t maxFrames){
    return fb.initialize(maxFrames);
};
bool submitFrame(float* bufferBegin, size_t numFloats){
    return fb.submitFrame(bufferBegin, numFloats);
};
bool shutdown(){
    return fb.shutdown();
};
void registerListener(FrameUpdateObserver* fuo){
    fb.assignFrameUpdateObserver(fuo);
};
void deleteListener(){
    fb.destroyFrameUpdateObserver();
};
void setResistanceOrigin(uint8_t* bufferBegin, bool isBilateral, float x1, float y1, float x2, float y2){

};
BodyState tempMethod(){
    return fb.getState();
}