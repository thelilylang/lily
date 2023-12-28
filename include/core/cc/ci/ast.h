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

#ifndef LILY_CORE_CC_CI_AST_H
#define LILY_CORE_CC_CI_AST_H

#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

typedef struct CIExpr CIExpr;
typedef struct CIDataType CIDataType;

enum CIDataTypeKind
{
    CI_DATA_TYPE_KIND_ARRAY,
    CI_DATA_TYPE_KIND__ATOMIC,
    CI_DATA_TYPE_KIND_CHAR,
    CI_DATA_TYPE_KIND_DOUBLE,
    CI_DATA_TYPE_KIND_DOUBLE__COMPLEX,
    CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY,
    CI_DATA_TYPE_KIND__DECIMAL32,
    CI_DATA_TYPE_KIND__DECIMAL64,
    CI_DATA_TYPE_KIND__DECIMAL128,
    CI_DATA_TYPE_KIND_FLOAT,
    CI_DATA_TYPE_KIND_FLOAT__COMPLEX,
    CI_DATA_TYPE_KIND_FLOAT__IMAGINARY,
    CI_DATA_TYPE_KIND_FUNCTION,
    CI_DATA_TYPE_KIND_INT,
    CI_DATA_TYPE_KIND_LONG_DOUBLE,
    CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX,
    CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY,
    CI_DATA_TYPE_KIND_LONG_INT,
    CI_DATA_TYPE_KIND_LONG_LONG_INT,
    CI_DATA_TYPE_KIND_PTR,
    CI_DATA_TYPE_KIND_SHORT_INT,
    CI_DATA_TYPE_KIND_SIGNED_CHAR,
    CI_DATA_TYPE_KIND_STRUCT,
    CI_DATA_TYPE_KIND_UNSIGNED_CHAR,
    CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT,
    CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT,
    CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT,
    CI_DATA_TYPE_KIND_UNION,
    CI_DATA_TYPE_KIND_VOID,
};

/**
 *
 * @brief Convert CIDataTypeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDataTypeKind, enum CIDataTypeKind self);
#endif

enum CIDataTypeArrayKind
{
    CI_DATA_TYPE_ARRAY_KIND_SIZED,
    CI_DATA_TYPE_ARRAY_KIND_NONE,
};

/**
 *
 * @brief Convert CIDataTypeArrayKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDataTypeArrayKind, enum CIDataTypeArrayKind self);
#endif

typedef struct CIDataTypeArray
{
    enum CIDataTypeArrayKind kind;
    struct CIDataType *data_type;
    union
    {
        Usize size;
    };
} CIDataTypeArray;

/**
 *
 * @brief Construct CIDataTypeArray type (CI_DATA_TYPE_ARRAY_KIND_SIZED).
 */
inline VARIANT_CONSTRUCTOR(CIDataTypeArray,
                           CIDataTypeArray,
                           sized,
                           struct CIDataType *data_type,
                           Usize size)
{
    return (CIDataTypeArray){ .kind = CI_DATA_TYPE_ARRAY_KIND_SIZED,
                              .data_type = data_type,
                              .size = size };
}

/**
 *
 * @brief Construct CIDataTypeArray type (CI_DATA_TYPE_ARRAY_KIND_NONE).
 */
inline VARIANT_CONSTRUCTOR(CIDataTypeArray,
                           CIDataTypeArray,
                           none,
                           struct CIDataType *data_type)
{
    return (CIDataTypeArray){
        .kind = CI_DATA_TYPE_ARRAY_KIND_NONE,
        .data_type = data_type,
    };
}

/**
 *
 * @brief Convert CIDataTypeArray in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeArray, const CIDataTypeArray *self);
#endif

/**
 *
 * @brief Free CIDataTypeArray type.
 */
DESTRUCTOR(CIDataTypeArray, const CIDataTypeArray *self);

typedef struct CIDataTypeFunction
{
    Vec *params; // Vec<CIDataType*>*
    struct CIDataType *return_data_type;
} CIDataTypeFunction;

/**
 *
 * @brief Construct CIDataTypeFunction type.
 */
inline CONSTRUCTOR(CIDataTypeFunction,
                   CIDataTypeFunction,
                   Vec *params,
                   struct CIDataType *return_data_type)
{
    return (CIDataTypeFunction){ .params = params,
                                 .return_data_type = return_data_type };
}

/**
 *
 * @brief Convert CIDataTypeFunction in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeFunction, const CIDataTypeFunction *self);
#endif

/**
 *
 * @brief Free CIDataTypeFunction type.
 */
DESTRUCTOR(CIDataTypeFunction, const CIDataTypeFunction *self);

typedef struct CIDataTypeStruct
{
    String *name;        // String* (&)
    Vec *generic_params; // Vec<CIDataType*>*?
} CIDataTypeStruct;

/**
 *
 * @brief Construct CIDataTypeStruct type.
 */
inline CONSTRUCTOR(CIDataTypeStruct,
                   CIDataTypeStruct,
                   String *name,
                   Vec *generic_params)
{
    return (CIDataTypeStruct){ .name = name, .generic_params = generic_params };
}

/**
 *
 * @brief Convert CIDataTypeStruct in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeStruct, const CIDataTypeStruct *self);
#endif

/**
 *
 * @brief Free CIDataTypeStruct type.
 */
DESTRUCTOR(CIDataTypeStruct, const CIDataTypeStruct *self);

typedef struct CIDataTypeUnion
{
    String *name;        // String* (&)
    Vec *generic_params; // Vec<CIDataType*>*?
} CIDataTypeUnion;

/**
 *
 * @brief Construct CIDataTypeUnion type.
 */
inline CONSTRUCTOR(CIDataTypeUnion,
                   CIDataTypeUnion,
                   String *name,
                   Vec *generic_params)
{
    return (CIDataTypeUnion){ .name = name, .generic_params = generic_params };
}

/**
 *
 * @brief Convert CIDataTypeUnion in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeUnion, const CIDataTypeUnion *self);
#endif

/**
 *
 * @brief Free CIDataTypeUnion type.
 */
DESTRUCTOR(CIDataTypeUnion, const CIDataTypeUnion *self);

typedef struct CIDataType
{
    enum CIDataTypeKind kind;
    union
    {
        CIDataTypeArray array;
        struct CIDataType *_atomic;
        CIDataTypeFunction function;
        struct CIDataType *ptr;
        CIDataTypeStruct struct_;
        CIDataTypeUnion union_;
    };
} CIDataType;

/**
 *
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, array, CIDataTypeArray array);

/**
 *
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND__ATOMIC).
 */
VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, _atomic, CIDataType *_atomic);

/**
 *
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND_FUNCTION).
 */
VARIANT_CONSTRUCTOR(CIDataType *,
                    CIDataType,
                    function,
                    CIDataTypeFunction function);

/**
 *
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND_PTR).
 */
VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, ptr, CIDataType *ptr);

/**
 *
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND_STRUCT).
 */
VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, struct, CIDataTypeStruct struct_);

/**
 *
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND_UNION).
 */
VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, union, CIDataTypeUnion union_);

/**
 *
 * @brief Construct CIDataType type.
 */
CONSTRUCTOR(CIDataType *, CIDataType, enum CIDataTypeKind kind);

/**
 *
 * @brief Convert CIDataType in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataType, const CIDataType *self);
#endif

/**
 *
 * @brief Free CIDataType type.
 */
DESTRUCTOR(CIDataType, CIDataType *self);

enum CIStorageClass
{
    CI_VISIBILITY_AUTO = 1 << 0,
    CI_VISIBILITY_CONST = 1 << 1,
    CI_VISIBILITY_CONSTEXPR = 1 << 2,
    CI_VISIBILITY_EXTERN = 1 << 3,
    CI_VISIBILITY_INLINE = 1 << 4,
    CI_VISIBILITY_REGISTER = 1 << 5,
    CI_VISIBILITY_STATIC = 1 << 6,
    CI_VISIBILITY_THREAD_LOCAL = 1 << 7,
    CI_VISIBILITY_TYPEDEF = 1 << 8
};

/**
 *
 * @brief Convert CIStorageClass in string.
 */
char *
to_string__CIStorageClass(int storage_class_flag);

enum CIDeclKind
{
    CI_DECL_KIND_ENUM,
    CI_DECL_KIND_FUNCTION,
    CI_DECL_KIND_STRUCT,
    CI_DECL_KIND_VARIABLE
};

enum CIDeclEnumVariantKind
{
    CI_DECL_ENUM_VARIANT_KIND_DEFAULT,
    CI_DECL_ENUM_VARIANT_KIND_CUSTOM
};

typedef struct CIDeclEnumVariant
{
    enum CIDeclEnumVariantKind kind;
    String *name;
    union
    {
        Usize value;
    };
} CIDeclEnumVariant;

typedef struct CIDeclEnum
{
    String *name;
    Vec *variants; // Vec<CIDeclEnumVariant*>*
} CIDeclEnum;

typedef struct CIDeclFunctionParam
{
    String *name;
    CIDataType *data_type;
} CIDeclFunctionParam;

typedef struct CIDeclFunction
{
    int storage_class_flag;
    String *name;
    Vec *params; // Vec<CIDeclFunctionParam*>*
    Vec *body;   // Vec<CIDeclFunctionItem*>*
} CIDeclFunction;

typedef struct CIDeclStructField
{
    String *name;
    CIDataType *data_type;
} CIDeclStructField;

typedef struct CIDeclStruct
{
    String *name;
    Vec *fields; // Vec<CIDeclStructField*>*
} CIDeclStruct;

typedef struct CIDeclVariable
{
    int storage_class_flag;
    String *name;
    CIExpr *expr; // CIExpr*?
} CIDeclVariable;

typedef struct CIDecl
{
    enum CIDeclKind kind;
    int storage_class_flag;
    union
    {
        CIDeclEnum enum_;
        CIDeclFunction function;
        CIDeclStruct struct_;
        CIDeclVariable variable;
    };
} CIDecl;

enum CIExprBinaryKind
{
    CI_EXPR_BINARY_KIND_ASSIGN,            // =
    CI_EXPR_BINARY_KIND_ASSIGN_ADD,        // +=
    CI_EXPR_BINARY_KIND_ASSIGN_SUB,        // -=
    CI_EXPR_BINARY_KIND_ASSIGN_MUL,        // *=
    CI_EXPR_BINARY_KIND_ASSIGN_DIV,        // /=
    CI_EXPR_BINARY_KIND_ASSIGN_MOD,        // %=
    CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND,    // &=
    CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR,     // |=
    CI_EXPR_BINARY_KIND_ASSIGN_XOR,        // ^=
    CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT, // <<=
    CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT, // >>=
    CI_EXPR_BINARY_KIND_ADD,               // +
    CI_EXPR_BINARY_KIND_SUB,               // -
    CI_EXPR_BINARY_KIND_MUL,               // *
    CI_EXPR_BINARY_KIND_DIV,               // /
    CI_EXPR_BINARY_KIND_MOD,               // %
    CI_EXPR_BINARY_KIND_BIT_AND,           // &
    CI_EXPR_BINARY_KIND_BIT_OR,            // |
    CI_EXPR_BINARY_KIND_BIT_XOR,           // ^
    CI_EXPR_BINARY_KIND_BIT_LSHIFT,        // <<
    CI_EXPR_BINARY_KIND_BIT_RSHIFT,        // >>
    CI_EXPR_BINARY_KIND_AND,               // &&
    CI_EXPR_BINARY_KIND_OR,                // ||
    CI_EXPR_BINARY_KIND_EQ,                // ==
    CI_EXPR_BINARY_KIND_NE,                // !=
    CI_EXPR_BINARY_KIND_LESS,              // <
    CI_EXPR_BINARY_KIND_GREATER,           // >
    CI_EXPR_BINARY_KIND_LESS_EQ,           // <=
    CI_EXPR_BINARY_KIND_GREATER_EQ,        // >=
    CI_EXPR_BINARY_KIND_DOT,               // .
    CI_EXPR_BINARY_KIND_ARROW,             // ->
};

typedef struct CIExprBinary
{
    enum CIExprBinaryKind kind;
    CIExpr *left;
    CIExpr *right;
} CIExprBinary;

enum CIExprUnaryKind
{
    CI_EXPR_UNARY_KIND_PRE_INCREMENT,  // ++<expr>
    CI_EXPR_UNARY_KIND_PRE_DECREMENT,  // --<expr>
    CI_EXPR_UNARY_KIND_POST_INCREMENT, // <expr>++
    CI_EXPR_UNARY_KIND_POST_DECREMENT, // <expr>--
    CI_EXPR_UNARY_KIND_POSITIVE,       // +
    CI_EXPR_UNARY_KIND_NEGATIVE,       // -
    CI_EXPR_UNARY_KIND_BIT_NOT,        // ~
    CI_EXPR_UNARY_KIND_NOT,            // !
    CI_EXPR_UNARY_KIND_DEREFERENCE,    // *
    CI_EXPR_UNARY_KIND_REF,            // &
};

typedef struct CIExprUnary
{
    enum CIExprUnaryKind kind;
    CIExpr *expr;
} CIExprUnary;

typedef struct CIExprTernary
{
    CIExpr *cond;
    CIExpr *if_;
    CIExpr *else_;
} CIExprTernary;

typedef struct CIExprCast
{
    CIDataType *data_type;
    CIExpr *expr;
} CIExprCast;

enum CIExprKind
{
    CI_EXPR_KIND_ALIGNOF,
    CI_EXPR_KIND_BINARY,
    CI_EXPR_KIND_CAST,
    CI_EXPR_KIND_DATA_TYPE,
    CI_EXPR_KIND_SIZEOF,
    CI_EXPR_KIND_TERNARY,
    CI_EXPR_KIND_UNARY,
};

struct CIExpr
{
    enum CIExprKind kind;
    union
    {
        CIDataType *alignof_;
        CIExprBinary binary;
        CIExprCast cast;
        CIDataType *data_type;
        CIExpr *sizeof_;
        CIExprTernary ternary;
        CIExprUnary unary;
    };
};

#endif // LILY_CORE_CC_CI_AST_H
