package com.example.myapplication

import java.nio.ByteBuffer

class PhysicsAPI {
    companion object {
        init {
            System.loadLibrary("android_wrapper_cpp")
        }
        @JvmStatic
        external fun testIncrement(value: Int): Int
        @JvmStatic
        external fun initializeBuffer(maxFrames: Int): Boolean
        @JvmStatic
        external fun submitFrame(frame: ByteBuffer, width: Int, height: Int, format: Int): Boolean
        inline fun submitFrame(frame: ByteBuffer, width: Int, height: Int, format: FrameFormat) =
            submitFrame(frame, width, height, format.ordinal)
        @JvmStatic
        external fun shutdown(): Boolean
        @JvmStatic
        //need to add a listener class when I learn how to do it
        external fun registerListener(): Boolean
    }
    enum class FrameFormat{
        YUV420,
        RGB,
        RGBA
    }
}