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

#include <base/alloc.h>

#include <core/lily/parser/ast/expr.h>
#include <core/lily/parser/ast/expr/call.h>

#include <stdio.h>

#ifdef ENV_DEBUG
#include <base/format.h>
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

// @brief Free LilyAstExprCall type (LILY_AST_EXPR_CALL_KIND_FUN).
static inline VARIANT_DESTRUCTOR(LilyAstExprCall,
                                 fun,
                                 const LilyAstExprCall *self);

// @brief Free LilyAstExprCall type (LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN).
static inline VARIANT_DESTRUCTOR(LilyAstExprCall,
                                 fun_builtin,
                                 const LilyAstExprCall *self);

// @brief Free LilyAstExprCall type (LILY_AST_EXPR_CALL_KIND_FUN_SYS).
static inline VARIANT_DESTRUCTOR(LilyAstExprCall,
                                 fun_sys,
                                 const LilyAstExprCall *self);

// @brief Free LilyAstExprCall type (LILY_AST_EXPR_CALL_KIND_LEN).
static inline VARIANT_DESTRUCTOR(LilyAstExprCall,
                                 len,
                                 const LilyAstExprCall *self);

// @brief Free LilyAstExprCall type (LILY_AST_EXPR_CALL_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyAstExprCall,
                                 record,
                                 const LilyAstExprCall *self);

// @brief Free LilyAstExprCall type (LILY_AST_EXPR_CALL_KIND_VARIANT).
static inline VARIANT_DESTRUCTOR(LilyAstExprCall,
                                 variant,
                                 const LilyAstExprCall *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprCallKind, enum LilyAstExprCallKind self)
{
    switch (self) {
        case LILY_AST_EXPR_CALL_KIND_FUN:
            return "LILY_AST_EXPR_CALL_KIND_FUN";
        case LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN:
            return "LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN";
        case LILY_AST_EXPR_CALL_KIND_FUN_SYS:
            return "LILY_AST_EXPR_CALL_KIND_FUN_SYS";
        case LILY_AST_EXPR_CALL_KIND_LEN:
            return "LILY_AST_EXPR_CALL_KIND_LEN";
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
    String *res = format__String(
      "LilyAstExprCallFun{{ id = {Sr}, ast_params_len = {d}, params =",
      to_string__Debug__LilyAstExpr(self->id),
      self->ast_params_len);

    DEBUG_VEC_STRING(self->params, res, LilyAstExprFunParamCall);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprCallFun, const LilyAstExprCallFun *self)
{
    FREE(LilyAstExpr, self->id);

    if (self->ast_params_len == self->params->len) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyAstExprFunParamCall);
    } else {
        LilyAstExprFunParamCall *last_normal_param = NULL;

        for (Usize i = self->params->len; i--;) {
            LilyAstExprFunParamCall *param = get__Vec(self->params, i);

            if (!last_normal_param) {
                if (param->kind == LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL) {
                    last_normal_param = param;
                    continue;
                }
            }

            FREE(LilyAstExprFunParamCall, param);
        }

        lily_free(last_normal_param);
    }

    FREE(Vec, self->params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallFunSys,
               const LilyAstExprCallFunSys *self)
{
    String *res = format__String("LilyAstExprCallFunSys{{ name = {S}, params =",
                                 self->name);

    DEBUG_VEC_STRING(self->params, res, LilyAstExprFunParamCall);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprCallFunSys, const LilyAstExprCallFunSys *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyAstExprFunParamCall);
    FREE(Vec, self->params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallFunBuiltin,
               const LilyAstExprCallFunBuiltin *self)
{
    String *res = format__String(
      "LilyAstExprCallFunBuiltin{{ name = {S}, params =", self->name);

    DEBUG_VEC_STRING(self->params, res, LilyAstExprFunParamCall);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprCallFunBuiltin, const LilyAstExprCallFunBuiltin *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));
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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallRecord,
               const LilyAstExprCallRecord *self)
{
    String *res = format__String("LilyAstExprCallRecord{{ id = {Sr}, params =",
                                 to_string__Debug__LilyAstExpr(self->id));

    DEBUG_VEC_STRING(self->params, res, LilyAstExprRecordParamCall);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprCallRecord, const LilyAstExprCallRecord *self)
{
    FREE(LilyAstExpr, self->id);
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyAstExprRecordParamCall);
    FREE(Vec, self->params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallVariant,
               const LilyAstExprCallVariant *self)
{
    return format__String("LilyAstExprCallVariant{{ id = {Sr}, value = {Sr} }",
                          to_string__Debug__LilyAstExpr(self->id),
                          to_string__Debug__LilyAstExpr(self->value));
}
#endif

DESTRUCTOR(LilyAstExprCallVariant, const LilyAstExprCallVariant *self)
{
    FREE(LilyAstExpr, self->id);
    FREE(LilyAstExpr, self->value);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprCall, const LilyAstExprCall *self)
{
    String *res =
      format__String("LilyAstExprCall{{ kind = {s}",
                     to_string__Debug__LilyAstExprCallKind(self->kind));

    switch (self->kind) {
        case LILY_AST_EXPR_CALL_KIND_FUN: {
            char *s = format(", fun = {Sr} }",
                             to_string__Debug__LilyAstExprCallFun(&self->fun));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN: {
            char *s = format(
              ", fun_builtin = {Sr} }",
              to_string__Debug__LilyAstExprCallFunBuiltin(&self->fun_builtin));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_CALL_KIND_FUN_SYS: {
            char *s =
              format(", fun_sys = {Sr} }",
                     to_string__Debug__LilyAstExprCallFunSys(&self->fun_sys));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_CALL_KIND_LEN: {
            char *s = format(", len = {Sr} }",
                             to_string__Debug__LilyAstExpr(self->len));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_CALL_KIND_RECORD: {
            char *s =
              format(", record = {Sr} }",
                     to_string__Debug__LilyAstExprCallRecord(&self->record));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_EXPR_CALL_KIND_VARIANT: {
            char *s =
              format(", variant = {Sr} }",
                     to_string__Debug__LilyAstExprCallVariant(&self->variant));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyAstExprCall, fun, const LilyAstExprCall *self)
{
    FREE(LilyAstExprCallFun, &self->fun);
}

VARIANT_DESTRUCTOR(LilyAstExprCall, fun_builtin, const LilyAstExprCall *self)
{
    FREE(LilyAstExprCallFunBuiltin, &self->fun_builtin);
}

VARIANT_DESTRUCTOR(LilyAstExprCall, fun_sys, const LilyAstExprCall *self)
{
    FREE(LilyAstExprCallFunSys, &self->fun_sys);
}

VARIANT_DESTRUCTOR(LilyAstExprCall, len, const LilyAstExprCall *self)
{
    FREE(LilyAstExpr, self->len);
}

VARIANT_DESTRUCTOR(LilyAstExprCall, record, const LilyAstExprCall *self)
{
    FREE(LilyAstExprCallRecord, &self->record);
}

VARIANT_DESTRUCTOR(LilyAstExprCall, variant, const LilyAstExprCall *self)
{
    FREE(LilyAstExprCallVariant, &self->variant);
}

DESTRUCTOR(LilyAstExprCall, const LilyAstExprCall *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_CALL_KIND_FUN:
            FREE_VARIANT(LilyAstExprCall, fun, self);
            break;
        case LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN:
            FREE_VARIANT(LilyAstExprCall, fun_builtin, self);
            break;
        case LILY_AST_EXPR_CALL_KIND_FUN_SYS:
            FREE_VARIANT(LilyAstExprCall, fun_sys, self);
            break;
        case LILY_AST_EXPR_CALL_KIND_LEN:
            FREE_VARIANT(LilyAstExprCall, len, self);
            break;
        case LILY_AST_EXPR_CALL_KIND_RECORD:
            FREE_VARIANT(LilyAstExprCall, record, self);
            break;
        case LILY_AST_EXPR_CALL_KIND_VARIANT:
            FREE_VARIANT(LilyAstExprCall, variant, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
