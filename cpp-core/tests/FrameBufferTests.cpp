#include <cassert>
#include <functional>
#include <tuple>
#include <FrameBuffer.h>
#include <iostream>
void testOrderingOfCalls();
void testCalls(std::function<bool()>,bool,const char*);
float buf[FRAME_SIZE];
int main() {
    testOrderingOfCalls();
};
void testOrderingOfCalls(){
    using Test = std::tuple<std::function<bool()>, bool, const char*>;

    std::vector<Test> tests = {
        //default constructor
        {[](){return FrameBuffer().initialize(1);}, true, "Initialization failed after default constructor"},
        {[](){return FrameBuffer().shutdown();}, false, "Shutdown occured with uninitialized buffer after default constructor"},
        {[](){return FrameBuffer().submitFrame(buf, FRAME_SIZE);}, false, "Frame submission occured with uninitialized buffer after default constructor"},
        
        //frame submission
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            return fb.submitFrame(buf, FRAME_SIZE);
        }, true, "Frame submission after init failed"},
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            fb.shutdown();
            return fb.submitFrame(buf, FRAME_SIZE);
        }, false, "Frame submission after shutdown succeeded"},
        {[](){
            FrameBuffer fb;
            fb.initialize(2);
            fb.submitFrame(buf, FRAME_SIZE);
            return fb.submitFrame(buf, FRAME_SIZE);
        }, true, "Consecutive frame submissions failed"},

        //initialization
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            return fb.initialize(1);
        }, false, "Double initialization succeeded"},
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            fb.shutdown();
            return fb.initialize(1);
        }, true, "Initialize after shutdown failed"},
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            fb.submitFrame(buf, FRAME_SIZE);
            return fb.initialize(1);
        }, false, "Initialize after frame submission succeeded"},

        //shutdown
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            return fb.shutdown();
        }, true, "Shutdown after init failed"},
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            fb.shutdown();
            return fb.shutdown();;
        }, false, "Double shutdown succeeded"},
        {[](){
            FrameBuffer fb;
            fb.initialize(1);
            fb.submitFrame(buf, FRAME_SIZE);
            return fb.shutdown();
        }, true, "Shutdown after frame submission failed"}
    };
    for (auto& t : tests){
        testCalls(std::get<0>(t),std::get<1>(t),std::get<2>(t));
    };
};
void testCalls(std::function<bool()> func1,
    bool expected,
    const char* onFail){
        if (!(func1() == expected)){
            std::cerr << onFail << std::endl;
            assert(false);
        }
};