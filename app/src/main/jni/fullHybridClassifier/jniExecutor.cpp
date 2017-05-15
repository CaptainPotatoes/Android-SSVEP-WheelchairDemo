//
// Created by mahmoodms on 4/3/2017.
//
#include "rt_nonfinite.h"
#include "eegcfilt.h"
#include "eogcfilt_a.h"
#include "EOGClassifier.h"
#include "EOGClassifier2.h"
/*Additional Includes*/
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG "jniExecutor-cpp"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define SAMPLING_RATE   250.0
#define  RETURN_LEN     7
//TODO: Will need to pass all data (4 arrays), and length of array.
// How do I get array length in C++?

/*static emxArray_real_T *argInit_zeros_nx1_real_T(int n) {
    //n is the size of the array to be initialized
    emxArray_real_T *Y;
    Y = emxCreateND_real_T(1,&n);
    for (int i = 0; i < n; ++i) {
        Y->data[i] = 0.0;
    }
    return Y;
}

static emxArray_real_T *argInit_Array_real_T(int size, jdouble *array) {
    emxArray_real_T *result;
    result = emxCreateND_real_T(1, &size);
    result->size[0U] = size;
    for (int i=0; i<size; i++) {
        result->data[i] = array[i];
    }
    return result;
}

extern "C" {
JNIEXPORT jdoubleArray JNICALL
//Call this function with (data, data, data, data, datalen, Fs);
//Don't need array size; can check size array in C.
Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jfullHybridClassifier(
        JNIEnv *env, jobject jobject1, jdoubleArray array1, jdoubleArray array2, jdoubleArray array3, jdoubleArray array4, jboolean eogOnly) {
    jdouble  *c_array_ch1 = env->GetDoubleArrayElements(array1, NULL);
    jdouble  *c_array_ch2 = env->GetDoubleArrayElements(array2, NULL);
    jdouble  *c_array_ch3 = env->GetDoubleArrayElements(array3, NULL);
    jdouble  *c_array_ch4 = env->GetDoubleArrayElements(array4, NULL);
    if (c_array_ch1==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (c_array_ch2==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (c_array_ch3==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (c_array_ch4==NULL) LOGE("ERROR - C_ARRAY IS NULL");
    int len = env->GetArrayLength(array1);
    emxArray_real_T *ch1 = argInit_Array_real_T(len, c_array_ch1);
    emxArray_real_T *ch2 = argInit_Array_real_T(len, c_array_ch2);
    emxArray_real_T *ch3 = argInit_Array_real_T(len, c_array_ch3);
    emxArray_real_T *ch4 = argInit_Array_real_T(len, c_array_ch4);
    //Easy way :: Or Create own function:
    boolean_T c_eogOnly = eogOnly;
    double r_array[RETURN_LEN];
    fullHybridClassifier(ch1, ch2, ch3, ch4, SAMPLING_RATE, c_eogOnly, r_array);
    //TODO: Call function fullHybridClassifier
    double *result = r_array;
    jdoubleArray m_result;
    m_result = env->NewDoubleArray(RETURN_LEN);
    env->SetDoubleArrayRegion(m_result, 0, RETURN_LEN, result);
    return m_result;
}
}

 extern "C" {
JNIEXPORT jdoubleArray JNICALL
//Call this function with (data, data, data, data, datalen, Fs);
//Don't need array size; can check size array in C.
Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jeegcfilt(
        JNIEnv *env, jobject jobject1, jdoubleArray array) {
    int len = 0;
    jdouble  *c_array = env->GetDoubleArrayElements(array, NULL);
    if (c_array!=NULL) {
        len = env->GetArrayLength(array);
    } else {
        LOGE("ERROR - C_ARRAY IS NULL");
    }
    emxArray_real_T *X = argInit_Array_real_T(len, c_array);
    emxArray_real_T *Y;
    emxInitArray_real_T(&Y, 2);
    Y = argInit_zeros_nx1_real_T(len);
    //Call function
    eegcfilt2(X,Y);
    double r_array[len];
    for (int i = 0; i < len; ++i) {
        r_array[i] = Y->data[i];
    }
    jdoubleArray m_result;
    m_result = env->NewDoubleArray(len);
    env->SetDoubleArrayRegion(m_result, 0, len, &r_array[0]);
    return m_result;
}
}
 */

static void argInit_1000x1_real_T(double result[1000]);
static double argInit_real_T();
static void main_EOGClassifier();

// Function Definitions

//
// Arguments    : double result[1000]
// Return Type  : void
//
static void argInit_1000x1_real_T(double result[1000])
{
    int idx0;

    // Loop over the array to initialize each element.
    for (idx0 = 0; idx0 < 1000; idx0++) {
        // Set the value of the array element.
        // Change this value to the value that the application requires.
        result[idx0] = argInit_real_T();
    }
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
    return 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_EOGClassifier()
{
    double dv4[1000];
    double dv5[1000];
    double Y;

    // Initialize function 'EOGClassifier' input arguments.
    // Initialize function input argument 'ch1'.
    // Initialize function input argument 'ch2'.
    // Call the entry-point 'EOGClassifier'.
    argInit_1000x1_real_T(dv4);
    argInit_1000x1_real_T(dv5);
    Y = EOGClassifier(dv4, dv5);
    LOGE("EOGCLASSIFIER Y: %f",Y);
}

extern "C" {
    JNIEXPORT jdouble JNICALL
    Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jeogclassifier(
            JNIEnv *env, jobject jobject1, jdoubleArray array1, jdoubleArray array2) {
        jdouble  *c_array_ch1 = env->GetDoubleArrayElements(array1, NULL);
        jdouble  *c_array_ch2 = env->GetDoubleArrayElements(array2, NULL);
        if (c_array_ch1==NULL) LOGE("ERROR - C_ARRAY IS NULL");
        if (c_array_ch2==NULL) LOGE("ERROR - C_ARRAY IS NULL");
        return EOGClassifier(c_array_ch1, c_array_ch2);
    }
}

extern "C" {
    JNIEXPORT jdouble JNICALL
    Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jeogclassifier2(
            JNIEnv *env, jobject jobject1, jdoubleArray array1, jdoubleArray array2) {
        jdouble  *c_array_ch1 = env->GetDoubleArrayElements(array1, NULL);
        jdouble  *c_array_ch2 = env->GetDoubleArrayElements(array2, NULL);
        if (c_array_ch1==NULL) LOGE("ERROR - C_ARRAY IS NULL");
        if (c_array_ch2==NULL) LOGE("ERROR - C_ARRAY IS NULL");
        return EOGClassifier2(c_array_ch1,c_array_ch2);
    }
}


extern "C" {
JNIEXPORT jdoubleArray JNICALL
//Call this function with (data, data, data, data, datalen, Fs);
//Don't need array size; can check size array in C.
Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jeogcfilt(
        JNIEnv *env, jobject jobject1, jdoubleArray array) {
    int len = 0;
    jdouble  *c_array = env->GetDoubleArrayElements(array, NULL);
    if (c_array!=NULL) {
        len = env->GetArrayLength(array);
    } else {
        LOGE("ERROR - C_ARRAY IS NULL");
    }
    //Call function
    double r_array[1000];
    eogcfilt_a(c_array,r_array);
    jdoubleArray m_result;
    m_result = env->NewDoubleArray(len);
    env->SetDoubleArrayRegion(m_result, 0, len, &r_array[0]);
    return m_result;
}
}

extern "C" {
JNIEXPORT jint JNICALL
Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jmainInitialization(
        JNIEnv *env, jobject obj, jboolean terminate) {
    if(!(bool)terminate) {
        eogcfilt_a_initialize();
        EOGClassifier_initialize();
        EOGClassifier2_initialize();
        main_EOGClassifier();
        return 0;
    } else {
//        fullHybridClassifier_terminate();
        return -1;
    }
}
}
