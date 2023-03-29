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

#ifndef LILY_CORE_LILY_AST_EXPR_CALL_H
#define LILY_CORE_LILY_AST_EXPR_CALL_H

#include <base/string.h>
#include <base/vec.h>

#include <core/lily/ast/data_type.h>

typedef struct LilyAstExpr LilyAstExpr;

enum LilyAstExprCallKind
{
    LILY_AST_EXPR_CALL_KIND_FUN,
    LILY_AST_EXPR_CALL_KIND_RECORD,
    LILY_AST_EXPR_CALL_KIND_VARIANT,
};

/**
 *
 * @brief Convert LilyAstExprCallKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprCallKind, enum LilyAstExprCallKind self);
#endif

enum LilyAstExprFunParamCallKind
{
    LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT,
    LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyAstExprFunParamCallKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprFunParamCallKind,
               enum LilyAstExprFunParamCallKind self);
#endif

typedef struct LilyAstExprFunParamCall
{
    enum LilyAstExprFunParamCallKind kind;
    LilyAstExpr *value;
    Location location;
    union
    {
        String *default_; // <default_> := <value>
    };
} LilyAstExprFunParamCall;

/**
 *
 * @brief Construct LilyAstExprFunParamCall type
 * (LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyAstExprFunParamCall *,
                    LilyAstExprFunParamCall,
                    default_,
                    LilyAstExpr *value,
                    Location location,
                    String *default_);

/**
 *
 * @brief Construct LilyAstExprFunParamCall type
 * (LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyAstExprFunParamCall *,
                    LilyAstExprFunParamCall,
                    normal,
                    LilyAstExpr *value,
                    Location location);

/**
 *
 * @brief Convert LilyAstExprFunParamCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprFunParamCall,
               const LilyAstExprFunParamCall *self);
#endif

/**
 *
 * @brief Free LilyAstExprFunParamCall type.
 */
DESTRUCTOR(LilyAstExprFunParamCall, LilyAstExprFunParamCall *self);

typedef struct LilyAstExprCallFun
{
    LilyAstExpr *id;
    Vec *params; // Vec<LilyAstExprFunParamCall*>*
} LilyAstExprCallFun;

/**
 *
 * @brief Construct LilyAstExprCallFun type.
 */
inline CONSTRUCTOR(LilyAstExprCallFun,
                   LilyAstExprCallFun,
                   LilyAstExpr *id,
                   Vec *params)
{
    return (LilyAstExprCallFun){ .id = id, .params = params };
}

/**
 *
 * @brief Convert LilyAstExprCallFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprCallFun, const LilyAstExprCallFun *self);
#endif

/**
 *
 * @brief Free LilyAstExprCallFun type.
 */
DESTRUCTOR(LilyAstExprCallFun, const LilyAstExprCallFun *self);

typedef struct LilyAstExprRecordParamCall
{
    String *name;
    LilyAstExpr *value;
} LilyAstExprRecordParamCall;

/**
 *
 * @brief Construct LilyAstExprRecordParamCall type.
 */
CONSTRUCTOR(LilyAstExprRecordParamCall *,
            LilyAstExprRecordParamCall,
            String *name,
            LilyAstExpr *value);

/**
 *
 * @brief Convert LilyAstExprRecordParamCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprRecordParamCall,
               const LilyAstExprRecordParamCall *self);
#endif

/**
 *
 * @brief Free LilyAstExprRecordParamCall type.
 */
DESTRUCTOR(LilyAstExprRecordParamCall, LilyAstExprRecordParamCall *self);

typedef struct LilyAstExprCallRecord
{
    LilyAstExpr *id;
    Vec *params; // Vec<LilyAstExprRecordParamCall*>*
} LilyAstExprCallRecord;

/**
 *
 * @brief Construct LilyAstExprCallRecord type.
 */
inline CONSTRUCTOR(LilyAstExprCallRecord,
                   LilyAstExprCallRecord,
                   LilyAstExpr *id,
                   Vec *params)
{
    return (LilyAstExprCallRecord){ .id = id, .params = params };
}

/**
 *
 * @brief Convert LilyAstExprCallRecord in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallRecord,
               const LilyAstExprCallRecord *self);
#endif

/**
 *
 * @brief Free LilyAstExprCallRecord type.
 */
DESTRUCTOR(LilyAstExprCallRecord, const LilyAstExprCallRecord *self);

typedef struct LilyAstExprCallVariant
{
    LilyAstExpr *id;
    LilyAstExpr *value;
} LilyAstExprCallVariant;

/**
 *
 * @brief Construct LilyAstExprCallVariant type.
 */
inline CONSTRUCTOR(LilyAstExprCallVariant,
                   LilyAstExprCallVariant,
                   LilyAstExpr *id,
                   LilyAstExpr *value)
{
    return (LilyAstExprCallVariant){ .id = id, .value = value };
}

/**
 *
 * @brief Convert LilyAstExprCallVariant in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallVariant,
               const LilyAstExprCallVariant *self);
#endif

/**
 *
 * @brief Free LilyAstExprCallVariant type.
 */
DESTRUCTOR(LilyAstExprCallVariant, const LilyAstExprCallVariant *self);

typedef struct LilyAstExprCall
{
    enum LilyAstExprCallKind kind;
    union
    {
        LilyAstExprCallFun fun;
        LilyAstExprCallRecord record;
        LilyAstExprCallVariant variant;
    };
} LilyAstExprCall;

/**
 *
 * @brief Construct LilyAstExprCall type
 * (LILY_AST_EXPR_CALL_KIND_FUN).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                           LilyAstExprCall,
                           fun,
                           LilyAstExprCallFun fun)
{
    return (LilyAstExprCall){ .kind = LILY_AST_EXPR_CALL_KIND_FUN, .fun = fun };
}

/**
 *
 * @brief Construct LilyAstExprCall type
 * (LILY_AST_EXPR_CALL_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                           LilyAstExprCall,
                           record,
                           LilyAstExprCallRecord record)
{
    return (LilyAstExprCall){ .kind = LILY_AST_EXPR_CALL_KIND_RECORD,
                              .record = record };
}

/**
 *
 * @brief Construct LilyAstExprCall type
 * (LILY_AST_EXPR_CALL_KIND_VARIANT).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                           LilyAstExprCall,
                           variant,
                           LilyAstExprCallVariant variant)
{
    return (LilyAstExprCall){ .kind = LILY_AST_EXPR_CALL_KIND_VARIANT,
                              .variant = variant };
}

/**
 *
 * @brief Convert LilyAstExprCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprCall, const LilyAstExprCall *self);
#endif

/**
 *
 * @brief Free LilyAstExprCall type.
 */
DESTRUCTOR(LilyAstExprCall, const LilyAstExprCall *self);

#endif // LILY_CORE_LILY_AST_EXPR_CALL_H
