//
// File: main.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 06-Oct-2016 23:23:58
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "fir_combined.h"
#include "bw_combined.h"
#include "fir_main.h"
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "ecg-main-cpp"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
// Function Declarations
static void argInit_1000x1_real_T(double result[1000]);
static double argInit_real_T();
static void main_fir_combined();

// Function Definitions

//
// Arguments    : double result[1000]
// Return Type  : void
//
static void argInit_1000x1_real_T(double result[1000])
{
  int b_j0;

  // Loop over the array to initialize each element.
  for (b_j0 = 0; b_j0 < 1000; b_j0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[b_j0] = argInit_real_T();
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
static void main_fir_combined()
{
  double dv2[1000];
  double ecg_out[1000];

  // Initialize function 'fir_combined' input arguments.
  // Initialize function input argument 'ecg_in'.
  // Call the entry-point 'fir_combined'.
  argInit_1000x1_real_T(dv2);
  fir_combined(dv2, ecg_out);
  argInit_1000x1_real_T(dv2);
  bw_combined(dv2, ecg_out);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
/*int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  fir_combined_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_fir_combined();

  // Terminate the application.
  // You do not need to do this more than one time.
  fir_combined_terminate();
  return 0;
}*/

extern "C" {
JNIEXPORT jint JNICALL
Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jmainFirFilter(
        JNIEnv *env, jobject obj, jboolean terminate) {
    if(!(bool)terminate) {
      fir_combined_initialize();
      main_fir_combined();
      return 0;
    } else {
      fir_combined_terminate();
      return -1;
    }
}
}

//
// File trailer for main.cpp
//
// [EOF]
//
