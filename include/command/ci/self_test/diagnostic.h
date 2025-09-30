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

#ifndef LILY_COMMAND_CI_SELF_TEST_DIAGNOSTIC_H
#define LILY_COMMAND_CI_SELF_TEST_DIAGNOSTIC_H

#include <base/string.h>

/**
 *
 * @param filename const char* (&)
 * @param time Time in seconds.
 */
void
display_pass_test_output__CISelfTestDiagnostic(int fd,
                                               const char *filename,
                                               double time);

/**
 *
 * @param filename const char* (&)
 */
void
display_failed_test_output__CISelfTestDiagnostic(int fd,
                                                 const char *filename,
                                                 int exit_status,
                                                 int kill_signal,
                                                 int stop_signal);

/**
 *
 * @param expected_stdout String* (&)
 * @param actual_stdout String* (&)
 * @param filename const char* (&)
 */
void
display_failed_expected_stdout_assertion_output__CISelfTestDiagnostic(
  int fd,
  String *expected_stdout,
  String *actual_stdout,
  const char *filename);

/**
 * @param binary_path String* (&)
 * @param filename const char* (&)
 */
void
display_failed_binary_not_exist__CISelfTestDiagnostic(int fd,
                                                      String *binary_path,
                                                      const char *filename);

void
display_failed_timeout__CISelfTestDiagnostic(int fd, const char *filename);

#endif // LILY_COMMAND_CI_SELF_TEST_DIAGNOSTIC_H
