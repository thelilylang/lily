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

#include <base/alloc.h>
#include <base/new.h>

#include <core/cc/ci/ast.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_ARRAY).
static VARIANT_DESTRUCTOR(CIDataType, array, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND__ATOMIC).
static VARIANT_DESTRUCTOR(CIDataType, _atomic, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(CIDataType, ptr, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_STRUCT).
static VARIANT_DESTRUCTOR(CIDataType, struct, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_UNION).
static VARIANT_DESTRUCTOR(CIDataType, union, CIDataType *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDataTypeKind, enum CIDataTypeKind self)
{
    switch (self) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return "CI_DATA_TYPE_KIND_ARRAY";
        case CI_DATA_TYPE_KIND__ATOMIC:
            return "CI_DATA_TYPE_KIND__ATOMIC";
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
        case CI_DATA_TYPE_KIND_PTR:
            return "CI_DATA_TYPE_KIND_PTR";
        case CI_DATA_TYPE_KIND_SHORT_INT:
            return "CI_DATA_TYPE_KIND_SHORT_INT";
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            return "CI_DATA_TYPE_KIND_SIGNED_CHAR";
        case CI_DATA_TYPE_KIND_STRUCT:
            return "CI_DATA_TYPE_KIND_STRUCT";
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
             ? format__String("CIDataTypeArray{{ kind = {s}, data_type = {Sr}, "
                              "size = {zu} }",
                              to_string__Debug__CIDataTypeArrayKind(self->kind),
                              to_string__Debug__CIDataType(self->data_type),
                              self->size)
             : format__String(
                 "CIDataTypeArray{{ kind = {s}, data_type = {Sr} }",
                 to_string__Debug__CIDataTypeArrayKind(self->kind),
                 to_string__Debug__CIDataType(self->data_type));
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
    String *res = from__String("CIDataTypeFunction{ params =");

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
        case CI_DATA_TYPE_KIND_FUNCTION:
            return format__String(
              "CIDataType{{ kind = {s}, function = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeFunction(&self->function));
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

VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self)
{
    FREE(CIDataTypeFunction, &self->function);
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
        case CI_DATA_TYPE_KIND_FUNCTION:
            FREE_VARIANT(CIDataType, function, self);
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
        case CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_CONST:
            return "CI_STORAGE_CLASS_CONST";
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
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONST:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONST";
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
        case CI_STORAGE_CLASS_AUTO:
            return "auto";
        case CI_STORAGE_CLASS_CONST:
            return "const";
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
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONST:
            return "static const";
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
IMPL_FOR_DEBUG(to_string, CIDeclEnumVariantKind, enum CIDeclEnumVariantKind self)
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
