//
// Created by mahmoodms on 4/3/2017.
//
#include "rt_nonfinite.h"
#include "classifySSVEP_types.h"
#include "classifySSVEP.h"
#include "convoluteSignals.h"
#include "classifySSVEP3.h"
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
Java_com_mahmoodms_bluetooth_eegssvepwheelchairdemo_DeviceControlActivity_jClassifySSVEP3(
        JNIEnv *env, jobject jobject1, jdoubleArray ch1, jdoubleArray ch2) {
    jdouble  *X1 = env->GetDoubleArrayElements(ch1, NULL);
    jdouble  *X2 = env->GetDoubleArrayElements(ch2, NULL);
    if (X1==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (X2==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    return classifySSVEP3(X1,X2,3.15);
}
}

extern "C" {
JNIEXPORT jdoubleArray JNICALL
Java_com_mahmoodms_bluetooth_eegssvepwheelchairdemo_DeviceControlActivity_jConvoluteSignals(
        JNIEnv *env, jobject jobject1, jdoubleArray ch1, jdoubleArray ch2) {
    jdouble  *X1 = env->GetDoubleArrayElements(ch1, NULL);
    jdouble  *X2 = env->GetDoubleArrayElements(ch2, NULL);
    if (X1==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (X2==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    double Y[1998];
    jdoubleArray m_result = env->NewDoubleArray(1998);
    convoluteSignals(X1,X2,Y);
    env->SetDoubleArrayRegion(m_result, 0, 1998, Y);
    return m_result;
}
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
