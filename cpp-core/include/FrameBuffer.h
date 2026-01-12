#pragma once
#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <vector>
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
    FrameFormat() = delete;
    FrameFormat(uint8_t h, uint8_t w, Format f)
        : height(h), width(w), format(f)
    {};
    size_t getFrameSize() const {
        switch (format){
            case Format::YUV420:
                return 0;
            case Format::RGB:
                return height * width * 3;
            case Format::RGBA:
                return height * width * 4;
        }
        throw std::logic_error("Unknown FrameFormat::Format");
    };
};
class FrameBuffer{
    public:
        FrameBuffer();
        bool shutdown();
        bool initialize(const FrameFormat& format, size_t maxFrames);
        bool submitFrame(uint8_t* bufferBegin, size_t length);
    private:
        FrameFormat format;
        size_t maxFrames;
        std::vector<uint8_t> buffer;
        size_t nextFrame;
};