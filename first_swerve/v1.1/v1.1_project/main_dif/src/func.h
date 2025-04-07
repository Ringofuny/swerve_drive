#ifndef FUNC_H
#define FUNC_H
#include "math.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int digitalWrite_f(double pin, double value);

namespace MAXIN {
    double fmin(double a, double b);
    double fmax(double a, double b);
}

#endif  // FUNC_H