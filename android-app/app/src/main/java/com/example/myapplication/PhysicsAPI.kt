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
        external fun submitFrame(frame: ByteBuffer): Boolean
        @JvmStatic
        external fun setResistanceOrigin(frame: ByteBuffer, isBilateral: Boolean, x1 : Float, y1 : Float, x2: Float, y2: Float)
        inline fun setResistanceOrigin(frame: ByteBuffer, x1 : Float, y1 : Float) =
            setResistanceOrigin(frame, false, x1, y1, 0f, 0f)
        inline fun setResistanceOrigin(frame: ByteBuffer, x1 : Float, y1 : Float, x2: Float, y2: Float) =
            setResistanceOrigin(frame, true, x1, y1, x2, y2)
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