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

#include <core/lily/checked/expr.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_ARRAY).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, array, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_BINARY).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, binary, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_CALL).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, call, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_CAST).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, cast, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_GROUPING).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, grouping, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_LAMBDA).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, lambda, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_LIST).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, list, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_LITERAL).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, literal, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, tuple, LilyCheckedExpr *self);

// Free LilyCheckedExpr type (LILY_AST_EXPR_KIND_UNARY).
static VARIANT_DESTRUCTOR(LilyCheckedExpr, unary, LilyCheckedExpr *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprKind, enum LilyCheckedExprKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_KIND_ARRAY:
            return "LILY_CHECKED_EXPR_KIND_ARRAY";
        case LILY_CHECKED_EXPR_KIND_BINARY:
            return "LILY_CHECKED_EXPR_KIND_BINARY";
        case LILY_CHECKED_EXPR_KIND_CALL:
            return "LILY_CHECKED_EXPR_KIND_CALL";
        case LILY_CHECKED_EXPR_KIND_CAST:
            return "LILY_CHECKED_EXPR_KIND_CAST";
        case LILY_CHECKED_EXPR_KIND_LAMBDA:
            return "LILY_CHECKED_EXPR_KIND_LAMBDA";
        case LILY_CHECKED_EXPR_KIND_LIST:
            return "LILY_CHECKED_EXPR_KIND_LIST";
        case LILY_CHECKED_EXPR_KIND_LITERAL:
            return "LILY_CHECKED_EXPR_KIND_LITERAL";
        case LILY_CHECKED_EXPR_KIND_SELF:
            return "LILY_CHECKED_EXPR_KIND_SELF";
        case LILY_CHECKED_EXPR_KIND_TUPLE:
            return "LILY_CHECKED_EXPR_KIND_TUPLE";
        case LILY_CHECKED_EXPR_KIND_UNARY:
            return "LILY_CHECKED_EXPR_KIND_UNARY";
        case LILY_CHECKED_EXPR_KIND_GROUPING:
            return "LILY_CHECKED_EXPR_KIND_GROUPING";
        case LILY_CHECKED_EXPR_KIND_UNKNOWN:
            return "LILY_CHECKED_EXPR_KIND_UNKNOWN";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    array,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprArray array)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_ARRAY;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    binary,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprBinary binary)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_BINARY;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->binary = binary;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    call,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprCall call)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_CALL;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->call = call;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    cast,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprCast cast)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_CAST;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->cast = cast;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    grouping,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExpr *grouping)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_GROUPING;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->grouping = grouping;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    lambda,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprLambda lambda)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_LAMBDA;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->lambda = lambda;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    list,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprList list)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_LIST;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    literal,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprLiteral literal)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_LITERAL;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->literal = literal;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    tuple,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprTuple tuple)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_TUPLE;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->tuple = tuple;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    unary,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprUnary unary)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_UNARY;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;
    self->unary = unary;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    unknown,
                    const Location *location,
                    const LilyAstExpr *ast_expr)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = LILY_CHECKED_EXPR_KIND_UNKNOWN;
    self->location = location;
    self->data_type =
      NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN, location);
    self->ast_expr = ast_expr;

    return self;
}

CONSTRUCTOR(LilyCheckedExpr *,
            LilyCheckedExpr,
            const Location *location,
            LilyCheckedDataType *data_type,
            const LilyAstExpr *ast_expr,
            enum LilyCheckedExprKind kind)
{
    LilyCheckedExpr *self = lily_malloc(sizeof(LilyCheckedExpr));

    self->kind = kind;
    self->location = location;
    self->data_type = data_type;
    self->ast_expr = ast_expr;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExpr, const LilyCheckedExpr *self)
{
    String *res = format__String(
      "LilyAstExpr{{ kind = {s}, ast_expr = {Sr}, location = {sa}, data_type = "
      "{Sr}",
      to_string__Debug__LilyCheckedExprKind(self->kind),
      to_string__Debug__LilyAstExpr(self->ast_expr),
      to_string__Debug__Location(self->location),
      to_string__Debug__LilyCheckedDataType(self->data_type));

    switch (self->kind) {
        case LILY_CHECKED_EXPR_KIND_ARRAY: {
            char *s =
              format(", array = {Sr} }",
                     to_string__Debug__LilyCheckedExprArray(&self->array));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_BINARY: {
            char *s =
              format(", binary = {sa} }",
                     to_string__Debug__LilyCheckedExprBinary(&self->binary));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_CALL: {
            char *s =
              format(", call = {Sr} }",
                     to_string__Debug__LilyCheckedExprCall(&self->call));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_CAST: {
            char *s =
              format(", cast = {Sr} }",
                     to_string__Debug__LilyCheckedExprCast(&self->cast));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_GROUPING: {
            char *s = format(", grouping = {Sr} }",
                             to_string__Debug__LilyCheckedExpr(self->grouping));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_LAMBDA: {
            char *s =
              format(", lambda = {Sr} }",
                     to_string__Debug__LilyCheckedExprLambda(&self->lambda));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_LIST: {
            char *s =
              format(", list = {Sr} }",
                     to_string__Debug__LilyCheckedExprList(&self->list));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_LITERAL: {
            char *s =
              format(", literal = {Sr} }",
                     to_string__Debug__LilyCheckedExprLiteral(&self->literal));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_TUPLE: {
            char *s =
              format(", tuple = {Sr} }",
                     to_string__Debug__LilyCheckedExprTuple(&self->tuple));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_UNARY: {
            char *s =
              format(", unary = {Sr} }",
                     to_string__Debug__LilyCheckedExprUnary(&self->unary));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_KIND_SELF:
        case LILY_CHECKED_EXPR_KIND_UNKNOWN:
            push_str__String(res, " }");
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExpr, array, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprArray, &self->array);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, binary, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprBinary, &self->binary);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, call, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprCall, &self->call);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, cast, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprCast, &self->cast);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, grouping, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExpr, self->grouping);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, lambda, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprLambda, &self->lambda);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, list, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprList, &self->list);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, literal, LilyCheckedExpr *self)
{
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, tuple, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprTuple, &self->tuple);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExpr, unary, LilyCheckedExpr *self)
{
    FREE(LilyCheckedExprUnary, &self->unary);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedExpr, LilyCheckedExpr *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_KIND_ARRAY:
            FREE_VARIANT(LilyCheckedExpr, array, self);
            break;
        case LILY_CHECKED_EXPR_KIND_BINARY:
            FREE_VARIANT(LilyCheckedExpr, binary, self);
            break;
        case LILY_CHECKED_EXPR_KIND_CALL:
            FREE_VARIANT(LilyCheckedExpr, call, self);
            break;
        case LILY_CHECKED_EXPR_KIND_CAST:
            FREE_VARIANT(LilyCheckedExpr, cast, self);
            break;
        case LILY_CHECKED_EXPR_KIND_GROUPING:
            FREE_VARIANT(LilyCheckedExpr, grouping, self);
            break;
        case LILY_CHECKED_EXPR_KIND_LAMBDA:
            FREE_VARIANT(LilyCheckedExpr, lambda, self);
            break;
        case LILY_CHECKED_EXPR_KIND_LIST:
            FREE_VARIANT(LilyCheckedExpr, list, self);
            break;
        case LILY_CHECKED_EXPR_KIND_LITERAL:
            FREE_VARIANT(LilyCheckedExpr, literal, self);
            break;
        case LILY_CHECKED_EXPR_KIND_TUPLE:
            FREE_VARIANT(LilyCheckedExpr, tuple, self);
            break;
        case LILY_CHECKED_EXPR_KIND_UNARY:
            FREE_VARIANT(LilyCheckedExpr, unary, self);
            break;
        case LILY_CHECKED_EXPR_KIND_SELF:
        case LILY_CHECKED_EXPR_KIND_UNKNOWN:
            FREE(LilyCheckedDataType, self->data_type);
            lily_free(self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
