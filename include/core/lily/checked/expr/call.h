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

#include <core/lily/builtin.h>
#include <core/lily/checked/access.h>
#include <core/lily/checked/data_type.h>
#include <core/lily/sys.h>

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprCallKind
{
    LILY_CHECKED_EXPR_CALL_KIND_ATTRIBUTE,
    LILY_CHECKED_EXPR_CALL_KIND_CLASS,
    LILY_CHECKED_EXPR_CALL_KIND_CONSTANT,
    LILY_CHECKED_EXPR_CALL_KIND_ERROR,
    LILY_CHECKED_EXPR_CALL_KIND_FIELD,
    LILY_CHECKED_EXPR_CALL_KIND_FUN,
    LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS,
    LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN,
    LILY_CHECKED_EXPR_CALL_KIND_METHOD,
    LILY_CHECKED_EXPR_CALL_KIND_MODULE,
    LILY_CHECKED_EXPR_CALL_KIND_RECORD,
    LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
    LILY_CHECKED_EXPR_CALL_KIND_VARIABLE,
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

typedef struct LilyCheckedExprCallError
{
    Vec *params; // Vec<LilyCheckedExpr*>*?
} LilyCheckedExprCallError;

inline CONSTRUCTOR(LilyCheckedExprCallError,
                   LilyCheckedExprCallError,
                   Vec *params)
{
    return (LilyCheckedExprCallError){ .params = params };
}

/**
 *
 * @brief Convert LilyCheckedExprCallError in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallError,
               const LilyCheckedExprCallError *self);
#endif

enum LilyCheckedExprCallFunParamKind
{
    LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT,
    LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT_OVERWRITE,
    LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyCheckedExprCallFunParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunParamKind,
               enum LilyCheckedExprCallFunParamKind self);
#endif

typedef struct LilyCheckedExprCallFunParam
{
    enum LilyCheckedExprCallFunParamKind kind;
    // NOTE: the pointer is a copy when kind is equal to
    // `LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT`
    LilyCheckedExpr *value; // LilyCheckedExpr* | LilyCheckedExpr* (&)
    Location location;
    union
    {
        String *default_; // String* (&) <default_> := <value>
    };
} LilyCheckedExprCallFunParam;

/**
 *
 * @brief Construct LilyCheckedExprCallFunParam type
 * (LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT).
 * @param value LilyCheckedExpr* (&)
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprCallFunParam *,
                    LilyCheckedExprCallFunParam,
                    default_,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_);

/**
 *
 * @brief Construct LilyCheckedExprCallFunParam type
 * (LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_DEFAULT_OVERWRITE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprCallFunParam *,
                    LilyCheckedExprCallFunParam,
                    default_overwrite,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_);

/**
 *
 * @brief Construct LilyCheckedExprCallFunParam type
 * (LILY_CHECKED_EXPR_CALL_FUN_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprCallFunParam *,
                    LilyCheckedExprCallFunParam,
                    normal,
                    LilyCheckedExpr *value,
                    Location location);

/**
 *
 * @brief Convert LilyCheckedExprCallFunParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunParam,
               const LilyCheckedExprCallFunParam *self);
#endif

typedef struct LilyCheckedExprCallFun
{
    Vec *params; // Vec<LilyCheckedExprCallFunParam*>*?
} LilyCheckedExprCallFun;

/**
 *
 * @brief Construct LilyCheckedExprCallFun type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallFun, LilyCheckedExprCallFun, Vec *params)
{
    return (LilyCheckedExprCallFun){ .params = params };
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

enum LilyCheckedExprCallMethodParamKind
{
    LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT,
    LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT_OVERWRITE,
    LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL,
    LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_SELF,
    LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_SELF,
    LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_MUT_SELF,
    LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_REF_MUT_SELF
};

/**
 *
 * @brief Convert LilyCheckedExprCallMethodParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallMethodParamKind,
               enum LilyCheckedExprCallMethodParamKind self);
#endif

typedef struct LilyCheckedExprCallMethodParam
{
    enum LilyCheckedExprCallMethodParamKind kind;
    // NOTE: the pointer is a copy when kind is equal to
    // `LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT`
    LilyCheckedExpr *value; // LilyCheckedExpr*? | LilyCheckedExpr* (&)
    Location location;
    union
    {
        String *default_; // String* (&) <default_> := <value>
    };
} LilyCheckedExprCallMethodParam;

/**
 *
 * @brief Construct LilyCheckedExprCallMethodParam type
 * (LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT).
 * @param value LilyCheckedExpr* (&)
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
                    LilyCheckedExprCallMethodParam,
                    default_,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_);

/**
 *
 * @brief Construct LilyCheckedExprCallMethodParam type
 * (LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_DEFAULT_OVERWRITE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
                    LilyCheckedExprCallMethodParam,
                    default_overwrite,
                    LilyCheckedExpr *value,
                    Location location,
                    String *default_);

/**
 *
 * @brief Construct LilyCheckedExprCallMethodParam type
 * (LILY_CHECKED_EXPR_CALL_METHOD_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
                    LilyCheckedExprCallMethodParam,
                    normal,
                    LilyCheckedExpr *value,
                    Location location);

/**
 *
 * @brief Construct LilyCheckedExprCallMethodParam type.
 */
CONSTRUCTOR(LilyCheckedExprCallMethodParam *,
            LilyCheckedExprCallMethodParam,
            enum LilyCheckedExprCallMethodParamKind kind,
            Location location);

/**
 *
 * @brief Convert LilyCheckedExprCallMethodParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallMethodParam,
               const LilyCheckedExprCallMethodParam *self);
#endif

typedef struct LilyCheckedExprCallMethod
{
    Vec *params; // Vec<LilyCheckedExprCallMethodParam*>*?
} LilyCheckedExprCallMethod;

/**
 *
 * @brief Construct LilyCheckedExprCallMethod type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallMethod,
                   LilyCheckedExprCallMethod,
                   Vec *params)
{
    return (LilyCheckedExprCallMethod){ .params = params };
}

/**
 *
 * @brief Convert LilyCheckedExprCallMethod in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallMethod,
               const LilyCheckedExprCallMethod *self);
#endif

typedef struct LilyCheckedExprCallRecordParam
{
    String *name; // String* (&)
    LilyCheckedExpr *value;
} LilyCheckedExprCallRecordParam;

/**
 *
 * @brief Construct LilyCheckedExprCallRecordParam type.
 */
CONSTRUCTOR(LilyCheckedExprCallRecordParam *,
            LilyCheckedExprCallRecordParam,
            String *name,
            LilyCheckedExpr *value);

/**
 *
 * @brief Convert LilyCheckedExprCallRecordParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecordParam,
               const LilyCheckedExprCallRecordParam *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCallRecordParam type.
 */
DESTRUCTOR(LilyCheckedExprCallRecordParam,
           LilyCheckedExprCallRecordParam *self);

typedef struct LilyCheckedExprCallRecord
{
    Vec *params; // Vec<LilyCheckedExprCallRecordParam*>*?
} LilyCheckedExprCallRecord;

/**
 *
 * @brief Construct LilyCheckedExprCallRecord type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallRecord,
                   LilyCheckedExprCallRecord,
                   Vec *params)
{
    return (LilyCheckedExprCallRecord){ .params = params };
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

typedef struct LilyCheckedExprCallVariant
{
    Vec *params; // Vec<LilyCheckedExpr*>*?
} LilyCheckedExprCallVariant;

/**
 *
 * @brief Construct LilyCheckedExprCallVariant type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallVariant,
                   LilyCheckedExprCallVariant,
                   Vec *params)
{
    return (LilyCheckedExprCallVariant){ .params = params };
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

typedef struct LilyCheckedExprCall
{
    enum LilyCheckedExprCallKind kind;
    LilyCheckedAccessScope scope; // NOTE: undef when kind is equal to
                                  // LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN or
                                  // LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS
    String *global_name;          // String* (&)
    union
    {
        LilyCheckedExprCallError error;
        LilyCheckedExprCallFun fun;
        const LilyBuiltinFun *fun_builtin; // const LilyBuiltinFun* (&)
        const LilySysFun *fun_sys;         // const LilySysFun* (&)
        LilyCheckedExprCallMethod method;
        LilyCheckedExprCallRecord record;
        LilyCheckedExprCallVariant variant;
    };
} LilyCheckedExprCall;

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_ERROR).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           error,
                           LilyCheckedAccessScope scope,
                           String *global_name,
                           LilyCheckedExprCallError error)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_ERROR,
                                  .scope = scope,
                                  .global_name = global_name,
                                  .error = error };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_FUN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           fun,
                           LilyCheckedAccessScope scope,
                           String *global_name,
                           LilyCheckedExprCallFun fun)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_FUN,
                                  .scope = scope,
                                  .global_name = global_name,
                                  .fun = fun };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           fun_builtin,
                           String *global_name,
                           const LilyBuiltinFun *fun_builtin)
{
    return (LilyCheckedExprCall){ .kind =
                                    LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN,
                                  .global_name = global_name,
                                  .fun_builtin = fun_builtin };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           fun_sys,
                           String *global_name,
                           const LilySysFun *fun_sys)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS,
                                  .global_name = global_name,
                                  .fun_sys = fun_sys };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_METHOD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           method,
                           LilyCheckedAccessScope scope,
                           String *global_name,
                           LilyCheckedExprCallMethod method)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_METHOD,
                                  .scope = scope,
                                  .global_name = global_name,
                                  .method = method };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           record,
                           LilyCheckedAccessScope scope,
                           String *global_name,
                           LilyCheckedExprCallRecord record)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_RECORD,
                                  .scope = scope,
                                  .global_name = global_name,
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
                           LilyCheckedAccessScope scope,
                           String *global_name,
                           LilyCheckedExprCallVariant variant)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_VARIANT,
                                  .scope = scope,
                                  .global_name = global_name,
                                  .variant = variant };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type.
 */
inline CONSTRUCTOR(LilyCheckedExprCall,
                   LilyCheckedExprCall,
                   enum LilyCheckedExprCallKind kind,
                   String *global_name,
                   LilyCheckedAccessScope scope)
{
    return (LilyCheckedExprCall){ .kind = kind,
                                  .scope = scope,
                                  .global_name = global_name };
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
