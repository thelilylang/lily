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
#include <base/command.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/platform.h>

#include <stdio.h>
#include <stdlib.h>

void
run__Command(const char *cmd)
{
    if (system(cmd) != 0) {
        FAILED("failed to execute command");
    }
}

String *
save__Command(const char *cmd)
{
#ifdef LILY_WINDOWS_OS
    FILE *file = _popen(cmd, "r");
#else
    FILE *file = popen(cmd, "r");
#endif

    if (!file) {
        FAILED("cannot run command");
    }

    String *output = NEW(String);
#define TEMP_BUFFER_LEN 1024
    char temp_buffer[TEMP_BUFFER_LEN];
    Usize temp_buffer_bytes = 0;

    while ((temp_buffer_bytes =
              fread(temp_buffer, 1, TEMP_BUFFER_LEN - 1, file)) > 0) {
        temp_buffer[temp_buffer_bytes] = '\0';
        push_str__String(output, temp_buffer);
    }

    ASSERT(feof(file));

#ifdef LILY_WINDOWS_OS
    _pclose(file);
#else
    pclose(file);
#endif

    return output;
}
