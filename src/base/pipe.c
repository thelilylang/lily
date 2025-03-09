/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#include <base/macros.h>
#include <base/pipe.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef LILY_UNIX_OS
void
create__Pipe(Pipefd pipefd)
{
    if (pipe(pipefd) == -1) {
        UNREACHABLE("cannot create pipe");
    }
}

void
create2__Pipe(Pipefd pipefd, PipeFlags flags)
{
    if (pipe2(pipefd, flags) == -1) {
        UNREACHABLE("cannot create pipe");
    }
}

void
close_read__Pipe(Pipefd pipefd)
{
    if (close(pipefd[PIPE_READ_FD]) == -1) {
        UNREACHABLE("failed to close read fd of pipe");
    }
}

void
close_write__Pipe(Pipefd pipefd)
{
    if (close(pipefd[PIPE_WRITE_FD]) == -1) {
        UNREACHABLE("failed to close write fd of pipe");
    }
}

void
close__Pipe(Pipefd pipefd)
{
    close_read__Pipe(pipefd);
    close_write__Pipe(pipefd);
}
#else
#error "this OS is not yet supported"
#endif
