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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_GENERIC_PARAM_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_GENERIC_PARAM_H

#include <base/macros.h>

#include <core/lily/analysis/checked/expr.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

enum LilyCheckedGenericParamKind
{
    LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT,
    LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyCheckedGenericParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGenericParamKind,
               enum LilyCheckedGenericParamKind self);
#endif

typedef struct LilyCheckedGenericParamConstraint
{
    String *name;     // String* (&)
    Vec *constraints; // Vec<LilyCheckedDataType*>*
} LilyCheckedGenericParamConstraint;

/**
 *
 * @brief Construct LilyCheckedGenericParamConstraint type.
 */
inline CONSTRUCTOR(LilyCheckedGenericParamConstraint,
                   LilyCheckedGenericParamConstraint,
                   String *name,
                   Vec *constraints)
{
    return (LilyCheckedGenericParamConstraint){ .name = name,
                                                .constraints = constraints };
}

/**
 *
 * @brief Convert LilyCheckedGenericParamConstraint in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGenericParamConstraint,
               const LilyCheckedGenericParamConstraint *self);
#endif

/**
 *
 * @brief Free LilyCheckedGenericParamConstraint type.
 */
DESTRUCTOR(LilyCheckedGenericParamConstraint,
           const LilyCheckedGenericParamConstraint *self);

typedef struct LilyCheckedGenericParam
{
    enum LilyCheckedGenericParamKind kind;
    const Location *location;
    union
    {
        LilyCheckedGenericParamConstraint constraint;
        String *normal; // String* (&) expected an identifier for normal param
    };
} LilyCheckedGenericParam;

/**
 *
 * @brief Construct LilyCheckedGenericParam type
 * (LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedGenericParam *,
                    LilyCheckedGenericParam,
                    constraint,
                    const Location *location,
                    LilyCheckedGenericParamConstraint constraint);

/**
 *
 * @brief Construct LilyCheckedGenericParam type
 * (LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedGenericParam *,
                    LilyCheckedGenericParam,
                    normal,
                    const Location *location,
                    String *normal);

/**
 *
 * @brief Get the name from generic param.
 * @return String* (&)
 */
String *
get_name__LilyCheckedGenericParam(const LilyCheckedGenericParam *self);

/**
 *
 * @brief Convert LilyCheckedGenericParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGenericParam,
               const LilyCheckedGenericParam *self);
#endif

/**
 *
 * @brief Free LilyCheckedGenericParam type.
 */
DESTRUCTOR(LilyCheckedGenericParam, LilyCheckedGenericParam *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_GENERIC_PARAM_H
