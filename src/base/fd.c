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

#include <base/fd.h>
#include <base/macros.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef LILY_UNIX_OS
#include <unistd.h>

int
open__Fd(const char *pathname, int flags)
{
    int res = open(pathname, flags);

    if (res == -1) {
        UNREACHABLE("failed to open fd");
    }

    return res;
}

int
open_with_mode__Fd(const char *pathname, int flags, mode_t mode)
{
    int res = open(pathname, flags, mode);

    if (res == -1) {
        UNREACHABLE("failed to open fd");
    }

    return res;
}

int
close__Fd(int fd)
{
    int res = close(fd);

    if (res == -1) {
        UNREACHABLE("failed to close fd");
    }

    return res;
}

FdReadResult
try_read__Fd(int fd, void *buffer, FdBufferSize count)
{
    return read(fd, buffer, count);
}

FdReadResult
read__Fd(int fd, void *buffer, FdBufferSize count)
{
    FdReadResult res = read(fd, buffer, count);

    if (res == -1) {
        UNREACHABLE("failed to read fd");
    }

    return res;
}

FdWriteResult
try_write__Fd(int fd, void *buffer, FdBufferSize count)
{
    return write(fd, buffer, count);
}

FdWriteResult
write__Fd(int fd, void *buffer, FdBufferSize count)
{
    FdWriteResult res = write(fd, buffer, count);

    if (res == -1) {
        UNREACHABLE("failed to write fd");
    }

    return res;
}
#else
#error "this OS is not yet supported"
#endif
