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

#define _GNU_SOURCE

#include <base/assert.h>
#include <base/color.h>
#include <base/fd.h>
#include <base/fork.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/print.h>
#include <base/std_fileno.h>

#include <command/ci/self_test/diagnostic.h>
#include <command/ci/self_test/poll.h>
#include <command/ci/self_test/process_unit.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void
read_pipe__CISelfTestPoll(String *buffer, int read_fd);

void
read_pipe__CISelfTestPoll(String *buffer, int read_fd)
{
    FdReadResult pipe_read_buffer_len;
    char pipe_read_buffer[1024];

    while ((pipe_read_buffer_len = try_read__Fd(
              read_fd, pipe_read_buffer, sizeof(pipe_read_buffer) - 1)) > 0) {
        pipe_read_buffer[pipe_read_buffer_len] = '\0';
        push_str_with_len__String(
          buffer, pipe_read_buffer, pipe_read_buffer_len);
    }
}

void
run__CISelfTestPoll(const CISelfTestProcessUnit *process_unit,
                    Atomic(Usize) * n_test_failed)
{
#define POLL_TIMEOUT 2 * CLOCKS_PER_SEC

    int exit_status;
    int kill_signal;
    int stop_signal;
    String *output = NEW(String);
    clock_t start = clock();

    while ((double)(clock() - start) < POLL_TIMEOUT) {
        exit_status = -1;
        kill_signal = -1;
        stop_signal = -1;

        // We must drain the pipe before using `waitpid`, because the pipe
        // buffer is only 4096 bytes under Linux (pipe(7)). Thus, if we reach
        // the end of this buffer, write to stdout is blocked (and this will
        // block the child process).
        read_pipe__CISelfTestPoll(output, process_unit->read_out_fd);

        Fork wait_pid = wait_or_run__Fork(
          process_unit->pid, &exit_status, &kill_signal, &stop_signal, true);

        if (wait_pid > 0) {
            String *diagnostic = NEW(String);

            // What we write on the diagnosic pipe is quite small, so it should
            // never block, because the (pipe) buffer limit is reached.
            read_pipe__CISelfTestPoll(diagnostic,
                                      process_unit->read_diagnostic_fd);

            bool has_diagnostic = diagnostic->len > 0;
            bool has_err = (exit_status != -1 && exit_status != EXIT_OK) ||
                           kill_signal != -1 || stop_signal != -1;

            if (has_err) {
                ++(*n_test_failed);

                if (output->len > 0) {
                    write__Fd(LILY_STDOUT_FILENO, output->buffer, output->len);
                }

                if (!has_diagnostic || (has_err && exit_status != EXIT_ERR)) {
                    has_diagnostic = false;

                    display_failed_test_output__CISelfTestDiagnostic(
                      LILY_STDOUT_FILENO,
                      process_unit->path->buffer,
                      exit_status,
                      kill_signal,
                      stop_signal);
                }
            }

            if (has_diagnostic) {
                write__Fd(
                  LILY_STDOUT_FILENO, diagnostic->buffer, diagnostic->len);
            }

            FREE(String, diagnostic);

            goto end;
        }
    }

    ASSERT(kill(process_unit->pid, 0) == 0);
    display_failed_timeout__CISelfTestDiagnostic(LILY_STDOUT_FILENO,
                                                 process_unit->path->buffer);

end:
    FREE(String, output);

#undef POLL_TIMEOUT
}
