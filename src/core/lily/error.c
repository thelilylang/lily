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

#include <base/alloc.h>
#include <base/color.h>
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
        case LILY_ERROR_KIND_INT8_OUT_OF_RANGE:
            return "Int8 is out of range";
        case LILY_ERROR_KIND_INT16_OUT_OF_RANGE:
            return "Int16 is out of range";
        case LILY_ERROR_KIND_INT32_OUT_OF_RANGE:
            return "Int32 is out of range";
        case LILY_ERROR_KIND_INT64_OUT_OF_RANGE:
            return "Int64 is out of range";
        case LILY_ERROR_KIND_UINT8_OUT_OF_RANGE:
            return "Uint8 is out of range";
        case LILY_ERROR_KIND_UINT16_OUT_OF_RANGE:
            return "Uint16 is out of range";
        case LILY_ERROR_KIND_UINT32_OUT_OF_RANGE:
            return "Uint32 is out of range";
        case LILY_ERROR_KIND_UINT64_OUT_OF_RANGE:
            return "Uint64 is out of range";
        case LILY_ERROR_KIND_ISIZE_OUT_OF_RANGE:
            return "Isize is ouf of range";
        case LILY_ERROR_KIND_USIZE_OUT_OF_RANGE:
            return "Usize is ouf of range";
        case LILY_ERROR_KIND_INVALID_LITERAL_SUFFIX:
            return "invalid literal suffix";
        case LILY_ERROR_KIND_INVALID_HEXADECIMAL_LITERAL:
            return "invalid hexadecimal literal";
        case LILY_ERROR_KIND_INVALID_OCTAL_LITERAL:
            return "invalid octal literal";
        case LILY_ERROR_KIND_INVALID_BIN_LITERAL:
            return "invalid binary literal";
        case LILY_ERROR_KIND_INVALID_FLOAT_LITERAL:
            return "invalid float literal";
        case LILY_ERROR_KIND_MISMATCHED_CLOSING_DELIMITER:
            return "mismatched closing delimiter";
        case LILY_ERROR_KIND_UNEXPECTED_CHARACTER:
            return "unexpected character";
        case LILY_ERROR_KIND_EXPECTED_IMPORT_VALUE:
            return "expected import value";
        case LILY_ERROR_KIND_EXPECTED_IDENTIFIER:
            return "expected identifier";
        case LILY_ERROR_KIND_DUPLICATE_PACKAGE_DECLARATION:
            return "duplicate package declaration";
        case LILY_ERROR_KIND_PACKAGE_NAME_ALREADY_DEFINED:
            return "package name is already defined";
        case LILY_ERROR_KIND_BAD_IMPORT_VALUE:
            return "bad import value";
        case LILY_ERROR_KIND_UNKNOWN_IMPORT_AT_FLAG:
            return "unknown import `@` flag";
        case LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE:
            return "unexpected character in import value";
        case LILY_ERROR_KIND_NAME_CONFLICT:
            return "name conflict";
        case LILY_ERROR_KIND_EOF_NOT_EXPECTED:
            return "EOF is not expected";
        case LILY_ERROR_KIND_EXPECTED_MODULE_IDENTIFIER:
            return "expected module identifier";
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
        case LILY_ERROR_KIND_INT8_OUT_OF_RANGE:
            return "0007";
        case LILY_ERROR_KIND_INT16_OUT_OF_RANGE:
            return "0008";
        case LILY_ERROR_KIND_INT32_OUT_OF_RANGE:
            return "0009";
        case LILY_ERROR_KIND_INT64_OUT_OF_RANGE:
            return "0010";
        case LILY_ERROR_KIND_UINT8_OUT_OF_RANGE:
            return "0011";
        case LILY_ERROR_KIND_UINT16_OUT_OF_RANGE:
            return "0012";
        case LILY_ERROR_KIND_UINT32_OUT_OF_RANGE:
            return "0013";
        case LILY_ERROR_KIND_UINT64_OUT_OF_RANGE:
            return "0014";
        case LILY_ERROR_KIND_ISIZE_OUT_OF_RANGE:
            return "0015";
        case LILY_ERROR_KIND_USIZE_OUT_OF_RANGE:
            return "0016";
        case LILY_ERROR_KIND_INVALID_LITERAL_SUFFIX:
            return "0017";
        case LILY_ERROR_KIND_INVALID_HEXADECIMAL_LITERAL:
            return "0018";
        case LILY_ERROR_KIND_INVALID_OCTAL_LITERAL:
            return "0019";
        case LILY_ERROR_KIND_INVALID_BIN_LITERAL:
            return "0020";
        case LILY_ERROR_KIND_INVALID_FLOAT_LITERAL:
            return "0021";
        case LILY_ERROR_KIND_MISMATCHED_CLOSING_DELIMITER:
            return "0022";
        case LILY_ERROR_KIND_UNEXPECTED_CHARACTER:
            return "0023";
        case LILY_ERROR_KIND_EXPECTED_IMPORT_VALUE:
            return "0024";
        case LILY_ERROR_KIND_EXPECTED_IDENTIFIER:
            return "0025";
        case LILY_ERROR_KIND_DUPLICATE_PACKAGE_DECLARATION:
            return "0026";
        case LILY_ERROR_KIND_PACKAGE_NAME_ALREADY_DEFINED:
            return "0027";
        case LILY_ERROR_KIND_BAD_IMPORT_VALUE:
            return "0028";
        case LILY_ERROR_KIND_UNKNOWN_IMPORT_AT_FLAG:
            return "0029";
        case LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE:
            return "0030";
        case LILY_ERROR_KIND_NAME_CONFLICT:
            return "0031";
        case LILY_ERROR_KIND_EOF_NOT_EXPECTED:
            return "0032";
        case LILY_ERROR_KIND_EXPECTED_MODULE_IDENTIFIER:
            return "0033";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
to_string__LilyError(const LilyError *self)
{
    char *msg = to_msg__LilyError(self);
    char *res =
      format("{sa}[{s}]: {s}", RED("error"), to_code__LilyError(self), msg);

    switch (self->kind) {
        case LILY_ERROR_KIND_UNEXPECTED_TOKEN:
            lily_free(msg);
            break;
        default:
            break;
    }

    return res;
}
