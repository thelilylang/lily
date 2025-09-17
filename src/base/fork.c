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

#include <base/fork.h>
#include <base/macros.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef LILY_UNIX_OS
static inline Fork
wait_base__Fork(Fork pid,
                int *exit_status,
                int *kill_signal,
                int *stop_signal,
                bool allow_w_err,
                int wait_options);

void
use__Fork(Fork pid,
          void (*child_process)(void),
          int *exit_status,
          int *kill_signal,
          int *stop_signal)
{
    switch (pid) {
        case -1:
            UNREACHABLE("failed to fork process");
        case 0:
            child_process();
            exit(EXIT_OK);
        default:
            wait__Fork(pid, exit_status, kill_signal, stop_signal, false);
    }
}

Fork
wait_base__Fork(Fork pid,
                int *exit_status,
                int *kill_signal,
                int *stop_signal,
                bool allow_w_err,
                int wait_options)
{
    int wstatus;
    Fork w = waitpid(pid, &wstatus, wait_options);

    if (w == -1) {
        if (allow_w_err) {
            return w;
        }

        UNREACHABLE("something wrong with waitpid");
    } else if (exit_status && WIFEXITED(wstatus)) {
        *exit_status = WEXITSTATUS(wstatus);
    } else if (kill_signal && WIFSIGNALED(wstatus)) {
        *kill_signal = WTERMSIG(wstatus);
    } else if (stop_signal && WIFSTOPPED(wstatus)) {
        *stop_signal = WSTOPSIG(wstatus);
    }

    return w;
}

Fork
wait__Fork(Fork pid,
           int *exit_status,
           int *kill_signal,
           int *stop_signal,
           bool allow_w_err)
{
    return wait_base__Fork(
      pid, exit_status, kill_signal, stop_signal, allow_w_err, 0);
}

Fork
wait_or_run__Fork(Fork pid,
                  int *exit_status,
                  int *kill_signal,
                  int *stop_signal,
                  bool allow_w_err)
{
    return wait_base__Fork(
      pid, exit_status, kill_signal, stop_signal, allow_w_err, WNOHANG);
}
#else
#error "this OS is not yet supported"
#endif

Fork
run__Fork()
{
#ifdef LILY_UNIX_OS
    return fork();
#else
#error "this OS is not yet supported"
#endif
}
