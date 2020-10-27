#include <jni.h>
#include <string>
#include "fluidsynth.h"

extern "C" JNIEXPORT jstring JNICALL
Java_net_medimuse_fluidsynth_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
