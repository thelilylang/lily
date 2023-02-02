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

#include <base/itoa.h>

#include <stdlib.h>
#include <string.h>

#define __itoa__(v, base)                              \
    char *buffer = malloc(1);                          \
    int buffer_size = 0;                               \
    if (v == 0) {                                      \
        buffer = realloc(buffer, buffer_size + 2);     \
        buffer[buffer_size] = '0';                     \
        buffer[++buffer_size] = '\0';                  \
        return buffer;                                 \
    }                                                  \
    buffer[0] = '\0';                                  \
    if (v < 0) {                                       \
        buffer = realloc(buffer, 2);                   \
        buffer[0] = '-';                               \
        buffer[++buffer_size] = '\0';                  \
        v = -v;                                        \
    }                                                  \
    if (base == 10) {                                  \
        while (v > 0) {                                \
            buffer = realloc(buffer, buffer_size + 2); \
            buffer[buffer_size] = (v % 10) + '0';      \
            buffer[++buffer_size] = '\0';              \
            v /= 10;                                   \
        }                                              \
    } else if (base == 2) {                            \
        while (v > 0) {                                \
            buffer = realloc(buffer, buffer_size + 2); \
            buffer[buffer_size] = (v % 2) + '0';       \
            buffer[++buffer_size] = '\0';              \
            v /= 2;                                    \
        }                                              \
    } else if (base == 8) {                            \
        while (v > 0) {                                \
            buffer = realloc(buffer, buffer_size + 2); \
            buffer[buffer_size] = (v % 8) + '0';       \
            buffer[++buffer_size] = '\0';              \
            v /= 8;                                    \
        }                                              \
    } else if (base == 16) {                           \
        while (v > 0) {                                \
            buffer = realloc(buffer, buffer_size + 2); \
            int n = (v % 16) + '0';                    \
            if (n > '9')                               \
                buffer[buffer_size] = n + 7;           \
            else                                       \
                buffer[buffer_size] = n;               \
            buffer[++buffer_size] = '\0';              \
            v /= 16;                                   \
        }                                              \
    }                                                  \
    for (int i = 0; i < buffer_size / 2; i++) {        \
        char tmp = buffer[i];                          \
        buffer[i] = buffer[buffer_size - i - 1];       \
        buffer[buffer_size - i - 1] = tmp;             \
    }                                                  \
    return buffer

char *
itoa__Int8(Int8 v, int base)
{
    __itoa__(v, base);
}

char *
itoa__Uint8(Uint8 v, int base)
{
    __itoa__(v, base);
}

char *
itoa__Int16(Int16 v, int base)
{
    __itoa__(v, base);
}

char *
itoa__Uint16(Uint16 v, int base)
{
    __itoa__(v, base);
}

char *
itoa__Int32(Int32 v, int base)
{
    __itoa__(v, base);
}

char *
itoa__Uint32(Uint32 v, int base)
{
    __itoa__(v, base);
}

char *
itoa__Int64(Int64 v, int base)
{
    __itoa__(v, base);
}

char *
itoa__Uint64(Uint64 v, int base)
{
    __itoa__(v, base);
}
