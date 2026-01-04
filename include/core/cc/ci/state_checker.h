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

#ifndef LILY_CORE_CC_CI_STATE_CHECKER_H
#define LILY_CORE_CC_CI_STATE_CHECKER_H

#include <base/types.h>

#include <core/cc/ci/result.h>

typedef struct CIStateCheckerState
{
    int flags;
} CIStateCheckerState;

/**
 *
 * @brief Construct CIStateCheckerState type.
 */
inline CONSTRUCTOR(CIStateCheckerState, CIStateCheckerState, int flags)
{
    return (CIStateCheckerState){ .flags = flags };
}

enum CIStateCheckerValueKind
{
    CI_STATE_CHECKER_VALUE_KIND_ARRAY,
    CI_STATE_CHECKER_VALUE_KIND_FUNCTION,
    CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR,
    CI_STATE_CHECKER_VALUE_KIND_PRIMARY,
    CI_STATE_CHECKER_VALUE_KIND_PTR,
    CI_STATE_CHECKER_VALUE_KIND_STRUCT,
    CI_STATE_CHECKER_VALUE_KIND_UNION,
    CI_STATE_CHECKER_VALUE_KIND_VARIABLE,
};

typedef struct CIStateCheckerValueFunction
{
    Rc *name;                           // Rc<String*>*
    OrderedHashMap *final_params_state; // OrderedHashMap<CIStateCheckerValue*>*
    struct CIStateCheckerValue *return_value;
} CIStateCheckerValueFunction;

/**
 *
 * @brief Construct CIStateCheckerValueFunction type.
 * @param name Rc<String*>* (&)
 */
inline CONSTRUCTOR(CIStateCheckerValueFunction,
                   CIStateCheckerValueFunction,
                   Rc *name,
                   OrderedHashMap *final_params_state,
                   struct CIStateCheckerValue *return_value)
{
    return (CIStateCheckerValueFunction){ .name = ref__Rc(name),
                                          .final_params_state =
                                            final_params_state,
                                          .return_value = return_value };
}

/**
 *
 * @brief Free CIStateCheckerValueFunction type.
 */
DESTRUCTOR(CIStateCheckerValueFunction,
           const CIStateCheckerValueFunction *self);

typedef struct CIStateCheckerValueFunctionPtr
{
    CIStateCheckerState state;
    Vec *params; // Vec<CIStateCheckerValue*>*
    struct CIStateCheckerValue *return_value;
} CIStateCheckerValueFunctionPtr;

/**
 *
 * @brief Construct CIStateCheckerValueFunctionPtr type.
 */
inline CONSTRUCTOR(CIStateCheckerValueFunctionPtr,
                   CIStateCheckerValueFunctionPtr,
                   CIStateCheckerState state,
                   Vec *params,
                   struct CIStateCheckerValue *return_value)
{
    return (CIStateCheckerValueFunctionPtr){ .state = state,
                                             .params = params,
                                             .return_value = return_value };
}

/**
 *
 * @brief Free CIStateCheckerValueFunctionPtr type.
 */
DESTRUCTOR(CIStateCheckerValueFunctionPtr,
           const CIStateCheckerValueFunctionPtr *self);

typedef struct CIStateCheckerValuePtr
{
    CIStateCheckerState state;
    struct CIStateCheckerValue *value;
} CIStateCheckerValuePtr;

/**
 *
 * @brief Construct CIStateCheckerValuePtr type.
 */
inline CONSTRUCTOR(CIStateCheckerValuePtr,
                   CIStateCheckerValuePtr,
                   CIStateCheckerState state,
                   struct CIStateCheckerValue *value)
{
    return (CIStateCheckerValuePtr){ .state = state, .value = value };
}

/**
 *
 * @brief Free CIStateCheckerValuePtr type.
 */
DESTRUCTOR(CIStateCheckerValuePtr, const CIStateCheckerValuePtr *self);

typedef struct CIStateCheckerValueStruct
{
    Rc *name;        // Rc<String*>*?
    HashMap *values; // HashMap<CIStateCheckerValue*>*
    CIStateCheckerState state;
    // -1: the value is not set
    // 0: the struct is not dropable
    // 1: the struct is dropable
    int is_dropable;
    bool is_anonymous;
} CIStateCheckerValueStruct;

/**
 *
 * @brief Construct CIStateCheckerValueStruct type.
 * @param name Rc<String*>*? (&)
 */
inline CONSTRUCTOR(CIStateCheckerValueStruct,
                   CIStateCheckerValueStruct,
                   Rc *name,
                   HashMap *values,
                   CIStateCheckerState state,
                   bool is_anonymous)
{
    return (CIStateCheckerValueStruct){ .name = name ? ref__Rc(name) : NULL,
                                        .values = values,
                                        .state = state,
                                        .is_dropable = -1,
                                        .is_anonymous = is_anonymous };
}

/**
 *
 * @brief Set `is_dropable` field.
 * @param CIStateCheckerValueStruct* (&)
 */
void
set_is_dropable__CIStateCheckerValueStruct(CIStateCheckerValueStruct *self,
                                           int is_dropable);

/**
 *
 * @brief Free CIStateCheckerValueStruct type.
 */
DESTRUCTOR(CIStateCheckerValueStruct, const CIStateCheckerValueStruct *self);

typedef struct CIStateCheckerValueVariable
{
    Rc *name; // Rc<String*>*
    struct CIStateCheckerValue *value;
    // In some cases, we want to inherit the scope owner from the parent, as in
    // the case of `fields`.
    bool inherit_of_parent_scope_owner;
    // In some cases, we don't want to have `scope_owner`, as in the case of
    // final params state.
    bool disable_scope_owner;
    Usize scope_owner;
} CIStateCheckerValueVariable;

/**
 *
 * @brief Construct CIStateCheckerValueVariable type.
 * @param name Rc<String*>* (&)
 */
inline CONSTRUCTOR(CIStateCheckerValueVariable,
                   CIStateCheckerValueVariable,
                   Rc *name,
                   struct CIStateCheckerValue *value)
{
    return (CIStateCheckerValueVariable){ .name = ref__Rc(name),
                                          .value = value,
                                          .inherit_of_parent_scope_owner =
                                            false,
                                          .disable_scope_owner = false,
                                          .scope_owner = 0 };
}

/**
 *
 * @brief Set `self->inherit_of_parent_scope_owner` to `true`.
 */
inline void
set_inherit_of_parent_scope_owner__CIStateCheckerValueVariable(
  CIStateCheckerValueVariable *self)
{
    self->inherit_of_parent_scope_owner = true;
}

/**
 *
 * @brief Set `self->disable_scope_owner` to `true`.
 */
inline void
set_disable_scope_owner__CIStateCheckerValueVariable(
  CIStateCheckerValueVariable *self)
{
    self->disable_scope_owner = true;
}

/**
 *
 * @brief Set `self->scope_owner`.
 */
inline void
set_scope_owner__CIStateCheckerValueVariable(CIStateCheckerValueVariable *self,
                                             Usize scope_owner)
{
    self->scope_owner = scope_owner;
}

/**
 *
 * @brief Free CIStateCheckerValueVariable type.
 */
DESTRUCTOR(CIStateCheckerValueVariable,
           const CIStateCheckerValueVariable *self);

typedef struct CIStateCheckerValue
{
    enum CIStateCheckerValueKind kind;
    Usize ref_count;
    union
    {
        CIStateCheckerValuePtr array;
        CIStateCheckerValueFunction function;
        CIStateCheckerValueFunctionPtr function_ptr;
        CIStateCheckerState primary;
        CIStateCheckerValuePtr ptr;
        CIStateCheckerValueStruct struct_;
        CIStateCheckerValueStruct union_;
        CIStateCheckerValueVariable variable;
    };
} CIStateCheckerValue;

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    array,
                    struct CIStateCheckerValuePtr array);

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_FUNCTION).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    function,
                    CIStateCheckerValueFunction function);

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    function_ptr,
                    CIStateCheckerValueFunctionPtr function_ptr);

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_PRIMARY).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    primary,
                    CIStateCheckerState primary);

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_PTR).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    ptr,
                    CIStateCheckerValuePtr ptr);

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_STRUCT).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    struct,
                    CIStateCheckerValueStruct struct_);

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_UNION).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    union,
                    CIStateCheckerValueStruct union_);

/**
 *
 * @brief Construct CIStateCheckerValue type
 * (CI_STATE_CHECKER_VALUE_KIND_VARIABLE).
 */
VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    variable,
                    CIStateCheckerValueVariable variable);

/**
 *
 * @brief Get name from value.
 * @return const String*? (&)
 */
const String *
get_name__CIStateCheckerValue(CIStateCheckerValue *self);

/**
 *
 * @brief Get return state from value.
 * @return CIStateCheckerState* (&)
 */
CIStateCheckerState *
get_return_state__CIStateCheckerValue(CIStateCheckerValue *self);

/**
 *
 * @brief Increment `ref_count`.
 * @return CIStateCheckerValue*
 */
inline CIStateCheckerValue *
ref__CIStateCheckerValue(CIStateCheckerValue *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free CIStateCheckerValue type.
 */
DESTRUCTOR(CIStateCheckerValue, CIStateCheckerValue *self);

typedef struct CIStateCheckerScope
{
    Usize id;
    struct CIStateCheckerScope *parent; // struct CIStateCheckerScope*? (&)
    // can contains structs, enums, unions or functions
    HashMap *values; // HashMap<CIStateCheckerValue*>*
} CIStateCheckerScope;

/**
 *
 * @brief Construct CIStateCheckerScope type.
 * @param parent CIStateCheckerScope*? (&)
 */
CONSTRUCTOR(CIStateCheckerScope *,
            CIStateCheckerScope,
            CIStateCheckerScope *parent);

/**
 *
 * @brief Add value to scope.
 */
void
add__CIStateCheckerScope(CIStateCheckerScope *self, CIStateCheckerValue *value);

/**
 *
 * @brief Get value from name.
 * @return CIStateCheckerValue*? (&)
 */
CIStateCheckerValue *
get__CIStateCheckerScope(CIStateCheckerScope *self, Rc *name);

/**
 *
 * @brief Free CIStateCheckerScope type.
 */
DESTRUCTOR(CIStateCheckerScope, CIStateCheckerScope *self);

typedef struct CIStateCheckerAnonymousNameGenerator
{
    Usize count;
    Vec *names; // Vec<Rc<String*>*>*
} CIStateCheckerAnonymousNameGenerator;

/**
 *
 * @brief Construct CIStateCheckerAnonymousNameGenerator type.
 */
inline CONSTRUCTOR(CIStateCheckerAnonymousNameGenerator,
                   CIStateCheckerAnonymousNameGenerator)
{
    return (CIStateCheckerAnonymousNameGenerator){ .count = 0,
                                                   .names = NEW(Vec) };
}

/**
 *
 * @brief Generate anonymous name.
 * @return Rc<String*>* (&)
 */
Rc *
run__CIStateCheckerAnonymousNameGenerator(
  CIStateCheckerAnonymousNameGenerator *self);

/**
 *
 * @brief Free CIStateCheckerAnonymousNameGenerator type.
 */
DESTRUCTOR(CIStateCheckerAnonymousNameGenerator,
           const CIStateCheckerAnonymousNameGenerator *self);

typedef struct CIStateChecker
{
    const CIResult *result;             // const CIResult* (&)
    const CIResultFile *file;           // const CIResultFile*? (&)
    CIStateCheckerScope *current_scope; // CIStateCheckerScope*?
    CIStateCheckerAnonymousNameGenerator anonymous_name_generator;
} CIStateChecker;

/**
 *
 * @brief Construct CIStateChecker type.
 */
inline CONSTRUCTOR(CIStateChecker, CIStateChecker, const CIResult *result)
{
    return (CIStateChecker){ .result = result,
                             .file = NULL,
                             .current_scope = NULL,
                             .anonymous_name_generator =
                               NEW(CIStateCheckerAnonymousNameGenerator) };
}

/**
 *
 * @brief Run the state checker.
 */
void
run__CIStateChecker(CIStateChecker *self);

/**
 *
 * @brief Free CIStateChecker type.
 */
DESTRUCTOR(CIStateChecker, const CIStateChecker *self);

#endif // LILY_CORE_CC_CI_STATE_CHECKER_H
