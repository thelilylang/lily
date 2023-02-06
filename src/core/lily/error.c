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
            return format("unexpected token");
        case LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL:
            return format("unclosed char literal");
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
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_string__LilyError(const LilyError *self)
{
    return format(
      "error[{s}]: {sa}", to_code__LilyError(self), to_msg__LilyError(self));
}