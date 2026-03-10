#pragma once
#include <jni.h>
extern JavaVM* g_vm;
// global variable for jvm access
JavaVM* g_vm = nullptr;
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_vm = vm;
    return JNI_VERSION_1_6;
}
