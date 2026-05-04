#include <jni.h>
class Logger{
private:
    bool isActive = false;
    JavaVM* globalJavaVM = nullptr;
    jclass bridgeClass;
    jmethodID logMethodID;
public:
    void activate(jclass bridgeClass, jmethodID logMethodID, JavaVM* globalJavaVM){
        this->bridgeClass = bridgeClass;
        this->logMethodID = logMethodID;
        this->globalJavaVM = globalJavaVM;
        this->isActive = true;
    }
    bool remoteLog(const std::string& message) {
        if (!isActive)
            return false;
        JNIEnv* env;
        jint getEnvRes = globalJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (getEnvRes == JNI_EDETACHED) {
            if (globalJavaVM->AttachCurrentThread(&env, nullptr) != JNI_OK) {
                return false; 
            }
        } else if (getEnvRes == JNI_EVERSION) {
            return false;
        }

        jstring jmsg = env->NewStringUTF(message.c_str());
        env->CallStaticVoidMethod(bridgeClass, logMethodID, jmsg);
        env->DeleteLocalRef(jmsg);
        return true;
    }
};
Logger publicAccessLogger;