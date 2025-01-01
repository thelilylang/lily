/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#include <base/macros.h>

#include <core/lily/parser/ast/expr/literal.h>

#ifdef ENV_DEBUG

#include <base/alloc.h>
#include <base/format.h>

#include <stdio.h>
#include <stdlib.h>

char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprLiteralKind,
               enum LilyAstExprLiteralKind self)
{
    switch (self) {
        case LILY_AST_EXPR_LITERAL_KIND_BOOL:
            return "LILY_AST_EXPR_LITERAL_KIND_BOOL";
        case LILY_AST_EXPR_LITERAL_KIND_BYTE:
            return "LILY_AST_EXPR_LITERAL_KIND_BYTE";
        case LILY_AST_EXPR_LITERAL_KIND_BYTES:
            return "LILY_AST_EXPR_LITERAL_KIND_BYTES";
        case LILY_AST_EXPR_LITERAL_KIND_CHAR:
            return "LILY_AST_EXPR_LITERAL_KIND_CHAR";
        case LILY_AST_EXPR_LITERAL_KIND_CSTR:
            return "LILY_AST_EXPR_LITERAL_KIND_CSTR";
        case LILY_AST_EXPR_LITERAL_KIND_FLOAT32:
            return "LILY_AST_EXPR_LITERAL_KIND_FLOAT32";
        case LILY_AST_EXPR_LITERAL_KIND_FLOAT64:
            return "LILY_AST_EXPR_LITERAL_KIND_FLOAT64";
        case LILY_AST_EXPR_LITERAL_KIND_INT32:
            return "LILY_AST_EXPR_LITERAL_KIND_INT32";
        case LILY_AST_EXPR_LITERAL_KIND_INT64:
            return "LILY_AST_EXPR_LITERAL_KIND_INT64";
        case LILY_AST_EXPR_LITERAL_KIND_NIL:
            return "LILY_AST_EXPR_LITERAL_KIND_NIL";
        case LILY_AST_EXPR_LITERAL_KIND_NONE:
            return "LILY_AST_EXPR_LITERAL_KIND_NONE";
        case LILY_AST_EXPR_LITERAL_KIND_STR:
            return "LILY_AST_EXPR_LITERAL_KIND_STR";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT32:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT32";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT64:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT64";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT8:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT8";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT16:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT16";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT32:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT32";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT64:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT64";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_ISIZE:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_ISIZE";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT8:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT8";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT16:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT16";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT32:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT32";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT64:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT64";
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_USIZE:
            return "LILY_AST_EXPR_LITERAL_KIND_SUFFIX_USIZE";
        case LILY_AST_EXPR_LITERAL_KIND_UINT32:
            return "LILY_AST_EXPR_LITERAL_KIND_UINT32";
        case LILY_AST_EXPR_LITERAL_KIND_UINT64:
            return "LILY_AST_EXPR_LITERAL_KIND_UINT64";
        case LILY_AST_EXPR_LITERAL_KIND_UNDEF:
            return "LILY_AST_EXPR_LITERAL_KIND_UNDEF";
        case LILY_AST_EXPR_LITERAL_KIND_UNIT:
            return "LILY_AST_EXPR_LITERAL_KIND_UNIT";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyAstExprLiteral, const LilyAstExprLiteral *self)
{
    String *res =
      format__String("LilyAstExprLiteral{{ kind = {s}, ",
                     to_string__Debug__LilyAstExprLiteralKind(self->kind));

    switch (self->kind) {
        case LILY_AST_EXPR_LITERAL_KIND_BOOL: {
            char *s = format("bool = {b} }", self->bool_);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_BYTE: {
            char *s = format("byte = {d} }", self->byte);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_BYTES: {
            char *s = format("bytes = b\"{s}\" }", (char *)self->bytes);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_CHAR: {
            char *s = format("char = '{c}' }", self->char_);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_CSTR: {
            char *s = format("cstr = c\"{s}\" }", self->cstr);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_FLOAT32: {
            char *s = format("float32 = {f} }", self->float32);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_FLOAT64: {
            char *s = format("float64 = {f} }", self->float64);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_INT32: {
            char *s = format("int32 = {d} }", self->int32);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_INT64: {
            char *s = format("int64 = {d} }", self->int64);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_NIL:
        case LILY_AST_EXPR_LITERAL_KIND_NONE:
        case LILY_AST_EXPR_LITERAL_KIND_UNDEF:
        case LILY_AST_EXPR_LITERAL_KIND_UNIT:
            break;
        case LILY_AST_EXPR_LITERAL_KIND_STR: {
            char *s = format("str = {S} }", self->str);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT32: {
            char *s = format("suffix_float32 = {f} }", self->suffix_float32);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT64: {
            char *s = format("suffix_float64 = {f} }", self->suffix_float64);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT8: {
            char *s = format("suffix_int8 = {d} }", self->suffix_int8);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT16: {
            char *s = format("suffix_int16 = {d} }", self->suffix_int16);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT32: {
            char *s = format("suffix_int32 = {d} }", self->suffix_int32);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT64: {
            char *s = format("suffix_int64 = {d} }", self->suffix_int64);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_ISIZE: {
            char *s = format("suffix_isize = {d} }", self->suffix_isize);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT8: {
            char *s = format("suffix_uint8 = {d} }", self->suffix_uint8);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT16: {
            char *s = format("suffix_uint16 = {d} }", self->suffix_uint16);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT32: {
            char *s = format("suffix_uint32 = {d} }", self->suffix_uint32);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT64: {
            char *s = format("suffix_uint64 = {d} }", self->suffix_uint64);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_USIZE: {
            char *s = format("suffix_usize = {d} }", self->suffix_usize);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_UINT32: {
            char *s = format("uint32 = {d} }", self->uint32);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_LITERAL_KIND_UINT64: {
            char *s = format("uint64 = {d} }", self->uint64);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif
