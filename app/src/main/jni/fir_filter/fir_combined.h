//
// File: fir_combined.h
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 06-Oct-2016 23:23:58
//
#ifndef __FIR_COMBINED_H__
#define __FIR_COMBINED_H__

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "fir_combined_types.h"

// Function Declarations
extern void fir_combined(const double ecg_in[1000], double ecg_out[1000]);
extern void fir_combined_initialize();
extern void fir_combined_terminate();

#endif

//
// File trailer for fir_combined.h
//
// [EOF]
//
