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
#include <base/command.h>
#include <base/format.h>
#include <base/new.h>

#include <core/cc/ci/builtin.h>

#include <stdio.h>

String *
generate_builtin__CIBuiltin(const CIConfig *config)
{
    String *builtin_h = NULL;

    switch (config->compiler.kind) {
        case CI_COMPILER_KIND_CLANG:
            builtin_h = save__Command("clang -dM -E - < /dev/null");
            break;
        case CI_COMPILER_KIND_GCC:
            builtin_h = save__Command("gcc -dM -E - < /dev/null");
            break;
        default:
            UNREACHABLE("unknown compiler");
    }

    // Add macro:
    // __STRICT_ANSI__
    // _ISOC99_SOURCE
    // _ISOC11_SOURCE
    // _ISOC2X_SOURCE
    switch (config->standard) {
        case CI_STANDARD_KR:
        case CI_STANDARD_89:
        case CI_STANDARD_95:
            push_str__String(builtin_h, "#define __STRICT_ANSI__");

            break;
        case CI_STANDARD_99:
            push_str__String(builtin_h, "#define _ISOC99_SOURCE");

            break;
        case CI_STANDARD_11:
            push_str__String(builtin_h, "#define _ISOC11_SOURCE");

            break;
        case CI_STANDARD_23:
            push_str__String(builtin_h, "#define _ISOC2X_SOURCE");

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
