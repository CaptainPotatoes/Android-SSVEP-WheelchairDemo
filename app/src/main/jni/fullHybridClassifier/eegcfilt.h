//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: eegcfilt.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Apr-2017 14:33:13
//
#ifndef EEGCFILT_H
#define EEGCFILT_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"

typedef struct emxArray_real_T
{
    double *data;
    int *size;
    int allocatedSize;
    int numDimensions;
    boolean_T canFreeData;
} emxArray_real_T;

// Function Declarations
extern void eegcfilt2(const emxArray_real_T *X, emxArray_real_T *Y);
extern void eegcfilt_initialize();
extern void eegcfilt_terminate();
extern emxArray_real_T *emxCreateND_real_T2(int numDimensions, int *size);
extern void emxDestroyArray_real_T2(emxArray_real_T *emxArray);
extern void emxInitArray_real_T2(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for eegcfilt.h
//
// [EOF]
//
