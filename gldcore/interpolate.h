
#ifndef GLD_INTERPOLATE
#define GLD_INTERPOLATE

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
