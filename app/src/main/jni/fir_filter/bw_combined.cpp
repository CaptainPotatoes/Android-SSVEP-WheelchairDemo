//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: bw_combined.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 15-Jan-2017 17:28:34
//

// Include Files
#include "rt_nonfinite.h"
#include "bw_combined.h"

/*Additional Includes*/
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "bw-filter-250Hz-cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ECG_ARRAY_SIZE 1000

extern "C" {
JNIEXPORT jdoubleArray JNICALL
Java_com_mahmoodms_bluetooth_eegssvepdemo_DeviceControlActivity_jBwFilter(
        JNIEnv *env, jobject jobject1, jdoubleArray array) {
  jdouble  *c_array;
  c_array = env->GetDoubleArrayElements(array, NULL);
  if (c_array==NULL) LOGE("ERROR - C_ARRAY IS NULL");
  double r_array[ECG_ARRAY_SIZE];
  bw_combined(c_array, r_array);
  double *result = r_array;
  jdoubleArray m_result;
  m_result = env->NewDoubleArray(ECG_ARRAY_SIZE);
  env->SetDoubleArrayRegion(m_result, 0, ECG_ARRAY_SIZE, result);
  return m_result;
}
}

// Function Declarations
static void filter(double b[4], double a[4], const double x[1018], const double
                   zi[3], double y[1018]);
static void filtfilt(const double x_in[1000], double y_out[1000]);
static void flipud(double x[1018]);

// Function Definitions

//
// Arguments    : double b[4]
//                double a[4]
//                const double x[1018]
//                const double zi[3]
//                double y[1018]
// Return Type  : void
//
static void filter(double b[4], double a[4], const double x[1018], const double
                   zi[3], double y[1018])
{
  double a1;
  int k;
  double dbuffer[4];
  int j;
  a1 = a[0];
  if ((!((!rtIsInf_fir(a[0])) && (!rtIsNaN_fir(a[0])))) || (a[0] == 0.0) || (!(a[0] !=
        1.0))) {
  } else {
    for (k = 0; k < 4; k++) {
      b[k] /= a1;
    }

    for (k = 0; k < 3; k++) {
      a[k + 1] /= a1;
    }

    a[0] = 1.0;
  }

  for (k = 0; k < 3; k++) {
    dbuffer[k + 1] = zi[k];
  }

  for (j = 0; j < 1018; j++) {
    for (k = 0; k < 3; k++) {
      dbuffer[k] = dbuffer[k + 1];
    }

    dbuffer[3] = 0.0;
    for (k = 0; k < 4; k++) {
      dbuffer[k] += x[j] * b[k];
    }

    for (k = 0; k < 3; k++) {
      dbuffer[k + 1] -= dbuffer[0] * a[k + 1];
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
  double y[1018];
  double dv4[4];
  double dv5[4];
  double a[3];
  static const double dv6[4] = { 0.975179811634754, -2.92553943490426,
    2.92553943490426, -0.975179811634754 };

  static const double dv7[4] = { 1.0, -2.94973583970635, 2.90072698835544,
    -0.950975665016249 };

  double b_y[1018];
  static const double b_a[3] = { -0.97517981165665291, 1.9503596233122031,
    -0.97517981165557921 };

  double c_y[1018];
  d2 = 2.0 * x_in[0];
  d3 = 2.0 * x_in[999];
  for (i = 0; i < 9; i++) {
    y[i] = d2 - x_in[9 - i];
  }

  memcpy(&y[9], &x_in[0], 1000U * sizeof(double));
  for (i = 0; i < 9; i++) {
    y[i + 1009] = d3 - x_in[998 - i];
  }

  for (i = 0; i < 4; i++) {
    dv4[i] = dv6[i];
    dv5[i] = dv7[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv4, dv5, b_y, a, y);
  flipud(y);
  for (i = 0; i < 4; i++) {
    dv4[i] = dv6[i];
    dv5[i] = dv7[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&c_y[0], &y[0], 1018U * sizeof(double));
  filter(dv4, dv5, c_y, a, y);
  flipud(y);
  memcpy(&y_out[0], &y[9], 1000U * sizeof(double));
}

//
// Arguments    : double x[1018]
// Return Type  : void
//
static void flipud(double x[1018])
{
  int i;
  double xtmp;
  for (i = 0; i < 509; i++) {
    xtmp = x[i];
    x[i] = x[1017 - i];
    x[1017 - i] = xtmp;
  }
}

//
// Butterworth Filter:
// {
//  High pass filter: Order 3, 250Hz Sampling Rate, Cutoff 5Hz
// b = [0.881838198574415,-2.64551459572324,2.64551459572324,-0.881838198574415];
// a = [1,-2.74883580921468,2.52823121914256,-0.777638560238081];
// ecg_o = filtfilt(b,a,ecg_in);
// Low pass filter: Order 3, 250Hz Sampling Rate, Cutoff 30Hz
// b_l = [0.0286353001709179,0.0859059005127536,0.0859059005127536,0.0286353001709179];
// a_l = [1,-1.51890863466294,0.960036970357047,-0.212045934326767];
// ecg_out = filtfilt(b_l,a_l,ecg_o);
// }
//  High pass filter: Order 3, 250Hz Sampling Rate, Cutoff 1Hz
// Arguments    : const double ecg_in[1000]
//                double ecg_out[1000]
// Return Type  : void
//
void bw_combined(const double ecg_in[1000], double ecg_out[1000])
{
  double ecg_o[1000];
  double d0;
  double d1;
  int i;
  double y[1018];
  double dv0[4];
  double dv1[4];
  double a[3];
  static const double dv2[4] = { 0.0286353001709179, 0.0859059005127536,
    0.0859059005127536, 0.0286353001709179 };

  static const double dv3[4] = { 1.0, -1.51890863466294, 0.960036970357047,
    -0.212045934326767 };

  double b_y[1018];
  static const double b_a[3] = { 0.97136469982909435, -0.6334498353466177,
    0.24068123449768747 };

  double c_y[1018];
  filtfilt(ecg_in, ecg_o);

  // Low pass filter: Order 3, 250Hz Sampling Rate, Cutoff 40Hz
  d0 = 2.0 * ecg_o[0];
  d1 = 2.0 * ecg_o[999];
  for (i = 0; i < 9; i++) {
    y[i] = d0 - ecg_o[9 - i];
  }

  memcpy(&y[9], &ecg_o[0], 1000U * sizeof(double));
  for (i = 0; i < 9; i++) {
    y[i + 1009] = d1 - ecg_o[998 - i];
  }

  for (i = 0; i < 4; i++) {
    dv0[i] = dv2[i];
    dv1[i] = dv3[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv0, dv1, b_y, a, y);
  flipud(y);
  for (i = 0; i < 4; i++) {
    dv0[i] = dv2[i];
    dv1[i] = dv3[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&c_y[0], &y[0], 1018U * sizeof(double));
  filter(dv0, dv1, c_y, a, y);
  flipud(y);
  memcpy(&ecg_out[0], &y[9], 1000U * sizeof(double));
}

//
// Arguments    : void
// Return Type  : void
//
void bw_combined_initialize()
{
  rt_InitInfAndNaN_fir(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void bw_combined_terminate()
{
  // (no terminate code required)
}

//
// File trailer for bw_combined.cpp
//
// [EOF]
//
