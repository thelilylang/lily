/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_BASE_ATOF_H
#define LILY_BASE_ATOF_H

#include <base/types.h>

#define FLOAT_SCIENTIFIC 0
#define FLOAT_NORMAL 1

// MAX: 340282346999999984391321947108527833088, 3.40282347E+38
// MIN: -340282346999999984391321947108527833088, -3.40282347E+38
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: this check not support lead by 0 will do nothing (e.g. 000000033)
// NOTE: not currently support 3e3 syntax (only syntax with sign)
#define CHECK_FLOAT32_SCIENTIFIC_OVERFLOW_FROM_STRING(x, len) \
    ((x[0] != '-' && (x[0] < '3' ? 1 : x[0] > '3' ? (x[len - 1] > '8' && (x[len - 2] == '-' || x[len - 2] == '+') ? 1 : x[len - 1] > '8' ? 0 : x[len - 1] < '8' && (x[len - 2] == '-' || x[len - 2] == '+') ? 1 : x[len - 2] > '3' ? 0 : x[len - 2] <= '3' && (x[len - 3] == '-' || x[len - 3] == '+') ? 1 : 0) : x[0] == '3' ? x[len - 1] > '8' && (x[len - 2] == '-' || x[len - 2] == '+') ? 1 : x[len - 1] > '8' ? 0 : x[len - 1] < '8' && (x[len - 2] == '-' || x[len - 2] == '+') ? 1 : x[len - 2] > '3' ? 0 : x[len - 2] <= '3' && (x[len - 3] == '-' || x[len - 3] == '+') ? 1 : 0)))

#define CHECK_FLOAT32_NORMAL_OVERFLOW_FROM_STRING(x, len) 0
#define CHECK_FLOAT32_OVERFLOW_FROM_STRING(x, len, float_kind) 0

// MAX:
// 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368,
// 1.7976931348623157E+308
// MIN:
// -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368,
// -1.7976931348623157E+308
// NOTE: return 0 if is overflow/underflow otherwise
// return 1.
// NOTE: this check not support lead by 0 will do nothing (e.g.
// 000000033)
// NOTE: not currently support 3e3 syntax (only syntax with sign)
#define CHECK_FLOAT64_SCIENTIFIC_OVERFLOW_FROM_STRING(x) 0
#define CHECK_FLOAT64_NORMAL_OVERFLOW_FROM_STRING(x) 0
#define CHECK_FLOAT64_OVERFLOW_FROM_STRING(x, float_kind) 0

/**
 *
 * @brief Convert a char* to Float32.
 */
Float32
atof__Float32(const char *s);

/**
 *
 * @brief Convert a char* to Float64.
 */
Float64
atof__Float64(const char *s);

#endif // LILY_BASE_ATOF_H