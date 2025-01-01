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
#include <base/new.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/global_name.h>
#include <core/lily/analysis/checked/signature.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(LilyCheckedSignatureFun *,
            LilyCheckedSignatureFun,
            String *global_name,
            Vec *types,
            HashMap *generic_params)
{
    LilyCheckedSignatureFun *self =
      lily_malloc(sizeof(LilyCheckedSignatureFun));

    self->global_name = global_name;
    self->ser_global_name = clone__String(global_name);
    self->types = types;
    self->generic_params = generic_params;

    generate_global_fun_name_with_vec__LilyCheckedGlobalName(
      self->ser_global_name, self->types);

    return self;
}

int
add_signature__LilyCheckedSignatureFun(String *global_name,
                                       Vec *types,
                                       HashMap *generic_params,
                                       Vec *signatures)
{
    ASSERT(types->len != 0);

    for (Usize i = 0; i < signatures->len; ++i) {
        Vec *pushed_types =
          CAST(LilyCheckedSignatureFun *, get__Vec(signatures, i))->types;

        ASSERT(types->len == pushed_types->len);

        bool is_match = true;

        for (Usize j = 0; j < pushed_types->len; ++j) {
            LilyCheckedDataType *types_dt = get__Vec(types, j);
            LilyCheckedDataType *pushed_types_dt = get__Vec(pushed_types, j);

            if (!eq__LilyCheckedDataType(types_dt, pushed_types_dt) ||
                types_dt->kind != pushed_types_dt->kind) {
                is_match = false;
                break;
            }
        }

        if (is_match) {
            return 1;
        }
    }

    push__Vec(signatures,
              NEW(LilyCheckedSignatureFun, global_name, types, generic_params));

    return 0;
}

void
reload_global_name__LilyCheckedSignatureFun(LilyCheckedSignatureFun *self)
{
    String *reload_ser_global_name = NEW(String);

    generate_global_fun_name_with_vec__LilyCheckedGlobalName(
      reload_ser_global_name, self->types);
    FREE(String, self->ser_global_name);
    self->ser_global_name = reload_ser_global_name;
}

bool
has_only_known_dt__LilyCheckedSignatureFun(const LilyCheckedSignatureFun *self)
{
    for (Usize i = 0; i < self->types->len; ++i) {
        LilyCheckedDataType *type = get__Vec(self->types, i);

        if (is_compiler_defined_and_known_dt__LilyCheckedDataType(type)) {
            return false;
        } else if (type->kind == LILY_CHECKED_DATA_TYPE_KIND_CUSTOM) {
            if (type->custom.kind ==
                LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC) {
                return false;
            }
        }
    }

    return true;
}

LilyCheckedSignatureFun *
get_user_defined_signature__LilyCheckedSignatureFun(Vec *signatures)
{
    for (Usize i = 0; i < signatures->len; ++i) {
        LilyCheckedSignatureFun *signature = get__Vec(signatures, i);

        if (has_only_known_dt__LilyCheckedSignatureFun(signature)) {
            return signature;
        }
    }

    return NULL;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedSignatureFun,
               const LilyCheckedSignatureFun *self)
{
    String *res = format__String("LilyCheckedSignatureFun{{ global_name = {S}, "
                                 "ser_global_name = {S}, types =",
                                 self->global_name,
                                 self->ser_global_name);

    DEBUG_VEC_STRING(self->types, res, LilyCheckedDataType);

    push_str__String(res, ", generic_params =");

    if (self->generic_params) {
        DEBUG_HASH_MAP_STRING(self->generic_params, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedSignatureFun, LilyCheckedSignatureFun *self)
{
    FREE(String, self->ser_global_name);
    FREE(LilyCheckedDataType, last__Vec(self->types));
    FREE(Vec, self->types);

    if (self->generic_params) {
        FREE_HASHMAP_VALUES(self->generic_params, LilyCheckedDataType);
        FREE(HashMap, self->generic_params);
    }

    lily_free(self);
}

CONSTRUCTOR(LilyCheckedSignatureType *,
            LilyCheckedSignatureType,
            String *global_name,
            OrderedHashMap *generic_params)
{
    LilyCheckedSignatureType *self =
      lily_malloc(sizeof(LilyCheckedSignatureType));

    self->global_name = global_name;
    self->generic_params = generic_params;
    self->ser_global_name = clone__String(self->global_name);

    if (self->generic_params) {
        generate_global_type_name_with_ordered_hash_map__LilyCheckedGlobalName(
          self->ser_global_name, self->generic_params);
    }

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedSignatureType,
               const LilyCheckedSignatureType *self)
{
    String *res =
      format__String("LilyCheckedSignatureType{{ global_name = {S}, "
                     "ser_global_name = {S}, generic_params =",
                     self->global_name,
                     self->ser_global_name);

    if (self->generic_params) {
        DEBUG_ORD_HASH_MAP_STRING(
          self->generic_params, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

int
add_signature__LilyCheckedSignatureType(String *global_name,
                                        OrderedHashMap *generic_params,
                                        Vec *signatures)
{
    for (Usize i = 0; i < signatures->len; ++i) {
        LilyCheckedSignatureType *signature = get__Vec(signatures, i);

        if (!strcmp(global_name->buffer, signature->global_name->buffer)) {
            ASSERT((generic_params && signature->generic_params) ||
                   (!generic_params && !signature->generic_params));
            if (generic_params) {
                ASSERT(generic_params->len == signature->generic_params->len);

                OrderedHashMapIter2 iter = NEW(OrderedHashMapIter2,
                                               generic_params,
                                               signature->generic_params);
                LilyCheckedDataType **current = NULL;
                bool is_match = true;

                while ((
                  current =
                    (LilyCheckedDataType **)next__OrderedHashMapIter2(&iter))) {
                    if (!eq__LilyCheckedDataType(current[0], current[1])) {
                        is_match = false;
                        break;
                    }
                }

                if (is_match) {
                    return 1;
                }
            } else {
                return 1;
            }
        }
    }

    push__Vec(signatures,
              NEW(LilyCheckedSignatureType, global_name, generic_params));

    return 0;
}

bool
has_only_known_dt__LilyCheckedSignatureType(
  const LilyCheckedSignatureType *self)
{
    if (self->generic_params) {
        OrderedHashMapIter iter = NEW(OrderedHashMapIter, self->generic_params);
        LilyCheckedDataType *current = NULL;

        while ((current = next__OrderedHashMapIter(&iter))) {
            switch (current->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                    switch (current->custom.kind) {
                        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC:
                            return false;
                        default:
                            continue;
                    }
                default:
                    continue;
            }
        }
    }

    return true;
}

LilyCheckedSignatureType *
get_user_defined_signature__LilyCheckedSignatureType(Vec *signatures)
{
    for (Usize i = 0; i < signatures->len; ++i) {
        LilyCheckedSignatureType *signature = get__Vec(signatures, i);

        if (has_only_known_dt__LilyCheckedSignatureType(signature)) {
            return signature;
        }
    }

    return NULL;
}

LilyCheckedSignatureType *
get_signature__LilyCheckedSignatureType(Vec *signatures,
                                        OrderedHashMap *generic_params)
{
    ASSERT(signatures);
    ASSERT(signatures->len > 0);

    if (generic_params) {
        for (Usize i = 0; i < signatures->len; ++i) {
            LilyCheckedSignatureType *signature = get__Vec(signatures, i);
            OrderedHashMapIter2 iter = NEW(
              OrderedHashMapIter2, generic_params, signature->generic_params);
            LilyCheckedDataType **current = NULL;

            while ((current = (LilyCheckedDataType **)next__OrderedHashMapIter2(
                      &iter))) {
                if (!eq__LilyCheckedDataType(current[0], current[1])) {
                    return signature;
                }
            }
        }

        return NULL;
    }

    return last__Vec(signatures);
}

DESTRUCTOR(LilyCheckedSignatureType, LilyCheckedSignatureType *self)
{
    FREE(String, self->ser_global_name);

    if (self->generic_params) {
        FREE_ORD_HASHMAP_VALUES(self->generic_params, LilyCheckedDataType);
        FREE(OrderedHashMap, self->generic_params);
    }

    lily_free(self);
}

CONSTRUCTOR(LilyCheckedSignatureVariant *,
            LilyCheckedSignatureVariant,
            String *global_name,
            LilyCheckedDataType *resolve_dt)
{
    LilyCheckedSignatureVariant *self =
      lily_malloc(sizeof(LilyCheckedSignatureVariant));

    self->global_name = global_name;
    self->ser_global_name = clone__String(self->global_name);
    self->resolve_dt = resolve_dt;

    if (self->resolve_dt) {
        generate_global_variant_name__LilyCheckedGlobalName(
          self->ser_global_name, self->resolve_dt);
    }

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedSignatureVariant,
               const LilyCheckedSignatureVariant *self)
{
    if (self->resolve_dt) {
        return format__String(
          "LilyCheckedSignatureVariant{{ global_name = {S}, ser_global_name = "
          "{S}, "
          "resolve_dt = {Sr} }",
          self->global_name,
          self->ser_global_name,
          to_string__Debug__LilyCheckedDataType(self->resolve_dt));
    }

    return format__String(
      "LilyCheckedSignatureVariant{{ global_name = {S}, ser_global_name = {S}, "
      "resolve_dt = NULL }",
      self->global_name,
      self->ser_global_name);
}
#endif

DESTRUCTOR(LilyCheckedSignatureVariant, LilyCheckedSignatureVariant *self)
{
    FREE(String, self->ser_global_name);

    if (self->resolve_dt) {
        FREE(LilyCheckedDataType, self->resolve_dt);
    }

    lily_free(self);
}
