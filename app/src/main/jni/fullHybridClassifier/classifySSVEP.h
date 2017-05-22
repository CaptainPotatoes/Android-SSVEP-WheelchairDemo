//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: classifySSVEP.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 22-May-2017 11:55:35
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
#include "omp.h"
#include "classifySSVEP_types.h"

// Variable Declarations
//extern omp_nest_lock_t emlrtNestLockGlobal;

// Function Declarations
extern double classifySSVEP(const double X[1000]);
extern void classifySSVEP_initialize();
extern void classifySSVEP_terminate();

#endif

//
// File trailer for classifySSVEP.h
//
// [EOF]
//
