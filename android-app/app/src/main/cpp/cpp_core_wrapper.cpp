#include <jni.h>
#include "jvmcontroller.h"
#include <array>

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
        jint maxFrames
) {
    return initializeBuffer(static_cast<size_t>(maxFrames));
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
    if (env->GetDirectBufferCapacity(frameBuffer) != sizeof(float) * FRAME_SIZE) {
        return false;
    }
    auto begin = static_cast<float*>(env->GetDirectBufferAddress(frameBuffer));
    return submitFrame(begin, env->GetDirectBufferCapacity(frameBuffer) / 4);
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
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_PhysicsAPI_setResistanceOrigin(
        JNIEnv* env,
        jobject _,
        jobject frameBuffer,
        jboolean isBilateral,
        jfloat x1,
        jfloat y1,
        jfloat x2,
        jfloat y2
        ) {

}