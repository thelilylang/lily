/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#include <base/alloc.h>
#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

typedef struct CIExpr CIExpr;
typedef struct CIDataType CIDataType;
typedef struct CIDeclFunctionItem CIDeclFunctionItem;

enum CIDataTypeKind
{
    CI_DATA_TYPE_KIND_ARRAY,
    CI_DATA_TYPE_KIND__ATOMIC,
    CI_DATA_TYPE_KIND_BOOL,
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
    CI_DATA_TYPE_KIND_PRE_CONST,
    CI_DATA_TYPE_KIND_POST_CONST,
    CI_DATA_TYPE_KIND_PTR,
    CI_DATA_TYPE_KIND_SHORT_INT,
    CI_DATA_TYPE_KIND_SIGNED_CHAR,
    CI_DATA_TYPE_KIND_STRUCT,
    CI_DATA_TYPE_KIND_UNSIGNED_INT,
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
    String *name; // String* (&)
    Vec *params;  // Vec<CIDataType*>*
    struct CIDataType *return_data_type;
} CIDataTypeFunction;

/**
 *
 * @brief Construct CIDataTypeFunction type.
 */
inline CONSTRUCTOR(CIDataTypeFunction,
                   CIDataTypeFunction,
                   String *name,
                   Vec *params,
                   struct CIDataType *return_data_type)
{
    return (CIDataTypeFunction){ .name = name,
                                 .params = params,
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
        struct CIDataType *pre_const;
        struct CIDataType *post_const;
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
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND_PRE_CONST).
 */
VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, pre_const, CIDataType *pre_const);

/**
 *
 * @brief Construct CIDataType type (CI_DATA_TYPE_KIND_POST_CONST).
 */
VARIANT_CONSTRUCTOR(CIDataType *,
                    CIDataType,
                    post_const,
                    CIDataType *post_const);

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
    CI_STORAGE_CLASS_NONE = 0,
    CI_STORAGE_CLASS_AUTO = 1 << 0,
    CI_STORAGE_CLASS_CONSTEXPR = 1 << 1,
    CI_STORAGE_CLASS_EXTERN = 1 << 2,
    CI_STORAGE_CLASS_INLINE = 1 << 3,
    CI_STORAGE_CLASS_REGISTER = 1 << 4,
    CI_STORAGE_CLASS_STATIC = 1 << 5,
    CI_STORAGE_CLASS_THREAD_LOCAL = 1 << 6,
    CI_STORAGE_CLASS_TYPEDEF = 1 << 7
};

/**
 *
 * @brief Convert CIStorageClass in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIStorageClass, int storage_class_flag);
#endif

/**
 *
 * @brief Convert CIStorageClass in string.
 */
char *
to_string__CIStorageClass(int storage_class_flag);

enum CIDeclKind
{
    CI_DECL_KIND_ENUM = 1 << 0,
    CI_DECL_KIND_FUNCTION = 1 << 1,
    CI_DECL_KIND_STRUCT = 1 << 2,
    CI_DECL_KIND_UNION = 1 << 3,
    CI_DECL_KIND_VARIABLE = 1 << 4,
};

/**
 *
 * @brief Convert CIDeclKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDeclKind, enum CIDeclKind self);
#endif

enum CIDeclEnumVariantKind
{
    CI_DECL_ENUM_VARIANT_KIND_DEFAULT,
    CI_DECL_ENUM_VARIANT_KIND_CUSTOM
};

/**
 *
 * @brief Convert CIDeclEnumVariantKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclEnumVariantKind,
               enum CIDeclEnumVariantKind self);
#endif

typedef struct CIDeclEnumVariant
{
    enum CIDeclEnumVariantKind kind;
    String *name; // String* (&)
    union
    {
        Isize value;
    };
} CIDeclEnumVariant;

/**
 *
 * @brief Construct CIDeclEnumVariant type (CI_DECL_ENUM_VARIANT_KIND_CUSTOM).
 */
VARIANT_CONSTRUCTOR(CIDeclEnumVariant *,
                    CIDeclEnumVariant,
                    custom,
                    String *name,
                    Isize value);

/**
 *
 * @brief Construct CIDeclEnumVariant type (CI_DECL_ENUM_VARIANT_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(CIDeclEnumVariant *,
                    CIDeclEnumVariant,
                    default,
                    String *name);

/**
 *
 * @brief Convert CIDeclEnumVariant in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnumVariant, const CIDeclEnumVariant *self);
#endif

/**
 *
 * @brief Free CIDeclEnumVariant type.
 */
inline DESTRUCTOR(CIDeclEnumVariant, CIDeclEnumVariant *self)
{
    lily_free(self);
}

typedef struct CIDeclEnum
{
    String *name;  // String* (&)
    Vec *variants; // Vec<CIDeclEnumVariant*>*
} CIDeclEnum;

/**
 *
 * @brief Construct CIDeclEnum type.
 */
inline CONSTRUCTOR(CIDeclEnum, CIDeclEnum, String *name, Vec *variants)
{
    return (CIDeclEnum){ .name = name, .variants = variants };
}

/**
 *
 * @brief Convert CIDeclEnum in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnum, const CIDeclEnum *self);
#endif

/**
 *
 * @brief Free CIDeclEnum type.
 */
DESTRUCTOR(CIDeclEnum, const CIDeclEnum *self);

typedef struct CIDeclFunctionParam
{
    String *name; // String* (&)
    CIDataType *data_type;
} CIDeclFunctionParam;

/**
 *
 * @brief Construct CIDeclFunctionParam type.
 */
CONSTRUCTOR(CIDeclFunctionParam *,
            CIDeclFunctionParam,
            String *name,
            CIDataType *data_type);

/**
 *
 * @brief Convert CIDeclFunctionParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionParam, const CIDeclFunctionParam *self);
#endif

/**
 *
 * @brief Free CIDeclFunctionParam type.
 */
DESTRUCTOR(CIDeclFunctionParam, CIDeclFunctionParam *self);

typedef struct CIDeclFunction
{
    String *name; // String* (&)
    Vec *params;  // Vec<CIDeclFunctionParam*>*
    Vec *body;    // Vec<CIDeclFunctionItem*>*
} CIDeclFunction;

/**
 *
 * @brief Construct CIDeclFunction type.
 */
inline CONSTRUCTOR(CIDeclFunction,
                   CIDeclFunction,
                   String *name,
                   Vec *params,
                   Vec *body)
{
    return (CIDeclFunction){ .name = name, .params = params, .body = body };
}

/**
 *
 * @brief Convert CIDeclFunction in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunction, const CIDeclFunction *self);
#endif

/**
 *
 * @brief Free CIDeclFunction type.
 */
DESTRUCTOR(CIDeclFunction, const CIDeclFunction *self);

typedef struct CIDeclStructField
{
    String *name; // String* (&)
    CIDataType *data_type;
} CIDeclStructField;

/**
 *
 * @brief Construct CIDeclStructField type.
 */
CONSTRUCTOR(CIDeclStructField *,
            CIDeclStructField,
            String *name,
            CIDataType *data_type);

/**
 *
 * @brief Convert CIDeclStructField in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStructField, const CIDeclStructField *self);
#endif

/**
 *
 * @brief Free CIDeclStructField type.
 */
DESTRUCTOR(CIDeclStructField, CIDeclStructField *self);

typedef struct CIDeclStruct
{
    String *name; // String* (&)
    Vec *fields;  // Vec<CIDeclStructField*>*
} CIDeclStruct;

/**
 *
 * @brief Construct CIDeclStruct type.
 */
inline CONSTRUCTOR(CIDeclStruct, CIDeclStruct, String *name, Vec *fields)
{
    return (CIDeclStruct){ .name = name, .fields = fields };
}

/**
 *
 * @brief Convert CIDeclStruct in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStruct, const CIDeclStruct *self);
#endif

/**
 *
 * @brief Free CIDeclStruct type.
 */
DESTRUCTOR(CIDeclStruct, const CIDeclStruct *self);

typedef struct CIDeclUnion
{
    String *name; // String* (&)
    Vec *fields;  // Vec<CIDeclStructField*>*
} CIDeclUnion;

/**
 *
 * @brief Construct CIDeclUnion type.
 */
inline CONSTRUCTOR(CIDeclUnion, CIDeclUnion, String *name, Vec *fields)
{
    return (CIDeclUnion){ .name = name, .fields = fields };
}

/**
 *
 * @brief Convert CIDeclUnion in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclUnion, const CIDeclUnion *self);
#endif

/**
 *
 * @brief Free CIDeclUnion type.
 */
DESTRUCTOR(CIDeclUnion, const CIDeclUnion *self);

typedef struct CIDeclVariable
{
    String *name; // String* (&)
    CIExpr *expr; // CIExpr*?
} CIDeclVariable;

/**
 *
 * @brief Construct CIDeclVariable type.
 */
inline CONSTRUCTOR(CIDeclVariable, CIDeclVariable, String *name, CIExpr *expr)
{
    return (CIDeclVariable){ .name = name, .expr = expr };
}

/**
 *
 * @brief Convert CIDeclVariable in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclVariable, const CIDeclVariable *self);
#endif

/**
 *
 * @brief Free CIDeclVariable type.
 */
DESTRUCTOR(CIDeclVariable, const CIDeclVariable *self);

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
        CIDeclUnion union_;
    };
} CIDecl;

/**
 *
 * @brief Construct CIDecl type (CI_DECL_KIND_ENUM).
 */
VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    enum,
                    int storage_class_flag,
                    CIDeclEnum enum_);

/**
 *
 * @brief Construct CIDecl type (CI_DECL_KIND_FUNCTION).
 */
VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    function,
                    int storage_class_flag,
                    CIDeclFunction function);

/**
 *
 * @brief Construct CIDecl type (CI_DECL_KIND_STRUCT).
 */
VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    struct,
                    int storage_class_flag,
                    CIDeclStruct struct_);

/**
 *
 * @brief Construct CIDecl type (CI_DECL_KIND_UNION).
 */
VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    union,
                    int storage_class_flag,
                    CIDeclUnion union_);

/**
 *
 * @brief Construct CIDecl type (CI_DECL_KIND_VARIABLE).
 */
VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    variable,
                    int storage_class_flag,
                    CIDeclVariable variable);

/**
 *
 * @brief Convert CIDecl in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDecl, const CIDecl *self);
#endif

/**
 *
 * @brief Free CIDecl type.
 */
DESTRUCTOR(CIDecl, CIDecl *self);

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

/**
 *
 * @brief Convert CIExprBinaryKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprBinaryKind, enum CIExprBinaryKind self);
#endif

typedef struct CIExprBinary
{
    enum CIExprBinaryKind kind;
    CIExpr *left;
    CIExpr *right;
} CIExprBinary;

/**
 *
 * @brief Construct CIExprBinary type.
 */
inline CONSTRUCTOR(CIExprBinary,
                   CIExprBinary,
                   enum CIExprBinaryKind kind,
                   CIExpr *left,
                   CIExpr *right)
{
    return (CIExprBinary){ .kind = kind, .left = left, .right = right };
}

/**
 *
 * @brief Convert CIExprBinary in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprBinary, const CIExprBinary *self);
#endif

/**
 *
 * @brief Free CIExprBinary type.
 */
DESTRUCTOR(CIExprBinary, const CIExprBinary *self);

enum CIExprLiteralKind
{
    CI_EXPR_LITERAL_KIND_BOOL, // NOTE: since C23
    CI_EXPR_LITERAL_KIND_CHAR,
    CI_EXPR_LITERAL_KIND_FLOAT,
    CI_EXPR_LITERAL_KIND_SIGNED_INT,
    CI_EXPR_LITERAL_KIND_STRING,
    CI_EXPR_LITERAL_KIND_UNSIGNED_INT,
};

/**
 *
 * @brief Convert CIExprLiteral in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprLiteralKind, enum CIExprLiteralKind self);
#endif

typedef struct CIExprLiteral
{
    enum CIExprLiteralKind kind;
    union
    {
        bool bool_;
        char char_;
        double float_;
        Isize signed_int;
        String *string; // String* (&)
        Usize unsigned_int;
    };
} CIExprLiteral;

/**
 *
 * @brief Construct CIExprLiteral type (CI_EXPR_LITERAL_KIND_BOOL).
 */
inline VARIANT_CONSTRUCTOR(CIExprLiteral, CIExprLiteral, bool, bool bool_)
{
    return (CIExprLiteral){ .kind = CI_EXPR_LITERAL_KIND_BOOL, .bool_ = bool_ };
}

/**
 *
 * @brief Construct CIExprLiteral type (CI_EXPR_LITERAL_KIND_CHAR).
 */
inline VARIANT_CONSTRUCTOR(CIExprLiteral, CIExprLiteral, char, char char_)
{
    return (CIExprLiteral){ .kind = CI_EXPR_LITERAL_KIND_CHAR, .char_ = char_ };
}

/**
 *
 * @brief Construct CIExprLiteral type (CI_EXPR_LITERAL_KIND_FLOAT).
 */
inline VARIANT_CONSTRUCTOR(CIExprLiteral, CIExprLiteral, float, double float_)
{
    return (CIExprLiteral){ .kind = CI_EXPR_LITERAL_KIND_FLOAT,
                            .float_ = float_ };
}

/**
 *
 * @brief Construct CIExprLiteral type (CI_EXPR_LITERAL_KIND_SIGNED_INT).
 */
inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                           CIExprLiteral,
                           signed_int,
                           Isize signed_int)
{
    return (CIExprLiteral){ .kind = CI_EXPR_LITERAL_KIND_SIGNED_INT,
                            .signed_int = signed_int };
}

/**
 *
 * @brief Construct CIExprLiteral type (CI_EXPR_LITERAL_KIND_STRING).
 */
inline VARIANT_CONSTRUCTOR(CIExprLiteral, CIExprLiteral, string, String *string)
{
    return (CIExprLiteral){ .kind = CI_EXPR_LITERAL_KIND_STRING,
                            .string = string };
}

/**
 *
 * @brief Construct CIExprLiteral type (CI_EXPR_LITERAL_KIND_UNSIGNED_INT).
 */
inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                           CIExprLiteral,
                           unsigned_int,
                           Usize unsigned_int)
{
    return (CIExprLiteral){ .kind = CI_EXPR_LITERAL_KIND_UNSIGNED_INT,
                            .unsigned_int = unsigned_int };
}

/**
 *
 * @brief Convert CIExprLiteral in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprLiteral, const CIExprLiteral *self);
#endif

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

/**
 *
 * @brief Convert CIExprUnaryKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprUnaryKind, enum CIExprUnaryKind self);
#endif

typedef struct CIExprUnary
{
    enum CIExprUnaryKind kind;
    CIExpr *expr;
} CIExprUnary;

/**
 *
 * @brief Construct CIExprUnary type.
 */
inline CONSTRUCTOR(CIExprUnary,
                   CIExprUnary,
                   enum CIExprUnaryKind kind,
                   CIExpr *expr)
{
    return (CIExprUnary){ .kind = kind, .expr = expr };
}

/**
 *
 * @brief Convert CIExprUnary in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprUnary, const CIExprUnary *self);
#endif

/**
 *
 * @brief Free CIExprUnary type.
 */
DESTRUCTOR(CIExprUnary, const CIExprUnary *self);

typedef struct CIExprTernary
{
    CIExpr *cond;
    CIExpr *if_;
    CIExpr *else_;
} CIExprTernary;

/**
 *
 * @brief Construct CIExprTernary type.
 */
inline CONSTRUCTOR(CIExprTernary,
                   CIExprTernary,
                   CIExpr *cond,
                   CIExpr *if_,
                   CIExpr *else_)
{
    return (CIExprTernary){ .cond = cond, .if_ = if_, .else_ = else_ };
}

/**
 *
 * @brief Convert CIExprTernary in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprTernary, const CIExprTernary *self);
#endif

/**
 *
 * @brief Free CIExprTernary type.
 */
DESTRUCTOR(CIExprTernary, const CIExprTernary *self);

typedef struct CIExprCast
{
    CIDataType *data_type;
    CIExpr *expr;
} CIExprCast;

/**
 *
 * @brief Construct CIExprCast type.
 */
inline CONSTRUCTOR(CIExprCast, CIExprCast, CIDataType *data_type, CIExpr *expr)
{
    return (CIExprCast){ .data_type = data_type, .expr = expr };
}

/**
 *
 * @brief Convert CIExprCast in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprCast, const CIExprCast *self);
#endif

/**
 *
 * @brief Free CIExprCast type.
 */
DESTRUCTOR(CIExprCast, const CIExprCast *self);

enum CIExprKind
{
    CI_EXPR_KIND_ALIGNOF,
    CI_EXPR_KIND_BINARY,
    CI_EXPR_KIND_CAST,
    CI_EXPR_KIND_DATA_TYPE,
    CI_EXPR_KIND_LITERAL,
    CI_EXPR_KIND_SIZEOF,
    CI_EXPR_KIND_TERNARY,
    CI_EXPR_KIND_UNARY,
};

/**
 *
 * @brief Convert CIExprKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprKind, enum CIExprKind self);
#endif

struct CIExpr
{
    enum CIExprKind kind;
    union
    {
        CIDataType *alignof_;
        CIExprBinary binary;
        CIExprCast cast;
        CIDataType *data_type;
        CIExprLiteral literal;
        CIExpr *sizeof_;
        CIExprTernary ternary;
        CIExprUnary unary;
    };
};

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_ALIGNOF).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, alignof, CIDataType *alignof_);

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_BINARY).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, binary, CIExprBinary binary);

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_CAST).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, cast, CIExprCast cast);

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_DATA_TYPE).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, data_type, CIDataType *data_type);

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_LITERAL).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, literal, CIExprLiteral literal);

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_SIZEOF).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, sizeof, CIExpr *sizeof_);

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_TERNARY).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, ternary, CIExprTernary ternary);

/**
 *
 * @brief Construct CIExpr type (CI_EXPR_KIND_UNARY).
 */
VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, unary, CIExprUnary unary);

/**
 *
 * @brief Convert CIExpr in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExpr, const CIExpr *self);
#endif

/**
 *
 * @brief Free CIExpr type.
 */
DESTRUCTOR(CIExpr, CIExpr *self);

enum CIStmtKind
{
    CI_STMT_KIND_BREAK,
    CI_STMT_KIND_CONTINUE,
    CI_STMT_KIND_DO_WHILE,
    CI_STMT_KIND_FOR,
    CI_STMT_KIND_GOTO,
    CI_STMT_KIND_IF,
    CI_STMT_KIND_RETURN,
    CI_STMT_KIND_SWITCH,
    CI_STMT_KIND_WHILE,
};

/**
 *
 * @brief Convert CIStmtKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIStmtKind, enum CIStmtKind self);
#endif

typedef struct CIStmtDoWhile
{
    Vec *body; // Vec<CIDeclFunctionItem*>*
    CIExpr *cond;
} CIStmtDoWhile;

/**
 *
 * @brief Construct CIStmtDoWhile type.
 */
inline CONSTRUCTOR(CIStmtDoWhile, CIStmtDoWhile, Vec *body, CIExpr *cond)
{
    return (CIStmtDoWhile){ .body = body, .cond = cond };
}

/**
 *
 * @brief Convert CIStmtDoWhile in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtDoWhile, const CIStmtDoWhile *self);
#endif

/**
 *
 * @brief Free CIStmtDoWhile type.
 */
DESTRUCTOR(CIStmtDoWhile, const CIStmtDoWhile *self);

typedef struct CIStmtFor
{
    Vec *body;         // Vec<CIDeclFunctionItem*>*
    Vec *init_clauses; // Vec<CIDeclFunctionItem*>*?
    Vec *exprs1;       // Vec<CIExpr*>*?
    Vec *exprs2;       // Vec<CIExpr*>*?
} CIStmtFor;

/**
 *
 * @brief Construct CIStmtFor type.
 */
inline CONSTRUCTOR(CIStmtFor,
                   CIStmtFor,
                   Vec *body,
                   Vec *init_clauses,
                   Vec *exprs1,
                   Vec *exprs2)
{
    return (CIStmtFor){ .body = body,
                        .init_clauses = init_clauses,
                        .exprs1 = exprs1,
                        .exprs2 = exprs2 };
}

/**
 *
 * @brief Convert CIStmtFor in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtFor, const CIStmtFor *self);
#endif

/**
 *
 * @brief Free CIStmtFor type.
 */
DESTRUCTOR(CIStmtFor, const CIStmtFor *self);

typedef struct CIStmtIfBranch
{
    CIExpr *cond;
    Vec *body; // Vec<CIDeclFunctionItem*>*
} CIStmtIfBranch;

/**
 *
 * @brief Construct CIStmtIfBranch type.
 */
CONSTRUCTOR(CIStmtIfBranch *, CIStmtIfBranch, CIExpr *cond, Vec *body);

/**
 *
 * @brief Convert CIStmtIfBranch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtIfBranch, const CIStmtIfBranch *self);
#endif

/**
 *
 * @brief Free CIStmtIfBranch type.
 */
DESTRUCTOR(CIStmtIfBranch, CIStmtIfBranch *self);

typedef struct CIStmtIf
{
    CIStmtIfBranch *if_;
    Vec *else_ifs; // Vec<CIStmtIfBranch*>*?
    Vec *else_;    // Vec<CIDeclFunctionItem*>*?
} CIStmtIf;

/**
 *
 * @brief Construct CIStmtIf type.
 */
inline CONSTRUCTOR(CIStmtIf,
                   CIStmtIf,
                   CIStmtIfBranch *if_,
                   Vec *else_ifs,
                   Vec *else_)
{
    return (CIStmtIf){ .if_ = if_, .else_ifs = else_ifs, .else_ = else_ };
}

/**
 *
 * @brief Convert CIStmtIf in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtIf, const CIStmtIf *self);
#endif

/**
 *
 * @brief Free CIStmtIf type.
 */
DESTRUCTOR(CIStmtIf, const CIStmtIf *self);

typedef struct CIStmtSwitchCase
{
    CIExpr *value;
    Vec *body; // Vec<CIDeclFunctionItem*>*
} CIStmtSwitchCase;

/**
 *
 * @brief Construct CIStmtSwitchCase type.
 */
CONSTRUCTOR(CIStmtSwitchCase *, CIStmtSwitchCase, CIExpr *value, Vec *body);

/**
 *
 * @brief Convert CIStmtSwitchCase in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtSwitchCase, const CIStmtSwitchCase *self);
#endif

/**
 *
 * @brief Free CIStmtSwitchCase type.
 */
DESTRUCTOR(CIStmtSwitchCase, CIStmtSwitchCase *self);

typedef struct CIStmtSwitch
{
    CIExpr *expr;
    Vec *cases; // Vec<CIStmtSwitchCase*>*
} CIStmtSwitch;

/**
 *
 * @brief Construct CIStmtSwitch type.
 */
inline CONSTRUCTOR(CIStmtSwitch, CIStmtSwitch, CIExpr *expr, Vec *cases)
{
    return (CIStmtSwitch){ .expr = expr, .cases = cases };
}

/**
 *
 * @brief Convert CIStmtSwitch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtSwitch, const CIStmtSwitch *self);
#endif

/**
 *
 * @brief Free CIStmtSwitch type.
 */
DESTRUCTOR(CIStmtSwitch, const CIStmtSwitch *self);

typedef struct CIStmtWhile
{
    CIExpr *cond;
    Vec *body; // Vec<CIDeclFunctionItem*>*
} CIStmtWhile;

/**
 *
 * @brief Construct CIStmtWhile type.
 */
inline CONSTRUCTOR(CIStmtWhile, CIStmtWhile, CIExpr *cond, Vec *body)
{
    return (CIStmtWhile){ .cond = cond, .body = body };
}

/**
 *
 * @brief Convert CIStmtWhile in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtWhile, const CIStmtWhile *self);
#endif

/**
 *
 * @brief Free CIStmtWhile type.
 */
DESTRUCTOR(CIStmtWhile, const CIStmtWhile *self);

typedef struct CIStmt
{
    enum CIStmtKind kind;
    union
    {
        CIStmtDoWhile do_while;
        CIStmtFor for_;
        String *goto_; // String* (&)
        CIStmtIf if_;
        CIExpr *return_;
        CIStmtSwitch switch_;
        CIStmtWhile while_;
    };
} CIStmt;

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_BREAK).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, break)
{
    return (CIStmt){ .kind = CI_STMT_KIND_BREAK };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_CONTINUE).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, continue)
{
    return (CIStmt){ .kind = CI_STMT_KIND_CONTINUE };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_DO_WHILE).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, do_while, CIStmtDoWhile do_while)
{
    return (CIStmt){ .kind = CI_STMT_KIND_DO_WHILE, .do_while = do_while };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_FOR).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, for, CIStmtFor for_)
{
    return (CIStmt){ .kind = CI_STMT_KIND_FOR, .for_ = for_ };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_GOTO).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, goto, String *goto_)
{
    return (CIStmt){ .kind = CI_STMT_KIND_GOTO, .goto_ = goto_ };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_IF).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, if, CIStmtIf if_)
{
    return (CIStmt){ .kind = CI_STMT_KIND_IF, .if_ = if_ };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_RETURN).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, return, CIExpr *return_)
{
    return (CIStmt){ .kind = CI_STMT_KIND_RETURN, .return_ = return_ };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_SWITCH).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, switch, CIStmtSwitch switch_)
{
    return (CIStmt){ .kind = CI_STMT_KIND_SWITCH, .switch_ = switch_ };
}

/**
 *
 * @brief Construct CIStmt type (CI_STMT_KIND_WHILE).
 */
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, while, CIStmtWhile while_)
{
    return (CIStmt){ .kind = CI_STMT_KIND_WHILE, .while_ = while_ };
}

/**
 *
 * @brief Convert CIStmt in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmt, const CIStmt *self);
#endif

/**
 *
 * @brief Free CIStmt type.
 */
DESTRUCTOR(CIStmt, const CIStmt *self);

enum CIDeclFunctionItemKind
{
    CI_DECL_FUNCTION_ITEM_KIND_DECL,
    CI_DECL_FUNCTION_ITEM_KIND_EXPR,
    CI_DECL_FUNCTION_ITEM_KIND_STMT,
};

/**
 *
 * @brief Convert CIDeclFunctionItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclFunctionItemKind,
               enum CIDeclFunctionItemKind self);
#endif

struct CIDeclFunctionItem
{
    enum CIDeclFunctionItemKind kind;
    union
    {
        CIDecl *decl;
        CIExpr *expr;
        CIStmt stmt;
    };
};

/**
 *
 * @brief Construct CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_DECL).
 */
VARIANT_CONSTRUCTOR(CIDeclFunctionItem *,
                    CIDeclFunctionItem,
                    decl,
                    CIDecl *decl);

/**
 *
 * @brief Construct CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_EXPR).
 */
VARIANT_CONSTRUCTOR(CIDeclFunctionItem *,
                    CIDeclFunctionItem,
                    expr,
                    CIExpr *expr);

/**
 *
 * @brief Construct CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_STMT).
 */
VARIANT_CONSTRUCTOR(CIDeclFunctionItem *,
                    CIDeclFunctionItem,
                    stmt,
                    CIStmt stmt);

/**
 *
 * @brief Convert CIDeclFunctionItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionItem, const CIDeclFunctionItem *self);
#endif

/**
 *
 * @brief Free CIDeclFunctionItem type.
 */
DESTRUCTOR(CIDeclFunctionItem, CIDeclFunctionItem *self);

#endif // LILY_CORE_CC_CI_AST_H
