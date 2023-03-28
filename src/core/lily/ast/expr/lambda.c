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

#include <core/lily/ast/body/fun.h>
#include <core/lily/ast/expr.h>
#include <core/lily/ast/expr/lambda.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstExprLambdaParam type (LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyAstExprLambdaParam,
                          default_,
                          LilyAstExprLambdaParam *self);

// Free LilyAstExprLambdaParam type (LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyAstExprLambdaParam,
                          normal,
                          LilyAstExprLambdaParam *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprLambdaParamKind,
               enum LilyAstExprLambdaParamKind self)
{
    switch (self) {
        case LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT:
            return "LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT";
        case LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL:
            return "LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstExprLambdaParam *,
                    LilyAstExprLambdaParam,
                    default_,
                    Location location,
                    String *name,
                    LilyAstDataType *data_type,
                    LilyAstExpr *optional)
{
    LilyAstExprLambdaParam *self = lily_malloc(sizeof(LilyAstExprLambdaParam));

    self->kind = LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT;
    self->location = location;
    self->name = name;
    self->data_type = data_type;
    self->optional = optional;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExprLambdaParam *,
                    LilyAstExprLambdaParam,
                    normal,
                    Location location,
                    String *name,
                    LilyAstDataType *data_type)
{
    LilyAstExprLambdaParam *self = lily_malloc(sizeof(LilyAstExprLambdaParam));

    self->kind = LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL;
    self->location = location;
    self->name = name;
    self->data_type = data_type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprLambdaParam,
               const LilyAstExprLambdaParam *self)
{
    String *res =
      format__String("LilyAstExprLambdaParam{{ kind = {s}, location = {sa}, "
                     "name = {S}, data_type = ",
                     to_string__Debug__LilyAstExprLambdaParamKind(self->kind),
                     to_string__Debug__Location(&self->location),
                     self->name);

    if (self->data_type) {
        String *s = to_string__Debug__LilyAstDataType(self->data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    switch (self->kind) {
        case LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT: {
            String *s = to_string__Debug__LilyAstExpr(self->optional);

            push_str__String(res, ", optional = ");
            APPEND_AND_FREE(res, s);
            push_str__String(res, " }");

            break;
        }
        case LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL:
            push_str__String(res, " }");
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyAstExprLambdaParam,
                   default_,
                   LilyAstExprLambdaParam *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->data_type) {
        FREE(LilyAstDataType, self->data_type);
    }

    FREE(LilyAstExpr, self->optional);

    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExprLambdaParam, normal, LilyAstExprLambdaParam *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->data_type) {
        FREE(LilyAstDataType, self->data_type);
    }

    lily_free(self);
}

DESTRUCTOR(LilyAstExprLambdaParam, LilyAstExprLambdaParam *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT:
            FREE_VARIANT(LilyAstExprLambdaParam, default_, self);
            break;
        case LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyAstExprLambdaParam, normal, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprLambda, const LilyAstExprLambda *self)
{
    String *res = from__String("LilyAstExprLambda{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyAstExprLambdaParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " return_data_type = ");

    if (self->return_data_type) {
        String *s = to_string__Debug__LilyAstDataType(self->return_data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, " body =");

    DEBUG_VEC_STRING(self->body, res, LilyAstBodyFunItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprLambda, const LilyAstExprLambda *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyAstExprLambdaParam);
        FREE(Vec, self->params);
    }

    if (self->return_data_type) {
        FREE(LilyAstDataType, self->return_data_type);
    }

    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, LilyAstBodyFunItem);
    FREE(Vec, self->body);
}
