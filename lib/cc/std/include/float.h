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
 * ISO C99 Standard: 5.2.4.2.2 Characteristics of floating types <float.h>
 */

#ifndef _CC_STD_FLOAT_H
#define _CC_STD_FLOAT_H

/* What the target's floating types are is only the compiler's to say, so each
   macro is the one it defines. The fallbacks are the values the standard gives
   as the least an implementation may define, IEC 60559 single and double where
   the standard leaves the value to the implementation. */

/* Rounding mode for floating-point addition. It is the mode the program runs
   under, so it is not a constant expression. */
#ifdef __FLT_ROUNDS__
#define FLT_ROUNDS __FLT_ROUNDS__
#else
#define FLT_ROUNDS (__builtin_flt_rounds())
#endif /* __FLT_ROUNDS__ */

/* Radix of the exponent representation. */
#ifdef __FLT_RADIX__
#define FLT_RADIX __FLT_RADIX__
#else
#define FLT_RADIX 2
#endif /* __FLT_RADIX__ */

/* How the types the operands are written with are evaluated. */
#ifdef __FLT_EVAL_METHOD__
#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#else
#define FLT_EVAL_METHOD 0
#endif /* __FLT_EVAL_METHOD__ */

/* Number of decimal digits that a floating type is read back with, whatever
   the type it was written from. Since C99. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 199901L
#ifdef __DECIMAL_DIG__
#define DECIMAL_DIG __DECIMAL_DIG__
#else
#define DECIMAL_DIG 10
#endif /* __DECIMAL_DIG__ */
#endif /* !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 199901L */

/* Number of digits in the significand, in FLT_RADIX. */
#define FLT_MANT_DIG __FLT_MANT_DIG__
#define DBL_MANT_DIG __DBL_MANT_DIG__
#define LDBL_MANT_DIG __LDBL_MANT_DIG__

/* Number of decimal digits a value of the type is read back with. */
#define FLT_DIG __FLT_DIG__
#define DBL_DIG __DBL_DIG__
#define LDBL_DIG __LDBL_DIG__

/* Least and greatest exponents, in FLT_RADIX and in decimal. */
#define FLT_MIN_EXP __FLT_MIN_EXP__
#define DBL_MIN_EXP __DBL_MIN_EXP__
#define LDBL_MIN_EXP __LDBL_MIN_EXP__

#define FLT_MIN_10_EXP __FLT_MIN_10_EXP__
#define DBL_MIN_10_EXP __DBL_MIN_10_EXP__
#define LDBL_MIN_10_EXP __LDBL_MIN_10_EXP__

#define FLT_MAX_EXP __FLT_MAX_EXP__
#define DBL_MAX_EXP __DBL_MAX_EXP__
#define LDBL_MAX_EXP __LDBL_MAX_EXP__

#define FLT_MAX_10_EXP __FLT_MAX_10_EXP__
#define DBL_MAX_10_EXP __DBL_MAX_10_EXP__
#define LDBL_MAX_10_EXP __LDBL_MAX_10_EXP__

/* Greatest finite value. */
#define FLT_MAX __FLT_MAX__
#define DBL_MAX __DBL_MAX__
#define LDBL_MAX __LDBL_MAX__

/* Difference between 1 and the least value greater than 1. */
#define FLT_EPSILON __FLT_EPSILON__
#define DBL_EPSILON __DBL_EPSILON__
#define LDBL_EPSILON __LDBL_EPSILON__

/* Least normalized positive value. */
#define FLT_MIN __FLT_MIN__
#define DBL_MIN __DBL_MIN__
#define LDBL_MIN __LDBL_MIN__

/* Whether the type holds subnormal numbers, the least positive value it holds
   where it does, and the digits it is written back with. Since C11. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 201112L
#define FLT_HAS_SUBNORM __FLT_HAS_DENORM__
#define DBL_HAS_SUBNORM __DBL_HAS_DENORM__
#define LDBL_HAS_SUBNORM __LDBL_HAS_DENORM__

#define FLT_TRUE_MIN __FLT_DENORM_MIN__
#define DBL_TRUE_MIN __DBL_DENORM_MIN__
#define LDBL_TRUE_MIN __LDBL_DENORM_MIN__

#define FLT_DECIMAL_DIG __FLT_DECIMAL_DIG__
#define DBL_DECIMAL_DIG __DBL_DECIMAL_DIG__
#define LDBL_DECIMAL_DIG __LDBL_DECIMAL_DIG__
#endif /* !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 201112L */

#endif /* _CC_STD_FLOAT_H */
