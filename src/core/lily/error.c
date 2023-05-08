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
        case LILY_ERROR_KIND_EXPECTED_FUN_IDENTIFIER:
            return "expected fun identifier";
        case LILY_ERROR_KIND_EXPECTED_TOKEN:
            return "expected token";
        case LILY_ERROR_KIND_UNEXPECTED_TOKEN_IN_FUNCTION_BODY:
            return "unexpected token in function body";
        case LILY_ERROR_KIND_BAD_KIND_OF_TYPE:
            return "bad kind of type";
        case LILY_ERROR_KIND_IMPL_IS_ALREADY_DEFINED:
            return "impl is already defined";
        case LILY_ERROR_KIND_INHERIT_IS_ALREADY_DEFINED:
            return "inherit is already defined";
        case LILY_ERROR_KIND_BAD_KIND_OF_OBJECT:
            return "bad kind of object";
        case LILY_ERROR_KIND_IMPL_IS_NOT_EXPECTED:
            return "impl is not expected";
        case LILY_ERROR_KIND_INHERIT_IS_NOT_EXPECTED:
            return "inherit is not expected";
        case LILY_ERROR_KIND_EXPECTED_DATA_TYPE:
            return "expected data type";
        case LILY_ERROR_KIND_SET_IS_DUPLICATE:
            return "set is duplicate";
        case LILY_ERROR_KIND_GET_IS_DUPLICATE:
            return "get is duplicate";
        case LILY_ERROR_KIND_EXPECTED_EXPRESSION:
            return "expected expression";
        case LILY_ERROR_KIND_MISS_ONE_OR_MANY_EXPRESSIONS:
            return "miss one or many expressions";
        case LILY_ERROR_KIND_MISS_ONE_OR_MANY_IDENTIFIERS:
            return "miss one or many identifiers";
        case LILY_ERROR_KIND_EXPECTED_ONE_OR_MANY_CHARACTERS:
            return "expected one or many characters";
        case LILY_ERROR_KIND_FEATURE_NOT_YET_SUPPORTED:
            return "feature not yet supported";
        case LILY_ERROR_KIND_EXPECTED_ASM_PARAM:
            return "expected asm param(s)";
        case LILY_ERROR_KIND_EXPECTED_COMPTIME_STRING_LITERAL:
            return "expected comptime string literal";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION:
            return "expected only one expression";
        case LILY_ERROR_KIND_VARIABLE_DECLARATION_IS_NOT_EXPECTED:
            return "variable declaration is not expected";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_PATTERN:
            return "expected only one pattern";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_DATA_TYPE:
            return "expected only one data type";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_GENERIC_PARAM:
            return "expected only one generic param";
        case LILY_ERROR_KIND_UNKNOWN_FROM_VALUE_IN_LIB:
            return "unknown from value in lib";
        case LILY_ERROR_KIND_MACRO_IS_NOT_FOUND:
            return "macro is not found";
        case LILY_ERROR_KIND_MACRO_DO_NOTHING:
            return "macro do nothing";
        case LILY_ERROR_KIND_MACRO_EXPAND_MISS_FEW_PARAMS:
            return "macro expand miss few params";
        case LILY_ERROR_KIND_MACRO_EXPAND_HAVE_TOO_MANY_PARAMS:
            return "macro expand have too many params";
        case LILY_ERROR_KIND_EXPECTED_IDENTIFIER_DOLLAR:
            return "expected identifier dollar";
        case LILY_ERROR_KIND_UNKNOWN_MACRO_DATA_TYPE:
            return "unknown data type of macro param";
        case LILY_ERROR_KIND_EXPECTED_MACRO_DATA_TYPE:
            return "expected macro of data type";
        case LILY_ERROR_KIND_MACRO_EXPECTED_ID:
            return "the macro expected an id";
        case LILY_ERROR_KIND_MACRO_EXPECTED_DT:
            return "the macro expected a dt";
        case LILY_ERROR_KIND_MACRO_EXPECTED_TK:
            return "the macro expected a tk";
        case LILY_ERROR_KIND_MACRO_EXPECTED_TKS:
            return "the macro expected a tks";
        case LILY_ERROR_KIND_MACRO_EXPECTED_STMT:
            return "the macro expected a stmt";
        case LILY_ERROR_KIND_MACRO_EXPECTED_EXPR:
            return "the macro expected an expr";
        case LILY_ERROR_KIND_MACRO_EXPECTED_PATH:
            return "the macro expected a path";
        case LILY_ERROR_KIND_MACRO_EXPECTED_PATT:
            return "the macro expected a patt";
        case LILY_ERROR_KIND_MACRO_EXPECTED_BLOCK:
            return "the macro expected a block";
        case LILY_ERROR_KIND_MACRO_DUPLICATE_PARAM:
            return "the macro parameters are duplicate";
        case LILY_ERROR_KIND_MACRO_IDENTIFIER_NOT_FOUND:
            return "the macro identifier is not found";
        case LILY_ERROR_KIND_EXPECTED_PATTERN:
            return "expected pattern";
        case LILY_ERROR_KIND_DUPLICATE_CONSTANT:
            return "duplicate constant";
        case LILY_ERROR_KIND_DUPLICATE_ERROR:
            return "duplicate error";
        case LILY_ERROR_KIND_DUPLICATE_FUN:
            return "duplicate fun";
        case LILY_ERROR_KIND_DUPLICATE_MODULE:
            return "duplicate module";
        case LILY_ERROR_KIND_DUPLICATE_CLASS:
            return "duplicate class";
        case LILY_ERROR_KIND_DUPLICATE_ENUM_OBJECT:
            return "duplicate enum object";
        case LILY_ERROR_KIND_DUPLICATE_RECORD_OBJECT:
            return "duplicate record object";
        case LILY_ERROR_KIND_DUPLICATE_TRAIT:
            return "duplicate trait";
        case LILY_ERROR_KIND_DUPLICATE_ALIAS:
            return "duplicate alias";
        case LILY_ERROR_KIND_DUPLICATE_ENUM:
            return "duplicate enum";
        case LILY_ERROR_KIND_DUPLICATE_RECORD:
            return "duplicate record";
        case LILY_ERROR_KIND_DUPLICATE_PARAM_NAME:
            return "duplicate param name";
        case LILY_ERROR_KIND_DUPLICATE_VARIABLE:
            return "duplicate variable";
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
        case LILY_ERROR_KIND_EXPECTED_FUN_IDENTIFIER:
            return "0034";
        case LILY_ERROR_KIND_EXPECTED_TOKEN:
            return "0035";
        case LILY_ERROR_KIND_UNEXPECTED_TOKEN_IN_FUNCTION_BODY:
            return "0036";
        case LILY_ERROR_KIND_BAD_KIND_OF_TYPE:
            return "0037";
        case LILY_ERROR_KIND_IMPL_IS_ALREADY_DEFINED:
            return "0038";
        case LILY_ERROR_KIND_INHERIT_IS_ALREADY_DEFINED:
            return "0039";
        case LILY_ERROR_KIND_BAD_KIND_OF_OBJECT:
            return "0040";
        case LILY_ERROR_KIND_IMPL_IS_NOT_EXPECTED:
            return "0041";
        case LILY_ERROR_KIND_INHERIT_IS_NOT_EXPECTED:
            return "0042";
        case LILY_ERROR_KIND_EXPECTED_DATA_TYPE:
            return "0043";
        case LILY_ERROR_KIND_SET_IS_DUPLICATE:
            return "0044";
        case LILY_ERROR_KIND_GET_IS_DUPLICATE:
            return "0045";
        case LILY_ERROR_KIND_EXPECTED_EXPRESSION:
            return "0046";
        case LILY_ERROR_KIND_MISS_ONE_OR_MANY_EXPRESSIONS:
            return "0047";
        case LILY_ERROR_KIND_MISS_ONE_OR_MANY_IDENTIFIERS:
            return "0048";
        case LILY_ERROR_KIND_EXPECTED_ONE_OR_MANY_CHARACTERS:
            return "0049";
        case LILY_ERROR_KIND_FEATURE_NOT_YET_SUPPORTED:
            return "0050";
        case LILY_ERROR_KIND_EXPECTED_ASM_PARAM:
            return "0051";
        case LILY_ERROR_KIND_EXPECTED_COMPTIME_STRING_LITERAL:
            return "0052";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION:
            return "0053";
        case LILY_ERROR_KIND_VARIABLE_DECLARATION_IS_NOT_EXPECTED:
            return "0054";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_PATTERN:
            return "0055";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_DATA_TYPE:
            return "0056";
        case LILY_ERROR_KIND_EXPECTED_ONLY_ONE_GENERIC_PARAM:
            return "0057";
        case LILY_ERROR_KIND_UNKNOWN_FROM_VALUE_IN_LIB:
            return "0058";
        case LILY_ERROR_KIND_MACRO_IS_NOT_FOUND:
            return "0059";
        case LILY_ERROR_KIND_MACRO_DO_NOTHING:
            return "0060";
        case LILY_ERROR_KIND_MACRO_EXPAND_MISS_FEW_PARAMS:
            return "0061";
        case LILY_ERROR_KIND_MACRO_EXPAND_HAVE_TOO_MANY_PARAMS:
            return "0062";
        case LILY_ERROR_KIND_EXPECTED_IDENTIFIER_DOLLAR:
            return "0063";
        case LILY_ERROR_KIND_UNKNOWN_MACRO_DATA_TYPE:
            return "0064";
        case LILY_ERROR_KIND_EXPECTED_MACRO_DATA_TYPE:
            return "0065";
        case LILY_ERROR_KIND_MACRO_EXPECTED_ID:
            return "0066";
        case LILY_ERROR_KIND_MACRO_EXPECTED_DT:
            return "0067";
        case LILY_ERROR_KIND_MACRO_EXPECTED_TK:
            return "0068";
        case LILY_ERROR_KIND_MACRO_EXPECTED_TKS:
            return "0069";
        case LILY_ERROR_KIND_MACRO_EXPECTED_STMT:
            return "0070";
        case LILY_ERROR_KIND_MACRO_EXPECTED_EXPR:
            return "0071";
        case LILY_ERROR_KIND_MACRO_EXPECTED_PATH:
            return "0072";
        case LILY_ERROR_KIND_MACRO_EXPECTED_PATT:
            return "0073";
        case LILY_ERROR_KIND_MACRO_EXPECTED_BLOCK:
            return "0074";
        case LILY_ERROR_KIND_MACRO_DUPLICATE_PARAM:
            return "0075";
        case LILY_ERROR_KIND_MACRO_IDENTIFIER_NOT_FOUND:
            return "0076";
        case LILY_ERROR_KIND_EXPECTED_PATTERN:
            return "0077";
        case LILY_ERROR_KIND_DUPLICATE_CONSTANT:
            return "0078";
        case LILY_ERROR_KIND_DUPLICATE_ERROR:
            return "0079";
        case LILY_ERROR_KIND_DUPLICATE_FUN:
            return "0080";
        case LILY_ERROR_KIND_DUPLICATE_MODULE:
            return "0081";
        case LILY_ERROR_KIND_DUPLICATE_CLASS:
            return "0082";
        case LILY_ERROR_KIND_DUPLICATE_ENUM_OBJECT:
            return "0083";
        case LILY_ERROR_KIND_DUPLICATE_RECORD_OBJECT:
            return "0084";
        case LILY_ERROR_KIND_DUPLICATE_TRAIT:
            return "0085";
        case LILY_ERROR_KIND_DUPLICATE_ALIAS:
            return "0086";
        case LILY_ERROR_KIND_DUPLICATE_ENUM:
            return "0087";
        case LILY_ERROR_KIND_DUPLICATE_RECORD:
            return "0088";
        case LILY_ERROR_KIND_DUPLICATE_PARAM_NAME:
            return "0089";
        case LILY_ERROR_KIND_DUPLICATE_VARIABLE:
            return "0090";
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
