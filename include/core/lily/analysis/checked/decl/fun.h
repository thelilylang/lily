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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_FUN_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_FUN_H

#include <base/string.h>

#include <core/lily/analysis/checked/access.h>
#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/decl/method.h>
#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/generic_param.h>
#include <core/lily/analysis/checked/scope.h>
#include <core/lily/analysis/checked/signature.h>
#include <core/lily/shared/visibility.h>

#include <core/shared/location.h>

enum LilyCheckedDeclFunParamKind
{
    LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT,
    LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyCheckedDeclFunParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParamKind,
               enum LilyCheckedDeclFunParamKind self);
#endif

typedef struct LilyCheckedDeclFunParam
{
    String *name;                   // String* (&)
    LilyCheckedDataType *data_type; // LilyCheckedDataType*?
    enum LilyCheckedDeclFunParamKind kind;
    const Location *location;
    bool is_moved;
    union
    {
        LilyCheckedExpr *default_;
    };
} LilyCheckedDeclFunParam;

/**
 *
 * @brief Construct LilyCheckedDeclFunParam type
 * (LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    default,
                    String *name,
                    LilyCheckedDataType *data_type,
                    const Location *location,
                    LilyCheckedExpr *default_);

/**
 *
 * @brief Construct LilyCheckedDeclFunParam type
 * (LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    normal,
                    String *name,
                    LilyCheckedDataType *data_type,
                    const Location *location);

/**
 *
 * @brief Convert LilyCheckedDeclFunParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParam,
               const LilyCheckedDeclFunParam *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclFunParam type.
 */
DESTRUCTOR(LilyCheckedDeclFunParam, LilyCheckedDeclFunParam *self);

typedef struct LilyCheckedDeclFun
{
    String *name; // String* (&)
    String *global_name;
    Vec *generic_params;                   // Vec<LilyCheckedGenericParam*>*?
    Vec *params;                           // Vec<LilyCheckedDeclFunParam*>*?
    LilyCheckedDataType *return_data_type; // LilyCheckedDataType*?
    Vec *body;                             // Vec<LilyCheckedBodyFunItem*>*
    LilyCheckedScope *scope;
    LilyCheckedAccessFun *access;
    Vec *used_compiler_generic; // Vec<String*>*
    Vec *signatures;            // Vec<LilyCheckedSignatureFun*>*
    Vec *fun_deps;              // Vec<LilyCheckedDecl* (&)>*
    Vec *unlock_data_type;      // Vec<LilyCheckedDataType* (&)>*
    HashMap *raises;            // HashMap<LilyCheckedDataType*>*
    enum LilyVisibility visibility;
    bool is_async;
    bool is_operator;
    bool can_raise;
    bool can_inline;
    bool is_main;
    bool is_recursive;
    bool is_checked;
    bool has_return;
    bool default_return_dt_is_set;
} LilyCheckedDeclFun;

/**
 *
 * @brief Construct LilyCheckedDeclFun type.
 */
inline CONSTRUCTOR(LilyCheckedDeclFun,
                   LilyCheckedDeclFun,
                   String *name,
                   String *global_name,
                   Vec *generic_params,
                   Vec *params,
                   LilyAstDataType *default_return_data_type,
                   LilyCheckedDataType *return_data_type,
                   Vec *body,
                   LilyCheckedScope *scope,
                   enum LilyVisibility visibility,
                   bool is_async,
                   bool is_operator)
{
    return (LilyCheckedDeclFun){ .name = name,
                                 .global_name = global_name,
                                 .generic_params = generic_params,
                                 .params = params,
                                 .return_data_type = return_data_type,
                                 .body = body,
                                 .scope = scope,
                                 .access = NULL,
                                 .used_compiler_generic = NEW(Vec),
                                 .signatures = NEW(Vec),
                                 .fun_deps = NEW(Vec),
                                 .unlock_data_type = NEW(Vec),
                                 .raises = NEW(HashMap),
                                 .visibility = visibility,
                                 .is_async = is_async,
                                 .is_operator = is_operator,
                                 .can_raise = false,
                                 .can_inline = false,
                                 .is_main = false,
                                 .is_recursive = false,
                                 .is_checked = false,
                                 .has_return = false,
                                 .default_return_dt_is_set =
                                   default_return_data_type ? true : false };
}

/**
 *
 * @brief Convert LilyCheckedDeclFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclFun, const LilyCheckedDeclFun *self);
#endif

/**
 *
 * @brief Check if contains compiler defined data type(s).
 */
bool
contains_uncertain_dt__LilyCheckedDeclFun(const LilyCheckedDeclFun *self);

/**
 *
 * @brief Add a signature and check if is not duplicate.
 * @return Return 1 if is failing otherwise return 0.
 * @param types Vec<LilyCheckedDataType* (&)>*
 * @param generic_params HashMap<LilyCheckedDataType* (&)>*
 */
inline int
add_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                  Vec *types,
                                  HashMap *generic_params)
{
    return add_signature__LilyCheckedSignatureFun(
      self->global_name, types, generic_params, self->signatures);
}

/**
 *
 * @brief Get signature from the global_name and the fun types.
 */
LilyCheckedSignatureFun *
get_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                  String *global_name,
                                  Vec *fun_types);

/**
 *
 * @brief Get serialized global name of pushed signature.
 */
String *
get_ser_global_name_of_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                                     Vec *types);

/**
 *
 * @brief Get if of the parametter from the compiler generic name.
 */
Usize
get_id_of_param_from_compiler_generic__LilyCheckedDeclFun(
  const LilyCheckedDeclFun *self,
  const String *compiler_generic_name);

/**
 *
 * @brief Add fun dependency to the fun.
 */
void
add_fun_dep__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                LilyCheckedDecl *fun_dep);

/**
 *
 * @brief Get the original (first) signature of the function.
 * @return LilyCheckedSignatureFun*? (&)
 */
LilyCheckedSignatureFun *
get_original_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self);

/**
 *
 * @brief Lock all data types contains in the `unlock_data_type` field.
 */
void
lock_data_types__LilyCheckedDeclFun(const LilyCheckedDeclFun *self);

/**
 *
 * @brief Collect raises (from fun call, method call or lambda call).
 * @param raises const HashMap<LilyCheckedDataType*>* (&)
 */
void
collect_raises__LilyCheckedDeclFun(const LilyCheckedDeclFun *self,
                                   LilyCheckedScope *scope,
                                   HashMap *raises,
                                   bool in_try);

/**
 *
 * @brief Add raise to the raises field.
 * @param raise LilyCheckedDataType* (&)
 * @param in_try If this is true, we don't insert `raise` in the function's
 * `raises` field.
 */
void
add_raise__LilyCheckedDeclFun(const LilyCheckedDeclFun *self,
                              LilyCheckedScope *scope,
                              LilyCheckedDataType *raise,
                              bool in_try);

/**
 *
 * @brief Free LilyCheckedDeclFun type.
 */
DESTRUCTOR(LilyCheckedDeclFun, const LilyCheckedDeclFun *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_FUN_H
