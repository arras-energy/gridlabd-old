/* File: interpolate.h
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef GLD_INTERPOLATE
#define GLD_INTERPOLATE

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

double interpolate_linear(double t, double x0, double y0, double x1, double y1);
double interpolate_quadratic(double t, double x0, double y0, double x1, double y1, double x2, double y2);

#ifdef __cplusplus
}
#endif

#endif

// EOF
