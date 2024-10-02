/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#define USE_FORK(pid, child_process, exit_status, kill_signal, stop_signal) \
    {                                                                       \
        switch (pid) {                                                      \
            case -1:                                                        \
                UNREACHABLE("failed to fork process");                      \
            case 0:                                                         \
                child_process;                                              \
                exit(EXIT_OK);                                              \
            default: {                                                      \
                int status;                                                 \
                pid_t w = waitpid(pid, &status, 0);                         \
                                                                            \
                if (w == -1) {                                              \
                    UNREACHABLE("something wrong with waitpid");            \
                } else {                                                    \
                    if (WIFEXITED(status)) {                                \
                        (exit_status) = WEXITSTATUS(status);                \
                    } else if (WIFSIGNALED(status)) {                       \
                        (kill_signal) = WTERMSIG(status);                   \
                    } else if (WIFSTOPPED(status)) {                        \
                        (stop_signal) = WSTOPSIG(status);                   \
                    }                                                       \
                }                                                           \
                                                                            \
                break;                                                      \
            }                                                               \
        }                                                                   \
    }
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
