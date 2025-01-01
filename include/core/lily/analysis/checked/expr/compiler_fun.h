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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_COMPILER_FUN_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_COMPILER_FUN_H

#include <base/macros.h>
#include <base/types.h>

#define EXPR_COMPILER_GET_FIELD(location, data_type, record, id) \
    NEW_VARIANT(                                                 \
      LilyCheckedExpr,                                           \
      compiler_fun,                                              \
      location,                                                  \
      data_type,                                                 \
      NULL,                                                      \
      NEW_VARIANT(LilyCheckedExprCompilerFun,                    \
                  get_field,                                     \
                  NEW(LilyCheckedExprCompilerFunGetField, record, id)))

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprCompilerFunKind
{
    LILY_CHECKED_EXPR_COMPILER_FUN_KIND_GET_FIELD,
};

/**
 *
 * @brief Convert LilyCheckedExprCompilerFunKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCompilerFunKind,
               enum LilyCheckedExprCompilerFunKind self);
#endif

typedef struct LilyCheckedExprCompilerFunGetField
{
    LilyCheckedExpr *record;
    Usize id;
} LilyCheckedExprCompilerFunGetField;

/**
 *
 * @brief Construct LilyCheckedExprCompilerFunGetField type.
 */
inline CONSTRUCTOR(LilyCheckedExprCompilerFunGetField,
                   LilyCheckedExprCompilerFunGetField,
                   LilyCheckedExpr *record,
                   Usize id)
{
    return (LilyCheckedExprCompilerFunGetField){ .record = record, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedExprCompilerFunGetField in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCompilerFunGetField,
               const LilyCheckedExprCompilerFunGetField *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCompilerFunGetField type.
 */
DESTRUCTOR(LilyCheckedExprCompilerFunGetField,
           const LilyCheckedExprCompilerFunGetField *self);

typedef struct LilyCheckedExprCompilerFun
{
    enum LilyCheckedExprCompilerFunKind kind;
    union
    {
        LilyCheckedExprCompilerFunGetField get_field;
    };
} LilyCheckedExprCompilerFun;

/**
 *
 * @brief Construct LilyCheckedExprCompilerFun type
 * (LILY_CHECKED_EXPR_COMPILER_FUN_KIND_GET_FIELD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCompilerFun,
                           LilyCheckedExprCompilerFun,
                           get_field,
                           LilyCheckedExprCompilerFunGetField get_field)
{
    return (LilyCheckedExprCompilerFun){
        .kind = LILY_CHECKED_EXPR_COMPILER_FUN_KIND_GET_FIELD,
        .get_field = get_field
    };
}

/**
 *
 * @brief Convert LilyCheckedExprCompilerFun in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCompilerFun,
               const LilyCheckedExprCompilerFun *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCompilerFun type.
 */
DESTRUCTOR(LilyCheckedExprCompilerFun, const LilyCheckedExprCompilerFun *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_COMPILER_FUN_H
