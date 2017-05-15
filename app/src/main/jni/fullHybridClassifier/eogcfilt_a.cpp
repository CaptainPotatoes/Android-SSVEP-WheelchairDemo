//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: eogcfilt_a.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 19-Apr-2017 20:15:53
//

// Include Files
#include "rt_nonfinite.h"
#include "eogcfilt_a.h"

// Function Declarations
static void filter(double b[7], double a[7], const double x[1036], const double
                   zi[6], double y[1036]);
static void flipud(double x[1036]);

// Function Definitions

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
  if ((!((!rtIsInf(a[0])) && (!rtIsNaN(a[0])))) || (a[0] == 0.0) || (a[0] == 1.0)) {
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
// Arguments    : const double X[1000]
//                double Y[1000]
// Return Type  : void
//
void eogcfilt_a(const double X[1000], double Y[1000])
{
  double d0;
  double d1;
  int i;
  double y[1036];
  double dv0[7];
  double dv1[7];
  double a[6];
  static const double dv2[7] = { 0.00152384286569762, 0.0, -0.00457152859709287,
    0.0, 0.00457152859709287, 0.0, -0.00152384286569762 };

  static const double dv3[7] = { 1.0, -5.50185125734413, 12.6273320649156,
    -15.4780040230721, 10.6885954819194, -3.94325070984852, 0.607178443629244 };

  double b_y[1036];
  static const double b_a[6] = { -0.0015238424883667707, -0.0015238445643849792,
    0.003047688797389825, 0.0030476829570614192, -0.0015238416068946374,
    -0.0015238430948047777 };

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
// Arguments    : void
// Return Type  : void
//
void eogcfilt_a_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// File trailer for eogcfilt_a.cpp
//
// [EOF]
//
