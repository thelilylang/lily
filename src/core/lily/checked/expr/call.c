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

/// @brief Free LilyCheckedExprCallError type.
static DESTRUCTOR(LilyCheckedExprCallError,
                  const LilyCheckedExprCallError *self);

/// @brief Free LilyCheckedExprCallFunParam type
/// (LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyCheckedExprCallFunParam,
                          default_,
                          LilyCheckedExprCallFunParam *self);

/// @brief Free LilyCheckedExprCallFunParam type
/// (LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT_OVERWRITE).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCallFunParam,
                                 default_overwrite,
                                 LilyCheckedExprCallFunParam *self);

/// @brief Free LilyCheckedExprCallFunParam type
/// (LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyCheckedExprCallFunParam,
                          normal,
                          LilyCheckedExprCallFunParam *self);

/// @brief Free LilyCheckedExprCallFunParam type.
static DESTRUCTOR(LilyCheckedExprCallFunParam,
                  LilyCheckedExprCallFunParam *self);

/// @brief Free LilyCheckedExprCallFun type.
static DESTRUCTOR(LilyCheckedExprCallFun, const LilyCheckedExprCallFun *self);

/// @brief Free LilyCheckedExprCallMethodParam type
/// (LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyCheckedExprCallMethodParam,
                          default_,
                          LilyCheckedExprCallMethodParam *self);

/// @brief Free LilyCheckedExprCallMethodParam type
/// (LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT_OVERWRITE).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCallMethodParam,
                                 default_overwrite,
                                 LilyCheckedExprCallMethodParam *self);

/// @brief Free LilyCheckedExprCallMethodParam type
/// (LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyCheckedExprCallMethodParam,
                          normal,
                          LilyCheckedExprCallMethodParam *self);

/// @brief Free LilyCheckedExprCallMethodParam type.
static DESTRUCTOR(LilyCheckedExprCallMethodParam,
                  LilyCheckedExprCallMethodParam *self);

/// @brief Free LilyCheckedExprCallMethod type.
static DESTRUCTOR(LilyCheckedExprCallMethod,
                  const LilyCheckedExprCallMethod *self);

/// @brief Free LilyCheckedExprCallRecord type.
static DESTRUCTOR(LilyCheckedExprCallRecord,
                  const LilyCheckedExprCallRecord *self);

/// @brief Free LilyCheckedExprCallVariant type.
static DESTRUCTOR(LilyCheckedExprCallVariant,
                  const LilyCheckedExprCallVariant *self);

/// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 cstr_len,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_ERROR).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 error,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_FUN).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 fun,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type
/// (LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 fun_builtin,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 fun_sys,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_METHOD).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 method,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 record,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type
/// (LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 record_field_access,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type
/// (LILY_CHECKED_EXPR_CALL_KIND_STR_LEN).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                                 str_len,
                                 const LilyCheckedExprCall *self);

/// @brief Free LilyCheckedExprCall type (LILY_CHECKED_EXPR_CALL_KIND_VARIANT).
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
        case LILY_CHECKED_EXPR_CALL_KIND_ATTRIBUTE:
            return "LILY_CHECKED_EXPR_CALL_KIND_ATTRIBUTE";
        case LILY_CHECKED_EXPR_CALL_KIND_CLASS:
            return "LILY_CHECKED_EXPR_CALL_KIND_CLASS";
        case LILY_CHECKED_EXPR_CALL_KIND_CONSTANT:
            return "LILY_CHECKED_EXPR_CALL_KIND_CONSTANT";
        case LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN:
            return "LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN";
        case LILY_CHECKED_EXPR_CALL_KIND_ERROR:
            return "LILY_CHECKED_EXPR_CALL_KIND_ERROR";
        case LILY_CHECKED_EXPR_CALL_KIND_FUN:
            return "LILY_CHECKED_EXPR_CALL_KIND_FUN";
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN:
            return "LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN";
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS:
            return "LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS";
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
            return "LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM";
        case LILY_CHECKED_EXPR_CALL_KIND_METHOD:
            return "LILY_CHECKED_EXPR_CALL_KIND_METHOD";
        case LILY_CHECKED_EXPR_CALL_KIND_MODULE:
            return "LILY_CHECKED_EXPR_CALL_KIND_MODULE";
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD:
            return "LILY_CHECKED_EXPR_CALL_KIND_RECORD";
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE:
            return "LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE";
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS:
            return "LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS";
        case LILY_CHECKED_EXPR_CALL_KIND_STR_LEN:
            return "LILY_CHECKED_EXPR_CALL_KIND_STR_LEN";
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
            return "LILY_CHECKED_EXPR_CALL_KIND_VARIABLE";
        case LILY_CHECKED_EXPR_CALL_KIND_VARIANT:
            return "LILY_CHECKED_EXPR_CALL_KIND_VARIANT";
        case LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN:
            return "LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallError,
               const LilyCheckedExprCallError *self)
{
    String *res = format__String("LilyCheckedExprCallError{{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedExpr);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallError, const LilyCheckedExprCallError *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedExpr);
        FREE(Vec, self->params);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunParamKind,
               enum LilyCheckedExprCallFunParamKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT:
            return "LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT";
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT_OVERWRITE:
            return "LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_OVERWRITE";
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL:
            return "LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedExprCallFunParam *,
                    LilyCheckedExprCallFunParam,
                    default_,
                    LilyCheckedExpr *value,
                    const Location *location,
                    String *default_)
{
    LilyCheckedExprCallFunParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallFunParam));

    self->kind = LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT;
    self->value = value;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExprCallFunParam *,
                    LilyCheckedExprCallFunParam,
                    default_overwrite,
                    LilyCheckedExpr *value,
                    const Location *location,
                    String *default_)
{
    LilyCheckedExprCallFunParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallFunParam));

    self->kind = LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT_OVERWRITE;
    self->value = value;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExprCallFunParam *,
                    LilyCheckedExprCallFunParam,
                    normal,
                    LilyCheckedExpr *value,
                    const Location *location)
{
    LilyCheckedExprCallFunParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallFunParam));

    self->kind = LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL;
    self->value = value;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunParam,
               const LilyCheckedExprCallFunParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT:
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT_OVERWRITE:
            return format__String(
              "LilyCheckedExprCallFunParam{{ kind = {s}, value = {Sr}, "
              "location = "
              "{sa}, default = {S} }",
              to_string__Debug__LilyCheckedExprCallFunParamKind(self->kind),
              to_string__Debug__LilyCheckedExpr(self->value),
              to_string__Debug__Location(self->location),
              self->default_);
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL:
            return format__String(
              "LilyCheckedExprCallFunParam{{ kind = {s}, value = {Sr}, "
              "location = "
              "{sa} }",
              to_string__Debug__LilyCheckedExprCallFunParamKind(self->kind),
              to_string__Debug__LilyCheckedExpr(self->value),
              to_string__Debug__Location(self->location));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprCallFunParam,
                   default_,
                   LilyCheckedExprCallFunParam *self)
{
    FREE(LilyCheckedExpr, self->value);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCallFunParam,
                   default_overwrite,
                   LilyCheckedExprCallFunParam *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCallFunParam,
                   normal,
                   LilyCheckedExprCallFunParam *self)
{
    FREE(LilyCheckedExpr, self->value);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedExprCallFunParam, LilyCheckedExprCallFunParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT:
            FREE_VARIANT(LilyCheckedExprCallFunParam, default_, self);
            break;
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT_OVERWRITE:
            FREE_VARIANT(LilyCheckedExprCallFunParam, default_overwrite, self);
            break;
        case LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedExprCallFunParam, normal, self);
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
    String *res = from__String("LilyCheckedExprCallFun{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedExprCallFunParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", generic_params =");

    if (self->generic_params) {
        DEBUG_STRING_HASH_MAP(self->generic_params, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallFun, const LilyCheckedExprCallFun *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedExprCallFunParam);
        FREE(Vec, self->params);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunBuiltin,
               const LilyCheckedExprCallFunBuiltin *self)
{
    String *res = from__String("LilyCheckedExprCallFunBuiltin{ params =");

    DEBUG_VEC_STRING(self->params, res, LilyCheckedExprCallFunParam);

    {
        char *s =
          format(", builtin_fun_signature = {Sr} }",
                 to_string__Debug__LilyBuiltinFun(self->builtin_fun_signature));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallFunBuiltin,
           const LilyCheckedExprCallFunBuiltin *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedExprCallFunParam);
        FREE(Vec, self->params);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunSys,
               const LilyCheckedExprCallFunSys *self)
{
    String *res = from__String("LilyCheckedExprCallFunSys{ params =");

    DEBUG_VEC_STRING(self->params, res, LilyCheckedExprCallFunParam);

    {
        char *s = format(", sys_fun_signature = {Sr} }",
                         to_string__Debug__LilySysFun(self->sys_fun_signature));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallFunSys, const LilyCheckedExprCallFunSys *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedExprCallFunParam);
        FREE(Vec, self->params);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallMethodParamKind,
               enum LilyCheckedExprCallMethodParamKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT:
            return "LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT";
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT_OVERWRITE:
            return "LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_OVERWRITE";
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL:
            return "LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL";
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_SELF:
            return "LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_SELF";
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_SELF:
            return "LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_SELF";
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_MUT_SELF:
            return "LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_MUT_SELF";
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_MUT_SELF:
            return "LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_MUT_SELF";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
                    LilyCheckedExprCallMethodParam,
                    default_,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_)
{
    LilyCheckedExprCallMethodParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallMethodParam));

    self->kind = LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT;
    self->value = value;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
                    LilyCheckedExprCallMethodParam,
                    default_overwrite,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_)
{
    LilyCheckedExprCallMethodParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallMethodParam));

    self->kind = LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT_OVERWRITE;
    self->value = value;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
                    LilyCheckedExprCallMethodParam,
                    normal,
                    LilyCheckedExpr *value,
                    Location location)
{
    LilyCheckedExprCallMethodParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallMethodParam));

    self->kind = LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL;
    self->value = value;
    self->location = location;

    return self;
}

CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
            LilyCheckedExprCallMethodParam,
            enum LilyCheckedExprCallMethodParamKind kind,
            Location location)
{
    LilyCheckedExprCallMethodParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallMethodParam));

    self->kind = kind;
    self->value = NULL;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallMethodParam,
               const LilyCheckedExprCallMethodParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT:
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT_OVERWRITE:
            return format__String(
              "LilyCheckedExprCallMethodParam{{ kind = {s}, value = {Sr}, "
              "location = "
              "{sa}, default = {S} }",
              to_string__Debug__LilyCheckedExprCallMethodParamKind(self->kind),
              to_string__Debug__LilyCheckedExpr(self->value),
              to_string__Debug__Location(&self->location),
              self->default_);
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL:
            return format__String(
              "LilyCheckedExprCallMethodParam{{ kind = {s}, value = {Sr}, "
              "location = "
              "{sa} }",
              to_string__Debug__LilyCheckedExprCallMethodParamKind(self->kind),
              to_string__Debug__LilyCheckedExpr(self->value),
              to_string__Debug__Location(&self->location));
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_SELF:
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_SELF:
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_MUT_SELF:
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_MUT_SELF:
            return format__String(
              "LilyCheckedExprCallFunMethodParam{{ kind = {s}, value = NULL, "
              "location = "
              "{sa} }",
              to_string__Debug__LilyCheckedExprCallMethodParamKind(self->kind),
              to_string__Debug__Location(&self->location));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprCallMethodParam,
                   default_,
                   LilyCheckedExprCallMethodParam *self)
{
    FREE(LilyCheckedExpr, self->value);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCallMethodParam,
                   default_overwrite,
                   LilyCheckedExprCallMethodParam *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCallMethodParam,
                   normal,
                   LilyCheckedExprCallMethodParam *self)
{
    FREE(LilyCheckedExpr, self->value);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedExprCallMethodParam, LilyCheckedExprCallMethodParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT:
            FREE_VARIANT(LilyCheckedExprCallMethodParam, default_, self);
            break;
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT_OVERWRITE:
            FREE_VARIANT(
              LilyCheckedExprCallMethodParam, default_overwrite, self);
            break;
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedExprCallMethodParam, normal, self);
            break;
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_SELF:
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_SELF:
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_MUT_SELF:
        case LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_MUT_SELF:
            lily_free(self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallMethod,
               const LilyCheckedExprCallMethod *self)
{
    String *res = from__String("LilyCheckedExprCallMethod{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedExprCallMethodParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallMethod, const LilyCheckedExprCallMethod *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer,
                          self->params->len,
                          LilyCheckedExprCallMethodParam);
        FREE(Vec, self->params);
    }
}

CONSTRUCTOR(LilyCheckedExprCallRecordParam *,
            LilyCheckedExprCallRecordParam,
            String *name,
            LilyCheckedExpr *value,
            Usize field_index)
{
    LilyCheckedExprCallRecordParam *self =
      lily_malloc(sizeof(LilyCheckedExprCallRecordParam));

    self->name = name;
    self->value = value;
    self->field_index = field_index;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecordParam,
               const LilyCheckedExprCallRecordParam *self)
{
    return format__String("LilyCheckedExprCallRecordParam{{ name = {S}, value "
                          "= {Sr}, field_index = {d} }",
                          self->name,
                          to_string__Debug__LilyCheckedExpr(self->value),
                          self->field_index);
}
#endif

DESTRUCTOR(LilyCheckedExprCallRecordParam, LilyCheckedExprCallRecordParam *self)
{
    FREE(LilyCheckedExpr, self->value);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecord,
               const LilyCheckedExprCallRecord *self)
{
    String *res = from__String("LilyCheckedExprCallRecord{{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedExprCallRecordParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallRecord, const LilyCheckedExprCallRecord *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer,
                          self->params->len,
                          LilyCheckedExprCallRecordParam);
        FREE(Vec, self->params);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecordFieldSingle,
               const LilyCheckedExprCallRecordFieldSingle *self)

{
    return format__String(
      "LilyCheckedExprCallRecordFieldSingle{{ record_access = {Sr}, "
      "record_global_name = {S}, index = {d} }",
      to_string__Debug__LilyCheckedAccessScope(&self->record_access),
      self->record_global_name,
      self->index);
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecordFieldAccess,
               const LilyCheckedExprCallRecordFieldAccess *self)
{
    String *res =
      from__String("LilyCheckedExprCallRecordFieldAccess{ accesses =");

    DEBUG_VEC_STRING(self->accesses, res, LilyCheckedExpr);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallRecordFieldAccess,
           const LilyCheckedExprCallRecordFieldAccess *self)
{
    FREE_BUFFER_ITEMS(
      self->accesses->buffer, self->accesses->len, LilyCheckedExpr);
    FREE(Vec, self->accesses);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallVariant,
               const LilyCheckedExprCallVariant *self)
{
    String *res = from__String("LilyCheckedExprCallVariant{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedExpr);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedExprCallVariant, const LilyCheckedExprCallVariant *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedExpr);
        FREE(Vec, self->params);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprCall, const LilyCheckedExprCall *self)
{
    String *res = NULL;

    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN:
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN:
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS:
        case LILY_CHECKED_EXPR_CALL_KIND_STR_LEN:
            res = format__String(
              "LilyCheckedExprCall{{ kind = {s}, scope = undef, global_name = "
              "NULL",
              to_string__Debug__LilyCheckedExprCallKind(self->kind));

            break;
        default:
            res = format__String(
              "LilyCheckedExprCall{{ kind = {s}, global_name = {S}, scope = "
              "{sa}",
              to_string__Debug__LilyCheckedExprCallKind(self->kind),
              self->global_name,
              to_string__Debug__LilyCheckedAccessScope(&self->scope));
    }

    switch (self->kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN: {
            char *s = format(", cstr_len = {Sr} }",
                             to_string__Debug__LilyCheckedExpr(self->cstr_len));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_ERROR: {
            char *s =
              format(", error = {Sr} }",
                     to_string__Debug__LilyCheckedExprCallError(&self->error));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_FUN: {
            char *s =
              format(", fun = {Sr} }",
                     to_string__Debug__LilyCheckedExprCallFun(&self->fun));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN: {
            char *s = format(", fun_builtin = {Sr} }",
                             to_string__Debug__LilyCheckedExprCallFunBuiltin(
                               &self->fun_builtin));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS: {
            char *s = format(
              ", fun_sys = {Sr} }",
              to_string__Debug__LilyCheckedExprCallFunSys(&self->fun_sys));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM: {
            char *s = format(", fun_param = {d} }", self->fun_param);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_METHOD: {
            char *s = format(
              ", method = {Sr} }",
              to_string__Debug__LilyCheckedExprCallMethod(&self->method));

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
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE: {
            char *s =
              format(", record_field_single = {Sr} }",
                     to_string__Debug__LilyCheckedExprCallRecordFieldSingle(
                       &self->record_field_single));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS: {
            char *s =
              format(", record_field_access = {Sr} }",
                     to_string__Debug__LilyCheckedExprCallRecordFieldAccess(
                       &self->record_field_access));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_STR_LEN: {
            char *s = format(", str_len = {Sr} }",
                             to_string__Debug__LilyCheckedExpr(self->str_len));

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
        case LILY_CHECKED_EXPR_CALL_KIND_ATTRIBUTE:
        case LILY_CHECKED_EXPR_CALL_KIND_CLASS:
        case LILY_CHECKED_EXPR_CALL_KIND_CONSTANT:
        case LILY_CHECKED_EXPR_CALL_KIND_MODULE:
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                   cstr_len,
                   const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExpr, self->cstr_len);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall, error, const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallError, &self->error);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall, fun, const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallFun, &self->fun);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                   fun_builtin,
                   const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallFunBuiltin, &self->fun_builtin);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                   fun_sys,
                   const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallFunSys, &self->fun_sys);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall, method, const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallMethod, &self->method);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall, record, const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallRecord, &self->record);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                   record_field_access,
                   const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExprCallRecordFieldAccess, &self->record_field_access);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCall,
                   str_len,
                   const LilyCheckedExprCall *self)
{
    FREE(LilyCheckedExpr, self->str_len);
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
        case LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN:
            FREE_VARIANT(LilyCheckedExprCall, cstr_len, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_ERROR:
            FREE_VARIANT(LilyCheckedExprCall, error, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_FUN:
            FREE_VARIANT(LilyCheckedExprCall, fun, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN:
            FREE_VARIANT(LilyCheckedExprCall, fun_builtin, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS:
            FREE_VARIANT(LilyCheckedExprCall, fun_sys, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE:
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_METHOD:
            FREE_VARIANT(LilyCheckedExprCall, method, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD:
            FREE_VARIANT(LilyCheckedExprCall, record, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS:
            FREE_VARIANT(LilyCheckedExprCall, record_field_access, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_STR_LEN:
            FREE_VARIANT(LilyCheckedExprCall, str_len, self);
            break;
        case LILY_CHECKED_EXPR_CALL_KIND_VARIANT:
            FREE_VARIANT(LilyCheckedExprCall, variant, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
