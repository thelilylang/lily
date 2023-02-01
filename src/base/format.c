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
#include <base/types.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *format(const char *fmt, ...) {
  va_list vl;
  char *res = malloc(1);
  Usize buffer_size = 0;
  Usize len = strlen(fmt);

  res[0] = '\0';
  va_start(vl, fmt);

  for (Usize i = 0; i < len; i++) {
    switch (fmt[i]) {
    case '{':
      switch (fmt[i + 1]) {
      case 's': {
        char *s = va_arg(vl, char *);
        res = realloc(res, buffer_size + strlen(s) + 1);

        for (Usize i = 0; i < strlen(s); i++) {
          res[buffer_size] = s[i];
          res[++buffer_size] = '\0';
        }

        if (fmt[i + 2] == 'a') {
          free(s);
          i += 3;
        } else
          i += 2;

        break;
      }
      case 'd':
        break;
      case 'f':
        break;
      case 'c':
        break;
      case 'b':
        break;
      case 'p':
        break;
      case 'x':
        break;
      case 'o':
        break;
      case 'u':
        break;
      case 'S':
        break;
      case '{':
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
    }
  }

  va_end(vl);

  return res;
}
