//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: classifySSVEP5.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 10-Jun-2017 00:27:00
//
#ifndef CLASSIFYSSVEP5_H
#define CLASSIFYSSVEP5_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "omp.h"
#include "classifySSVEP5_types.h"

// Function Declarations
extern double classifySSVEP5(const double X1[1000], const double X2[1000],
  double thresholdFraction);
extern void classifySSVEP5_initialize();
extern void classifySSVEP5_terminate();

#endif

//
// File trailer for classifySSVEP5.h
//
// [EOF]
//
