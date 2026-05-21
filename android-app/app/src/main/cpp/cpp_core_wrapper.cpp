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

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_PhysicsAPI_nativeGetBatchState(
        JNIEnv* env,
        jobject thiz,
        jintArray indices,
        jfloatArray output_buffer,
        jlong state_index){
    jint* nativeIndices = static_cast<jint*>(env->GetPrimitiveArrayCritical(indices, nullptr));
    jfloat* nativeOutputBuffer = static_cast<jfloat*>(env->GetPrimitiveArrayCritical(output_buffer, nullptr));
    jsize requestCount = env->GetArrayLength(indices);

    std::vector<size_t> requestedUpdateIndexes(nativeIndices, nativeIndices + requestCount);
    if (state_index > -1)
    {
        fb.updateState(
            static_cast<float*>(nativeOutputBuffer),
            requestedUpdateIndexes,
            static_cast<size_t>(state_index)
        );
    }
    else
    {
        fb.updateState(
            static_cast<float*>(nativeOutputBuffer),
            requestedUpdateIndexes
        );
    }
    // 0 tells the JVM "output_buffer" was modified, commit the data back to Kotlin
    env->ReleasePrimitiveArrayCritical(indices, nativeIndices, JNI_ABORT);
    env->ReleasePrimitiveArrayCritical(output_buffer, nativeOutputBuffer, 0);
}