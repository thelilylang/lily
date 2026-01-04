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

#ifndef LILY_BASE_FD_H
#define LILY_BASE_FD_H

#include <base/platform.h>

#ifdef LILY_UNIX_OS
#include <fcntl.h>
#include <sys/types.h>

typedef ssize_t FdReadResult;
typedef ssize_t FdWriteResult;
typedef size_t FdBufferSize;

/**
 *
 * @brief Wrapper of open(...) function.
 */
int
open__Fd(const char *pathname, int flags);

/**
 *
 * @brief Wrapper of open(...) function.
 */
int
open_with_mode__Fd(const char *pathname, int flags, mode_t mode);

/**
 *
 * @brief Wrapper of close(...) function.
 */
int
close__Fd(int fd);

/**
 *
 * @brief Wrapper of read(...) function which may fail.
 * @see https://man7.org/linux/man-pages/man2/read.2.html
 */
FdReadResult
try_read__Fd(int fd, void *buffer, FdBufferSize count);

/**
 *
 * @brief Wrapper of read(...) function.
 * @see https://man7.org/linux/man-pages/man2/read.2.html
 */
FdReadResult
read__Fd(int fd, void *buffer, FdBufferSize count);

/**
 *
 * @brief Wrapper of write(...) function which may fail.
 * @see https://man7.org/linux/man-pages/man2/write.2.html
 */
FdWriteResult
try_write__Fd(int fd, void *buffer, FdBufferSize count);

/**
 *
 * @brief Wrapper of write(...) function.
 * @see https://man7.org/linux/man-pages/man2/write.2.html
 */
FdWriteResult
write__Fd(int fd, void *buffer, FdBufferSize count);
#else
#error "this OS is not yet supported"
#endif

#endif // LILY_BASE_FD_H
