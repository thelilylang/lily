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

#include <base/atof.h>

#define __atof__(type, s)                                                      \
    type res = 0;                                                              \
    type power = 1.0;                                                          \
    int i = 0;                                                                 \
    bool is_neg = false;                                                       \
    if (s[i] == '-') {                                                         \
        ++i;                                                                   \
        is_neg = true;                                                         \
    }                                                                          \
    while (s[i] >= '0' && s[i] <= '9') {                                       \
        res = 10.0 * res + (s[i] - '0');                                       \
        ++i;                                                                   \
    }                                                                          \
    if (s[i] == '.') {                                                         \
        ++i;                                                                   \
        for (; s[i] >= '0' && s[i] <= '9'; i++) {                              \
            res = 10.0 * res + (s[i] - '0');                                   \
            power *= 10.0;                                                     \
        }                                                                      \
        return is_neg ? -res / power : res / power;                            \
    } else if (s[i] == 'e' || s[i] == 'E') {                                   \
        Uint64 exp = 0;                                                        \
        type exp_coeff = 1.0;                                                  \
        bool exp_is_neg = false;                                               \
        ++i;                                                                   \
                                                                               \
        if (s[i] == '-') {                                                     \
            ++i;                                                               \
            exp_is_neg = true;                                                 \
        } else if (s[i] == '+') {                                              \
            ++i;                                                               \
        }                                                                      \
        while (s[i] >= '0' && s[i] <= '9') {                                   \
            exp = 10 * exp + (s[i] - '0');                                     \
            ++i;                                                               \
        }                                                                      \
        while (exp > 0) {                                                      \
            exp_coeff *= 10.0;                                                 \
            --exp;                                                             \
        }                                                                      \
        return is_neg       ? exp_is_neg ? -res / exp_coeff : -res * exp_coeff \
               : exp_is_neg ? res / exp_coeff                                  \
                            : res * exp_coeff;                                 \
    }                                                                          \
    return is_neg ? -res : res;

Float32
atof__Float32(const char *s)
{
    __atof__(Float32, s);
}

Float64
atof__Float64(const char *s)
{
    __atof__(Float64, s);
}
