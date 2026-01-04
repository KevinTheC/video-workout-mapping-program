#include <jni.h>
#include "controller.h"

extern "C"
JNIEXPORT jint JNICALL
        Java_com_example_myapplication_PhysicsAPI_testIncrement(
        JNIEnv* env,
        jobject,
        jint value
) {
    return testIncrement(value);
}