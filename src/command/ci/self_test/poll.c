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
#include <base/fork.h>
#include <base/macros.h>
#include <base/print.h>

#include <command/ci/self_test/poll.h>

#include <stdio.h>
#include <stdlib.h>

void
run__CISelfTestPoll()
{
    while (true) {
        int exit_status = -1;
        int kill_signal = -1;
        int stop_signal = -1;

        if (wait__Fork(-1, &exit_status, &kill_signal, &stop_signal, true) >
            0) {
            if ((exit_status != -1 && exit_status != EXIT_OK) ||
                kill_signal != -1 || stop_signal != -1) {
                PRINTLN("{sa}", RED("ERR"));
            }

            continue;
        }

        break;
    }
}
