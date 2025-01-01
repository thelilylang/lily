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

#ifndef LILY_CORE_LILY_PARSER_AST_EXPR_CALL_H
#define LILY_CORE_LILY_PARSER_AST_EXPR_CALL_H

#include <base/string.h>
#include <base/vec.h>

#include <core/lily/parser/ast/data_type.h>

typedef struct LilyAstExpr LilyAstExpr;

enum LilyAstExprCallKind
{
    LILY_AST_EXPR_CALL_KIND_FUN,
    LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN,
    LILY_AST_EXPR_CALL_KIND_FUN_SYS,
    LILY_AST_EXPR_CALL_KIND_LEN,
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
    Usize ast_params_len; // Will use in the analysis (with `|>` operator).
    Vec *params;          // Vec<LilyAstExprFunParamCall*>*
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
    return (LilyAstExprCallFun){ .id = id,
                                 .ast_params_len = params->len,
                                 .params = params };
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

typedef struct LilyAstExprCallFunSys
{
    String *name;
    Vec *params; // Vec<LilyAstExprFunParamCall*>*
} LilyAstExprCallFunSys;

/**
 *
 * @brief Construct LilyAstExprCallFunSys type.
 */
inline CONSTRUCTOR(LilyAstExprCallFunSys,
                   LilyAstExprCallFunSys,
                   String *name,
                   Vec *params)
{
    return (LilyAstExprCallFunSys){ .name = name, .params = params };
}

/**
 *
 * @brief Convert LilyAstExprCallFunSys in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallFunSys,
               const LilyAstExprCallFunSys *self);
#endif

/**
 *
 * @brief Free LilyAstExprCallFunSys type.
 */
DESTRUCTOR(LilyAstExprCallFunSys, const LilyAstExprCallFunSys *self);

typedef struct LilyAstExprCallFunBuiltin
{
    String *name;
    Vec *params; // Vec<LilyAstExprFunParamCall*>*
} LilyAstExprCallFunBuiltin;

/**
 *
 * @brief Construct LilyAstExprCallFunSys type.
 */
inline CONSTRUCTOR(LilyAstExprCallFunBuiltin,
                   LilyAstExprCallFunBuiltin,
                   String *name,
                   Vec *params)
{
    return (LilyAstExprCallFunBuiltin){ .name = name, .params = params };
}

/**
 *
 * @brief Convert LilyAstExprCallFunBuiltin in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprCallFunBuiltin,
               const LilyAstExprCallFunBuiltin *self);
#endif

/**
 *
 * @brief Free LilyAstExprCallFunBuiltin type.
 */
DESTRUCTOR(LilyAstExprCallFunBuiltin, const LilyAstExprCallFunBuiltin *self);

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
        LilyAstExprCallFunBuiltin fun_builtin;
        LilyAstExprCallFunSys fun_sys;
        LilyAstExpr *len;
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
 * (LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                           LilyAstExprCall,
                           fun_builtin,
                           LilyAstExprCallFunBuiltin fun_builtin)
{
    return (LilyAstExprCall){ .kind = LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN,
                              .fun_builtin = fun_builtin };
}

/**
 *
 * @brief Construct LilyAstExprCall type
 * (LILY_AST_EXPR_CALL_KIND_FUN_SYS).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                           LilyAstExprCall,
                           fun_sys,
                           LilyAstExprCallFunSys fun_sys)
{
    return (LilyAstExprCall){ .kind = LILY_AST_EXPR_CALL_KIND_FUN_SYS,
                              .fun_sys = fun_sys };
}

/**
 *
 * @brief Construct LilyAstExprCall type
 * (LILY_AST_EXPR_CALL_KIND_LEN).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                           LilyAstExprCall,
                           len,
                           LilyAstExpr *len)
{
    return (LilyAstExprCall){ .kind = LILY_AST_EXPR_CALL_KIND_LEN, .len = len };
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

#endif // LILY_CORE_LILY_PARSER_AST_EXPR_CALL_H
