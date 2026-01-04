package com.example.myapplication

class PhysicsAPI {
    companion object {
        init {
            System.loadLibrary("android_wrapper_cpp")
        }
        @JvmStatic
        external fun testIncrement(value: Int): Int
    }
}