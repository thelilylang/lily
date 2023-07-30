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

#ifndef LILY_BASE_FORMAT_H
#define LILY_BASE_FORMAT_H

/**
 *
 * @brief Format string.
 *
 * List of format specifiers:
 * - {s}: String
 * - {sa}: String allocated (free after use)
 * - {d:b}: Integer (binary)
 * - {d:o}: Integer (octal)
 * - {d:x}: Integer (hex)
 * - {d}: Integer
 * - {f}: Float
 * - {c}: Char
 * - {b}: Boolean
 * - {p}: Pointer
 * - {u}: Unsigned
 * - {u:b}: Unsigned (binary)
 * - {u:o}: Unsigned (octal)
 * - {u:x}: Unsigned (hex)
 * - {u}: Unsigned
 * - {S}: Dynamic String (base/string.h)
 * - {Sr}: Raw dynamic string (free it after use) (base/string.h)
 * - {zu} size_t
 */
char *
format(const char *fmt, ...);

#endif // LILY_BASE_FORMAT_H
