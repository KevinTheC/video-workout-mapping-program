#include "cpp_global.h"
JavaVM* g_vm = nullptr;
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_vm = vm;
    return JNI_VERSION_1_6;
}
