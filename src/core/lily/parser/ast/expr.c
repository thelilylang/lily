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
#include <base/assert.h>

#include <core/lily/parser/ast/expr.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_ACCESS).
static VARIANT_DESTRUCTOR(LilyAstExpr, access, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_ARRAY).
static VARIANT_DESTRUCTOR(LilyAstExpr, array, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_BINARY).
static VARIANT_DESTRUCTOR(LilyAstExpr, binary, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_CALL).
static VARIANT_DESTRUCTOR(LilyAstExpr, call, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_CAST).
static VARIANT_DESTRUCTOR(LilyAstExpr, cast, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_GROUPING).
static VARIANT_DESTRUCTOR(LilyAstExpr, grouping, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_IDENTIFIER).
static VARIANT_DESTRUCTOR(LilyAstExpr, identifier, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR).
static VARIANT_DESTRUCTOR(LilyAstExpr, identifier_dollar, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_LAMBDA).
static VARIANT_DESTRUCTOR(LilyAstExpr, lambda, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_LIST).
static VARIANT_DESTRUCTOR(LilyAstExpr, list, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_LITERAL).
static VARIANT_DESTRUCTOR(LilyAstExpr, literal, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_TRY).
static VARIANT_DESTRUCTOR(LilyAstExpr, try, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyAstExpr, tuple, LilyAstExpr *self);

// Free LilyAstExpr type (LILY_AST_EXPR_KIND_UNARY).
static VARIANT_DESTRUCTOR(LilyAstExpr, unary, LilyAstExpr *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprKind, enum LilyAstExprKind self)
{
    switch (self) {
        case LILY_AST_EXPR_KIND_ACCESS:
            return "LILY_AST_EXPR_KIND_ACCESS";
        case LILY_AST_EXPR_KIND_ARRAY:
            return "LILY_AST_EXPR_KIND_ARRAY";
        case LILY_AST_EXPR_KIND_BINARY:
            return "LILY_AST_EXPR_KIND_BINARY";
        case LILY_AST_EXPR_KIND_CALL:
            return "LILY_AST_EXPR_KIND_CALL";
        case LILY_AST_EXPR_KIND_CAST:
            return "LILY_AST_EXPR_KIND_CAST";
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            return "LILY_AST_EXPR_KIND_IDENTIFIER";
        case LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR:
            return "LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR";
        case LILY_AST_EXPR_KIND_LAMBDA:
            return "LILY_AST_EXPR_KIND_LAMBDA";
        case LILY_AST_EXPR_KIND_LIST:
            return "LILY_AST_EXPR_KIND_LIST";
        case LILY_AST_EXPR_KIND_LITERAL:
            return "LILY_AST_EXPR_KIND_LITERAL";
        case LILY_AST_EXPR_KIND_SELF:
            return "LILY_AST_EXPR_KIND_SELF";
        case LILY_AST_EXPR_KIND_TRY:
            return "LILY_AST_EXPR_KIND_TRY";
        case LILY_AST_EXPR_KIND_TUPLE:
            return "LILY_AST_EXPR_KIND_TUPLE";
        case LILY_AST_EXPR_KIND_UNARY:
            return "LILY_AST_EXPR_KIND_UNARY";
        case LILY_AST_EXPR_KIND_GROUPING:
            return "LILY_AST_EXPR_KIND_GROUPING";
        case LILY_AST_EXPR_KIND_WILDCARD:
            return "LILY_AST_EXPR_KIND_WILDCARD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    access,
                    Location location,
                    LilyAstExprAccess access)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_ACCESS;
    self->location = location;
    self->access = access;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    array,
                    Location location,
                    LilyAstExprArray array)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_ARRAY;
    self->location = location;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    binary,
                    Location location,
                    LilyAstExprBinary binary)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_BINARY;
    self->location = location;
    self->binary = binary;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    call,
                    Location location,
                    LilyAstExprCall call)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_CALL;
    self->location = location;
    self->call = call;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    cast,
                    Location location,
                    LilyAstExprCast cast)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_CAST;
    self->location = location;
    self->cast = cast;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    grouping,
                    Location location,
                    LilyAstExpr *grouping)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_GROUPING;
    self->location = location;
    self->grouping = grouping;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    identifier,
                    Location location,
                    LilyAstExprIdentifier identifier)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_IDENTIFIER;
    self->location = location;
    self->identifier = identifier;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    identifier_dollar,
                    Location location,
                    LilyAstExprIdentifierDollar identifier_dollar)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR;
    self->location = location;
    self->identifier_dollar = identifier_dollar;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    lambda,
                    Location location,
                    LilyAstExprLambda lambda)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_LAMBDA;
    self->location = location;
    self->lambda = lambda;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    list,
                    Location location,
                    LilyAstExprList list)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_LIST;
    self->location = location;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    literal,
                    Location location,
                    LilyAstExprLiteral literal)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_LITERAL;
    self->location = location;
    self->literal = literal;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    try,
                    Location location,
                    LilyAstExprTry try)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_TRY;
    self->location = location;
    self->try = try;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    tuple,
                    Location location,
                    LilyAstExprTuple tuple)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_TUPLE;
    self->location = location;
    self->tuple = tuple;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    unary,
                    Location location,
                    LilyAstExprUnary unary)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_EXPR_KIND_UNARY;
    self->location = location;
    self->unary = unary;

    return self;
}

CONSTRUCTOR(LilyAstExpr *,
            LilyAstExpr,
            Location location,
            enum LilyAstExprKind kind)
{
    LilyAstExpr *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = kind;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExpr, const LilyAstExpr *self)
{
    String *res = format__String("LilyAstExpr{{ kind = {s}, location = {sa}",
                                 to_string__Debug__LilyAstExprKind(self->kind),
                                 to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_AST_EXPR_KIND_ACCESS: {
            char *s =
              format(", access = {Sr} }",
                     to_string__Debug__LilyAstExprAccess(&self->access));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_ARRAY: {
            char *s = format(", array = {Sr} }",
                             to_string__Debug__LilyAstExprArray(&self->array));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_BINARY: {
            char *s =
              format(", binary = {sa} }",
                     to_string__Debug__LilyAstExprBinary(&self->binary));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_CALL: {
            char *s = format(", call = {Sr} }",
                             to_string__Debug__LilyAstExprCall(&self->call));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_CAST: {
            char *s = format(", cast = {Sr} }",
                             to_string__Debug__LilyAstExprCast(&self->cast));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_GROUPING: {
            char *s = format(", grouping = {Sr} }",
                             to_string__Debug__LilyAstExpr(self->grouping));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_IDENTIFIER: {
            char *s = format(
              ", identifier = {Sr} }",
              to_string__Debug__LilyAstExprIdentifier(&self->identifier));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR: {
            char *s = format(", identifier_dollar = {Sr} }",
                             to_string__Debug__LilyAstExprIdentifierDollar(
                               &self->identifier_dollar));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_LAMBDA: {
            char *s =
              format(", lambda = {Sr} }",
                     to_string__Debug__LilyAstExprLambda(&self->lambda));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_LIST: {
            char *s = format(", list = {Sr} }",
                             to_string__Debug__LilyAstExprList(&self->list));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_LITERAL: {
            char *s =
              format(", literal = {Sr} }",
                     to_string__Debug__LilyAstExprLiteral(&self->literal));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_TRY: {
            char *s = format(", try = {Sr} }",
                             to_string__Debug__LilyAstExprTry(&self->try));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_TUPLE: {
            char *s = format(", tuple = {Sr} }",
                             to_string__Debug__LilyAstExprTuple(&self->tuple));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_UNARY: {
            char *s = format(", unary = {Sr} }",
                             to_string__Debug__LilyAstExprUnary(&self->unary));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_KIND_SELF:
        case LILY_AST_EXPR_KIND_WILDCARD:
            push_str__String(res, " }");
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

String *
to_string__LilyAstExpr(const LilyAstExpr *self)
{
    TODO("implements");
}

Vec *
get_generic_params__LilyAstExpr(LilyAstExpr *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            switch (self->access.kind) {
                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                    return get_generic_params__LilyAstExpr(
                      last__Vec(self->access.global_path));
                case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
                    return get_generic_params__LilyAstExpr(
                      last__Vec(self->access.self_path));
                case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
                    return get_generic_params__LilyAstExpr(
                      last__Vec(self->access.Self_path));
                case LILY_AST_EXPR_ACCESS_KIND_PATH:
                    return get_generic_params__LilyAstExpr(
                      last__Vec(self->access.path));
                default:
                    UNREACHABLE("not expected in this context");
            }
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            return self->identifier.generic_params;
        default:
            UNREACHABLE("not expected in this context");
    }
}

Vec *
get_generic_params_from_variant_call__LilyAstExpr(LilyAstExpr *self)
{
    ASSERT(self->kind == LILY_AST_EXPR_KIND_CALL);
    ASSERT(self->call.kind == LILY_AST_EXPR_CALL_KIND_VARIANT);

    switch (self->call.variant.id->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            switch (self->call.variant.id->kind) {
                case LILY_AST_EXPR_ACCESS_KIND_PATH:
                    return CAST(LilyAstExpr *,
                                get__Vec(
                                  self->call.variant.id->access.path,
                                  self->call.variant.id->access.path->len - 2))
                      ->identifier.generic_params;
                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                    if (self->call.variant.id->access.global_path->len > 1) {
                        return CAST(LilyAstExpr *,
                                    get__Vec(
                                      self->call.variant.id->access.global_path,
                                      self->call.variant.id->access.global_path
                                          ->len -
                                        2))
                          ->identifier.generic_params;
                    }

                    // example of case: global.A
                    return NULL;
                default:
                    UNREACHABLE("cannot get in this context");
            }
        default:
            return NULL;
    }
}

String *
get_last_name__LilyAstExpr(const LilyAstExpr *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            return self->identifier.name;
        case LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR:
            return self->identifier_dollar.name;
        case LILY_AST_EXPR_KIND_ACCESS:
            switch (self->access.kind) {
                case LILY_AST_EXPR_ACCESS_KIND_AT:
                    if (self->access.at.access) {
                        return get_last_name__LilyAstExpr(
                          self->access.at.access);
                    }

                    return NULL;
                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                    return get_last_name__LilyAstExpr(
                      last__Vec(self->access.global_path));
                case LILY_AST_EXPR_ACCESS_KIND_HOOK:
                    return get_last_name__LilyAstExpr(self->access.hook.access);
                case LILY_AST_EXPR_ACCESS_KIND_PATH:
                    return get_last_name__LilyAstExpr(
                      last__Vec(self->access.path));
                case LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH:
                    return get_last_name__LilyAstExpr(
                      last__Vec(self->access.global_path));
                case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
                    return get_last_name__LilyAstExpr(
                      last__Vec(self->access.property_init));
                case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
                    return get_last_name__LilyAstExpr(
                      last__Vec(self->access.self_path));
                case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
                    return get_last_name__LilyAstExpr(
                      last__Vec(self->access.Self_path));
                default:
                    UNREACHABLE("unknown variant");
            }
            break;
        default:
            return NULL;
    }
}

VARIANT_DESTRUCTOR(LilyAstExpr, access, LilyAstExpr *self)
{
    FREE(LilyAstExprAccess, &self->access);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, array, LilyAstExpr *self)
{
    FREE(LilyAstExprArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, binary, LilyAstExpr *self)
{
    FREE(LilyAstExprBinary, &self->binary);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, call, LilyAstExpr *self)
{
    FREE(LilyAstExprCall, &self->call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, cast, LilyAstExpr *self)
{
    FREE(LilyAstExprCast, &self->cast);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, grouping, LilyAstExpr *self)
{
    FREE(LilyAstExpr, self->grouping);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, identifier, LilyAstExpr *self)
{
    FREE(LilyAstExprIdentifier, &self->identifier);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, identifier_dollar, LilyAstExpr *self)
{
    FREE(LilyAstExprIdentifierDollar, &self->identifier_dollar);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, lambda, LilyAstExpr *self)
{
    FREE(LilyAstExprLambda, &self->lambda);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, list, LilyAstExpr *self)
{
    FREE(LilyAstExprList, &self->list);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, literal, LilyAstExpr *self)
{
    FREE(LilyAstExprLiteral, &self->literal);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, try, LilyAstExpr *self)
{
    FREE(LilyAstExprTry, &self->try);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, tuple, LilyAstExpr *self)
{
    FREE(LilyAstExprTuple, &self->tuple);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExpr, unary, LilyAstExpr *self)
{
    FREE(LilyAstExprUnary, &self->unary);
    lily_free(self);
}

DESTRUCTOR(LilyAstExpr, LilyAstExpr *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            FREE_VARIANT(LilyAstExpr, access, self);
            break;
        case LILY_AST_EXPR_KIND_ARRAY:
            FREE_VARIANT(LilyAstExpr, array, self);
            break;
        case LILY_AST_EXPR_KIND_BINARY:
            FREE_VARIANT(LilyAstExpr, binary, self);
            break;
        case LILY_AST_EXPR_KIND_CALL:
            FREE_VARIANT(LilyAstExpr, call, self);
            break;
        case LILY_AST_EXPR_KIND_CAST:
            FREE_VARIANT(LilyAstExpr, cast, self);
            break;
        case LILY_AST_EXPR_KIND_GROUPING:
            FREE_VARIANT(LilyAstExpr, grouping, self);
            break;
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            FREE_VARIANT(LilyAstExpr, identifier, self);
            break;
        case LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR:
            FREE_VARIANT(LilyAstExpr, identifier_dollar, self);
            break;
        case LILY_AST_EXPR_KIND_LAMBDA:
            FREE_VARIANT(LilyAstExpr, lambda, self);
            break;
        case LILY_AST_EXPR_KIND_LIST:
            FREE_VARIANT(LilyAstExpr, list, self);
            break;
        case LILY_AST_EXPR_KIND_LITERAL:
            FREE_VARIANT(LilyAstExpr, literal, self);
            break;
        case LILY_AST_EXPR_KIND_TRY:
            FREE_VARIANT(LilyAstExpr, try, self);
            break;
        case LILY_AST_EXPR_KIND_TUPLE:
            FREE_VARIANT(LilyAstExpr, tuple, self);
            break;
        case LILY_AST_EXPR_KIND_UNARY:
            FREE_VARIANT(LilyAstExpr, unary, self);
            break;
        case LILY_AST_EXPR_KIND_SELF:
        case LILY_AST_EXPR_KIND_WILDCARD:
            lily_free(self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
