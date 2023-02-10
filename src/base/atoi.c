/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define __atoi__(type, s)              \
    type res = 0;                      \
    int i = 0;                         \
    bool is_neg = false;               \
    if (s[i] == '-') {                 \
        i++;                           \
        is_neg = true;                 \
    }                                  \
    while (s[i]) {                     \
        res = res * 10 + (s[i] - '0'); \
        i++;                           \
    }                                  \
    return is_neg ? -res : res

Int8
atoi__Int8(const char *s)
{
    __atoi__(Int8, s);
}

Int16
atoi__Int16(const char *s)
{
    __atoi__(Int16, s);
}

Int32
atoi__Int32(const char *s)
{
    __atoi__(Int32, s);
}

Int64
atoi__Int64(const char *s)
{
    __atoi__(Int64, s);
}

Uint8
atoi__Uint8(const char *s)
{
    __atoi__(Uint8, s);
}

Uint16
atoi__Uint16(const char *s)
{
    __atoi__(Uint16, s);
}

Uint32
atoi__Uint32(const char *s)
{
    __atoi__(Uint32, s);
}

Uint64
atoi__Uint64(const char *s)
{
    __atoi__(Uint64, s);
}

Isize
atoi__Isize(const char *s)
{
    __atoi__(Isize, s);
}

Usize
atoi__Usize(const char *s)
{
    __atoi__(Usize, s);
}
