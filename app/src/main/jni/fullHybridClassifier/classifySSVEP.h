//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: classifySSVEP.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 21-May-2017 20:05:43
//
#ifndef CLASSIFYSSVEP_H
#define CLASSIFYSSVEP_H

// Include Files
#include <cmath>
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "classifySSVEP_types.h"

// Variable Declarations

// Function Declarations
extern void classifySSVEP(const emxArray_real_T2 *X, double start, double Fs,
  double FS[520], double *CLASS);
extern void classifySSVEP_initialize();
extern void classifySSVEP_terminate();
extern emxArray_real_T2 *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real_T2 *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real_T2 *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T2 *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T2 *emxArray);
extern void emxInitArray_real_T(emxArray_real_T2 **pEmxArray, int numDimensions);

#endif

//
// File trailer for classifySSVEP.h
//
// [EOF]
//
