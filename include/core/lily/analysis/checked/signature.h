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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SIGNATURE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SIGNATURE_H

#include <base/hash_map.h>
#include <base/macros.h>
#include <base/ordered_hash_map.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/data_type.h>

typedef struct LilyCheckedSignatureFun
{
    String *global_name; // String* (&)
    String *ser_global_name;
    // [params, return_data_type]
    // NOTE: the return data type must be drop.
    Vec *types; // Vec<LilyCheckedDataType* (&)...LilyCheckedDataType*>*
    HashMap *generic_params; // HashMap<LilyCheckedDataType*>*?
} LilyCheckedSignatureFun;

/**
 *
 * @brief Construct LilyCheckedSignatureFun type.
 * @param global_name String* (&)
 */
CONSTRUCTOR(LilyCheckedSignatureFun *,
            LilyCheckedSignatureFun,
            String *global_name,
            Vec *types,
            HashMap *generic_params);

/**
 *
 * @brief Try to add a new signature to signatures.
 * @param types Vec<LilyCheckedDataType* (&)>*
 * @param generic_params HashMap<LilyCheckedDataType* (&)>*
 * @param signatures Vec<LilyCheckedSignatureFun*>*
 */
int
add_signature__LilyCheckedSignatureFun(String *global_name,
                                       Vec *types,
                                       HashMap *generic_params,
                                       Vec *signatures);

/**
 *
 * @brief Reload global name.
 */
void
reload_global_name__LilyCheckedSignatureFun(LilyCheckedSignatureFun *self);

/**
 *
 * @brief Check if the signature contains only known data type (without generic
 * and compiler defined data type).
 */
bool
has_only_known_dt__LilyCheckedSignatureFun(const LilyCheckedSignatureFun *self);

/**
 *
 * @brief Get a user defined signature (without compiler defined type(s) or
 * generic type) of the signatures.
 * @return LilyCheckedSignatureFun*? (&)
 */
LilyCheckedSignatureFun *
get_user_defined_signature__LilyCheckedSignatureFun(Vec *signatures);

/**
 *
 * @brief Convert LilyCheckedSignatureFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedSignatureFun,
               const LilyCheckedSignatureFun *self);
#endif

/**
 *
 * @brief Free LilyCheckedSignatureFun type.
 */
DESTRUCTOR(LilyCheckedSignatureFun, LilyCheckedSignatureFun *self);

typedef struct LilyCheckedSignatureType
{
    String *global_name; // String* (&)
    String *ser_global_name;
    OrderedHashMap *generic_params; // OrderedHashMap<LilyCheckedDataType*>*?
} LilyCheckedSignatureType;

/**
 *
 * @brief Construct LilyCheckedSignatureType type.
 */
CONSTRUCTOR(LilyCheckedSignatureType *,
            LilyCheckedSignatureType,
            String *global_name,
            OrderedHashMap *generic_params);

/**
 *
 * @brief Try to add a signature to signatures.
 * @return Return 1 if is failing to push a new signature (already presents)
 * otherwise return 0.
 * @param generic_params OrderedHashMap<LilyCheckedDataType* (&)>*?
 * @param signatures Vec<LilyCheckedSignatureType*>*
 */
int
add_signature__LilyCheckedSignatureType(String *global_name,
                                        OrderedHashMap *generic_params,
                                        Vec *signatures);

/**
 *
 * @brief Check if the signature contains only known data type (without
 * generic).
 */
bool
has_only_known_dt__LilyCheckedSignatureType(
  const LilyCheckedSignatureType *self);

/**
 *
 * @brief Get a user defined signature (without compiler defined type(s) or
 * generic type) of the signatures.
 * @return LilyCheckedSignatureType*? (&)
 */
LilyCheckedSignatureType *
get_user_defined_signature__LilyCheckedSignatureType(Vec *signatures);

/**
 *
 * @brief Get a signature with generic params.
 */
LilyCheckedSignatureType *
get_signature__LilyCheckedSignatureType(Vec *signatures,
                                        OrderedHashMap *generic_params);

/**
 *
 * @brief Convert LilyCheckedSignatureType in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedSignatureType,
               const LilyCheckedSignatureType *self);
#endif

/**
 *
 * @brief Free LilyCheckedSignature type.
 */
DESTRUCTOR(LilyCheckedSignatureType, LilyCheckedSignatureType *self);

typedef struct LilyCheckedSignatureVariant
{
    String *global_name; // String* (&)
    String *ser_global_name;
    LilyCheckedDataType *resolve_dt; // LilyCheckedDataType*?
} LilyCheckedSignatureVariant;

/**
 *
 * @brief Construct LilyCheckedSignatureVariant type.
 */
CONSTRUCTOR(LilyCheckedSignatureVariant *,
            LilyCheckedSignatureVariant,
            String *global_name,
            LilyCheckedDataType *resolve_dt);

/**
 *
 * @brief Try to add a signature to signatures.
 * @param signatures Vec<LilyCheckedSignatureVariant*>*
 */
inline void
add_signature__LilyCheckedSignatureVariant(String *global_name,
                                           LilyCheckedDataType *resolve_dt,
                                           Vec *signatures)
{
    push__Vec(signatures,
              NEW(LilyCheckedSignatureVariant, global_name, resolve_dt));
}

/**
 *
 * @brief Convert LilyCheckedSignatureVariant in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedSignatureVariant,
               const LilyCheckedSignatureVariant *self);
#endif

/**
 *
 * @brief Free LilyCheckedSignatureVariant type.
 */
DESTRUCTOR(LilyCheckedSignatureVariant, LilyCheckedSignatureVariant *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SIGNATURE_H
