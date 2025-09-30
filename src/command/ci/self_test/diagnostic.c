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

#include <base/alloc.h>
#include <base/format.h>
#include <base/new.h>

#include <command/ci/self_test/diagnostic.h>

#include <stdio.h>
#include <string.h>

void
display_pass_test_output__CISelfTestDiagnostic(int fd,
                                               const char *filename,
                                               double time)
{
    dprintf(fd,
            "\r\x1b[37m\x1b[42mPASS\x1b[0m \x1b[30m%s (%.2f s)\n\x1b[0m",
            filename,
            time);
}

void
display_failed_test_output__CISelfTestDiagnostic(int fd,
                                                 const char *filename,
                                                 int exit_status,
                                                 int kill_signal,
                                                 int stop_signal)
{
    char *exit_status_msg = exit_status != -1
                              ? format("\x1b[31m[Exit={d}]\x1b[0m", exit_status)
                              : NULL;
    char *signal_msg =
      kill_signal != -1 || stop_signal != -1
        ? format("\x1b[31m[Signal={s}]\x1b[0m",
                 strsignal(kill_signal == -1 ? stop_signal : kill_signal))
        : NULL;

    dprintf(fd,
            "\r\x1b[37m\x1b[41mFAILED\x1b[0m \x1b[30m%s (? s)\x1b[0m%s%s%s\n",
            filename,
            exit_status == -1 && kill_signal == -1 && stop_signal == -1 ? ""
                                                                        : " ",
            exit_status_msg ? exit_status_msg : "",
            signal_msg ? signal_msg : "");

    if (exit_status_msg) {
        lily_free(exit_status_msg);
    }

    if (signal_msg) {
        lily_free(signal_msg);
    }
}

void
display_failed_expected_stdout_assertion_output__CISelfTestDiagnostic(
  int fd,
  String *expected_stdout,
  String *actual_stdout,
  const char *filename)
{
    String *disabled_escapes_expected_stdout = clone__String(expected_stdout);

    disable_constant_escapes__String(disabled_escapes_expected_stdout);

    String *disabled_escapes_actual_stdout = clone__String(actual_stdout);

    disable_constant_escapes__String(disabled_escapes_actual_stdout);

    display_failed_test_output__CISelfTestDiagnostic(fd, filename, -1, -1, -1);

    dprintf(fd, "\x1b[31m@expected_stdout assertion failed:\n\x1b[0m");
    dprintf(fd,
            "\x1b[32m  - Expected: %s\n\x1b[0m",
            disabled_escapes_expected_stdout->buffer);
    dprintf(fd,
            "\x1b[31m  - Actual: %s\n\x1b[0m\n",
            disabled_escapes_actual_stdout->buffer);

    FREE(String, disabled_escapes_expected_stdout);
    FREE(String, disabled_escapes_actual_stdout);

    exit(EXIT_ERR);
}

void
display_failed_binary_not_exist__CISelfTestDiagnostic(int fd,
                                                      String *binary_path,
                                                      const char *filename)
{
    display_failed_test_output__CISelfTestDiagnostic(fd, filename, -1, -1, -1);
    dprintf(fd,
            "\x1b[31mThe following binary file does not exist: %s\n\x1b[0m",
            binary_path->buffer);
    exit(EXIT_ERR);
}

void
display_failed_timeout__CISelfTestDiagnostic(int fd, const char *filename)
{
    dprintf(
      fd,
      "\r\x1b[37m\x1b[41mFAILED\x1b[0m \x1b[30m%s (? s)\x1b[0m [TIMEOUT]\n",
      filename);
}
