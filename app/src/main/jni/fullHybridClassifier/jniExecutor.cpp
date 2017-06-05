//
// Created by mahmoodms on 4/3/2017.
//
#include "rt_nonfinite.h"
#include "classifySSVEP_types.h"
#include "classifySSVEP.h"
/*Additional Includes*/
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG "jniExecutor-cpp"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define SAMPLING_RATE   250.0
#define  RETURN_LEN     7
//TODO: Will need to pass all data (4 arrays), and length of array.
// How do I get array length in C++?

// Function Definitions

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
    return 0.0;
}

extern "C" {
    JNIEXPORT jdouble JNICALL
    Java_com_mahmoodms_bluetooth_eegssvepwheelchairdemo_DeviceControlActivity_jssvepclassifier1(
            JNIEnv *env, jobject jobject1, jdoubleArray array1) {
        jdouble  *X = env->GetDoubleArrayElements(array1, NULL);
        if (X==NULL) LOGE("ERROR - C_ARRAY IS NULL");
        return classifySSVEP(X,(double)3);
    }
}

extern "C" {
JNIEXPORT jdouble JNICALL
Java_com_mahmoodms_bluetooth_eegssvepwheelchairdemo_DeviceControlActivity_jssvepclassifier2(
        JNIEnv *env, jobject jobject1, jdoubleArray array1, jdouble thresholdFraction) {
    jdouble  *X = env->GetDoubleArrayElements(array1, NULL);
    if (X==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    return classifySSVEP(X,thresholdFraction);
}
}


extern "C" {
JNIEXPORT jint JNICALL
Java_com_mahmoodms_bluetooth_eegssvepwheelchairdemo_DeviceControlActivity_jmainInitialization(
        JNIEnv *env, jobject obj, jboolean terminate) {
    if(!(bool)terminate) {
        classifySSVEP_initialize();
//        main_EOGClassifier();
        return 0;
    } else {
        return -1;
    }
}
}
