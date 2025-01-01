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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_CALL_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_CALL_H

#include <base/hash_map.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/access.h>
#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/functions/builtin.h>
#include <core/lily/functions/sys.h>

typedef struct LilyCheckedExpr LilyCheckedExpr;
typedef struct LilyCheckedDecl LilyCheckedDecl;

enum LilyCheckedExprCallKind
{
    LILY_CHECKED_EXPR_CALL_KIND_ATTRIBUTE,
    LILY_CHECKED_EXPR_CALL_KIND_CATCH_VARIABLE,
    LILY_CHECKED_EXPR_CALL_KIND_CLASS,
    LILY_CHECKED_EXPR_CALL_KIND_CONSTANT,
    LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN,
    LILY_CHECKED_EXPR_CALL_KIND_ERROR,
    LILY_CHECKED_EXPR_CALL_KIND_ENUM,
    LILY_CHECKED_EXPR_CALL_KIND_FUN,
    LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS,
    LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN,
    LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM,
    LILY_CHECKED_EXPR_CALL_KIND_METHOD,
    LILY_CHECKED_EXPR_CALL_KIND_MODULE,
    LILY_CHECKED_EXPR_CALL_KIND_RECORD,
    LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE,
    LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS,
    LILY_CHECKED_EXPR_CALL_KIND_STR_LEN,
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
    const Location *location;
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
                    const Location *location,
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
                    const Location *location,
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
                    const Location *location);

/**
 *
 * @brief Check if the both fun params are equal.
 */
bool
eq__LilyCheckedExprCallFunParam(const LilyCheckedExprCallFunParam *self,
                                const LilyCheckedExprCallFunParam *other);

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
    LilyCheckedDecl *fun;    // LilyCheckedDecl* (&)
    Vec *params;             // Vec<LilyCheckedExprCallFunParam*>*?
    HashMap *generic_params; // HashMap<LilyCheckedDataType*>*? (&)
} LilyCheckedExprCallFun;

/**
 *
 * @brief Construct LilyCheckedExprCallFun type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallFun,
                   LilyCheckedExprCallFun,
                   LilyCheckedDecl *fun,
                   Vec *params,
                   HashMap *generic_params)
{
    return (LilyCheckedExprCallFun){ .fun = fun,
                                     .params = params,
                                     .generic_params = generic_params };
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

typedef struct LilyCheckedExprCallFunBuiltin
{
    Vec *params; // Vec<LilyCheckedExprCallFunParam*>*?
    const LilyBuiltinFun *builtin_fun_signature; // const LilyBuiltinFun* (&)
} LilyCheckedExprCallFunBuiltin;

/**
 *
 * @brief Construct LilyCheckedExprCallFunBuiltin type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallFunBuiltin,
                   LilyCheckedExprCallFunBuiltin,
                   Vec *params,
                   const LilyBuiltinFun *builtin_fun_signature)
{
    return (LilyCheckedExprCallFunBuiltin){ .params = params,
                                            .builtin_fun_signature =
                                              builtin_fun_signature };
}

/**
 *
 * @brief Convert LilyCheckedExprCallFunBuiltin in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunBuiltin,
               const LilyCheckedExprCallFunBuiltin *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCallFunBuiltin type.
 */
DESTRUCTOR(LilyCheckedExprCallFunBuiltin,
           const LilyCheckedExprCallFunBuiltin *self);

typedef struct LilyCheckedExprCallFunSys
{
    Vec *params;                         // Vec<LilyCheckedExprCallFunParam*>*?
    const LilySysFun *sys_fun_signature; // const LilySysFun* (&)
} LilyCheckedExprCallFunSys;

/**
 *
 * @brief Construct LilyCheckedExprCallFunSys type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallFunSys,
                   LilyCheckedExprCallFunSys,
                   Vec *params,
                   const LilySysFun *sys_fun_signature)
{
    return (LilyCheckedExprCallFunSys){ .params = params,
                                        .sys_fun_signature =
                                          sys_fun_signature };
}

/**
 *
 * @brief Convert LilyCheckedExprCallFunSys in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallFunSys,
               const LilyCheckedExprCallFunSys *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCallFunSys type.
 */
DESTRUCTOR(LilyCheckedExprCallFunSys, const LilyCheckedExprCallFunSys *self);

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
 * @brief Check if the both method params are equal.
 */
bool
eq__LilyCheckedExprCallMethodParam(const LilyCheckedExprCallMethodParam *self,
                                   const LilyCheckedExprCallMethodParam *other);

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
    Usize field_index;
} LilyCheckedExprCallRecordParam;

/**
 *
 * @brief Construct LilyCheckedExprCallRecordParam type.
 */
CONSTRUCTOR(LilyCheckedExprCallRecordParam *,
            LilyCheckedExprCallRecordParam,
            String *name,
            LilyCheckedExpr *value,
            Usize field_index);

/**
 *
 * @brief Check if the both record params are equal.
 */
bool
eq__LilyCheckedExprCallRecordParam(const LilyCheckedExprCallRecordParam *self,
                                   const LilyCheckedExprCallRecordParam *other);

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

typedef struct LilyCheckedExprCallRecordFieldSingle
{
    LilyCheckedAccessScope record_access;
    String *record_global_name; // String* (&)
    Usize index;                // index of field in record
} LilyCheckedExprCallRecordFieldSingle;

/**
 *
 * @brief Construct LilyCheckedExprCallRecordFieldSingle type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallRecordFieldSingle,
                   LilyCheckedExprCallRecordFieldSingle,
                   LilyCheckedAccessScope record_access,
                   String *record_global_name,
                   Usize index)
{
    return (LilyCheckedExprCallRecordFieldSingle){ .record_access =
                                                     record_access,
                                                   .record_global_name =
                                                     record_global_name,
                                                   .index = index };
}

/**
 *
 * @brief Check if the both record fields are equal.
 */
bool
eq__LilyCheckedExprCallRecordFieldSingle(
  const LilyCheckedExprCallRecordFieldSingle *self,
  const LilyCheckedExprCallRecordFieldSingle *other);

/**
 *
 * @brief Convert LilyCheckedExprCallRecordFieldSingle in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecordFieldSingle,
               const LilyCheckedExprCallRecordFieldSingle *self);
#endif

typedef struct LilyCheckedExprCallRecordFieldAccess
{
    // NOTE: the accesses Vector is only composed of variant call expression.
    Vec *accesses; // Vec<LilyCheckedExpr*>*
} LilyCheckedExprCallRecordFieldAccess;

/**
 *
 * @brief Construct LilyCheckedExprCallRecordFieldAccess type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallRecordFieldAccess,
                   LilyCheckedExprCallRecordFieldAccess,
                   Vec *accesses)
{
    return (LilyCheckedExprCallRecordFieldAccess){ .accesses = accesses };
}

/**
 *
 * @brief Check if the both field accesses are equal.
 */
bool
eq__LilyCheckedExprCallRecordFieldAccess(
  const LilyCheckedExprCallRecordFieldAccess *self,
  const LilyCheckedExprCallRecordFieldAccess *other);

/**
 *
 * @brief Convert LilyCheckedExprCallRecordFieldAccess in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCallRecordFieldAccess,
               const LilyCheckedExprCallRecordFieldAccess *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprCallRecordFieldAccess type.
 */
DESTRUCTOR(LilyCheckedExprCallRecordFieldAccess,
           const LilyCheckedExprCallRecordFieldAccess *self);

typedef struct LilyCheckedExprCallVariant
{
    LilyCheckedExpr *value; // LilyCheckedExpr*?
} LilyCheckedExprCallVariant;

/**
 *
 * @brief Construct LilyCheckedExprCallVariant type.
 */
inline CONSTRUCTOR(LilyCheckedExprCallVariant,
                   LilyCheckedExprCallVariant,
                   LilyCheckedExpr *value)
{
    return (LilyCheckedExprCallVariant){ .value = value };
}

/**
 *
 * @brief Check if the both variants are equal.
 */
bool
eq__LilyCheckedExprCallVariant(const LilyCheckedExprCallVariant *self,
                               const LilyCheckedExprCallVariant *other);

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
                                  // LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS or
                                  // LILY_CHECKED_EXPR_CALL_KIND_STR_LEN or
                                  // LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN or
                                  // LILY_CHECKED_EXPR_CALL_KIND_CATCH_VARIABLE
    String *global_name;          // String*? (&)
    union
    {
        HashMap *catch_variable; // HashMap<LilyCheckedDataType*>* (&)
        LilyCheckedExpr *cstr_len;
        LilyCheckedDecl *enum_; // LilyCheckedDecl* (&)
        LilyCheckedExprCallError error;
        LilyCheckedExprCallFun fun;
        LilyCheckedExprCallFunBuiltin fun_builtin;
        LilyCheckedExprCallFunSys fun_sys;
        Usize fun_param; // index of fun param
        LilyCheckedExprCallMethod method;
        LilyCheckedExprCallRecord record;
        LilyCheckedExprCallRecordFieldSingle record_field_single;
        LilyCheckedExprCallRecordFieldAccess record_field_access;
        LilyCheckedExpr *str_len;
        LilyCheckedExprCallVariant variant;
    };
} LilyCheckedExprCall;

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           cstr_len,
                           LilyCheckedExpr *cstr_len)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN,
                                  .global_name = NULL,
                                  .cstr_len = cstr_len };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_CATCH_VARIABLE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           catch_variable,
                           String *global_name,
                           HashMap *catch_variable)
{
    return (LilyCheckedExprCall){ .kind =
                                    LILY_CHECKED_EXPR_CALL_KIND_CATCH_VARIABLE,
                                  .global_name = global_name,
                                  .catch_variable = catch_variable };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_ENUM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           enum,
                           LilyCheckedAccessScope scope,
                           String *global_name,
                           LilyCheckedDecl *enum_)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_ENUM,
                                  .scope = scope,
                                  .global_name = global_name,
                                  .enum_ = enum_ };
}

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
                           LilyCheckedExprCallFunBuiltin fun_builtin)
{
    return (LilyCheckedExprCall){ .kind =
                                    LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN,
                                  .global_name = NULL,
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
                           LilyCheckedExprCallFunSys fun_sys)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS,
                                  .global_name = NULL,
                                  .fun_sys = fun_sys };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           fun_param,
                           LilyCheckedAccessScope scope,
                           String *global_name,
                           Usize fun_param)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM,
                                  .scope = scope,
                                  .global_name = global_name,
                                  .fun_param = fun_param };
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
 * (LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE).
 */
inline VARIANT_CONSTRUCTOR(
  LilyCheckedExprCall,
  LilyCheckedExprCall,
  record_field_single,
  LilyCheckedAccessScope scope,
  String *global_name,
  LilyCheckedExprCallRecordFieldSingle record_field_single)
{
    return (LilyCheckedExprCall){
        .kind = LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE,
        .scope = scope,
        .global_name = global_name,
        .record_field_single = record_field_single
    };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS).
 */
inline VARIANT_CONSTRUCTOR(
  LilyCheckedExprCall,
  LilyCheckedExprCall,
  record_field_access,
  LilyCheckedAccessScope scope,
  String *global_name,
  LilyCheckedExprCallRecordFieldAccess record_field_access)
{
    return (LilyCheckedExprCall){
        .kind = LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS,
        .scope = scope,
        .global_name = global_name,
        .record_field_access = record_field_access
    };
}

/**
 *
 * @brief Construct LilyCheckedExprCall type
 * (LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                           LilyCheckedExprCall,
                           str_len,
                           LilyCheckedExpr *str_len)
{
    return (LilyCheckedExprCall){ .kind = LILY_CHECKED_EXPR_CALL_KIND_STR_LEN,
                                  .global_name = NULL,
                                  .str_len = str_len };
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
 * @brief Check if the both calls are equal.
 */
bool
eq__LilyCheckedExprCall(const LilyCheckedExprCall *self,
                        const LilyCheckedExprCall *other);

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

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_CALL_H
