//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: extractPowerSpectrum.h
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 19-Sep-2017 15:55:51
//
#ifndef EXTRACTPOWERSPECTRUM_H
#define EXTRACTPOWERSPECTRUM_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "extractPowerSpectrum_types.h"

// Function Declarations
extern void extractPowerSpectrum(const double X1[500], const double X2[500],
  double PSD[499]);
extern void extractPowerSpectrum_initialize();
extern void extractPowerSpectrum_terminate();

#endif

//
// File trailer for extractPowerSpectrum.h
//
// [EOF]
//
