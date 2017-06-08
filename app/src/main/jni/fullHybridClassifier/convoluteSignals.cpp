//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: convoluteSignals.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 07-Jun-2017 11:36:06
//

// Include Files
#include "rt_nonfinite.h"
#include "convoluteSignals.h"

// Function Declarations
static void conv(const double A[1000], const double B[1000], double C[1999]);
static void filter(double b[7], double a[7], const double x[1036], const double
                   zi[6], double y[1036]);
static void flipud(double x[1036]);
static void ssvepcfilt2(const double X[1000], double Y[1000]);

// Function Definitions

//
// Arguments    : const double A[1000]
//                const double B[1000]
//                double C[1999]
// Return Type  : void
//
static void conv(const double A[1000], const double B[1000], double C[1999]) {
  int jC;
  int jA2;
  double s;
  int k;
  for (jC = 0; jC < 1999; jC++) {
    if (1000 < jC + 1) {
      jA2 = 999;
    } else {
      jA2 = jC;
    }

    s = 0.0;
    if (1000 < jC + 2) {
      k = jC - 999;
    } else {
      k = 0;
    }

    while (k + 1 <= jA2 + 1) {
      s += A[k] * B[jC - k];
      k++;
    }

    C[jC] = s;
  }
}

//
// Arguments    : double b[7]
//                double a[7]
//                const double x[1036]
//                const double zi[6]
//                double y[1036]
// Return Type  : void
//
static void filter(double b[7], double a[7], const double x[1036], const double
                   zi[6], double y[1036])
{
  double a1;
  int k;
  double dbuffer[7];
  int j;
  a1 = a[0];
  if ((!((!rtIsInf(a[0])) && (!rtIsNaN(a[0])))) || (a[0] == 0.0) || ((a[0] == 1.0))) {
  } else {
    for (k = 0; k < 7; k++) {
      b[k] /= a1;
    }

    for (k = 0; k < 6; k++) {
      a[k + 1] /= a1;
    }

    a[0] = 1.0;
  }

  for (k = 0; k < 6; k++) {
    dbuffer[k + 1] = zi[k];
  }

  for (j = 0; j < 1036; j++) {
    for (k = 0; k < 6; k++) {
      dbuffer[k] = dbuffer[k + 1];
    }

    dbuffer[6] = 0.0;
    for (k = 0; k < 7; k++) {
      dbuffer[k] += x[j] * b[k];
    }

    for (k = 0; k < 6; k++) {
      dbuffer[k + 1] -= dbuffer[0] * a[k + 1];
    }

    y[j] = dbuffer[0];
  }
}

//
// Arguments    : double x[1036]
// Return Type  : void
//
static void flipud(double x[1036])
{
  int i;
  double xtmp;
  for (i = 0; i < 518; i++) {
    xtmp = x[i];
    x[i] = x[1035 - i];
    x[1035 - i] = xtmp;
  }
}

//
// [5 40] bandpass butterworth N=3
// Arguments    : const double X[1000]
//                double Y[1000]
// Return Type  : void
//
static void ssvepcfilt2(const double X[1000], double Y[1000])
{
  double d0;
  double d1;
  int i;
  double y[1036];
  double dv0[7];
  double dv1[7];
  double a[6];
  static const double dv2[7] = { 0.0418768282347742, 0.0, -0.125630484704323,
    0.0, 0.125630484704323, 0.0, -0.0418768282347742 };

  static const double dv3[7] = { 1.0, -3.99412602172993, 6.79713743558926,
    -6.44840721730666, 3.65712515526032, -1.17053739881085, 0.159769122451512 };

  double b_y[1036];
  static const double b_a[6] = { -0.041876828234757295, -0.041876828234824783,
    0.083753656469613066, 0.0837536564695041, -0.041876828234757114,
    -0.04187682823477689 };

  double c_y[1036];
  d0 = 2.0 * X[0];
  d1 = 2.0 * X[999];
  for (i = 0; i < 18; i++) {
    y[i] = d0 - X[18 - i];
  }

  memcpy(&y[18], &X[0], 1000U * sizeof(double));
  for (i = 0; i < 18; i++) {
    y[i + 1018] = d1 - X[998 - i];
  }

  for (i = 0; i < 7; i++) {
    dv0[i] = dv2[i];
    dv1[i] = dv3[i];
  }

  for (i = 0; i < 6; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1036U * sizeof(double));
  filter(dv0, dv1, b_y, a, y);
  flipud(y);
  for (i = 0; i < 7; i++) {
    dv0[i] = dv2[i];
    dv1[i] = dv3[i];
  }

  for (i = 0; i < 6; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&c_y[0], &y[0], 1036U * sizeof(double));
  filter(dv0, dv1, c_y, a, y);
  flipud(y);
  memcpy(&Y[0], &y[18], 1000U * sizeof(double));
}

//
// filters and convolutes 2 data channels.
// Arguments    : const double X1[1000]
//                const double X2[1000]
//                double Y[1998]
// Return Type  : void
//
void convoluteSignals(const double X1[1000], const double X2[1000], double Y
                      [1998])
{
  double fch[1000];
  double fch2[1000];
  double conv2ch[1999];
  ssvepcfilt2(X1, fch);

  // [5 40]
  ssvepcfilt2(X2, fch2);
  conv(fch, fch2, conv2ch);

  //  if length is odd:
  memcpy(&Y[0], &conv2ch[0], 1998U * sizeof(double));
}

//
// Arguments    : void
// Return Type  : void
//
void convoluteSignals_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void convoluteSignals_terminate()
{
  // (no terminate code required)
}

//
// File trailer for convoluteSignals.cpp
//
// [EOF]
//
