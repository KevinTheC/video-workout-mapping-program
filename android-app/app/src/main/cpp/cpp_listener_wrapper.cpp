#include <jni.h>
#include "cpp_global.h"
#include "jvmcontroller.h"

jobject frameUpdateGlobalRef = nullptr;
jmethodID frameUpdateMethodID = nullptr;

class KotlinFrameUpdateObserver : public FrameUpdateObserver{
public:
    virtual ~KotlinFrameUpdateObserver() {

    }
    virtual void onFrameUpdate(FrameUpdateData fud) {
        JNIEnv* currentEnv;
        jint res = g_vm->GetEnv((void**)&currentEnv, JNI_VERSION_1_6);

        if (res == JNI_EDETACHED) {
            g_vm->AttachCurrentThread(&currentEnv, nullptr);
        }

        jobject responseObj = currentEnv->NewObject(frameUpdateResponseClass, frameUpdateResponseConstructor, fud.angle);
        currentEnv->CallObjectMethod(frameUpdateGlobalRef, frameUpdateMethodID, responseObj);
        currentEnv->DeleteLocalRef(responseObj);
    };
    KotlinFrameUpdateObserver(JNIEnv* env, jobject ref, jmethodID ID, jclass clazz) :
     frameUpdateGlobalRef(ref),
     frameUpdateMethodID(ID),
     frameUpdateResponseConstructor(env->GetMethodID(clazz, "<init>", "(F)V")),
     frameUpdateResponseClass(clazz) {}
private:
    //ref for listener
    jobject frameUpdateGlobalRef;
    //id for lambda invoke method on listener
    jmethodID frameUpdateMethodID;
    //id for response constructor
    jmethodID frameUpdateResponseConstructor;
    jclass frameUpdateResponseClass;
};
//idk man something is wrong in here
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_PhysicsAPI_deleteListener(
        JNIEnv* env,
        jobject _
        ) {
    deleteListener();
    env->DeleteGlobalRef(frameUpdateGlobalRef);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_PhysicsAPI_registerListener(
        JNIEnv* env,
        jobject caller,
        jobject listener,
        jclass responseClass
) {
    if (frameUpdateGlobalRef) {
        Java_com_example_myapplication_PhysicsAPI_deleteListener(env, caller);
    }
    frameUpdateGlobalRef = env->NewGlobalRef(listener);

    jclass lambdaClass = env->GetObjectClass(frameUpdateGlobalRef);
    frameUpdateMethodID = env->GetMethodID(lambdaClass, "invoke", "(Ljava/lang/Object;)Ljava/lang/Object;");
    //DONT FORGET TO PIN CLASSES! THEY ARE OBJECTS APPARENTLY and the response class gets GC'd after this call
    responseClass = (jclass)env->NewGlobalRef(responseClass);

    registerListener(new KotlinFrameUpdateObserver(env, frameUpdateGlobalRef, frameUpdateMethodID, responseClass));
}
