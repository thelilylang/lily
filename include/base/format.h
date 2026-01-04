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

#ifndef LILY_BASE_FORMAT_H
#define LILY_BASE_FORMAT_H

#include <stdarg.h>

/**
 *
 * @brief Format string.
 *
 * List of format specifiers:
 * - {s}: String (char*)
 * - {sa}: String allocated (free after use)
 * - {d:b}: Integer, Int32 (binary)
 * - {d:o}: Integer, Int32 (octal)
 * - {d:x}: Integer, Int32 (hex)
 * - {d}: Integer, Int32
 * - {f}: Double, Float64
 * - {f.n}: Double, Float64
 * - {c}: Char
 * - {b}: Boolean
 * - {p}: Pointer
 * - {u:b}: Unsigned, Uint32 (binary)
 * - {u:o}: Unsigned, Uint32 (octal)
 * - {u:x}: Unsigned, Uint32 (hex)
 * - {u}: Unsigned, Uint32
 * - {S}: Dynamic String (base/string.h)
 * - {Sr}: Raw dynamic string (free after use) (base/string.h)
 * - {zi:b} ssize_t, Isize (binary)
 * - {zi:o} ssize_t, Isize (octal)
 * - {zi:x} ssize_t, Isize (hex)
 * - {zi} ssize_t, Isize
 * - {zu:b} size_t, Usize (binary)
 * - {zu:o} size_t, Usize (octal)
 * - {zu:x} size_t, Usize (hex)
 * - {zu} size_t, Usize
 */
char *
format(const char *fmt, ...);

/**
 *
 * @brief Alternative version of the format function, to pass the argument
 * directly.
 */
char *
vformat(const char *fmt, va_list arg);

#endif // LILY_BASE_FORMAT_H
