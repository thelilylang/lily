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
#include <core/lily/checked/expr/call.h>

#include <stdio.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#include <stdlib.h>
#endif

// @brief Free LilyCheckedExprFunParamCall type
// (LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyCheckedExprFunParamCall,
                          default_,
                          LilyCheckedExprFunParamCall *self);

// @brief Free LilyCheckedExprFunParamCall type
// (LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyCheckedExprFunParamCall,
                          normal,
                          LilyCheckedExprFunParamCall *self);

// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_FUN).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 fun,
                                 const LilyCheckedExprCall *self);

// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 record,
                                 const LilyCheckedExprCall *self);

// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_VARIANT).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 variant,
                                 const LilyCheckedExprCall *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallKind,
               enum LilyCheckedExprCallKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_CALL_KIND_FUN:
            return "LILY_CHECKED_EXPR_CALL_KIND_FUN";
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD:
            return "LILY_CHECKED_EXPR_CALL_KIND_RECORD";
        case LILY_CHECKED_EXPR_CALL_KIND_VARIANT:
            return "LILY_CHECKED_EXPR_CALL_KIND_VARIANT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprFunParamCallKind,
               enum LilyCheckedExprFunParamCallKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
            return "LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT";
        case LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
            return "LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedExprFunParamCall *,
                    LilyCheckedExprFunParamCall,
                    default_,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_)
{
    LilyCheckedExprFunParamCall *self =
      lily_malloc(sizeof(LilyCheckedExprFunParamCall));

    self->kind = LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT;
    self->value = value;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExprFunParamCall *,
                    LilyCheckedExprFunParamCall,
                    normal,
                    LilyCheckedExpr *value,
                    Location location)
{
    LilyCheckedExprFunParamCall *self =
      lily_malloc(sizeof(LilyCheckedExprFunParamCall));

    self->kind = LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL;
    self->value = value;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprFunParamCall,
               const LilyCheckedExprFunParamCall *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
            return format__String(
              "LilyCheckedExprFunParamCall{{ kind = {s}, value = {Sr}, "
              "location = "
              "{sa}, default = {S} }",
              to_string__Debug__LilyCheckedExprFunParamCallKind(self->kind),
              to_string__Debug__LilyCheckedExpr(self->value),
              to_string__Debug__Location(&self->location),
              self->default_);
        case LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
            return format__String(
              "LilyCheckedExprFunParamCall{{ kind = {s}, value = {Sr}, "
              "location = "
              "{sa} }",
              to_string__Debug__LilyCheckedExprFunParamCallKind(self->kind),
              to_string__Debug__LilyCheckedExpr(self->value),
              to_string__Debug__Location(&self->location));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprFunParamCall,
                   default_,
                   LilyCheckedExprFunParamCall *self)
{
    FREE(LilyCheckedExpr, self->value);
    FREE_MOVE(self->default_, FREE(String, self->default_));
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExprFunParamCall,
                   normal,
                   LilyCheckedExprFunParamCall *self)
{
    FREE(LilyCheckedExpr, self->value);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedExprFunParamCall, LilyCheckedExprFunParamCall *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
            FREE_VARIANT(LilyCheckedExprFunParamCall, default_, self);
            break;
        case LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedExprFunParamCall, normal, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFun,
               const LilyCheckedExprCallFun *self)
{
    String *res = format__String("LilyCheckedExprCallFun{{ id = {Sr}, params =",
                                 to_string__Debug__LilyCheckedExpr(self->id));

    DEBUG_VEC_STRING(self->params, res, LilyCheckedExprFunParamCall);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallFun, const LilyCheckedExprCallFun *self)
{
    FREE(LilyCheckedExpr, self->id);

    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyCheckedExprFunParamCall);
    FREE(Vec, self->params);
}

CONSTRUCTOR(LilyCheckedExprRecordParamCall *,
            LilyCheckedExprRecordParamCall,
            String *name,
            LilyCheckedExpr *value)
{
    LilyCheckedExprRecordParamCall *self =
      lily_malloc(sizeof(LilyCheckedExprRecordParamCall));

    self->name = name;
    self->value = value;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprRecordParamCall,
               const LilyCheckedExprRecordParamCall *self)
{
    return format__String(
      "LilyCheckedExprRecordParamCall{{ name = {S}, value = {Sr} }",
      self->name,
      to_string__Debug__LilyCheckedExpr(self->value));
}
#endif

DESTRUCTOR(LilyCheckedExprRecordParamCall, LilyCheckedExprRecordParamCall *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));
    FREE(LilyCheckedExpr, self->value);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecord,
               const LilyCheckedExprCallRecord *self)
{
    String *res =
      format__String("LilyCheckedExprCallRecord{{ id = {S}, params =",
                     to_string__Debug__LilyCheckedExpr(self->id));

    DEBUG_VEC_STRING(self->params, res, LilyCheckedExprRecordParamCall);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallRecord, const LilyCheckedExprCallRecord *self)
{
    FREE(LilyCheckedExpr, self->id);
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyCheckedExprRecordParamCall);
    FREE(Vec, self->params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallVariant,
               const LilyCheckedExprCallVariant *self)
{
    return format__String(
      "LilyCheckedExprCallVariant{{ id = {Sr}, value = {Sr} }",
      to_string__Debug__LilyCheckedExpr(self->id),
      to_string__Debug__LilyCheckedExpr(self->value));
}
#endif

DESTRUCTOR(LilyCheckedExprCallVariant, const LilyCheckedExprCallVariant *self)
{
    FREE(LilyCheckedExpr, self->id);
    FREE(LilyCheckedExpr, self->value);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprCall, const LilyCheckedExprCall *self)
{
    String *res =
      format__String("LilyCheckedExprCall{{ kind = {s}, scope = {sa}",
                     to_string__Debug__LilyCheckedExprCallKind(self->kind),
                     to_string__Debug__LilyCheckedScope(&self->scope));

    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_FUN: {
            char *s =
              format(", fun = {Sr} }",
                     to_string__Debug__LilyCheckedExprCallFun(&self->fun));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD: {
            char *s = format(
              ", record = {Sr} }",
              to_string__Debug__LilyCheckedExprCallRecord(&self->record));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_VARIANT: {
            char *s = format(
              ", variant = {Sr} }",
              to_string__Debug__LilyCheckedExprCallVariant(&self->variant));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprCall, fun, const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallFun, &self->fun);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall, record, const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallRecord, &self->record);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                   variant,
                   const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallVariant, &self->variant);
}

DESTRUCTOR(LilyCheckedExprCall, const LilyCheckedExprCall *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_FUN:
            FREE_VARIANT(LilyCheckedExprCall, fun, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD:
            FREE_VARIANT(LilyCheckedExprCall, record, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_VARIANT:
            FREE_VARIANT(LilyCheckedExprCall, variant, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
