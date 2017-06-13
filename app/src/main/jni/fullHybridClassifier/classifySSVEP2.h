//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: classifySSVEP2.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 13-Jun-2017 13:52:12
//
#ifndef CLASSIFYSSVEP2_H
#define CLASSIFYSSVEP2_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "omp.h"
#include "classifySSVEP2_types.h"

// Function Declarations
extern void classifySSVEP2(const double X1[1000], const double X2[1000], double
  thresholdFraction, double *Y, double *CLASS0);
extern void classifySSVEP2_initialize();
extern void classifySSVEP2_terminate();

#endif

//
// File trailer for classifySSVEP2.h
//
// [EOF]
//
