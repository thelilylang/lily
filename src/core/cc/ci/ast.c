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

#include <base/new.h>

#include <core/cc/ci/ast.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_ARRAY).
static VARIANT_DESTRUCTOR(CIDataType, array, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND__ATOMIC).
static VARIANT_DESTRUCTOR(CIDataType, _atomic, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_ENUM).
static inline VARIANT_DESTRUCTOR(CIDataType, enum, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_PRE_CONST).
static VARIANT_DESTRUCTOR(CIDataType, pre_const, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_POST_CONST).
static VARIANT_DESTRUCTOR(CIDataType, post_const, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(CIDataType, ptr, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_STRUCT).
static VARIANT_DESTRUCTOR(CIDataType, struct, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_UNION).
static VARIANT_DESTRUCTOR(CIDataType, union, CIDataType *self);

/// @brief Free CIDecl type (CI_DECL_KIND_ENUM).
static VARIANT_DESTRUCTOR(CIDecl, enum, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIDecl, function, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_STRUCT).
static VARIANT_DESTRUCTOR(CIDecl, struct, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_UNION).
static VARIANT_DESTRUCTOR(CIDecl, union, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_VARIABLE).
static VARIANT_DESTRUCTOR(CIDecl, variable, CIDecl *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_ALIGNOF).
static VARIANT_DESTRUCTOR(CIExpr, alignof, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_BINARY).
static VARIANT_DESTRUCTOR(CIExpr, binary, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_CAST).
static VARIANT_DESTRUCTOR(CIExpr, cast, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_DATA_TYPE).
static VARIANT_DESTRUCTOR(CIExpr, data_type, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_IDENTIFIER).
static inline VARIANT_DESTRUCTOR(CIExpr, identifier, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_LITERAL).
static inline VARIANT_DESTRUCTOR(CIExpr, literal, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_SIZEOF).
static VARIANT_DESTRUCTOR(CIExpr, sizeof, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_TERNARY).
static VARIANT_DESTRUCTOR(CIExpr, ternary, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_UNARY).
static VARIANT_DESTRUCTOR(CIExpr, unary, CIExpr *self);

/// @brief Free CIStmt type (CI_STMT_KIND_DO_WHILE).
static inline VARIANT_DESTRUCTOR(CIStmt, do_while, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_FOR).
static inline VARIANT_DESTRUCTOR(CIStmt, for, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_IF).
static inline VARIANT_DESTRUCTOR(CIStmt, if, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_RETURN).
static inline VARIANT_DESTRUCTOR(CIStmt, return, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_SWITCH).
static inline VARIANT_DESTRUCTOR(CIStmt, switch, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_WHILE).
static inline VARIANT_DESTRUCTOR(CIStmt, while, const CIStmt *self);

/// @brief Free CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_DECL).
static VARIANT_DESTRUCTOR(CIDeclFunctionItem, decl, CIDeclFunctionItem *self);

/// @brief Free CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_EXPR).
static VARIANT_DESTRUCTOR(CIDeclFunctionItem, expr, CIDeclFunctionItem *self);

/// @brief Free CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_STMT).
static VARIANT_DESTRUCTOR(CIDeclFunctionItem, stmt, CIDeclFunctionItem *self);

CONSTRUCTOR(CIEnumID *, CIEnumID, CIFileID file_id, Usize id)
{
    CIEnumID *self = lily_malloc(sizeof(CIEnumID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIStructID *, CIStructID, CIFileID file_id, Usize id)
{
    CIStructID *self = lily_malloc(sizeof(CIStructID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIFunctionID *, CIFunctionID, CIFileID file_id, Usize id)
{
    CIFunctionID *self = lily_malloc(sizeof(CIFunctionID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIUnionID *, CIUnionID, CIFileID file_id, Usize id)
{
    CIUnionID *self = lily_malloc(sizeof(CIUnionID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDataTypeKind, enum CIDataTypeKind self)
{
    switch (self) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return "CI_DATA_TYPE_KIND_ARRAY";
        case CI_DATA_TYPE_KIND__ATOMIC:
            return "CI_DATA_TYPE_KIND__ATOMIC";
        case CI_DATA_TYPE_KIND_BOOL:
            return "CI_DATA_TYPE_KIND_BOOL";
        case CI_DATA_TYPE_KIND_CHAR:
            return "CI_DATA_TYPE_KIND_CHAR";
        case CI_DATA_TYPE_KIND_DOUBLE:
            return "CI_DATA_TYPE_KIND_DOUBLE";
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
            return "CI_DATA_TYPE_KIND_DOUBLE__COMPLEX";
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
            return "CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY";
        case CI_DATA_TYPE_KIND__DECIMAL32:
            return "CI_DATA_TYPE_KIND__DECIMAL32";
        case CI_DATA_TYPE_KIND__DECIMAL64:
            return "CI_DATA_TYPE_KIND__DECIMAL64";
        case CI_DATA_TYPE_KIND__DECIMAL128:
            return "CI_DATA_TYPE_KIND__DECIMAL128";
        case CI_DATA_TYPE_KIND_ENUM:
            return "CI_DATA_TYPE_KIND_ENUM";
        case CI_DATA_TYPE_KIND_FLOAT:
            return "CI_DATA_TYPE_KIND_FLOAT";
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
            return "CI_DATA_TYPE_KIND_FLOAT__COMPLEX";
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
            return "CI_DATA_TYPE_KIND_FLOAT__IMAGINARY";
        case CI_DATA_TYPE_KIND_FUNCTION:
            return "CI_DATA_TYPE_KIND_FUNCTION";
        case CI_DATA_TYPE_KIND_INT:
            return "CI_DATA_TYPE_KIND_INT";
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
            return "CI_DATA_TYPE_KIND_LONG_DOUBLE";
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
            return "CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX";
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            return "CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY";
        case CI_DATA_TYPE_KIND_LONG_INT:
            return "CI_DATA_TYPE_KIND_LONG_INT";
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
            return "CI_DATA_TYPE_KIND_LONG_LONG_INT";
        case CI_DATA_TYPE_KIND_PRE_CONST:
            return "CI_DATA_TYPE_KIND_PRE_CONST";
        case CI_DATA_TYPE_KIND_POST_CONST:
            return "CI_DATA_TYPE_KIND_POST_CONST";
        case CI_DATA_TYPE_KIND_PTR:
            return "CI_DATA_TYPE_KIND_PTR";
        case CI_DATA_TYPE_KIND_SHORT_INT:
            return "CI_DATA_TYPE_KIND_SHORT_INT";
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            return "CI_DATA_TYPE_KIND_SIGNED_CHAR";
        case CI_DATA_TYPE_KIND_STRUCT:
            return "CI_DATA_TYPE_KIND_STRUCT";
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_INT";
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
            return "CI_DATA_TYPE_KIND_UNSIGNED_CHAR";
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT";
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT";
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT";
        case CI_DATA_TYPE_KIND_UNION:
            return "CI_DATA_TYPE_KIND_UNION";
        case CI_DATA_TYPE_KIND_VOID:
            return "CI_DATA_TYPE_KIND_VOID";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDataTypeArrayKind, enum CIDataTypeArrayKind self)
{
    switch (self) {
        case CI_DATA_TYPE_ARRAY_KIND_SIZED:
            return "CI_DATA_TYPE_ARRAY_KIND_SIZED";
        case CI_DATA_TYPE_ARRAY_KIND_NONE:
            return "CI_DATA_TYPE_ARRAY_KIND_NONE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeArray, const CIDataTypeArray *self)
{
    return self->kind == CI_DATA_TYPE_ARRAY_KIND_SIZED
             ? format__String(
                 "CIDataTypeArray{{ kind = {s}, data_type = {Sr}, name = {s}, "
                 "size = {zu} }",
                 to_string__Debug__CIDataTypeArrayKind(self->kind),
                 to_string__Debug__CIDataType(self->data_type),
                 self->name ? self->name->buffer : "NULL",
                 self->size)
             : format__String(
                 "CIDataTypeArray{{ kind = {s}, data_type = {Sr}, name = {s} }",
                 to_string__Debug__CIDataTypeArrayKind(self->kind),
                 to_string__Debug__CIDataType(self->data_type),
                 self->name ? self->name->buffer : "NULL");
}
#endif

DESTRUCTOR(CIDataTypeArray, const CIDataTypeArray *self)
{
    FREE(CIDataType, self->data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeFunction, const CIDataTypeFunction *self)
{
    String *res =
      format__String("CIDataTypeFunction{{ name = {S}, params =", self->name);

    DEBUG_VEC_STRING(self->params, res, CIDataType);

    {
        String *s =
          format__String(", return_data_type = {Sr} }",
                         to_string__Debug__CIDataType(self->return_data_type));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIDataTypeFunction, const CIDataTypeFunction *self)
{
    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, CIDataType);
    FREE(Vec, self->params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeStruct, const CIDataTypeStruct *self)
{
    String *res = format__String(
      "CIDataTypeStruct{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, CIDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIDataTypeStruct, const CIDataTypeStruct *self)
{
    FREE_BUFFER_ITEMS(
      self->generic_params->buffer, self->generic_params->len, CIDataType);
    FREE(Vec, self->generic_params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeUnion, const CIDataTypeUnion *self)
{
    String *res = format__String(
      "CIDataTypeUnion{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, CIDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIDataTypeUnion, const CIDataTypeUnion *self)
{
    FREE_BUFFER_ITEMS(
      self->generic_params->buffer, self->generic_params->len, CIDataType);
    FREE(Vec, self->generic_params);
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, array, CIDataTypeArray array)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_ARRAY;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, _atomic, CIDataType *_atomic)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND__ATOMIC;
    self->_atomic = _atomic;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, enum, String *enum_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_ENUM;
    self->enum_ = enum_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *,
                    CIDataType,
                    function,
                    CIDataTypeFunction function)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_FUNCTION;
    self->function = function;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, pre_const, CIDataType *pre_const)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_PRE_CONST;
    self->pre_const = pre_const;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *,
                    CIDataType,
                    post_const,
                    CIDataType *post_const)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_POST_CONST;
    self->post_const = post_const;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, ptr, CIDataType *ptr)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_PTR;
    self->ptr = ptr;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, struct, CIDataTypeStruct struct_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_STRUCT;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, union, CIDataTypeUnion union_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_UNION;
    self->union_ = union_;

    return self;
}

CONSTRUCTOR(CIDataType *, CIDataType, enum CIDataTypeKind kind)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = kind;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataType, const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return format__String(
              "CIDataType{{ kind = {s}, array = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeArray(&self->array));
        case CI_DATA_TYPE_KIND__ATOMIC:
            return format__String("CIDataType{{ kind = {s}, _atomic = {Sr} }",
                                  to_string__Debug__CIDataTypeKind(self->kind),
                                  to_string__Debug__CIDataType(self->_atomic));
        case CI_DATA_TYPE_KIND_ENUM:
            return format__String("CIDataType{{ kind = {s}, enum = {S} }",
                                  to_string__Debug__CIDataTypeKind(self->kind),
                                  self->enum_);
        case CI_DATA_TYPE_KIND_FUNCTION:
            return format__String(
              "CIDataType{{ kind = {s}, function = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeFunction(&self->function));
        case CI_DATA_TYPE_KIND_PRE_CONST:
            return format__String(
              "CIDataType{{ kind = {s}, pre_const = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataType(self->pre_const));
        case CI_DATA_TYPE_KIND_POST_CONST:
            return format__String(
              "CIDataType{{ kind = {s}, post_const = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataType(self->post_const));
        case CI_DATA_TYPE_KIND_PTR:
            return format__String("CIDataType{{ kind = {s}, ptr = {Sr} }",
                                  to_string__Debug__CIDataTypeKind(self->kind),
                                  to_string__Debug__CIDataType(self->ptr));
        case CI_DATA_TYPE_KIND_STRUCT:
            return format__String(
              "CIDataType{{ kind = {s}, struct_ = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeStruct(&self->struct_));
        case CI_DATA_TYPE_KIND_UNION:
            return format__String(
              "CIDataType{{ kind = {s}, union_ = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeUnion(&self->union_));
        default:
            return format__String("CIDataType{{ kind = {s} }",
                                  to_string__Debug__CIDataTypeKind(self->kind));
    }
}
#endif

VARIANT_DESTRUCTOR(CIDataType, array, CIDataType *self)
{
    FREE(CIDataTypeArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, _atomic, CIDataType *self)
{
    FREE(CIDataType, self->_atomic);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, enum, CIDataType *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self)
{
    FREE(CIDataTypeFunction, &self->function);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, pre_const, CIDataType *self)
{
    FREE(CIDataType, self->pre_const);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, post_const, CIDataType *self)
{
    FREE(CIDataType, self->post_const);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, ptr, CIDataType *self)
{
    FREE(CIDataType, self->ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, struct, CIDataType *self)
{
    FREE(CIDataTypeStruct, &self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, union, CIDataType *self)
{
    FREE(CIDataTypeUnion, &self->union_);
    lily_free(self);
}

DESTRUCTOR(CIDataType, CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            FREE_VARIANT(CIDataType, array, self);
            break;
        case CI_DATA_TYPE_KIND__ATOMIC:
            FREE_VARIANT(CIDataType, _atomic, self);
            break;
        case CI_DATA_TYPE_KIND_ENUM:
            FREE_VARIANT(CIDataType, enum, self);
            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            FREE_VARIANT(CIDataType, function, self);
            break;
        case CI_DATA_TYPE_KIND_PRE_CONST:
            FREE_VARIANT(CIDataType, pre_const, self);
            break;
        case CI_DATA_TYPE_KIND_POST_CONST:
            FREE_VARIANT(CIDataType, post_const, self);
            break;
        case CI_DATA_TYPE_KIND_PTR:
            FREE_VARIANT(CIDataType, ptr, self);
            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            FREE_VARIANT(CIDataType, struct, self);
            break;
        case CI_DATA_TYPE_KIND_UNION:
            FREE_VARIANT(CIDataType, union, self);
            break;
        default:
            lily_free(self);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIStorageClass, int storage_class_flag)
{
    switch (storage_class_flag) {
        case CI_STORAGE_CLASS_NONE:
            return "CI_STORAGE_CLASS_NONE";
        case CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_CONSTEXPR:
            return "CI_STORAGE_CLASS_CONSTEXPR";
        case CI_STORAGE_CLASS_EXTERN:
            return "CI_STORAGE_CLASS_EXTERN";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO | "
                   "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC | "
                   "CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC | "
                   "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO | CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC | "
                   "CI_STORAGE_CLASS_AUTO | CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_INLINE:
            return "CI_STORAGE_CLASS_INLINE";
        case CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_REGISTER | CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_REGISTER | CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_STATIC:
            return "CI_STORAGE_CLASS_STATIC";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONSTEXPR:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONSTEXPR";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_EXTERN:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_EXTERN";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_INLINE:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_INLINE";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_THREAD_LOCAL:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_THREAD_LOCAL";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_TYPEDEF:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_TYPEDEF";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO | "
                   "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_THREAD_LOCAL:
            return "CI_STORAGE_CLASS_THREAD_LOCAL";
        case CI_STORAGE_CLASS_TYPEDEF:
            return "CI_STORAGE_CLASS_TYPEDEF";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

char *
to_string__CIStorageClass(int storage_class_flag)
{
    switch (storage_class_flag) {
        case CI_STORAGE_CLASS_NONE:
            return "";
        case CI_STORAGE_CLASS_AUTO:
            return "auto";
        case CI_STORAGE_CLASS_CONSTEXPR:
            return "constexpr";
        case CI_STORAGE_CLASS_EXTERN:
            return "extern";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO:
            return "extern auto";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "extern auto register";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO:
            return "extern static auto";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_REGISTER:
            return "extern static register";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO | CI_STORAGE_CLASS_REGISTER:
            return "extern static auto register";
        case CI_STORAGE_CLASS_INLINE:
            return "inline";
        case CI_STORAGE_CLASS_REGISTER:
            return "register";
        case CI_STORAGE_CLASS_REGISTER | CI_STORAGE_CLASS_AUTO:
            return "register auto";
        case CI_STORAGE_CLASS_STATIC:
            return "static";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO:
            return "static auto";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONSTEXPR:
            return "static constexpr";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_EXTERN:
            return "static extern";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_INLINE:
            return "static inline";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_REGISTER:
            return "static register";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_THREAD_LOCAL:
            return "static thread_local";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_TYPEDEF:
            return "static typedef";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "static auto register";
        case CI_STORAGE_CLASS_THREAD_LOCAL:
            return "thread_local";
        case CI_STORAGE_CLASS_TYPEDEF:
            return "typedef";
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDeclKind, enum CIDeclKind self)
{
    switch (self) {
        case CI_DECL_KIND_ENUM:
            return "CI_DECL_KIND_ENUM";
        case CI_DECL_KIND_FUNCTION:
            return "CI_DECL_KIND_FUNCTION";
        case CI_DECL_KIND_STRUCT:
            return "CI_DECL_KIND_STRUCT";
        case CI_DECL_KIND_UNION:
            return "CI_DECL_KIND_UNION";
        case CI_DECL_KIND_VARIABLE:
            return "CI_DECL_KIND_VARIABLE";
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclEnumVariantKind,
               enum CIDeclEnumVariantKind self)
{
    switch (self) {
        case CI_DECL_ENUM_VARIANT_KIND_CUSTOM:
            return "CI_DECL_ENUM_VARIANT_KIND_CUSTOM";
        case CI_DECL_ENUM_VARIANT_KIND_DEFAULT:
            return "CI_DECL_ENUM_VARIANT_KIND_DEFAULT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(CIDeclEnumVariant *,
                    CIDeclEnumVariant,
                    custom,
                    String *name,
                    Isize value)
{
    CIDeclEnumVariant *self = lily_malloc(sizeof(CIDeclEnumVariant));

    self->kind = CI_DECL_ENUM_VARIANT_KIND_CUSTOM;
    self->name = name;
    self->value = value;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclEnumVariant *,
                    CIDeclEnumVariant,
                    default,
                    String *name)
{
    CIDeclEnumVariant *self = lily_malloc(sizeof(CIDeclEnumVariant));

    self->kind = CI_DECL_ENUM_VARIANT_KIND_DEFAULT;
    self->name = name;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnumVariant, const CIDeclEnumVariant *self)
{
    switch (self->kind) {
        case CI_DECL_ENUM_VARIANT_KIND_CUSTOM:
            // TODO: change {d} format specifier.
            return format__String(
              "CIDeclEnumVariant{{ kind = {s}, name = {S}, value = {d} }",
              to_string__Debug__CIDeclEnumVariantKind(self->kind),
              self->name,
              self->value);
        case CI_DECL_ENUM_VARIANT_KIND_DEFAULT:
            return format__String(
              "CIDeclEnumVariant{{ kind = {s}, name = {S} }",
              to_string__Debug__CIDeclEnumVariantKind(self->kind),
              self->name);
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnum, const CIDeclEnum *self)
{
    String *res =
      format__String("CIDeclEnum{{ name = {S}, variants =", self->name);

    if (self->variants) {
        DEBUG_VEC_STRING(self->variants, res, CIDeclEnumVariant);
        push_str__String(res, " }");
    } else {
        push_str__String(res, " NULL");
    }

    return res;
}
#endif

DESTRUCTOR(CIDeclEnum, const CIDeclEnum *self)
{
    if (self->variants) {
        FREE_BUFFER_ITEMS(
          self->variants->buffer, self->variants->len, CIDeclEnumVariant);
        FREE(Vec, self->variants);
    }
}

CONSTRUCTOR(CIDeclFunctionParam *,
            CIDeclFunctionParam,
            String *name,
            CIDataType *data_type)
{
    CIDeclFunctionParam *self = lily_malloc(sizeof(CIDeclFunctionParam));

    self->name = name;
    self->data_type = data_type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionParam, const CIDeclFunctionParam *self)
{
    if (self->name) {
        return format__String(
          "CIDeclFunctionParam{{ name = {S}, data_type = {Sr} }",
          self->name,
          to_string__Debug__CIDataType(self->data_type));
    }

    return format__String(
      "CIDeclFunctionParam{{ name = NULL, data_type = {Sr} }",
      to_string__Debug__CIDataType(self->data_type));
}
#endif

DESTRUCTOR(CIDeclFunctionParam, CIDeclFunctionParam *self)
{
    FREE(CIDataType, self->data_type);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunction, const CIDeclFunction *self)
{
    String *res = format__String(
      "CIDeclFunction{{ name = {S}, return_data_type = {Sr}, params =",
      to_string__Debug__CIDataType(self->return_data_type));

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, CIDeclFunctionParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");

    if (self->body) {
        DEBUG_VEC_STRING(self->body, res, CIDeclFunctionItem);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIDeclFunction, const CIDeclFunction *self)
{
    FREE(CIDataType, self->return_data_type);

    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, CIDeclFunctionParam);
        FREE(Vec, self->params);
    }

    if (self->body) {
        FREE_BUFFER_ITEMS(
          self->body->buffer, self->body->len, CIDeclFunctionItem);
        FREE(Vec, self->body);
    }
}

CONSTRUCTOR(CIDeclStructField *,
            CIDeclStructField,
            String *name,
            CIDataType *data_type)
{
    CIDeclStructField *self = lily_malloc(sizeof(CIDeclStructField));

    self->name = name;
    self->data_type = data_type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStructField, const CIDeclStructField *self)
{
    return format__String("CIDeclStructField{{ name = {S}, data_type = {Sr} }",
                          self->name,
                          to_string__Debug__CIDataType(self->data_type));
}
#endif

DESTRUCTOR(CIDeclStructField, CIDeclStructField *self)
{
    FREE(CIDataType, self->data_type);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStruct, const CIDeclStruct *self)
{
    String *res =
      format__String("CIDeclStruct{{ name = {S}, fields =", self->name);

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
        push_str__String(res, " }");
    } else {
        push_str__String(res, " NULL");
    }

    return res;
}
#endif

DESTRUCTOR(CIDeclStruct, const CIDeclStruct *self)
{
    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclUnion, const CIDeclUnion *self)
{
    String *res =
      format__String("CIDeclUnion{{ name = {S}, fields =", self->name);

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
        push_str__String(res, " }");
    } else {
        push_str__String(res, " NULL");
    }

    return res;
}
#endif

DESTRUCTOR(CIDeclUnion, const CIDeclUnion *self)
{
    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclVariable, const CIDeclVariable *self)
{
    if (self->expr) {
        return format__String("CIDeclVariable{{ name = {S}, expr = {Sr} }",
                              self->name,
                              to_string__Debug__CIExpr(self->expr));
    }

    return format__String("CIDeclVariable{{ name = {S}, expr = NULL }",
                          self->name);
}
#endif

DESTRUCTOR(CIDeclVariable, const CIDeclVariable *self)
{
    if (self->expr) {
        FREE(CIExpr, self->expr);
    }
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    enum,
                    int storage_class_flag,
                    bool is_prototype,
                    String *typedef_name,
                    CIDeclEnum enum_)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_ENUM;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->typedef_name = typedef_name;
    self->enum_ = enum_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    function,
                    int storage_class_flag,
                    bool is_prototype,
                    CIDeclFunction function)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_FUNCTION;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->typedef_name = NULL;
    self->function = function;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    struct,
                    int storage_class_flag,
                    bool is_prototype,
                    String *typedef_name,
                    CIDeclStruct struct_)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_STRUCT;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->typedef_name = typedef_name;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    union,
                    int storage_class_flag,
                    bool is_prototype,
                    String *typedef_name,
                    CIDeclUnion union_)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_UNION;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->typedef_name = typedef_name;
    self->union_ = union_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    variable,
                    int storage_class_flag,
                    bool is_prototype,
                    CIDeclVariable variable)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_VARIABLE;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->typedef_name = NULL;
    self->variable = variable;

    return self;
}

String *
get_name__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            return self->enum_.name;
        case CI_DECL_KIND_FUNCTION:
            return self->function.name;
        case CI_DECL_KIND_STRUCT:
            return self->struct_.name;
        case CI_DECL_KIND_UNION:
            return self->union_.name;
        case CI_DECL_KIND_VARIABLE:
            return self->variable.name;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDecl, const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, typedef_name = {s}, enum_ = {Sr} }",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              self->typedef_name ? self->typedef_name->buffer : "NULL",
              to_string__Debug__CIDeclEnum(&self->enum_));
        case CI_DECL_KIND_FUNCTION:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, typedef_name = NULL, function = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclFunction(&self->function));
        case CI_DECL_KIND_STRUCT:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, typedef_name = {s}, struct_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              self->typedef_name ? self->typedef_name->buffer : "NULL",
              to_string__Debug__CIDeclStruct(&self->struct_));
        case CI_DECL_KIND_VARIABLE:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, typedef_name = NULL, variable = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclVariable(&self->variable));
        case CI_DECL_KIND_UNION:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, typedef_name = {s}, union_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              self->typedef_name ? self->typedef_name->buffer : "NULL",
              to_string__Debug__CIDeclUnion(&self->union_));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIDecl, enum, CIDecl *self)
{
    FREE(CIDeclEnum, &self->enum_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, function, CIDecl *self)
{
    FREE(CIDeclFunction, &self->function);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, struct, CIDecl *self)
{
    FREE(CIDeclStruct, &self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, union, CIDecl *self)
{
    FREE(CIDeclUnion, &self->union_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, variable, CIDecl *self)
{
    FREE(CIDeclVariable, &self->variable);
    lily_free(self);
}

DESTRUCTOR(CIDecl, CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            FREE_VARIANT(CIDecl, enum, self);
            break;
        case CI_DECL_KIND_FUNCTION:
            FREE_VARIANT(CIDecl, function, self);
            break;
        case CI_DECL_KIND_STRUCT:
            FREE_VARIANT(CIDecl, struct, self);
            break;
        case CI_DECL_KIND_UNION:
            FREE_VARIANT(CIDecl, union, self);
            break;
        case CI_DECL_KIND_VARIABLE:
            FREE_VARIANT(CIDecl, variable, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprBinaryKind, enum CIExprBinaryKind self)
{
    switch (self) {
        case CI_EXPR_BINARY_KIND_ASSIGN:
            return "CI_EXPR_BINARY_KIND_ASSIGN";
        case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
            return "CI_EXPR_BINARY_KIND_ASSIGN_ADD";
        case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
            return "CI_EXPR_BINARY_KIND_ASSIGN_SUB";
        case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
            return "CI_EXPR_BINARY_KIND_ASSIGN_MUL";
        case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
            return "CI_EXPR_BINARY_KIND_ASSIGN_DIV";
        case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
            return "CI_EXPR_BINARY_KIND_ASSIGN_MOD";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR";
        case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
            return "CI_EXPR_BINARY_KIND_ASSIGN_XOR";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT";
        case CI_EXPR_BINARY_KIND_ADD:
            return "CI_EXPR_BINARY_KIND_ADD";
        case CI_EXPR_BINARY_KIND_SUB:
            return "CI_EXPR_BINARY_KIND_SUB";
        case CI_EXPR_BINARY_KIND_MUL:
            return "CI_EXPR_BINARY_KIND_MUL";
        case CI_EXPR_BINARY_KIND_DIV:
            return "CI_EXPR_BINARY_KIND_DIV";
        case CI_EXPR_BINARY_KIND_MOD:
            return "CI_EXPR_BINARY_KIND_MOD";
        case CI_EXPR_BINARY_KIND_BIT_AND:
            return "CI_EXPR_BINARY_KIND_BIT_AND";
        case CI_EXPR_BINARY_KIND_BIT_OR:
            return "CI_EXPR_BINARY_KIND_BIT_OR";
        case CI_EXPR_BINARY_KIND_BIT_XOR:
            return "CI_EXPR_BINARY_KIND_BIT_XOR";
        case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
            return "CI_EXPR_BINARY_KIND_BIT_LSHIFT";
        case CI_EXPR_BINARY_KIND_BIT_RSHIFT:
            return "CI_EXPR_BINARY_KIND_BIT_RSHIFT";
        case CI_EXPR_BINARY_KIND_AND:
            return "CI_EXPR_BINARY_KIND_AND";
        case CI_EXPR_BINARY_KIND_OR:
            return "CI_EXPR_BINARY_KIND_OR";
        case CI_EXPR_BINARY_KIND_EQ:
            return "CI_EXPR_BINARY_KIND_EQ";
        case CI_EXPR_BINARY_KIND_NE:
            return "CI_EXPR_BINARY_KIND_NE";
        case CI_EXPR_BINARY_KIND_LESS:
            return "CI_EXPR_BINARY_KIND_LESS";
        case CI_EXPR_BINARY_KIND_GREATER:
            return "CI_EXPR_BINARY_KIND_GREATER";
        case CI_EXPR_BINARY_KIND_LESS_EQ:
            return "CI_EXPR_BINARY_KIND_LESS_EQ";
        case CI_EXPR_BINARY_KIND_GREATER_EQ:
            return "CI_EXPR_BINARY_KIND_GREATER_EQ";
        case CI_EXPR_BINARY_KIND_DOT:
            return "CI_EXPR_BINARY_KIND_DOT";
        case CI_EXPR_BINARY_KIND_ARROW:
            return "CI_EXPR_BINARY_KIND_ARROW";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprBinary, const CIExprBinary *self)
{
    return format__String(
      "CIExprBinary{{ kind = {s}, left = {Sr}, right = {Sr} }",
      to_string__Debug__CIExprBinaryKind(self->kind),
      to_string__Debug__CIExpr(self->left),
      to_string__Debug__CIExpr(self->right));
}
#endif

DESTRUCTOR(CIExprBinary, const CIExprBinary *self)
{
    FREE(CIExpr, self->left);
    FREE(CIExpr, self->right);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprLiteralKind, enum CIExprLiteralKind self)
{
    switch (self) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            return "CI_EXPR_LITERAL_KIND_BOOL";
        case CI_EXPR_LITERAL_KIND_CHAR:
            return "CI_EXPR_LITERAL_KIND_CHAR";
        case CI_EXPR_LITERAL_KIND_FLOAT:
            return "CI_EXPR_LITERAL_KIND_FLOAT";
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            return "CI_EXPR_LITERAL_KIND_SIGNED_INT";
        case CI_EXPR_LITERAL_KIND_STRING:
            return "CI_EXPR_LITERAL_KIND_STRING";
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            return "CI_EXPR_LITERAL_KIND_UNSIGNED_INT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprLiteral, const CIExprLiteral *self)
{
    switch (self->kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            return format__String(
              "CIExprLiteral{{ kind = {s}, bool_ = {b} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->bool_);
        case CI_EXPR_LITERAL_KIND_CHAR:
            return format__String(
              "CIExprLiteral{{ kind = {s}, char_ = {c} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->char_);
        case CI_EXPR_LITERAL_KIND_FLOAT:
            return format__String(
              "CIExprLiteral{{ kind = {s}, float_ = {f} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->float_);
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            // TODO: Update {d} format specifier.
            return format__String(
              "CIExprLiteral{{ kind = {s}, signed_int = {d} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->signed_int);
        case CI_EXPR_LITERAL_KIND_STRING:
            return format__String(
              "CIExprLiteral{{ kind = {s}, string = {S} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->string);
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            return format__String(
              "CIExprLiteral{{ kind = {s}, unsigned_int = {zu} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->unsigned_int);
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprUnaryKind, enum CIExprUnaryKind self)
{
    switch (self) {
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
            return "CI_EXPR_UNARY_KIND_PRE_INCREMENT";
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            return "CI_EXPR_UNARY_KIND_PRE_DECREMENT";
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
            return "CI_EXPR_UNARY_KIND_POST_INCREMENT";
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
            return "CI_EXPR_UNARY_KIND_POST_DECREMENT";
        case CI_EXPR_UNARY_KIND_POSITIVE:
            return "CI_EXPR_UNARY_KIND_POSITIVE";
        case CI_EXPR_UNARY_KIND_NEGATIVE:
            return "CI_EXPR_UNARY_KIND_NEGATIVE";
        case CI_EXPR_UNARY_KIND_BIT_NOT:
            return "CI_EXPR_UNARY_KIND_BIT_NOT";
        case CI_EXPR_UNARY_KIND_NOT:
            return "CI_EXPR_UNARY_KIND_NOT";
        case CI_EXPR_UNARY_KIND_DEREFERENCE:
            return "CI_EXPR_UNARY_KIND_DEREFERENCE";
        case CI_EXPR_UNARY_KIND_REF:
            return "CI_EXPR_UNARY_KIND_REF";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprUnary, const CIExprUnary *self)
{
    return format__String("CIExprUnary{{ kind = {s}, expr = {Sr} }",
                          to_string__Debug__CIExprUnaryKind(self->kind),
                          to_string__Debug__CIExpr(self->expr));
}
#endif

DESTRUCTOR(CIExprUnary, const CIExprUnary *self)
{
    FREE(CIExpr, self->expr);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprTernary, const CIExprTernary *self)
{
    return format__String(
      "CIExprTernary{{ cond = {Sr}, if_ = {Sr}, else_ = {Sr} }",
      to_string__Debug__CIExpr(self->cond),
      to_string__Debug__CIExpr(self->if_),
      to_string__Debug__CIExpr(self->else_));
}
#endif

DESTRUCTOR(CIExprTernary, const CIExprTernary *self)
{
    FREE(CIExpr, self->cond);
    FREE(CIExpr, self->if_);
    FREE(CIExpr, self->else_);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprCast, const CIExprCast *self)
{
    return format__String("CIExprCast{{ data_type = {Sr}, expr = {Sr} }",
                          to_string__Debug__CIDataType(self->data_type),
                          to_string__Debug__CIExpr(self->expr));
}
#endif

DESTRUCTOR(CIExprCast, const CIExprCast *self)
{
    FREE(CIDataType, self->data_type);
    FREE(CIExpr, self->expr);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprKind, enum CIExprKind self)
{
    switch (self) {
        case CI_EXPR_KIND_ALIGNOF:
            return "CI_EXPR_KIND_ALIGNOF";
        case CI_EXPR_KIND_BINARY:
            return "CI_EXPR_KIND_BINARY";
        case CI_EXPR_KIND_CAST:
            return "CI_EXPR_KIND_CAST";
        case CI_EXPR_KIND_DATA_TYPE:
            return "CI_EXPR_KIND_DATA_TYPE";
        case CI_EXPR_KIND_IDENTIFIER:
            return "CI_EXPR_KIND_IDENTIFIER";
        case CI_EXPR_KIND_LITERAL:
            return "CI_EXPR_KIND_LITERAL";
        case CI_EXPR_KIND_SIZEOF:
            return "CI_EXPR_KIND_SIZEOF";
        case CI_EXPR_KIND_TERNARY:
            return "CI_EXPR_KIND_TERNARY";
        case CI_EXPR_KIND_UNARY:
            return "CI_EXPR_KIND_UNARY";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, alignof, CIExpr *alignof_)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_ALIGNOF;
    self->alignof_ = alignof_;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, binary, CIExprBinary binary)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_BINARY;
    self->binary = binary;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, cast, CIExprCast cast)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_CAST;
    self->cast = cast;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, data_type, CIDataType *data_type)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_DATA_TYPE;
    self->data_type = data_type;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, identifier, String *identifier)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_IDENTIFIER;
    self->identifier = identifier;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, literal, CIExprLiteral literal)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_LITERAL;
    self->literal = literal;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, sizeof, CIExpr *sizeof_)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_SIZEOF;
    self->sizeof_ = sizeof_;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, ternary, CIExprTernary ternary)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_TERNARY;
    self->ternary = ternary;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, unary, CIExprUnary unary)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_UNARY;
    self->unary = unary;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExpr, const CIExpr *self)
{
    switch (self->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            return format__String("CIExpr{{ kind = {s}, alignof_ = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExpr(self->alignof_));
        case CI_EXPR_KIND_BINARY:
            return format__String(
              "CIExpr{{ kind = {s}, binary = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprBinary(&self->binary));
        case CI_EXPR_KIND_CAST:
            return format__String("CIExpr{{ kind = {s}, cast = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExprCast(&self->cast));
        case CI_EXPR_KIND_DATA_TYPE:
            return format__String(
              "CIExpr{{ kind = {s}, data_type = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIDataType(self->data_type));
        case CI_EXPR_KIND_IDENTIFIER:
            return format__String("CIExpr{{ kind = {s}, identifier = {S} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  self->identifier);
        case CI_EXPR_KIND_LITERAL:
            return format__String(
              "CIExpr{{ kind = {s}, literal = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprLiteral(&self->literal));
        case CI_EXPR_KIND_SIZEOF:
            return format__String("CIExpr{{ kind = {s}, sizeof_ = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExpr(self->sizeof_));
        case CI_EXPR_KIND_TERNARY:
            return format__String(
              "CIExpr{{ kind = {s}, ternary = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprTernary(&self->ternary));
        case CI_EXPR_KIND_UNARY:
            return format__String("CIExpr{{ kind = {s}, unary = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExprUnary(&self->unary));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIExpr, alignof, CIExpr *self)
{
    FREE(CIExpr, self->alignof_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, binary, CIExpr *self)
{
    FREE(CIExprBinary, &self->binary);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, cast, CIExpr *self)
{
    FREE(CIExprCast, &self->cast);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, data_type, CIExpr *self)
{
    FREE(CIDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, identifier, CIExpr *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, literal, CIExpr *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, sizeof, CIExpr *self)
{
    FREE(CIExpr, self->sizeof_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, ternary, CIExpr *self)
{
    FREE(CIExprTernary, &self->ternary);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, unary, CIExpr *self)
{
    FREE(CIExprUnary, &self->unary);
    lily_free(self);
}

DESTRUCTOR(CIExpr, CIExpr *self)
{
    switch (self->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            FREE_VARIANT(CIExpr, alignof, self);
            break;
        case CI_EXPR_KIND_BINARY:
            FREE_VARIANT(CIExpr, binary, self);
            break;
        case CI_EXPR_KIND_CAST:
            FREE_VARIANT(CIExpr, cast, self);
            break;
        case CI_EXPR_KIND_DATA_TYPE:
            FREE_VARIANT(CIExpr, data_type, self);
            break;
        case CI_EXPR_KIND_IDENTIFIER:
            FREE_VARIANT(CIExpr, identifier, self);
            break;
        case CI_EXPR_KIND_LITERAL:
            FREE_VARIANT(CIExpr, literal, self);
            break;
        case CI_EXPR_KIND_SIZEOF:
            FREE_VARIANT(CIExpr, sizeof, self);
            break;
        case CI_EXPR_KIND_TERNARY:
            FREE_VARIANT(CIExpr, ternary, self);
            break;
        case CI_EXPR_KIND_UNARY:
            FREE_VARIANT(CIExpr, unary, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIStmtKind, enum CIStmtKind self)
{
    switch (self) {
        case CI_STMT_KIND_BREAK:
            return "CI_STMT_KIND_BREAK";
        case CI_STMT_KIND_CONTINUE:
            return "CI_STMT_KIND_CONTINUE";
        case CI_STMT_KIND_DO_WHILE:
            return "CI_STMT_KIND_DO_WHILE";
        case CI_STMT_KIND_FOR:
            return "CI_STMT_KIND_FOR";
        case CI_STMT_KIND_GOTO:
            return "CI_STMT_KIND_GOTO";
        case CI_STMT_KIND_IF:
            return "CI_STMT_KIND_IF";
        case CI_STMT_KIND_RETURN:
            return "CI_STMT_KIND_RETURN";
        case CI_STMT_KIND_SWITCH:
            return "CI_STMT_KIND_SWITCH";
        case CI_STMT_KIND_WHILE:
            return "CI_STMT_KIND_WHILE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtDoWhile, const CIStmtDoWhile *self)
{
    String *res = format__String("CIStmtDoWhile{{ body =");

    DEBUG_VEC_STRING(self->body, res, CIDeclFunctionItem);

    {
        String *s = format__String(", cond = {Sr} }",
                                   to_string__Debug__CIExpr(self->cond));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIStmtDoWhile, const CIStmtDoWhile *self)
{
    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, CIDeclFunctionItem);
    FREE(Vec, self->body);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtFor, const CIStmtFor *self)
{
    String *res = format__String("CIStmtFor{ body =");

    DEBUG_VEC_STRING(self->body, res, CIDeclFunctionItem);

    if (self->init_clause) {
        String *s = format__String(
          ", init_clause = {Sr}",
          to_string__Debug__CIDeclFunctionItem(self->init_clause));

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, ", init_clause = NULL");
    }

    if (self->expr1) {
        String *s = format__String(", expr1 = {Sr}",
                                   to_string__Debug__CIExpr(self->expr1));

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, ", expr1 = NULL");
    }

    push_str__String(res, ", exprs2 =");

    if (self->exprs2) {
        DEBUG_VEC_STRING(self->exprs2, res, CIExpr);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIStmtFor, const CIStmtFor *self)
{
    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, CIDeclFunctionItem);
    FREE(Vec, self->body);

    if (self->init_clause) {
        FREE(CIDeclFunctionItem, self->init_clause);
    }

    if (self->expr1) {
        FREE(CIExpr, self->expr1);
    }

    if (self->exprs2) {
        FREE_BUFFER_ITEMS(self->exprs2->buffer, self->exprs2->len, CIExpr);
        FREE(Vec, self->exprs2);
    }
}

CONSTRUCTOR(CIStmtIfBranch *, CIStmtIfBranch, CIExpr *cond, Vec *body)
{
    CIStmtIfBranch *self = lily_malloc(sizeof(CIStmtIfBranch));

    self->cond = cond;
    self->body = body;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtIfBranch, const CIStmtIfBranch *self)
{
    String *res = format__String("CIStmtIfBranch{{ body =");

    DEBUG_VEC_STRING(self->body, res, CIDeclFunctionItem);

    {
        String *s = format__String(", cond = {Sr} }",
                                   to_string__Debug__CIExpr(self->cond));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIStmtIfBranch, CIStmtIfBranch *self)
{
    FREE(CIExpr, self->cond);
    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, CIDeclFunctionItem);
    FREE(Vec, self->body);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtIf, const CIStmtIf *self)
{
    String *res = format__String("CIStmtIf{{ if = {Sr}",
                                 to_string__Debug__CIStmtIfBranch(self->if_));

    push_str__String(res, ", else_ifs =");

    if (self->else_ifs) {
        DEBUG_VEC_STRING(self->else_ifs, res, CIStmtIfBranch);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", else_ =");

    if (self->else_) {
        DEBUG_VEC_STRING(self->else_, res, CIDeclFunctionItem);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIStmtIf, const CIStmtIf *self)
{
    FREE(CIStmtIfBranch, self->if_);

    if (self->else_ifs) {
        FREE_BUFFER_ITEMS(
          self->else_ifs->buffer, self->else_ifs->len, CIStmtIfBranch);
        FREE(Vec, self->else_ifs);
    }

    if (self->else_) {
        FREE_BUFFER_ITEMS(
          self->else_->buffer, self->else_->len, CIDeclFunctionItem);
        FREE(Vec, self->else_);
    }
}

CONSTRUCTOR(CIStmtSwitchCase *, CIStmtSwitchCase, CIExpr *value, Vec *body)
{
    CIStmtSwitchCase *self = lily_malloc(sizeof(CIStmtSwitchCase));

    self->value = value;
    self->body = body;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtSwitchCase, const CIStmtSwitchCase *self)
{
    String *res = format__String("CIStmtSwitchCase{{ body =");

    DEBUG_VEC_STRING(self->body, res, CIDeclFunctionItem);

    {
        String *s = format__String(", value = {Sr} }",
                                   to_string__Debug__CIExpr(self->value));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIStmtSwitchCase, CIStmtSwitchCase *self)
{
    FREE(CIExpr, self->value);
    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, CIDeclFunctionItem);
    FREE(Vec, self->body);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtSwitch, const CIStmtSwitch *self)
{
    String *res = format__String("CIStmtSwitch{{ cases =");

    DEBUG_VEC_STRING(self->cases, res, CIStmtSwitchCase);

    {
        String *s = format__String(", expr = {Sr} }",
                                   to_string__Debug__CIExpr(self->expr));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIStmtSwitch, const CIStmtSwitch *self)
{
    FREE(CIExpr, self->expr);
    FREE_BUFFER_ITEMS(self->cases->buffer, self->cases->len, CIStmtSwitchCase);
    FREE(Vec, self->cases);

    if (self->default_case) {
        FREE_BUFFER_ITEMS(self->default_case->buffer,
                          self->default_case->len,
                          CIDeclFunctionItem);
        FREE(Vec, self->default_case);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtWhile, const CIStmtWhile *self)
{
    String *res = format__String("CIStmtWhile{{ body =");

    DEBUG_VEC_STRING(self->body, res, CIDeclFunctionItem);

    {
        String *s = format__String(", cond = {Sr} }",
                                   to_string__Debug__CIExpr(self->cond));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIStmtWhile, const CIStmtWhile *self)
{
    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, CIDeclFunctionItem);
    FREE(Vec, self->body);
    FREE(CIExpr, self->cond);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmt, const CIStmt *self)
{
    switch (self->kind) {
        case CI_STMT_KIND_BREAK:
        case CI_STMT_KIND_CONTINUE:
            return format__String("CIStmt{ kind = {s} }",
                                  to_string__Debug__CIStmtKind(self->kind));
        case CI_STMT_KIND_DO_WHILE:
            return format__String(
              "CIStmt{{ kind = {s}, do_while = {Sr} }",
              to_string__Debug__CIStmtKind(self->kind),
              to_string__Debug__CIStmtDoWhile(&self->do_while));
        case CI_STMT_KIND_FOR:
            return format__String("CIStmt{{ kind = {s}, for_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIStmtFor(&self->for_));
        case CI_STMT_KIND_GOTO:
            return format__String("CIStmt{{ kind = {s}, goto_ = {S} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  self->goto_);
        case CI_STMT_KIND_IF:
            return format__String("CIStmt{{ kind = {s}, if_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIStmtIf(&self->if_));
        case CI_STMT_KIND_RETURN:
            return format__String("CIStmt{ kind = {s}, return_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIExpr(self->return_));
        case CI_STMT_KIND_SWITCH:
            return format__String(
              "CIStmt{{ kind = {s}, switch_ = {Sr} }",
              to_string__Debug__CIStmtKind(self->kind),
              to_string__Debug__CIStmtSwitch(&self->switch_));
        case CI_STMT_KIND_WHILE:
            return format__String("CIStmt{{ kind = {s}, while_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIStmtWhile(&self->while_));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIStmt, do_while, const CIStmt *self)
{
    FREE(CIStmtDoWhile, &self->do_while);
}

VARIANT_DESTRUCTOR(CIStmt, for, const CIStmt *self)
{
    FREE(CIStmtFor, &self->for_);
}

VARIANT_DESTRUCTOR(CIStmt, if, const CIStmt *self)
{
    FREE(CIStmtIf, &self->if_);
}

VARIANT_DESTRUCTOR(CIStmt, return, const CIStmt *self)
{
    FREE(CIExpr, self->return_);
}

VARIANT_DESTRUCTOR(CIStmt, switch, const CIStmt *self)
{
    FREE(CIStmtSwitch, &self->switch_);
}

VARIANT_DESTRUCTOR(CIStmt, while, const CIStmt *self)
{
    FREE(CIStmtWhile, &self->while_);
}

DESTRUCTOR(CIStmt, const CIStmt *self)
{
    switch (self->kind) {
        case CI_STMT_KIND_BREAK:
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_GOTO:
            break;
        case CI_STMT_KIND_DO_WHILE:
            FREE_VARIANT(CIStmt, do_while, self);
            break;
        case CI_STMT_KIND_FOR:
            FREE_VARIANT(CIStmt, for, self);
            break;
        case CI_STMT_KIND_IF:
            FREE_VARIANT(CIStmt, if, self);
            break;
        case CI_STMT_KIND_RETURN:
            FREE_VARIANT(CIStmt, return, self);
            break;
        case CI_STMT_KIND_SWITCH:
            FREE_VARIANT(CIStmt, switch, self);
            break;
        case CI_STMT_KIND_WHILE:
            FREE_VARIANT(CIStmt, while, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclFunctionItemKind,
               enum CIDeclFunctionItemKind self)
{
    switch (self) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            return "CI_DECL_FUNCTION_ITEM_KIND_DECL";
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            return "CI_DECL_FUNCTION_ITEM_KIND_EXPR";
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            return "CI_DECL_FUNCTION_ITEM_KIND_STMT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(CIDeclFunctionItem *,
                    CIDeclFunctionItem,
                    decl,
                    CIDecl *decl)
{
    CIDeclFunctionItem *self = lily_malloc(sizeof(CIDeclFunctionItem));

    self->kind = CI_DECL_FUNCTION_ITEM_KIND_DECL;
    self->decl = decl;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclFunctionItem *,
                    CIDeclFunctionItem,
                    expr,
                    CIExpr *expr)
{
    CIDeclFunctionItem *self = lily_malloc(sizeof(CIDeclFunctionItem));

    self->kind = CI_DECL_FUNCTION_ITEM_KIND_EXPR;
    self->expr = expr;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclFunctionItem *, CIDeclFunctionItem, stmt, CIStmt stmt)
{
    CIDeclFunctionItem *self = lily_malloc(sizeof(CIDeclFunctionItem));

    self->kind = CI_DECL_FUNCTION_ITEM_KIND_STMT;
    self->stmt = stmt;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionItem, const CIDeclFunctionItem *self)
{
    switch (self->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            return format__String(
              "CIDeclFunctionItem{{ kind = {s}, decl = {Sr} }",
              to_string__Debug__CIDeclFunctionItemKind(self->kind),
              to_string__Debug__CIDecl(self->decl));
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            return format__String(
              "CIDeclFunctionItem{{ kind = {s}, expr = {Sr} }",
              to_string__Debug__CIDeclFunctionItemKind(self->kind),
              to_string__Debug__CIExpr(self->expr));
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            return format__String(
              "CIDeclFunctionItem{{ kind = {s}, stmt = {Sr} }",
              to_string__Debug__CIDeclFunctionItemKind(self->kind),
              to_string__Debug__CIStmt(&self->stmt));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIDeclFunctionItem, decl, CIDeclFunctionItem *self)
{
    FREE(CIDecl, self->decl);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclFunctionItem, expr, CIDeclFunctionItem *self)
{
    FREE(CIExpr, self->expr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclFunctionItem, stmt, CIDeclFunctionItem *self)
{
    FREE(CIStmt, &self->stmt);
    lily_free(self);
}

DESTRUCTOR(CIDeclFunctionItem, CIDeclFunctionItem *self)
{
    switch (self->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            FREE_VARIANT(CIDeclFunctionItem, decl, self);
            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            FREE_VARIANT(CIDeclFunctionItem, expr, self);
            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            FREE_VARIANT(CIDeclFunctionItem, stmt, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
