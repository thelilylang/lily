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

#ifndef LILY_BASE_FORK_H
#define LILY_BASE_FORK_H

#include <base/platform.h>

#ifdef LILY_UNIX_OS
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef pid_t Fork;

/**
 *
 * @brief This function is used to manage child processes and pass the function
 * to be executed in the child process. In addition, you can optionally pass
 * parameters to find out the exit code or signal returned by the child process.
 * @param exit_status int*? (&)
 * @param kill_signal int*? (&)
 * @param stop_signal int*? (&)
 */
void
use__Fork(Fork pid,
          void (*child_process)(void),
          int *exit_status,
          int *kill_signal,
          int *stop_signal);
#else
#error "this OS is not yet supported"
#endif

/**
 *
 * @brief Fork the process.
 */
Fork
run__Fork();

#endif // LILY_BASE_FORK_H
