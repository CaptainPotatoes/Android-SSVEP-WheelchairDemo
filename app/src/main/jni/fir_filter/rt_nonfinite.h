/*
 * File: rt_nonfinite.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 06-Oct-2016 23:23:58
 */

#ifndef __RT_NONFINITE_H__
#define __RT_NONFINITE_H__
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#include <float.h>
#endif

#include <stddef.h>
#include "rtwtypes.h"

extern real_T rtInf_fir;
extern real_T rtMinusInf_fir;
extern real_T rtNaN_fir;
extern real32_T rtInfF_fir;
extern real32_T rtMinusInfF_fir;
extern real32_T rtNaNF_fir;
extern void rt_InitInfAndNaN_fir(size_t realSize);
extern boolean_T rtIsInf_fir(real_T value);
extern boolean_T rtIsInfF_fir(real32_T value);
extern boolean_T rtIsNaN_fir(real_T value);
extern boolean_T rtIsNaNF_fir(real32_T value);
typedef struct {
  struct {
    uint32_T wordH;
    uint32_T wordL;
  } words;
} BigEndianIEEEDouble;

typedef struct {
  struct {
    uint32_T wordL;
    uint32_T wordH;
  } words;
} LittleEndianIEEEDouble;

typedef struct {
  union {
    real32_T wordLreal;
    uint32_T wordLuint;
  } wordL;
} IEEESingle;

#endif

/*
 * File trailer for rt_nonfinite.h
 *
 * [EOF]
 */
