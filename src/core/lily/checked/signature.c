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
#include <base/new.h>

#include <core/lily/checked/data_type.h>
#include <core/lily/checked/global_name.h>
#include <core/lily/checked/signature.h>

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
contains_known_dt__LilyCheckedSignatureFun(const LilyCheckedSignatureFun *self)
{
    for (Usize i = 0; i < self->types->len; ++i) {
        LilyCheckedDataType *type = get__Vec(self->types, i);

        if (is_compiler_defined_and_known_dt__LilyCheckedDataType(type)) {
            return true;
        } else if (type->kind == LILY_CHECKED_DATA_TYPE_KIND_CUSTOM) {
            if (type->custom.kind ==
                LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC) {
                return true;
            }
        }
    }

    return false;
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
        DEBUG_STRING_HASH_MAP(self->generic_params, res, LilyCheckedDataType);
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
    FREE(Vec, self->types);
    FREE_HASHMAP_VALUES(self->generic_params, LilyCheckedDataType);
    FREE(HashMap, self->generic_params);
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

    DEBUG_STRING_ORD_HASH_MAP(self->generic_params, res, LilyCheckedDataType);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedSignatureType, LilyCheckedSignatureType *self)
{
    FREE(String, self->ser_global_name);
    FREE_ORD_HASHMAP_VALUES(self->generic_params, LilyCheckedDataType);
    FREE(OrderedHashMap, self->generic_params);
    lily_free(self);
}