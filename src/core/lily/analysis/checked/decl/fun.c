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
#include <base/assert.h>

#include <core/lily/analysis/checked/decl.h>
#include <core/lily/analysis/checked/decl/fun.h>
#include <core/lily/analysis/checked/parent.h>
#include <core/lily/analysis/checked/signature.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyCheckedDeclFunParam type (LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                          default,
                          LilyCheckedDeclFunParam *self);

// Free LilyCheckedDeclFunParam type (LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                          normal,
                          LilyCheckedDeclFunParam *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParamKind,
               enum LilyCheckedDeclFunParamKind self)
{
    switch (self) {
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT:
            return "LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT";
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL:
            return "LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    default,
                    String *name,
                    LilyCheckedDataType *data_type,
                    const Location *location,
                    LilyCheckedExpr *default_)
{
    LilyCheckedDeclFunParam *self =
      lily_malloc(sizeof(LilyCheckedDeclFunParam));

    self->name = name;
    self->data_type = data_type;
    self->kind = LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT;
    self->location = location;
    self->default_ = default_;
    self->is_moved = false;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    normal,
                    String *name,
                    LilyCheckedDataType *data_type,
                    const Location *location)
{
    LilyCheckedDeclFunParam *self =
      lily_malloc(sizeof(LilyCheckedDeclFunParam));

    self->name = name;
    self->data_type = data_type;
    self->kind = LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL;
    self->location = location;
    self->is_moved = false;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParam,
               const LilyCheckedDeclFunParam *self)
{
    String *res = format__String(
      "LilyCheckedDeclFunParam{{ name = {S}, kind = {s}, location = "
      "{sa}, is_moved = {b}, data_type =",
      self->name,
      to_string__Debug__LilyCheckedDeclFunParamKind(self->kind),
      to_string__Debug__Location(self->location),
      self->is_moved);

    if (self->data_type) {
        String *s = to_string__Debug__LilyCheckedDataType(self->data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, " NULL");
    }

    switch (self->kind) {
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT: {
            String *s = to_string__Debug__LilyCheckedExpr(self->default_);

            push_str__String(res, ", expr = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        default:
            break;
    }

    push_str__String(res, " }");

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                   default,
                   LilyCheckedDeclFunParam *self)
{
    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    FREE(LilyCheckedExpr, self->default_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                   normal,
                   LilyCheckedDeclFunParam *self)
{
    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    lily_free(self);
}

DESTRUCTOR(LilyCheckedDeclFunParam, LilyCheckedDeclFunParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT:
            FREE_VARIANT(LilyCheckedDeclFunParam, default, self);
            break;
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedDeclFunParam, normal, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclFun, const LilyCheckedDeclFun *self)
{
    String *res = format__String(
      "LilyCheckedDeclFun{{ name = {S}, global_name = {S}, generic_params = ",
      self->name,
      self->global_name);

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, LilyCheckedGenericParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedDeclFunParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", return_data_type = ");

    if (self->return_data_type) {
        String *s =
          to_string__Debug__LilyCheckedDataType(self->return_data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyCheckedBodyFunItem);

    {
        char *s = format(", scope = {Sr}, access = NULL",
                         to_string__Debug__LilyCheckedScope(self->scope));

        PUSH_STR_AND_FREE(res, s);
    }

    push_str__String(res, ", used_compiler_generic = { ");

    for (Usize i = 0; i < self->used_compiler_generic->len; ++i) {
        append__String(res, get__Vec(self->used_compiler_generic, i));

        if (i + 1 != self->used_compiler_generic->len) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, " }");

    push_str__String(res, ", signatures =");
    DEBUG_VEC_STRING(self->signatures, res, LilyCheckedSignatureFun);

    push_str__String(res, ", fun_deps =");
    DEBUG_VEC_STRING(self->fun_deps, res, LilyCheckedDecl);

    push_str__String(res, ", unlock_data_type =");
    DEBUG_VEC_STRING(self->unlock_data_type, res, LilyCheckedDataType);

    push_str__String(res, ", raises =");
    DEBUG_HASH_MAP_STRING(self->raises, res, LilyCheckedDataType);

    {
        char *s = format(", visibility = {s}, is_async = {b}, is_operator = "
                         "{b}, can_raise = {b}, can_inline = {b}, is_main = "
                         "{b}, is_recursive = {b}, is_checked = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_async,
                         self->is_operator,
                         self->can_raise,
                         self->can_inline,
                         self->is_main,
                         self->is_recursive,
                         self->is_checked);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

bool
contains_uncertain_dt__LilyCheckedDeclFun(const LilyCheckedDeclFun *self)
{
    if (self->params) {
        for (Usize i = 0; i < self->params->len; ++i) {
            LilyCheckedDeclFunParam *param = get__Vec(self->params, i);

            switch (param->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                    switch (param->data_type->custom.kind) {
                        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC:
                            return true;
                        default:
                            break;
                    }
                default:
                    if (is_compiler_defined_and_known_dt__LilyCheckedDataType(
                          param->data_type)) {
                        return true;
                    }
            }
        }
    }

    if (self->return_data_type) {
        switch (self->return_data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                switch (self->return_data_type->custom.kind) {
                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC:
                        return true;
                    default:
                        return false;
                }
            default:
                return is_compiler_defined_and_known_dt__LilyCheckedDataType(
                  self->return_data_type);
        }
    }

    return false;
}

LilyCheckedSignatureFun *
get_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                  String *global_name,
                                  Vec *fun_types)
{
    for (Usize i = 0; i < self->signatures->len; ++i) {
        LilyCheckedSignatureFun *signature = get__Vec(self->signatures, i);

        if (strcmp(signature->global_name->buffer, global_name->buffer) &&
            fun_types->len == signature->types->len) {
            continue;
        }

        bool is_match = true;

        for (Usize j = 0; j < fun_types->len; ++j) {
            if (!eq__LilyCheckedDataType(get__Vec(fun_types, j),
                                         get__Vec(signature->types, j))) {
                is_match = false;
                break;
            }
        }

        if (is_match) {
            return signature;
        }
    }

    return NULL;
}

String *
get_ser_global_name_of_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                                     Vec *types)
{
    LilyCheckedSignatureFun *signature =
      get_signature__LilyCheckedDeclFun(self, self->global_name, types);

    ASSERT(signature);

    return signature->ser_global_name;
}

Usize
get_id_of_param_from_compiler_generic__LilyCheckedDeclFun(
  const LilyCheckedDeclFun *self,
  const String *compiler_generic_name)
{
    for (Usize i = 0; i < self->params->len; ++i) {
        LilyCheckedDataType *data_type =
          CAST(LilyCheckedDeclFunParam *, get__Vec(self->params, i))->data_type;

        switch (data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
                if (!strcmp(data_type->compiler_generic.name->buffer,
                            compiler_generic_name->buffer)) {
                    return i;
                }

                break;
            default:
                break;
        }
    }

    UNREACHABLE("this function have a bug: "
                "`get_id_of_param_from_compiler_generic__LilyCheckedDeclFun`");
}

void
add_fun_dep__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                LilyCheckedDecl *fun_dep)
{
    if (!strcmp(self->global_name->buffer, fun_dep->fun.global_name->buffer)) {
        return;
    }

    for (Usize i = 0; i < self->fun_deps->len; ++i) {
        if (!strcmp(CAST(LilyCheckedDecl *, get__Vec(self->fun_deps, i))
                      ->fun.global_name->buffer,
                    fun_dep->fun.global_name->buffer)) {
            return;
        }
    }

    push__Vec(self->fun_deps, fun_dep);
}

LilyCheckedSignatureFun *
get_original_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self)
{
    if (self->signatures->len == 0) {
        return NULL;
    }

    return get__Vec(self->signatures, 0);
}

void
lock_data_types__LilyCheckedDeclFun(const LilyCheckedDeclFun *self)
{
    for (Usize i = 0; i < self->unlock_data_type->len; ++i) {
        CAST(LilyCheckedDataType *, get__Vec(self->unlock_data_type, i))
          ->is_lock = true;
    }
}

void
collect_raises__LilyCheckedDeclFun(const LilyCheckedDeclFun *self,
                                   LilyCheckedScope *scope,
                                   HashMap *raises,
                                   bool in_try)
{
    HashMapIter iter = NEW(HashMapIter, raises);
    LilyCheckedDataType *current = NULL;

    while ((current = next__HashMapIter(&iter))) {
        add_raise__LilyCheckedDeclFun(self, scope, current, in_try);
    }
}

void
add_raise__LilyCheckedDeclFun(const LilyCheckedDeclFun *self,
                              LilyCheckedScope *scope,
                              LilyCheckedDataType *raise,
                              bool in_try)
{
    ASSERT(raise->kind == LILY_CHECKED_DATA_TYPE_KIND_CUSTOM
             ? raise->custom.kind == LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ERROR
             : false);

    if (!scope->raises) {
        scope->raises = NEW(HashMap);
    }

    LilyCheckedDataType *match =
      get__HashMap(self->raises, raise->custom.global_name->buffer);

    if (!match) {
        if (!in_try) {
            insert__HashMap(self->raises,
                            raise->custom.global_name->buffer,
                            ref__LilyCheckedDataType(raise));
        }

        insert__HashMap(scope->raises,
                        raise->custom.global_name->buffer,
                        ref__LilyCheckedDataType(raise));
    } else {
        LilyCheckedDataType *match =
          get__HashMap(scope->raises, raise->custom.global_name->buffer);

        if (!match) {
            insert__HashMap(scope->raises,
                            raise->custom.global_name->buffer,
                            ref__LilyCheckedDataType(raise));
        }
    }
}

DESTRUCTOR(LilyCheckedDeclFun, const LilyCheckedDeclFun *self)
{
    FREE(String, self->global_name);

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(self->generic_params->buffer,
                          self->generic_params->len,
                          LilyCheckedGenericParam);
        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedDeclFunParam);
        FREE(Vec, self->params);
    }

    if (self->return_data_type) {
        FREE(LilyCheckedDataType, self->return_data_type);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body);

    FREE(LilyCheckedScope, self->scope->parent->scope);
    FREE(LilyCheckedScope, self->scope);
    FREE(LilyCheckedAccessFun, self->access);

    FREE_BUFFER_ITEMS(self->used_compiler_generic->buffer,
                      self->used_compiler_generic->len,
                      String);
    FREE(Vec, self->used_compiler_generic);

    FREE_BUFFER_ITEMS(
      self->signatures->buffer, self->signatures->len, LilyCheckedSignatureFun);
    FREE(Vec, self->signatures);

    FREE(Vec, self->fun_deps);
    FREE(Vec, self->unlock_data_type);

    FREE_HASHMAP_VALUES(self->raises, LilyCheckedDataType);
    FREE(HashMap, self->raises);
}
