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

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void
read_pipe__CISelfTestPoll(String *buffer, int read_fd);

/// @brief Make `read_fd` non-blocking, so that reading from it never suspends
/// the polling loop when the child has nothing to say.
static void
set_non_blocking__CISelfTestPoll(int read_fd);

/// @brief Number of seconds elapsed since `start`, on a clock that advances
/// even while the process is not consuming any CPU.
static double
elapsed_since__CISelfTestPoll(const struct timespec *start);

enum CISelfTestPollHandleFlagReturnStatus
{
    CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_SKIP,
    CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_FAILED,
    CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_SUCCESS
};

/// @param expected const Vec<String*>*? (&)
/// @param actual String* (&)
static enum CISelfTestPollHandleFlagReturnStatus
handle_expected_compiler_error__CISelfTestPoll(
  const CISelfTestProcessUnit *process_unit,
  const Vec *expected,
  String *actual);

enum CISelfTestPollHandleFlagReturnStatus
handle_expected_compiler_error__CISelfTestPoll(
  const CISelfTestProcessUnit *process_unit,
  const Vec *expected,
  String *actual)
{
    if (!expected) {
        return CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_SKIP;
    }

    // NOTE: Each assertion is a containment check, not an equality one. A
    // diagnostic embeds the absolute path of the file it points at, which
    // depends on where the test suite is run from, so a fixture can only assert
    // the stable part of the message (the error code and its text). Every
    // assertion has to hold, and each is looked up on its own, because the
    // lines they describe are not contiguous in the output.
    for (Usize i = 0; i < expected->len; ++i) {
        String *current = get__Vec(expected, i);

        if (!strstr(actual->buffer, current->buffer)) {
            display_failed_expected_compiler_error_assertion_output__CISelfTestDiagnostic(
              current, actual, process_unit->path->buffer);

            return CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_FAILED;
        }
    }

    return CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_SUCCESS;
}

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
set_non_blocking__CISelfTestPoll(int read_fd)
{
    const int flags = fcntl(read_fd, F_GETFL, 0);

    if (flags == -1 || fcntl(read_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        UNREACHABLE("failed to make the pipe non-blocking");
    }
}

double
elapsed_since__CISelfTestPoll(const struct timespec *start)
{
    struct timespec current;

    clock_gettime(CLOCK_MONOTONIC, &current);

    return (double)(current.tv_sec - start->tv_sec) +
           (double)(current.tv_nsec - start->tv_nsec) / 1000000000.0;
}

void
run__CISelfTestPoll(const CISelfTestProcessUnit *process_unit,
                    Atomic(Usize) * n_test_failed)
{
#define POLL_TIMEOUT 2.0         /* seconds */
#define POLL_INTERVAL_NS 1000000 /* 1 ms */

    int exit_status;
    int kill_signal;
    int stop_signal;
    String *output = NEW(String);
    String *compiler_error = NEW(String);

    // The read ends must not block. A test that hangs without writing anything
    // would otherwise leave us stuck inside `read`, never re-checking the
    // deadline below, which makes the timeout unreachable.
    set_non_blocking__CISelfTestPoll(process_unit->read_out_fd);
    set_non_blocking__CISelfTestPoll(process_unit->read_compiler_error_fd);
    set_non_blocking__CISelfTestPoll(process_unit->read_diagnostic_fd);

    // NOTE: This must be a wall clock, not `clock()`: the latter measures the
    // CPU time of the whole process, which several poll threads consume at
    // once, and which does not advance at all while we sleep between two
    // iterations.
    struct timespec start;

    clock_gettime(CLOCK_MONOTONIC, &start);

    while (elapsed_since__CISelfTestPoll(&start) < POLL_TIMEOUT) {
        exit_status = -1;
        kill_signal = -1;
        stop_signal = -1;

        // We must drain the pipe before using `waitpid`, because the pipe
        // buffer is only 4096 bytes under Linux (pipe(7)). Thus, if we reach
        // the end of this buffer, write to stdout is blocked (and this will
        // block the child process).
        read_pipe__CISelfTestPoll(output, process_unit->read_out_fd);
        read_pipe__CISelfTestPoll(compiler_error,
                                  process_unit->read_compiler_error_fd);

        Fork wait_pid = wait_or_run__Fork(
          process_unit->pid, &exit_status, &kill_signal, &stop_signal, true);

        if (wait_pid > 0) {
            // The child is gone, so its write ends are closed and these reads
            // now stop on a real end of file. This drain is what guarantees we
            // have the whole output: a non-blocking read above may well have
            // stopped on `EAGAIN` in the middle of what the child was writing.
            read_pipe__CISelfTestPoll(output, process_unit->read_out_fd);
            read_pipe__CISelfTestPoll(compiler_error,
                                      process_unit->read_compiler_error_fd);

            String *diagnostic = NEW(String);

            // What we write on the diagnosic pipe is quite small, so it should
            // never block, because the (pipe) buffer limit is reached.
            read_pipe__CISelfTestPoll(diagnostic,
                                      process_unit->read_diagnostic_fd);
            bool has_diagnostic = diagnostic->len > 0;

            if (has_diagnostic) {
                goto write_diagnostic;
            }

            bool has_err = (exit_status != -1 && exit_status != EXIT_OK) ||
                           kill_signal != -1 || stop_signal != -1;
            enum CISelfTestPollHandleFlagReturnStatus return_status;

            return_status = handle_expected_compiler_error__CISelfTestPoll(
              process_unit,
              process_unit->metadata.expected_compiler_errors,
              compiler_error);

            if (return_status ==
                CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_FAILED) {
                ++(*n_test_failed);

                goto clean_wait;
            } else if (return_status ==
                       CI_SELF_TEST_POLL_HANDLE_FLAG_RETURN_STATUS_SUCCESS) {
                // A test asserting a compiler error is expected to exit with a
                // failing status, so that part is no longer an error. Dying on
                // a signal is another matter: the compiler must report the
                // diagnostic and stop, not report it and then crash.
                has_err = kill_signal != -1 || stop_signal != -1;
            }

            if (has_err) {
                ++(*n_test_failed);

                if (output->len > 0) {
                    write__Fd(LILY_STDERR_FILENO, output->buffer, output->len);
                }

                display_failed_test_output__CISelfTestDiagnostic(
                  LILY_STDERR_FILENO,
                  process_unit->path->buffer,
                  exit_status,
                  kill_signal,
                  stop_signal);

                goto clean_wait;
            }

            goto display_pass;

        write_diagnostic:
            ++(*n_test_failed);
            write__Fd(LILY_STDERR_FILENO, diagnostic->buffer, diagnostic->len);

            goto clean_wait;

        display_pass:
            display_pass_test_output__CISelfTestDiagnostic(
              process_unit->path->buffer,
              (double)(clock() - process_unit->start) / CLOCKS_PER_SEC);

        clean_wait:
            FREE(String, diagnostic);

            goto done;
        }

        // Now that the reads no longer block, nothing else in this loop waits,
        // so we yield instead of spinning on the CPU until the next iteration.
        nanosleep(
          &(struct timespec){ .tv_sec = 0, .tv_nsec = POLL_INTERVAL_NS }, NULL);
    }

    // The child has run out of time. It is still alive at this point (the loop
    // is only left without reaching `done` when `waitpid` never reported the
    // process as terminated), so we must kill it and reap it, otherwise it
    // would be left running for the remainder of the suite.
    //
    // The whole process group is targeted, not just the child: the test binary
    // it runs is a process of its own, and it is usually the one that hangs.
    ++(*n_test_failed);

    if (kill(-process_unit->pid, SIGKILL) == -1) {
        kill(process_unit->pid, SIGKILL);
    }

    wait__Fork(process_unit->pid, NULL, NULL, NULL, true);

    display_failed_timeout__CISelfTestDiagnostic(LILY_STDERR_FILENO,
                                                 process_unit->path->buffer);

done:
    FREE(String, output);
    FREE(String, compiler_error);

#undef POLL_TIMEOUT
#undef POLL_INTERVAL_NS
}
