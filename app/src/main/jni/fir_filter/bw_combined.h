//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: bw_combined.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 15-Jan-2017 17:28:34
//
#ifndef BW_COMBINED_H
#define BW_COMBINED_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "bw_combined_types.h"

// Function Declarations
extern void bw_combined(const double ecg_in[1000], double ecg_out[1000]);
extern void bw_combined_initialize();
extern void bw_combined_terminate();

#endif

//
// File trailer for bw_combined.h
//
// [EOF]
//
