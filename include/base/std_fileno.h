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

#ifndef LILY_BASE_STD_FILENO_H
#define LILY_BASE_STD_FILENO_H

#include <base/platform.h>

#undef LILY_STDIN_FILENO
#undef LILY_STDOUT_FILENO
#undef LILY_STDERR_FILENO

#ifdef LILY_UNIX_OS
// [...]
// On program startup, the integer file descriptors associated with the
// streams stdin, stdout, and stderr are 0, 1, and 2, respectively. The
// preprocessor symbols STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO [...]
//
// From: https://linux.die.net/man/3/stdout

#define LILY_STDIN_FILENO 0
#define LILY_STDOUT_FILENO 1
#define LILY_STDERR_FILENO 2
#elif LILY_WINDOWS_OS
#define LILY_STDIN_FILENO 0
#define LILY_STDOUT_FILENO 1
#define LILY_STDERR_FILENO 2
#else
#error "this OS is not yet supported"
#endif

#endif // LILY_BASE_STD_FILENO_H
