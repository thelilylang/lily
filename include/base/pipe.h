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

#ifndef LILY_BASE_PIPE_H
#define LILY_BASE_PIPE_H

#include <base/platform.h>

#ifdef LILY_UNIX_OS
#define _GNU_SOURCE

#include <unistd.h>

#undef PIPE_READ_FD
#undef PIPE_WRITE_FD

#define PIPE_READ_FD 0
#define PIPE_WRITE_FD 1

typedef int Pipefd[2];
typedef int PipeFlags;

/**
 *
 * @brief Create a pipe via pipe(...) function.
 */
void
create__Pipe(Pipefd pipefd);

/**
 *
 * @brief Create a pipe via pipe2(...) function.
 * @note On MacOS this function call only `create__Pipe`, because pipe2(...)
 * function doesn't exist.
 */
void
create2__Pipe(Pipefd pipefd, PipeFlags flags);

/**
 *
 * @brief Close read fd of pipe.
 */
void
close_read__Pipe(Pipefd pipefd);

/**
 *
 * @brief Close write fd of pipe.
 */
void
close_write__Pipe(Pipefd pipefd);

/**
 *
 * @brief Close read and write fd of pipe.
 */
void
close__Pipe(Pipefd pipefd);
#else
#error "this OS is not yet supported"
#endif

#endif // LILY_BASE_PIPE_H
