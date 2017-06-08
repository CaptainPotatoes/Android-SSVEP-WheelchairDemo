//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: classifySSVEP4.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 08-Jun-2017 13:36:57
//
#ifndef CLASSIFYSSVEP4_H
#define CLASSIFYSSVEP4_H

// Include Files
#include <cmath>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "omp.h"
#include "classifySSVEP4_types.h"

// Variable Declarations
//extern omp_nest_lock_t emlrtNestLockGlobal;

// Function Declarations
extern double classifySSVEP4(const double X1[1000], const double X2[1000],
  double thresholdFraction);
extern void classifySSVEP4_initialize();
extern void classifySSVEP4_terminate();

#endif

//
// File trailer for classifySSVEP4.h
//
// [EOF]
//
