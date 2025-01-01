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

#ifndef LILY_CORE_LILY_PARSER_AST_GENERIC_PARAM_H
#define LILY_CORE_LILY_PARSER_AST_GENERIC_PARAM_H

#include <base/macros.h>

#include <core/lily/parser/ast/expr.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

enum LilyAstGenericParamKind
{
    LILY_AST_GENERIC_PARAM_KIND_CONSTRAINT,
    LILY_AST_GENERIC_PARAM_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyAstGenericParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstGenericParamKind,
               enum LilyAstGenericParamKind self);
#endif

typedef struct LilyAstGenericParamConstraint
{
    String *name;
    Vec *constraints; // Vec<LilyAstDataType*>*
} LilyAstGenericParamConstraint;

/**
 *
 * @brief Construct LilyAstGenericParamConstraint type.
 */
inline CONSTRUCTOR(LilyAstGenericParamConstraint,
                   LilyAstGenericParamConstraint,
                   String *name,
                   Vec *constraints)
{
    return (LilyAstGenericParamConstraint){ .name = name,
                                            .constraints = constraints };
}

/**
 *
 * @brief Convert LilyAstGenericParamConstraint in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstGenericParamConstraint,
               const LilyAstGenericParamConstraint *self);
#endif

/**
 *
 * @brief Free LilyAstGenericParamConstraint type.
 */
DESTRUCTOR(LilyAstGenericParamConstraint,
           const LilyAstGenericParamConstraint *self);

typedef struct LilyAstGenericParam
{
    enum LilyAstGenericParamKind kind;
    Location location;
    union
    {
        LilyAstGenericParamConstraint constraint;
        String *normal; // expected an identifier for normal param
    };
} LilyAstGenericParam;

/**
 *
 * @brief Construct LilyAstGenericParam type
 * (LILY_AST_GENERIC_PARAM_KIND_CONSTRAINT).
 */
VARIANT_CONSTRUCTOR(LilyAstGenericParam *,
                    LilyAstGenericParam,
                    constraint,
                    Location location,
                    LilyAstGenericParamConstraint constraint);

/**
 *
 * @brief Construct LilyAstGenericParam type
 * (LILY_AST_GENERIC_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyAstGenericParam *,
                    LilyAstGenericParam,
                    normal,
                    Location location,
                    String *normal);

/**
 *
 * @brief Convert LilyAstGenericParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstGenericParam, const LilyAstGenericParam *self);
#endif

/**
 *
 * @brief Free LilyAstGenericParam type.
 */
DESTRUCTOR(LilyAstGenericParam, LilyAstGenericParam *self);

#endif // LILY_CORE_LILY_PARSER_AST_GENERIC_PARAM_H
