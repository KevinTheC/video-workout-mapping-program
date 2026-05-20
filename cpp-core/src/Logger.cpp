#include "Logger.h"
Logger publicAccessLogger = Logger();
void Logger::activate(jclass bridgeClass, jmethodID logMethodID, JavaVM* globalJavaVM){
    this->bridgeClass = bridgeClass;
    this->logMethodID = logMethodID;
    this->globalJavaVM = globalJavaVM;
    this->isActive = true;
};