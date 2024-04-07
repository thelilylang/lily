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

#define _GNU_SOURCE

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
    FILE *f = _popen(cmd, "r");
#else
    FILE *f = popen(cmd, "r");
#endif

    if (!f) {
        FAILED("cannot run command");
    }

    String *output = NEW(String);

    for (char c = fgetc(f); c; c = fgetc(f)) {
        push__String(output, c);
    }

#ifdef LILY_WINDOWS_OS
    _pclose(f);
#else
    pclose(f);
#endif

    return output;
}
