/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#include <base/hash_map.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/symbol.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyCheckedDeclModule LilyCheckedDeclModule;

enum LilyCheckedScopeAddStatus
{
    LILY_CHECKED_SCOPE_ADD_STATUS_SUCCESS,
    LILY_CHECKED_SCOPE_ADD_STATUS_EXPECTED_PASCAL_CASE,
    LILY_CHECKED_SCOPE_ADD_STATUS_EXPECTED_SNAKE_CASE,
    LILY_CHECKED_SCOPE_ADD_STATUS_EXPECTED_UPPER_SNAKE_CASE,
    LILY_CHECKED_SCOPE_ADD_STATUS_DUPLICATE
};

enum LilyCheckedSearchStatusKind
{
    LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_PRIVATE_SYMBOL,
    LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_SYMBOL_NOT_FOUND,
    LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_SUCCESS
};

typedef struct LilyCheckedSearchStatus
{
    enum LilyCheckedSearchStatusKind kind;
    LilyCheckedSymbol *symbol; // LilyCheckedSymbol*? (&)
} LilyCheckedSearchStatus;

/**
 *
 * @brief Construct LilyCheckedSearchStatus type
 * (LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_PRIVATE_SYMBOL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSearchStatus,
                           LilyCheckedSearchStatus,
                           private_symbol)
{
    return (LilyCheckedSearchStatus){
        .kind = LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_PRIVATE_SYMBOL,
        .symbol = NULL
    };
}

/**
 *
 * @brief Construct LilyCheckedSearchStatus type
 * (LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_SYMBOL_NOT_FOUND).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSearchStatus,
                           LilyCheckedSearchStatus,
                           symbol_not_found)
{
    return (LilyCheckedSearchStatus){
        .kind = LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_PRIVATE_SYMBOL,
        .symbol = NULL
    };
}

/**
 *
 * @brief Construct LilyCheckedSearchStatus type
 * (LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_SUCCESS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSearchStatus,
                           LilyCheckedSearchStatus,
                           success,
                           LilyCheckedSymbol *symbol)
{
    return (LilyCheckedSearchStatus){
        .kind = LILY_CHECKED_MODULE_SEARCH_STATUS_KIND_SUCCESS, .symbol = symbol
    };
}

typedef struct LilyCheckedScopeLocal
{
    const LilyCheckedDeclModule *module; // const LilyCheckedDeclModule* (&)
    Usize id;
    HashMap *variables;                   // HashMap<LilyCheckedSymbol*>*?
    HashMap *params;                      // HashMap<LilyCheckedSymbol*>*?
    HashMap *labels;                      // HashMap<LilyCheckedSymbol*>*?
    HashMap *funs;                        // HashMap<LilyCheckedSymbol*>*?
    struct LilyCheckedScopeLocal *parent; // struct LilyCheckedScopeLocal*? (&)
} LilyCheckedScopeLocal;

/**
 *
 * @brief Construct LilyCheckedScopeLocal type.
 */
CONSTRUCTOR(LilyCheckedScopeLocal *,
            LilyCheckedScopeLocal,
            const LilyCheckedDeclModule *module,
            Usize id,
            LilyCheckedScopeLocal *parent);

/**
 *
 * @brief Add a variable to the scope.
 */
enum LilyCheckedScopeAddStatus
add_variable__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                                    char *name,
                                    Usize scope_id,
                                    Usize id);

/**
 *
 * @brief Add a param to the scope.
 */
enum LilyCheckedScopeAddStatus
add_param__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                                 char *name,
                                 Usize id);

/**
 *
 * @brief Add a label to the scope.
 */
enum LilyCheckedScopeAddStatus
add_label__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                                 char *name,
                                 Usize scope_id,
                                 Usize id);

/**
 *
 * @brief Add a fun to the scope.
 */
enum LilyCheckedScopeAddStatus
add_fun__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                               char *name,
                               Usize package_id,
                               Usize id);

/**
 *
 * @brief Search variable from local scope.
 * @return LilyCheckedSymbol*? (&)
 */
LilyCheckedSymbol *
search_variable__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                       char *name);

/**
 *
 * @brief Search param from local scope.
 * @return LilyCheckedSymbol*? (&)
 */
LilyCheckedSymbol *
search_param__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                    char *name);

/**
 *
 * @brief Search label from local scope.
 * @return LilyCheckedSymbol*? (&)
 */
LilyCheckedSymbol *
search_label__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                    char *name);

/**
 *
 * @brief Search fun from local scope.
 * @return LilyCheckedSymbol*? (&)
 */
LilyCheckedSymbol *
search_fun__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                  char *name);

/**
 *
 * @brief Search identifier from local scope.
 * @return LilyCheckedSymbol*? (&)
 */
LilyCheckedSymbol *
search_identifier__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                         char *name);

/**
 *
 * @brief Free LilyCheckedScopeLocal type.
 */
DESTRUCTOR(LilyCheckedScopeLocal, LilyCheckedScopeLocal *self);

// NOTE: LilyCheckedScope, represents all kind of scope, except the scope of the
// function.
typedef struct LilyCheckedScope
{
    HashMap *constants;        // HashMap<LilyCheckedSymbol*>*?
    HashMap *enums;            // HashMap<LilyCheckedSymbol*>*?
    HashMap *records;          // HashMap<LilyCheckedSymbol*>*?
    HashMap *aliases;          // HashMap<LilyCheckedSymbol*>*?
    HashMap *errors;           // HashMap<LilyCheckedSymbol*>*?
    HashMap *enums_object;     // HashMap<LilyCheckedSymbol*>*?
    HashMap *records_object;   // HashMap<LilyCheckedSymbol*>*?
    HashMap *classes;          // HashMap<LilyCheckedSymbol*>*?
    HashMap *traits;           // HashMap<LilyCheckedSymbol*>*?
    HashMap *funs;             // HashMap<Vec<LilyCheckedSymbol*>*>*?
    LilyCheckedSymbol *parent; // LilyCheckedSymbol*?
    Vec *children;             // Vec<LilyCheckedSymbol*>*?
} LilyCheckedScope;

/**
 *
 * @brief Construct LilyCheckedScope type.
 */
CONSTRUCTOR(LilyCheckedScope *, LilyCheckedScope, LilyCheckedSymbol *parent);

/**
 *
 * @brief Add constant to the scope.
 */
enum LilyCheckedScopeAddStatus
add_constant__LilyCheckedScope(LilyCheckedScope *self,
                               char *name,
                               Usize package_id,
                               Usize id,
                               enum LilyVisibility visibility);

/**
 *
 * @brief Add enum to the scope.
 */
enum LilyCheckedScopeAddStatus
add_enum__LilyCheckedScope(LilyCheckedScope *self,
                           char *name,
                           Usize id,
                           enum LilyVisibility visibility);

/**
 *
 * @brief Add record to the scope.
 */
enum LilyCheckedScopeAddStatus
add_record__LilyCheckedScope(LilyCheckedScope *self,
                             char *name,
                             Usize id,
                             enum LilyVisibility visibility);

/**
 *
 * @brief Add alias to the scope.
 */
enum LilyCheckedScopeAddStatus
add_alias__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility);

/**
 *
 * @brief Add error to the scope.
 */
enum LilyCheckedScopeAddStatus
add_error__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility);

/**
 *
 * @brief Add enum object to the scope.
 */
enum LilyCheckedScopeAddStatus
add_enum_object__LilyCheckedScope(LilyCheckedScope *self,
                                  char *name,
                                  Usize id,
                                  enum LilyVisibility visibility);

/**
 *
 * @brief Add module to the scope.
 */
enum LilyCheckedScopeAddStatus
add_module__LilyCheckedScope(LilyCheckedScope *self,
                             char *name,
                             Usize id,
                             enum LilyVisibility visibility);

/**
 *
 * @brief Add record object to the scope.
 */
enum LilyCheckedScopeAddStatus
add_record_object__LilyCheckedScope(LilyCheckedScope *self,
                                    char *name,
                                    Usize id,
                                    enum LilyVisibility visibility);

/**
 *
 * @brief Add class to the scope.
 */
enum LilyCheckedScopeAddStatus
add_class__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility);

/**
 *
 * @brief Add trait to the scope.
 */
enum LilyCheckedScopeAddStatus
add_trait__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility);

/**
 *
 * @brief Add fun to the scope.
 */
enum LilyCheckedScopeAddStatus
add_fun__LilyCheckedScope(LilyCheckedScope *self,
                          char *name,
                          Usize id,
                          enum LilyVisibility visibility);

/**
 *
 * @brief Search a constant in the scope.
 */
LilyCheckedSearchStatus
search_constant__LilyCheckedScope(const LilyCheckedDeclModule *module,
                                  char *name,
                                  bool must_pub);

/**
 *
 * @brief Search identifier from module scope.
 */
LilyCheckedSearchStatus
search_identifier__LilyCheckedScope(const LilyCheckedDeclModule *module,
                                    char *name,
                                    bool must_pub);

/**
 *
 * @brief Search data type from module scope.
 */
LilyCheckedSearchStatus
search_data_type__LilyCheckedScope(const LilyCheckedDeclModule *module,
                                   char *name,
                                   bool must_pub);

/**
 *
 * @brief Free LilyCheckedScope type.
 */
DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_H
