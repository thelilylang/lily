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

#include <core/lily/diagnostic/warning.h>

#include <stdio.h>
#include <stdlib.h>

char *
to_msg__LilyWarning(const LilyWarning *self)
{
    switch (self->kind) {
        case LILY_WARNING_KIND_UNUSED_PAREN:
            return "unused paren";
        case LILY_WARNING_KIND_UNUSED_SEMICOLON:
            return "unused semicolon";
        case LILY_WARNING_KIND_UNUSED_MACRO_PARAM:
            return "unused macro parameter";
        case LILY_WARNING_KIND_UNREACHABLE_CODE:
            return "unreachable code";
        case LILY_WARNING_KIND_UNUSED_MULTIPLE_CAPTURE:
            return "unused multiple capture";
        case LILY_WARNING_KIND_UNUSED_CAPTURE:
            return "unused capture";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_code__LilyWarning(const LilyWarning *self)
{
    switch (self->kind) {
        case LILY_WARNING_KIND_UNUSED_PAREN:
            return "0001";
        case LILY_WARNING_KIND_UNUSED_SEMICOLON:
            return "0002";
        case LILY_WARNING_KIND_UNUSED_MACRO_PARAM:
            return "0003";
        case LILY_WARNING_KIND_UNREACHABLE_CODE:
            return "0004";
        case LILY_WARNING_KIND_UNUSED_MULTIPLE_CAPTURE:
            return "0005";
        case LILY_WARNING_KIND_UNUSED_CAPTURE:
            return "0006";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_string__LilyWarning(const LilyWarning *self)
{
    char *msg = to_msg__LilyWarning(self);
    char *res = format(
      "{sa}[{s}]: {s}", YELLOW("warning"), to_code__LilyWarning(self), msg);

    switch (self->kind) {
        default:
            break;
    }

    return res;
}
