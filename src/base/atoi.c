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

#include <base/atoi.h>
#include <base/macros.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __atoi__(type, s, base)                      \
    type res = 0;                                    \
    int i = 0;                                       \
    bool is_neg = false;                             \
    if (s[i] == '-') {                               \
        i++;                                         \
        is_neg = true;                               \
    }                                                \
    if (base == 2) {                                 \
        while (s[i]) {                               \
            res = res * 2 + (s[i] - '0');            \
            i++;                                     \
        }                                            \
    } else if (base == 8) {                          \
        while (s[i]) {                               \
            res = res * 8 + (s[i] - '0');            \
            i++;                                     \
        }                                            \
    } else if (base == 10) {                         \
        while (s[i]) {                               \
            res = res * 10 + (s[i] - '0');           \
            i++;                                     \
        }                                            \
    } else if (base == 16) {                         \
        while (s[i]) {                               \
            if (s[i] >= 'a' && s[i] <= 'f') {        \
                res = res * 16 + (s[i] - 87);        \
            } else if (s[i] >= 'A' && s[i] <= 'F') { \
                res = res * 16 + (s[i] - 55);        \
            } else {                                 \
                res = res * 16 + (s[i] - '0');       \
            }                                        \
            i++;                                     \
        }                                            \
    } else {                                         \
        UNREACHABLE("unknown base");                 \
    }                                                \
    res = is_neg ? -res : res;

Int8
atoi__Int8(const char *s, int base)
{
    __atoi__(Int8, s, base);
    return res;
}

Int16
atoi__Int16(const char *s, int base)
{
    __atoi__(Int16, s, base);
    return res;
}

Int32
atoi__Int32(const char *s, int base)
{
    __atoi__(Int32, s, base);
    return res;
}

Int64
atoi__Int64(const char *s, int base)
{
    __atoi__(Int64, s, base);
    return res;
}

Uint8
atoi__Uint8(const char *s, int base)
{
    __atoi__(Uint8, s, base);
    return res;
}

Uint16
atoi__Uint16(const char *s, int base)
{
    __atoi__(Uint16, s, base);
    return res;
}

Uint32
atoi__Uint32(const char *s, int base)
{
    __atoi__(Uint32, s, base);
    return res;
}

Uint64
atoi__Uint64(const char *s, int base)
{
    __atoi__(Uint64, s, base);
    return res;
}

Isize
atoi__Isize(const char *s, int base)
{
    __atoi__(Isize, s, base);
    return res;
}

Usize
atoi__Usize(const char *s, int base)
{
    __atoi__(Usize, s, base);
    return res;
}

Optional *
atoi_safe__Int8(const char *s, int base)
{
    if (!CHECK_INT8_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Int8, s, base);

    return SOME((Int8 *)(Uptr)res);
}

Optional *
atoi_safe__Int16(const char *s, int base)
{
    if (!CHECK_INT16_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Int16, s, base);

    return SOME((Int8 *)(Uptr)res);
}

Optional *
atoi_safe__Int32(const char *s, int base)
{
    if (!CHECK_INT32_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Int32, s, base);

    return SOME((Int32 *)(Uptr)res);
}

Optional *
atoi_safe__Int64(const char *s, int base)
{
    if (!CHECK_INT64_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Int64, s, base);

    return SOME((Int64 *)(Uptr)res);
}

Optional *
atoi_safe__Uint8(const char *s, int base)
{
    if (!CHECK_UINT8_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Uint8, s, base);

    return SOME((Uint8 *)(Uptr)res);
}

Optional *
atoi_safe__Uint16(const char *s, int base)
{
    if (!CHECK_UINT16_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Uint16, s, base);

    return SOME((Uint16 *)(Uptr)res);
}

Optional *
atoi_safe__Uint32(const char *s, int base)
{
    if (!CHECK_UINT32_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Uint32, s, base);

    return SOME((Uint32 *)(Uptr)res);
}

Optional *
atoi_safe__Uint64(const char *s, int base)
{
    if (!CHECK_UINT64_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Uint64, s, base);

    return SOME((Uint64 *)(Uptr)res);
}

Optional *
atoi_safe__Isize(const char *s, int base)
{
    if (!CHECK_ISIZE_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Isize, s, base);

    return SOME((Isize *)(Uptr)res);
}

Optional *
atoi_safe__Usize(const char *s, int base)
{
    if (!CHECK_USIZE_OVERFLOW_FROM_STRING(s, base)) {
        return NONE;
    }

    __atoi__(Usize, s, base);

    return SOME((Usize *)(Uptr)res);
}