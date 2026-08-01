/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * ISO C99 Standard: 7.12 Mathematics <math.h>
 */

#ifndef _CC_STD_MATH_H
#define _CC_STD_MATH_H

#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.12: float_t and double_t are the types floating expressions are evaluated
   with, which is what FLT_EVAL_METHOD says (5.2.4.2.2). */
#if !defined(__FLT_EVAL_METHOD__) || __FLT_EVAL_METHOD__ == 0
typedef float float_t;
typedef double double_t;
#elif __FLT_EVAL_METHOD__ == 1
typedef double float_t;
typedef double double_t;
#elif __FLT_EVAL_METHOD__ == 2
typedef long double float_t;
typedef long double double_t;
#else
typedef float float_t;
typedef double double_t;
#endif /* !defined(__FLT_EVAL_METHOD__) || __FLT_EVAL_METHOD__ == 0 */

/* A value too great to be held by the type, which is a positive infinity where
   the type holds one. */
#define HUGE_VAL (__builtin_huge_val())
#define HUGE_VALF (__builtin_huge_valf())
#define HUGE_VALL (__builtin_huge_vall())

#define INFINITY (__builtin_inff())
#define NAN (__builtin_nanf(""))

/* 7.12.3: what a value is classified as. The numbers are only ever compared
   against each other, so what matters is that they are distinct. */
#define FP_NAN 0
#define FP_INFINITE 1
#define FP_ZERO 2
#define FP_SUBNORMAL 3
#define FP_NORMAL 4

#define FP_ILOGB0 (-__INT_MAX__ - 1)
#define FP_ILOGBNAN __INT_MAX__

/* 7.12.1: how a function reports the error it was given. */
#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)

#define fpclassify(x) \
    __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x) __builtin_isfinite(x)
#define isinf(x) __builtin_isinf(x)
#define isnan(x) __builtin_isnan(x)
#define isnormal(x) __builtin_isnormal(x)
#define signbit(x) __builtin_signbit(x)

/* 7.12.14: a comparison that holds no exception where an operand is a NaN. */
#define isgreater(x, y) __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isless(x, y) __builtin_isless(x, y)
#define islessequal(x, y) __builtin_islessequal(x, y)
#define islessgreater(x, y) __builtin_islessgreater(x, y)
#define isunordered(x, y) __builtin_isunordered(x, y)

/* 7.12.4 Trigonometric functions */
extern double
acos(double x);
extern float
acosf(float x);
extern long double
acosl(long double x);
extern double
asin(double x);
extern float
asinf(float x);
extern long double
asinl(long double x);
extern double
atan(double x);
extern float
atanf(float x);
extern long double
atanl(long double x);
extern double
atan2(double y, double x);
extern float
atan2f(float y, float x);
extern long double
atan2l(long double y, long double x);
extern double
cos(double x);
extern float
cosf(float x);
extern long double
cosl(long double x);
extern double
sin(double x);
extern float
sinf(float x);
extern long double
sinl(long double x);
extern double
tan(double x);
extern float
tanf(float x);
extern long double
tanl(long double x);

/* 7.12.5 Hyperbolic functions */
extern double
acosh(double x);
extern float
acoshf(float x);
extern long double
acoshl(long double x);
extern double
asinh(double x);
extern float
asinhf(float x);
extern long double
asinhl(long double x);
extern double
atanh(double x);
extern float
atanhf(float x);
extern long double
atanhl(long double x);
extern double
cosh(double x);
extern float
coshf(float x);
extern long double
coshl(long double x);
extern double
sinh(double x);
extern float
sinhf(float x);
extern long double
sinhl(long double x);
extern double
tanh(double x);
extern float
tanhf(float x);
extern long double
tanhl(long double x);

/* 7.12.6 Exponential and logarithmic functions */
extern double
exp(double x);
extern float
expf(float x);
extern long double
expl(long double x);
extern double
exp2(double x);
extern float
exp2f(float x);
extern long double
exp2l(long double x);
extern double
expm1(double x);
extern float
expm1f(float x);
extern long double
expm1l(long double x);
extern double
frexp(double value, int *exp);
extern float
frexpf(float value, int *exp);
extern long double
frexpl(long double value, int *exp);
extern int
ilogb(double x);
extern int
ilogbf(float x);
extern int
ilogbl(long double x);
extern double
ldexp(double x, int exp);
extern float
ldexpf(float x, int exp);
extern long double
ldexpl(long double x, int exp);
extern double
log(double x);
extern float
logf(float x);
extern long double
logl(long double x);
extern double
log10(double x);
extern float
log10f(float x);
extern long double
log10l(long double x);
extern double
log1p(double x);
extern float
log1pf(float x);
extern long double
log1pl(long double x);
extern double
log2(double x);
extern float
log2f(float x);
extern long double
log2l(long double x);
extern double
logb(double x);
extern float
logbf(float x);
extern long double
logbl(long double x);
extern double
modf(double value, double *iptr);
extern float
modff(float value, float *iptr);
extern long double
modfl(long double value, long double *iptr);
extern double
scalbn(double x, int n);
extern float
scalbnf(float x, int n);
extern long double
scalbnl(long double x, int n);
extern double
scalbln(double x, long int n);
extern float
scalblnf(float x, long int n);
extern long double
scalblnl(long double x, long int n);

/* 7.12.7 Power and absolute-value functions */
extern double
cbrt(double x);
extern float
cbrtf(float x);
extern long double
cbrtl(long double x);
extern double
fabs(double x);
extern float
fabsf(float x);
extern long double
fabsl(long double x);
extern double
hypot(double x, double y);
extern float
hypotf(float x, float y);
extern long double
hypotl(long double x, long double y);
extern double
pow(double x, double y);
extern float
powf(float x, float y);
extern long double
powl(long double x, long double y);
extern double
sqrt(double x);
extern float
sqrtf(float x);
extern long double
sqrtl(long double x);

/* 7.12.8 Error and gamma functions */
extern double
erf(double x);
extern float
erff(float x);
extern long double
erfl(long double x);
extern double
erfc(double x);
extern float
erfcf(float x);
extern long double
erfcl(long double x);
extern double
lgamma(double x);
extern float
lgammaf(float x);
extern long double
lgammal(long double x);
extern double
tgamma(double x);
extern float
tgammaf(float x);
extern long double
tgammal(long double x);

/* 7.12.9 Nearest integer functions */
extern double
ceil(double x);
extern float
ceilf(float x);
extern long double
ceill(long double x);
extern double
floor(double x);
extern float
floorf(float x);
extern long double
floorl(long double x);
extern double
nearbyint(double x);
extern float
nearbyintf(float x);
extern long double
nearbyintl(long double x);
extern double
rint(double x);
extern float
rintf(float x);
extern long double
rintl(long double x);
extern long int
lrint(double x);
extern long int
lrintf(float x);
extern long int
lrintl(long double x);
extern long long int
llrint(double x);
extern long long int
llrintf(float x);
extern long long int
llrintl(long double x);
extern double
round(double x);
extern float
roundf(float x);
extern long double
roundl(long double x);
extern long int
lround(double x);
extern long int
lroundf(float x);
extern long int
lroundl(long double x);
extern long long int
llround(double x);
extern long long int
llroundf(float x);
extern long long int
llroundl(long double x);
extern double
trunc(double x);
extern float
truncf(float x);
extern long double
truncl(long double x);

/* 7.12.10 Remainder functions */
extern double
fmod(double x, double y);
extern float
fmodf(float x, float y);
extern long double
fmodl(long double x, long double y);
extern double
remainder(double x, double y);
extern float
remainderf(float x, float y);
extern long double
remainderl(long double x, long double y);
extern double
remquo(double x, double y, int *quo);
extern float
remquof(float x, float y, int *quo);
extern long double
remquol(long double x, long double y, int *quo);

/* 7.12.11 Manipulation functions */
extern double
copysign(double x, double y);
extern float
copysignf(float x, float y);
extern long double
copysignl(long double x, long double y);
extern double
nan(const char *tagp);
extern float
nanf(const char *tagp);
extern long double
nanl(const char *tagp);
extern double
nextafter(double x, double y);
extern float
nextafterf(float x, float y);
extern long double
nextafterl(long double x, long double y);
extern double
nexttoward(double x, long double y);
extern float
nexttowardf(float x, long double y);
extern long double
nexttowardl(long double x, long double y);

/* 7.12.12 Maximum, minimum, and positive difference functions */
extern double
fdim(double x, double y);
extern float
fdimf(float x, float y);
extern long double
fdiml(long double x, long double y);
extern double
fmax(double x, double y);
extern float
fmaxf(float x, float y);
extern long double
fmaxl(long double x, long double y);
extern double
fmin(double x, double y);
extern float
fminf(float x, float y);
extern long double
fminl(long double x, long double y);

/* 7.12.13 Floating multiply-add */
extern double
fma(double x, double y, double z);
extern float
fmaf(float x, float y, float z);
extern long double
fmal(long double x, long double y, long double z);

__END_DECLS

#endif /* _CC_STD_MATH_H */
