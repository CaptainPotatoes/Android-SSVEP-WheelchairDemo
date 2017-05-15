/*
 * File: rt_nonfinite.cpp
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 06-Oct-2016 23:23:58
 */

/*
 * Abstract:
 *      MATLAB for code generation function to initialize non-finites,
 *      (Inf, NaN and -Inf).
 */
#include "rt_nonfinite.h"
#include "rtGetNaN.h"
#include "rtGetInf.h"

real_T rtInf_fir;
real_T rtMinusInf_fir;
real_T rtNaN_fir;
real32_T rtInfF_fir;
real32_T rtMinusInfF_fir;
real32_T rtNaNF_fir;

/* Function: rt_InitInfAndNaN ==================================================
 * Abstract:
 * Initialize the rtInf, rtMinusInf, and rtNaN needed by the
 * generated code. NaN is initialized as non-signaling. Assumes IEEE.
 */
void rt_InitInfAndNaN_fir(size_t realSize)
{
  (void) (realSize);
  rtNaN_fir = rtGetNaN_fir();
  rtNaNF_fir = rtGetNaNF_fir();
  rtInf_fir = rtGetInf_fir();
  rtInfF_fir = rtGetInfF_fir();
  rtMinusInf_fir = rtGetMinusInf_fir();
  rtMinusInfF_fir = rtGetMinusInfF_fir();
}

/* Function: rtIsInf ==================================================
 * Abstract:
 * Test if value is infinite
 */
boolean_T rtIsInf_fir(real_T value)
{
  return ((value==rtInf_fir || value==rtMinusInf_fir) ? 1U : 0U);
}

/* Function: rtIsInfF =================================================
 * Abstract:
 * Test if single-precision value is infinite
 */
boolean_T rtIsInfF_fir(real32_T value)
{
  return(((value)==rtInfF_fir || (value)==rtMinusInfF_fir) ? 1U : 0U);
}

/* Function: rtIsNaN ==================================================
 * Abstract:
 * Test if value is not a number
 */
boolean_T rtIsNaN_fir(real_T value)
{

#if defined(_MSC_VER) && (_MSC_VER <= 1200)

  return _isnan(value)? TRUE:FALSE;

#else

  return (value!=value)? 1U:0U;

#endif

}

/* Function: rtIsNaNF =================================================
 * Abstract:
 * Test if single-precision value is not a number
 */
boolean_T rtIsNaNF_fir(real32_T value)
{

#if defined(_MSC_VER) && (_MSC_VER <= 1200)

  return _isnan((real_T)value)? true:false;

#else

  return (value!=value)? 1U:0U;

#endif

}

/*
 * File trailer for rt_nonfinite.cpp
 *
 * [EOF]
 */
