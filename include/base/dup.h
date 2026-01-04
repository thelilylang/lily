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

#ifndef LILY_BASE_DUP_H
#define LILY_BASE_DUP_H

#include <base/platform.h>

#ifdef LILY_UNIX_OS
/**
 *
 * @brief Wrapper of dup(...) function.
 * @see https://man7.org/linux/man-pages/man2/dup.2.html
 */
int
run__Dup(int oldfd);

/**
 *
 * @brief Wrapper of dup2(...) function.
 * @see https://man7.org/linux/man-pages/man2/dup.2.html
 */
int
run2__Dup(int oldfd, int newfd);
#elifdef LILY_WINDOWS_OS
/**
 *
 * @brief Wrapper of _dup(...) function.
 * @see
 * https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/dup-dup2?view=msvc-170
 */
int
run__Dup(int oldfd);

/**
 *
 * @brief Wrapper of _dup2(...) function.
 * @see
 * https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/dup-dup2?view=msvc-170
 */
int
run2__Dup(int oldfd, int newfd);
#else
#error "this OS is not yet supported"
#endif

#endif // LILY_BASE_DUP_H
