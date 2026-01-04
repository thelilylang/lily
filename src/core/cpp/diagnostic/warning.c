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

#include <base/color.h>
#include <base/format.h>

#include <core/cpp/diagnostic/warning.h>

#include <stdio.h>
#include <stdlib.h>

char *
to_msg__CppWarning(const CppWarning *self)
{
    switch (self->kind) {
        case CPP_WARNING_KIND_UNUSED_PAREN:
            return "unused paren";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_code__CppWarning(const CppWarning *self)
{
    switch (self->kind) {
        case CPP_WARNING_KIND_UNUSED_PAREN:
            return "0001";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_string__CppWarning(const CppWarning *self)
{
    char *msg = to_msg__CppWarning(self);
    char *res = format(
      "{sa}[{s}]: {s}", YELLOW("warning"), to_code__CppWarning(self), msg);

    switch (self->kind) {
        default:
            break;
    }

    return res;
}
