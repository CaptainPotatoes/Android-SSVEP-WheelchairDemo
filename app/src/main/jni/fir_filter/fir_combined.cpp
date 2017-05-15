//
// File: fir_combined.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 06-Oct-2016 23:23:58
//

// Include Files
#include "rt_nonfinite.h"
#include "fir_combined.h"
/*Additional Includes*/
/*

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "fir-filter-250Hz-cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ECG_ARRAY_SIZE 1000
*//** JNI STUFF:
 *//*
extern "C" {
JNIEXPORT jdoubleArray JNICALL
Java_com_mahmoodms_bluetooth_ecgfallsensordemo_DeviceControlActivity_jFirFilter(
        JNIEnv *env, jobject jobject1, jdoubleArray array) {
  jdouble  *c_array;
  c_array = env->GetDoubleArrayElements(array, NULL);
  if (c_array==NULL) LOGE("ERROR - C_ARRAY IS NULL");
  double r_array[ECG_ARRAY_SIZE];
//  ecg_h_fcn(c_array, r_array);
  fir_combined(c_array, r_array);
  double *result = r_array;
  jdoubleArray m_result;
  m_result = env->NewDoubleArray(ECG_ARRAY_SIZE);
  env->SetDoubleArrayRegion(m_result, 0, ECG_ARRAY_SIZE, result);
  return m_result;
}
}*/
// Function Declarations
static void filter(const double b[17], const double x[1096], const double zi[16],
                   double y[1096]);
static void filtfilt(const double x_in[1000], double y_out[1000]);
static void flipud(double x[1096]);

// Function Definitions

//
// Arguments    : const double b[17]
//                const double x[1096]
//                const double zi[16]
//                double y[1096]
// Return Type  : void
//
static void filter(const double b[17], const double x[1096], const double zi[16],
                   double y[1096])
{
  double dbuffer[17];
  int j;
  int k;
  memcpy(&dbuffer[1], &zi[0], sizeof(double) << 4);
  for (j = 0; j < 1096; j++) {
    for (k = 0; k < 16; k++) {
      dbuffer[k] = dbuffer[k + 1];
    }

    dbuffer[16] = 0.0;
    for (k = 0; k < 17; k++) {
      dbuffer[k] += x[j] * b[k];
    }

    y[j] = dbuffer[0];
  }
}

//
// Arguments    : const double x_in[1000]
//                double y_out[1000]
// Return Type  : void
//
static void filtfilt(const double x_in[1000], double y_out[1000])
{
  double d2;
  double d3;
  int i;
  double y[1096];
  double a[16];
  static const double b_a[16] = { 1.0024076962513326, 1.0076489466189831,
    1.0168062722674225, 1.0225850244084509, 1.00472781852257,
    0.93662600451180233, 0.80166120164082333, 0.60830766946304837,
    0.3916923305369524, 0.19833879835917742, 0.063373995488198415,
    -0.0047278185225693888, -0.02258502440845029, -0.016806272267421891,
    -0.0076489466189825009, -0.00240769625133207 };

  double b_y[1096];
  static const double dv1[17] = { -0.00240769625133207, -0.00524125036765043,
    -0.00915732564843939, -0.0057787521410284, 0.0178572058858809,
    0.0681018140107678, 0.134964802870979, 0.193353532177775, 0.216615338926096,
    0.193353532177775, 0.134964802870979, 0.0681018140107678, 0.0178572058858809,
    -0.0057787521410284, -0.00915732564843939, -0.00524125036765043,
    -0.00240769625133207 };

  double c_y[1096];
  d2 = 2.0 * x_in[0];
  d3 = 2.0 * x_in[999];
  for (i = 0; i < 48; i++) {
    y[i] = d2 - x_in[48 - i];
  }

  memcpy(&y[48], &x_in[0], 1000U * sizeof(double));
  for (i = 0; i < 48; i++) {
    y[i + 1048] = d3 - x_in[998 - i];
  }

  for (i = 0; i < 16; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1096U * sizeof(double));
  filter(dv1, b_y, a, y);
  flipud(y);
  for (i = 0; i < 16; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&c_y[0], &y[0], 1096U * sizeof(double));
  filter(dv1, c_y, a, y);
  flipud(y);
  memcpy(&y_out[0], &y[48], 1000U * sizeof(double));
}

//
// Arguments    : double x[1096]
// Return Type  : void
//
static void flipud(double x[1096])
{
  int i;
  double xtmp;
  for (i = 0; i < 548; i++) {
    xtmp = x[i];
    x[i] = x[1095 - i];
    x[1095 - i] = xtmp;
  }
}

//
// fir_combined FIR combined filter
//    Just pass the ECG, and out comes the signal.
// % Low Pass Filt, fc = 40, N_order = 16, fs = 250Hz.
//  b = [0.00311301067710570,0.00356453967202243,-0.00282061286925786,-0.0219401093529242,-0.0329652200237208,0.00948027569642495,0.124059174875593,0.258210341588488,0.318597199472538,0.258210341588488,0.124059174875593,0.00948027569642495,-0.0329652200237208,-0.0219401093529242,-0.00282061286925786,0.00356453967202243,0.00311301067710570];
//  for fc = 27
// Arguments    : const double ecg_in[1000]
//                double ecg_out[1000]
// Return Type  : void
//
void fir_combined(const double ecg_in[1000], double ecg_out[1000])
{
  double ecg_o[1000];
  double d0;
  double d1;
  int i;
  double y[1096];
  double a[16];
  static const double b_a[16] = { 0.66534426305825178, 0.66938477655070316,
    0.6772036653506287, 0.690859161613159, 0.711615810885629,
    0.73965760829042693, 0.77399582816039658, 0.81259562895504622,
    -0.14994606077276368, -0.11134625997811409, -0.0770080401081445,
    -0.048966242703346596, -0.0282095934308766, -0.0145540971683463,
    -0.00673520836842076, -0.00269469487596934 };

  double b_y[1096];
  static const double dv0[17] = { -0.00269469487596934, -0.00404051349245142,
    -0.00781888879992554, -0.0136554962625303, -0.02075664927247,
    -0.0280417974047979, -0.0343382198699696, -0.0385998007946496,
    0.96254168972781, -0.0385998007946496, -0.0343382198699696,
    -0.0280417974047979, -0.02075664927247, -0.0136554962625303,
    -0.00781888879992554, -0.00404051349245142, -0.00269469487596934 };

  double c_y[1096];
  filtfilt(ecg_in, ecg_o);

  // % High Pass Filter, fc = 5; N_order=16, fs=250Hz
  d0 = 2.0 * ecg_o[0];
  d1 = 2.0 * ecg_o[999];
  for (i = 0; i < 48; i++) {
    y[i] = d0 - ecg_o[48 - i];
  }

  memcpy(&y[48], &ecg_o[0], 1000U * sizeof(double));
  for (i = 0; i < 48; i++) {
    y[i + 1048] = d1 - ecg_o[998 - i];
  }

  for (i = 0; i < 16; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1096U * sizeof(double));
  filter(dv0, b_y, a, y);
  flipud(y);
  for (i = 0; i < 16; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&c_y[0], &y[0], 1096U * sizeof(double));
  filter(dv0, c_y, a, y);
  flipud(y);
  memcpy(&ecg_out[0], &y[48], 1000U * sizeof(double));
}

//
// Arguments    : void
// Return Type  : void
//
void fir_combined_initialize()
{
  rt_InitInfAndNaN_fir(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void fir_combined_terminate()
{
  // (no terminate code required)
}

//
// File trailer for fir_combined.cpp
//
// [EOF]
//
