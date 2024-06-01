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

#include <base/alloc.h>
#include <base/assert.h>
#include <base/command.h>
#include <base/format.h>
#include <base/new.h>

#include <core/cc/ci/builtin.h>
#include <core/cc/ci/result.h>

// https://gcc.gnu.org/onlinedocs/gcc/Standards.html
static const char *std[CI_STANDARD_23 + 1] = {
    [CI_STANDARD_NONE] = "",
    [CI_STANDARD_KR] = "c89", // NOTE: Not real K&R support of GCC, Clang
    [CI_STANDARD_89] = "c89", [CI_STANDARD_95] = "iso9899:199409",
    [CI_STANDARD_99] = "c99", [CI_STANDARD_11] = "c11",
    [CI_STANDARD_17] = "c17", [CI_STANDARD_23] = "c2x",
};

static CIResultFile *builtin_file_ref = NULL; // CIResultFile* (&)

String *
generate__CIBuiltin(const CIConfig *config)
{
    char *command = format("{S} -dM -E -std={s} - < /dev/null",
                           config->compiler.path,
                           std[config->standard]);
    String *builtin_h = save__Command(command);

    lily_free(command);

    // Add macro:
    // __STRICT_ANSI__
    // _ISOC99_SOURCE
    // _ISOC11_SOURCE
    // _ISOC2X_SOURCE
    switch (config->standard) {
        case CI_STANDARD_KR:
        case CI_STANDARD_89:
        case CI_STANDARD_95:
            push_str__String(builtin_h, "#define __STRICT_ANSI__\n");

            break;
        case CI_STANDARD_99:
            push_str__String(builtin_h, "#define _ISOC99_SOURCE\n");

            break;
        case CI_STANDARD_11:
            push_str__String(builtin_h, "#define _ISOC11_SOURCE\n");

            break;
        case CI_STANDARD_23:
            push_str__String(builtin_h, "#define _ISOC2X_SOURCE\n");

            break;
        default:
            break;
    }

    // Add _XOPEN_SOURCE macro.
    switch (config->standard) {
        case CI_STANDARD_99:
            push_str__String(builtin_h, "#define _XOPEN_SOURCE 600\n");

            break;
        default:
            push_str__String(builtin_h, "#define _XOPEN_SOURCE 500\n");
    }

    return builtin_h;
}

void
set__CIBuiltin(CIResultFile *builtin_file)
{
    builtin_file_ref = builtin_file;
}

CIResultFile *
get_ref__CIBuiltin()
{
    ASSERT(builtin_file_ref);

    return builtin_file_ref;
}
