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

#include <base/format.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <base/types.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUSH_STR(s)                                  \
    res = realloc(res, buffer_size + strlen(s) + 1); \
    for (Usize j = 0; s[j++];) {                     \
        res[buffer_size] = s[j - 1];                 \
        res[++buffer_size] = '\0';                   \
    }

#define REV_STR(s, size)                 \
    for (int j = 0; j < size / 2; j++) { \
        char tmp = s[j];                 \
        s[j] = s[size - j - 1];          \
        s[size - j - 1] = tmp;           \
    }

#define PUSH_INT(d, base)                    \
    if (d < 0) {                             \
        res = realloc(res, buffer_size + 2); \
        res[buffer_size] = '-';              \
        res[++buffer_size] = '\0';           \
        d = -d;                              \
    }                                        \
    while (d > 0) {                          \
        res = realloc(res, buffer_size + 2); \
        res[buffer_size] = (d % base) + '0'; \
        res[++buffer_size] = '\0';           \
        d /= base;                           \
    }                                        \
    REV_STR(res, buffer_size);

#define PUSH_INT_BASE_16(d)                     \
    if (d < 0) {                                \
        res = realloc(res, buffer_size + 2);    \
        res[buffer_size] = '-';                 \
        res[++buffer_size] = '\0';              \
        d = -d;                                 \
    }                                           \
    while (d > 0) {                             \
        res = realloc(res, buffer_size + 2);    \
        int n = (d % 16) + '0';                 \
        res[buffer_size] = n > '9' ? n + 7 : n; \
        res[++buffer_size] = '\0';              \
        d /= 16;                                \
    }                                           \
    REV_STR(res, buffer_size);

char *
format(const char *fmt, ...)
{
    va_list vl;
    char *res = malloc(1);
    Usize buffer_size = 0;
    Usize len = strlen(fmt);

    res[0] = '\0';
    va_start(vl, fmt);

    for (Usize i = 0; i < len;) {
        switch (fmt[i]) {
            case '{':
                switch (fmt[i + 1]) {
                    case 's': {
                        char *s = va_arg(vl, char *);

                        PUSH_STR(s);

                        if (fmt[i + 2] == 'a') {
                            free(s);
                            i += 3;
                        } else
                            i += 2;

                        break;
                    }
                    case 'd': {
                        int d = va_arg(vl, int);

                        switch (fmt[i + 2]) {
                            case ':':
                                switch (fmt[i + 3]) {
                                    case 'b': {
                                        PUSH_INT(d, 2);

                                        break;
                                    }
                                    case 'o': {
                                        PUSH_INT(d, 8);

                                        break;
                                    }
                                    case 'x': {
                                        PUSH_INT_BASE_16(d);

                                        break;
                                    }
                                    default:
                                        FAILED("unknown specifier");
                                }

                                i += 4;

                                break;
                            default:
                                PUSH_INT(d, 10);

                                i += 2;
                        }

                        break;
                    }
                    case 'f':
                        break;
                    case 'c': {
                        char c = va_arg(vl, int);
                        res = realloc(res, buffer_size + 2);

                        res[buffer_size] = c;
                        res[++buffer_size] = '\0';

                        i += 2;

                        break;
                    }
                    case 'b': {
                        bool b = va_arg(vl, int);

                        if (b) {
                            res = realloc(res, buffer_size + 5);

                            res[buffer_size++] = 't';
                            res[buffer_size++] = 'r';
                            res[buffer_size++] = 'u';
                            res[buffer_size++] = 'e';
                            res[buffer_size] = '\0';
                        } else {
                            res = realloc(res, buffer_size + 6);
                            ;

                            res[buffer_size++] = 'f';
                            res[buffer_size++] = 'a';
                            res[buffer_size++] = 'l';
                            res[buffer_size++] = 's';
                            res[buffer_size++] = 'e';
                            res[buffer_size] = '\0';
                        }

                        i += 2;

                        break;
                    }
                    case 'p':
                        break;
                    case 'u': {
                        int d = va_arg(vl, unsigned int);

                        switch (fmt[i + 2]) {
                            case ':':
                                switch (fmt[i + 3]) {
                                    case 'b': {
                                        PUSH_INT(d, 2);

                                        break;
                                    }
                                    case 'o': {
                                        PUSH_INT(d, 8);

                                        break;
                                    }
                                    case 'x': {
                                        PUSH_INT_BASE_16(d);

                                        break;
                                    }
                                    default:
                                        FAILED("unknown specifier");
                                }

                                i += 4;

                                break;
                            default:
                                PUSH_INT(d, 10);

                                i += 2;
                        }

                        break;
                    }
                    case 'S': {
                        String *s = va_arg(vl, String *);

                        res = realloc(res, buffer_size + s->len + 1);
                        buffer_size += s->len;

                        strcat(res, s->buffer);

                        if (fmt[i + 2] == 'r') {
                            FREE(String, s);
                            i += 3;
                        } else {
                            i += 2;
                        }

                        break;
                    }
                    case '{':
                        res = realloc(res, buffer_size + 2);
                        res[buffer_size] = '{';
                        res[++buffer_size] = '\0';

                        i += 2;

                        break;
                    default:
                        FAILED("unknown specifier");
                }

                if ((fmt[i - 2] != '{' && fmt[i - 1] != '{') && fmt[i] != '}') {
                    FAILED("expected `}`");
                } else
                    i++;

                break;
            default:
                res = realloc(res, buffer_size + 2);
                res[buffer_size] = fmt[i];
                res[++buffer_size] = '\0';
                i++;
        }
    }

    va_end(vl);

    return res;
}
