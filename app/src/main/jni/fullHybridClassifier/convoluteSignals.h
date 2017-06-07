//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: convoluteSignals.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 07-Jun-2017 11:36:06
//
#ifndef CONVOLUTESIGNALS_H
#define CONVOLUTESIGNALS_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "convoluteSignals_types.h"

// Function Declarations
extern void convoluteSignals(const double X1[1000], const double X2[1000],
  double Y[1998]);
extern void convoluteSignals_initialize();
extern void convoluteSignals_terminate();

#endif

//
// File trailer for convoluteSignals.h
//
// [EOF]
//
