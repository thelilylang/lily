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

#ifndef LILY_CORE_LILY_CHECKED_EXPR_CALL_H
#define LILY_CORE_LILY_CHECKED_EXPR_CALL_H

#include <base/string.h>
#include <base/vec.h>

#include <core/lily/checked/data_type.h>
#include <core/lily/checked/scope.h>

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprCallKind
{
    LILY_CHECKED_EXPR_CALL_KIND_FUN,
    LILY_CHECKED_EXPR_CALL_KIND_RECORD,
    LILY_CHECKED_EXPR_CALL_KIND_VARIANT,
};

/**
 *
 * @brief Convert LilyCheckedExprCallKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallKind,
               enum LilyCheckedExprCallKind self);
#endif

enum LilyCheckedExprFunParamCallKind
{
    LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT,
    LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyCheckedExprFunParamCallKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprFunParamCallKind,
               enum LilyCheckedExprFunParamCallKind self);
#endif

typedef struct LilyCheckedExprFunParamCall
{
    enum LilyCheckedExprFunParamCallKind kind;
    LilyCheckedExpr *value;
    Location location;
    union
    {
        String *default_; // <default_> := <value>
    };
} LilyCheckedExprFunParamCall;

/**
 *
 * @brief Construct LilyCheckedExprFunParamCall type
 * (LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprFunParamCall *,
                    LilyCheckedExprFunParamCall,
                    default_,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_);

/**
 *
 * @brief Construct LilyCheckedExprFunParamCall type
 * (LILY_CHECKED_EXPR_FUN_PARAM_CALL_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprFunParamCall *,
                    LilyCheckedExprFunParamCall,
                    normal,
                    LilyCheckedExpr *value,
                    Location location);

/**
 *
 * @brief Convert LilyCheckedExprFunParamCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprFunParamCall,
               const LilyCheckedExprFunParamCall *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprFunParamCall type.
 */
DESTRUCTOR(LilyCheckedExprFunParamCall, LilyCheckedExprFunParamCall *self);

typedef struct LilyCheckedExprCallFun
{
    LilyCheckedExpr *id;
    Vec *params; // Vec<LilyCheckedExprFunParamCall*>*
} LilyCheckedExprCallFun;

/**
 *
 * @brief Construct LilyCheckedExprCallFun type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallFun,
                   LilyCheckedExprCallFun,
                   LilyCheckedExpr *id,
                   Vec *params)
{
    return (LilyCheckedExprCallFun){ .id = id, .params = params };
}

/**
 *
 * @brief Convert LilyCheckedExprCallFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFun,
               const LilyCheckedExprCallFun *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCallFun type.
 */
DESTRUCTOR(LilyCheckedExprCallFun, const LilyCheckedExprCallFun *self);

typedef struct LilyCheckedExprRecordParamCall
{
    String *name;
    LilyCheckedExpr *value;
} LilyCheckedExprRecordParamCall;

/**
 *
 * @brief Construct LilyCheckedExprRecordParamCall type.
 */
CONSTRUCTOR(LilyCheckedExprRecordParamCall *,
            LilyCheckedExprRecordParamCall,
            String *name,
            LilyCheckedExpr *value);

/**
 *
 * @brief Convert LilyCheckedExprRecordParamCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprRecordParamCall,
               const LilyCheckedExprRecordParamCall *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprRecordParamCall type.
 */
DESTRUCTOR(LilyCheckedExprRecordParamCall,
           LilyCheckedExprRecordParamCall *self);

typedef struct LilyCheckedExprCallRecord
{
    LilyCheckedExpr *id;
    Vec *params; // Vec<LilyCheckedExprRecordParamCall*>*
} LilyCheckedExprCallRecord;

/**
 *
 * @brief Construct LilyCheckedExprCallRecord type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallRecord,
                   LilyCheckedExprCallRecord,
                   LilyCheckedExpr *id,
                   Vec *params)
{
    return (LilyCheckedExprCallRecord){ .id = id, .params = params };
}

/**
 *
 * @brief Convert LilyCheckedExprCallRecord in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecord,
               const LilyCheckedExprCallRecord *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCallRecord type.
 */
DESTRUCTOR(LilyCheckedExprCallRecord, const LilyCheckedExprCallRecord *self);

typedef struct LilyCheckedExprCallVariant
{
    LilyCheckedExpr *id;
    LilyCheckedExpr *value;
} LilyCheckedExprCallVariant;

/**
 *
 * @brief Construct LilyCheckedExprCallVariant type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallVariant,
                   LilyCheckedExprCallVariant,
                   LilyCheckedExpr *id,
                   LilyCheckedExpr *value)
{
    return (LilyCheckedExprCallVariant){ .id = id, .value = value };
}

/**
 *
 * @brief Convert LilyCheckedExprCallVariant in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallVariant,
               const LilyCheckedExprCallVariant *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCallVariant type.
 */
DESTRUCTOR(LilyCheckedExprCallVariant, const LilyCheckedExprCallVariant *self);

typedef struct LilyCheckedExprCall
{
    enum LilyCheckedExprCallKind kind;
    LilyCheckedScope scope;
    union
    {
        LilyCheckedExprCallFun fun;
        LilyCheckedExprCallRecord record;
        LilyCheckedExprCallVariant variant;
    };
} LilyCheckedExprCall;

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_FUN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           fun,
                           LilyCheckedScope scope,
                           LilyCheckedExprCallFun fun)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_FUN,
                                  .scope = scope,
                                  .fun = fun };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           record,
                           LilyCheckedScope scope,
                           LilyCheckedExprCallRecord record)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_RECORD,
                                  .scope = scope,
                                  .record = record };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_VARIANT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           variant,
                           LilyCheckedScope scope,
                           LilyCheckedExprCallVariant variant)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_VARIANT,
                                  .scope = scope,
                                  .variant = variant };
}

/**
 *
 * @brief Convert LilyCheckedExprCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprCall, const LilyCheckedExprCall *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCall type.
 */
DESTRUCTOR(LilyCheckedExprCall, const LilyCheckedExprCall *self);

#endif // LILY_CORE_LILY_CHECKED_EXPR_CALL_H
