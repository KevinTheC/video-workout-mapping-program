#pragma once
#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <algorithm>
struct FrameFormat{
    uint8_t height;
    uint8_t width;
    enum Format{
        YUV420,
        RGB,
        RGBA
    };
    Format format;
    size_t getFrameSize() const {
        switch (format){
            case Format::YUV420:
                return 0;
            case Format::RGB:
                return 0;
            case Format::RGBA:
                return 0;
        }
        throw std::logic_error("Unknown FrameFormat::Format");
    };
};
class FrameBuffer{
    public:
        FrameBuffer();
        bool shutdown();
        bool initialize(FrameFormat format, size_t maxFrames);
        bool submitFrame(uint8_t* bufferBegin, size_t length);
        ~FrameBuffer();
    private:
        FrameFormat format;
        size_t maxFrames;
        uint8_t* buffer;
        uint8_t* next;
};