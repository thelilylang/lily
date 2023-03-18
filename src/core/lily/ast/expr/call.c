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

#include <core/lily/ast/expr.h>
#include <core/lily/ast/expr/call.h>

#include <stdio.h>

#ifdef ENV_DEBUG
#include <stdlib.h>
#endif

// @brief Free LilyAstExprFunParamCall type
// (LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyAstExprFunParamCall,
                          default_,
                          LilyAstExprFunParamCall *self);

// @brief Free LilyAstExprFunParamCall type
// (LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyAstExprFunParamCall,
                          normal,
                          LilyAstExprFunParamCall *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprCallKind, enum LilyAstExprCallKind self)
{
    switch (self) {
        case LILY_AST_EXPR_CALL_KIND_FUN:
            return "LILY_AST_EXPR_CALL_KIND_FUN";
        case LILY_AST_EXPR_CALL_KIND_RECORD:
            return "LILY_AST_EXPR_CALL_KIND_RECORD";
        case LILY_AST_EXPR_CALL_KIND_VARIANT:
            return "LILY_AST_EXPR_CALL_KIND_VARIANT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprFunParamCallKind,
               enum LilyAstExprFunParamCallKind self)
{
    switch (self) {
        case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
            return "LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT";
        case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
            return "LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstExprFunParamCall *,
                    LilyAstExprFunParamCall,
                    default_,
                    LilyAstExpr *value,
                    Location location,
                    String *default_)
{
    LilyAstExprFunParamCall *self =
      lily_malloc(sizeof(LilyAstExprFunParamCall));

    self->kind = LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT;
    self->value = value;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstExprFunParamCall *,
                    LilyAstExprFunParamCall,
                    normal,
                    LilyAstExpr *value,
                    Location location)
{
    LilyAstExprFunParamCall *self =
      lily_malloc(sizeof(LilyAstExprFunParamCall));

    self->kind = LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL;
    self->value = value;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprFunParamCall,
               const LilyAstExprFunParamCall *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
            return format__String(
              "LilyAstExprFunParamCall{{ kind = {s}, value = {Sr}, location = "
              "{sa}, default = {S} }",
              to_string__Debug__LilyAstExprFunParamCallKind(self->kind),
              to_string__Debug__LilyAstExpr(self->value),
              to_string__Debug__Location(&self->location),
              self->default_);
        case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
            return format__String(
              "LilyAstExprFunParamCall{{ kind = {s}, value = {Sr}, location = "
              "{sa} }",
              to_string__Debug__LilyAstExprFunParamCallKind(self->kind),
              to_string__Debug__LilyAstExpr(self->value),
              to_string__Debug__Location(&self->location));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstExprFunParamCall,
                   default_,
                   LilyAstExprFunParamCall *self)
{
    FREE(LilyAstExpr, self->value);
    FREE_MOVE(self->default_, FREE(String, self->default_));
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstExprFunParamCall,
                   normal,
                   LilyAstExprFunParamCall *self)
{
    FREE(LilyAstExpr, self->value);
    lily_free(self);
}

DESTRUCTOR(LilyAstExprFunParamCall, LilyAstExprFunParamCall *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
            FREE_VARIANT(LilyAstExprFunParamCall, default_, self);
            break;
        case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
            FREE_VARIANT(LilyAstExprFunParamCall, normal, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprCallFun, const LilyAstExprCallFun *self)
{
    String *res = format__String("LilyAstExprCallFun{{ id = {Sr}, params =",
                                 to_string__Debug__LilyAstExpr(self->id));

    DEBUG_VEC_STRING(self->params, res, LilyAstExprFunParamCall);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprCallFun, const LilyAstExprCallFun *self)
{
    FREE(LilyAstExpr, self->id);

    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyAstExprFunParamCall);
    FREE(Vec, self->params);
}

CONSTRUCTOR(LilyAstExprRecordParamCall *,
            LilyAstExprRecordParamCall,
            String *name,
            LilyAstExpr *value)
{
    LilyAstExprRecordParamCall *self =
      lily_malloc(sizeof(LilyAstExprRecordParamCall));

    self->name = name;
    self->value = value;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprRecordParamCall,
               const LilyAstExprRecordParamCall *self)
{
    return format__String(
      "LilyAstExprRecordParamCall{{ name = {S}, value = {Sr} }",
      self->name,
      to_string__Debug__LilyAstExpr(self->value));
}
#endif

DESTRUCTOR(LilyAstExprRecordParamCall, LilyAstExprRecordParamCall *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));
    FREE(LilyAstExpr, self->value);
    lily_free(self);
}

DESTRUCTOR(LilyAstExprCallRecord, const LilyAstExprCallRecord *self)
{
    FREE(LilyAstExpr, self->id);
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyAstExprRecordParamCall);
    FREE(Vec, self->params);
}
