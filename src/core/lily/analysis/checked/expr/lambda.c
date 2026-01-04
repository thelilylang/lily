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

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/lambda.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedExprLambdaParam type
// (LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyCheckedExprLambdaParam,
                          default_,
                          LilyCheckedExprLambdaParam *self);

// Free LilyCheckedExprLambdaParam type
// (LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyCheckedExprLambdaParam,
                          normal,
                          LilyCheckedExprLambdaParam *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprLambdaParamKind,
               enum LilyCheckedExprLambdaParamKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT:
            return "LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT";
        case LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL:
            return "LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedExprLambdaParam *,
                    LilyCheckedExprLambdaParam,
                    default_,
                    Location location,
                    String *name,
                    LilyCheckedDataType *data_type,
                    LilyCheckedExpr *optional)
{
    LilyCheckedExprLambdaParam *self =
      lily_malloc(sizeof(LilyCheckedExprLambdaParam));

    self->kind = LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT;
    self->location = location;
    self->name = name;
    self->data_type = data_type;
    self->optional = optional;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExprLambdaParam *,
                    LilyCheckedExprLambdaParam,
                    normal,
                    Location location,
                    String *name,
                    LilyCheckedDataType *data_type)
{
    LilyCheckedExprLambdaParam *self =
      lily_malloc(sizeof(LilyCheckedExprLambdaParam));

    self->kind = LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL;
    self->location = location;
    self->name = name;
    self->data_type = data_type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprLambdaParam,
               const LilyCheckedExprLambdaParam *self)
{
    String *res = format__String(
      "LilyCheckedExprLambdaParam{{ kind = {s}, location = {sa}, "
      "name = {S}, data_type = ",
      to_string__Debug__LilyCheckedExprLambdaParamKind(self->kind),
      to_string__Debug__Location(&self->location),
      self->name);

    if (self->data_type) {
        String *s = to_string__Debug__LilyCheckedDataType(self->data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    switch (self->kind) {
        case LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT: {
            String *s = to_string__Debug__LilyCheckedExpr(self->optional);

            push_str__String(res, ", optional = ");
            APPEND_AND_FREE(res, s);
            push_str__String(res, " }");

            break;
        }
        case LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL:
            push_str__String(res, " }");
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprLambdaParam,
                   default_,
                   LilyCheckedExprLambdaParam *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    FREE(LilyCheckedExpr, self->optional);

    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExprLambdaParam,
                   normal,
                   LilyCheckedExprLambdaParam *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    lily_free(self);
}

DESTRUCTOR(LilyCheckedExprLambdaParam, LilyCheckedExprLambdaParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT:
            FREE_VARIANT(LilyCheckedExprLambdaParam, default_, self);
            break;
        case LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedExprLambdaParam, normal, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprLambda,
               const LilyCheckedExprLambda *self)
{
    String *res = from__String("LilyCheckedExprLambda{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedExprLambdaParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " return_data_type = ");

    if (self->return_data_type) {
        String *s =
          to_string__Debug__LilyCheckedDataType(self->return_data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, " body =");

    DEBUG_VEC_STRING(self->body, res, LilyCheckedBodyFunItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprLambda, const LilyCheckedExprLambda *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedExprLambdaParam);
        FREE(Vec, self->params);
    }

    if (self->return_data_type) {
        FREE(LilyCheckedDataType, self->return_data_type);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body);
}
