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

#include <base/alloc.h>
#include <base/color.h>
#include <base/format.h>

#include <core/cc/ci/diagnostic/error.h>

#include <stdio.h>
#include <stdlib.h>

char *
to_msg__CIError(const CIError *self)
{
    switch (self->kind) {
        case CI_ERROR_KIND_UNCLOSED_COMMENT_BLOCK:
            return "unclosed comment block";
        case CI_ERROR_KIND_UNCLOSED_COMMENT_DOC:
            return "unclosed comment doc";
        case CI_ERROR_KIND_MISMATCHED_CLOSING_DELIMITER:
            return "mismatched closing delimiter";
        case CI_ERROR_KIND_UNCLOSED_CHAR_LITERAL:
            return "unclosed char literal";
        case CI_ERROR_KIND_INVALID_ESCAPE:
            return "invalid escape";
        case CI_ERROR_KIND_UNCLOSED_STRING_LITERAL:
            return "unclosed string literal";
        case CI_ERROR_KIND_REQUIRED_C89_OR_LATER:
            return "required C89 or later required";
        case CI_ERROR_KIND_REQUIRED_C95_OR_LATER:
            return "required C95 or later standard";
        case CI_ERROR_KIND_REQUIRED_C99_OR_LATER:
            return "required C99 or later standard";
        case CI_ERROR_KIND_REQUIRED_C11_OR_LATER:
            return "required C11 or later standard";
        case CI_ERROR_KIND_REQUIRED_C17_OR_LATER:
            return "required C17 or later standard";
        case CI_ERROR_KIND_REQUIRED_C23_OR_LATER:
            return "required C23 or later standard";
        case CI_ERROR_KIND_PREPROCESSOR_ERROR:
            return self->preprocessor_error;
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_code__CIError(const CIError *self)
{
    switch (self->kind) {
        case CI_ERROR_KIND_UNCLOSED_COMMENT_BLOCK:
            return "0001";
        case CI_ERROR_KIND_UNCLOSED_COMMENT_DOC:
            return "0002";
        case CI_ERROR_KIND_MISMATCHED_CLOSING_DELIMITER:
            return "0003";
        case CI_ERROR_KIND_INVALID_ESCAPE:
            return "0004";
        case CI_ERROR_KIND_UNCLOSED_STRING_LITERAL:
            return "0005";
        case CI_ERROR_KIND_REQUIRED_C89_OR_LATER:
            return "0006";
        case CI_ERROR_KIND_REQUIRED_C95_OR_LATER:
            return "0007";
        case CI_ERROR_KIND_REQUIRED_C99_OR_LATER:
            return "0008";
        case CI_ERROR_KIND_REQUIRED_C11_OR_LATER:
            return "0009";
        case CI_ERROR_KIND_REQUIRED_C17_OR_LATER:
            return "0010";
        case CI_ERROR_KIND_REQUIRED_C23_OR_LATER:
            return "0011";
        case CI_ERROR_KIND_PREPROCESSOR_ERROR:
            return "0012";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_string__CIError(const CIError *self)
{
    char *msg = to_msg__CIError(self);
    char *res =
      format("{sa}[{s}]: {s}", RED("error"), to_code__CIError(self), msg);

    return res;
}
