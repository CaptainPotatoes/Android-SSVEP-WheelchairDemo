//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: eegcfilt.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Apr-2017 14:33:13
//

// Include Files
#include "rt_nonfinite.h"
#include "eegcfilt.h"
/*Additional Includes*/
#include <jni.h>
#include <android/log.h>
#define  LOG_TAG "eegcfilt-cpp"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static emxArray_real_T *argInit_Array_real_T(int size, jdouble *array) {
  emxArray_real_T *result;
  result = emxCreateND_real_T2(1, &size);
  result->size[0U] = size;
  for (int i=0; i<size; i++) {
    result->data[i] = array[i];
  }
  return result;
}


// Type Definitions
#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
  void *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray__common

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_int32_T

// Function Declarations
static void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
static void emxFree_int32_T(emxArray_int32_T **pEmxArray);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
static void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
static void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions);
static void filter(const emxArray_real_T *x, const double zi[10],
                   emxArray_real_T *y);

// Function Definitions

//
// Arguments    : emxArray__common *emxArray
//                int oldNumel
//                int elementSize
// Return Type  : void
//
static void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize)
{
  int newNumel;
  int i;
  void *newData;
  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }

    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i <<= 1;
      }
    }

    newData = calloc((unsigned int)i, (unsigned int)elementSize);
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, (unsigned int)(elementSize * oldNumel));
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }

    emxArray->data = newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

//
// Arguments    : emxArray_int32_T **pEmxArray
// Return Type  : void
//
static void emxFree_int32_T(emxArray_int32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_int32_T *)NULL) {
    if (((*pEmxArray)->data != (int *)NULL) && (*pEmxArray)->canFreeData) {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_int32_T *)NULL;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
// Return Type  : void
//
static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (double *)NULL) && (*pEmxArray)->canFreeData) {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

//
// Arguments    : emxArray_int32_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions)
{
  emxArray_int32_T *emxArray;
  int i;
  *pEmxArray = (emxArray_int32_T *)malloc(sizeof(emxArray_int32_T));
  emxArray = *pEmxArray;
  emxArray->data = (int *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxArray_real_T *emxArray;
  int i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (double *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxArray_real_T *emxArray;
  int i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (double *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}


static void filter(const emxArray_real_T *x, const double zi[10],
                   emxArray_real_T *y)
{
  unsigned int unnamed_idx_0;
  int j;
  double dbuffer[11];
  int k;
  double b_dbuffer;
  static const double dv0[11] = { 2.13961520749732E-5, 0.0,
                                  -0.000106980760374866, 0.0, 0.000213961520749732, 0.0, -0.000213961520749732,
                                  0.0, 0.000106980760374866, 0.0, -2.13961520749732E-5 };

  static const double dv1[11] = { 1.0, -8.77043379286888, 35.0068378010024,
                                  -83.7229808056309, 132.845833785487, -146.117834417428, 112.823239428442,
                                  -60.389449129414, 21.4471017127118, -4.56451967201817, 0.442209182399621 };

  unnamed_idx_0 = (unsigned int)x->size[0];
  j = y->size[0];
  y->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)y, j, (int)sizeof(double));
  memcpy(&dbuffer[1], &zi[0], 10U * sizeof(double));
  for (j = 0; j + 1 <= x->size[0]; j++) {
    for (k = 0; k < 10; k++) {
      dbuffer[k] = dbuffer[k + 1];
    }

    dbuffer[10] = 0.0;
    for (k = 0; k < 11; k++) {
      b_dbuffer = dbuffer[k] + x->data[j] * dv0[k];
      dbuffer[k] = b_dbuffer;
    }

    for (k = 0; k < 10; k++) {
      dbuffer[k + 1] -= dbuffer[0] * dv1[k + 1];
    }

    y->data[j] = dbuffer[0];
  }
}

//
// EOGCFILT EEG filter for conversion to C.
//  Vectorize:
// Arguments    : const emxArray_real_T *X
//                emxArray_real_T *Y
// Return Type  : void
//
void eegcfilt2(const emxArray_real_T *X, emxArray_real_T *Y)
{
  int m;
  emxArray_real_T *x;
  int i;
  int md2;
  emxArray_real_T *y;
  double xtmp;
  double d0;
  double a[10];
  emxArray_real_T *b_y;
  static const double b_a[10] = { -2.1396152021655335E-5, -2.1396152489276133E-5,
                                  8.558460975207999E-5, 8.5584605288149449E-5, -0.00012837690837852629,
                                  -0.00012837691616921775, 8.5584610596008311E-5, 8.5584607376171939E-5,
                                  -2.1396151855180404E-5, -2.1396152098550849E-5 };

  emxArray_real_T *c_y;
  emxArray_int32_T *r0;

  //  Fs = 250, N = 5
  //  flim = [8 18], bandpass
  m = X->size[0];
  emxInit_real_T(&x, 1);
  if (m == 1) {
    m = X->size[0];
    i = x->size[0];
    x->size[0] = m;
    emxEnsureCapacity((emxArray__common *)x, i, (int)sizeof(double));
    for (i = 0; i < m; i++) {
      x->data[i] = X->data[i];
    }
  } else {
    i = x->size[0];
    x->size[0] = X->size[0];
    emxEnsureCapacity((emxArray__common *)x, i, (int)sizeof(double));
    md2 = X->size[0];
    for (i = 0; i < md2; i++) {
      x->data[i] = X->data[i];
    }
  }

  if (x->size[0] == 0) {
    i = Y->size[0] * Y->size[1];
    Y->size[0] = 0;
    Y->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)Y, i, (int)sizeof(double));
  } else {
    emxInit_real_T(&y, 1);
    xtmp = 2.0 * x->data[0];
    d0 = 2.0 * x->data[x->size[0] - 1];
    m = x->size[0] - 2;
    i = y->size[0];
    y->size[0] = 60 + x->size[0];
    emxEnsureCapacity((emxArray__common *)y, i, (int)sizeof(double));
    for (i = 0; i < 30; i++) {
      y->data[i] = xtmp - x->data[30 - i];
    }

    md2 = x->size[0];
    for (i = 0; i < md2; i++) {
      y->data[i + 30] = x->data[i];
    }

    for (i = 0; i < 30; i++) {
      y->data[(i + x->size[0]) + 30] = d0 - x->data[m - i];
    }

    xtmp = y->data[0];
    for (i = 0; i < 10; i++) {
      a[i] = b_a[i] * xtmp;
    }

    emxInit_real_T(&b_y, 1);
    i = b_y->size[0];
    b_y->size[0] = y->size[0];
    emxEnsureCapacity((emxArray__common *)b_y, i, (int)sizeof(double));
    md2 = y->size[0];
    for (i = 0; i < md2; i++) {
      b_y->data[i] = y->data[i];
    }

    filter(b_y, a, y);
    m = y->size[0];
    md2 = y->size[0] >> 1;
    i = 1;
    emxFree_real_T(&b_y);
    while (i <= md2) {
      xtmp = y->data[i - 1];
      y->data[i - 1] = y->data[m - i];
      y->data[m - i] = xtmp;
      i++;
    }

    xtmp = y->data[0];
    for (i = 0; i < 10; i++) {
      a[i] = b_a[i] * xtmp;
    }

    emxInit_real_T(&c_y, 1);
    i = c_y->size[0];
    c_y->size[0] = y->size[0];
    emxEnsureCapacity((emxArray__common *)c_y, i, (int)sizeof(double));
    md2 = y->size[0];
    for (i = 0; i < md2; i++) {
      c_y->data[i] = y->data[i];
    }

    filter(c_y, a, y);
    m = y->size[0];
    md2 = y->size[0] >> 1;
    i = 1;
    emxFree_real_T(&c_y);
    while (i <= md2) {
      xtmp = y->data[i - 1];
      y->data[i - 1] = y->data[m - i];
      y->data[m - i] = xtmp;
      i++;
    }

    m = X->size[0];
    if (m == 1) {
      i = x->size[0] + 30;
      md2 = i - 31;
      m = Y->size[0] * Y->size[1];
      Y->size[0] = 1;
      Y->size[1] = i - 30;
      emxEnsureCapacity((emxArray__common *)Y, m, (int)sizeof(double));
      for (i = 0; i <= md2; i++) {
        Y->data[Y->size[0] * i] = y->data[30 + i];
      }
    } else {
      emxInit_int32_T(&r0, 1);
      md2 = x->size[0];
      i = Y->size[0] * Y->size[1];
      Y->size[0] = md2;
      Y->size[1] = 1;
      emxEnsureCapacity((emxArray__common *)Y, i, (int)sizeof(double));
      i = r0->size[0];
      r0->size[0] = md2;
      emxEnsureCapacity((emxArray__common *)r0, i, (int)sizeof(int));
      for (i = 0; i < md2; i++) {
        r0->data[i] = 31 + i;
      }

      for (i = 0; i < md2; i++) {
        Y->data[i] = y->data[r0->data[i] - 1];
      }

      emxFree_int32_T(&r0);
    }

    emxFree_real_T(&y);
  }

  emxFree_real_T(&x);
}

void eegcfilt_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void eegcfilt_terminate()
{
  // (no terminate code required)
}

//
// Arguments    : int numDimensions
//                int *size
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreateND_real_T2(int numDimensions, int *size)
{
  emxArray_real_T *emx;
  int numEl;
  int i;
  emxInit_real_T1(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (double *)calloc((unsigned int)numEl, sizeof(double));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : emxArray_real_T *emxArray
// Return Type  : void
//
void emxDestroyArray_real_T2(emxArray_real_T *emxArray)
{
  emxFree_real_T(&emxArray);
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInitArray_real_T2(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxInit_real_T1(pEmxArray, numDimensions);
}

//
// File trailer for eegcfilt.cpp
//
// [EOF]
//

