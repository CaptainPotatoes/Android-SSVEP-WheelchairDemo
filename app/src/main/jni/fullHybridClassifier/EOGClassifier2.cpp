//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: EOGClassifier.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 27-Apr-2017 14:44:04
//

// Include Files
#include "rt_nonfinite.h"
#include "EOGClassifier2.h"

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

#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

struct emxArray_boolean_T
{
  boolean_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_boolean_T

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

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T

// Variable Definitions

// Function Declarations
static void assignOutputs(const double y[250], const double x[250], const
  emxArray_real_T *iPk, emxArray_real_T *YpkOut, emxArray_real_T *XpkOut);
static void b_diff(const emxArray_real_T *x, emxArray_real_T *y);
static void b_merge(int idx[294], double x[294], int offset, int np, int nq, int
                    iwork[294], double xwork[294]);
static void b_merge_block(int idx[294], double x[294], int offset, int n, int
  preSortLevel, int iwork[294], double xwork[294]);
static void b_sort(emxArray_real_T *x, int dim, emxArray_int32_T *idx);
static void c_findPeaksSeparatedByMoreThanM(const emxArray_real_T *iPk,
  emxArray_real_T *idx);
static void c_sort(double x[294], int idx[294]);
static void combinePeaks(const emxArray_real_T *iPk, const emxArray_real_T *iInf,
  emxArray_real_T *iPkOut);
static void diff(const double x[1000], double y[999]);
static void do_vectors(const emxArray_real_T *a, const emxArray_real_T *b,
  emxArray_real_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib);
static void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
static void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
static void emxFree_int32_T(emxArray_int32_T **pEmxArray);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions);
static void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
static void emxInit_int32_T1(emxArray_int32_T **pEmxArray, int numDimensions);
static void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
static void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions);
static double eps(double x);
static void featureExtractionEOG3(const double X[250], double LTH2, double UTH2,
  double F[13]);
static void filter(double b[7], double a[7], const double x[1036], const double
                   zi[6], double y[1036]);
static void findLocalMaxima(const double yTemp[250], emxArray_real_T *iPk,
  emxArray_real_T *iInflect);
static void flipud(double x[1036]);
static void getAllPeaks(const double y[250], emxArray_real_T *iPk,
  emxArray_real_T *iInf, emxArray_real_T *iInflect);
static void keepAtMostNpPeaks(emxArray_real_T *idx);
static double knn(const double tsX[26]);
static void merge(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int np,
                  int nq, emxArray_int32_T *iwork, emxArray_real_T *xwork);
static void merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset,
  int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_real_T *xwork);
static void merge_pow2_block(int idx[294], double x[294], int offset);
static void orderPeaks(const double Y[250], const emxArray_real_T *iPk,
  emxArray_real_T *idx);
static void removePeaksBelowMinPeakHeight(const double Y[250], emxArray_real_T
  *iPk, double Ph);
static void removePeaksBelowThreshold(const double Y[250], emxArray_real_T *iPk,
  double Th);
static double skip_to_last_equal_value(int *k, const emxArray_real_T *x);
static void sort(emxArray_real_T *x, emxArray_int32_T *idx);
static void sortIdx(emxArray_real_T *x, emxArray_int32_T *idx);
static double trapz(const emxArray_real_T *x);

// Function Definitions

//
// Arguments    : const double y[250]
//                const double x[250]
//                const emxArray_real_T *iPk
//                emxArray_real_T *YpkOut
//                emxArray_real_T *XpkOut
// Return Type  : void
//
static void assignOutputs(const double y[250], const double x[250], const
  emxArray_real_T *iPk, emxArray_real_T *YpkOut, emxArray_real_T *XpkOut)
{
  int i2;
  int loop_ub;
  i2 = YpkOut->size[0];
  YpkOut->size[0] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)YpkOut, i2, (int)sizeof(double));
  loop_ub = iPk->size[0];
  for (i2 = 0; i2 < loop_ub; i2++) {
    YpkOut->data[i2] = y[(int)iPk->data[i2] - 1];
  }

  i2 = XpkOut->size[0];
  XpkOut->size[0] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)XpkOut, i2, (int)sizeof(double));
  loop_ub = iPk->size[0];
  for (i2 = 0; i2 < loop_ub; i2++) {
    XpkOut->data[i2] = x[(int)iPk->data[i2] - 1];
  }
}

//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
static void b_diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int iyLead;
  int orderForDim;
  emxArray_real_T *work;
  int ySize_idx_0;
  int m;
  double tmp1;
  int k;
  double tmp2;
  if (x->size[0] == 0) {
    iyLead = y->size[0];
    y->size[0] = 0;
    emxEnsureCapacity((emxArray__common *)y, iyLead, (int)sizeof(double));
  } else {
    if (x->size[0] - 1 <= 1) {
      orderForDim = x->size[0] - 1;
    } else {
      orderForDim = 1;
    }

    if (orderForDim < 1) {
      iyLead = y->size[0];
      y->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)y, iyLead, (int)sizeof(double));
    } else {
      emxInit_real_T(&work, 1);
      ySize_idx_0 = x->size[0] - orderForDim;
      iyLead = work->size[0];
      work->size[0] = orderForDim;
      emxEnsureCapacity((emxArray__common *)work, iyLead, (int)sizeof(double));
      iyLead = y->size[0];
      y->size[0] = ySize_idx_0;
      emxEnsureCapacity((emxArray__common *)y, iyLead, (int)sizeof(double));
      if (!(y->size[0] == 0)) {
        ySize_idx_0 = 1;
        iyLead = 0;
        work->data[0] = x->data[0];
        if (orderForDim >= 2) {
          for (m = 1; m < orderForDim; m++) {
            tmp1 = x->data[ySize_idx_0];
            for (k = 0; k + 1 <= m; k++) {
              tmp2 = work->data[k];
              work->data[k] = tmp1;
              tmp1 -= tmp2;
            }

            work->data[m] = tmp1;
            ySize_idx_0++;
          }
        }

        for (m = orderForDim + 1; m <= x->size[0]; m++) {
          tmp1 = x->data[ySize_idx_0];
          for (k = 0; k + 1 <= orderForDim; k++) {
            tmp2 = work->data[k];
            work->data[k] = tmp1;
            tmp1 -= tmp2;
          }

          ySize_idx_0++;
          y->data[iyLead] = tmp1;
          iyLead++;
        }
      }

      emxFree_real_T(&work);
    }
  }
}

//
// Arguments    : int idx[294]
//                double x[294]
//                int offset
//                int np
//                int nq
//                int iwork[294]
//                double xwork[294]
// Return Type  : void
//
static void b_merge(int idx[294], double x[294], int offset, int np, int nq, int
                    iwork[294], double xwork[294])
{
  int n;
  int qend;
  int p;
  int iout;
  int exitg1;
  if ((np == 0) || (nq == 0)) {
  } else {
    n = np + nq;
    for (qend = 0; qend + 1 <= n; qend++) {
      iwork[qend] = idx[offset + qend];
      xwork[qend] = x[offset + qend];
    }

    p = 0;
    n = np;
    qend = np + nq;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork[p] <= xwork[n]) {
        idx[iout] = iwork[p];
        x[iout] = xwork[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx[iout] = iwork[n];
        x[iout] = xwork[n];
        if (n + 1 < qend) {
          n++;
        } else {
          n = iout - p;
          while (p + 1 <= np) {
            idx[(n + p) + 1] = iwork[p];
            x[(n + p) + 1] = xwork[p];
            p++;
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

//
// Arguments    : int idx[294]
//                double x[294]
//                int offset
//                int n
//                int preSortLevel
//                int iwork[294]
//                double xwork[294]
// Return Type  : void
//
static void b_merge_block(int idx[294], double x[294], int offset, int n, int
  preSortLevel, int iwork[294], double xwork[294])
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        b_merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 1; nTail <= nPairs; nTail++) {
      b_merge(idx, x, offset + (nTail - 1) * tailOffset, bLen, bLen, iwork,
              xwork);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    b_merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
  }
}

//
// Arguments    : emxArray_real_T *x
//                int dim
//                emxArray_int32_T *idx
// Return Type  : void
//
static void b_sort(emxArray_real_T *x, int dim, emxArray_int32_T *idx)
{
  int i5;
  emxArray_real_T *vwork;
  int k;
  unsigned int unnamed_idx_0;
  int vstride;
  emxArray_int32_T *iidx;
  int j;
  if (dim <= 1) {
    i5 = x->size[0];
  } else {
    i5 = 1;
  }

  emxInit_real_T(&vwork, 1);
  k = vwork->size[0];
  vwork->size[0] = i5;
  emxEnsureCapacity((emxArray__common *)vwork, k, (int)sizeof(double));
  unnamed_idx_0 = (unsigned int)x->size[0];
  k = idx->size[0];
  idx->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)idx, k, (int)sizeof(int));
  if (dim > 2) {
    vstride = x->size[0];
  } else {
    vstride = 1;
    k = 1;
    while (k <= dim - 1) {
      k = x->size[0];
      vstride *= k;
      k = 2;
    }
  }

  emxInit_int32_T1(&iidx, 1);
  for (j = 0; j + 1 <= vstride; j++) {
    for (k = 0; k + 1 <= i5; k++) {
      vwork->data[k] = x->data[j + k * vstride];
    }

    sortIdx(vwork, iidx);
    for (k = 0; k + 1 <= i5; k++) {
      x->data[j + k * vstride] = vwork->data[k];
      idx->data[j + k * vstride] = iidx->data[k];
    }
  }

  emxFree_int32_T(&iidx);
  emxFree_real_T(&vwork);
}

//
// Arguments    : const emxArray_real_T *iPk
//                emxArray_real_T *idx
// Return Type  : void
//
static void c_findPeaksSeparatedByMoreThanM(const emxArray_real_T *iPk,
  emxArray_real_T *idx)
{
  int ndbl;
  int apnd;
  int cdiff;
  int absb;
  emxArray_real_T *y;
  if (iPk->size[0] < 1) {
    ndbl = 0;
    apnd = 0;
  } else {
    ndbl = (int)std::floor(((double)iPk->size[0] - 1.0) + 0.5);
    apnd = ndbl + 1;
    cdiff = (ndbl - iPk->size[0]) + 1;
    absb = iPk->size[0];
    if (std::fabs((double)cdiff) < 4.4408920985006262E-16 * (double)absb) {
      ndbl++;
      apnd = iPk->size[0];
    } else if (cdiff > 0) {
      apnd = ndbl;
    } else {
      ndbl++;
    }
  }

  emxInit_real_T1(&y, 2);
  absb = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = ndbl;
  emxEnsureCapacity((emxArray__common *)y, absb, (int)sizeof(double));
  if (ndbl > 0) {
    y->data[0] = 1.0;
    if (ndbl > 1) {
      y->data[ndbl - 1] = apnd;
      cdiff = (ndbl - 1) / 2;
      for (absb = 1; absb < cdiff; absb++) {
        y->data[absb] = 1.0 + (double)absb;
        y->data[(ndbl - absb) - 1] = apnd - absb;
      }

      if (cdiff << 1 == ndbl - 1) {
        y->data[cdiff] = (1.0 + (double)apnd) / 2.0;
      } else {
        y->data[cdiff] = 1.0 + (double)cdiff;
        y->data[cdiff + 1] = apnd - cdiff;
      }
    }
  }

  absb = idx->size[0];
  idx->size[0] = y->size[1];
  emxEnsureCapacity((emxArray__common *)idx, absb, (int)sizeof(double));
  cdiff = y->size[1];
  for (absb = 0; absb < cdiff; absb++) {
    idx->data[absb] = y->data[y->size[0] * absb];
  }

  emxFree_real_T(&y);
}

//
// Arguments    : double x[294]
//                int idx[294]
// Return Type  : void
//
static void c_sort(double x[294], int idx[294])
{
  double x4[4];
  short idx4[4];
  int m;
  double xwork[294];
  int nNaNs;
  int ib;
  int k;
  signed char perm[4];
  int iwork[294];
  int i2;
  int i3;
  int i4;
  memset(&idx[0], 0, 294U * sizeof(int));
  for (m = 0; m < 4; m++) {
    x4[m] = 0.0;
    idx4[m] = 0;
  }

  memset(&xwork[0], 0, 294U * sizeof(double));
  nNaNs = -293;
  ib = 0;
  for (k = 0; k < 294; k++) {
    if (rtIsNaN(x[k])) {
      idx[-nNaNs] = k + 1;
      xwork[-nNaNs] = x[k];
      nNaNs++;
    } else {
      ib++;
      idx4[ib - 1] = (short)(k + 1);
      x4[ib - 1] = x[k];
      if (ib == 4) {
        ib = (k - nNaNs) - 296;
        if (x4[0] <= x4[1]) {
          m = 1;
          i2 = 2;
        } else {
          m = 2;
          i2 = 1;
        }

        if (x4[2] <= x4[3]) {
          i3 = 3;
          i4 = 4;
        } else {
          i3 = 4;
          i4 = 3;
        }

        if (x4[m - 1] <= x4[i3 - 1]) {
          if (x4[i2 - 1] <= x4[i3 - 1]) {
            perm[0] = (signed char)m;
            perm[1] = (signed char)i2;
            perm[2] = (signed char)i3;
            perm[3] = (signed char)i4;
          } else if (x4[i2 - 1] <= x4[i4 - 1]) {
            perm[0] = (signed char)m;
            perm[1] = (signed char)i3;
            perm[2] = (signed char)i2;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)m;
            perm[1] = (signed char)i3;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)i2;
          }
        } else if (x4[m - 1] <= x4[i4 - 1]) {
          if (x4[i2 - 1] <= x4[i4 - 1]) {
            perm[0] = (signed char)i3;
            perm[1] = (signed char)m;
            perm[2] = (signed char)i2;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)i3;
            perm[1] = (signed char)m;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)i2;
          }
        } else {
          perm[0] = (signed char)i3;
          perm[1] = (signed char)i4;
          perm[2] = (signed char)m;
          perm[3] = (signed char)i2;
        }

        idx[ib] = idx4[perm[0] - 1];
        idx[ib + 1] = idx4[perm[1] - 1];
        idx[ib + 2] = idx4[perm[2] - 1];
        idx[ib + 3] = idx4[perm[3] - 1];
        x[ib] = x4[perm[0] - 1];
        x[ib + 1] = x4[perm[1] - 1];
        x[ib + 2] = x4[perm[2] - 1];
        x[ib + 3] = x4[perm[3] - 1];
        ib = 0;
      }
    }
  }

  if (ib > 0) {
    for (m = 0; m < 4; m++) {
      perm[m] = 0;
    }

    if (ib == 1) {
      perm[0] = 1;
    } else if (ib == 2) {
      if (x4[0] <= x4[1]) {
        perm[0] = 1;
        perm[1] = 2;
      } else {
        perm[0] = 2;
        perm[1] = 1;
      }
    } else if (x4[0] <= x4[1]) {
      if (x4[1] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 2;
        perm[2] = 3;
      } else if (x4[0] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 3;
        perm[2] = 2;
      } else {
        perm[0] = 3;
        perm[1] = 1;
        perm[2] = 2;
      }
    } else if (x4[0] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 1;
      perm[2] = 3;
    } else if (x4[1] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 3;
      perm[2] = 1;
    } else {
      perm[0] = 3;
      perm[1] = 2;
      perm[2] = 1;
    }

    for (k = 1; k <= ib; k++) {
      idx[(k - nNaNs) - ib] = idx4[perm[k - 1] - 1];
      x[(k - nNaNs) - ib] = x4[perm[k - 1] - 1];
    }
  }

  m = (nNaNs + 293) >> 1;
  for (k = 1; k <= m; k++) {
    ib = idx[k - nNaNs];
    idx[k - nNaNs] = idx[294 - k];
    idx[294 - k] = ib;
    x[k - nNaNs] = xwork[294 - k];
    x[294 - k] = xwork[k - nNaNs];
  }

  if (((nNaNs + 293) & 1) != 0) {
    x[(m - nNaNs) + 1] = xwork[(m - nNaNs) + 1];
  }

  memset(&iwork[0], 0, 294U * sizeof(int));
  m = 2;
  if (1 - nNaNs > 1) {
    ib = (1 - nNaNs) >> 8;
    if (ib > 0) {
      for (m = 1; m <= ib; m++) {
        merge_pow2_block(idx, x, (m - 1) << 8);
      }

      m = ib << 8;
      ib = 1 - (nNaNs + m);
      if (ib > 0) {
        b_merge_block(idx, x, m, ib, 2, iwork, xwork);
      }

      m = 8;
    }

    b_merge_block(idx, x, 0, 1 - nNaNs, m, iwork, xwork);
  }
}

//
// Arguments    : const emxArray_real_T *iPk
//                const emxArray_real_T *iInf
//                emxArray_real_T *iPkOut
// Return Type  : void
//
static void combinePeaks(const emxArray_real_T *iPk, const emxArray_real_T *iInf,
  emxArray_real_T *iPkOut)
{
  emxArray_int32_T *ia;
  emxArray_int32_T *ib;
  emxInit_int32_T1(&ia, 1);
  emxInit_int32_T1(&ib, 1);
  do_vectors(iPk, iInf, iPkOut, ia, ib);
  emxFree_int32_T(&ib);
  emxFree_int32_T(&ia);
}

//
// Arguments    : const double x[1000]
//                double y[999]
// Return Type  : void
//
static void diff(const double x[1000], double y[999])
{
  int ixLead;
  int iyLead;
  double work;
  int m;
  double tmp2;
  ixLead = 1;
  iyLead = 0;
  work = x[0];
  for (m = 0; m < 999; m++) {
    tmp2 = work;
    work = x[ixLead];
    tmp2 = x[ixLead] - tmp2;
    ixLead++;
    y[iyLead] = tmp2;
    iyLead++;
  }
}

//
// Arguments    : const emxArray_real_T *a
//                const emxArray_real_T *b
//                emxArray_real_T *c
//                emxArray_int32_T *ia
//                emxArray_int32_T *ib
// Return Type  : void
//
static void do_vectors(const emxArray_real_T *a, const emxArray_real_T *b,
  emxArray_real_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib)
{
  int na;
  int nb;
  int ncmax;
  int ibfirst;
  int nc;
  int nia;
  int nib;
  int iafirst;
  int ialast;
  int iblast;
  int b_ialast;
  double ak;
  int b_iblast;
  double bk;
  boolean_T p;
  emxArray_int32_T *b_ia;
  emxArray_int32_T *b_ib;
  emxArray_real_T *b_c;
  na = a->size[0];
  nb = b->size[0];
  ncmax = a->size[0] + b->size[0];
  ibfirst = c->size[0];
  c->size[0] = ncmax;
  emxEnsureCapacity((emxArray__common *)c, ibfirst, (int)sizeof(double));
  ibfirst = ia->size[0];
  ia->size[0] = a->size[0];
  emxEnsureCapacity((emxArray__common *)ia, ibfirst, (int)sizeof(int));
  ibfirst = ib->size[0];
  ib->size[0] = b->size[0];
  emxEnsureCapacity((emxArray__common *)ib, ibfirst, (int)sizeof(int));
  nc = -1;
  nia = -1;
  nib = 0;
  iafirst = 1;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= na) && (iblast <= nb)) {
    b_ialast = ialast;
    ak = skip_to_last_equal_value(&b_ialast, a);
    ialast = b_ialast;
    b_iblast = iblast;
    bk = skip_to_last_equal_value(&b_iblast, b);
    iblast = b_iblast;
    if ((std::fabs(bk - ak) < eps(bk / 2.0)) || (rtIsInf(ak) && rtIsInf(bk) &&
         ((ak > 0.0) == (bk > 0.0)))) {
      p = true;
    } else {
      p = false;
    }

    if (p) {
      nc++;
      c->data[nc] = ak;
      nia++;
      ia->data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else {
      if ((ak < bk) || rtIsNaN(bk)) {
        p = true;
      } else {
        p = false;
      }

      if (p) {
        nc++;
        nia++;
        c->data[nc] = ak;
        ia->data[nia] = iafirst;
        ialast = b_ialast + 1;
        iafirst = b_ialast + 1;
      } else {
        nc++;
        nib++;
        c->data[nc] = bk;
        ib->data[nib - 1] = ibfirst + 1;
        iblast = b_iblast + 1;
        ibfirst = b_iblast;
      }
    }
  }

  while (ialast <= na) {
    iafirst = ialast;
    ak = skip_to_last_equal_value(&iafirst, a);
    nc++;
    nia++;
    c->data[nc] = ak;
    ia->data[nia] = ialast;
    ialast = iafirst + 1;
  }

  while (iblast <= nb) {
    iafirst = iblast;
    bk = skip_to_last_equal_value(&iafirst, b);
    nc++;
    nib++;
    c->data[nc] = bk;
    ib->data[nib - 1] = iblast;
    iblast = iafirst + 1;
  }

  if (a->size[0] > 0) {
    if (1 > nia + 1) {
      iafirst = -1;
    } else {
      iafirst = nia;
    }

    emxInit_int32_T1(&b_ia, 1);
    ibfirst = b_ia->size[0];
    b_ia->size[0] = iafirst + 1;
    emxEnsureCapacity((emxArray__common *)b_ia, ibfirst, (int)sizeof(int));
    for (ibfirst = 0; ibfirst <= iafirst; ibfirst++) {
      b_ia->data[ibfirst] = ia->data[ibfirst];
    }

    ibfirst = ia->size[0];
    ia->size[0] = b_ia->size[0];
    emxEnsureCapacity((emxArray__common *)ia, ibfirst, (int)sizeof(int));
    iafirst = b_ia->size[0];
    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      ia->data[ibfirst] = b_ia->data[ibfirst];
    }

    emxFree_int32_T(&b_ia);
  }

  if (b->size[0] > 0) {
    if (1 > nib) {
      iafirst = 0;
    } else {
      iafirst = nib;
    }

    emxInit_int32_T1(&b_ib, 1);
    ibfirst = b_ib->size[0];
    b_ib->size[0] = iafirst;
    emxEnsureCapacity((emxArray__common *)b_ib, ibfirst, (int)sizeof(int));
    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      b_ib->data[ibfirst] = ib->data[ibfirst];
    }

    ibfirst = ib->size[0];
    ib->size[0] = b_ib->size[0];
    emxEnsureCapacity((emxArray__common *)ib, ibfirst, (int)sizeof(int));
    iafirst = b_ib->size[0];
    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      ib->data[ibfirst] = b_ib->data[ibfirst];
    }

    emxFree_int32_T(&b_ib);
  }

  if (ncmax > 0) {
    if (1 > nc + 1) {
      iafirst = -1;
    } else {
      iafirst = nc;
    }

    emxInit_real_T(&b_c, 1);
    ibfirst = b_c->size[0];
    b_c->size[0] = iafirst + 1;
    emxEnsureCapacity((emxArray__common *)b_c, ibfirst, (int)sizeof(double));
    for (ibfirst = 0; ibfirst <= iafirst; ibfirst++) {
      b_c->data[ibfirst] = c->data[ibfirst];
    }

    ibfirst = c->size[0];
    c->size[0] = b_c->size[0];
    emxEnsureCapacity((emxArray__common *)c, ibfirst, (int)sizeof(double));
    iafirst = b_c->size[0];
    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      c->data[ibfirst] = b_c->data[ibfirst];
    }

    emxFree_real_T(&b_c);
  }
}

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
// Arguments    : emxArray_boolean_T **pEmxArray
// Return Type  : void
//
static void emxFree_boolean_T(emxArray_boolean_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_boolean_T *)NULL) {
    if (((*pEmxArray)->data != (boolean_T *)NULL) && (*pEmxArray)->canFreeData)
    {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_boolean_T *)NULL;
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
// Arguments    : emxArray_boolean_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions)
{
  emxArray_boolean_T *emxArray;
  int i;
  *pEmxArray = (emxArray_boolean_T *)malloc(sizeof(emxArray_boolean_T));
  emxArray = *pEmxArray;
  emxArray->data = (boolean_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
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
// Arguments    : emxArray_int32_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_int32_T1(emxArray_int32_T **pEmxArray, int numDimensions)
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

//
// Arguments    : double x
// Return Type  : double
//
static double eps(double x)
{
  double r;
  double absxk;
  int exponent;
  absxk = std::fabs(x);
  if ((!rtIsInf(absxk)) && (!rtIsNaN(absxk))) {
    if (absxk <= 2.2250738585072014E-308) {
      r = 4.94065645841247E-324;
    } else {
      frexp(absxk, &exponent);
      r = std::ldexp(1.0, exponent - 53);
    }
  } else {
    r = rtNaN;
  }

  return r;
}

//
// % FeatureExtractionEOG3 (from diff signal)
//  Accepts 2 low threshold and 2 upper thresholds.
//  Upper Thresholds UTH1 UTH2
//  Lower Threshold. LTH1 LTH2
// %-TODO: FINALIZE THRESHOLDS:
//  UTH1 = 0.4E-4;
//  UTH2 = 2.75E-4;
//  LTH1 = -0.5E-4;
//  LTH2 = -2.75E-4;
// Arguments    : const double X[250]
//                double LTH2
//                double UTH2
//                double F[13]
// Return Type  : void
//
static void featureExtractionEOG3(const double X[250], double LTH2, double UTH2,
  double F[13])
{
  int ixstart;
  double mtmp;
  int itmp;
  int ix;
  boolean_T exitg2;
  double b_mtmp;
  int b_itmp;
  boolean_T exitg1;
  emxArray_real_T *Pmin;
  emxArray_real_T *iPk;
  emxArray_real_T *idx;
  emxArray_real_T *b_iPk;
  double dv4[250];
  emxArray_real_T *c_iPk;
  emxArray_real_T *Pmax;
  double Yin[250];
  emxArray_real_T *d_iPk;
  double Famplitude;
  double xbar;
  double r;
  double y;
  emxArray_int32_T *r0;
  emxArray_real_T *b_X;
  double FInt1;
  emxArray_real_T *c_X;
  double FInt2;
  boolean_T x[250];
  double FcountMin;
  double FcountMax;
  double FcountMaxHigh;
  double FcountMinLow;
  ixstart = 1;
  mtmp = X[0];
  itmp = 1;
  if (rtIsNaN(X[0])) {
    ix = 2;
    exitg2 = false;
    while ((!exitg2) && (ix < 251)) {
      ixstart = ix;
      if (!rtIsNaN(X[ix - 1])) {
        mtmp = X[ix - 1];
        itmp = ix;
        exitg2 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < 250) {
    while (ixstart + 1 < 251) {
      if (X[ixstart] > mtmp) {
        mtmp = X[ixstart];
        itmp = ixstart + 1;
      }

      ixstart++;
    }
  }

  ixstart = 1;
  b_mtmp = X[0];
  b_itmp = 1;
  if (rtIsNaN(X[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix < 251)) {
      ixstart = ix;
      if (!rtIsNaN(X[ix - 1])) {
        b_mtmp = X[ix - 1];
        b_itmp = ix;
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < 250) {
    while (ixstart + 1 < 251) {
      if (X[ixstart] < b_mtmp) {
        b_mtmp = X[ixstart];
        b_itmp = ixstart + 1;
      }

      ixstart++;
    }
  }

  emxInit_real_T(&Pmin, 1);
  emxInit_real_T(&iPk, 1);
  emxInit_real_T(&idx, 1);
  emxInit_real_T(&b_iPk, 1);
  getAllPeaks(X, iPk, Pmin, idx);
  removePeaksBelowMinPeakHeight(X, iPk, 4.0E-5);
  removePeaksBelowThreshold(X, iPk, 0.0);
  combinePeaks(iPk, Pmin, b_iPk);
  c_findPeaksSeparatedByMoreThanM(b_iPk, idx);
  orderPeaks(X, b_iPk, idx);
  keepAtMostNpPeaks(idx);
  for (ix = 0; ix < 250; ix++) {
    dv4[ix] = 1.0 + (double)ix;
  }

  emxInit_real_T(&c_iPk, 1);
  ix = c_iPk->size[0];
  c_iPk->size[0] = idx->size[0];
  emxEnsureCapacity((emxArray__common *)c_iPk, ix, (int)sizeof(double));
  ixstart = idx->size[0];
  for (ix = 0; ix < ixstart; ix++) {
    c_iPk->data[ix] = b_iPk->data[(int)idx->data[ix] - 1];
  }

  emxInit_real_T(&Pmax, 1);
  assignOutputs(X, dv4, c_iPk, Pmax, idx);
  emxFree_real_T(&c_iPk);
  for (ix = 0; ix < 250; ix++) {
    Yin[ix] = -X[ix];
  }

  getAllPeaks(Yin, iPk, Pmin, idx);
  removePeaksBelowMinPeakHeight(Yin, iPk, 5.0E-5);
  removePeaksBelowThreshold(Yin, iPk, 0.0);
  combinePeaks(iPk, Pmin, b_iPk);
  c_findPeaksSeparatedByMoreThanM(b_iPk, idx);
  orderPeaks(Yin, b_iPk, idx);
  keepAtMostNpPeaks(idx);
  emxFree_real_T(&iPk);
  for (ix = 0; ix < 250; ix++) {
    dv4[ix] = 1.0 + (double)ix;
  }

  emxInit_real_T(&d_iPk, 1);
  ix = d_iPk->size[0];
  d_iPk->size[0] = idx->size[0];
  emxEnsureCapacity((emxArray__common *)d_iPk, ix, (int)sizeof(double));
  ixstart = idx->size[0];
  for (ix = 0; ix < ixstart; ix++) {
    d_iPk->data[ix] = b_iPk->data[(int)idx->data[ix] - 1];
  }

  emxFree_real_T(&b_iPk);
  assignOutputs(Yin, dv4, d_iPk, Pmin, idx);
  emxFree_real_T(&d_iPk);
  emxFree_real_T(&idx);
  if (Pmax->size[0] == 0) {
    ix = Pmax->size[0];
    Pmax->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)Pmax, ix, (int)sizeof(double));
    Pmax->data[0] = 0.0;
  }

  if (Pmin->size[0] == 0) {
    ix = Pmin->size[0];
    Pmin->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)Pmin, ix, (int)sizeof(double));
    Pmin->data[0] = 0.0;
  } else {
    ix = Pmin->size[0];
    emxEnsureCapacity((emxArray__common *)Pmin, ix, (int)sizeof(double));
    ixstart = Pmin->size[0];
    for (ix = 0; ix < ixstart; ix++) {
      Pmin->data[ix] = -Pmin->data[ix];
    }
  }

  Famplitude = mtmp - b_mtmp;
  ix = 0;
  xbar = X[0];
  for (ixstart = 0; ixstart < 249; ixstart++) {
    ix++;
    xbar += X[ix];
  }

  xbar /= 250.0;
  ix = 0;
  r = X[0] - xbar;
  y = r * r;
  for (ixstart = 0; ixstart < 249; ixstart++) {
    ix++;
    r = X[ix] - xbar;
    y += r * r;
  }

  emxInit_int32_T1(&r0, 1);
  y /= 249.0;
  ixstart = 0;
  for (ix = 0; ix < 250; ix++) {
    if ((X[ix] > 4.0E-5) && (X[ix] < UTH2)) {
      ixstart++;
    }
  }

  ix = r0->size[0];
  r0->size[0] = ixstart;
  emxEnsureCapacity((emxArray__common *)r0, ix, (int)sizeof(int));
  ixstart = 0;
  for (ix = 0; ix < 250; ix++) {
    if ((X[ix] > 4.0E-5) && (X[ix] < UTH2)) {
      r0->data[ixstart] = ix + 1;
      ixstart++;
    }
  }

  emxInit_real_T(&b_X, 1);
  ix = b_X->size[0];
  b_X->size[0] = r0->size[0];
  emxEnsureCapacity((emxArray__common *)b_X, ix, (int)sizeof(double));
  ixstart = r0->size[0];
  for (ix = 0; ix < ixstart; ix++) {
    b_X->data[ix] = X[r0->data[ix] - 1];
  }

  FInt1 = trapz(b_X);
  ixstart = 0;
  emxFree_real_T(&b_X);
  for (ix = 0; ix < 250; ix++) {
    if ((X[ix] > LTH2) && (X[ix] < -5.0E-5)) {
      ixstart++;
    }
  }

  ix = r0->size[0];
  r0->size[0] = ixstart;
  emxEnsureCapacity((emxArray__common *)r0, ix, (int)sizeof(int));
  ixstart = 0;
  for (ix = 0; ix < 250; ix++) {
    if ((X[ix] > LTH2) && (X[ix] < -5.0E-5)) {
      r0->data[ixstart] = ix + 1;
      ixstart++;
    }
  }

  emxInit_real_T(&c_X, 1);
  ix = c_X->size[0];
  c_X->size[0] = r0->size[0];
  emxEnsureCapacity((emxArray__common *)c_X, ix, (int)sizeof(double));
  ixstart = r0->size[0];
  for (ix = 0; ix < ixstart; ix++) {
    c_X->data[ix] = -X[r0->data[ix] - 1];
  }

  emxFree_int32_T(&r0);
  FInt2 = trapz(c_X);
  emxFree_real_T(&c_X);
  for (ix = 0; ix < 250; ix++) {
    x[ix] = ((X[ix] > LTH2) && (X[ix] < -5.0E-5));
  }

  FcountMin = x[0];
  for (ixstart = 0; ixstart < 249; ixstart++) {
    FcountMin += (double)x[ixstart + 1];
  }

  // Count between bottom two lines.
  for (ix = 0; ix < 250; ix++) {
    x[ix] = ((X[ix] > 4.0E-5) && (X[ix] < UTH2));
  }

  FcountMax = x[0];
  for (ixstart = 0; ixstart < 249; ixstart++) {
    FcountMax += (double)x[ixstart + 1];
  }

  for (ix = 0; ix < 250; ix++) {
    x[ix] = (X[ix] > UTH2);
  }

  FcountMaxHigh = x[0];
  for (ixstart = 0; ixstart < 249; ixstart++) {
    FcountMaxHigh += (double)x[ixstart + 1];
  }

  for (ix = 0; ix < 250; ix++) {
    x[ix] = (X[ix] < LTH2);
  }

  FcountMinLow = x[0];
  for (ixstart = 0; ixstart < 249; ixstart++) {
    FcountMinLow += (double)x[ixstart + 1];
  }

  // %% PLOT FEATURES %%%
  F[0] = mtmp;
  F[1] = b_mtmp;
  F[2] = Pmax->data[0];
  F[3] = Pmin->data[0];
  F[4] = Famplitude;
  F[5] = std::sqrt(y);
  F[6] = FInt1;
  F[7] = FInt2;
  F[8] = Famplitude / (double)(itmp - b_itmp);
  F[9] = FcountMin;
  F[10] = FcountMax;
  F[11] = FcountMaxHigh;
  F[12] = FcountMinLow;

  // {
  // EOF
  // }
  emxFree_real_T(&Pmin);
  emxFree_real_T(&Pmax);
}

//
// Arguments    : double b[7]
//                double a[7]
//                const double x[1036]
//                const double zi[6]
//                double y[1036]
// Return Type  : void
//
static void filter(double b[7], double a[7], const double x[1036], const double
                   zi[6], double y[1036])
{
  double a1;
  int k;
  double dbuffer[7];
  int j;
  a1 = a[0];
  if ((!((!rtIsInf(a[0])) && (!rtIsNaN(a[0])))) || (a[0] == 0.0) || (!(a[0] !=
        1.0))) {
  } else {
    for (k = 0; k < 7; k++) {
      b[k] /= a1;
    }

    for (k = 0; k < 6; k++) {
      a[k + 1] /= a1;
    }

    a[0] = 1.0;
  }

  for (k = 0; k < 6; k++) {
    dbuffer[k + 1] = zi[k];
  }

  for (j = 0; j < 1036; j++) {
    for (k = 0; k < 6; k++) {
      dbuffer[k] = dbuffer[k + 1];
    }

    dbuffer[6] = 0.0;
    for (k = 0; k < 7; k++) {
      dbuffer[k] += x[j] * b[k];
    }

    for (k = 0; k < 6; k++) {
      dbuffer[k + 1] -= dbuffer[0] * a[k + 1];
    }

    y[j] = dbuffer[0];
  }
}

//
// Arguments    : const double yTemp[250]
//                emxArray_real_T *iPk
//                emxArray_real_T *iInflect
// Return Type  : void
//
static void findLocalMaxima(const double yTemp[250], emxArray_real_T *iPk,
  emxArray_real_T *iInflect)
{
  double b_yTemp[252];
  boolean_T yFinite[252];
  int ii;
  boolean_T x[251];
  emxArray_int32_T *b_ii;
  int idx;
  int i1;
  boolean_T exitg3;
  emxArray_int32_T *r1;
  boolean_T guard3 = false;
  emxArray_real_T *iTemp;
  emxArray_real_T *c_yTemp;
  emxArray_real_T *s;
  int nx;
  emxArray_boolean_T *b_x;
  emxArray_real_T *r2;
  double c_x;
  boolean_T exitg2;
  boolean_T guard2 = false;
  emxArray_int32_T *c_ii;
  boolean_T exitg1;
  boolean_T guard1 = false;
  b_yTemp[0] = rtNaN;
  memcpy(&b_yTemp[1], &yTemp[0], 250U * sizeof(double));
  b_yTemp[251] = rtNaN;
  for (ii = 0; ii < 252; ii++) {
    yFinite[ii] = !rtIsNaN(b_yTemp[ii]);
  }

  for (ii = 0; ii < 251; ii++) {
    x[ii] = ((b_yTemp[ii] != b_yTemp[ii + 1]) && (yFinite[ii] || yFinite[ii + 1]));
  }

  emxInit_int32_T1(&b_ii, 1);
  idx = 0;
  i1 = b_ii->size[0];
  b_ii->size[0] = 251;
  emxEnsureCapacity((emxArray__common *)b_ii, i1, (int)sizeof(int));
  ii = 1;
  exitg3 = false;
  while ((!exitg3) && (ii < 252)) {
    guard3 = false;
    if (x[ii - 1]) {
      idx++;
      b_ii->data[idx - 1] = ii;
      if (idx >= 251) {
        exitg3 = true;
      } else {
        guard3 = true;
      }
    } else {
      guard3 = true;
    }

    if (guard3) {
      ii++;
    }
  }

  emxInit_int32_T1(&r1, 1);
  i1 = b_ii->size[0];
  if (1 > idx) {
    b_ii->size[0] = 0;
  } else {
    b_ii->size[0] = idx;
  }

  emxEnsureCapacity((emxArray__common *)b_ii, i1, (int)sizeof(int));
  i1 = r1->size[0];
  r1->size[0] = 1 + b_ii->size[0];
  emxEnsureCapacity((emxArray__common *)r1, i1, (int)sizeof(int));
  r1->data[0] = 1;
  ii = b_ii->size[0];
  for (i1 = 0; i1 < ii; i1++) {
    r1->data[i1 + 1] = b_ii->data[i1] + 1;
  }

  emxInit_real_T(&iTemp, 1);
  i1 = iTemp->size[0];
  iTemp->size[0] = r1->size[0];
  emxEnsureCapacity((emxArray__common *)iTemp, i1, (int)sizeof(double));
  ii = r1->size[0];
  for (i1 = 0; i1 < ii; i1++) {
    iTemp->data[i1] = 1.0 + (double)(r1->data[i1] - 1);
  }

  emxFree_int32_T(&r1);
  emxInit_real_T(&c_yTemp, 1);
  i1 = c_yTemp->size[0];
  c_yTemp->size[0] = iTemp->size[0];
  emxEnsureCapacity((emxArray__common *)c_yTemp, i1, (int)sizeof(double));
  ii = iTemp->size[0];
  for (i1 = 0; i1 < ii; i1++) {
    c_yTemp->data[i1] = b_yTemp[(int)iTemp->data[i1] - 1];
  }

  emxInit_real_T(&s, 1);
  b_diff(c_yTemp, s);
  nx = s->size[0];
  ii = 0;
  emxFree_real_T(&c_yTemp);
  while (ii + 1 <= nx) {
    if (s->data[ii] < 0.0) {
      c_x = -1.0;
    } else if (s->data[ii] > 0.0) {
      c_x = 1.0;
    } else if (s->data[ii] == 0.0) {
      c_x = 0.0;
    } else {
      c_x = s->data[ii];
    }

    s->data[ii] = c_x;
    ii++;
  }

  emxInit_boolean_T(&b_x, 1);
  emxInit_real_T(&r2, 1);
  b_diff(s, r2);
  i1 = b_x->size[0];
  b_x->size[0] = r2->size[0];
  emxEnsureCapacity((emxArray__common *)b_x, i1, (int)sizeof(boolean_T));
  ii = r2->size[0];
  for (i1 = 0; i1 < ii; i1++) {
    b_x->data[i1] = (r2->data[i1] < 0.0);
  }

  emxFree_real_T(&r2);
  nx = b_x->size[0];
  idx = 0;
  i1 = b_ii->size[0];
  b_ii->size[0] = b_x->size[0];
  emxEnsureCapacity((emxArray__common *)b_ii, i1, (int)sizeof(int));
  ii = 1;
  exitg2 = false;
  while ((!exitg2) && (ii <= nx)) {
    guard2 = false;
    if (b_x->data[ii - 1]) {
      idx++;
      b_ii->data[idx - 1] = ii;
      if (idx >= nx) {
        exitg2 = true;
      } else {
        guard2 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard2) {
      ii++;
    }
  }

  if (b_x->size[0] == 1) {
    if (idx == 0) {
      i1 = b_ii->size[0];
      b_ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)b_ii, i1, (int)sizeof(int));
    }
  } else {
    i1 = b_ii->size[0];
    if (1 > idx) {
      b_ii->size[0] = 0;
    } else {
      b_ii->size[0] = idx;
    }

    emxEnsureCapacity((emxArray__common *)b_ii, i1, (int)sizeof(int));
  }

  if (1 > s->size[0] - 1) {
    ii = 0;
  } else {
    ii = s->size[0] - 1;
  }

  if (2 > s->size[0]) {
    i1 = 0;
  } else {
    i1 = 1;
  }

  idx = b_x->size[0];
  b_x->size[0] = ii;
  emxEnsureCapacity((emxArray__common *)b_x, idx, (int)sizeof(boolean_T));
  for (idx = 0; idx < ii; idx++) {
    b_x->data[idx] = (s->data[idx] != s->data[i1 + idx]);
  }

  emxFree_real_T(&s);
  emxInit_int32_T1(&c_ii, 1);
  nx = b_x->size[0];
  idx = 0;
  i1 = c_ii->size[0];
  c_ii->size[0] = b_x->size[0];
  emxEnsureCapacity((emxArray__common *)c_ii, i1, (int)sizeof(int));
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx)) {
    guard1 = false;
    if (b_x->data[ii - 1]) {
      idx++;
      c_ii->data[idx - 1] = ii;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
    }
  }

  if (b_x->size[0] == 1) {
    if (idx == 0) {
      i1 = c_ii->size[0];
      c_ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)c_ii, i1, (int)sizeof(int));
    }
  } else {
    i1 = c_ii->size[0];
    if (1 > idx) {
      c_ii->size[0] = 0;
    } else {
      c_ii->size[0] = idx;
    }

    emxEnsureCapacity((emxArray__common *)c_ii, i1, (int)sizeof(int));
  }

  emxFree_boolean_T(&b_x);
  i1 = iInflect->size[0];
  iInflect->size[0] = c_ii->size[0];
  emxEnsureCapacity((emxArray__common *)iInflect, i1, (int)sizeof(double));
  ii = c_ii->size[0];
  for (i1 = 0; i1 < ii; i1++) {
    iInflect->data[i1] = iTemp->data[c_ii->data[i1]] - 1.0;
  }

  emxFree_int32_T(&c_ii);
  i1 = iPk->size[0];
  iPk->size[0] = b_ii->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, i1, (int)sizeof(double));
  ii = b_ii->size[0];
  for (i1 = 0; i1 < ii; i1++) {
    iPk->data[i1] = iTemp->data[b_ii->data[i1]] - 1.0;
  }

  emxFree_int32_T(&b_ii);
  emxFree_real_T(&iTemp);
}

//
// Arguments    : double x[1036]
// Return Type  : void
//
static void flipud(double x[1036])
{
  int i;
  double xtmp;
  for (i = 0; i < 518; i++) {
    xtmp = x[i];
    x[i] = x[1035 - i];
    x[1035 - i] = xtmp;
  }
}

//
// Arguments    : const double y[250]
//                emxArray_real_T *iPk
//                emxArray_real_T *iInf
//                emxArray_real_T *iInflect
// Return Type  : void
//
static void getAllPeaks(const double y[250], emxArray_real_T *iPk,
  emxArray_real_T *iInf, emxArray_real_T *iInflect)
{
  boolean_T x[250];
  int ii;
  emxArray_int32_T *b_ii;
  int idx;
  int i0;
  boolean_T exitg1;
  boolean_T guard1 = false;
  double yTemp[250];
  for (ii = 0; ii < 250; ii++) {
    x[ii] = (rtIsInf(y[ii]) && (y[ii] > 0.0));
  }

  emxInit_int32_T1(&b_ii, 1);
  idx = 0;
  i0 = b_ii->size[0];
  b_ii->size[0] = 250;
  emxEnsureCapacity((emxArray__common *)b_ii, i0, (int)sizeof(int));
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii < 251)) {
    guard1 = false;
    if (x[ii - 1]) {
      idx++;
      b_ii->data[idx - 1] = ii;
      if (idx >= 250) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
    }
  }

  i0 = b_ii->size[0];
  if (1 > idx) {
    b_ii->size[0] = 0;
  } else {
    b_ii->size[0] = idx;
  }

  emxEnsureCapacity((emxArray__common *)b_ii, i0, (int)sizeof(int));
  i0 = iInf->size[0];
  iInf->size[0] = b_ii->size[0];
  emxEnsureCapacity((emxArray__common *)iInf, i0, (int)sizeof(double));
  ii = b_ii->size[0];
  for (i0 = 0; i0 < ii; i0++) {
    iInf->data[i0] = b_ii->data[i0];
  }

  memcpy(&yTemp[0], &y[0], 250U * sizeof(double));
  i0 = b_ii->size[0];
  b_ii->size[0] = iInf->size[0];
  emxEnsureCapacity((emxArray__common *)b_ii, i0, (int)sizeof(int));
  ii = iInf->size[0];
  for (i0 = 0; i0 < ii; i0++) {
    b_ii->data[i0] = (int)iInf->data[i0];
  }

  ii = b_ii->size[0];
  for (i0 = 0; i0 < ii; i0++) {
    yTemp[b_ii->data[i0] - 1] = rtNaN;
  }

  emxFree_int32_T(&b_ii);
  findLocalMaxima(yTemp, iPk, iInflect);
}

//
// Arguments    : emxArray_real_T *idx
// Return Type  : void
//
static void keepAtMostNpPeaks(emxArray_real_T *idx)
{
  emxArray_real_T *b_idx;
  int i6;
  int loop_ub;
  if (idx->size[0] > 250) {
    emxInit_real_T(&b_idx, 1);
    i6 = b_idx->size[0];
    b_idx->size[0] = 250;
    emxEnsureCapacity((emxArray__common *)b_idx, i6, (int)sizeof(double));
    for (i6 = 0; i6 < 250; i6++) {
      b_idx->data[i6] = idx->data[i6];
    }

    i6 = idx->size[0];
    idx->size[0] = b_idx->size[0];
    emxEnsureCapacity((emxArray__common *)idx, i6, (int)sizeof(double));
    loop_ub = b_idx->size[0];
    for (i6 = 0; i6 < loop_ub; i6++) {
      idx->data[i6] = b_idx->data[i6];
    }

    emxFree_real_T(&b_idx);
  }
}

//
// function yfit = knnclassification(testsamplesX,samplesX, samplesY, Knn, type)
//  Classify using the Nearest neighbor algorithm
//  Inputs:
//   tX    - Train samples
//  tY    - Train labels
//    tsX (testsamplesX) - Test  samples to classify
//  Knn         - Number of nearest neighbors
//
//  Outputs
//  result - Predicted targets
// if nargin < 5
//     type = '2norm';
// end
// Arguments    : const double tsX[26]
// Return Type  : double
//
static double knn(const double tsX[26])
{
  double yfit;
  int iwork[294];
  int idx[294];
  int k;
  int i;
  static const signed char a[294] = { 1, 1, 1, 1, 1, 3, 3, 3, 3, 6, 6, 3, 3, 3,
    3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
    1, 1, 1, 1, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 1, 1, 1, 1, 3, 3, 3,
    3, 6, 6, 6, 6, 3, 3, 3, 3, 1, 1, 1, 1, 5, 4, 4, 4, 5, 5, 5, 5, 5, 4, 4, 4, 4,
    1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3,
    3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 6, 6, 6, 6, 6, 3, 3, 3, 1, 1, 1, 1, 1, 3, 3, 3,
    3, 6, 6, 3, 3, 3, 3, 1, 1, 1, 1, 4, 4, 4, 4, 5, 5, 5, 4, 4, 4, 4, 1, 1, 1, 1,
    4, 4, 4, 4, 5, 5, 5, 5, 4, 4, 4, 4, 1, 1, 1, 1, 1, 4, 4, 4, 4, 5, 5, 5, 5, 5,
    5, 5, 5, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1,
    2, 2, 2, 2, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 2, 2,
    2, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 };

  emxArray_real_T *Uc;
  int high_i;
  int nb;
  int mid_i;
  int p;
  int q;
  int qEnd;
  int kEnd;
  int i3;
  int exitg5;
  boolean_T eok;
  static double x[7644];
  double y[7644];
  int b_k;
  static const double tX[7644] = { 0.000506203204630212, 0.000653421553649643,
    0.000653851539346269, 0.000653074791500862, 0.000149401567826332,
    0.000172064221145374, 0.00020037229218825, 0.00020760229886732,
    0.000210122488952632, 4.69338736212048E-5, 4.1905988526177E-5,
    0.000235650903249796, 0.000197755205634661, 0.000205220187603293,
    0.000206733059207471, 0.000368135157108165, 0.000679380180622912,
    0.00067354377930634, 0.000670360262760216, 0.000667745799490809,
    0.00026488696614303, 0.000205145204418392, 0.000209459926484155,
    0.000212874381598852, 0.000215489003442063, 0.000396836941315023,
    0.000648419038268201, 0.000644634101816123, 0.000642246513038339,
    0.0006413824451123, 1.05702136460731E-5, 1.52100061187241E-5,
    1.43656723294612E-5, 1.52491246155688E-5, 1.03710998928488E-5,
    1.04264838711727E-5, 1.13067773497509E-5, 1.03405708261822E-5,
    1.62135518374146E-5, 0.00055215936598822, 0.000625261022455498,
    0.000625281092170202, 0.000624497066744164, 1.69198502567413E-5,
    8.59641109721448E-6, 1.49501452062183E-5, 1.5923483250802E-5,
    1.25006025075809E-5, 1.54723920115371E-5, 7.36051549319851E-5,
    7.36773303421801E-5, 7.19884274779315E-5, 7.31998694111136E-5,
    2.94631235363161E-5, 2.00929353900678E-5, 1.97510370252736E-5,
    1.73566225671056E-5, 2.15908918204967E-5, 1.875410248372E-5,
    0.000572976786639885, 0.000566416724813911, 0.000567891159566254,
    0.000565195772556708, 0.000164786089793966, 0.000179852765806577,
    0.00018249088210625, 0.000186862230924144, 0.000106268240355691,
    9.85518254689806E-5, 9.60703159917486E-5, 4.69611446851956E-5,
    0.000221591533362357, 0.000223714690580713, 0.000227148339209426,
    0.000228529344520473, 0.000571213672005585, 0.000555171848795276,
    0.000554347168091888, 0.000551512651089181, 0.000135122928400434,
    0.000117547817067564, 0.000117264439106739, 0.000118333208025947,
    5.48785901510107E-5, 5.39862221302272E-5, 5.40445361880657E-5,
    5.42765752894477E-5, 3.42666489889101E-5, 1.15347292356208E-5,
    1.31730529250039E-5, 1.1555965478536E-5, 1.09094413948402E-5,
    0.000492313686896242, 0.000534512953070503, 0.000532755126522672,
    0.000530556973138342, 0.00028031842381281, 0.000190312392123071,
    0.000196444240854469, 0.000199706870091094, 0.000201846279470801,
    0.000399819210990882, 0.000478660615759749, 0.000476545807687982,
    0.000475326182083965, 0.000307712215646274, 0.000525974895193228,
    0.000563711432220438, 0.000557873018980576, 0.000555574845709092,
    4.53423891055154E-5, 0.000188023347165646, 0.000192467361270318,
    0.000194565215902008, 0.000195415964257309, 0.000286811962747598,
    0.000296253840246635, 0.000298974711407454, 0.000301110389198279,
    0.000332202827350685, 0.000591162299446557, 0.000588427119997063,
    0.000587149632576167, 0.000585610081125766, 0.00020227027526213,
    0.000221389901241328, 0.000227173960520158, 0.000229850104596555,
    2.47114838749485E-5, 2.82924483979289E-5, 3.3355387896189E-5,
    2.89697511703235E-5, 3.08577268879262E-5, 0.000142231975145607,
    0.000272370597985697, 0.000272969337248048, 0.000487651245012077,
    0.000547242829898251, 0.00053865200677907, 0.000536005214957568,
    0.000535329232200928, 0.00016245157071614, 0.000169211803444764,
    0.000171565348883927, 0.000173675043397824, 4.71171272810233E-5,
    4.0121268196254E-5, 0.000313436234086631, 0.000334792725045994,
    0.000340052379158077, 0.000343216075731401, 0.000579332910749996,
    0.00057071590683091, 0.000568156017721171, 0.00056697485535909,
    2.32091452766463E-5, 2.51528779127883E-5, 2.37397545465035E-5,
    2.24515532264375E-5, 6.89096169275921E-5, 7.0281826447603E-5,
    7.07501835519501E-5, 1.83433909498075E-5, 2.68721781669529E-5,
    2.63912204080605E-5, 2.5041862692563E-5, 0.000573245648197597,
    0.000572164381251719, 0.000570625928407816, 0.000569644250486277,
    1.64514298968736E-5, 1.65999957528365E-5, 1.60064740702875E-5,
    1.45448770407045E-5, 8.51045121145721E-5, 8.51755243570964E-5,
    8.69231018280025E-5, 8.71030422165712E-5, 3.45222154432236E-5,
    2.27494376816837E-5, 3.12151636159191E-5, 2.93485649238849E-5,
    0.000467500979242849, 0.00052533528048322, 0.000522998217581203,
    0.000520761792731922, 1.3787013234404E-5, 1.8755284450479E-5,
    1.17606995396957E-5, 2.0355560709803E-5, 1.35773381994933E-5,
    8.98337427905629E-5, 9.3507352890508E-5, 9.00448320164183E-5,
    9.21206006719604E-5, 7.29408946087993E-5, 7.23617844024596E-5,
    7.33622063902834E-5, 7.33970732720599E-5, 1.1073236358228E-5,
    1.71272855577753E-5, 1.42639520085448E-5, 2.09675309911968E-5,
    0.000555626565009091, 0.000551432728712292, 0.000550399700463786,
    0.000548510403014007, 0.000575517713831392, 0.000571579064947186,
    0.000569927580257488, 0.000567073037602756, 0.000570177378604387,
    0.000576926411538841, 0.000572154082255237, 0.000574854708986371,
    4.06213240276566E-5, 0.000569168893948126, 0.000611100400925535,
    0.000608937776489264, 0.000606976791501694, 0.000514767471707507,
    0.000468853707003861, 0.000600451305312134, 0.000594405523977957,
    0.000593118706926388, 0.000489354469632413, 0.000602743913077504,
    0.000601834489738073, 0.000598961489569931, 0.000595653094242136,
    0.000371387352686766, 0.000585906334912077, 0.000565629470433097,
    0.000585715796895615, 0.000584514871862233, 0.000542184571692569,
    0.000602483522250745, 0.000602782619960606, 0.000595536761996432,
    0.000500305906842891, 0.000438315419082145, 0.000441607417545678,
    0.000438875552968653, 0.000437053763084372, 0.000654969895538033,
    0.000652071966819221, 0.000651004725802267, 0.000649826325538414,
    0.000584458644010099, 0.000641845152202308, 0.00063829802764445,
    0.000637151127481978, 0.000614104150940733, 0.00059872226467719,
    0.000596180032147412, 0.00059481519163007, 0.000588423876120709,
    0.000555705010040313, 0.000582130501857259, 0.000580139707249668,
    0.000578936155326826, 0.00053949410416441, 0.000621998164889805,
    0.000617208082834397, 0.000617608406368013, 0.000615776868906625,
    0.000615111829881204, 0.000621146547193474, 0.000618575293561904,
    0.000616904849595831, 0.00042308699774791, 0.0005255194544779,
    0.000553317121580112, 0.000582077862883622, 0.000579533388103122,
    0.000517275196833826, 0.000564624402553722, 0.000553175356669581,
    0.000559187218660414, 0.000557243194178301, 0.000518806349405345,
    0.000502026635735339, 0.000347141543683723, 0.000481947271577469,
    0.000482408680705821, 0.000480723531489544, 0.00047977986272235,
    0.000530320546313339, 0.000583888411047522, 0.000579655766160469,
    0.000579175894318432, 0.000605909887607648, 0.000596211193080641,
    0.000600746231813899, -0.000124509106159094, -0.000497742580578788,
    -0.000497286948831836, -0.000498128233228922, -0.000498739202656245,
    -4.50807910771689E-5, -3.60242368730532E-5, -2.94180510932531E-5,
    -2.71581506513584E-5, -0.000216309569072829, -0.000242563471062339,
    -5.03400245124804E-5, -3.33580100863966E-5, -3.41155968815426E-5,
    -3.18176000410559E-5, -6.42920720359168E-5, -0.000447777623018534,
    -0.000452785993648334, -0.000456413694218278, -0.000459386384492911,
    -6.79934297464703E-5, -7.45321876285071E-5, -7.07719714261946E-5,
    -6.7021622856087E-5, -6.41202509155401E-5, -6.90581260872642E-5,
    -0.000468286305595683, -0.000472563291964481, -0.00047528724794386,
    -0.000476303380362562, -4.45024104740224E-5, -5.39852131742483E-5,
    -5.47410691161271E-5, -5.397271514399E-5, -3.12911924612671E-5,
    -5.11716442563843E-5, -5.2995046787198E-5, -5.39709060205489E-5,
    -5.47250171557725E-5, -8.73988287736363E-5, -0.000360957567222821,
    -0.000360886639752586, -0.000361810223038316, -0.000364161537837065,
    -2.82369659250114E-5, -3.36998301195124E-5, -3.34134256867888E-5,
    -3.59297825979492E-5, -3.38756850411402E-5, -6.2660249616301E-5,
    -6.21389059549115E-5, -6.39968426096589E-5, -6.26584823354932E-5,
    -1.33122614385115E-5, -1.16941300486826E-5, -6.15349846521495E-5,
    -6.36087045677498E-5, -6.55202613715272E-5, -6.60936547717754E-5,
    -0.000424003310661467, -0.000432358857144328, -0.00043072163241871,
    -0.000433811164895219, -3.55192933534816E-5, -4.04268368807255E-5,
    -3.71601613953039E-5, -3.15440379300177E-5, -0.000189161971110237,
    -0.000197991622041989, -0.000200918840270814, -0.000202272680934273,
    -5.87155743378537E-5, -5.63133936881538E-5, -5.24792272895535E-5,
    -5.09806059586289E-5, -0.000509362810203761, -0.000526372371004784,
    -0.000527257553741236, -0.000530448398090086, -4.18637809516064E-5,
    -0.000116761283183596, -0.000117080422744925, -0.000115834867450271,
    -2.50987441707083E-5, -2.25763750808984E-5, -2.25097069557187E-5,
    -2.22242031914626E-5, -1.13187340647179E-5, -6.18484151416307E-5,
    -6.79002925912746E-5, -6.93124120196591E-5, -6.97675679805147E-5,
    -0.000196542925700641, -0.000367261067595563, -0.000369252232705225,
    -0.000371820168059298, -5.73111669182691E-5, -6.98800632163171E-5,
    -6.32016124479785E-5, -5.95648922287427E-5, -5.70638725353664E-5,
    -9.42579316611913E-5, -0.000324564090275379, -0.00032695370089749,
    -0.000328395449757041, -0.000328921599782582, -0.000276919281379705,
    -0.000385596657529911, -0.000392169945567135, -0.00039483615755618,
    -0.000396120150578848, -5.38585157628244E-5, -4.86859233704074E-5,
    -4.63414298897862E-5, -4.53531820816268E-5, -8.9948386005274E-5,
    -7.96916959656742E-5, -7.66119147142951E-5, -7.41552259558385E-5,
    -6.20882042474437E-5, -0.000345925145155783, -0.000348986013064666,
    -0.000350429929809041, -0.000352250443051023, -4.21613674150318E-5,
    -5.64721412851063E-5, -5.01084482014051E-5, -4.71044414409125E-5,
    -8.9230018572171E-5, -9.96775491718814E-5, -0.000104703744942332,
    -0.000107284712049367, -0.000116264303103904, -2.21646310486854E-5,
    -0.000148180297736785, -0.000147539156393082, -8.80928470222879E-5,
    -0.000331375084752498, -0.000343655989176864, -0.000346921283250198,
    -0.000347771236109771, -3.20479557109248E-5, -2.48866797031219E-5,
    -2.27805228528504E-5, -1.77306650924467E-5, -0.000110286308346999,
    -0.000115243332103698, -9.88344603941745E-5, -0.000141777572788101,
    -0.000135891371803338, -0.000132278458731989, -0.000370961776981545,
    -0.000390480347741444, -0.00039352573500139, -0.000394864676260981,
    -6.84753458497333E-5, -6.75046203283884E-5, -6.87255222605783E-5,
    -6.98180484236827E-5, -3.65512184439678E-5, -3.50414948608758E-5,
    -3.4527766051565E-5, -8.1934925115964E-5, -8.8485342423852E-5,
    -8.96975360931403E-5, -9.05905544749614E-5, -0.000366463224358458,
    -0.000366591316118615, -0.000368431214668387, -0.000369595710420949,
    -6.33654925909669E-5, -6.44678839089473E-5, -6.49381858980164E-5,
    -6.60639302232802E-5, -4.6366792676196E-5, -4.5401140481312E-5,
    -4.34807677449201E-5, -4.32717080035978E-5, -7.48960130636585E-5,
    -7.87683363397365E-5, -8.15627712340216E-5, -8.2766903112383E-5,
    -0.000116375492631743, -0.000353852969894208, -0.000356656148091832,
    -0.000359518327518356, -0.000359680781142179, -3.3999187084714E-5,
    -4.45139651934162E-5, -4.32560448152651E-5, -4.25309672644836E-5,
    -5.02888715545695E-5, -4.43443682881005E-5, -4.81567580125228E-5,
    -4.5816243908898E-5, -4.20017437727459E-5, -4.25565339839772E-5,
    -4.14535757373381E-5, -4.14085075510383E-5, -6.55421506326737E-5,
    -7.11035486878865E-5, -7.36061165600523E-5, -7.55968307141138E-5,
    -0.000488430426060983, -0.000410709596137187, -0.000411855029333043,
    -0.000413980203279057, -0.000400329489739267, -0.000411842728192319,
    -0.000413679534238508, -0.000416949701572467, -0.000482518002167026,
    -0.000425753725326826, -0.000431117126872979, -0.000427963820389642,
    -0.00043121908942818, -0.00055203131929071, -0.000474379412172358,
    -0.00047680098259947, -0.000479056438171494, -0.000431603619103574,
    -9.26049983927847E-5, -0.000422952829561821, -0.000429470537617553,
    -0.000430895875616568, -0.000434140890260455, -0.000384929946884586,
    -0.000395474181451487, -0.000398950288918884, -0.000403118968947422,
    -0.000373704598938642, -0.000477467807115299, -0.000499473133027585,
    -0.000477311915538667, -0.000478684260237095, -0.000461010582474692,
    -0.00039288967101411, -0.000401345553242819, -0.000418059887818511,
    -0.000423055283188855, -0.000390543711307502, -0.000386177538372865,
    -0.000389439654906253, -0.000391688005967039, -0.000358277924676008,
    -0.000361704430613677, -0.000362944085432606, -0.00036430272622747,
    -0.00016173014457131, -0.00038006201996629, -0.000384040411071947,
    -0.000385395490699619, -0.00010701772281017, -0.000428482200708684,
    -0.000431399604830007, -0.000432957176705259, -0.00043493930485348,
    -0.000470509008806154, -0.000440589948492912, -0.000442807895737014,
    -0.000444184245130404, -0.000409719332868205, -0.000409240474394562,
    -0.000424307926740804, -0.000423859628822192, -0.00042594304102778,
    -0.000452924953256693, -0.000440404142237155, -0.000443284646206166,
    -0.000445207212555847, -0.00037101057920368, -0.000123294350226568,
    -0.000391178337928594, -0.000401053428060088, -0.000405098561579975,
    -0.000405713561014541, -9.22196934119521E-5, -0.000361136043512191,
    -0.000397160742494102, -0.000400323100544755, -0.000401568793150352,
    -0.000405294456898216, -5.58406782066416E-5, -0.000346503237789168,
    -0.000346010186509702, -0.000347898074679293, -0.000348964485018706,
    -0.000192562820128035, -0.000363167267559783, -0.000367871760894816,
    -0.000368426004458956, -0.000104627154348346, -0.000413649523664545,
    -0.000401952594976443, 0.000506203204630212, 0.000653421553649643,
    0.000653851539346269, 0.000653074791500862, 0.0, 0.000172064221145374,
    0.00020037229218825, 0.00020760229886732, 0.000210122488952632,
    4.69338736212048E-5, 4.1905988526177E-5, 0.0, 0.000197755205634661,
    0.000205220187603293, 0.000206733059207471, 0.0, 0.000679380180622912,
    0.00067354377930634, 0.000670360262760216, 0.000667745799490809,
    0.00026488696614303, 0.000205145204418392, 0.000209459926484155,
    0.000212874381598852, 0.000215489003442063, 0.0, 0.000648419038268201,
    0.000644634101816123, 0.000642246513038339, 0.0006413824451123, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00055215936598822, 0.000625261022455498,
    0.000625281092170202, 0.000624497066744164, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    7.36051549319851E-5, 7.36773303421801E-5, 7.19884274779315E-5,
    7.31998694111136E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000572976786639885,
    0.000566416724813911, 0.000567891159566254, 0.000565195772556708,
    0.000164786089793966, 0.000179852765806577, 0.00018249088210625,
    0.000186862230924144, 0.000106268240355691, 9.85518254689806E-5,
    9.60703159917486E-5, 4.69611446851956E-5, 0.000221591533362357,
    0.000223714690580713, 0.000227148339209426, 0.000228529344520473,
    0.000571213672005585, 0.000555171848795276, 0.000554347168091888,
    0.000551512651089181, 0.000135122928400434, 0.000117547817067564,
    0.000117264439106739, 0.000118333208025947, 5.48785901510107E-5,
    5.39862221302272E-5, 5.40445361880657E-5, 5.42765752894477E-5, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.000492313686896242, 0.000534512953070503, 0.000532755126522672,
    0.000530556973138342, 0.00028031842381281, 0.000190312392123071,
    0.000196444240854469, 0.000199706870091094, 0.000201846279470801,
    0.000399819210990882, 0.000478660615759749, 0.000476545807687982,
    0.000475326182083965, 0.0, 0.000525974895193228, 0.000563711432220438,
    0.000557873018980576, 0.000555574845709092, 0.0, 0.000188023347165646,
    0.000192467361270318, 0.000194565215902008, 0.000195415964257309,
    0.000286811962747598, 0.000296253840246635, 0.000298974711407454,
    0.000301110389198279, 0.0, 0.000591162299446557, 0.000588427119997063,
    0.000587149632576167, 0.000585610081125766, 0.00020227027526213,
    0.000221389901241328, 0.000227173960520158, 0.000229850104596555, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.000142231975145607, 0.000272370597985697,
    0.000272969337248048, 0.0, 0.000547242829898251, 0.00053865200677907,
    0.000536005214957568, 0.000535329232200928, 0.00016245157071614,
    0.000169211803444764, 0.000171565348883927, 0.000173675043397824,
    4.71171272810233E-5, 4.0121268196254E-5, 0.000313436234086631,
    0.000334792725045994, 0.000340052379158077, 0.000343216075731401,
    0.000579332910749996, 0.00057071590683091, 0.000568156017721171,
    0.00056697485535909, 0.0, 0.0, 0.0, 0.0, 6.89096169275921E-5,
    7.0281826447603E-5, 7.07501835519501E-5, 0.0, 0.0, 0.0, 0.0,
    0.000573245648197597, 0.000572164381251719, 0.000570625928407816,
    0.000569644250486277, 0.0, 0.0, 0.0, 0.0, 8.51045121145721E-5,
    8.51755243570964E-5, 8.69231018280025E-5, 8.71030422165712E-5, 0.0, 0.0, 0.0,
    0.0, 0.000467500979242849, 0.00052533528048322, 0.000522998217581203,
    0.000520761792731922, 0.0, 0.0, 0.0, 0.0, 0.0, 8.98337427905629E-5,
    9.3507352890508E-5, 9.00448320164183E-5, 9.21206006719604E-5,
    7.29408946087993E-5, 7.23617844024596E-5, 7.33622063902834E-5,
    7.33970732720599E-5, 0.0, 0.0, 0.0, 0.0, 0.000555626565009091,
    0.000551432728712292, 0.000550399700463786, 0.000548510403014007,
    0.000575517713831392, 0.000571579064947186, 0.000569927580257488,
    0.000567073037602756, 0.000570177378604387, 0.000576926411538841,
    0.000572154082255237, 0.000574854708986371, 0.0, 0.00053705471138956,
    0.000611100400925535, 0.000608937776489264, 0.000606976791501694, 0.0,
    0.000468853707003861, 0.000600451305312134, 0.000594405523977957,
    0.000593118706926388, 0.000489354469632413, 0.000602743913077504,
    0.000601834489738073, 0.000598961489569931, 0.000595653094242136,
    0.000371387352686766, 0.000585906334912077, 0.000565629470433097,
    0.000585715796895615, 0.000584514871862233, 0.000542184571692569,
    0.000602483522250745, 0.000602782619960606, 0.000595536761996432,
    0.000500305906842891, 0.000438315419082145, 0.000441607417545678,
    0.000438875552968653, 0.000437053763084372, 0.000654969895538033,
    0.000652071966819221, 0.000651004725802267, 0.000649826325538414,
    0.000584458644010099, 0.000641845152202308, 0.00063829802764445,
    0.000637151127481978, 0.0, 0.00059872226467719, 0.000596180032147412,
    0.00059481519163007, 0.0, 0.000555705010040313, 0.000582130501857259,
    0.000580139707249668, 0.000578936155326826, 0.00053949410416441,
    0.000621998164889805, 0.000617208082834397, 0.000617608406368013,
    0.000615776868906625, 0.000615111829881204, 0.000621146547193474,
    0.000618575293561904, 0.000616904849595831, 0.00042308699774791,
    0.0005255194544779, 0.000553317121580112, 0.000582077862883622,
    0.000579533388103122, 0.000517275196833826, 0.000564624402553722,
    0.000553175356669581, 0.000559187218660414, 0.000557243194178301,
    0.000505310643431957, 0.000502026635735339, 0.0, 0.000481947271577469,
    0.000482408680705821, 0.000480723531489544, 0.00047977986272235,
    0.000530320546313339, 0.000583888411047522, 0.000579655766160469,
    0.000579175894318432, 0.0, 0.000596211193080641, 0.000600746231813899,
    -0.000124509106159094, -0.000497742580578788, -0.000497286948831836,
    -0.000498128233228922, -0.000498739202656245, 0.0, 0.0, 0.0, 0.0,
    -0.000216309569072829, -0.000242563471062339, -5.03400245124804E-5, 0.0, 0.0,
    0.0, -6.42920720359168E-5, -0.000447777623018534, -0.000452785993648334,
    -0.000456413694218278, -0.000459386384492911, -6.79934297464703E-5,
    -7.45321876285071E-5, -7.07719714261946E-5, -6.7021622856087E-5,
    -6.41202509155401E-5, -6.90581260872642E-5, -0.000468286305595683,
    -0.000472563291964481, -0.00047528724794386, -0.000476303380362562, 0.0,
    -5.39852131742483E-5, -5.47410691161271E-5, -5.397271514399E-5, 0.0,
    -5.11716442563843E-5, -5.2995046787198E-5, -5.39709060205489E-5,
    -5.47250171557725E-5, -8.73988287736363E-5, -0.000360957567222821,
    -0.000360886639752586, -0.000361810223038316, -0.000364161537837065, 0.0,
    0.0, 0.0, 0.0, 0.0, -6.2660249616301E-5, -6.21389059549115E-5,
    -6.39968426096589E-5, -6.26584823354932E-5, 0.0, 0.0, -6.15349846521495E-5,
    -6.36087045677498E-5, -6.55202613715272E-5, -6.60936547717754E-5,
    -0.000424003310661467, -0.000432358857144328, -0.00043072163241871,
    -0.000433811164895219, 0.0, 0.0, 0.0, 0.0, -0.000189161971110237,
    -0.000197991622041989, -0.000200918840270814, -0.000202272680934273,
    -5.87155743378537E-5, -5.63133936881538E-5, -5.24792272895535E-5,
    -5.09806059586289E-5, -0.000509362810203761, -0.000526372371004784,
    -0.000527257553741236, -0.000530448398090086, 0.0, -0.000116761283183596,
    -0.000117080422744925, -0.000115834867450271, 0.0, 0.0, 0.0, 0.0, 0.0,
    -6.18484151416307E-5, -6.79002925912746E-5, -6.93124120196591E-5,
    -6.97675679805147E-5, -0.000196542925700641, -0.000367261067595563,
    -0.000369252232705225, -0.000371820168059298, -5.73111669182691E-5,
    -6.98800632163171E-5, -6.32016124479785E-5, -5.95648922287427E-5,
    -5.70638725353664E-5, -9.42579316611913E-5, -0.000324564090275379,
    -0.00032695370089749, -0.000328395449757041, -0.000328921599782582,
    -0.000276919281379705, -0.000385596657529911, -0.000392169945567135,
    -0.00039483615755618, -0.000396120150578848, -5.38585157628244E-5, 0.0, 0.0,
    0.0, -8.9948386005274E-5, -7.96916959656742E-5, -7.66119147142951E-5,
    -7.41552259558385E-5, -6.20882042474437E-5, -0.000345925145155783,
    -0.000348986013064666, -0.000350429929809041, -0.000352250443051023, 0.0,
    -5.64721412851063E-5, -5.01084482014051E-5, 0.0, -8.9230018572171E-5,
    -9.96775491718814E-5, -0.000104703744942332, -0.000107284712049367,
    -0.000116264303103904, 0.0, -0.000148180297736785, -0.000147539156393082,
    -8.80928470222879E-5, -0.000331375084752498, -0.000343655989176864,
    -0.000346921283250198, -0.000347771236109771, 0.0, 0.0, 0.0, 0.0,
    -0.000110286308346999, -0.000115243332103698, -5.57021429074393E-5,
    -0.000141777572788101, -0.000135891371803338, -0.000132278458731989,
    -0.000370961776981545, -0.000390480347741444, -0.00039352573500139,
    -0.000394864676260981, -6.84753458497333E-5, -6.75046203283884E-5,
    -6.87255222605783E-5, -6.98180484236827E-5, 0.0, 0.0, 0.0,
    -8.1934925115964E-5, -8.8485342423852E-5, -8.96975360931403E-5,
    -9.05905544749614E-5, -0.000366463224358458, -0.000366591316118615,
    -0.000368431214668387, -0.000369595710420949, -6.33654925909669E-5,
    -6.44678839089473E-5, -6.49381858980164E-5, -6.60639302232802E-5, 0.0, 0.0,
    0.0, 0.0, -7.48960130636585E-5, -7.87683363397365E-5, -8.15627712340216E-5,
    -8.2766903112383E-5, -0.000116375492631743, -0.000353852969894208,
    -0.000356656148091832, -0.000359518327518356, -0.000359680781142179, 0.0,
    0.0, 0.0, 0.0, -5.02888715545695E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -6.55421506326737E-5, -7.11035486878865E-5, -7.36061165600523E-5,
    -7.55968307141138E-5, -0.000488430426060983, -0.000410709596137187,
    -0.000411855029333043, -0.000413980203279057, -0.000400329489739267,
    -0.000411842728192319, -0.000413679534238508, -0.000416949701572467, 0.0,
    -0.000425753725326826, -0.000431117126872979, -0.000427963820389642,
    -0.00043121908942818, -0.00055203131929071, -0.000474379412172358,
    -0.00047680098259947, -0.000479056438171494, -0.000431603619103574,
    -9.26049983927847E-5, -0.000422952829561821, -0.000429470537617553,
    -0.000430895875616568, -0.000434140890260455, -0.000384929946884586,
    -0.000395474181451487, -0.000398950288918884, -0.000403118968947422,
    -0.000373704598938642, -0.000477467807115299, -0.000499473133027585,
    -0.000477311915538667, -0.000478684260237095, -0.000461010582474692,
    -0.00039288967101411, -0.000401345553242819, -0.000418059887818511,
    -0.000423055283188855, -0.000390543711307502, -0.000386177538372865,
    -0.000389439654906253, -0.000391688005967039, -0.000358277924676008,
    -0.000361704430613677, -0.000362944085432606, -0.00036430272622747,
    -8.22342371352078E-5, -0.00038006201996629, -0.000384040411071947,
    -0.000385395490699619, -0.00010701772281017, -0.000428482200708684,
    -0.000431399604830007, -0.000432957176705259, -0.00043493930485348,
    -0.000470509008806154, -0.000440589948492912, -0.000442807895737014,
    -0.000444184245130404, -0.000409719332868205, -0.000409240474394562,
    -0.000424307926740804, -0.000423859628822192, -0.00042594304102778,
    -0.000452924953256693, -0.000440404142237155, -0.000443284646206166,
    -0.000445207212555847, -0.00037101057920368, -7.79931718300018E-5,
    -0.000391178337928594, -0.000401053428060088, -0.000405098561579975,
    -0.000405713561014541, -9.22196934119521E-5, -0.000361136043512191,
    -0.000397160742494102, -0.000400323100544755, -0.000401568793150352,
    -0.000405294456898216, -5.58406782066416E-5, -0.000346503237789168,
    -0.000346010186509702, -0.000347898074679293, -0.000348964485018706,
    -6.93217068869342E-5, -0.000363167267559783, -0.000367871760894816,
    -0.000368426004458956, -0.000104627154348346, -0.000413649523664545,
    -0.000401952594976443, 0.000630712310789306, 0.00115116413422843,
    0.0011511384881781, 0.00115120302472978, 0.000648140770482576,
    0.000217145012222543, 0.000236396529061304, 0.000237020349960573,
    0.000237280639603991, 0.000263243442694034, 0.000284469459588516,
    0.000285990927762276, 0.000231113215721057, 0.000239335784484836,
    0.000238550659248526, 0.000432427229144082, 0.00112715780364145,
    0.00112632977295467, 0.00112677395697849, 0.00112713218398372,
    0.0003328803958895, 0.000279677392046899, 0.00028023189791035,
    0.000279896004454939, 0.000279609254357603, 0.000465895067402287,
    0.00111670534386388, 0.0011171973937806, 0.0011175337609822,
    0.00111768582547486, 5.50726241200954E-5, 6.91952192929724E-5,
    6.91067414455883E-5, 6.92218397595588E-5, 4.16622923541159E-5,
    6.15981281275571E-5, 6.43018241369488E-5, 6.43114768467311E-5,
    7.09385689931871E-5, 0.000639558194761856, 0.000986218589678319,
    0.000986167731922787, 0.000986307289782479, 0.000381081388093806,
    3.68333770222259E-5, 4.86499753257307E-5, 4.93369089375908E-5,
    4.84303851055301E-5, 4.93480770526773E-5, 0.000136265404548286,
    0.000135816236297092, 0.00013598527008759, 0.000135858351746607,
    4.27753849748277E-5, 3.17870654387504E-5, 8.12860216774232E-5,
    8.09653271348554E-5, 8.71111531920239E-5, 8.48477572554954E-5,
    0.000996980097301352, 0.000998775581958239, 0.000998612791984964,
    0.000999006937451927, 0.000200305383147448, 0.000220279602687303,
    0.000219651043501554, 0.000218406268854162, 0.000295430211465928,
    0.00029654344751097, 0.000296989156262563, 0.000249233825619468,
    0.00028030710770021, 0.000280028084268867, 0.000279627566498979,
    0.000279509950479102, 0.00108057648220935, 0.00108154421980006,
    0.00108160472183312, 0.00108196104917927, 0.000176986709352041,
    0.00023430910025116, 0.000234344861851664, 0.000234168075476219,
    7.9977334321719E-5, 7.65625972111257E-5, 7.65542431437843E-5,
    7.65007784809104E-5, 4.55853830536281E-5, 7.33831443772515E-5,
    8.10733455162785E-5, 8.08683774981951E-5, 8.06770093753549E-5,
    0.000688856612596883, 0.000901774020666065, 0.000902007359227897,
    0.00090237714119764, 0.000337629590731079, 0.000260192455339388,
    0.000259645853302448, 0.000259271762319837, 0.000258910152006168,
    0.000494077142652073, 0.000803224706035128, 0.000803499508585472,
    0.000803721631841006, 0.000636633815428856, 0.000802894176572933,
    0.000949308089750349, 0.000950042964547711, 0.000950411003265272,
    0.000441462539684363, 0.00024188186292847, 0.000241153284640725,
    0.000240906645791794, 0.000240769146338936, 0.000376760348752872,
    0.000375945536212309, 0.000375586626121749, 0.000375265615154118,
    0.000394291031598129, 0.00093708744460234, 0.00093741313306173,
    0.000937579562385208, 0.000937860524176789, 0.000244431642677161,
    0.000277862042526435, 0.000277282408721564, 0.000276954546037468,
    0.00011394150244712, 0.00012796999756981, 0.000138059132838521,
    0.00013625446321969, 0.00014712202999183, 0.000164396606194292,
    0.000420550895722482, 0.00042050849364113, 0.000575744092034365,
    0.000878617914650748, 0.000882307995955934, 0.000882926498207765,
    0.000883100468310699, 0.000194499526427065, 0.000194098483147886,
    0.000194345871736778, 0.000191405708490271, 0.000157403435628023,
    0.000155364600299952, 0.000412270694480805, 0.000476570297834095,
    0.000475943750961414, 0.00047549453446339, 0.00095029468773154,
    0.000961196254572354, 0.000961681752722561, 0.000961839531620071,
    9.16844911263796E-5, 9.26574982411767E-5, 9.24652768070818E-5,
    9.22696016501201E-5, 0.00010546083537156, 0.000105323321308479,
    0.000105277949603515, 0.000100278316065772, 0.000115357520590805,
    0.000116088756501201, 0.000115632417167524, 0.000939708872556055,
    0.000938755697370335, 0.000939057143076203, 0.000939239960907226,
    7.98169224878405E-5, 8.10678796617838E-5, 8.09446599683038E-5,
    8.06088072639847E-5, 0.000131471304790768, 0.000130576664838408,
    0.000130403869572923, 0.000130374750220169, 0.000109418228506882,
    0.00010151777402142, 0.000112777934849941, 0.000112115468036268,
    0.000583876471874592, 0.000879188250377428, 0.000879654365673035,
    0.000880280120250278, 0.000373467794376583, 5.2754471535193E-5,
    5.62746647331119E-5, 6.36116055250681E-5, 5.61083054639768E-5,
    0.000140122614345132, 0.000137851721178609, 0.000138201590028941,
    0.000137936844580858, 0.000114942638381545, 0.000114918318386437,
    0.000114815782127622, 0.000114805580823098, 7.66153869909017E-5,
    8.82308342456618E-5, 8.78700685685971E-5, 9.65643617053105E-5,
    0.00104405699107007, 0.000962142324849479, 0.000962254729796829,
    0.000962490606293064, 0.000975847203570659, 0.000983421793139505,
    0.000983607114495996, 0.000984022739175223, 0.00105269538077141,
    0.00100268013686567, 0.00100327120912822, 0.00100281852937601,
    0.000471840413455837, 0.00112120021323884, 0.00108547981309789,
    0.00108573875908873, 0.00108603322967319, 0.000946371090811081,
    0.000561458705396646, 0.00102340413487395, 0.00102387606159551,
    0.00102401458254296, 0.000923495359892868, 0.000987673859962091,
    0.00099730867118956, 0.000997911778488815, 0.000998772063189558,
    0.000745091951625408, 0.00106337414202738, 0.00106510260346068,
    0.00106302771243428, 0.00106319913209933, 0.00100319515416726,
    0.000995373193264855, 0.00100412817320343, 0.00101359664981494,
    0.000923361190031746, 0.000828859130389647, 0.000827784955918543,
    0.000828315207874906, 0.000828741769051411, 0.00101324782021404,
    0.0010137763974329, 0.00101394881123487, 0.00101412905176588,
    0.000746188788581409, 0.0010219071721686, 0.0010223384387164,
    0.0010225466181816, 0.000721121873750903, 0.00102720446538587,
    0.00102757963697742, 0.00102777236833533, 0.00102336318097419,
    0.00102621401884647, 0.00102272045035017, 0.00102294760298668,
    0.00102312040045723, 0.000949213437032615, 0.00103123863928437,
    0.0010415160095752, 0.0010414680351902, 0.00104171990993441,
    0.0010680367831379, 0.00106155068943063, 0.00106185993976807,
    0.00106211206215168, 0.00079409757695159, 0.000648813804704468,
    0.000944495459508706, 0.000983131290943711, 0.000984631949683098,
    0.000922988757848367, 0.000656844095965674, 0.000914311400181772,
    0.000956347961154516, 0.000957566294723056, 0.000920375142555698,
    0.000907321092633555, 0.000402982221890365, 0.000828450509366638,
    0.000828418867215523, 0.000828621606168837, 0.000828744347741056,
    0.000722883366441374, 0.000947055678607305, 0.000947527527055284,
    0.000947601898777388, 0.000710537041955994, 0.00100986071674519,
    0.00100269882679034, 0.000126222697665877, 0.00017599932709708,
    0.00017603514051521, 0.000176059690555005, 0.000106713503581055,
    4.09926342956121E-5, 4.41336290504611E-5, 4.54321388837292E-5,
    4.58446525073635E-5, 5.01325947045224E-5, 5.37942925573204E-5,
    4.46532229955164E-5, 4.31841939499256E-5, 4.51701076698597E-5,
    4.56108046180143E-5, 6.41736603628044E-5, 0.000194170446428045,
    0.000193949456553129, 0.000193969635181033, 0.000181433914927352,
    6.40470070015531E-5, 4.585458948469E-5, 4.80190172336491E-5,
    4.83847848583206E-5, 4.91205363218144E-5, 6.83719516739661E-5,
    0.000182651789057002, 0.000182778839905769, 0.000182913949424567,
    0.000167087142185008, 1.2114180450432E-5, 1.36458297878586E-5,
    1.37758626585314E-5, 1.38166504337861E-5, 6.91860310314227E-6,
    1.28488009836832E-5, 1.327392864073E-5, 1.33962991955848E-5,
    1.15940813072377E-5, 0.000123949067180044, 0.00017127627018674,
    0.000171287704587701, 0.000171338792742842, 9.44713462316351E-5,
    6.96606326164944E-6, 1.02962563444529E-5, 1.00496298351518E-5,
    9.63382587355669E-6, 8.5799500045121E-6, 2.02351973992E-5,
    2.17659783502549E-5, 2.19224282231744E-5, 2.17472852240796E-5,
    7.94555421235263E-6, 6.9667092766014E-6, 1.61447731800691E-5,
    1.6606878277397E-5, 1.69784025305824E-5, 1.70949853165245E-5,
    0.000158723393219517, 0.000159377561444774, 0.000159261575329062,
    0.000159308150633799, 3.9613988578177E-5, 4.233731001795E-5,
    4.30260962865431E-5, 4.31985897841356E-5, 5.34676283508455E-5,
    5.61952915614785E-5, 5.65256588991435E-5, 5.17919978524534E-5,
    5.07647328991687E-5, 5.1341296744339E-5, 5.18251874288191E-5,
    4.8869063552577E-5, 0.000172847833631409, 0.000174284239059423,
    0.000176957124432127, 0.000176706967667423, 3.17040031723678E-5,
    3.64132988733581E-5, 3.64451931568737E-5, 3.64210417385055E-5,
    1.33693389542413E-5, 1.3124923042843E-5, 1.30082775304737E-5,
    1.31385948649914E-5, 8.91896820955341E-6, 1.4567749415091E-5,
    1.60427003246693E-5, 1.62341391749446E-5, 1.62973779552823E-5,
    0.000115005129460707, 0.000153921430613057, 0.000153977751219207,
    0.00015414034806313, 5.47908482732203E-5, 4.62303160355682E-5,
    4.73184759231031E-5, 4.78411855526845E-5, 4.77303567508681E-5,
    0.000100492490550057, 0.000137102033290532, 0.000137209250427729,
    0.000137258470005141, 8.84481566263571E-5, 0.000123728181474774,
    0.000159410283001533, 0.000159720339909354, 0.000159860050720678,
    8.96664212517747E-5, 4.43868838909889E-5, 4.52627028769896E-5,
    4.55620675366117E-5, 4.12608562957859E-5, 6.88932381268453E-5,
    7.07087864278818E-5, 7.08648912462282E-5, 7.05596719789186E-5,
    5.69591810364295E-5, 0.000162593035806385, 0.000162759795309876,
    0.000162832827534241, 0.000151248142692182, 4.72985153962925E-5,
    5.20482933596809E-5, 5.29700567967108E-5, 5.31793169299577E-5,
    2.69737395875788E-5, 2.99944326267102E-5, 3.2177675953578E-5,
    3.28971597376883E-5, 3.29137782542685E-5, 3.06519349257369E-5,
    6.88998913826616E-5, 6.89297004733922E-5, 8.69575551665489E-5,
    0.000155269521181394, 0.000154994405004706, 0.00015519732169812,
    0.000133416177771627, 3.73113148165876E-5, 3.85135339135051E-5,
    3.88446677598336E-5, 3.79873198866404E-5, 3.60917479558624E-5,
    3.71699357342353E-5, 7.34964264813493E-5, 8.48489831581339E-5,
    8.54603757119568E-5, 8.56397903382755E-5, 0.000169179549551521,
    0.000169973397743795, 0.000170153079732604, 0.000170165634035078,
    1.84211370266319E-5, 1.84484425620865E-5, 1.83829438917078E-5,
    1.86044128258958E-5, 1.69270069318987E-5, 1.69010700583061E-5,
    1.65697441423856E-5, 1.9630275188034E-5, 2.21171078211038E-5,
    2.2498782723166E-5, 2.25468686768109E-5, 0.000167331883347019,
    0.000168025771130524, 0.000168117931184687, 0.000158472082433531,
    1.55739740088527E-5, 1.59348952545504E-5, 1.58753733593442E-5,
    1.57680244915957E-5, 2.14087353070463E-5, 2.14302249940366E-5,
    2.15568160477053E-5, 2.14438512140751E-5, 1.85836698705656E-5,
    1.99306218040562E-5, 2.13952669200538E-5, 2.16133354568108E-5,
    0.000114385760319198, 0.000162767950973273, 0.000162841565274165,
    0.000163047665003989, 9.6640155589296E-5, 1.04726295397612E-5,
    1.22403495996527E-5, 1.27032906152003E-5, 1.23939858347771E-5,
    2.15193060127537E-5, 2.33250786483573E-5, 2.40671636500194E-5,
    2.43221020298916E-5, 1.8435693300263E-5, 1.86429584269173E-5,
    1.86498839892108E-5, 1.83539857791796E-5, 1.66396587246813E-5,
    1.886208198862E-5, 1.92336800945168E-5, 1.99948726088816E-5,
    0.000142081442462351, 0.000148811054597141, 0.000148861406758757,
    0.000148985826702551, 0.000156524183572205, 0.000157596595315441,
    0.000157646664899863, 0.000157878024451965, 0.000141360440392607,
    0.000158237779862574, 0.000158398568286632, 0.000158313333802466,
    8.14554932673694E-5, 0.000198705277419446, 0.000226873977824012,
    0.000226960985087132, 0.000226838506233749, 0.000120753313252274,
    0.000109324847558346, 0.000208415139124563, 0.000210307146590587,
    0.00021038493923595, 0.000164823590989752, 0.000175332245238554,
    0.000209080033392698, 0.000209313881440534, 0.000209366541557259,
    0.000116613132814332, 0.000164390457182429, 0.000213170919564738,
    0.000237912083923322, 0.000238015289471273, 0.000172532616389297,
    0.000172011092611892, 0.000228387711543502, 0.000228809371117779,
    0.000153616467967166, 0.000154764542695675, 0.000154983262420094,
    0.000155178036097078, 0.000145052325622587, 0.00018067506703142,
    0.000181538078293608, 0.00018159841031301, 0.000177764573146615,
    0.000132447346172471, 0.000177119369417705, 0.000177253746075623,
    0.000177305414721337, 0.000112194336827415, 0.000170853055606108,
    0.000170940785409946, 0.000171009704525524, 0.000138426308405755,
    0.000162671368977704, 0.000222976793554749, 0.000223085293300185,
    0.00021993547010657, 0.000150725351414999, 0.000165071974039727,
    0.000166415764583016, 0.000166406248966728, 0.000166508145059307,
    0.00017097500129139, 0.000210494320388255, 0.000210580329544241,
    0.00021047458706054, 0.000122902923467912, 0.000121195856967477,
    0.000191291009269225, 0.000214790012528472, 0.000214949529016287,
    0.000165386453238781, 0.000115738241222968, 0.000162175504069181,
    0.000213979291887359, 0.000214181544379125, 0.000184021216551553,
    0.000139383358375129, 5.97844004401527E-5, 0.000135353056843735,
    0.000135463396076237, 0.000135539857062548, 0.000124130132892834,
    0.000121114463300099, 0.000162119606427706, 0.00016229086457633,
    0.000162306020401572, 0.000118009384481765, 0.000177417105696682,
    0.000209541394541916, 0.000947119634676665, 0.00134562932146931,
    0.00134900479591892, 0.00134288436571739, 0.000101695117964746,
    0.00245564427763483, 0.00263814647813374, 0.00288175910085282,
    0.00293217266725261, 0.000272093169177141, 8.30174923417011E-5,
    0.00203035577864265, 0.00270384363350812, 0.00295034979201318,
    0.0029820467343358, 0.00140238520790779, 0.00179644461657677,
    0.00173792916709794, 0.00170681193926884, 0.00055051894425065,
    0.00465988229710005, 0.00275650320157589, 0.00314898503530393,
    0.00328166920449582, 0.00344683097027668, 0.00108214875875808,
    0.00173975690816934, 0.00162532399226504, 0.00160455274320556,
    0.000373151203505721, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.00206241590816608, 0.00191413584930139, 0.0019143288932464,
    0.00190579008401398, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000817890930100923,
    0.000819908596259604, 0.000798128656747031, 0.000813745041371966, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.00193000142866649, 0.00181485530460044,
    0.00182931414192533, 0.00180305134951495, 0.0024996753213882,
    0.00269101879788563, 0.00274629606120319, 0.00301189824254951,
    0.0016362302870249, 0.0019518235141236, 0.0017358694200927,
    0.000357233452937317, 0.00327246055956537, 0.00331644019256902,
    0.00344129862786286, 0.00276018060563476, 0.00207391954489727,
    0.00254597382866547, 0.00425371050709735, 0.00406729369410308,
    0.00227464907112443, 0.00145471895435403, 0.00145021229506736,
    0.0015122326080142, 0.000499984524138803, 0.000448680393974168,
    0.000491840402968088, 0.000494160873765115, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.00241380417376481, 0.00244399708202278, 0.00242141227725373,
    0.00239325487545388, 0.00285147063189008, 0.00292501459631944,
    0.00315721381523464, 0.00323208800763513, 0.00328112027281264,
    0.00152727375676527, 0.00242621542116463, 0.00239898483088129,
    0.00238334261851318, 0.000944795616791782, 0.00174348791888163,
    0.00230509585185912, 0.00223628732841104, 0.00220932227277572,
    8.78411396258693E-5, 0.00282684442571881, 0.00291950027876245,
    0.00301241107205556, 0.00221298840926219, 0.00460160823596596,
    0.00482820100825615, 0.00489352445265081, 0.0047809200110557,
    0.00174110410434332, 0.00177818608797084, 0.00175136020948689,
    0.00173879565450623, 0.00052559867020701, 0.00305252751006402,
    0.00342673286181991, 0.00360469025584953, 0.00366608657477434, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.00199548768675762, 0.00411703020984294, 0.00413020931917847,
    0.000966953979193688, 0.00269104618912874, 0.00251668634016921,
    0.00247981475263432, 0.000977296295481003, 0.00225138311857112,
    0.00248327051937234, 0.00253284225211115, 0.00251902846191918,
    0.00035952510331615, 0.0, 0.00496193082901648, 0.00432009442465242,
    0.00410374678815994, 0.00389959559770517, 0.00225738698578949,
    0.00209991342386705, 0.00239416691754711, 0.00237880962181185, 0.0, 0.0, 0.0,
    0.0, 0.000718513820079563, 0.000779574888642559, 0.000785649460558359, 0.0,
    0.0, 0.0, 0.0, 0.00267006567998594, 0.00260404448624794, 0.00258267953041862,
    0.00105620546149985, 0.0, 0.0, 0.0, 0.0, 0.00104964032023035,
    0.00104932525849012, 0.00111950376012732, 0.00112235930969509, 0.0, 0.0, 0.0,
    0.0, 0.00276592877597011, 0.00318797916778109, 0.00314323681121022,
    0.00300380734055605, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00117028183625443,
    0.00122787509575836, 0.00117246510565752, 0.00120568336844133,
    0.000811332570089794, 0.000759481388128738, 0.000860034545857356,
    0.000860515391687986, 0.0, 0.0, 0.0, 0.0, 0.00185612759227709,
    0.00183160757538857, 0.00182157549656762, 0.00212784878090466,
    0.00201849790814366, 0.00198280304454086, 0.00191366399784969,
    0.00194426976896474, 0.00180244681484702, 0.00192200215103022,
    0.00187544897643572, 0.00190154569404422, 0.0, 0.00248869562827593,
    0.00354315107750696, 0.00349786761067673, 0.00314934792921398,
    0.000581410005656843, 0.00257934149082, 0.0034887386390339,
    0.00330512378213642, 0.00328012861947915, 0.00145016555615801,
    0.00271004864003255, 0.00496858832604752, 0.00519835125875165,
    0.00505639678901351, 0.00272233418518258, 0.00205258904346229,
    0.00457249117396456, 0.00475838413587597, 0.004713605939536,
    0.00285772565695272, 0.00219576516026775, 0.00422223987716547,
    0.00435599316375527, 0.00211606693864686, 0.00530504047563161,
    0.00541277269292126, 0.00532316318480489, 0.00147343041212164,
    0.00204708792073515, 0.00201074350977812, 0.00199795125039161,
    0.000778167089418976, 0.00163190828964946, 0.00195516967204187,
    0.00191654855218837, 0.00190407398815377, 0.000787711360159441,
    0.00188858315801495, 0.00186094615673563, 0.00184616011170602,
    0.000610874497485755, 0.00305445232682028, 0.00416832248498093,
    0.00411690914882737, 0.00280170642908023, 0.000776543795537728,
    0.00169711701175325, 0.00198173382806511, 0.00198565345052789,
    0.00196777615697332, 0.00153529258681003, 0.00404945151961161,
    0.00394181623116943, 0.00363108266626845, 0.00197131298382303,
    0.00230292438198989, 0.00401087971215019, 0.00392321343586729,
    0.00385834786988589, 0.00149586872420909, 0.00109149190360933,
    0.00239045923357305, 0.00427453593214471, 0.00421984572098431,
    0.00278280165833792, 0.00133731809615382, 0.00164521259629606,
    0.00233579695084287, 0.00234157482484028, 0.00232011985494523,
    0.000705662124990633, 0.00180584182136976, 0.00198307909320314,
    0.00193709507256201, 0.00193189169663525, 0.000817885350614243,
    0.00299358124912435, 0.00554318044577948, 0.0086903572511514,
    0.00158680962543647, 0.00158070498421092, 0.00159041680113863,
    0.00159767376175334, 0.0, 0.0, 0.0, 0.0, 0.00327496298719443,
    0.00366161934028134, 5.03241202071529E-5, 0.0, 0.0, 0.0,
    0.000651844348916829, 0.00319081941998191, 0.00286955180128358,
    0.00272653480853107, 0.00284636361732808, 0.000564519922727691,
    0.000672065532819839, 0.000459407390571709, 0.000433078553222337,
    0.00036106400309207, 0.000835883064201482, 0.0020626829228181,
    0.00217846862498844, 0.00222188705191008, 0.00229063211781924, 0.0,
    0.000265621680080283, 0.00032060456081872, 0.000265559572698962, 0.0,
    0.000152746478250094, 0.000261290599292337, 0.000266183937012604,
    0.000321357856961386, 0.00264783692128925, 0.00342104488739447,
    0.00342606251899979, 0.00344705128309306, 0.00315424282943445, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.000414082500532437, 0.000411520467680391, 0.000479049890900103,
    0.000468342607916906, 0.0, 0.0, 0.00058333352394741, 0.000656696770616768,
    0.000731533179544752, 0.000738415160939945, 0.00251965121314406,
    0.0025136973155623, 0.002431305437121, 0.00259400539986433, 0.0, 0.0, 0.0,
    0.0, 0.00360160283143131, 0.00395303032471682, 0.00409053091605072,
    0.00412994482633213, 0.000392968270285548, 0.000274059209307146,
    0.000104322539504925, 0.00010132527848002, 0.00187817208226065,
    0.00198580449757057, 0.00199766764405703, 0.0020410736218963, 0.0,
    0.00165425227845355, 0.00166006937313674, 0.00163731556686683, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.000469258682091568, 0.000629589378852584, 0.000696546430767225,
    0.000701539064241042, 0.00185446467012967, 0.00293212142263144,
    0.0030240164446877, 0.00307506743992569, 0.000384011977575581,
    0.000640749640068092, 0.000475007920325823, 0.000396218253609057,
    0.000329102744860731, 0.00575864598466355, 0.00398259106158945,
    0.00410098142374338, 0.0041441641456834, 0.00415886280086131,
    0.0019094301966955, 0.00315845769314325, 0.00307977163234053,
    0.00326970492090778, 0.00243591867238782, 0.000313749696449121, 0.0, 0.0,
    0.0, 0.00134490570501758, 0.000916874926917967, 0.000876521718945319,
    0.000794236073484968, 0.000580847329633314, 0.00356756362180745,
    0.00364471364360702, 0.00373193970790802, 0.00377620133422921, 0.0,
    0.000273275211192244, 0.0, 0.0, 0.00150055536165254, 0.0018769372785944,
    0.00236100510788803, 0.00264528823240596, 0.00216905359190361, 0.0,
    0.00205684602059592, 0.00204522559400896, 0.00372708395842632,
    0.00469904917806662, 0.00445368891742752, 0.00433044273028892,
    0.00435662806660737, 0.0, 0.0, 0.0, 0.0, 0.00301489158007759,
    0.00323453536636099, 0.000588793579952065, 0.00237787220079052,
    0.00208396436605678, 0.00195856812314934, 0.00247672481168602,
    0.00381257786554212, 0.00368267744685045, 0.0037175045987133,
    0.000813452179458003, 0.000749180911357338, 0.000816887659363117,
    0.000831132829892927, 0.0, 0.0, 0.0, 0.000946740706964466,
    0.00115139411349826, 0.00122383892091465, 0.00123822289789749,
    0.00321150046285774, 0.00392301443137475, 0.00374812030616973,
    0.00377875793857043, 0.000589975306889592, 0.000600965751012125,
    0.000605669012823086, 0.000616927398512269, 0.0, 0.0, 0.0, 0.0,
    0.000881182753235383, 0.00104184624975356, 0.00108366445928025,
    0.001154691341191, 0.00170302410878816, 0.00435336670989879,
    0.00449029665867317, 0.00429646022324424, 0.00403756052593635, 0.0, 0.0, 0.0,
    0.0, 5.02687247784343E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.000728374935481278, 0.000906173692601975, 0.000995532376059937,
    0.00102548123337373, 0.000485089503686609, 0.00258572055996329,
    0.00266165945669943, 0.00270951800775298, 0.00211966236007732,
    0.00216384742977607, 0.0022466065578355, 0.00235458136832888,
    0.000318403602627204, 0.0022195461207994, 0.00236073181250594,
    0.00225405223547318, 0.00160800885568761, 0.00410803176701061,
    0.00372209284246727, 0.00384156310983537, 0.00396625938476821,
    0.00252695681266233, 0.00461924279793057, 0.00197355946150312,
    0.00354230590775952, 0.00368943461343023, 0.00401702534238902,
    0.00169996328340742, 0.00501614970976693, 0.0053089855549776,
    0.00525727521899152, 0.0035296722600892, 0.001883187669371,
    0.00340337254578846, 0.00463296428284019, 0.00469240582249811,
    0.00277381622815061, 0.000876536182255533, 0.00483515743294632,
    0.00463361227559098, 0.00359158877075262, 0.00334997964614293,
    0.00329526896132061, 0.00342365228127417, 0.00347844079965957,
    0.00377564084521113, 0.00395384782924559, 0.00398684358936357,
    0.00407618711646715, 0.00198781631507211, 0.00334046335408711,
    0.00343581260975266, 0.0035219199310869, 0.00652963768524941,
    0.00267914446565486, 0.00290037748703504, 0.0029882605528923,
    0.00281303374568895, 0.00247815466136547, 0.00481511976989447,
    0.00497476358200407, 0.00503523489847412, 0.00242526303282131,
    0.00206717848639548, 0.00237289597743863, 0.00236495732532737,
    0.00245548270086552, 0.00175946181776219, 0.00428261687465586,
    0.00444112591208571, 0.00451089320372203, 0.00241804231634321,
    0.00149359600295767, 0.00453251464852827, 0.00553504924551655,
    0.00571391189297009, 0.00538951133706004, 0.00803579675114314,
    0.00349194661913253, 0.00612880006445431, 0.0062280764278186,
    0.00626841120237456, 0.00203255967000895, 0.000271370645450202,
    0.00317870895442544, 0.00316783985848088, 0.00320938025643892,
    0.00323294465334207, 0.00148264611696379, 0.00344235664476575,
    0.00366658680620814, 0.00368150324364934, 0.00821095462896874,
    0.00293844927803297, 0.00473562833891576, 2.86687413995139E-5,
    -5.75582067114215E-5, -5.75569244089052E-5, -5.75601512364892E-5,
    -5.89218882256888E-5, 1.08572506111272E-5, 1.24419225821739E-5,
    1.24747552610828E-5, 1.24884547159995E-5, -1.31621721347017E-5,
    1.42234729794258E-5, 1.24343881635772E-5, -1.35948950424151E-5,
    -2.75098602856133E-6, -2.74196160055778E-6, 1.96557831429128E-5,
    -4.17465853200536E-5, -4.17159175168397E-5, -4.17323687769812E-5,
    -4.17456364438415E-5, -1.51309270858864E-5, -1.47198627393105E-5,
    -1.55684387727972E-5, -1.55497780252744E-5, -1.55338474643113E-5,
    2.21854794001089E-5, -4.65293893276618E-5, -4.65498914075252E-5,
    -4.65639067075916E-5, -4.65702427281192E-5, -2.62250591048073E-6,
    2.66135458819125E-6, 2.65795159406109E-6, 2.66237845229072E-6,
    -3.65458704860666E-7, 1.04403606995859E-6, 5.5432607014611E-7,
    5.54409283161475E-7, 2.87200684182944E-7, 3.0455152131517E-5,
    -3.1813502892849E-5, -3.18118623200899E-5, -3.18163641865316E-5,
    1.61475164446528E-6, -2.40741026289058E-7, 1.24743526476232E-6,
    1.26504894711771E-6, 1.24180474629564E-6, 1.26533530904301E-6,
    -8.01561203225212E-6, -7.54534646094953E-6, -7.55473722708835E-6,
    -7.54768620814482E-6, 2.44430771284729E-7, 2.09125430518095E-7,
    3.25144086709693E-6, 3.37355529728564E-6, 5.4786888800015E-7,
    3.83926503418531E-7, -4.53172771500614E-5, -4.53988900890109E-5,
    -4.53914905447711E-5, -4.54094062478149E-5, 9.53835157844989E-6,
    -4.68680005717665E-6, -4.67342645747987E-6, -4.64694189051408E-6,
    -1.01872486712389E-5, -1.02256361210679E-5, -1.02410053883642E-5,
    3.27939244236142E-6, -1.33479575095338E-5, -1.33346706794698E-5,
    -1.33155984047133E-5, -1.3309997641862E-5, -4.6981586183015E-5,
    -4.70236617304374E-5, -4.70262922536141E-5, -4.70417847469247E-5,
    -9.77827123491937E-7, -1.23320579079558E-5, -1.2333940097456E-5,
    -1.23246355513799E-5, -1.7772740960382E-6, -4.25347762284031E-6,
    -4.25301350798802E-6, -4.25004324893947E-6, 1.51951276845427E-6,
    -3.66915721886257E-6, 1.22838402297392E-6, 1.20699070892829E-6,
    1.20413446828888E-6, -3.28026958379468E-5, -3.33990378024469E-5,
    -3.34076799714036E-5, -3.34213755999126E-5, -1.46795474230904E-5,
    -1.1826929788154E-5, -1.18020842410203E-5, -1.17850801054471E-5,
    -1.17686432730076E-5, 1.97630857060829E-5, -3.0893257924428E-5,
    -3.09038272532874E-5, -3.09123704554233E-5, -3.35070429173082E-5,
    -3.82330560272825E-5, -3.7972323590014E-5, -3.80017185819084E-5,
    -3.80164401306109E-5, 1.80188791707903E-6, -1.15181839489748E-5,
    -1.20576642320363E-5, -1.20453322895897E-5, -1.20384573169468E-5,
    -1.63808847283857E-5, -1.63454580961874E-5, -1.63298533096413E-5,
    -1.63158963110486E-5, 1.87757634094347E-5, -3.47069423926793E-5,
    -3.47190049282122E-5, -3.47251689772299E-5, -3.47355749695107E-5,
    1.16396020322458E-5, -1.32315258345921E-5, -1.32039242248364E-5,
    -1.31883117160699E-5, 3.45277280142786E-6, 1.347052605998E-6,
    1.26659754897725E-6, 8.20810019395724E-7, 3.06504229149646E-6,
    -8.65245295759432E-6, -1.91159498055674E-5, -1.91140224382332E-5,
    2.61701860015621E-5, -2.25286644782243E-5, -2.2623281947588E-5,
    -2.26391409796863E-5, -2.26436017515564E-5, -4.98716734428373E-6,
    9.70492415739428E-6, 9.71729358683889E-6, -4.90783867923771E-6,
    2.38490053981852E-6, 2.35400909545382E-6, -2.16984576042529E-5,
    -1.98570957430873E-5, -1.98309896233923E-5, -2.06736754114518E-5,
    -2.87968087191376E-5, -2.91271592294653E-5, -2.91418712946231E-5,
    -2.91466524733355E-5, 3.05614970421265E-6, 3.08858327470589E-6,
    3.08217589356939E-6, 3.07565338833734E-6, -5.55057028271368E-6,
    -5.54333270044625E-6, -5.54094471597448E-6, 4.01113264263086E-6,
    1.6021877859834E-6, 1.61234384029446E-6, 1.60600579399339E-6,
    -2.93659022673767E-5, -2.9336115542823E-5, -2.93455357211313E-5,
    -2.93512487783508E-5, 1.99542306219601E-6, 1.97726535760448E-6,
    1.97425999922692E-6, 1.96606846985328E-6, -6.91954235740885E-6,
    -6.87245604412676E-6, -6.86336155646961E-6, -6.86182895895626E-6,
    4.97355584122192E-6, 4.61444427370092E-6, 1.01601743108055E-6,
    1.010049261588E-6, -2.16250545138738E-5, -2.0933053580415E-5,
    -2.09441515636437E-5, -2.09590504821495E-5, 1.68989952206599E-6,
    -3.10320420795253E-6, -3.51716654581949E-6, 4.78282748308783E-7,
    -3.50676909149855E-6, -6.67250544500631E-6, -6.56436767517183E-6,
    -6.58102809661625E-6, -6.56842117051707E-6, -6.38570213230807E-6,
    -6.38435102146871E-6, -6.37865456264564E-6, -6.37808782350545E-6,
    -1.50226249001768E-6, 1.73001635775807E-6, 1.72294252095288E-6,
    5.74787867293515E-7, -4.97169995747654E-5, -4.81071162424739E-5,
    -4.81127364898415E-5, -4.81245303146532E-5, -4.43566910713936E-5,
    -4.47009905972502E-5, -4.47094142952726E-5, -4.47283063261465E-5,
    -5.01283514653054E-5, -4.77466731840794E-5, -4.7774819482296E-5,
    -4.77532633036197E-5, 1.90258231232192E-6, 4.00428647585299E-5,
    -4.71947744825171E-5, -4.7206033003858E-5, -4.72188360727473E-5,
    -4.73185545405541E-5, 2.67361288284117E-5, -5.11702067436977E-5,
    -5.11938030797755E-5, -5.12007291271478E-5, 4.61747679946434E-5,
    -2.53249707682587E-5, -2.55720172099887E-5, -2.55874814997132E-5,
    -2.56095400817835E-5, -3.92153658750215E-5, -5.06368639060655E-5,
    -5.07191715933658E-5, -5.06203672587754E-5, -5.0628530099968E-5,
    -2.50798788541815E-5, -3.5549042616602E-5, -3.58617204715509E-5,
    -1.16505362047695E-5, -3.84733829179894E-5, -2.43782097173425E-5,
    -2.43466163505454E-5, -2.43622119963208E-5, -2.43747579132768E-5,
    -2.89499377204012E-5, -2.81604554842472E-5, -2.81652447565242E-5,
    -2.81702514379412E-5, -3.73094394290704E-5, -3.19345991302687E-5,
    -3.19480762098874E-5, -3.19545818181749E-5, 3.13531249456914E-5,
    -4.1088178615435E-5, -4.11031854790968E-5, -4.11108947334132E-5,
    -4.26401325405912E-5, -4.88673342307841E-5, -4.87009738261986E-5,
    -4.87117906184134E-5, -4.87200190693919E-5, -3.79685374813046E-5,
    -4.68744836038349E-5, -4.73416367988728E-5, -4.73394561450093E-5,
    -4.73509049970184E-5, -4.64363818755607E-5, -4.61543778013317E-5,
    -4.6167823468177E-5, -4.61787853109425E-5, -3.78141703310281E-5,
    -3.08958954621175E-5, -2.95154831096471E-5, -1.11719464879967E-5,
    -1.1188999428217E-5, -4.85783556762299E-5, 2.52632344602182E-5,
    -2.40608263205729E-5, -1.00668206437317E-5, -1.00796452076111E-5,
    -1.0004077636475E-5, -4.32057663158836E-5, 1.91896296138269E-5,
    -3.76568413348472E-5, -3.7655403055251E-5, -3.76646184622199E-5,
    -3.76701976245935E-5, -3.80464929705986E-5, -3.64252184079733E-5,
    -3.64433664252032E-5, -3.64462268760534E-5, 2.84214816782398E-5,
    -2.24413492610041E-5, -2.22821961508965E-5, 119.0, 12.0, 12.0, 12.0, 12.0,
    0.0, 0.0, 0.0, 0.0, 23.0, 23.0, 2.0, 0.0, 0.0, 0.0, 12.0, 26.0, 22.0, 22.0,
    23.0, 10.0, 11.0, 8.0, 8.0, 7.0, 15.0, 15.0, 16.0, 16.0, 17.0, 0.0, 6.0, 7.0,
    6.0, 0.0, 4.0, 6.0, 6.0, 7.0, 44.0, 23.0, 23.0, 23.0, 20.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 8.0, 8.0, 9.0, 9.0, 0.0, 0.0, 11.0, 12.0, 13.0, 13.0, 16.0, 19.0,
    18.0, 20.0, 0.0, 0.0, 0.0, 0.0, 27.0, 29.0, 30.0, 30.0, 8.0, 6.0, 3.0, 3.0,
    13.0, 14.0, 14.0, 14.0, 0.0, 19.0, 19.0, 19.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9.0,
    11.0, 12.0, 12.0, 16.0, 19.0, 20.0, 20.0, 8.0, 11.0, 9.0, 8.0, 7.0, 86.0,
    28.0, 29.0, 29.0, 29.0, 13.0, 21.0, 21.0, 23.0, 18.0, 7.0, 0.0, 0.0, 0.0,
    19.0, 14.0, 14.0, 13.0, 11.0, 23.0, 23.0, 24.0, 24.0, 0.0, 6.0, 1.0, 0.0,
    21.0, 24.0, 31.0, 35.0, 27.0, 0.0, 19.0, 19.0, 62.0, 30.0, 31.0, 31.0, 31.0,
    0.0, 0.0, 0.0, 0.0, 35.0, 36.0, 10.0, 24.0, 21.0, 20.0, 13.0, 26.0, 26.0,
    26.0, 14.0, 13.0, 14.0, 14.0, 0.0, 0.0, 0.0, 14.0, 16.0, 17.0, 17.0, 21.0,
    26.0, 26.0, 26.0, 11.0, 11.0, 11.0, 11.0, 0.0, 0.0, 0.0, 0.0, 14.0, 16.0,
    16.0, 17.0, 24.0, 28.0, 29.0, 28.0, 25.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 13.0, 15.0, 16.0, 16.0, 4.0, 21.0, 22.0, 22.0, 12.0,
    16.0, 17.0, 18.0, 3.0, 16.0, 17.0, 16.0, 13.0, 56.0, 25.0, 26.0, 27.0, 18.0,
    78.0, 12.0, 26.0, 28.0, 32.0, 10.0, 31.0, 34.0, 34.0, 24.0, 13.0, 26.0, 33.0,
    33.0, 23.0, 6.0, 27.0, 30.0, 24.0, 22.0, 23.0, 24.0, 24.0, 22.0, 27.0, 27.0,
    28.0, 26.0, 24.0, 24.0, 25.0, 101.0, 20.0, 23.0, 24.0, 24.0, 18.0, 34.0,
    35.0, 35.0, 17.0, 11.0, 18.0, 18.0, 19.0, 12.0, 29.0, 30.0, 30.0, 16.0, 20.0,
    28.0, 35.0, 36.0, 33.0, 121.0, 23.0, 39.0, 39.0, 39.0, 15.0, 6.0, 22.0, 22.0,
    22.0, 22.0, 17.0, 24.0, 26.0, 26.0, 123.0, 19.0, 30.0, 6.0, 9.0, 9.0, 9.0,
    2.0, 20.0, 19.0, 21.0, 21.0, 7.0, 3.0, 13.0, 20.0, 22.0, 22.0, 9.0, 11.0,
    11.0, 11.0, 4.0, 27.0, 20.0, 27.0, 28.0, 30.0, 7.0, 11.0, 10.0, 10.0, 3.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 12.0, 12.0, 12.0, 12.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 14.0, 14.0, 14.0, 14.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 12.0, 11.0, 11.0, 11.0, 22.0, 22.0, 22.0, 26.0, 21.0, 30.0, 27.0, 9.0,
    22.0, 22.0, 23.0, 17.0, 12.0, 20.0, 35.0, 34.0, 24.0, 17.0, 17.0, 18.0, 11.0,
    10.0, 11.0, 11.0, 0.0, 0.0, 0.0, 0.0, 0.0, 14.0, 14.0, 14.0, 14.0, 15.0,
    22.0, 24.0, 24.0, 24.0, 9.0, 14.0, 14.0, 14.0, 6.0, 11.0, 13.0, 13.0, 13.0,
    3.0, 22.0, 22.0, 23.0, 16.0, 25.0, 25.0, 25.0, 23.0, 10.0, 11.0, 11.0, 11.0,
    4.0, 23.0, 23.0, 24.0, 24.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 23.0, 23.0, 7.0,
    16.0, 15.0, 15.0, 7.0, 20.0, 22.0, 22.0, 21.0, 9.0, 1.0, 24.0, 22.0, 21.0,
    21.0, 14.0, 13.0, 14.0, 14.0, 0.0, 0.0, 0.0, 0.0, 13.0, 14.0, 14.0, 0.0, 0.0,
    0.0, 0.0, 16.0, 15.0, 15.0, 7.0, 0.0, 0.0, 0.0, 0.0, 16.0, 16.0, 17.0, 17.0,
    0.0, 0.0, 0.0, 0.0, 16.0, 20.0, 20.0, 18.0, 0.0, 0.0, 0.0, 0.0, 0.0, 17.0,
    17.0, 17.0, 17.0, 14.0, 13.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 11.0, 11.0,
    11.0, 12.0, 12.0, 12.0, 11.0, 12.0, 11.0, 11.0, 11.0, 11.0, 1.0, 15.0, 20.0,
    20.0, 17.0, 4.0, 15.0, 20.0, 19.0, 19.0, 8.0, 17.0, 29.0, 30.0, 27.0, 14.0,
    12.0, 28.0, 30.0, 30.0, 18.0, 13.0, 24.0, 25.0, 12.0, 36.0, 36.0, 36.0, 9.0,
    13.0, 13.0, 13.0, 6.0, 10.0, 12.0, 12.0, 12.0, 6.0, 12.0, 12.0, 12.0, 4.0,
    21.0, 23.0, 23.0, 16.0, 5.0, 10.0, 11.0, 11.0, 11.0, 10.0, 24.0, 23.0, 20.0,
    10.0, 13.0, 23.0, 23.0, 23.0, 9.0, 7.0, 15.0, 25.0, 25.0, 17.0, 7.0, 10.0,
    14.0, 14.0, 14.0, 5.0, 11.0, 12.0, 12.0, 12.0, 6.0, 21.0, 35.0, 15.0, 15.0,
    15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, 18.0,
    18.0, 18.0, 13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 16.0, 16.0, 16.0, 11.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 13.0, 16.0, 16.0, 16.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 14.0,
    14.0, 14.0, 14.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    14.0, 14.0, 14.0, 14.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 12.0, 14.0, 14.0, 14.0, 0.0, 0.0, 0.0, 0.0, 0.0, 11.0, 13.0,
    13.0, 13.0, 0.0, 14.0, 14.0, 14.0, 14.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 2.0, 16.0, 16.0, 16.0, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 7.0, 15.0, 15.0, 15.0, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 4.0, 5.0, 6.0, 17.0, 17.0, 16.0, 16.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 16.0, 16.0, 16.0, 13.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 13.0, 16.0, 16.0, 16.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    13.0, 13.0, 13.0, 12.0, 14.0, 14.0, 14.0, 14.0, 14.0, 14.0, 14.0, 14.0, 0.0,
    21.0, 28.0, 28.0, 28.0, 5.0, 11.0, 26.0, 26.0, 26.0, 11.0, 18.0, 26.0, 25.0,
    24.0, 6.0, 14.0, 29.0, 31.0, 31.0, 16.0, 17.0, 31.0, 30.0, 13.0, 14.0, 14.0,
    14.0, 13.0, 18.0, 18.0, 18.0, 17.0, 15.0, 17.0, 17.0, 17.0, 9.0, 16.0, 16.0,
    16.0, 7.0, 13.0, 28.0, 28.0, 27.0, 13.0, 15.0, 14.0, 14.0, 14.0, 16.0, 26.0,
    26.0, 26.0, 10.0, 13.0, 25.0, 28.0, 28.0, 12.0, 10.0, 17.0, 29.0, 29.0, 18.0,
    12.0, 3.0, 13.0, 13.0, 13.0, 10.0, 14.0, 16.0, 16.0, 16.0, 10.0, 18.0, 25.0,
    0.0, 16.0, 16.0, 16.0, 16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 19.0, 19.0, 20.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 18.0, 18.0,
    18.0, 18.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 17.0, 17.0,
    17.0, 17.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 14.0, 15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 17.0, 18.0, 18.0, 18.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 11.0, 11.0, 11.0, 11.0, 1.0, 15.0, 16.0, 16.0, 13.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 12.0, 13.0, 14.0, 14.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 18.0, 17.0, 18.0, 18.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 17.0, 16.0, 17.0, 17.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 16.0, 16.0,
    17.0, 17.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 8.0, 13.0, 13.0, 13.0, 14.0, 15.0, 15.0, 15.0, 7.0, 14.0,
    14.0, 14.0, 10.0, 19.0, 29.0, 29.0, 29.0, 14.0, 0.0, 28.0, 27.0, 27.0, 27.0,
    18.0, 27.0, 27.0, 28.0, 11.0, 14.0, 21.0, 31.0, 31.0, 18.0, 19.0, 30.0, 32.0,
    15.0, 17.0, 17.0, 17.0, 17.0, 16.0, 17.0, 17.0, 17.0, 0.0, 17.0, 17.0, 17.0,
    0.0, 16.0, 16.0, 16.0, 17.0, 15.0, 28.0, 28.0, 28.0, 15.0, 14.0, 15.0, 15.0,
    15.0, 16.0, 27.0, 27.0, 27.0, 11.0, 0.0, 17.0, 27.0, 27.0, 27.0, 0.0, 15.0,
    27.0, 27.0, 27.0, 14.0, 0.0, 10.0, 10.0, 10.0, 10.0, 0.0, 14.0, 14.0, 14.0,
    0.0, 17.0, 28.0, 5.06448624585505E-5, 5.07026809874748E-5,
    4.8164918893501E-5, 4.88723636758236E-5, 4.9954999406375E-5,
    1.13778016257739E-5, 3.7128356386545E-5, 3.3160899051158E-5,
    3.3160397092902E-5, 2.63847870666996E-5, 2.82485960641077E-5,
    8.29372317786749E-6, 7.21403250065078E-6, 1.63154176600385E-5,
    1.04335429396001E-5, 9.42464768942536E-6, 6.90166142593117E-5,
    3.93182543121998E-5, 4.07073852486376E-5, 4.10438012107526E-5,
    0.000130057926407613, 1.378050307008E-5, 1.31220148474602E-5,
    1.29973680735334E-5, 1.45167709080362E-5, 1.62096100329164E-5,
    7.5092549333946E-5, 7.56941317618875E-5, 7.56321083217762E-5,
    7.56054254988756E-5, 0.000142304797571197, 0.000166500009728686,
    0.000170745149521381, 0.000173572771979687, 0.000153172911002341,
    0.000172640889685427, 0.000175470374093779, 0.000179718627992929,
    0.000181235283846012, 7.88568659861276E-5, 8.34364032439703E-5,
    8.25610368481599E-5, 8.24979971394049E-5, 6.05965374365768E-5,
    0.000111462794385934, 0.000103435304048245, 0.000109423728666905,
    0.000110611922332673, 7.40302243592314E-5, 3.41077548446722E-5,
    2.86314311420356E-5, 2.54781291210207E-5, 2.36917813679163E-5,
    6.82914531625695E-5, 4.3309720433564E-5, 0.000185409831787897,
    0.000196983891219017, 0.000200015357989306, 0.000203810499448963,
    4.26277267224666E-5, 4.2468788537403E-5, 4.1589082127222E-5,
    4.20671309947188E-5, 1.51780635008942E-5, 1.43955459761878E-5,
    1.87373688344001E-5, 1.86990355253825E-5, 4.02282744830138E-5,
    4.0344738178086E-5, 4.05080781518138E-5, 4.00397511794484E-5,
    1.6127881613635E-5, 9.72196318555578E-6, 9.48746265631746E-6,
    9.74450648596456E-6, 0.00015755021865633, 0.000173592173829929,
    0.00018834234446797, 0.000183038580029773, 2.20386335176608E-5,
    0.000144057947716643, 0.000148413943451498, 0.000150317793585367,
    3.4692916807076E-5, 5.71086081559234E-5, 5.07193757513847E-5,
    4.90475894227784E-5, 4.10916758747612E-5, 0.000179505446167943,
    0.000187651183224265, 0.000193376118516561, 0.000196910060435787,
    4.97770302443451E-5, 4.94619127833865E-5, 5.03187182177868E-5,
    5.0747421732608E-5, 1.44254329476897E-5, 2.21866389899249E-5,
    1.15352761531174E-5, 1.25264503914006E-5, 1.21483915073637E-5,
    5.14526995576757E-5, 5.35220207141422E-5, 5.71976714091167E-5,
    5.65627765627225E-5, 5.74076360916944E-5, 4.93729334717512E-5,
    5.12096272281172E-5, 5.06174286497903E-5, 5.05322595669579E-5,
    2.11136131953819E-5, 1.45876461557719E-5, 1.95527649959419E-5,
    1.64058909229043E-5, 1.48758243167109E-5, 1.01482829984985E-5,
    1.09330306892916E-5, 2.94056669886662E-5, 3.02717003688428E-5,
    4.81819692208983E-5, 1.17882325420719E-5, 1.15798561260749E-5,
    1.16972682806544E-5, 1.12755373451165E-5, 1.17260561307751E-5,
    1.14750401908972E-5, 1.11519369931563E-5, 1.34735572241063E-5,
    4.97276503751738E-5, 5.00625710661413E-5, 5.11024553554609E-5,
    5.14621129918994E-5, 4.49831912236173E-5, 3.63718183675432E-5,
    2.3532087592741E-5, 2.46305606233396E-5, 3.85963894083375E-5,
    5.9149752033824E-5, 6.36822102859574E-5, 6.10481150601723E-5,
    6.07360484694971E-5, 1.35523786009234E-5, 1.98424030994216E-5,
    1.84931417427033E-5, 1.79191147476163E-5, 2.9360241669422E-5,
    3.07159182365431E-5, 2.14812215980952E-5, 3.05886447927627E-5,
    2.21790143448675E-5, 2.3241609525598E-5, 6.47067769811929E-5,
    6.43627594060715E-5, 6.6752251020484E-5, 6.73497662598425E-5,
    0.000158117583455514, 0.000163073411448379, 0.000168863638688345,
    0.000170584538398315, 2.86939769012582E-5, 2.41614967089892E-5,
    1.68371523215347E-5, 0.000184202438803271, 0.000192774213575774,
    0.000198542025008103, 0.000204024076550469, 4.12740939449499E-5,
    4.21148613446982E-5, 4.30471085726E-5, 4.22472657556843E-5,
    0.000151757847505726, 0.000155161578386153, 0.000158032347617402,
    0.000160453469378656, 3.96786147915987E-5, 2.99841743765525E-5,
    2.37280015074862E-5, 2.14273296838521E-5, 0.000188325614013168,
    0.000196778534292915, 0.000202572540535949, 0.000205811171567844,
    5.08788819536398E-5, 7.61595297330056E-5, 7.61657190170098E-5,
    7.50749559854248E-5, 4.50172595614861E-5, 9.83470196677947E-5,
    0.000105285522827002, 0.000111700746048199, 0.000115292391998593,
    2.90826352148357E-5, 2.39377597887597E-5, 2.57848889697454E-5,
    2.90286532139387E-5, 3.73405866266447E-5, 3.25830293791682E-5,
    2.67753818420768E-5, 2.12500603407693E-5, 0.000202863059701325,
    0.000214684310901092, 0.000221766896489776, 0.000225845370048957,
    2.12623807436635E-5, 2.13625200391652E-5, 2.16416596460789E-5,
    2.18761266157059E-5, 1.50241882782828E-5, 1.32825799852249E-5,
    1.9409092244306E-5, 1.30677344655787E-5, 7.92708482564039E-6,
    1.2424195940064E-5, 1.8157601368685E-5, 1.23624614682718E-5,
    9.61671599124748E-6, 2.62116832345313E-5, 2.71483997506152E-5,
    3.12031630913472E-5, 2.82944305596779E-5, 1.29546339645812E-5,
    1.60666163042735E-5, 2.77808986028121E-5, 2.93564191578675E-5,
    2.87288139718477E-5, 2.82314305123603E-5, 2.18207893873528E-5,
    2.3699481452154E-5, 2.39202755369883E-5, 2.25098089636443E-5,
    1.62602522180101E-5, 1.69797653082286E-5, 3.56190713621435E-5,
    3.58286759332059E-5, 3.58814375853571E-5, 3.52565905325894E-5,
    2.57431308463093E-5, 2.99396402646013E-5, 2.9489295244419E-5,
    1.4611029766872E-5, 5.37225859910214E-5, 5.44245795676164E-5,
    5.36626363207049E-5, 5.37610098689158E-5, 2.61535644620446E-5,
    2.80133857127708E-5, 2.66908825145287E-5, 2.67267821221928E-5,
    2.35093380440652E-5, 3.05888038358741E-5, 3.21242278346501E-5,
    3.0534387897784E-5, 1.21697779373332E-5, 1.89430321716295E-5,
    3.20654248667126E-5, 1.86169000027756E-5, 1.81528005742641E-5,
    2.56624210193647E-5, 2.97838643232469E-5, 2.84988824714603E-5,
    2.92806787791082E-5, 2.41877004046977E-5, 1.23269149318207E-5,
    1.13086669778661E-5, 1.1071618545134E-5, 2.54263959532107E-5,
    3.22641584560869E-5, 3.66316726715592E-5, 3.71590091735519E-5,
    3.62076469349474E-5, 3.35359786479486E-5, 1.64332424081425E-5,
    2.04129233339788E-5, 2.26628203642565E-5, 2.13809574866764E-5,
    3.96096049702088E-5, 1.04542904659428E-5, 4.32195065997681E-5,
    5.48634653036394E-5, 5.51006547702902E-5, 5.53426690402484E-5,
    2.19380820397706E-5, 2.4743442971574E-5, 2.76939364247515E-5,
    2.76507505630698E-5, 2.7586073895596E-5, 2.42826576140234E-5,
    1.53080982898631E-5, 1.50132820315856E-5, 1.60604023262154E-5,
    1.67920120379989E-5, 1.01078384722659E-5, 3.17575150132611E-5,
    3.57591433552095E-5, -9.67419138879792E-6, -2.90436667284152E-5,
    -3.17794303219823E-5, -3.10313730919528E-5, -2.9841833653709E-5,
    -2.54567578307682E-5, -3.2597534885425E-5, -3.1703233982089E-5,
    -3.17439639535687E-5, -8.5697787756388E-6, -9.9480938919727E-6,
    -1.03627109218691E-5, -8.82364364093478E-6, -1.00700632618022E-5,
    -1.19683511567002E-5, -1.03211233645887E-5, -5.87691943661875E-5,
    -4.97313536815522E-5, -4.81873734158702E-5, -4.78060286842586E-5,
    -0.000181157475990069, -1.04223632062459E-5, -2.0528140742687E-5,
    -2.04846132776016E-5, -2.1795969949146E-5, -3.65821191818582E-5,
    -4.74363348973281E-5, -4.66903656271267E-5, -4.6746345930272E-5,
    -4.67602154518277E-5, -3.94346216146713E-5, -2.65880691320827E-5,
    -2.21581910498643E-5, -1.8676154695506E-5, -2.78760268393303E-5,
    -3.06089579053406E-5, -2.51736534223005E-5, -2.03386714302096E-5,
    -2.49670910947321E-5, -2.47000719097977E-5, -5.19548133964921E-5,
    -5.29553948414266E-5, -5.30206258619918E-5, -5.34822349576739E-5,
    -2.25922677831465E-5, -3.00001921562957E-5, -3.00769362695203E-5,
    -2.1782606692468E-5, -1.92838850119608E-5, -0.000165754390637097,
    -0.000171549850537989, -0.000175127034964289, -0.000177190791293121,
    -0.000160252142230262, -0.000168265422758452, -3.93314826824839E-5,
    -4.33088129056659E-5, -3.91483514286634E-5, -3.38243895727905E-5,
    -2.00344407854478E-5, -2.01087086596157E-5, -2.02935844341524E-5,
    -1.97056942760372E-5, -3.34364358106252E-5, -3.60515483063769E-5,
    -3.84023084653733E-5, -3.83980427523887E-5, -3.86845343994532E-5,
    -3.85268658387956E-5, -3.83413484046094E-5, -3.8235728010176E-5,
    -1.19426787122143E-5, -1.10320026139691E-5, -1.13865297872517E-5,
    -1.10336095382579E-5, -5.15678296413692E-5, -0.000133616517749235,
    -0.000184418123950237, -0.000193088300087689, -0.000194933456026919,
    -2.16257447721532E-5, -2.22794358019292E-5, -1.94617116584135E-5,
    -0.000169671968271885, -0.000179011378895681, -0.000184047347945255,
    -0.000185287989412102, -0.000151875866085997, -4.0341264001455E-5,
    -3.27213589895947E-5, -2.75377690101295E-5, -2.43868134996124E-5,
    -2.32041833905547E-5, -4.65431406412945E-5, -4.55997458265797E-5,
    -4.50769771752296E-5, -0.000188234768486297, -1.40240076488747E-5,
    -1.25932542459185E-5, -1.76487057996683E-5, -1.83052876115519E-5,
    -1.7573819386564E-5, -5.41994781749694E-5, -5.03234037379771E-5,
    -5.10934816261274E-5, -4.99158709721395E-5, -1.86353640507614E-5,
    -3.85604565028258E-5, -2.49023081629133E-5, -2.49924476709464E-5,
    -2.56220657961753E-5, -2.18157789978786E-5, -2.5611804681224E-5,
    -2.39434737371311E-5, -2.48265361557902E-5, -1.63408885496511E-5,
    -1.7050149872984E-5, -1.93854547716711E-5, -1.89231919687201E-5,
    -1.91014408756185E-5, -1.58563312574581E-5, -1.61213150821873E-5,
    -1.46662567945538E-5, -1.51376640724225E-5, -1.89588534521713E-5,
    -1.94008340633139E-5, -2.08162685579868E-5, -2.14700793688618E-5,
    -3.52733646324975E-5, -3.48822354723927E-5, -3.37287675574566E-5,
    -3.33017113764953E-5, -1.51376207857492E-5, -1.56896340935043E-5,
    -3.28580619084411E-5, -3.13903284516128E-5, -2.40617045243025E-5,
    -4.03852700348849E-5, -7.01142155727072E-5, -3.82276199955803E-5,
    -3.8604005745853E-5, -1.65306266788091E-5, -1.74665471754473E-5,
    -1.86749388533947E-5, -1.90980930715155E-5, -8.83216952725163E-6,
    -1.67872686503049E-5, -1.63752367233236E-5, -1.77933736192763E-5,
    -2.6484812274953E-5, -2.47351585822399E-5, -4.54084335918332E-5,
    -4.29651969081177E-5, -4.02783915402955E-5, -3.95655419965259E-5,
    -3.76685560125827E-5, -3.25238056093418E-5, -2.59954478498794E-5,
    -2.40026274559478E-5, -0.000145891365308494, -0.000149145812294526,
    -0.00015108819782918, -3.5277640294804E-5, -3.17811771244545E-5,
    -2.5295562752113E-5, -4.89435393348483E-5, -2.90059291518282E-5,
    -3.03252234886114E-5, -2.92827783688001E-5, -3.02269137722331E-5,
    -2.62234146020065E-5, -2.46592766890501E-5, -1.91237614048112E-5,
    -2.68999150934139E-5, -0.000129096207341612, -0.000139479504943107,
    -0.000146534247028291, -0.000149194048952011, -3.03367369165291E-5,
    -2.46847467097106E-5, -3.10192125967285E-5, -2.53387835614306E-5,
    -2.80027344680496E-5, -9.83460079656046E-5, -9.83231922406052E-5,
    -9.96092267222112E-5, -9.98796266184878E-5, -4.44922742749623E-5,
    -3.74257917939399E-5, -3.04051508654461E-5, -2.70563773022263E-5,
    -0.000131979626163839, -0.000143254859474345, -0.000149471944638107,
    -0.000152391652427176, -0.000120826357340562, -0.0001282046522373,
    -0.000132621835815418, -0.000135382878034704, -4.56424018615812E-5,
    -3.43034271929952E-5, -2.79611166497714E-5, -3.46714715503235E-5,
    -2.20739611317868E-5, -2.36412908522773E-5, -2.33334619068912E-5,
    -2.30380888496659E-5, -2.49609461056131E-5, -1.05122333429623E-5,
    -1.14873683132267E-5, -1.02929350523593E-5, -1.2266205992549E-5,
    -1.46122671966513E-5, -1.50567189149038E-5, -1.46852116907162E-5,
    -1.37571461521024E-5, -1.92516382538055E-5, -1.80810164376463E-5,
    -2.71741300034071E-5, -2.57259205503876E-5, -2.62965015544443E-5,
    -7.10333470227512E-6, -1.64486497032989E-5, -2.93457406753819E-5,
    -1.73925871742373E-5, -1.78898854225341E-5, -2.24301071885997E-5,
    -2.56988231267873E-5, -2.54292102633907E-5, -2.74070775127923E-5,
    -2.7138353629135E-5, -1.38256509596576E-5, -2.99779129085692E-5,
    -2.9393546876951E-5, -2.93426190779471E-5, -3.00427497852902E-5,
    -1.90084774930451E-5, -1.86993267487781E-5, -1.96323589270919E-5,
    -1.9786901765559E-5, -3.32884314853901E-5, -3.3365139737546E-5,
    -3.43758783245644E-5, -3.28451858268346E-5, -1.210069164828E-5,
    -1.44502006430675E-5, -1.54511409841162E-5, -1.5467442465723E-5,
    -1.36799554694685E-5, -2.25485194452015E-5, -2.08445801066023E-5,
    -2.25837499036093E-5, -1.14054708839617E-5, -2.54394054641693E-5,
    -2.86075286602015E-5, -2.57700817202809E-5, -2.63022171057431E-5,
    -1.65527081295265E-5, -4.31272206580706E-5, -4.08860852252795E-5,
    -3.96460240358674E-5, -4.06385379924492E-5, -1.19230599289746E-5,
    -1.19614012103202E-5, -1.23040529269879E-5, -1.29138210206625E-5,
    -1.37288167140023E-5, -6.62988470037592E-5, -6.55944126499488E-5,
    -6.6993173202356E-5, -6.62204656015584E-5, -1.34860918627796E-5,
    -1.65925929152594E-5, -1.42675848488075E-5, -1.51932657535346E-5,
    -3.19556072579726E-5, -1.13443218682358E-5, -1.86441269411061E-5,
    -7.44453784210608E-5, -7.40192566794214E-5, -7.3664498770889E-5,
    -7.47694311611242E-5, -2.20238296787055E-5, -2.09452800443042E-5,
    -1.54563577961674E-5, -1.55313818349333E-5, -1.51968468911384E-5,
    -1.01367309472961E-5, -1.55505174030471E-5, -1.42499680667555E-5,
    -1.86505554436609E-5, -1.03023851183375E-5, -1.36227039689586E-5,
    -3.06201082315537E-5, 0.0, 5.07026809874748E-5, 4.8164918893501E-5,
    4.88723636758236E-5, 4.9954999406375E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.07073852486376E-5, 4.10438012107526E-5, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 7.5092549333946E-5, 7.56941317618875E-5,
    7.56321083217762E-5, 7.56054254988756E-5, 0.000142304797571197,
    0.000166500009728686, 0.000170745149521381, 0.000173572771979687, 0.0,
    0.000172640889685427, 0.000175470374093779, 0.000179718627992929,
    0.000181235283846012, 7.88568659861276E-5, 8.34364032439703E-5,
    8.25610368481599E-5, 8.24979971394049E-5, 0.0, 0.000111462794385934,
    0.000103435304048245, 0.000109423728666905, 0.000110611922332673,
    7.40302243592314E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 4.3309720433564E-5,
    0.000185409831787897, 0.000196983891219017, 0.000200015357989306,
    0.000203810499448963, 4.26277267224666E-5, 4.2468788537403E-5,
    4.1589082127222E-5, 4.20671309947188E-5, 0.0, 0.0, 0.0, 0.0,
    4.02282744830138E-5, 4.0344738178086E-5, 4.05080781518138E-5, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.00015755021865633, 0.000173592173829929, 0.00018834234446797,
    0.000183038580029773, 0.0, 0.000144057947716643, 0.000148413943451498,
    0.000150317793585367, 0.0, 5.71086081559234E-5, 5.07193757513847E-5,
    4.90475894227784E-5, 4.10916758747612E-5, 0.000179505446167943,
    0.000187651183224265, 0.000193376118516561, 0.000196910060435787,
    4.97770302443451E-5, 4.94619127833865E-5, 5.03187182177868E-5,
    5.0747421732608E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 5.14526995576757E-5,
    5.35220207141422E-5, 5.71976714091167E-5, 5.65627765627225E-5, 0.0,
    4.93729334717512E-5, 5.12096272281172E-5, 5.06174286497903E-5,
    5.05322595669579E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.97276503751738E-5, 5.00625710661413E-5,
    5.11024553554609E-5, 5.14621129918994E-5, 0.0, 0.0, 0.0, 0.0, 0.0,
    5.9149752033824E-5, 6.36822102859574E-5, 6.10481150601723E-5,
    6.07360484694971E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    6.47067769811929E-5, 6.43627594060715E-5, 6.6752251020484E-5,
    6.73497662598425E-5, 0.000158117583455514, 0.000163073411448379,
    0.000168863638688345, 0.000170584538398315, 0.0, 0.0, 0.0,
    0.000184202438803271, 0.000192774213575774, 0.000198542025008103,
    0.000204024076550469, 4.12740939449499E-5, 4.21148613446982E-5,
    4.30471085726E-5, 4.22472657556843E-5, 0.000151757847505726,
    0.000155161578386153, 0.000158032347617402, 0.000160453469378656, 0.0, 0.0,
    0.0, 0.0, 0.000188325614013168, 0.000196778534292915, 0.000202572540535949,
    0.000205811171567844, 5.08788819536398E-5, 7.61595297330056E-5,
    7.61657190170098E-5, 7.50749559854248E-5, 0.0, 9.83470196677947E-5,
    0.000105285522827002, 0.000111700746048199, 0.000115292391998593, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000202863059701325, 0.000214684310901092,
    0.000221766896489776, 0.000225845370048957, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 5.37225859910214E-5, 5.44245795676164E-5, 5.36626363207049E-5, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 4.32195065997681E-5, 5.48634653036394E-5, 5.51006547702902E-5,
    5.53426690402484E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, -5.87691943661875E-5, 0.0, 0.0, 0.0, -0.000181157475990069,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, -5.19548133964921E-5, -5.29553948414266E-5,
    -5.30206258619918E-5, -5.34822349576739E-5, 0.0, 0.0, 0.0, 0.0, 0.0,
    -0.000165754390637097, -0.000171549850537989, -0.000175127034964289,
    -0.000177190791293121, -0.000160252142230262, -0.000168265422758452, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.000184418123950237, -0.000193088300087689,
    -0.000194933456026919, 0.0, 0.0, 0.0, -0.000169671968271885,
    -0.000179011378895681, -0.000184047347945255, -0.000185287989412102,
    -0.000151875866085997, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -0.000188234768486297, 0.0, 0.0, 0.0, 0.0, 0.0, -5.41994781749694E-5,
    -5.03234037379771E-5, -5.10934816261274E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, -0.000145891365308494, -0.000149145812294526,
    -0.00015108819782918, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, -0.000129096207341612, -0.000139479504943107, -0.000146534247028291,
    -0.000149194048952011, 0.0, 0.0, 0.0, 0.0, 0.0, -9.83460079656046E-5,
    -9.83231922406052E-5, -9.96092267222112E-5, -9.98796266184878E-5, 0.0, 0.0,
    0.0, 0.0, -0.000131979626163839, -0.000143254859474345,
    -0.000149471944638107, -0.000152391652427176, -0.000120826357340562,
    -0.0001282046522373, -0.000132621835815418, -0.000135382878034704, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -6.62988470037592E-5, -6.55944126499488E-5,
    -6.6993173202356E-5, -6.62204656015584E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -7.44453784210608E-5, -7.40192566794214E-5, -7.3664498770889E-5,
    -7.47694311611242E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 6.03190538473484E-5, 7.974634771589E-5, 7.99443492154833E-5,
    7.99037367677763E-5, 7.9796833060084E-5, 3.68345594565421E-5,
    6.972589127197E-5, 6.4864133033247E-5, 6.49043610464708E-5,
    3.49545658423384E-5, 3.81966899560804E-5, 1.86564340997366E-5,
    1.60376761415856E-5, 2.63854809218406E-5, 2.24018940963003E-5,
    1.9745771054014E-5, 0.000127785808625499, 8.9049607993752E-5,
    8.88947586645078E-5, 8.88498298950112E-5, 0.000311215402397683,
    2.42028662763258E-5, 3.36501555901472E-5, 3.3481981351135E-5,
    3.63127408571822E-5, 5.27917292147746E-5, 0.000122528884231274,
    0.000122384497389014, 0.000122378454252048, 0.000122365640950703,
    0.000181739419185868, 0.000193088078860768, 0.000192903340571246,
    0.000192248926675193, 0.000181048937841671, 0.000203249847590767,
    0.000200644027516079, 0.000200057299423139, 0.000206202374940744,
    0.000103556937895925, 0.000135391216640462, 0.000135516431689587,
    0.000135518623001397, 0.000114078772394251, 0.000134055062169081,
    0.000133435496204541, 0.000139500664936426, 0.000132394529025141,
    9.33141093711922E-5, 0.00019986214548177, 0.000200181281680025,
    0.00020060516408531, 0.000200882572661037, 0.000228543595392832,
    0.000211575143192016, 0.000224741314470381, 0.000240292704124683,
    0.00023916370941797, 0.000237634889021754, 6.26621675079144E-5,
    6.25774971970187E-5, 6.18826665613744E-5, 6.1772825270756E-5,
    4.86144993115194E-5, 5.04470942825647E-5, 5.71396772997734E-5,
    5.70970782777712E-5, 7.89128088824669E-5, 7.88716040168816E-5,
    7.88494265564232E-5, 7.82754791896244E-5, 2.80705603258492E-5,
    2.07539657995249E-5, 2.08739924435691E-5, 2.07781160242225E-5,
    0.000209118048297699, 0.000307208691579164, 0.000372760468418207,
    0.000376126880117462, 0.00021697208954458, 0.000165683692488796,
    0.000170693379253427, 0.000169779505243781, 0.000204364885078961,
    0.000236119987051605, 0.00023476672369664, 0.00023433557883488,
    0.000192967541960758, 0.000219846710169398, 0.00022037254221386,
    0.00022091388752669, 0.0002212968739354, 7.29812136348998E-5,
    9.60050534246809E-5, 9.59184640443665E-5, 9.58243989078376E-5,
    0.000202660201433986, 3.62106466387996E-5, 2.41285303990359E-5,
    3.01751561910689E-5, 3.04536791189156E-5, 6.90265189442397E-5,
    0.000107721498889112, 0.000107521075147094, 0.00010765625818885,
    0.000107323507063834, 6.80082975225125E-5, 8.9770083730943E-5,
    7.55197368127036E-5, 7.55247072379043E-5, 4.67356789915571E-5,
    3.64034251536505E-5, 4.51645696771659E-5, 4.03493646600353E-5,
    3.97023604725011E-5, 2.64891715481496E-5, 2.79831805622756E-5,
    4.87911217603374E-5, 4.91948923375629E-5, 6.72834100965168E-5,
    2.764456379953E-5, 2.77011712082623E-5, 2.63635250752082E-5,
    2.6413201417539E-5, 3.06849095829464E-5, 3.08758742542111E-5,
    3.19682055511431E-5, 3.49436365929682E-5, 8.50010150076713E-5,
    8.4944806538534E-5, 8.48312229129175E-5, 8.47638243683947E-5,
    6.01208120093665E-5, 5.20614524610475E-5, 5.6390149501182E-5,
    5.60208890749525E-5, 6.265809393264E-5, 9.95350220687088E-5,
    0.000133796425858665, 9.92757350557526E-5, 9.93400542153502E-5,
    3.00830052797326E-5, 3.73089502748689E-5, 3.7168080596098E-5,
    3.70172078191318E-5, 3.81924111966736E-5, 4.7503186886848E-5,
    3.78564583214188E-5, 4.8382018412039E-5, 4.86638266198205E-5,
    4.79767681078379E-5, 0.000110115210573026, 0.000107327956314189,
    0.000107030642560779, 0.000106915308256368, 0.000195786139468097,
    0.000195597217057721, 0.000194859086538225, 0.000194587165854263,
    0.000174585342209752, 0.000173307309003515, 0.000167925350150715,
    0.000219480079098075, 0.000224555390700229, 0.000223837587760216,
    0.000252967615885317, 7.02800230967781E-5, 7.24400848333096E-5,
    7.23298869414001E-5, 7.24741795279174E-5, 0.000177981262107733,
    0.000179820855075203, 0.000177156109022213, 0.00018735338447207,
    0.000168774822133211, 0.000169463679319659, 0.000170262248535778,
    0.000170621378635863, 0.000218662350929698, 0.000221463281002626,
    0.000233591753132677, 0.000231149955129275, 7.88816164216894E-5,
    0.00017450553769861, 0.000174488911257615, 0.000174684182707636,
    0.000144896886179974, 0.000142839293942757, 0.000142711314620942,
    0.000142105896913645, 0.00014234876930082, 0.000161062261378675,
    0.000167192619263105, 0.000175256833607852, 0.000181420305641114,
    0.000158166943967207, 0.000160787681616468, 0.000159397217657494,
    0.000156632938375474, 0.000248505461562907, 0.000248987738094087,
    0.000249728013139548, 0.00026051684159928, 4.33363418754503E-5,
    4.50038108914425E-5, 4.497512155297E-5, 4.49142154653718E-5,
    3.99851343838959E-5, 2.37948133281872E-5, 3.08964605575327E-5,
    2.3360669517938E-5, 2.01932908181893E-5, 2.70364631367153E-5,
    3.32143202835888E-5, 2.7047673158988E-5, 2.33738621433499E-5,
    4.54633214883368E-5, 4.52294161882615E-5, 5.83772930947543E-5,
    5.40203511100655E-5, 3.92511355190255E-5, 2.31699510065486E-5,
    4.4229548306111E-5, 5.87021598332494E-5, 4.6121401146085E-5,
    4.61213159348944E-5, 4.42508965759525E-5, 4.93983045789413E-5,
    4.9349485800379E-5, 4.99168864764366E-5, 4.3398605847145E-5,
    3.08054162678862E-5, 6.55969842707127E-5, 6.52222228101569E-5,
    6.52240566633042E-5, 6.52993403178796E-5, 4.47516083393544E-5,
    4.86389670133794E-5, 4.9121654171511E-5, 3.43979315324309E-5,
    8.70110174764115E-5, 8.77897193051624E-5, 8.80385146452692E-5,
    8.66061956957504E-5, 3.82542561103246E-5, 4.24635863558383E-5,
    4.21420234986449E-5, 4.21942245879158E-5, 3.71892935135337E-5,
    5.31373232810755E-5, 5.29688079412524E-5, 5.31181378013933E-5,
    2.3575248821295E-5, 4.43824376357988E-5, 6.06729535269141E-5,
    4.43869817230565E-5, 4.44550176800071E-5, 4.22151291488912E-5,
    7.29110849813175E-5, 6.93849676967398E-5, 6.89267028149756E-5,
    6.48262383971469E-5, 2.42499748607953E-5, 2.32700681881863E-5,
    2.3375671472122E-5, 3.83402169738732E-5, 4.59929751700892E-5,
    0.000102930519675318, 0.000102753421823501, 0.000103200820137303,
    9.9756444249507E-5, 2.99193342709221E-5, 3.70055162492382E-5,
    3.6930405213064E-5, 3.6574223240211E-5, 7.15652122281814E-5,
    2.17986123341786E-5, 6.18636335408742E-5, 0.0001293088437247,
    0.000129119911449712, 0.000129007167811137, 9.67075132008948E-5,
    4.67672726502795E-5, 4.86392164690556E-5, 4.31071083592372E-5,
    4.31174557305293E-5, 3.94795045051618E-5, 2.54448292371592E-5,
    3.05637994346326E-5, 3.03103703929709E-5, 3.54425674816598E-5,
    2.04102235906034E-5, 4.53802189822197E-5, 6.63792515867633E-5,
    8.5231217653574E-6, 1.27674063040039E-5, 1.38965414670666E-5,
    1.4019100438139E-5, 1.22030079047831E-5, 6.75505847493062E-6,
    1.24330915969846E-5, 1.20322033215832E-5, 1.27072244812962E-5,
    7.10281678574177E-6, 7.06503268988757E-6, 4.56573387092258E-6,
    3.87000077707406E-6, 4.3444463321967E-6, 4.8006702365926E-6,
    4.04978652764827E-6, 1.86948577898725E-5, 1.60230558620887E-5,
    1.60004445331733E-5, 1.58298226993777E-5, 4.57624182146791E-5,
    5.47946997420159E-6, 6.17120640240092E-6, 6.32142581449629E-6,
    7.06016063543227E-6, 7.74436979806238E-6, 1.94028634769339E-5,
    1.96057457209256E-5, 1.97067322084141E-5, 1.85454103741405E-5,
    3.91793371790353E-5, 4.08194909442089E-5, 4.1670870009333E-5,
    4.1724708091291E-5, 2.56512733333898E-5, 4.05370664409638E-5,
    4.13853312694373E-5, 4.24893144731328E-5, 3.71607985963592E-5,
    1.86914058480304E-5, 2.21772808483821E-5, 2.18835352363983E-5,
    2.18593908587885E-5, 1.42965659067973E-5, 2.49241635249978E-5,
    2.81872224619115E-5, 2.96205589744786E-5, 2.96366306853944E-5,
    1.81558163161321E-5, 3.89133575436985E-5, 4.0541925545893E-5,
    4.09517286674768E-5, 4.13828697391234E-5, 4.27117839460953E-5,
    4.42707481302631E-5, 4.66178998453531E-5, 5.01094763460788E-5,
    5.10037192401537E-5, 5.15418458363369E-5, 1.10187386894303E-5,
    1.06412277899598E-5, 1.0353610967915E-5, 1.03934784183963E-5,
    8.5639434968992E-6, 9.0929501628703E-6, 9.82277057653517E-6,
    9.77101285983054E-6, 1.38321984452981E-5, 1.43576607639263E-5,
    1.44177044692347E-5, 1.20059467469746E-5, 5.52674219082358E-6,
    4.66003692391668E-6, 4.7326316221465E-6, 4.35499316718668E-6,
    4.41469277511559E-5, 5.08728159216815E-5, 6.88570240171309E-5,
    6.97270914990281E-5, 4.73611253769666E-5, 3.56743755197613E-5,
    3.63564399016609E-5, 3.6518164562841E-5, 4.37022736413165E-5,
    4.69672432351708E-5, 4.7978481028406E-5, 4.79505396765373E-5,
    4.10821431070587E-5, 4.14724221073489E-5, 4.34491060447629E-5,
    4.49659194999297E-5, 4.54407797370598E-5, 1.3270435627919E-5,
    1.48719537215883E-5, 1.47152765702373E-5, 1.47928731277922E-5,
    3.61120213586952E-5, 5.80190948824938E-6, 4.74930322312037E-6,
    6.27335518353028E-6, 6.6807737813614E-6, 1.20434440920977E-5,
    1.98960892158642E-5, 2.01848072532899E-5, 2.04216720797835E-5,
    1.70771820216372E-5, 1.26004450179482E-5, 1.42666514739049E-5,
    1.37870081017178E-5, 1.40945170604305E-5, 7.52944699405121E-6,
    7.38402181278621E-6, 8.43773381028382E-6, 8.50052997443641E-6,
    8.44693108327745E-6, 5.42978054944572E-6, 6.00246853325085E-6,
    8.38482107630594E-6, 8.33268964101138E-6, 9.25951641095946E-6,
    6.62388622680537E-6, 6.98826418798616E-6, 6.66047708169956E-6,
    6.12270982650271E-6, 5.64758483283395E-6, 6.34915168491389E-6,
    7.23341608124036E-6, 7.7438283745552E-6, 1.50376437437386E-5,
    1.48610711211012E-5, 1.61327911947025E-5, 1.54955433421784E-5,
    9.19863906980025E-6, 9.71227813571292E-6, 9.750390051758E-6,
    1.05093449753065E-5, 1.08296332098976E-5, 2.07344066592421E-5,
    2.31182138919437E-5, 2.15347856129738E-5, 2.08438591735808E-5,
    6.6410714169953E-6, 7.31685336965815E-6, 7.86245911232614E-6,
    7.96195045742541E-6, 7.41163614573609E-6, 7.75240057086366E-6,
    7.0559196159384E-6, 8.45067500697346E-6, 1.008165594284E-5,
    9.97499413126455E-6, 1.84432637126083E-5, 1.99016284585815E-5,
    2.11036455071838E-5, 2.06982048066743E-5, 4.16659209095464E-5,
    4.31777664661725E-5, 4.34985942647995E-5, 4.35871554175458E-5,
    3.85615062444496E-5, 3.89036971043549E-5, 3.81052660108055E-5,
    4.55421890135708E-5, 4.80609117719833E-5, 4.9650006239848E-5,
    5.13656877331065E-5, 1.27289102827078E-5, 1.23860036265192E-5,
    1.27967395317536E-5, 1.26928197668973E-5, 3.76070933633772E-5,
    3.89330733703542E-5, 3.92669869968394E-5, 3.91193428883946E-5,
    3.71617165241841E-5, 3.95656997931724E-5, 4.04395752163434E-5,
    4.05137965416394E-5, 4.62494156175458E-5, 4.87655657602354E-5,
    5.05044307032527E-5, 5.10125195204594E-5, 1.84718938132494E-5,
    3.43743185551389E-5, 3.42026919417836E-5, 3.44545755766661E-5,
    2.63962398569622E-5, 3.16994982384511E-5, 3.27366394851754E-5,
    3.40847162067211E-5, 3.43060909247858E-5, 3.59807668451218E-5,
    3.79137006045953E-5, 3.97355920101166E-5, 4.0393375577126E-5,
    3.51263780449679E-5, 3.70277596783328E-5, 3.75196220308133E-5,
    3.74096592740643E-5, 5.02177144960372E-5, 5.37340458384354E-5,
    5.55052951321816E-5, 5.63723243864944E-5, 7.61472372039495E-6,
    9.79967134149003E-6, 9.94390893006446E-6, 1.0058147721717E-5,
    6.82476468443892E-6, 5.19848594691581E-6, 6.12382416018823E-6,
    5.33947567164384E-6, 4.97863625231178E-6, 6.2051866499456E-6,
    6.56637171091509E-6, 6.16569227636222E-6, 4.82370217210009E-6,
    7.73995725861444E-6, 7.67830313591777E-6, 1.06195686835761E-5,
    9.64665264440838E-6, 7.0671087645878E-6, 4.5139517282156E-6,
    8.42299741496451E-6, 9.78336811244468E-6, 9.12698826970892E-6,
    7.9701678203989E-6, 8.60967680442287E-6, 1.10446504354873E-5,
    1.10164930687413E-5, 1.15800528095741E-5, 8.28407973738884E-6,
    5.9694525398717E-6, 1.14619862905385E-5, 1.23192300190703E-5,
    1.21961939089786E-5, 1.18968693890813E-5, 8.40767598063711E-6,
    9.94460033278023E-6, 9.78591427372427E-6, 6.6625222775138E-6,
    1.63644891098985E-5, 1.60821879043367E-5, 1.75719584010914E-5,
    1.35456708678907E-5, 7.9970302171452E-6, 8.60530108004309E-6,
    8.69828747856318E-6, 8.59483653073065E-6, 7.55005030486938E-6,
    9.78735382992623E-6, 8.61832654556435E-6, 9.11925354595694E-6,
    4.43622460167612E-6, 8.3198333047199E-6, 1.00776087904941E-5,
    8.37959213257651E-6, 7.71256130682406E-6, 9.39209960280087E-6,
    1.46233053503182E-5, 1.36799835449383E-5, 1.36451685763192E-5,
    1.1386781631937E-5, 6.06405937710357E-6, 5.02153607879451E-6,
    5.44176773998923E-6, 7.3982083607636E-6, 8.80782887028301E-6,
    1.88581273499193E-5, 1.88901182492596E-5, 1.88844293495433E-5,
    1.70819423318436E-5, 6.06836346550175E-6, 8.36318043917424E-6,
    8.67899000590226E-6, 8.15483209746688E-6, 1.29002151815974E-5,
    4.7523424320304E-6, 1.42437105647618E-5, 2.63453872134824E-5,
    2.68549213111796E-5, 2.66444172289047E-5, 1.9956135755498E-5,
    8.26507930400046E-6, 8.66810362373088E-6, 7.70377965350552E-6,
    7.87812993865641E-6, 6.85421482094754E-6, 5.34946822401411E-6,
    5.53472704673387E-6, 5.47667521633036E-6, 6.61054132791605E-6,
    4.84575979896259E-6, 9.21366499509777E-6, 1.27455144371321E-5,
    0.000143091607328284, 0.000333901563561698, 0.000275331923309551,
    0.000279576689581966, 0.000191025869854146, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.000303562854226023, 0.0, 8.09293802973913E-5,
    8.16022154274049E-5, 0.00044081134673781, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.000756257093015284, 0.000763606084599016, 0.00076286196946233,
    0.000762541877781699, 0.00245731425060248, 0.00285066604641989,
    0.00295310681690021, 0.00302137986524164, 0.00102935560418625,
    0.00274413599733416, 0.00285624977201209, 0.00295425116478369,
    0.00193971429409119, 0.000759146771973284, 0.00097959470831933,
    0.000967335897660065, 0.000966453188792035, 0.000104139587966369,
    0.00145435784383631, 0.00201307376535288, 0.00228543671003297,
    0.00236530681281886, 0.000873325839024809, 0.0, 0.0, 0.0, 0.0,
    0.000361330250512968, 0.00017082185037481, 0.00329642660078817,
    0.00363409575820274, 0.00376375129213071, 0.00386657730518834,
    0.000126513917125927, 0.000126046637381861, 0.000123413454745091,
    0.000124837769076607, 0.0, 0.0, 0.0, 0.0, 8.03363854541912E-5,
    8.05692884617902E-5, 0.000121173579938689, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.00332194549498715, 0.00394188765879148, 0.00473201950327336,
    0.0044473600820005, 0.0, 0.00248091969764703, 0.00258601827701157,
    0.00272212275690527, 0.0, 0.00051883893415005, 0.000377716671483689,
    0.000281822949573273, 8.17076271245343E-5, 0.00263613226569794,
    0.00292517298774928, 0.00310191778682509, 0.00318345565436741,
    0.00042731791928631, 0.000283777848936607, 0.000331434989602272,
    0.000334418206635255, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00033826733474072,
    0.000691613669410048, 0.000873351198879721, 0.000821219037258393,
    0.000407841565932524, 0.000377461811523026, 0.000432832362816615,
    0.00042751002819551, 0.000426744740538352, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.000136187907531817, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.000560400268219224, 0.00056440625729796, 0.0006182090871114,
    0.000537196573412083, 0.000131429845867678, 0.0, 0.0, 0.0, 0.0,
    0.00100526160387608, 0.00122254095270785, 0.00108377516143577,
    0.00103508523291586, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.000911273072555066, 0.000904437444300592, 0.000942679487263815,
    0.000995099857100769, 0.00288211386209653, 0.00310136114878705,
    0.00325195776199999, 0.00329672358955925, 0.0, 0.0, 0.0, 0.00326450933528058,
    0.00348604558032557, 0.00373248907373771, 0.00388099415203611,
    0.00012303120745243, 0.000166188936063882, 0.000211302109210978,
    0.000166718589842054, 0.00254702993759341, 0.00272047633232276,
    0.00279285202289349, 0.00284335335535088, 0.0, 0.0, 0.0, 0.0,
    0.00333902419191708, 0.00365136869319715, 0.00380824843127255,
    0.0038959841339236, 0.000707752241424, 0.00146733488112957,
    0.00146747444545746, 0.00144273443246871, 0.0, 0.00212470982513485,
    0.00235961795630628, 0.00267009781163889, 0.0028239268829799, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0035123365322066, 0.00399334328063305,
    0.00418605133305136, 0.00439476942617886, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.00054613695242152, 0.000553865523295996, 0.000545454110654338,
    0.000238887400067461, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000338398995982107, 0.00103602133502249,
    0.00104091333056966, 0.00104586864564139, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000395702625567868, 0.0, 0.0, 0.0,
    0.00326552828959289, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000205716652512185,
    0.000260627430468318, 0.000260953866984082, 0.000263278192430687, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.00257347031357416, 0.00270170306667084, 0.0028938524985722,
    0.00294147556412285, 0.00280633230059206, 0.0030251163489355, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.000102571253157949, 0.00081743161346363, 0.00378657904644944,
    0.00417388800133558, 0.00424868614336778, 0.0, 0.0, 0.0, 0.00309978861381771,
    0.00338158585147882, 0.00356678722374382, 0.00365646085095714,
    0.00251710753346897, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.00206077529471204, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000318226134098611,
    5.01835800181827E-5, 0.000101800984227168, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000259565911106608,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.00258201292597228, 0.002714716807081,
    0.00261896084527281, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.00230377769820667, 0.00268958861669701, 0.00298208291919513,
    0.00305456102180307, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00183943753626137,
    0.00183877414468889, 0.00186722546124865, 0.00192666190099969, 0.0, 0.0, 0.0,
    0.0, 0.00269763713455608, 0.0028342937587419, 0.00311505648533428,
    0.00325267017441681, 0.00230857458226852, 0.00259877572703042,
    0.00276891318706866, 0.00284469801138761, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.000612259214126708, 0.000604953420955934, 0.000618941930760512,
    0.00061121431572269, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00113777570264078,
    0.00113173349200776, 0.00112571861957335, 0.00114445162927772, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 8.49564138695048E-7,
    -4.19717619557316E-6, -4.2075973271307E-6, -4.20545982988297E-6,
    -4.19983331895179E-6, -1.8417279728271E-6, 1.10676017892016E-6,
    1.09939208530927E-6, 1.10007391604188E-6, 4.99350940604835E-7,
    -1.23215128890582E-6, 9.42244146451346E-8, 9.43392714210916E-7,
    1.61874116084912E-7, -2.87203770465389E-7, 1.23411069087588E-6,
    6.38929043127496E-6, -4.04770945426145E-6, -4.04067084838672E-6,
    -4.03862863159142E-6, -1.03738467465894E-5, 1.06152922264587E-7,
    -7.47781235336604E-7, -7.44044030025222E-7, 4.08008324238002E-7,
    -2.18147641383366E-7, -6.44888864375127E-6, -6.4412893362639E-6,
    -6.44097127642359E-6, -6.44029689214228E-6, 8.26088269026673E-6,
    -2.64504217617491E-6, -1.62103647538862E-6, 9.15471079405683E-6,
    9.52889146535111E-6, -3.33196471460274E-6, -8.02576110064316E-6,
    -8.00229197692554E-6, -9.16454999736642E-7, 4.70713354072388E-6,
    -5.88657463654184E-6, -5.89201876911246E-6, -5.89211404353898E-6,
    -6.33770957745837E-6, 7.05552958784636E-6, -2.96523324898979E-6,
    -9.89366417988833E-7, -6.52189798153403E-7, -4.91126891427327E-6,
    -8.32758939507373E-6, -8.34088673666771E-6, -8.35854850355459E-6,
    -8.37010719420987E-6, 8.79013828433968E-6, 4.06875275369261E-6,
    1.02155142941082E-5, -3.87568877620457E-6, -3.8574791841608E-6,
    -3.83282079067345E-6, 3.96595996885534E-7, 3.98582784694387E-7,
    -3.64015685655144E-6, -3.63369560416211E-6, -3.95239831800971E-7,
    -4.1013897790703E-7, 5.89068838141994E-7, 5.88629672966714E-7,
    -3.28803370343612E-6, -3.28631683403673E-6, -3.2853927731843E-6,
    -3.55797632680111E-6, 4.83975178031884E-7, 4.15079315990498E-7,
    1.13445611106354E-7, 4.15562320484449E-7, -6.33691055447572E-6,
    -3.23377570083331E-6, -3.80367824916538E-6, -3.8380293889537E-6,
    2.97222040472027E-6, 7.53107693130892E-6, -1.19365999477921E-6,
    -1.18726926743903E-6, 7.86018788765233E-6, 4.91916639690843E-6,
    4.89097341034667E-6, 4.88199122572667E-6, -9.1889305695599E-6,
    1.09923355084699E-5, 1.1018627110693E-5, 1.10456943763345E-5,
    1.106484369677E-5, 3.31732789249544E-6, -5.05289754866742E-6,
    -5.04834021286139E-6, -5.04338941620198E-6, 7.50593338644394E-6,
    7.87187970408686E-7, 1.41932531759034E-6, -2.92962681466688E-7,
    -2.95666787562288E-7, 1.09565903086095E-6, -3.16827937909152E-6,
    -3.16238456314982E-6, -3.16636053496617E-6, -3.25222748678285E-6,
    1.4784412504894E-6, -1.26436737649215E-6, -3.97472299014229E-6,
    -3.97498459146865E-6, -4.24869809014156E-6, -7.4292704395205E-7,
    3.860219630527E-7, 3.77096865981639E-7, 3.71050097873842E-7,
    -2.02207416398089E-7, 4.99699652897779E-7, 3.84182061105019E-7,
    3.87361356988685E-7, 8.20529391420936E-7, -2.11027204576565E-7,
    -2.11459322200475E-7, -1.38755395132675E-6, -1.39016849565995E-6,
    -2.43531028436083E-7, -2.43117120111898E-7, 4.0984878911722E-7,
    1.76483013095799E-7, -3.69569630468136E-6, -3.69325245819713E-6,
    -3.68831403969207E-6, -3.68538366819108E-6, 9.85587082120763E-7,
    8.97611249328405E-7, -1.08442595194581E-6, -1.07732478990293E-6,
    4.54044158932174E-7, -3.11046943964715E-6, -1.17365285840934E-6,
    -3.10236672049227E-6, -3.10437669422969E-6, -1.68061482009679E-7,
    5.73983850382598E-7, 5.71816624555354E-7, 5.6949550490972E-7,
    3.10507408103038E-7, -2.89653578578341E-7, 3.74816419023949E-7,
    2.65835266000215E-7, -4.81820065542777E-7, -4.75017506018197E-7,
    -3.93268609189379E-6, -3.70096401083411E-6, -3.69071181244067E-6,
    -3.68673476746098E-6, -8.15775581117069E-6, -8.14988404407171E-6,
    -8.11912860575936E-6, -8.10779857726094E-6, 1.51813341051959E-6,
    -7.87760495470523E-6, 1.48606504558155E-6, -9.14500329575313E-6,
    -8.98221562800916E-6, -8.95350351040864E-6, -1.67528222440608E-6,
    -3.194546504399E-6, -3.44952784920522E-6, -3.44428033054286E-6,
    -3.4511514060913E-6, -3.35813702090062E-6, -1.56365960934959E-6,
    -1.99051807890127E-6, -7.83905374360127E-7, -7.3380357449222E-6,
    -7.06098663831913E-6, -7.0942603556574E-6, -7.10922410982761E-6,
    -9.1109312887374E-6, -8.202343740838E-6, -2.06718365604139E-6,
    -2.04557482415287E-6, 4.96110795104964E-7, -5.81685125662034E-6,
    -5.8162970419205E-6, -5.82280609025453E-6, -6.8998517228559E-6,
    2.69508101778787E-6, -7.13556573104708E-6, -7.10529484568225E-6,
    2.84697538601639E-6, -7.32101188084885E-6, 3.41209427067561E-6,
    1.49792165476797E-6, 8.80681095345216E-7, -7.18940654396395E-6,
    1.56104545258707E-6, -7.24532807534066E-6, -7.83164691877368E-6,
    1.12957027983139E-5, 1.13176244588221E-5, 1.13512733245249E-5,
    -1.56937856385109E-6, -1.8056809114771E-6, -1.36375184519523E-6,
    -1.36288247130212E-6, -1.361036832284E-6, -1.42804051371057E-6,
    2.37948133281872E-7, 5.61753828318777E-7, -2.40831644514825E-7,
    4.12107975881415E-7, -1.28745062555787E-6, 2.96556431103471E-7,
    -1.28798443614229E-6, 1.47935836350316E-7, -2.67431302872569E-6,
    -2.66055389342715E-6, 2.01301010671567E-6, -3.97208464044599E-7,
    2.56543369405395E-7, 6.61998600187103E-7, -8.04173605565655E-7,
    -4.65890157406741E-7, -8.23596449037232E-7, -8.38569380634444E-7,
    -1.92395202504141E-6, -7.71848509045957E-7, -7.71085715630922E-7,
    -7.79951351194322E-7, 4.42842916807602E-7, -1.23221665071545E-6,
    -3.64427690392848E-6, -3.62345682278649E-6, -3.6235587035169E-6,
    -3.62774112877109E-6, 8.28733487765823E-7, -6.39986408070782E-7,
    -6.63806137452851E-7, -1.47630607435326E-7, -1.9775231244639E-6,
    -1.99522089329914E-6, -2.00087533284703E-6, -2.06205227847025E-6,
    -1.15921988213105E-6, -4.08303714959983E-7, -3.39855028214878E-7,
    -3.40276004741257E-7, 3.29108792155165E-7, -3.32108270506722E-6,
    -3.31055049632827E-6, -3.31988361258708E-6, -1.81348067856115E-6,
    -1.70701683214611E-6, 6.25494366256846E-7, -1.70719160473294E-6,
    -1.70980837230797E-6, -1.83544039777788E-6, 2.43036949937725E-6,
    -8.67312096209247E-7, -8.61583785187195E-7, -2.40097179248692E-6,
    1.9246011794282E-7, -3.47314450569945E-7, -3.48890618986895E-7,
    2.28215577225436E-7, 5.34802036861502E-7, -1.49174666196114E-6,
    -1.48918002642755E-6, -1.49566405996092E-6, -4.53438382952305E-6,
    -4.60297450321878E-7, 5.28650232131974E-7, 5.59551594137333E-7,
    5.54154897578954E-7, 1.19275353713636E-6, 9.08275513924109E-7,
    8.47447034806496E-7, -2.1916753173678E-6, -2.18847307541884E-6,
    -2.18656216629047E-6, -3.45383975717481E-6, 2.65723140058406E-7,
    2.79535726833653E-7, -1.95941401632896E-6, -1.9598843513877E-6,
    -2.19330580584232E-6, 1.27224146185796E-6, -7.64094985865816E-7,
    -7.57759259824273E-7, -1.95815289953922E-7, 4.34260076395816E-7,
    4.40584650312813E-7, -1.16454827345199E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 8.0, 0.0, 0.0, 0.0, 26.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 5.0, 6.0, 6.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 22.0, 22.0, 24.0,
    24.0, 24.0, 25.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0, 9.0, 35.0, 37.0, 37.0, 0.0, 0.0,
    0.0, 25.0, 26.0, 27.0, 28.0, 23.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 7.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 24.0, 25.0, 23.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 24.0, 26.0, 28.0, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 23.0, 23.0,
    23.0, 24.0, 0.0, 0.0, 0.0, 0.0, 30.0, 27.0, 29.0, 30.0, 25.0, 27.0, 28.0,
    28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 11.0, 11.0, 11.0, 11.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 18.0, 18.0, 18.0, 18.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, 8.0, 7.0, 7.0, 5.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6.0, 0.0, 3.0, 3.0, 6.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 13.0, 13.0, 13.0, 13.0, 22.0, 25.0, 25.0, 25.0, 10.0,
    23.0, 24.0, 24.0, 15.0, 12.0, 15.0, 15.0, 15.0, 3.0, 18.0, 30.0, 32.0, 33.0,
    15.0, 0.0, 0.0, 0.0, 0.0, 7.0, 5.0, 26.0, 27.0, 28.0, 28.0, 4.0, 4.0, 4.0,
    4.0, 0.0, 0.0, 0.0, 0.0, 3.0, 3.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 31.0, 34.0,
    40.0, 38.0, 0.0, 25.0, 25.0, 27.0, 0.0, 11.0, 9.0, 7.0, 3.0, 21.0, 23.0,
    24.0, 24.0, 10.0, 7.0, 8.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 8.0, 15.0, 18.0,
    17.0, 9.0, 9.0, 10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 13.0, 13.0, 14.0, 12.0,
    4.0, 0.0, 0.0, 0.0, 0.0, 20.0, 23.0, 21.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 17.0, 17.0, 17.0, 18.0, 25.0, 27.0, 27.0, 27.0, 0.0,
    0.0, 0.0, 26.0, 26.0, 28.0, 28.0, 4.0, 5.0, 6.0, 5.0, 24.0, 26.0, 26.0, 26.0,
    0.0, 0.0, 0.0, 0.0, 26.0, 28.0, 28.0, 28.0, 16.0, 24.0, 24.0, 24.0, 1.0,
    28.0, 30.0, 33.0, 34.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 25.0, 28.0,
    28.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 12.0, 12.0, 12.0, 6.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 9.0, 22.0, 22.0, 22.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  double dist[294];
  int c_k;
  emxArray_real_T *edges;
  double s;
  int exitg4;
  int exitg3;
  emxArray_real_T *nn;
  short outsize_idx_0;
  boolean_T guard1 = false;
  int exitg2;
  boolean_T exitg1;
  for (k = 0; k <= 293; k += 2) {
    if (a[k] <= a[k + 1]) {
      idx[k] = k + 1;
      idx[k + 1] = k + 2;
    } else {
      idx[k] = k + 2;
      idx[k + 1] = k + 1;
    }
  }

  i = 2;
  while (i < 294) {
    high_i = i << 1;
    nb = 1;
    for (mid_i = 1 + i; mid_i < 295; mid_i = qEnd + i) {
      p = nb;
      q = mid_i;
      qEnd = nb + high_i;
      if (qEnd > 295) {
        qEnd = 295;
      }

      k = 0;
      kEnd = qEnd - nb;
      while (k + 1 <= kEnd) {
        if (a[idx[p - 1] - 1] <= a[idx[q - 1] - 1]) {
          iwork[k] = idx[p - 1];
          p++;
          if (p == mid_i) {
            while (q < qEnd) {
              k++;
              iwork[k] = idx[q - 1];
              q++;
            }
          }
        } else {
          iwork[k] = idx[q - 1];
          q++;
          if (q == qEnd) {
            while (p < mid_i) {
              k++;
              iwork[k] = idx[p - 1];
              p++;
            }
          }
        }

        k++;
      }

      for (k = 0; k + 1 <= kEnd; k++) {
        idx[(nb + k) - 1] = iwork[k];
      }

      nb = qEnd;
    }

    i = high_i;
  }

  emxInit_real_T(&Uc, 1);
  high_i = Uc->size[0];
  Uc->size[0] = 294;
  emxEnsureCapacity((emxArray__common *)Uc, high_i, (int)sizeof(double));
  for (k = 0; k < 294; k++) {
    Uc->data[k] = a[idx[k] - 1];
  }

  nb = -1;
  k = 0;
  while (k + 1 <= 294) {
    i = (int)Uc->data[k];
    do {
      exitg5 = 0;
      k++;
      if (k + 1 > 294) {
        exitg5 = 1;
      } else {
        eok = (std::fabs((double)i - Uc->data[k]) < eps((double)i / 2.0));
        if (!eok) {
          exitg5 = 1;
        }
      }
    } while (exitg5 == 0);

    nb++;
    Uc->data[nb] = i;
  }

  high_i = Uc->size[0];
  if (1 > nb + 1) {
    i3 = -1;
  } else {
    i3 = nb;
  }

  Uc->size[0] = i3 + 1;
  emxEnsureCapacity((emxArray__common *)Uc, high_i, (int)sizeof(double));
  for (high_i = 0; high_i < 294; high_i++) {
    for (i = 0; i < 26; i++) {
      x[high_i + 294 * i] = tX[high_i + 294 * i] - tsX[i];
    }
  }

  for (b_k = 1; b_k < 7645; b_k++) {
    c_k = b_k;
    y[c_k - 1] = x[c_k - 1] * x[c_k - 1];
  }

  for (nb = 0; nb < 294; nb++) {
    s = y[nb];
    for (k = 0; k < 25; k++) {
      s += y[nb + (k + 1) * 294];
    }

    dist[nb] = s;
  }

  emxInit_real_T1(&edges, 2);
  c_sort(dist, idx);
  i = Uc->size[0];
  high_i = edges->size[0] * edges->size[1];
  edges->size[0] = 1;
  edges->size[1] = (short)(i + 1);
  emxEnsureCapacity((emxArray__common *)edges, high_i, (int)sizeof(double));
  k = 0;
  do {
    exitg4 = 0;
    i = Uc->size[0];
    if (k <= i - 2) {
      edges->data[1 + k] = Uc->data[k] + (Uc->data[1 + k] - Uc->data[k]) / 2.0;
      k++;
    } else {
      exitg4 = 1;
    }
  } while (exitg4 == 0);

  edges->data[0] = rtMinusInf;
  edges->data[edges->size[1] - 1] = rtInf;
  k = 1;
  do {
    exitg3 = 0;
    i = Uc->size[0];
    if (k - 1 <= i - 2) {
      edges->data[k] += eps(edges->data[k]);
      k++;
    } else {
      exitg3 = 1;
    }
  } while (exitg3 == 0);

  emxInit_real_T(&nn, 1);
  outsize_idx_0 = (short)edges->size[1];
  high_i = nn->size[0];
  nn->size[0] = outsize_idx_0;
  emxEnsureCapacity((emxArray__common *)nn, high_i, (int)sizeof(double));
  i = outsize_idx_0;
  for (high_i = 0; high_i < i; high_i++) {
    nn->data[high_i] = 0.0;
  }

  i = edges->size[1];
  guard1 = false;
  if (i > 1) {
    nb = 1;
    do {
      exitg2 = 0;
      if (nb + 1 <= i) {
        if (!(edges->data[nb] >= edges->data[nb - 1])) {
          eok = false;
          exitg2 = 1;
        } else {
          nb++;
        }
      } else {
        guard1 = true;
        exitg2 = 1;
      }
    } while (exitg2 == 0);
  } else {
    guard1 = true;
  }

  if (guard1) {
    eok = true;
  }

  if (!eok) {
    high_i = nn->size[0];
    nn->size[0] = outsize_idx_0;
    emxEnsureCapacity((emxArray__common *)nn, high_i, (int)sizeof(double));
    i = outsize_idx_0;
    for (high_i = 0; high_i < i; high_i++) {
      nn->data[high_i] = rtNaN;
    }
  } else {
    i = 0;
    if ((a[idx[0] - 1] >= edges->data[0]) && (a[idx[0] - 1] < edges->data
         [edges->size[1] - 1])) {
      i = 1;
      nb = 2;
      high_i = edges->size[1];
      while (high_i > nb) {
        mid_i = (i >> 1) + (high_i >> 1);
        if (((i & 1) == 1) && ((high_i & 1) == 1)) {
          mid_i++;
        }

        if (a[idx[0] - 1] >= edges->data[mid_i - 1]) {
          i = mid_i;
          nb = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }
    }

    if (a[idx[0] - 1] == edges->data[edges->size[1] - 1]) {
      i = edges->size[1];
    }

    if (i > 0) {
      nn->data[i - 1] = 1.0;
    }
  }

  high_i = edges->size[0] * edges->size[1];
  edges->size[0] = 1;
  edges->size[1] = nn->size[0] - 1;
  emxEnsureCapacity((emxArray__common *)edges, high_i, (int)sizeof(double));
  for (k = 0; k <= nn->size[0] - 2; k++) {
    edges->data[k] = nn->data[k];
  }

  if (nn->size[0] - 1 > 0) {
    edges->data[edges->size[1] - 1] += nn->data[nn->size[0] - 1];
  }

  emxFree_real_T(&nn);
  i = 1;
  nb = edges->size[1];
  s = edges->data[0];
  high_i = 0;
  if (edges->size[1] > 1) {
    if (rtIsNaN(edges->data[0])) {
      mid_i = 2;
      exitg1 = false;
      while ((!exitg1) && (mid_i <= nb)) {
        i = mid_i;
        if (!rtIsNaN(edges->data[mid_i - 1])) {
          s = edges->data[mid_i - 1];
          high_i = mid_i - 1;
          exitg1 = true;
        } else {
          mid_i++;
        }
      }
    }

    if (i < edges->size[1]) {
      while (i + 1 <= nb) {
        if (edges->data[i] > s) {
          s = edges->data[i];
          high_i = i;
        }

        i++;
      }
    }
  }

  emxFree_real_T(&edges);
  yfit = Uc->data[high_i];
  emxFree_real_T(&Uc);
  return yfit;
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real_T *x
//                int offset
//                int np
//                int nq
//                emxArray_int32_T *iwork
//                emxArray_real_T *xwork
// Return Type  : void
//
static void merge(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int np,
                  int nq, emxArray_int32_T *iwork, emxArray_real_T *xwork)
{
  int n;
  int qend;
  int p;
  int iout;
  int exitg1;
  if ((np == 0) || (nq == 0)) {
  } else {
    n = np + nq;
    for (qend = 0; qend + 1 <= n; qend++) {
      iwork->data[qend] = idx->data[offset + qend];
      xwork->data[qend] = x->data[offset + qend];
    }

    p = 0;
    n = np;
    qend = np + nq;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork->data[p] >= xwork->data[n]) {
        idx->data[iout] = iwork->data[p];
        x->data[iout] = xwork->data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx->data[iout] = iwork->data[n];
        x->data[iout] = xwork->data[n];
        if (n + 1 < qend) {
          n++;
        } else {
          n = iout - p;
          while (p + 1 <= np) {
            idx->data[(n + p) + 1] = iwork->data[p];
            x->data[(n + p) + 1] = xwork->data[p];
            p++;
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real_T *x
//                int offset
//                int n
//                int preSortLevel
//                emxArray_int32_T *iwork
//                emxArray_real_T *xwork
// Return Type  : void
//
static void merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset,
  int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_real_T *xwork)
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 1; nTail <= nPairs; nTail++) {
      merge(idx, x, offset + (nTail - 1) * tailOffset, bLen, bLen, iwork, xwork);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
  }
}

//
// Arguments    : int idx[294]
//                double x[294]
//                int offset
// Return Type  : void
//
static void merge_pow2_block(int idx[294], double x[294], int offset)
{
  int iwork[256];
  double xwork[256];
  int b;
  int bLen;
  int bLen2;
  int nPairs;
  int k;
  int blockOffset;
  int q;
  int p;
  int exitg1;
  for (b = 0; b < 6; b++) {
    bLen = 1 << (b + 2);
    bLen2 = bLen << 1;
    nPairs = 256 >> (b + 3);
    for (k = 1; k <= nPairs; k++) {
      blockOffset = (offset + (k - 1) * bLen2) - 1;
      for (q = 1; q <= bLen2; q++) {
        iwork[q - 1] = idx[blockOffset + q];
        xwork[q - 1] = x[blockOffset + q];
      }

      p = 0;
      q = bLen;
      do {
        exitg1 = 0;
        blockOffset++;
        if (xwork[p] <= xwork[q]) {
          idx[blockOffset] = iwork[p];
          x[blockOffset] = xwork[p];
          if (p + 1 < bLen) {
            p++;
          } else {
            exitg1 = 1;
          }
        } else {
          idx[blockOffset] = iwork[q];
          x[blockOffset] = xwork[q];
          if (q + 1 < bLen2) {
            q++;
          } else {
            q = blockOffset - p;
            while (p + 1 <= bLen) {
              idx[(q + p) + 1] = iwork[p];
              x[(q + p) + 1] = xwork[p];
              p++;
            }

            exitg1 = 1;
          }
        }
      } while (exitg1 == 0);
    }
  }
}

//
// Arguments    : const double Y[250]
//                const emxArray_real_T *iPk
//                emxArray_real_T *idx
// Return Type  : void
//
static void orderPeaks(const double Y[250], const emxArray_real_T *iPk,
  emxArray_real_T *idx)
{
  emxArray_real_T *x;
  int i4;
  int loop_ub;
  emxArray_int32_T *iidx;
  emxArray_real_T *b_idx;
  if (idx->size[0] == 0) {
  } else {
    emxInit_real_T(&x, 1);
    i4 = x->size[0];
    x->size[0] = idx->size[0];
    emxEnsureCapacity((emxArray__common *)x, i4, (int)sizeof(double));
    loop_ub = idx->size[0];
    for (i4 = 0; i4 < loop_ub; i4++) {
      x->data[i4] = Y[(int)iPk->data[(int)idx->data[i4] - 1] - 1];
    }

    emxInit_int32_T1(&iidx, 1);
    emxInit_real_T(&b_idx, 1);
    sort(x, iidx);
    i4 = b_idx->size[0];
    b_idx->size[0] = iidx->size[0];
    emxEnsureCapacity((emxArray__common *)b_idx, i4, (int)sizeof(double));
    loop_ub = iidx->size[0];
    emxFree_real_T(&x);
    for (i4 = 0; i4 < loop_ub; i4++) {
      b_idx->data[i4] = idx->data[iidx->data[i4] - 1];
    }

    emxFree_int32_T(&iidx);
    i4 = idx->size[0];
    idx->size[0] = b_idx->size[0];
    emxEnsureCapacity((emxArray__common *)idx, i4, (int)sizeof(double));
    loop_ub = b_idx->size[0];
    for (i4 = 0; i4 < loop_ub; i4++) {
      idx->data[i4] = b_idx->data[i4];
    }

    emxFree_real_T(&b_idx);
  }
}

//
// Arguments    : const double Y[250]
//                emxArray_real_T *iPk
//                double Ph
// Return Type  : void
//
static void removePeaksBelowMinPeakHeight(const double Y[250], emxArray_real_T
  *iPk, double Ph)
{
  int end;
  int trueCount;
  int i;
  int partialTrueCount;
  if (!(iPk->size[0] == 0)) {
    end = iPk->size[0] - 1;
    trueCount = 0;
    for (i = 0; i <= end; i++) {
      if (Y[(int)iPk->data[i] - 1] > Ph) {
        trueCount++;
      }
    }

    partialTrueCount = 0;
    for (i = 0; i <= end; i++) {
      if (Y[(int)iPk->data[i] - 1] > Ph) {
        iPk->data[partialTrueCount] = iPk->data[i];
        partialTrueCount++;
      }
    }

    end = iPk->size[0];
    iPk->size[0] = trueCount;
    emxEnsureCapacity((emxArray__common *)iPk, end, (int)sizeof(double));
  }
}

//
// Arguments    : const double Y[250]
//                emxArray_real_T *iPk
//                double Th
// Return Type  : void
//
static void removePeaksBelowThreshold(const double Y[250], emxArray_real_T *iPk,
  double Th)
{
  int c;
  emxArray_real_T *base;
  int k;
  int trueCount;
  double extremum;
  int partialTrueCount;
  c = iPk->size[0];
  emxInit_real_T(&base, 1);
  k = base->size[0];
  base->size[0] = c;
  emxEnsureCapacity((emxArray__common *)base, k, (int)sizeof(double));
  for (k = 0; k + 1 <= c; k++) {
    if ((Y[(int)(iPk->data[k] - 1.0) - 1] >= Y[(int)(iPk->data[k] + 1.0) - 1]) ||
        rtIsNaN(Y[(int)(iPk->data[k] + 1.0) - 1])) {
      extremum = Y[(int)(iPk->data[k] - 1.0) - 1];
    } else {
      extremum = Y[(int)(iPk->data[k] + 1.0) - 1];
    }

    base->data[k] = extremum;
  }

  k = iPk->size[0] - 1;
  trueCount = 0;
  for (c = 0; c <= k; c++) {
    if (Y[(int)iPk->data[c] - 1] - base->data[c] >= Th) {
      trueCount++;
    }
  }

  partialTrueCount = 0;
  for (c = 0; c <= k; c++) {
    if (Y[(int)iPk->data[c] - 1] - base->data[c] >= Th) {
      iPk->data[partialTrueCount] = iPk->data[c];
      partialTrueCount++;
    }
  }

  emxFree_real_T(&base);
  k = iPk->size[0];
  iPk->size[0] = trueCount;
  emxEnsureCapacity((emxArray__common *)iPk, k, (int)sizeof(double));
}

//
// Arguments    : int *k
//                const emxArray_real_T *x
// Return Type  : double
//
static double skip_to_last_equal_value(int *k, const emxArray_real_T *x)
{
  double xk;
  boolean_T exitg1;
  boolean_T p;
  xk = x->data[*k - 1];
  exitg1 = false;
  while ((!exitg1) && (*k < x->size[0])) {
    if ((std::fabs(xk - x->data[*k]) < eps(xk / 2.0)) || (rtIsInf(x->data[*k]) &&
         rtIsInf(xk) && ((x->data[*k] > 0.0) == (xk > 0.0)))) {
      p = true;
    } else {
      p = false;
    }

    if (p) {
      (*k)++;
    } else {
      exitg1 = true;
    }
  }

  return xk;
}

//
// Arguments    : emxArray_real_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
static void sort(emxArray_real_T *x, emxArray_int32_T *idx)
{
  int dim;
  dim = 2;
  if (x->size[0] != 1) {
    dim = 1;
  }

  b_sort(x, dim, idx);
}

//
// Arguments    : emxArray_real_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
static void sortIdx(emxArray_real_T *x, emxArray_int32_T *idx)
{
  emxArray_real_T *b_x;
  unsigned int unnamed_idx_0;
  int ib;
  int m;
  int n;
  double x4[4];
  int idx4[4];
  emxArray_int32_T *iwork;
  emxArray_real_T *xwork;
  int nNaNs;
  int k;
  int wOffset;
  signed char perm[4];
  int nNonNaN;
  int p;
  int i4;
  int nBlocks;
  int b_iwork[256];
  double b_xwork[256];
  int b;
  int bLen;
  int bLen2;
  int nPairs;
  int exitg1;
  emxInit_real_T(&b_x, 1);
  unnamed_idx_0 = (unsigned int)x->size[0];
  ib = b_x->size[0];
  b_x->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)b_x, ib, (int)sizeof(double));
  m = x->size[0];
  for (ib = 0; ib < m; ib++) {
    b_x->data[ib] = x->data[ib];
  }

  ib = idx->size[0];
  idx->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)idx, ib, (int)sizeof(int));
  m = (int)unnamed_idx_0;
  for (ib = 0; ib < m; ib++) {
    idx->data[ib] = 0;
  }

  n = x->size[0];
  for (m = 0; m < 4; m++) {
    x4[m] = 0.0;
    idx4[m] = 0;
  }

  emxInit_int32_T1(&iwork, 1);
  ib = iwork->size[0];
  iwork->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)iwork, ib, (int)sizeof(int));
  m = iwork->size[0];
  ib = iwork->size[0];
  iwork->size[0] = m;
  emxEnsureCapacity((emxArray__common *)iwork, ib, (int)sizeof(int));
  for (ib = 0; ib < m; ib++) {
    iwork->data[ib] = 0;
  }

  emxInit_real_T(&xwork, 1);
  unnamed_idx_0 = (unsigned int)x->size[0];
  ib = xwork->size[0];
  xwork->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)xwork, ib, (int)sizeof(double));
  m = xwork->size[0];
  ib = xwork->size[0];
  xwork->size[0] = m;
  emxEnsureCapacity((emxArray__common *)xwork, ib, (int)sizeof(double));
  for (ib = 0; ib < m; ib++) {
    xwork->data[ib] = 0.0;
  }

  nNaNs = 0;
  ib = 0;
  for (k = 0; k + 1 <= n; k++) {
    if (rtIsNaN(b_x->data[k])) {
      idx->data[(n - nNaNs) - 1] = k + 1;
      xwork->data[(n - nNaNs) - 1] = b_x->data[k];
      nNaNs++;
    } else {
      ib++;
      idx4[ib - 1] = k + 1;
      x4[ib - 1] = b_x->data[k];
      if (ib == 4) {
        ib = k - nNaNs;
        if (x4[0] >= x4[1]) {
          m = 1;
          wOffset = 2;
        } else {
          m = 2;
          wOffset = 1;
        }

        if (x4[2] >= x4[3]) {
          p = 3;
          i4 = 4;
        } else {
          p = 4;
          i4 = 3;
        }

        if (x4[m - 1] >= x4[p - 1]) {
          if (x4[wOffset - 1] >= x4[p - 1]) {
            perm[0] = (signed char)m;
            perm[1] = (signed char)wOffset;
            perm[2] = (signed char)p;
            perm[3] = (signed char)i4;
          } else if (x4[wOffset - 1] >= x4[i4 - 1]) {
            perm[0] = (signed char)m;
            perm[1] = (signed char)p;
            perm[2] = (signed char)wOffset;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)m;
            perm[1] = (signed char)p;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)wOffset;
          }
        } else if (x4[m - 1] >= x4[i4 - 1]) {
          if (x4[wOffset - 1] >= x4[i4 - 1]) {
            perm[0] = (signed char)p;
            perm[1] = (signed char)m;
            perm[2] = (signed char)wOffset;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)p;
            perm[1] = (signed char)m;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)wOffset;
          }
        } else {
          perm[0] = (signed char)p;
          perm[1] = (signed char)i4;
          perm[2] = (signed char)m;
          perm[3] = (signed char)wOffset;
        }

        idx->data[ib - 3] = idx4[perm[0] - 1];
        idx->data[ib - 2] = idx4[perm[1] - 1];
        idx->data[ib - 1] = idx4[perm[2] - 1];
        idx->data[ib] = idx4[perm[3] - 1];
        b_x->data[ib - 3] = x4[perm[0] - 1];
        b_x->data[ib - 2] = x4[perm[1] - 1];
        b_x->data[ib - 1] = x4[perm[2] - 1];
        b_x->data[ib] = x4[perm[3] - 1];
        ib = 0;
      }
    }
  }

  wOffset = (x->size[0] - nNaNs) - 1;
  if (ib > 0) {
    for (m = 0; m < 4; m++) {
      perm[m] = 0;
    }

    if (ib == 1) {
      perm[0] = 1;
    } else if (ib == 2) {
      if (x4[0] >= x4[1]) {
        perm[0] = 1;
        perm[1] = 2;
      } else {
        perm[0] = 2;
        perm[1] = 1;
      }
    } else if (x4[0] >= x4[1]) {
      if (x4[1] >= x4[2]) {
        perm[0] = 1;
        perm[1] = 2;
        perm[2] = 3;
      } else if (x4[0] >= x4[2]) {
        perm[0] = 1;
        perm[1] = 3;
        perm[2] = 2;
      } else {
        perm[0] = 3;
        perm[1] = 1;
        perm[2] = 2;
      }
    } else if (x4[0] >= x4[2]) {
      perm[0] = 2;
      perm[1] = 1;
      perm[2] = 3;
    } else if (x4[1] >= x4[2]) {
      perm[0] = 2;
      perm[1] = 3;
      perm[2] = 1;
    } else {
      perm[0] = 3;
      perm[1] = 2;
      perm[2] = 1;
    }

    for (k = 1; k <= ib; k++) {
      idx->data[(wOffset - ib) + k] = idx4[perm[k - 1] - 1];
      b_x->data[(wOffset - ib) + k] = x4[perm[k - 1] - 1];
    }
  }

  m = nNaNs >> 1;
  for (k = 1; k <= m; k++) {
    ib = idx->data[wOffset + k];
    idx->data[wOffset + k] = idx->data[n - k];
    idx->data[n - k] = ib;
    b_x->data[wOffset + k] = xwork->data[n - k];
    b_x->data[n - k] = xwork->data[wOffset + k];
  }

  if ((nNaNs & 1) != 0) {
    b_x->data[(wOffset + m) + 1] = xwork->data[(wOffset + m) + 1];
  }

  nNonNaN = x->size[0] - nNaNs;
  m = 2;
  if (nNonNaN > 1) {
    if (x->size[0] >= 256) {
      nBlocks = nNonNaN >> 8;
      if (nBlocks > 0) {
        for (i4 = 1; i4 <= nBlocks; i4++) {
          n = (i4 - 1) << 8;
          for (b = 0; b < 6; b++) {
            bLen = 1 << (b + 2);
            bLen2 = bLen << 1;
            nPairs = 256 >> (b + 3);
            for (k = 1; k <= nPairs; k++) {
              m = n + (k - 1) * bLen2;
              for (ib = 0; ib + 1 <= bLen2; ib++) {
                b_iwork[ib] = idx->data[m + ib];
                b_xwork[ib] = b_x->data[m + ib];
              }

              p = 0;
              wOffset = bLen;
              ib = m - 1;
              do {
                exitg1 = 0;
                ib++;
                if (b_xwork[p] >= b_xwork[wOffset]) {
                  idx->data[ib] = b_iwork[p];
                  b_x->data[ib] = b_xwork[p];
                  if (p + 1 < bLen) {
                    p++;
                  } else {
                    exitg1 = 1;
                  }
                } else {
                  idx->data[ib] = b_iwork[wOffset];
                  b_x->data[ib] = b_xwork[wOffset];
                  if (wOffset + 1 < bLen2) {
                    wOffset++;
                  } else {
                    ib = (ib - p) + 1;
                    while (p + 1 <= bLen) {
                      idx->data[ib + p] = b_iwork[p];
                      b_x->data[ib + p] = b_xwork[p];
                      p++;
                    }

                    exitg1 = 1;
                  }
                }
              } while (exitg1 == 0);
            }
          }
        }

        m = nBlocks << 8;
        ib = nNonNaN - m;
        if (ib > 0) {
          merge_block(idx, b_x, m, ib, 2, iwork, xwork);
        }

        m = 8;
      }
    }

    merge_block(idx, b_x, 0, nNonNaN, m, iwork, xwork);
  }

  if ((nNaNs > 0) && (nNonNaN > 0)) {
    for (k = 0; k + 1 <= nNaNs; k++) {
      xwork->data[k] = b_x->data[nNonNaN + k];
      iwork->data[k] = idx->data[nNonNaN + k];
    }

    for (k = nNonNaN - 1; k + 1 > 0; k--) {
      b_x->data[nNaNs + k] = b_x->data[k];
      idx->data[nNaNs + k] = idx->data[k];
    }

    for (k = 0; k + 1 <= nNaNs; k++) {
      b_x->data[k] = xwork->data[k];
      idx->data[k] = iwork->data[k];
    }
  }

  emxFree_real_T(&xwork);
  emxFree_int32_T(&iwork);
  ib = x->size[0];
  x->size[0] = b_x->size[0];
  emxEnsureCapacity((emxArray__common *)x, ib, (int)sizeof(double));
  m = b_x->size[0];
  for (ib = 0; ib < m; ib++) {
    x->data[ib] = b_x->data[ib];
  }

  emxFree_real_T(&b_x);
}

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
static double trapz(const emxArray_real_T *x)
{
  double z;
  int iy;
  double ylast;
  int k;
  if (x->size[0] == 0) {
    z = 0.0;
  } else {
    z = 0.0;
    iy = 0;
    ylast = x->data[0];
    for (k = 0; k <= x->size[0] - 2; k++) {
      iy++;
      z += (ylast + x->data[iy]) / 2.0;
      ylast = x->data[iy];
    }
  }

  return z;
}

//
// classifyEOG Classifier for EOG directions, EOG Blinks
//    INPUTS
//  ch1 (Raw data) - last 4 seconds of data (1000dp)
//  ch2 (Raw data) - last 4 seconds of data (1000dp)
//    OUTPUTS
//  Y - K-Nearest Neighbor Fit of EOG Classification.
// %% CLASSES:
//  0- Null class (no threshold exceeded)
//  Classifier 2: Blink/Single Blink
//  1- TODO - Single Blink
//  2- TODO - Double Blink
//  Classifier 1: Directions
// %%%%%%%%%%%%%%%
//      (3)Up    %
// (4) <-|-> (5) %
//      (6)Down  %
// %%%%%%%%%%%%%%%
//  Initialize Constants:
//  Thresholds:
// Arguments    : const double ch1[1000]
//                const double ch2[1000]
// Return Type  : double
//
double EOGClassifier2(const double ch1[1000], const double ch2[1000])
{
  double Y;
  double F[26];
  double mtmp;
  double d0;
  int ii;
  double y[1036];
  double dv0[7];
  double dv1[7];
  double a[6];
  static const double dv2[7] = { 0.00129953028712882, 0.0, -0.00389859086138647,
    0.0, 0.00389859086138647, 0.0, -0.001299530287129 };

  static const double dv3[7] = { 1.0, -5.52855503341017, 12.7497009825319,
    -15.7020189237831, 10.8934835499658, -4.03693878488703, 0.624328210166808 };

  double b_y[1036];
  static const double b_a[6] = { -0.0012998403321167991, -0.0012981262313379218,
    0.0025965116491606776, 0.0026013799814291558, -0.0013005883499336175,
    -0.0012993367172965836 };

  double c_y[1036];
  double d_y[1036];
  double e_y[1036];
  double f_y[1036];
  double dchf[1998];
  double maxval[2];
  emxArray_int32_T *b_ii;
  int ix;
  int idx;
  int b_ix;
  boolean_T exitg2;
  boolean_T exitg1;

  //  UTH2 = 2.75E-4;
  // TODO: Change to lower number
  //  Load Training Data:
  //  Initialize Variables:
  memset(&F[0], 0, 26U * sizeof(double));

  // %%OUTPUT Variable:
  Y = 0.0;

  // %%%TODO: REPLACE WITH COEFF FILT FOR MATLAB CODER:
  //  Filter Data:
  //  Butterworth Order 3 [0.15-9.5Hz] for EOG.
  mtmp = 2.0 * ch1[0];
  d0 = 2.0 * ch1[999];
  for (ii = 0; ii < 18; ii++) {
    y[ii] = mtmp - ch1[18 - ii];
  }

  memcpy(&y[18], &ch1[0], 1000U * sizeof(double));
  for (ii = 0; ii < 18; ii++) {
    y[ii + 1018] = d0 - ch1[998 - ii];
  }

  for (ii = 0; ii < 7; ii++) {
    dv0[ii] = dv2[ii];
    dv1[ii] = dv3[ii];
  }

  for (ii = 0; ii < 6; ii++) {
    a[ii] = b_a[ii] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1036U * sizeof(double));
  filter(dv0, dv1, b_y, a, y);
  flipud(y);
  for (ii = 0; ii < 7; ii++) {
    dv0[ii] = dv2[ii];
    dv1[ii] = dv3[ii];
  }

  for (ii = 0; ii < 6; ii++) {
    a[ii] = b_a[ii] * y[0];
  }

  memcpy(&c_y[0], &y[0], 1036U * sizeof(double));
  filter(dv0, dv1, c_y, a, y);
  flipud(y);

  //  Butterworth Order 3 [0.15-9.5Hz] for EOG.
  mtmp = 2.0 * ch2[0];
  d0 = 2.0 * ch2[999];
  for (ii = 0; ii < 18; ii++) {
    d_y[ii] = mtmp - ch2[18 - ii];
  }

  memcpy(&d_y[18], &ch2[0], 1000U * sizeof(double));
  for (ii = 0; ii < 18; ii++) {
    d_y[ii + 1018] = d0 - ch2[998 - ii];
  }

  for (ii = 0; ii < 7; ii++) {
    dv0[ii] = dv2[ii];
    dv1[ii] = dv3[ii];
  }

  for (ii = 0; ii < 6; ii++) {
    a[ii] = b_a[ii] * d_y[0];
  }

  memcpy(&e_y[0], &d_y[0], 1036U * sizeof(double));
  filter(dv0, dv1, e_y, a, d_y);
  flipud(d_y);
  for (ii = 0; ii < 7; ii++) {
    dv0[ii] = dv2[ii];
    dv1[ii] = dv3[ii];
  }

  for (ii = 0; ii < 6; ii++) {
    a[ii] = b_a[ii] * d_y[0];
  }

  memcpy(&f_y[0], &d_y[0], 1036U * sizeof(double));
  filter(dv0, dv1, f_y, a, d_y);
  flipud(d_y);
  memset(&dchf[0], 0, 1998U * sizeof(double));

  //  Take Differential:
  diff(*(double (*)[1000])&y[18], *(double (*)[999])&dchf[0]);
  diff(*(double (*)[1000])&d_y[18], *(double (*)[999])&dchf[999]);

  //  Plot ???
  //  Check If Threshold Exceeded:
  for (ii = 0; ii < 2; ii++) {
    ix = ii * 250 + 250;
    b_ix = ii * 250 + 1;
    mtmp = dchf[((ix - 250) % 250 + 999 * ((ix - 250) / 250)) + 749];
    if (rtIsNaN(dchf[((ix - 250) % 250 + 999 * ((ix - 250) / 250)) + 749])) {
      idx = b_ix;
      exitg2 = false;
      while ((!exitg2) && (idx + 1 <= ix)) {
        b_ix = idx + 1;
        if (!rtIsNaN(dchf[(idx % 250 + 999 * (idx / 250)) + 749])) {
          mtmp = dchf[(idx % 250 + 999 * (idx / 250)) + 749];
          exitg2 = true;
        } else {
          idx++;
        }
      }
    }

    if (b_ix < ix) {
      while (b_ix + 1 <= ix) {
        if (dchf[(b_ix % 250 + 999 * (b_ix / 250)) + 749] > mtmp) {
          mtmp = dchf[(b_ix % 250 + 999 * (b_ix / 250)) + 749];
        }

        b_ix++;
      }
    }

    maxval[ii] = mtmp;
  }

  emxInit_int32_T(&b_ii, 2);
  idx = 0;
  for (ii = 0; ii < 2; ii++) {
    ix = b_ii->size[0] * b_ii->size[1];
    b_ii->size[ii] = 1;
    emxEnsureCapacity((emxArray__common *)b_ii, ix, (int)sizeof(int));
  }

  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii < 3)) {
    if (maxval[ii - 1] > 4.0E-5) {
      idx = 1;
      b_ii->data[0] = ii;
      exitg1 = true;
    } else {
      ii++;
    }
  }

  if (idx == 0) {
    ii = b_ii->size[0] * b_ii->size[1];
    b_ii->size[0] = 1;
    b_ii->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)b_ii, ii, (int)sizeof(int));
  }

  // b_ii
  //  > 2.75 E-4 ? 3.25
  if (!(b_ii->size[1] == 0)) {
    // && isempty(thresholdCheck2)
    //  Extract Features from Last Second of Data:
    featureExtractionEOG3(*(double (*)[250])&dchf[749], -0.000275, 0.000325,
                          *(double (*)[13])&F[0]);
    featureExtractionEOG3(*(double (*)[250])&dchf[1748], -0.000275, 0.000325,
                          *(double (*)[13])&F[13]);

    //  Run Through Classification:
    Y = knn(F);
  }

  emxFree_int32_T(&b_ii);

  //  Blink/Double Blink Classifier
  // {
  // if ~isempty(thresholdCheck2) % thresholdCheck implied.
  //     F0(1, 1:13) = featureExtractionEOG2( dchf(end-249:end,1), LTH1, LTH2, UTH1, UTH2, true ); 
  //     F0(1,14:26) = featureExtractionEOG2( dchf(end-249:end,2), LTH1, LTH2, UTH1, UTH2, true ); 
  //     % Run Through KNN Classifier:
  //     Y = knn(F0,tX0,tY0,3);
  // end
  // }
  return Y;
}

//
// Arguments    : void
// Return Type  : void
//
void EOGClassifier2_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void EOGClassifier2_terminate()
{
}

//
// File trailer for EOGClassifier.cpp
//
// [EOF]
//
