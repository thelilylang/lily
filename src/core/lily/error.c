/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#include <base/format.h>

#include <core/lily/error.h>

#include <stdio.h>
#include <stdlib.h>

char *
to_msg__LilyError(const LilyError *self)
{
    switch (self->kind) {
        case LILY_ERROR_KIND_UNEXPECTED_TOKEN:
            return format("unexpected token: `{s}`", self->unexpected_token);
        case LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL:
            return "unclosed char literal";
        case LILY_ERROR_KIND_INVALID_ESCAPE:
            return "invalid escape";
        case LILY_ERROR_KIND_UNCLOSED_COMMENT_BLOCK:
            return "unclosed comment block";
        case LILY_ERROR_KIND_INVALID_CHAR_LITERAL:
            return "invalid char literal";
        case LILY_ERROR_KIND_UNCLOSED_STRING_LITERAL:
            return "unclosed string literal";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_code__LilyError(const LilyError *self)
{
    switch (self->kind) {
        case LILY_ERROR_KIND_UNEXPECTED_TOKEN:
            return "0001";
        case LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL:
            return "0002";
        case LILY_ERROR_KIND_INVALID_ESCAPE:
            return "0003";
        case LILY_ERROR_KIND_UNCLOSED_COMMENT_BLOCK:
            return "0004";
        case LILY_ERROR_KIND_INVALID_CHAR_LITERAL:
            return "0005";
        case LILY_ERROR_KIND_UNCLOSED_STRING_LITERAL:
            return "0006";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_string__LilyError(const LilyError *self)
{
    char *msg = to_msg__LilyError(self);
    char *res = format("error[{s}]: {s}", to_code__LilyError(self), msg);

    switch (self->kind) {
        case LILY_ERROR_KIND_UNEXPECTED_TOKEN:
            free(msg);
            break;
        default:
            break;
    }

    return res;
}