#ifndef UTIL_H
#define UTIL_H

#include <math.h>

double drand();
double rnd(double n);
double sgn(double x);

#ifdef UTIL_IMPLEMENTATION_H

double drand()
{
    return rand() / (RAND_MAX + 1.0);
}

double rnd(double n)
{
    return floor(drand() * n);
}

double sgn(double n)
{
    if (n < 0.0)
        return -1.0; if (n > 0.0) return 1.0; return n;
}

#endif /* UTIL_IMPLEMENTATION_H */

#endif /* UTIL_H */