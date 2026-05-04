#include <jni.h>
#include "jvmcontroller.h"
#include "cpp_global.h"
#include <sstream>
#include <iomanip>
#include <array>
#include "Logger.h"

extern "C" JNIEXPORT void JNICALL Java_com_example_myapplication_PhysicsAPI_initCppLogger(
        JNIEnv* env,
        jobject _,
        jobject bridge
        ){
    jobject loggerGlobalRef = env->NewGlobalRef(bridge);
    jclass bridgeLocalClass = env->GetObjectClass(loggerGlobalRef);
    jclass bridgePermClass = (jclass)env->NewGlobalRef(bridgeLocalClass);
    env->DeleteLocalRef(bridgeLocalClass);
    jmethodID logMethodID = env->GetStaticMethodID(bridgePermClass, "logFromCpp", "(Ljava/lang/String;)V");
    publicAccessLogger.activate(bridgePermClass, logMethodID, g_vm);
}

extern "C"
JNIEXPORT jint JNICALL
        Java_com_example_myapplication_PhysicsAPI_testIncrement(
        JNIEnv* env,
        jobject _,
        jint value
) {
    return testIncrement(value);
}
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
//external fun submitFrame(frame: ByteBuffer, metadata: FrameMetaData): Boolean
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
    float* begin = reinterpret_cast<float*>(env->GetDirectBufferAddress(frameBuffer));
    return submitFrame(begin, env->GetDirectBufferCapacity(frameBuffer) / 4);
}
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
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_PhysicsAPI_logFromCpp(
        JNIEnv* env,
        jobject _,
        jstring str
        ){}