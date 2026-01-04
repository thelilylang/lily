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

#include <core/lily/diagnostic/error.h>

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
        case LILY_ERROR_KIND_BREAK_IS_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "break is not expected in this context";
        case LILY_ERROR_KIND_NEXT_IS_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "next is not expected in this context";
        case LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH:
            return "data type don't match";
        case LILY_ERROR_KIND_PATH_IS_NOT_EXPECTED_AFTER_SYS_IMPORT_FLAG:
            return "the path is not expected after the `sys` import flag";
        case LILY_ERROR_KIND_IDENTIFIER_NOT_FOUND:
            return "the identifier is not found";
        case LILY_ERROR_KIND_VALUE_HAS_BEEN_MOVED:
            return "the value has been moved";
        case LILY_ERROR_KIND_CANNOT_USE_ANY_IN_SAFE_MODE:
            return "you cannot use any in safe mode";
        case LILY_ERROR_KIND_CANNOT_CAST_TO_ANY_IN_SAFE_MODE:
            return "you cannot cast to any in safe mode";
        case LILY_ERROR_KIND_BAD_LITERAL_CAST:
            return "bad literal cast";
        case LILY_ERROR_KIND_UNKNOWN_CAST:
            return "unknown cast";
        case LILY_ERROR_KIND_EXPECTED_MAIN_FUNCTION:
            return "expected main function";
        case LILY_ERROR_KIND_EXPECTED_MUTABLE_VARIABLE:
            return "expected mutable variable";
        case LILY_ERROR_KIND_EXPECTED_BOOLEAN_EXPRESSION:
            return "expected boolean expression";
        case LILY_ERROR_KIND_FUNCTION_IS_NOT_FOUND:
            return "this function is not found";
        case LILY_ERROR_KIND_DUPLICATE_FIELD:
            return "duplicate field";
        case LILY_ERROR_KIND_UNKNOWN_TYPE:
            return "unknown type";
        case LILY_ERROR_KIND_FIELD_IS_NOT_FOUND:
            return "the field is not found";
        case LILY_ERROR_KIND_BAD_SYS_FUNCTION:
            return "bad sys function";
        case LILY_ERROR_KIND_IMPORT_SYS_REQUIRED:
            return "the import of `@sys` is required";
        case LILY_ERROR_KIND_TOO_MANY_ITEMS_IN_MACRO_EXPAND:
            return "too many items in macro expand";
        case LILY_ERROR_KIND_DUPLICATE_VARIANT:
            return "duplicate variant";
        case LILY_ERROR_KIND_IMPORT_BUILTIN_REQUIRED:
            return "the import of `@builtin` is required";
        case LILY_ERROR_KIND_BAD_BUILTIN_FUNCTION:
            return "bad builtin function";
        case LILY_ERROR_KIND_DATA_TYPE_NOT_FOUND:
            return "data type not found";
        case LILY_ERROR_KIND_EXPECTED_DATA_TYPE_IS_NOT_GUARANTEED:
            return "expected data type is not guaranteed";
        case LILY_ERROR_KIND_CALL_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "this call is not expected in this context";
        case LILY_ERROR_KIND_NUMBER_OF_PARAMS_MISMATCHED:
            return "the number of params mismatched";
        case LILY_ERROR_KIND_TOO_MANY_PARAMS:
            return "too many params";
        case LILY_ERROR_KIND_DEFAULT_PARAM_IS_NOT_EXPECTED:
            return "default param is not expected";
        case LILY_ERROR_KIND_THERE_IS_NO_FIELD_IN_TRAIT:
            return "there is no field in trait";
        case LILY_ERROR_KIND_EXPECTED_CUSTOM_DATA_TYPE:
            return "expected custom data type";
        case LILY_ERROR_KIND_EXPECTED_OBJECT_DECL_AS_PARENT:
            return "expected object declaration as parent";
        case LILY_ERROR_KIND_EXPECTED_METHOD_AS_PARENT:
            return "expected method as parent";
        case LILY_ERROR_KIND_THIS_KIND_OF_DATA_TYPE_IS_NOT_EXPECTED:
            return "this kind of data type is not expected in this context";
        case LILY_ERROR_KIND_MAIN_FUNCTION_IS_NOT_CALLABLE:
            return "the main function is not callable";
        case LILY_ERROR_KIND_IMPOSSIBLE_TO_GET_RETURN_DATA_TYPE:
            return "impossible to get a return data type";
        case LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW:
            return "comptime cast overflow";
        case LILY_ERROR_KIND_THIS_DATA_TYPE_CANNOT_BE_DROPPED:
            return "this data type cannot be dropped";
        case LILY_ERROR_KIND_VALUE_HAS_BEEN_DROPPED:
            return "value has been dropped";
        case LILY_ERROR_KIND_THIS_KIND_OF_VALUE_IS_NOT_ALLOWED_TO_BE_DROP:
            return "this kind of value is not allowed to be drop";
        case LILY_ERROR_KIND_THIS_KIND_OF_EXPR_IS_NOT_ALLOWED_TO_BE_DROP:
            return "this kind of expression is not allowed to be drop";
        case LILY_ERROR_KIND_ERROR_DECL_NOT_FOUND:
            return "the error declaration is not found";
        case LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH_WITH_INFER_DATA_TYPE:
            return "the data type doesn't match with the inferred data type";
        case LILY_ERROR_KIND_GENERIC_PARAMS_ARE_NOT_EXPECTED_IN_MAIN_FUNCTION:
            return "the generic params are not expected in the main function";
        case LILY_ERROR_KIND_NO_EXPLICIT_PARAMS_ARE_EXPECTED_IN_MAIN_FUNCTION:
            return "no explicit params are expected in the main function";
        case LILY_ERROR_KIND_OPERATOR_CANNOT_HAVE_COMPILER_DEFINED_DATA_TYPE_AS_PARAMETER:
            return "operator cannot have compiler defined data type as "
                   "parameter";
        case LILY_ERROR_KIND_THIS_RETURN_DATA_TYPE_IS_NOT_EXPECTED_FOR_A_MAIN_FUNCTION:
            return "this return data type is not expected for a main function";
        case LILY_ERROR_KIND_OPERATOR_MUST_HAVE_RETURN_DATA_TYPE:
            return "the operator must have a return data type";
        case LILY_ERROR_KIND_MAIN_FUNCTION_CANNOT_BE_RECURSIVE:
            return "the main function cannot be recursive";
        case LILY_ERROR_KIND_OPERATOR_IS_NOT_VALID:
            return "the operator is not valid";
        case LILY_ERROR_KIND_DUPLICATE_OPERATOR:
            return "the operator is duplicate";
        case LILY_ERROR_KIND_INFINITE_DATA_TYPE:
            return "the data type is infinite";
        case LILY_ERROR_KIND_TUPLES_HAVE_NOT_SAME_SIZE:
            return "the tuples have not the same size";
        case LILY_ERROR_KIND_EXPECTED_FUN_CALL:
            return "expected fun call";
        case LILY_ERROR_KIND_UNEXPECTED_CLOSE:
            return "unexpected close";
        case LILY_ERROR_KIND_SELF_IMPORT:
            return "self import";
        case LILY_ERROR_KIND_RECURSIVE_IMPORT:
            return "recursive import";
        case LILY_ERROR_KIND_EXPECTED_ERROR_DATA_TYPE:
            return "expected error data type";
        case LILY_ERROR_KIND_UNEXPECTED_CALL_EXPR:
            return "unexpected call expression";
        case LILY_ERROR_KIND_UNEXPECTED_WILDCARD:
            return "unexpected wildcard";
        case LILY_ERROR_KIND_UNEXPECTED_PATH:
            return "unexpected path";
        case LILY_ERROR_KIND_EXPECTED_PATH:
            return "expected path";
        case LILY_ERROR_KIND_RESTRICTED_CHARACTER_ON_IDENTIFIER_STRING:
            return "restricted character on identifier string";
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
        case LILY_ERROR_KIND_BREAK_IS_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "0091";
        case LILY_ERROR_KIND_NEXT_IS_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "0092";
        case LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH:
            return "0093";
        case LILY_ERROR_KIND_PATH_IS_NOT_EXPECTED_AFTER_SYS_IMPORT_FLAG:
            return "0094";
        case LILY_ERROR_KIND_IDENTIFIER_NOT_FOUND:
            return "0095";
        case LILY_ERROR_KIND_VALUE_HAS_BEEN_MOVED:
            return "0096";
        case LILY_ERROR_KIND_CANNOT_USE_ANY_IN_SAFE_MODE:
            return "0097";
        case LILY_ERROR_KIND_CANNOT_CAST_TO_ANY_IN_SAFE_MODE:
            return "0098";
        case LILY_ERROR_KIND_BAD_LITERAL_CAST:
            return "0099";
        case LILY_ERROR_KIND_UNKNOWN_CAST:
            return "0100";
        case LILY_ERROR_KIND_EXPECTED_MAIN_FUNCTION:
            return "0101";
        case LILY_ERROR_KIND_EXPECTED_MUTABLE_VARIABLE:
            return "0102";
        case LILY_ERROR_KIND_EXPECTED_BOOLEAN_EXPRESSION:
            return "0103";
        case LILY_ERROR_KIND_FUNCTION_IS_NOT_FOUND:
            return "0104";
        case LILY_ERROR_KIND_DUPLICATE_FIELD:
            return "0105";
        case LILY_ERROR_KIND_UNKNOWN_TYPE:
            return "0106";
        case LILY_ERROR_KIND_FIELD_IS_NOT_FOUND:
            return "0107";
        case LILY_ERROR_KIND_BAD_SYS_FUNCTION:
            return "0108";
        case LILY_ERROR_KIND_IMPORT_SYS_REQUIRED:
            return "0109";
        case LILY_ERROR_KIND_TOO_MANY_ITEMS_IN_MACRO_EXPAND:
            return "0110";
        case LILY_ERROR_KIND_DUPLICATE_VARIANT:
            return "0111";
        case LILY_ERROR_KIND_IMPORT_BUILTIN_REQUIRED:
            return "0112";
        case LILY_ERROR_KIND_BAD_BUILTIN_FUNCTION:
            return "0113";
        case LILY_ERROR_KIND_DATA_TYPE_NOT_FOUND:
            return "0114";
        case LILY_ERROR_KIND_EXPECTED_DATA_TYPE_IS_NOT_GUARANTEED:
            return "0115";
        case LILY_ERROR_KIND_CALL_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "0116";
        case LILY_ERROR_KIND_NUMBER_OF_PARAMS_MISMATCHED:
            return "0117";
        case LILY_ERROR_KIND_TOO_MANY_PARAMS:
            return "0118";
        case LILY_ERROR_KIND_DEFAULT_PARAM_IS_NOT_EXPECTED:
            return "0119";
        case LILY_ERROR_KIND_THERE_IS_NO_FIELD_IN_TRAIT:
            return "0120";
        case LILY_ERROR_KIND_EXPECTED_CUSTOM_DATA_TYPE:
            return "0121";
        case LILY_ERROR_KIND_EXPECTED_OBJECT_DECL_AS_PARENT:
            return "0122";
        case LILY_ERROR_KIND_EXPECTED_METHOD_AS_PARENT:
            return "0123";
        case LILY_ERROR_KIND_THIS_KIND_OF_DATA_TYPE_IS_NOT_EXPECTED:
            return "0124";
        case LILY_ERROR_KIND_MAIN_FUNCTION_IS_NOT_CALLABLE:
            return "0125";
        case LILY_ERROR_KIND_IMPOSSIBLE_TO_GET_RETURN_DATA_TYPE:
            return "0126";
        case LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW:
            return "0127";
        case LILY_ERROR_KIND_THIS_DATA_TYPE_CANNOT_BE_DROPPED:
            return "0128";
        case LILY_ERROR_KIND_VALUE_HAS_BEEN_DROPPED:
            return "0129";
        case LILY_ERROR_KIND_THIS_KIND_OF_VALUE_IS_NOT_ALLOWED_TO_BE_DROP:
            return "0130";
        case LILY_ERROR_KIND_THIS_KIND_OF_EXPR_IS_NOT_ALLOWED_TO_BE_DROP:
            return "0131";
        case LILY_ERROR_KIND_ERROR_DECL_NOT_FOUND:
            return "0132";
        case LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH_WITH_INFER_DATA_TYPE:
            return "0133";
        case LILY_ERROR_KIND_GENERIC_PARAMS_ARE_NOT_EXPECTED_IN_MAIN_FUNCTION:
            return "0134";
        case LILY_ERROR_KIND_NO_EXPLICIT_PARAMS_ARE_EXPECTED_IN_MAIN_FUNCTION:
            return "0135";
        case LILY_ERROR_KIND_OPERATOR_CANNOT_HAVE_COMPILER_DEFINED_DATA_TYPE_AS_PARAMETER:
            return "0136";
        case LILY_ERROR_KIND_THIS_RETURN_DATA_TYPE_IS_NOT_EXPECTED_FOR_A_MAIN_FUNCTION:
            return "0137";
        case LILY_ERROR_KIND_OPERATOR_MUST_HAVE_RETURN_DATA_TYPE:
            return "0138";
        case LILY_ERROR_KIND_MAIN_FUNCTION_CANNOT_BE_RECURSIVE:
            return "0139";
        case LILY_ERROR_KIND_OPERATOR_IS_NOT_VALID:
            return "0140";
        case LILY_ERROR_KIND_DUPLICATE_OPERATOR:
            return "0141";
        case LILY_ERROR_KIND_INFINITE_DATA_TYPE:
            return "0142";
        case LILY_ERROR_KIND_TUPLES_HAVE_NOT_SAME_SIZE:
            return "0143";
        case LILY_ERROR_KIND_EXPECTED_FUN_CALL:
            return "0144";
        case LILY_ERROR_KIND_UNEXPECTED_CLOSE:
            return "0145";
        case LILY_ERROR_KIND_SELF_IMPORT:
            return "0146";
        case LILY_ERROR_KIND_RECURSIVE_IMPORT:
            return "0147";
        case LILY_ERROR_KIND_EXPECTED_ERROR_DATA_TYPE:
            return "0148";
        case LILY_ERROR_KIND_UNEXPECTED_CALL_EXPR:
            return "0149";
        case LILY_ERROR_KIND_UNEXPECTED_WILDCARD:
            return "0150";
        case LILY_ERROR_KIND_UNEXPECTED_PATH:
            return "0151";
        case LILY_ERROR_KIND_EXPECTED_PATH:
            return "0152";
        case LILY_ERROR_KIND_RESTRICTED_CHARACTER_ON_IDENTIFIER_STRING:
            return "0153";
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
