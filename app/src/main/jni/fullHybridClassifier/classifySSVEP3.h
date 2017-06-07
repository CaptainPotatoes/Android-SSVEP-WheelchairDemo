//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: classifySSVEP3.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 07-Jun-2017 12:50:12
//
#ifndef CLASSIFYSSVEP3_H
#define CLASSIFYSSVEP3_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "omp.h"
#include "classifySSVEP3_types.h"

// Variable Declarations

// Function Declarations
extern double classifySSVEP3(const double X1[1000], const double X2[1000],
  double thresholdFraction);
extern void classifySSVEP3_initialize();
extern void classifySSVEP3_terminate();

#endif

//
// File trailer for classifySSVEP3.h
//
// [EOF]
//
