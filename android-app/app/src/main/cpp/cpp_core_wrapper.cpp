#include <jni.h>
#include "controller.h"

extern "C"
JNIEXPORT jint JNICALL
        Java_com_example_myapplication_PhysicsAPI_testIncrement(
        JNIEnv* env,
        jobject _,
        jint value
) {
    return testIncrement(value);
}
//@JvmStatic
//        external fun initializeBuffer(maxFrames: Int): Boolean
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_myapplication_PhysicsAPI_initializeBuffer(
        JNIEnv* env,
        jobject _,
        jint maxFrames,
        jint height,
        jint width,
        jint format
) {
    return initializeBuffer(
            FrameFormat{
                static_cast<uint8_t>(height),
                static_cast<uint8_t>(width),
                static_cast<FrameFormat::Format>(format)},
            static_cast<uint8_t>(maxFrames));
}

//@JvmStatic
//        external fun submitFrame(frame: ByteBuffer, metadata: FrameMetaData): Boolean
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_myapplication_PhysicsAPI_submitFrame(
        JNIEnv* env,
        jobject _,
        jobject frameBuffer
) {
    uint8_t* begin = static_cast<uint8_t*>(env->GetDirectBufferAddress(frameBuffer));
    size_t capacity = env->GetDirectBufferCapacity(frameBuffer);
    return submitFrame(begin, capacity);
}
//@JvmStatic
//        external fun shutdown(): Boolean
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_myapplication_PhysicsAPI_shutdown(
        JNIEnv* env,
        jobject _
) {
    return shutdown();
}

//@JvmStatic
////need to add a listener class when I learn how to do it
//external fun registerListener(): Boolean
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_myapplication_PhysicsAPI_registerListener(
        JNIEnv* env,
        jobject _
) {
    return registerListener();
}
