#include "gsl_math.h"
#include "gsl_cblas.h"
#include "gsl_cblas__cblas.h"

#include "gsl_cblas__hypot.c"

void
cblas_ztpsv (const enum CBLAS_ORDER order, const enum CBLAS_UPLO Uplo,
             const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_DIAG Diag,
             const int N, const void *Ap, void *X, const int incX)
{
#define BASE double
#include "gsl_cblas__source_tpsv_c.h"
#undef BASE
}
