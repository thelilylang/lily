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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/scope_container.h>
#include <core/lily/analysis/checked/scope_decls.h>
#include <core/lily/analysis/checked/scope_response.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyCheckedParent LilyCheckedParent;
typedef struct LilyCheckedStmtVariable LilyCheckedStmtVariable;
typedef struct LilyCheckedBodyFunItem LilyCheckedBodyFunItem;

typedef struct LilyCheckedScopeCatch
{
    String *name;             // String* (&)
    const Location *location; // const Location* (&)
    HashMap *raises;          // HashMap<LilyCheckedDataType*>* (&)
} LilyCheckedScopeCatch;

/**
 *
 * @brief Construct LilyCheckedScopeCatch type.
 */
CONSTRUCTOR(LilyCheckedScopeCatch *,
            LilyCheckedScopeCatch,
            String *name,
            const Location *location,
            HashMap *raises);

/**
 *
 * @brief Convert LilyCheckedScopeCatch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeCatch,
               const LilyCheckedScopeCatch *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeCatch type.
 */
inline DESTRUCTOR(LilyCheckedScopeCatch, LilyCheckedScopeCatch *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScope
{
    Usize id;
    HashMap *raises;              // HashMap<LilyCheckedDataType*>*?
    LilyCheckedScopeCatch *catch; // LilyCheckedScopeCatch*?
    HashMap *
      captured_variables; // HashMap<LilyCheckedScopeContainerCapturedVariable*>*?
    HashMap *modules;     // HashMap<LilyCheckedScopeContainerModule*>*?
    HashMap *constants;   // HashMap<LilyCheckedScopeContainerConstant*>*?
    HashMap *enums;       // HashMap<LilyCheckedScopeContainerEnum*>*?
    HashMap *records;     // HashMap<LilyCheckedScopeContainerRecord*>*?
    HashMap *aliases;     // HashMap<LilyCheckedScopeContainerAlias*>*?
    HashMap *errors;      // HashMap<LilyCheckedScopeContainerError*>*?
    HashMap *enums_object; // HashMap<LilyCheckedScopeContainerEnumObject*>*?
    HashMap
      *records_object;  // HashMap<LilyCheckedScopeContainerRecordObject*>*?
    HashMap *classes;   // HashMap<LilyCheckedScopeContainerClass*>*?
    HashMap *traits;    // HashMap<LilyCheckedScopeContainerTrait*>*?
    Vec *funs;          // Vec<LilyCheckedScopeContainerFun*>*?
    HashMap *labels;    // HashMap<LilyCheckedScopeContainerLabel*>*?
    HashMap *variables; // HashMap<LilyCheckedScopeContainerVariable*>*?
    HashMap *params;    // HashMap<LilyCheckedScopeContainerVariable*>*?
    HashMap *generics;  // HashMap<LilyCheckedScopeContainerGeneric*>*?
    Vec *methods;       // Vec<LilyCheckedScopeContainerMethod*>*?
    LilyCheckedParent *parent; // LilyCheckedParent*?
    LilyCheckedScopeDecls decls;
    bool has_return;
} LilyCheckedScope;

/**
 *
 * @brief Construct LilyCheckedScope type.
 * @param parent LilyCheckedAccessScope*?
 */
CONSTRUCTOR(LilyCheckedScope *,
            LilyCheckedScope,
            LilyCheckedParent *parent,
            LilyCheckedScopeDecls decls);

/**
 *
 * @brief Add a captured variable to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_captured_variable__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerCapturedVariable *captured_variable);

/**
 *
 * @brief Add a module to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_module__LilyCheckedScope(LilyCheckedScope *self,
                             LilyCheckedScopeContainerModule *module);

/**
 *
 * @brief Add a constant to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_constant__LilyCheckedScope(LilyCheckedScope *self,
                               LilyCheckedScopeContainerConstant *constant);

/**
 *
 * @brief Add an enum to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_enum__LilyCheckedScope(LilyCheckedScope *self,
                           LilyCheckedScopeContainerEnum *enum_);

/**
 *
 * @brief Add a record to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_record__LilyCheckedScope(LilyCheckedScope *self,
                             LilyCheckedScopeContainerRecord *record);

/**
 *
 *
 * @brief Add an alias to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_alias__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerAlias *alias);

/**
 *
 *
 * @brief Add an error to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_error__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerError *error);

/**
 *
 * @brief Add an enum object to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_enum_object__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerEnumObject *enum_object);

/**
 *
 * @brief Add a record object to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_record_object__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerRecordObject *record_object);

/**
 *
 * @brief Add a class to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_class__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerClass *class);

/**
 *
 * @brief Add a trait to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_trait__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerTrait *trait);

/**
 *
 * @brief Add a fun to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_fun__LilyCheckedScope(LilyCheckedScope *self,
                          LilyCheckedScopeContainerFun *fun);

/**
 *
 * @brief Add a label to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_label__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerLabel *label);

/**
 *
 * @brief Add a variable to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_variable__LilyCheckedScope(LilyCheckedScope *self,
                               LilyCheckedScopeContainerVariable *variable);

/**
 *
 * @brief Add a param to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_param__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerVariable *param);

/**
 *
 * @brief Add a generic param to the scope.
 * @return Return the status 0 for success otherwise 1 for failure.
 */
int
add_generic__LilyCheckedScope(LilyCheckedScope *self,
                              LilyCheckedScopeContainerGeneric *generic);

/**
 *
 * @brief Search a pushed fun in the scope (using for the fun overload).
 * @return LilyCheckedScopeContainerFun*? (&)
 */
LilyCheckedScopeContainerFun *
get_fun_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                           const String *name);

/**
 *
 * @brief Get parent (fun, method or lambda).
 * @return const LilyCheckedScopeDecls*? (&)
 */
const LilyCheckedScopeDecls *
get_parent__LilyCheckedScope(const LilyCheckedScope *self);

/**
 *
 * @brief Search a module in the scope.
 */
LilyCheckedScopeResponse
search_module__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a variable in the scope.
 */
LilyCheckedScopeResponse
search_variable__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a fun in the scope.
 */
LilyCheckedScopeResponse
search_fun__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a constant in the scope.
 */
LilyCheckedScopeResponse
search_constant__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search an error in the scope.
 */
LilyCheckedScopeResponse
search_error__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search an alias in the scope.
 */
LilyCheckedScopeResponse
search_alias__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a record in the scope.
 */
LilyCheckedScopeResponse
search_record__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search an enum in the scope.
 */
LilyCheckedScopeResponse
search_enum__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a generic param in the scope.
 */
LilyCheckedScopeResponse
search_generic__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a class in the scope.
 */
LilyCheckedScopeResponse
search_class__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a record object in the scope.
 */
LilyCheckedScopeResponse
search_record_object__LilyCheckedScope(LilyCheckedScope *self,
                                       const String *name);

/**
 *
 * @brief Search an enum object in the scope.
 */
LilyCheckedScopeResponse
search_enum_object__LilyCheckedScope(LilyCheckedScope *self,
                                     const String *name);

/**
 *
 * @brief Search a trait in the scope.
 */
LilyCheckedScopeResponse
search_trait__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a field in the scope.
 */
LilyCheckedScopeResponse
search_field__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a variant in the scope.
 */
LilyCheckedScopeResponse
search_variant__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search an identifier in the scope.
 */
LilyCheckedScopeResponse
search_identifier__LilyCheckedScope(LilyCheckedScope *self, const String *name);

/**
 *
 * @brief Search a custom type in the scope.
 */
LilyCheckedScopeResponse
search_custom_type__LilyCheckedScope(LilyCheckedScope *self,
                                     const String *name);

/**
 *
 * @brief Get scope from an id.
 */
LilyCheckedScope *
get_scope_from_id__LilyCheckedScope(LilyCheckedScope *self, Usize id);

/**
 *
 * @brief Get scope from an id.
 * @return LilyCheckedScope*?
 */
LilyCheckedScope *
safe_get_scope_from_id__LilyCheckedScope(LilyCheckedScope *self, Usize id);

/**
 *
 * @brief Get response from scope id and kind of response.
 */
LilyCheckedScopeResponse
search_from_scope_id__LilyCheckedScope(
  LilyCheckedScope *self,
  Usize id,
  const String *name,
  enum LilyCheckedScopeResponseKind res_kind);

/**
 *
 * @brief Try to get the current fun declaration (include lambda) in the scope.
 */
LilyCheckedScopeDecls *
get_current_fun__LilyCheckedScope(LilyCheckedScope *self);

/**
 *
 * @brief Try to get the current method declaration in the scope.
 */
LilyCheckedDecl *
get_current_method__LilyCheckedScope(LilyCheckedScope *self);

/**
 *
 * @brief Try to get the current object declaration in the scope.
 */
LilyCheckedDecl *
get_current_object__LilyCheckedScope(LilyCheckedScope *self);

/**
 *
 * @brief Get decl from id and position
 */
LilyCheckedDecl *
get_decl_from_id__LilyCheckedScope(LilyCheckedScope *self, Usize id, Usize pos);

/**
 *
 * @brief Get fun item from id and position.
 */
LilyCheckedBodyFunItem *
get_variable_from_id__LilyCheckedScope(LilyCheckedScope *self,
                                       Usize id,
                                       String *name);

/**
 *
 * @brief Set a catch name to the scope.
 */
void
set_catch_name__LilyCheckedScope(LilyCheckedScope *self,
                                 String *catch_name,
                                 const Location *location,
                                 HashMap *raises);

/**
 *
 * @brief Convert LilyCheckedScope in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedScope, const LilyCheckedScope *self);
#endif

/**
 *
 * @brief Free LilyCheckedScope type.
 */
DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_H
