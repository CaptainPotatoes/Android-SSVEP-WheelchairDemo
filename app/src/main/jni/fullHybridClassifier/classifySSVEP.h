//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: classifySSVEP.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Jun-2017 14:03:52
//
#ifndef CLASSIFYSSVEP_H
#define CLASSIFYSSVEP_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "classifySSVEP_types.h"

// Function Declarations
extern void classifySSVEP(const double X1[1000], const double X2[1000], double
  thresholdFraction, double *Y, double *CLASS0);
extern void classifySSVEP_initialize();
extern void classifySSVEP_terminate();

#endif

//
// File trailer for classifySSVEP.h
//
// [EOF]
//
