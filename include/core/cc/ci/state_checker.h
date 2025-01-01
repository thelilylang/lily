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

#ifndef LILY_CORE_CC_CI_STATE_CHECKER_H
#define LILY_CORE_CC_CI_STATE_CHECKER_H

#include <base/types.h>

#include <core/cc/ci/result.h>

enum CIStateCheckerStateKind
{
    CI_STATE_CHECKER_STATE_KIND_NONE = 0,
    CI_STATE_CHECKER_STATE_KIND_HEAP = 1 << 0,
    CI_STATE_CHECKER_STATE_KIND_NON_NULL = 1 << 1,
    CI_STATE_CHECKER_STATE_KIND_STACK = 1 << 2,
    CI_STATE_CHECKER_STATE_KIND_TRACE = 1 << 3,
};

typedef struct CIStateCheckerState
{
    Usize flags;
    Usize copy_count;
} CIStateCheckerState;

/**
 *
 * @brief Construct CIStateCheckerState type.
 */
inline CONSTRUCTOR(CIStateCheckerState, CIStateCheckerState, Usize flags)
{
    return (CIStateCheckerState){ .flags = flags, .copy_count = 0 };
}

/**
 *
 * @brief Increment `copy_count` field.
 */
inline void
increment_copy__CIStateCheckerState(CIStateCheckerState *self)
{
    ++self->copy_count;
}

/**
 *
 * @brief Decrement `copy_count` field.
 */
inline void
decrement_copy__CIStateCheckerState(CIStateCheckerState *self)
{
    --self->copy_count;
}

enum CIStateCheckerValueKind
{
    CI_STATE_CHECKER_VALUE_KIND_STRUCT,
    CI_STATE_CHECKER_VALUE_KIND_UNION,
    CI_STATE_CHECKER_VALUE_KIND_VARIABLE,
};

typedef struct CIStateCheckerValueStruct
{
    HashMap *values; // HashMap<CIStateCheckerValue*>*
} CIStateCheckerValueStruct;

/**
 *
 * @brief Construct CIStateCheckerValueStruct type.
 */
inline CONSTRUCTOR(CIStateCheckerValueStruct,
                   CIStateCheckerValueStruct,
                   HashMap *values)
{
    return (CIStateCheckerValueStruct){ .values = values };
}

/**
 *
 * @brief Free CIStateCheckerValueStruct type.
 */
DESTRUCTOR(CIStateCheckerValueStruct, const CIStateCheckerValueStruct *self);

typedef struct CIStateCheckerValueVariable
{
    const String *name; // const String* (&)
    CIStateCheckerState state;
} CIStateCheckerValueVariable;

/**
 *
 * @brief Construct CIStateCheckerValueVariable type.
 */
inline CONSTRUCTOR(CIStateCheckerValueVariable,
                   CIStateCheckerValueVariable,
                   const String *name,
                   CIStateCheckerState state)
{
    return (CIStateCheckerValueVariable){ .name = name, .state = state };
}

typedef struct CIStateCheckerValue
{
    enum CIStateCheckerValueKind kind;
    Usize ref_count;
    union
    {
        CIStateCheckerValueStruct struct_;
        CIStateCheckerValueStruct union_;
        CIStateCheckerValueVariable variable;
    };
} CIStateCheckerValue;

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
 * @brief Free CIStateCheckerValue type.
 */
DESTRUCTOR(CIStateCheckerValue, CIStateCheckerValue *self);

typedef struct CIStateCheckerScope
{
    struct CIStateCheckerScope *parent; // struct CIStateCheckerScope*? (&)
    HashMap *values;                    // HashMap<CIStateCheckerValue*>*
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
 * @brief Free CIStateCheckerScope type.
 */
DESTRUCTOR(CIStateCheckerScope, CIStateCheckerScope *self);

typedef struct CIStateChecker
{
    const CIResult *result;     // const CIResult* (&)
    const CIResultFile *file;   // const CIResultFile*? (&)
    CIStateCheckerScope *scope; // CIStateCheckerScope*?
} CIStateChecker;

/**
 *
 * @brief Construct CIStateChecker type.
 */
inline CONSTRUCTOR(CIStateChecker, CIStateChecker, const CIResult *result)
{
    return (CIStateChecker){ .result = result, .file = NULL, .scope = NULL };
}

/**
 *
 * @brief Run the state checker.
 */
void
run__CIStateChecker(CIStateChecker *self);

#endif // LILY_CORE_CC_CI_STATE_CHECKER_H
