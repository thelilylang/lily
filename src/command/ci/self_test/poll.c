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

#include <base/color.h>
#include <base/fd.h>
#include <base/fork.h>
#include <base/macros.h>
#include <base/print.h>
#include <base/std_fileno.h>

#include <command/ci/self_test/diagnostic.h>
#include <command/ci/self_test/poll.h>
#include <command/ci/self_test/process_unit.h>

#include <stdio.h>
#include <stdlib.h>

void
run__CISelfTestPoll(Vec *process_units, Usize *n_test_failed)
{
    for (Usize i = 0; i < process_units->len; ++i) {
        CISelfTestProcessUnit *process_unit = get__Vec(process_units, i);
        int exit_status = -1;
        int kill_signal = -1;
        int stop_signal = -1;

        if (wait__Fork(process_unit->pid,
                       &exit_status,
                       &kill_signal,
                       &stop_signal,
                       true) > 0) {
            FdReadResult bytes_read;
            char buffer[256];

            while ((bytes_read = try_read__Fd(
                      process_unit->read_fd, buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytes_read] = '\0';
                write__Fd(LILY_STDOUT_FILENO, buffer, bytes_read);
            }

            if ((exit_status != -1 && exit_status != EXIT_OK) ||
                kill_signal != -1 || stop_signal != -1) {
                ++(*n_test_failed);

                display_failed_test_output__CISelfTestDiagnostic(
                  process_unit->path->buffer,
                  exit_status,
                  kill_signal,
                  stop_signal);
            }
        }
    }
}
