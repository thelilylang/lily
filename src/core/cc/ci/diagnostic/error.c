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
            return "required C89 or later standard";
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
        case CI_ERROR_KIND_EXPECTED_TOKEN:
            return format("expected {s}", self->expected_token);
        case CI_ERROR_KIND_ACCESS_TO_DROPPED_STRUCT_OR_UNION:
            return "not expected to access a dropped struct or union";
        case CI_ERROR_KIND_ACCESS_TO_FREED_POINTER:
            return "not expected to access a freed pointer";
        case CI_ERROR_KIND_ACCESS_TO_NULLABLE_POINTER:
            return "not expected to access a nullable pointer";
        case CI_ERROR_KIND_ACCESS_TO_UNDEFINED_VALUE:
            return "not expected to access an undefined value";
        case CI_ERROR_KIND_ALIGNOF_IN_PREPROCESSOR_CONDITION:
            return "`alignof` is not expected in a preprocessor condition";
        case CI_ERROR_KIND_BAD_INTEGER_LITERAL:
            return "bad integer literal";
        case CI_ERROR_KIND_BIT_NOT_ON_FLOAT:
            return "the `~` operation is not expected for a float";
        case CI_ERROR_KIND_BREAK_OUTSIDE_OF_LOOP_OR_SWITCH:
            return "`break` is not expected outside of a loop or a switch";
        case CI_ERROR_KIND_CALL_ON_NON_FUNCTION:
            return "call made on something that is not a function";
        case CI_ERROR_KIND_CALL_TO_UNKNOWN_FUNCTION:
            return "unknown function, it is impossible to call an unknown "
                   "function";
        case CI_ERROR_KIND_CANNOT_ASSIGN_TO_ARRAY_DATA_TYPE:
            return "cannot assign an expression to an array data type";
        case CI_ERROR_KIND_CANNOT_ASSIGN_TO_CONST_DATA_TYPE:
            return "cannot assign an expression to a constant data type";
        case CI_ERROR_KIND_CANNOT_COMBINE_STORAGE_CLASS_WITH_TYPEDEF:
            return "cannot combine other storage class specifier(s) with "
                   "`typedef`";
        case CI_ERROR_KIND_CANNOT_DEFINE_CI_VA_ARGS:
            return "cannot define `__CI_VA_ARGS__`";
        case CI_ERROR_KIND_CANNOT_INFER_INITIALIZER:
            return "cannot infer the data type of an initializer";
        case CI_ERROR_KIND_CANNOT_INFER_ON_UNKNOWN_IDENTIFIER:
            return "cannot infer on an unknown identifier";
        case CI_ERROR_KIND_CANNOT_INFER_PATH:
            return "cannot infer the data type of this path";
        case CI_ERROR_KIND_CANNOT_REDEFINE_BUILTIN_FUNCTION:
            return "cannot redefine a builtin function";
        case CI_ERROR_KIND_CANNOT_REDEFINE_PREDEFINED_MACRO:
            return "cannot undef or redefine a standard predefined macro "
                   "outside of the predefined file";
        case CI_ERROR_KIND_CANNOT_RESOLVE_CAST_AT_PREPROCESSOR_TIME:
            return "cannot resolve a cast at preprocessor-time";
        case CI_ERROR_KIND_CANNOT_RESOLVE_SIZE_OF_ANY_DATA_TYPE:
            return "cannot resolve the size of an `any` data type";
        case CI_ERROR_KIND_CANNOT_RESOLVE_SIZE_OF_TYPE_INFO:
            return "cannot resolve the size of a type info data type";
        case CI_ERROR_KIND_CANNOT_USE_NON_COMPTIME_VALUE:
            return "cannot use a non-comptime value";
        case CI_ERROR_KIND_CASE_OUTSIDE_OF_SWITCH:
            return "`case` is not expected outside of a switch";
        case CI_ERROR_KIND_CI_YAML_NOT_FOUND:
            return "could not find `CI.yaml` in the current working directory "
                   "or a parent directory";
        case CI_ERROR_KIND_CONTINUE_OUTSIDE_OF_LOOP:
            return "`continue` is not expected outside of a loop";
        case CI_ERROR_KIND_DATA_TYPES_DONT_MATCH:
            return "the data types do not match";
        case CI_ERROR_KIND_DECLARATION_IS_NOT_FOUND:
            return "the declaration is not found";
        case CI_ERROR_KIND_DECLARATION_OUTSIDE_OF_VARIABLE_INITIALIZATION:
            return "cannot declare an array, a struct call or a union call "
                   "outside of a variable initialization";
        case CI_ERROR_KIND_DEFAULT_OUTSIDE_OF_SWITCH:
            return "`default` is not expected outside of a switch";
        case CI_ERROR_KIND_DIRECT_MEMORY_LEAK:
            return "direct memory leak detected";
        case CI_ERROR_KIND_DUPLICATE_DEFINE_NAME:
            return "duplicate `#define` name";
        case CI_ERROR_KIND_DUPLICATE_FIELD:
            return "duplicate field";
        case CI_ERROR_KIND_DUPLICATE_INPUT:
            return "duplicate input";
        case CI_ERROR_KIND_DUPLICATE_INPUT_BINARY:
            return "duplicate input binary";
        case CI_ERROR_KIND_DUPLICATE_INPUT_HEADER:
            return "duplicate input header";
        case CI_ERROR_KIND_DUPLICATE_INPUT_LIBRARY:
            return "duplicate input library";
        case CI_ERROR_KIND_ELIFDEF_AFTER_ELSE:
            return "cannot add `#elifdef` after `#else`";
        case CI_ERROR_KIND_ELIFNDEF_AFTER_ELSE:
            return "cannot add `#elifndef` after `#else`";
        case CI_ERROR_KIND_ELIF_AFTER_ELSE:
            return "cannot add `#elif` after `#else`";
        case CI_ERROR_KIND_ELSE_AFTER_ELSE:
            return "cannot add `#else` after `#else`";
        case CI_ERROR_KIND_ENUM_IS_ALREADY_DEFINED:
            return "this enum is already defined";
        case CI_ERROR_KIND_ENUM_VARIANT_IS_ALREADY_DEFINED:
            return "this enum variant is already defined";
        case CI_ERROR_KIND_EXCESS_ELEMENTS_IN_STRUCT_OR_UNION_INITIALIZER:
            return "excess elements in the struct or union initializer";
        case CI_ERROR_KIND_EXCESS_ELEMENTS_IN_UNION_INITIALIZER:
            return "excess elements in the union initializer";
        case CI_ERROR_KIND_EXPECTED_ARRAY_COMPATIBLE_DATA_TYPE:
            return "expected an array compatible data type";
        case CI_ERROR_KIND_EXPECTED_ARRAY_INITIALIZATION_EXPRESSION:
            return "an initialization expression is required for this array "
                   "data type";
        case CI_ERROR_KIND_EXPECTED_ARRAY_OR_POINTER:
            return "expected an array or a pointer";
        case CI_ERROR_KIND_EXPECTED_ATTRIBUTE_IDENTIFIER:
            return "expected an attribute identifier";
        case CI_ERROR_KIND_EXPECTED_AT_LEAST_EOF:
            return "expected to have at least an EOF token";
        case CI_ERROR_KIND_EXPECTED_AT_LEAST_ONE_GENERIC_PARAM:
            return "expected at least one generic param";
        case CI_ERROR_KIND_EXPECTED_BOOLEAN_COMPATIBLE_DATA_TYPE:
            return "expected a boolean compatible expression data type";
        case CI_ERROR_KIND_EXPECTED_COMPILE_TIME_EXPRESSION:
            return "expected to have an expression resolvable at compile-time";
        case CI_ERROR_KIND_EXPECTED_DATA_TYPE:
            return "expected a data type";
        case CI_ERROR_KIND_EXPECTED_DATA_TYPE_CONTEXT:
            return "expected `!heap`, `!non_null`, `!stack`, `!trace`, "
                   "`!index`, `!realloc`, `!drop`, `!static` or `!free` as "
                   "data type context";
        case CI_ERROR_KIND_EXPECTED_DECLARATION_DEFINITION:
            return "expected to have the definition of the declaration at this "
                   "point";
        case CI_ERROR_KIND_EXPECTED_DIGIT:
            return "expected a digit";
        case CI_ERROR_KIND_EXPECTED_ENDIF:
            return "expected `#endif`";
        case CI_ERROR_KIND_EXPECTED_EXPRESSION:
            return "expected an expression";
        case CI_ERROR_KIND_EXPECTED_EXPRESSION_IN_IF_CONDITION:
            return "expected an expression in the `#if` preprocessor condition";
        case CI_ERROR_KIND_EXPECTED_GENERIC_PARAM:
            return "expected a generic param, to use a generic data type";
        case CI_ERROR_KIND_EXPECTED_IDENTIFIER:
            return "expected an identifier";
        case CI_ERROR_KIND_EXPECTED_IDENTIFIER_AFTER_DATA_TYPE_CONTEXT:
            return "expected an identifier after the data type context, e.g. "
                   "`!<id>`";
        case CI_ERROR_KIND_EXPECTED_IF_CONDITION:
            return "expected an `if` condition";
        case CI_ERROR_KIND_EXPECTED_INDEX_CONTEXT_ON_RETURN_VALUE:
            return "expected to have the `!index` context flag on the return "
                   "value";
        case CI_ERROR_KIND_EXPECTED_INTEGER:
            return "expected an integer";
        case CI_ERROR_KIND_EXPECTED_INTEGER_COMPATIBLE_DATA_TYPE:
            return "expected an integer compatible data type";
        case CI_ERROR_KIND_EXPECTED_INTEGER_DATA_TYPE:
            return "expected an integer data type";
        case CI_ERROR_KIND_EXPECTED_INTEGER_FOR_BIT_OPERATION:
            return "expected an integer compatible data type for a bit "
                   "operation";
        case CI_ERROR_KIND_EXPECTED_INTEGER_FOR_LOGICAL_OPERATION:
            return "expected an integer compatible data type for a logical "
                   "operation";
        case CI_ERROR_KIND_EXPECTED_INTEGER_LITERAL_COMPATIBLE_VALUE:
            return "expected to have an integer literal compatible value";
        case CI_ERROR_KIND_EXPECTED_INTEGER_OR_FLOAT_FOR_ARITHMETIC_OPERATION:
            return "expected an integer or float compatible data type for an "
                   "arithmetic operation";
        case CI_ERROR_KIND_EXPECTED_INTEGER_OR_FLOAT_FOR_COMPARISON_OPERATION:
            return "expected an integer or float compatible data type for a "
                   "comparison operation";
        case CI_ERROR_KIND_EXPECTED_KEY:
            return "expected to have this key";
        case CI_ERROR_KIND_EXPECTED_LITERAL_EXPRESSION:
            return "expected a literal expression (passing a constant or an "
                   "enum variant is not yet supported)";
        case CI_ERROR_KIND_EXPECTED_MACRO_PARAM_AFTER_STRINGIFY:
            return "expected a macro param after `#`";
        case CI_ERROR_KIND_EXPECTED_MAPPING:
            return "expected to have a mapping here";
        case CI_ERROR_KIND_EXPECTED_NAME:
            return "expected to have a name";
        case CI_ERROR_KIND_EXPECTED_NEW_LINE:
            return "expected a new line";
        case CI_ERROR_KIND_EXPECTED_NON_NULL_EXPRESSION:
            return "expected a non-null expression";
        case CI_ERROR_KIND_EXPECTED_ONE_OR_MANY_VARIANTS:
            return "expected one or many variants";
        case CI_ERROR_KIND_EXPECTED_ONLY_GENERIC_DATA_TYPE:
            return "expected only a generic data type for the moment";
        case CI_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION:
            return "expected only one expression";
        case CI_ERROR_KIND_EXPECTED_POINTER_WITH_DEPTH_OF_ONE:
            return "expected a pointer with a depth of one";
        case CI_ERROR_KIND_EXPECTED_POSITIVE_INTEGER_VALUE:
            return "expected to have a positive integer value";
        case CI_ERROR_KIND_EXPECTED_RHS_OF_MERGE:
            return "expected a right parameter to `##`";
        case CI_ERROR_KIND_EXPECTED_SCALAR_VALUE:
            return "expected a scalar value";
        case CI_ERROR_KIND_EXPECTED_SEQUENCE_VALUE:
            return "expected a sequence value";
        case CI_ERROR_KIND_EXPECTED_STRING_LITERAL:
            return "expected a string literal";
        case CI_ERROR_KIND_EXPECTED_STRING_TOKEN:
            return "expected a string token";
        case CI_ERROR_KIND_EXPECTED_STRING_VALUE:
            return "expected a string value";
        case CI_ERROR_KIND_EXPECTED_STRUCT_OR_UNION:
            return "expected a struct or a union";
        case CI_ERROR_KIND_EXPECTED_STRUCT_OR_UNION_DATA_TYPE:
            return "expected a struct or union data type";
        case CI_ERROR_KIND_EXPECTED_TYPE:
            return "expected a type";
        case CI_ERROR_KIND_EXPECTED_VALID_FOR_INIT_CLAUSE:
            return "expected a valid `for` init-clause";
        case CI_ERROR_KIND_EXPECTED_VOID_RETURN_DATA_TYPE:
            return "expected a `void` return data type";
        case CI_ERROR_KIND_EXPRESSION_NOT_RESOLVABLE_AT_PREPROCESSOR_TIME:
            return "this expression is not resolvable at preprocessor-time";
        case CI_ERROR_KIND_FAILED_TO_FETCH_BUILTIN_MACROS:
            return "failed to fetch the builtin macros";
        case CI_ERROR_KIND_FAILED_TO_FETCH_DEFAULT_INCLUDE_PATHS:
            return "failed to fetch the default include paths";
        case CI_ERROR_KIND_FAILED_TO_OPEN_EMBED_PATH:
            return "failed to open the embed path";
        case CI_ERROR_KIND_FIELD_IS_NOT_FOUND:
            return "the field is not found";
        case CI_ERROR_KIND_FIELD_NOT_FOUND:
            return "the field is not found";
        case CI_ERROR_KIND_FLAG_IS_ALREADY_SET:
            return "this flag is already set";
        case CI_ERROR_KIND_FUNCTION_IS_ALREADY_DEFINED:
            return "this function is already defined";
        case CI_ERROR_KIND_GENERIC_PARAMS_ARE_NOT_FOUND:
            return "the generic params are not found";
        case CI_ERROR_KIND_GENERIC_PARAMS_COUNT_MISMATCH:
            return "the number of generic params does not correspond to its "
                   "declaration";
        case CI_ERROR_KIND_GENERIC_PARAMS_IN_VARIABLE_CONTEXT:
            return "generic params are not expected in a variable context";
        case CI_ERROR_KIND_GENERIC_PARAMS_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "generic params are not expected in this context";
        case CI_ERROR_KIND_GENERIC_SELECTION_NO_MATCH:
            return "no association of this generic selection matches the type "
                   "of its controlling expression, and it has no `default`";
        case CI_ERROR_KIND_GENERIC_PARAM_NAME_NOT_FOUND:
            return "the generic param name is not found";
        case CI_ERROR_KIND_GOTO_LABEL_DOES_NOT_EXIST:
            return "the label specified in the `goto` does not exist";
        case CI_ERROR_KIND_IDENTIFIER_IS_NOT_FOUND:
            return "the identifier is not found";
        case CI_ERROR_KIND_INCLUDE_FILE_IS_NOT_FOUND:
            return "the include file is not found";
        case CI_ERROR_KIND_INCOMPATIBLE_DATA_TYPE_CONTEXT:
            return "incompatible data type context";
        case CI_ERROR_KIND_INCOMPLETE_ENUM_TYPE:
            return "the enum type is incomplete";
        case CI_ERROR_KIND_INCOMPLETE_STRUCT_TYPE:
            return "the struct type is incomplete";
        case CI_ERROR_KIND_INCOMPLETE_TYPEDEF_TYPE:
            return "the typedef type is incomplete";
        case CI_ERROR_KIND_INCOMPLETE_UNION_TYPE:
            return "the union type is incomplete";
        case CI_ERROR_KIND_INVALID_HAS_FEATURE_ARGUMENT:
            return "the feature called in `__has_feature` is not valid";
        case CI_ERROR_KIND_INVALID_VALUE:
            return "invalid value";
        case CI_ERROR_KIND_LABEL_IS_ALREADY_DEFINED:
            return "this label is already defined";
        case CI_ERROR_KIND_MACRO_PARAMS_COUNT_MISMATCH:
            return "the number of macro params does not correspond to its "
                   "declaration";
        case CI_ERROR_KIND_MERGE_AT_START:
            return "`##` is not expected at the start of a macro";
        case CI_ERROR_KIND_NAME_IS_ALREADY_DEFINED:
            return "this name is already defined";
        case CI_ERROR_KIND_NAME_IS_NOT_EXPECTED_HERE:
            return "a name is not expected here";
        case CI_ERROR_KIND_NEGATIVE_BIT_FIELD:
            return "a bit field cannot be negative";
        case CI_ERROR_KIND_NO_FIELDS:
            return "there are no fields";
        case CI_ERROR_KIND_OPERATION_EXPECTED_FLOAT_OR_INTEGER:
            return "this operation is not allowed for this data type, expected "
                   "a float or an integer";
        case CI_ERROR_KIND_OPERATION_EXPECTED_INTEGER:
            return "this operation is not allowed for this data type, expected "
                   "an integer";
        case CI_ERROR_KIND_OPERATION_EXPECTED_POINTER:
            return "this operation is not allowed for this data type, expected "
                   "a pointer";
        case CI_ERROR_KIND_OPERATION_IS_NOT_POSSIBLE:
            return "this kind of operation is not possible";
        case CI_ERROR_KIND_OPERATION_ON_NON_INT_CASTABLE_TYPES:
            return "cannot perform this operation with types that are not "
                   "castable to int";
        case CI_ERROR_KIND_PARAMS_COUNT_MISMATCH:
            return "the number of params does not match";
        case CI_ERROR_KIND_POST_DECREMENT_AT_PREPROCESSOR_TIME:
            return "`rhs--` cannot be resolved at preprocessor-time";
        case CI_ERROR_KIND_POST_INCREMENT_AT_PREPROCESSOR_TIME:
            return "`rhs++` cannot be resolved at preprocessor-time";
        case CI_ERROR_KIND_POTENTIAL_DIRECT_MEMORY_LEAK:
            return "potential direct memory leak detected";
        case CI_ERROR_KIND_PREPROCESSOR_IN_MACRO:
            return "preprocessors are not expected in a macro";
        case CI_ERROR_KIND_PRE_DECREMENT_AT_PREPROCESSOR_TIME:
            return "`--rhs` cannot be resolved at preprocessor-time";
        case CI_ERROR_KIND_PRE_INCREMENT_AT_PREPROCESSOR_TIME:
            return "`++rhs` cannot be resolved at preprocessor-time";
        case CI_ERROR_KIND_READ_OF_DROPPED_IDENTIFIER:
            return "cannot read an identifier that has already been dropped";
        case CI_ERROR_KIND_READ_OF_FREED_IDENTIFIER:
            return "cannot read an identifier that has already been freed";
        case CI_ERROR_KIND_READ_OF_UNDEFINED_IDENTIFIER:
            return "cannot read an identifier with an undefined value";
        case CI_ERROR_KIND_SINGLE_LINE_COMMENT_BEFORE_C99:
            return "a single line comment is not available before C99";
        case CI_ERROR_KIND_SIZEOF_IN_PREPROCESSOR_CONDITION:
            return "`sizeof` is not expected in a preprocessor condition";
        case CI_ERROR_KIND_STRINGIFY_OUTSIDE_OF_MACRO:
            return "`#` is not expected outside of a macro";
        case CI_ERROR_KIND_STRUCT_IS_ALREADY_DEFINED:
            return "this struct is already defined";
        case CI_ERROR_KIND_STRUCT_IS_NOT_FOUND:
            return "the struct is not found";
        case CI_ERROR_KIND_STRUCT_OR_UNION_IS_NOT_FOUND:
            return "the struct or union is not found";
        case CI_ERROR_KIND_STRUCT_TYPEDEF_OR_UNION_NOT_FOUND:
            return "the struct, typedef or union is not found";
        case CI_ERROR_KIND_TOO_MANY_DOTS_IN_FLOAT_LITERAL:
            return "a float literal cannot contain more than one `.`";
        case CI_ERROR_KIND_TYPEDEF_IS_ALREADY_DEFINED:
            return "this typedef is already defined";
        case CI_ERROR_KIND_UNEXPECTED_BODY_WITH_TYPEDEF:
            return "not expected to have a body when `typedef` is passed";
        case CI_ERROR_KIND_UNEXPECTED_CLOSING_DELIMITER:
            return "`)`, `}` or `]` is not expected here";
        case CI_ERROR_KIND_UNEXPECTED_COMMA_AFTER_VARIADIC_PARAM:
            return "the last param is `...`, so `,` is not expected";
        case CI_ERROR_KIND_UNEXPECTED_DATA_TYPE_WITH_INITIALIZER:
            return "this data type is not expected with an initializer";
        case CI_ERROR_KIND_UNEXPECTED_ELIF:
            return "the `#elif` preprocessor is not expected here";
        case CI_ERROR_KIND_UNEXPECTED_ELIFDEF:
            return "the `#elifdef` preprocessor is not expected here";
        case CI_ERROR_KIND_UNEXPECTED_ELIFNDEF:
            return "the `#elifndef` preprocessor is not expected here";
        case CI_ERROR_KIND_UNEXPECTED_ELSE:
            return "the `#else` preprocessor is not expected here";
        case CI_ERROR_KIND_UNEXPECTED_EOF:
            return "unexpected EOF";
        case CI_ERROR_KIND_UNEXPECTED_ERROR_IN_IF_CONDITION:
            return "unexpected error in the `#if` preprocessor condition";
        case CI_ERROR_KIND_UNEXPECTED_EXPRESSION_WITH_TYPEDEF:
            return "an expression is not expected with the `typedef` storage "
                   "class";
        case CI_ERROR_KIND_UNEXPECTED_FLOAT_SUFFIX:
            return "these suffixes are not expected for a floating point";
        case CI_ERROR_KIND_UNEXPECTED_GENERIC_PARAMS:
            return "not expected to have generic params";
        case CI_ERROR_KIND_UNEXPECTED_INITIALIZER_AT_PREPROCESSOR_TIME:
            return "an initializer is not expected at preprocessor-time";
        case CI_ERROR_KIND_UNEXPECTED_INTEGER_SUFFIX:
            return "these suffixes are not expected for an integer";
        case CI_ERROR_KIND_UNEXPECTED_LHS_OF_MERGE:
            return "this token is not expected as the lhs of `##`";
        case CI_ERROR_KIND_UNEXPECTED_OPERATION_ON_STRING:
            return "this kind of operation is not expected for a string";
        case CI_ERROR_KIND_UNEXPECTED_PATH_IN_ARRAY_INITIALIZATION:
            return "a path is not expected for the initialization of an array";
        case CI_ERROR_KIND_UNEXPECTED_RHS_OF_MERGE:
            return "this token is not expected as the rhs of `##`";
        case CI_ERROR_KIND_UNEXPECTED_STRING_AT_PREPROCESSOR_TIME:
            return "not expected to have a string as result at "
                   "preprocessor-time";
        case CI_ERROR_KIND_UNEXPECTED_TOKEN:
            return "unexpected token";
        case CI_ERROR_KIND_UNION_DECLARATION_IS_NOT_FOUND:
            return "the union declaration is not found";
        case CI_ERROR_KIND_UNION_IS_ALREADY_DEFINED:
            return "this union is already defined";
        case CI_ERROR_KIND_UNKNOWN_FILE_EXTENSION:
            return "unknown extension, expected `.ci`, `.c`, `.hci` or `.h`";
        case CI_ERROR_KIND_UNKNOWN_STANDARD_ATTRIBUTE:
            return "unknown standard attribute";
        case CI_ERROR_KIND_UNSURE_OPERATION_AT_COMPILE_TIME:
            return "this operation is unsure at compile-time";
        case CI_ERROR_KIND_VARIABLE_DECLARATION_IN_INIT_CLAUSE_BEFORE_C99:
            return "a variable declaration in a `for` init-clause is not "
                   "available before C99";
        case CI_ERROR_KIND_VARIABLE_DECLARATION_IN_LABEL:
            return "a variable declaration is not expected in a label";
        case CI_ERROR_KIND_VARIABLE_IS_ALREADY_DEFINED:
            return "this variable is already defined";
        case CI_ERROR_KIND_VA_ARGS_IN_NON_VARIADIC_MACRO:
            return "`__VA_ARGS__` cannot be used in a macro which has no "
                   "variadic param";
        case CI_ERROR_KIND_PREPROCESSOR_ERROR:
            return self->preprocessor_error;
        case CI_ERROR_KIND_STATIC_ASSERT_FAILED:
            return self->static_assert_failed ? self->static_assert_failed
                                              : "static assertion failed";
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
        case CI_ERROR_KIND_UNCLOSED_CHAR_LITERAL:
            return "0013";
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
        case CI_ERROR_KIND_EXPECTED_TOKEN:
            return "0014";
        case CI_ERROR_KIND_ACCESS_TO_DROPPED_STRUCT_OR_UNION:
            return "0015";
        case CI_ERROR_KIND_ACCESS_TO_FREED_POINTER:
            return "0016";
        case CI_ERROR_KIND_ACCESS_TO_NULLABLE_POINTER:
            return "0017";
        case CI_ERROR_KIND_ACCESS_TO_UNDEFINED_VALUE:
            return "0018";
        case CI_ERROR_KIND_ALIGNOF_IN_PREPROCESSOR_CONDITION:
            return "0019";
        case CI_ERROR_KIND_BAD_INTEGER_LITERAL:
            return "0020";
        case CI_ERROR_KIND_BIT_NOT_ON_FLOAT:
            return "0021";
        case CI_ERROR_KIND_BREAK_OUTSIDE_OF_LOOP_OR_SWITCH:
            return "0022";
        case CI_ERROR_KIND_CALL_TO_UNKNOWN_FUNCTION:
            return "0023";
        case CI_ERROR_KIND_CANNOT_ASSIGN_TO_ARRAY_DATA_TYPE:
            return "0024";
        case CI_ERROR_KIND_CANNOT_COMBINE_STORAGE_CLASS_WITH_TYPEDEF:
            return "0025";
        case CI_ERROR_KIND_CANNOT_DEFINE_CI_VA_ARGS:
            return "0026";
        case CI_ERROR_KIND_CANNOT_INFER_INITIALIZER:
            return "0027";
        case CI_ERROR_KIND_CANNOT_INFER_ON_UNKNOWN_IDENTIFIER:
            return "0028";
        case CI_ERROR_KIND_CANNOT_INFER_PATH:
            return "0029";
        case CI_ERROR_KIND_CANNOT_REDEFINE_BUILTIN_FUNCTION:
            return "0030";
        case CI_ERROR_KIND_CANNOT_REDEFINE_PREDEFINED_MACRO:
            return "0031";
        case CI_ERROR_KIND_CANNOT_RESOLVE_CAST_AT_PREPROCESSOR_TIME:
            return "0032";
        case CI_ERROR_KIND_CANNOT_RESOLVE_SIZE_OF_ANY_DATA_TYPE:
            return "0033";
        case CI_ERROR_KIND_CANNOT_RESOLVE_SIZE_OF_TYPE_INFO:
            return "0034";
        case CI_ERROR_KIND_CANNOT_USE_NON_COMPTIME_VALUE:
            return "0035";
        case CI_ERROR_KIND_CASE_OUTSIDE_OF_SWITCH:
            return "0036";
        case CI_ERROR_KIND_CI_YAML_NOT_FOUND:
            return "0037";
        case CI_ERROR_KIND_CONTINUE_OUTSIDE_OF_LOOP:
            return "0038";
        case CI_ERROR_KIND_DATA_TYPES_DONT_MATCH:
            return "0039";
        case CI_ERROR_KIND_DECLARATION_IS_NOT_FOUND:
            return "0040";
        case CI_ERROR_KIND_DECLARATION_OUTSIDE_OF_VARIABLE_INITIALIZATION:
            return "0041";
        case CI_ERROR_KIND_DEFAULT_OUTSIDE_OF_SWITCH:
            return "0042";
        case CI_ERROR_KIND_DIRECT_MEMORY_LEAK:
            return "0043";
        case CI_ERROR_KIND_DUPLICATE_DEFINE_NAME:
            return "0044";
        case CI_ERROR_KIND_DUPLICATE_FIELD:
            return "0045";
        case CI_ERROR_KIND_DUPLICATE_INPUT:
            return "0046";
        case CI_ERROR_KIND_DUPLICATE_INPUT_BINARY:
            return "0047";
        case CI_ERROR_KIND_DUPLICATE_INPUT_HEADER:
            return "0048";
        case CI_ERROR_KIND_DUPLICATE_INPUT_LIBRARY:
            return "0049";
        case CI_ERROR_KIND_ELIFDEF_AFTER_ELSE:
            return "0050";
        case CI_ERROR_KIND_ELIFNDEF_AFTER_ELSE:
            return "0051";
        case CI_ERROR_KIND_ELIF_AFTER_ELSE:
            return "0052";
        case CI_ERROR_KIND_ELSE_AFTER_ELSE:
            return "0053";
        case CI_ERROR_KIND_ENUM_IS_ALREADY_DEFINED:
            return "0054";
        case CI_ERROR_KIND_ENUM_VARIANT_IS_ALREADY_DEFINED:
            return "0055";
        case CI_ERROR_KIND_EXCESS_ELEMENTS_IN_STRUCT_OR_UNION_INITIALIZER:
            return "0056";
        case CI_ERROR_KIND_EXCESS_ELEMENTS_IN_UNION_INITIALIZER:
            return "0057";
        case CI_ERROR_KIND_EXPECTED_ARRAY_COMPATIBLE_DATA_TYPE:
            return "0058";
        case CI_ERROR_KIND_EXPECTED_ARRAY_INITIALIZATION_EXPRESSION:
            return "0059";
        case CI_ERROR_KIND_EXPECTED_ARRAY_OR_POINTER:
            return "0060";
        case CI_ERROR_KIND_EXPECTED_ATTRIBUTE_IDENTIFIER:
            return "0061";
        case CI_ERROR_KIND_EXPECTED_AT_LEAST_EOF:
            return "0062";
        case CI_ERROR_KIND_EXPECTED_AT_LEAST_ONE_GENERIC_PARAM:
            return "0063";
        case CI_ERROR_KIND_EXPECTED_BOOLEAN_COMPATIBLE_DATA_TYPE:
            return "0064";
        case CI_ERROR_KIND_EXPECTED_COMPILE_TIME_EXPRESSION:
            return "0065";
        case CI_ERROR_KIND_EXPECTED_DATA_TYPE:
            return "0066";
        case CI_ERROR_KIND_EXPECTED_DATA_TYPE_CONTEXT:
            return "0067";
        case CI_ERROR_KIND_EXPECTED_DECLARATION_DEFINITION:
            return "0068";
        case CI_ERROR_KIND_EXPECTED_DIGIT:
            return "0069";
        case CI_ERROR_KIND_EXPECTED_ENDIF:
            return "0070";
        case CI_ERROR_KIND_EXPECTED_EXPRESSION:
            return "0071";
        case CI_ERROR_KIND_EXPECTED_EXPRESSION_IN_IF_CONDITION:
            return "0072";
        case CI_ERROR_KIND_EXPECTED_GENERIC_PARAM:
            return "0073";
        case CI_ERROR_KIND_EXPECTED_IDENTIFIER:
            return "0074";
        case CI_ERROR_KIND_EXPECTED_IDENTIFIER_AFTER_DATA_TYPE_CONTEXT:
            return "0075";
        case CI_ERROR_KIND_EXPECTED_IF_CONDITION:
            return "0076";
        case CI_ERROR_KIND_EXPECTED_INDEX_CONTEXT_ON_RETURN_VALUE:
            return "0077";
        case CI_ERROR_KIND_EXPECTED_INTEGER:
            return "0078";
        case CI_ERROR_KIND_EXPECTED_INTEGER_COMPATIBLE_DATA_TYPE:
            return "0079";
        case CI_ERROR_KIND_EXPECTED_INTEGER_DATA_TYPE:
            return "0080";
        case CI_ERROR_KIND_EXPECTED_INTEGER_FOR_BIT_OPERATION:
            return "0081";
        case CI_ERROR_KIND_EXPECTED_INTEGER_FOR_LOGICAL_OPERATION:
            return "0082";
        case CI_ERROR_KIND_EXPECTED_INTEGER_LITERAL_COMPATIBLE_VALUE:
            return "0083";
        case CI_ERROR_KIND_EXPECTED_INTEGER_OR_FLOAT_FOR_ARITHMETIC_OPERATION:
            return "0084";
        case CI_ERROR_KIND_EXPECTED_INTEGER_OR_FLOAT_FOR_COMPARISON_OPERATION:
            return "0085";
        case CI_ERROR_KIND_EXPECTED_KEY:
            return "0086";
        case CI_ERROR_KIND_EXPECTED_LITERAL_EXPRESSION:
            return "0087";
        case CI_ERROR_KIND_EXPECTED_MACRO_PARAM_AFTER_STRINGIFY:
            return "0088";
        case CI_ERROR_KIND_EXPECTED_MAPPING:
            return "0089";
        case CI_ERROR_KIND_EXPECTED_NAME:
            return "0090";
        case CI_ERROR_KIND_EXPECTED_NEW_LINE:
            return "0091";
        case CI_ERROR_KIND_EXPECTED_NON_NULL_EXPRESSION:
            return "0092";
        case CI_ERROR_KIND_EXPECTED_ONE_OR_MANY_VARIANTS:
            return "0093";
        case CI_ERROR_KIND_EXPECTED_ONLY_GENERIC_DATA_TYPE:
            return "0094";
        case CI_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION:
            return "0095";
        case CI_ERROR_KIND_EXPECTED_POINTER_WITH_DEPTH_OF_ONE:
            return "0096";
        case CI_ERROR_KIND_EXPECTED_POSITIVE_INTEGER_VALUE:
            return "0097";
        case CI_ERROR_KIND_EXPECTED_RHS_OF_MERGE:
            return "0098";
        case CI_ERROR_KIND_EXPECTED_SCALAR_VALUE:
            return "0099";
        case CI_ERROR_KIND_EXPECTED_SEQUENCE_VALUE:
            return "0100";
        case CI_ERROR_KIND_EXPECTED_STRING_LITERAL:
            return "0101";
        case CI_ERROR_KIND_EXPECTED_STRING_TOKEN:
            return "0102";
        case CI_ERROR_KIND_EXPECTED_STRING_VALUE:
            return "0103";
        case CI_ERROR_KIND_EXPECTED_STRUCT_OR_UNION:
            return "0104";
        case CI_ERROR_KIND_EXPECTED_STRUCT_OR_UNION_DATA_TYPE:
            return "0105";
        case CI_ERROR_KIND_EXPECTED_TYPE:
            return "0106";
        case CI_ERROR_KIND_EXPECTED_VALID_FOR_INIT_CLAUSE:
            return "0107";
        case CI_ERROR_KIND_EXPECTED_VOID_RETURN_DATA_TYPE:
            return "0108";
        case CI_ERROR_KIND_EXPRESSION_NOT_RESOLVABLE_AT_PREPROCESSOR_TIME:
            return "0109";
        case CI_ERROR_KIND_FAILED_TO_FETCH_BUILTIN_MACROS:
            return "0110";
        case CI_ERROR_KIND_FAILED_TO_FETCH_DEFAULT_INCLUDE_PATHS:
            return "0111";
        case CI_ERROR_KIND_FAILED_TO_OPEN_EMBED_PATH:
            return "0112";
        case CI_ERROR_KIND_FIELD_IS_NOT_FOUND:
            return "0113";
        case CI_ERROR_KIND_FIELD_NOT_FOUND:
            return "0114";
        case CI_ERROR_KIND_FLAG_IS_ALREADY_SET:
            return "0115";
        case CI_ERROR_KIND_FUNCTION_IS_ALREADY_DEFINED:
            return "0116";
        case CI_ERROR_KIND_GENERIC_PARAMS_ARE_NOT_FOUND:
            return "0117";
        case CI_ERROR_KIND_GENERIC_PARAMS_IN_VARIABLE_CONTEXT:
            return "0118";
        case CI_ERROR_KIND_GENERIC_PARAMS_NOT_EXPECTED_IN_THIS_CONTEXT:
            return "0119";
        case CI_ERROR_KIND_GENERIC_PARAM_NAME_NOT_FOUND:
            return "0120";
        case CI_ERROR_KIND_GOTO_LABEL_DOES_NOT_EXIST:
            return "0121";
        case CI_ERROR_KIND_IDENTIFIER_IS_NOT_FOUND:
            return "0122";
        case CI_ERROR_KIND_INCLUDE_FILE_IS_NOT_FOUND:
            return "0123";
        case CI_ERROR_KIND_INCOMPATIBLE_DATA_TYPE_CONTEXT:
            return "0124";
        case CI_ERROR_KIND_INCOMPLETE_ENUM_TYPE:
            return "0125";
        case CI_ERROR_KIND_INCOMPLETE_STRUCT_TYPE:
            return "0126";
        case CI_ERROR_KIND_INCOMPLETE_TYPEDEF_TYPE:
            return "0127";
        case CI_ERROR_KIND_INCOMPLETE_UNION_TYPE:
            return "0128";
        case CI_ERROR_KIND_INVALID_HAS_FEATURE_ARGUMENT:
            return "0129";
        case CI_ERROR_KIND_INVALID_VALUE:
            return "0130";
        case CI_ERROR_KIND_LABEL_IS_ALREADY_DEFINED:
            return "0131";
        case CI_ERROR_KIND_MACRO_PARAMS_COUNT_MISMATCH:
            return "0132";
        case CI_ERROR_KIND_MERGE_AT_START:
            return "0133";
        case CI_ERROR_KIND_NAME_IS_ALREADY_DEFINED:
            return "0134";
        case CI_ERROR_KIND_NAME_IS_NOT_EXPECTED_HERE:
            return "0135";
        case CI_ERROR_KIND_NEGATIVE_BIT_FIELD:
            return "0136";
        case CI_ERROR_KIND_NO_FIELDS:
            return "0137";
        case CI_ERROR_KIND_OPERATION_EXPECTED_FLOAT_OR_INTEGER:
            return "0138";
        case CI_ERROR_KIND_OPERATION_EXPECTED_INTEGER:
            return "0139";
        case CI_ERROR_KIND_OPERATION_EXPECTED_POINTER:
            return "0140";
        case CI_ERROR_KIND_OPERATION_IS_NOT_POSSIBLE:
            return "0141";
        case CI_ERROR_KIND_OPERATION_ON_NON_INT_CASTABLE_TYPES:
            return "0142";
        case CI_ERROR_KIND_PARAMS_COUNT_MISMATCH:
            return "0143";
        case CI_ERROR_KIND_POST_DECREMENT_AT_PREPROCESSOR_TIME:
            return "0144";
        case CI_ERROR_KIND_POST_INCREMENT_AT_PREPROCESSOR_TIME:
            return "0145";
        case CI_ERROR_KIND_POTENTIAL_DIRECT_MEMORY_LEAK:
            return "0146";
        case CI_ERROR_KIND_PREPROCESSOR_IN_MACRO:
            return "0147";
        case CI_ERROR_KIND_PRE_DECREMENT_AT_PREPROCESSOR_TIME:
            return "0148";
        case CI_ERROR_KIND_PRE_INCREMENT_AT_PREPROCESSOR_TIME:
            return "0149";
        case CI_ERROR_KIND_READ_OF_DROPPED_IDENTIFIER:
            return "0150";
        case CI_ERROR_KIND_READ_OF_FREED_IDENTIFIER:
            return "0151";
        case CI_ERROR_KIND_READ_OF_UNDEFINED_IDENTIFIER:
            return "0152";
        case CI_ERROR_KIND_SINGLE_LINE_COMMENT_BEFORE_C99:
            return "0153";
        case CI_ERROR_KIND_SIZEOF_IN_PREPROCESSOR_CONDITION:
            return "0154";
        case CI_ERROR_KIND_STRINGIFY_OUTSIDE_OF_MACRO:
            return "0155";
        case CI_ERROR_KIND_STRUCT_IS_ALREADY_DEFINED:
            return "0156";
        case CI_ERROR_KIND_STRUCT_IS_NOT_FOUND:
            return "0157";
        case CI_ERROR_KIND_STRUCT_OR_UNION_IS_NOT_FOUND:
            return "0158";
        case CI_ERROR_KIND_STRUCT_TYPEDEF_OR_UNION_NOT_FOUND:
            return "0159";
        case CI_ERROR_KIND_TOO_MANY_DOTS_IN_FLOAT_LITERAL:
            return "0160";
        case CI_ERROR_KIND_TYPEDEF_IS_ALREADY_DEFINED:
            return "0161";
        case CI_ERROR_KIND_UNEXPECTED_BODY_WITH_TYPEDEF:
            return "0162";
        case CI_ERROR_KIND_UNEXPECTED_CLOSING_DELIMITER:
            return "0163";
        case CI_ERROR_KIND_UNEXPECTED_COMMA_AFTER_VARIADIC_PARAM:
            return "0164";
        case CI_ERROR_KIND_UNEXPECTED_DATA_TYPE_WITH_INITIALIZER:
            return "0165";
        case CI_ERROR_KIND_UNEXPECTED_ELIF:
            return "0166";
        case CI_ERROR_KIND_UNEXPECTED_ELIFDEF:
            return "0167";
        case CI_ERROR_KIND_UNEXPECTED_ELIFNDEF:
            return "0168";
        case CI_ERROR_KIND_UNEXPECTED_ELSE:
            return "0169";
        case CI_ERROR_KIND_UNEXPECTED_EOF:
            return "0170";
        case CI_ERROR_KIND_UNEXPECTED_ERROR_IN_IF_CONDITION:
            return "0171";
        case CI_ERROR_KIND_UNEXPECTED_EXPRESSION_WITH_TYPEDEF:
            return "0172";
        case CI_ERROR_KIND_UNEXPECTED_FLOAT_SUFFIX:
            return "0173";
        case CI_ERROR_KIND_UNEXPECTED_GENERIC_PARAMS:
            return "0174";
        case CI_ERROR_KIND_UNEXPECTED_INITIALIZER_AT_PREPROCESSOR_TIME:
            return "0175";
        case CI_ERROR_KIND_UNEXPECTED_INTEGER_SUFFIX:
            return "0176";
        case CI_ERROR_KIND_UNEXPECTED_LHS_OF_MERGE:
            return "0177";
        case CI_ERROR_KIND_UNEXPECTED_OPERATION_ON_STRING:
            return "0178";
        case CI_ERROR_KIND_UNEXPECTED_PATH_IN_ARRAY_INITIALIZATION:
            return "0179";
        case CI_ERROR_KIND_UNEXPECTED_RHS_OF_MERGE:
            return "0180";
        case CI_ERROR_KIND_UNEXPECTED_STRING_AT_PREPROCESSOR_TIME:
            return "0181";
        case CI_ERROR_KIND_UNEXPECTED_TOKEN:
            return "0182";
        case CI_ERROR_KIND_UNION_DECLARATION_IS_NOT_FOUND:
            return "0183";
        case CI_ERROR_KIND_UNION_IS_ALREADY_DEFINED:
            return "0184";
        case CI_ERROR_KIND_UNKNOWN_FILE_EXTENSION:
            return "0185";
        case CI_ERROR_KIND_UNKNOWN_STANDARD_ATTRIBUTE:
            return "0186";
        case CI_ERROR_KIND_UNSURE_OPERATION_AT_COMPILE_TIME:
            return "0187";
        case CI_ERROR_KIND_VARIABLE_DECLARATION_IN_INIT_CLAUSE_BEFORE_C99:
            return "0188";
        case CI_ERROR_KIND_VARIABLE_DECLARATION_IN_LABEL:
            return "0189";
        case CI_ERROR_KIND_VARIABLE_IS_ALREADY_DEFINED:
            return "0190";
        case CI_ERROR_KIND_VA_ARGS_IN_NON_VARIADIC_MACRO:
            return "0191";
        case CI_ERROR_KIND_GENERIC_PARAMS_COUNT_MISMATCH:
            return "0192";
        case CI_ERROR_KIND_STATIC_ASSERT_FAILED:
            return "0193";
        case CI_ERROR_KIND_GENERIC_SELECTION_NO_MATCH:
            return "0194";
        case CI_ERROR_KIND_CALL_ON_NON_FUNCTION:
            return "0195";
        case CI_ERROR_KIND_CANNOT_ASSIGN_TO_CONST_DATA_TYPE:
            return "0196";
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
