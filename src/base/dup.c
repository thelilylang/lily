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

#include <base/dup.h>
#include <base/macros.h>

#include <stdio.h>
#include <stdlib.h>

#if defined(LILY_UNIX_OS) || defined(LILY_WINDOWS_OS)
#ifdef LILY_UNIX_OS
#include <unistd.h>

#define DUP(oldfd) dup(oldfd)
#define DUP2(oldfd, newfd) dup2(oldfd, newfd)
#else
#include <io.h>

#define DUP(oldfd) _dup(oldfd)
#define DUP2(oldfd, newfd) _dup2(oldfd, newfd)
#endif

int
run__Dup(int oldfd)
{
    int res = DUP(oldfd);

    if (res == -1) {
        UNREACHABLE("failed to perform dup(...)");
    }

    return res;
}

int
run2__Dup(int oldfd, int newfd)
{
    int res = DUP2(oldfd, newfd);

    if (res == -1) {
        UNREACHABLE("failed to perform dup2(...)");
    }

    return res;
}
#undef DUP
#undef DUP2
#else
#error "this OS is not yet supported"
#endif
