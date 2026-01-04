/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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
#include <base/format.h>
#include <base/print.h>

#include <core/lily/parser/ast/data_type.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstDataTypeLambda type.
static DESTRUCTOR(LilyAstDataTypeLambda, const LilyAstDataTypeLambda *self);

// Free LilyAstDataTypeArray type.
static DESTRUCTOR(LilyAstDataTypeArray, const LilyAstDataTypeArray *self);

// Free LilyAstDataTypeCustom type.
static DESTRUCTOR(LilyAstDataTypeCustom, const LilyAstDataTypeCustom *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_ARRAY).
static VARIANT_DESTRUCTOR(LilyAstDataType, array, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_CUSTOM).
static VARIANT_DESTRUCTOR(LilyAstDataType, custom, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_LAMBDA).
static VARIANT_DESTRUCTOR(LilyAstDataType, lambda, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_LIST).
static VARIANT_DESTRUCTOR(LilyAstDataType, list, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_MUT).
static VARIANT_DESTRUCTOR(LilyAstDataType, mut, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_OPTIONAL).
static VARIANT_DESTRUCTOR(LilyAstDataType, optional, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(LilyAstDataType, ptr, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_REF).
static VARIANT_DESTRUCTOR(LilyAstDataType, ref, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_RESULT).
static VARIANT_DESTRUCTOR(LilyAstDataType, result, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_TRACE).
static VARIANT_DESTRUCTOR(LilyAstDataType, trace, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyAstDataType, tuple, LilyAstDataType *self);

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeLambda,
               const LilyAstDataTypeLambda *self)
{
    String *res = from__String("LilyAstDataTypeLambda{ params = ");

    if (self->params) {
        push_str__String(res, "{ ");

        for (Usize i = 0; i < self->params->len; i++) {
            String *s = format__String(
              "{Sr}, ",
              to_string__Debug__LilyAstDataType(get__Vec(self->params, i)));

            APPEND_AND_FREE(res, s);
        }
    } else {
        push_str__String(res, "NULL");
    }

    {
        String *s = format__String(
          "}, return_type = {Sr} }",
          self->return_type
            ? to_string__Debug__LilyAstDataType(self->return_type)
            : from__String("NULL"));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyAstDataTypeLambda, const LilyAstDataTypeLambda *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyAstDataType);
        FREE(Vec, self->params);
    }

    if (self->return_type)
        FREE(LilyAstDataType, self->return_type);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeArrayKind,
               enum LilyAstDataTypeArrayKind self)
{
    switch (self) {
        case LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC:
            return "LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC";
        case LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS:
            return "LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS";
        case LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED:
            return "LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED";
        case LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED:
            return "LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeArray,
               const LilyAstDataTypeArray *self)
{
    switch (self->kind) {
        case LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED:
            return format(
              "LilyAstDataTypeArray{{ kind = {s}, data_type = {Sr}, size = {d} "
              "}",
              to_string__Debug__LilyAstDataTypeArrayKind(self->kind),
              self->data_type
                ? to_string__Debug__LilyAstDataType(self->data_type)
                : from__String("NULL"),
              self->size);
        default:
            return format(
              "LilyAstDataTypeArray{{ kind = {s}, data_type = {Sr} }",
              to_string__Debug__LilyAstDataTypeArrayKind(self->kind),
              self->data_type
                ? to_string__Debug__LilyAstDataType(self->data_type)
                : from__String("NULL"));
    }
}
#endif

DESTRUCTOR(LilyAstDataTypeArray, const LilyAstDataTypeArray *self)
{
    if (self->data_type)
        FREE(LilyAstDataType, self->data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeCustom,
               const LilyAstDataTypeCustom *self)
{
    String *res = format__String(
      "LilyAstDataTypeCustom{{ name = {S}, generics = {{ ", self->name);

    if (self->generics) {
        for (Usize i = 0; i < self->generics->len; i++) {
            char *s = format(
              "{Sr}, ",
              to_string__Debug__LilyAstDataType(get__Vec(self->generics, i)));

            push_str__String(res, s);

            lily_free(s);
        }

        push__String(res, '}');
    } else {
        push_str__String(res, "NULL }");
    }

    return res;
}
#endif

DESTRUCTOR(LilyAstDataTypeCustom, const LilyAstDataTypeCustom *self)
{
    if (self->generics) {
        FREE_BUFFER_ITEMS(
          self->generics->buffer, self->generics->len, LilyAstDataType);
        FREE(Vec, self->generics);
    }

    FREE_MOVE(self->name, FREE(String, self->name));
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeResult,
               const LilyAstDataTypeResult *self)
{
    String *res = format__String("LilyAstDataTypeResult{{ ok = {Sr}, errs =",
                                 to_string__Debug__LilyAstDataType(self->ok));

    if (self->errs) {
        DEBUG_VEC_STRING(self->errs, res, LilyAstDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstDataTypeResult, const LilyAstDataTypeResult *self)
{
    FREE(LilyAstDataType, self->ok);

    if (self->errs) {
        FREE_BUFFER_ITEMS(self->errs->buffer, self->errs->len, LilyAstDataType);
        FREE(Vec, self->errs);
    }
}

CONSTRUCTOR(LilyAstDataType *,
            LilyAstDataType,
            enum LilyAstDataTypeKind kind,
            Location location)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = kind;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    array,
                    Location location,
                    LilyAstDataTypeArray array)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_ARRAY;
    self->location = location;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    custom,
                    Location location,
                    LilyAstDataTypeCustom custom)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_CUSTOM;
    self->location = location;
    self->custom = custom;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    lambda,
                    Location location,
                    LilyAstDataTypeLambda lambda)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_LAMBDA;
    self->location = location;
    self->lambda = lambda;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    list,
                    Location location,
                    LilyAstDataType *list)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_LIST;
    self->location = location;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    mut,
                    Location location,
                    LilyAstDataType *mut)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_MUT;
    self->location = location;
    self->mut = mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    optional,
                    Location location,
                    LilyAstDataType *optional)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_OPTIONAL;
    self->location = location;
    self->optional = optional;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    ptr,
                    Location location,
                    LilyAstDataType *ptr)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_PTR;
    self->location = location;
    self->ptr = ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    ref,
                    Location location,
                    LilyAstDataType *ref)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_REF;
    self->location = location;
    self->ref = ref;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    result,
                    Location location,
                    LilyAstDataTypeResult result)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_RESULT;
    self->location = location;
    self->result = result;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    trace,
                    Location location,
                    LilyAstDataType *trace)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_TRACE;
    self->location = location;
    self->trace = trace;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    tuple,
                    Location location,
                    Vec *tuple)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_TUPLE;
    self->location = location;
    self->tuple = tuple;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstDataTypeKind, enum LilyAstDataTypeKind self)
{
    switch (self) {
        case LILY_AST_DATA_TYPE_KIND_ANY:
            return "LILY_AST_DATA_TYPE_KIND_ANY";
        case LILY_AST_DATA_TYPE_KIND_ARRAY:
            return "LILY_AST_DATA_TYPE_KIND_ARRAY";
        case LILY_AST_DATA_TYPE_KIND_BOOL:
            return "LILY_AST_DATA_TYPE_KIND_BOOL";
        case LILY_AST_DATA_TYPE_KIND_BYTE:
            return "LILY_AST_DATA_TYPE_KIND_BIT_CHAR";
        case LILY_AST_DATA_TYPE_KIND_BYTES:
            return "LILY_AST_DATA_TYPE_KIND_BIT_STR";
        case LILY_AST_DATA_TYPE_KIND_CHAR:
            return "LILY_AST_DATA_TYPE_KIND_CHAR";
        case LILY_AST_DATA_TYPE_KIND_CSHORT:
            return "LILY_AST_DATA_TYPE_KIND_CSHORT";
        case LILY_AST_DATA_TYPE_KIND_CUSHORT:
            return "LILY_AST_DATA_TYPE_KIND_CUSHORT";
        case LILY_AST_DATA_TYPE_KIND_CINT:
            return "LILY_AST_DATA_TYPE_KIND_CINT";
        case LILY_AST_DATA_TYPE_KIND_CUINT:
            return "LILY_AST_DATA_TYPE_KIND_CUINT";
        case LILY_AST_DATA_TYPE_KIND_CLONG:
            return "LILY_AST_DATA_TYPE_KIND_CLONG";
        case LILY_AST_DATA_TYPE_KIND_CULONG:
            return "LILY_AST_DATA_TYPE_KIND_CULONG";
        case LILY_AST_DATA_TYPE_KIND_CLONGLONG:
            return "LILY_AST_DATA_TYPE_KIND_CLONGLONG";
        case LILY_AST_DATA_TYPE_KIND_CULONGLONG:
            return "LILY_AST_DATA_TYPE_KIND_CULONGLONG";
        case LILY_AST_DATA_TYPE_KIND_CFLOAT:
            return "LILY_AST_DATA_TYPE_KIND_CFLOAT";
        case LILY_AST_DATA_TYPE_KIND_CDOUBLE:
            return "LILY_AST_DATA_TYPE_KIND_CDOUBLE";
        case LILY_AST_DATA_TYPE_KIND_CSTR:
            return "LILY_AST_DATA_TYPE_KIND_CSTR";
        case LILY_AST_DATA_TYPE_KIND_CVOID:
            return "LILY_AST_DATA_TYPE_KIND_CVOID";
        case LILY_AST_DATA_TYPE_KIND_CUSTOM:
            return "LILY_AST_DATA_TYPE_KIND_CUSTOM";
        case LILY_AST_DATA_TYPE_KIND_FLOAT32:
            return "LILY_AST_DATA_TYPE_KIND_FLOAT32";
        case LILY_AST_DATA_TYPE_KIND_FLOAT64:
            return "LILY_AST_DATA_TYPE_KIND_FLOAT64";
        case LILY_AST_DATA_TYPE_KIND_INT16:
            return "LILY_AST_DATA_TYPE_KIND_INT16";
        case LILY_AST_DATA_TYPE_KIND_INT32:
            return "LILY_AST_DATA_TYPE_KIND_INT32";
        case LILY_AST_DATA_TYPE_KIND_INT64:
            return "LILY_AST_DATA_TYPE_KIND_INT64";
        case LILY_AST_DATA_TYPE_KIND_INT8:
            return "LILY_AST_DATA_TYPE_KIND_INT8";
        case LILY_AST_DATA_TYPE_KIND_ISIZE:
            return "LILY_AST_DATA_TYPE_KIND_ISIZE";
        case LILY_AST_DATA_TYPE_KIND_LAMBDA:
            return "LILY_AST_DATA_TYPE_KIND_LAMBDA";
        case LILY_AST_DATA_TYPE_KIND_LIST:
            return "LILY_AST_DATA_TYPE_KIND_LIST";
        case LILY_AST_DATA_TYPE_KIND_MUT:
            return "LILY_AST_DATA_TYPE_KIND_MUT";
        case LILY_AST_DATA_TYPE_KIND_NEVER:
            return "LILY_AST_DATA_TYPE_KIND_NEVER";
        case LILY_AST_DATA_TYPE_KIND_OBJECT:
            return "LILY_AST_DATA_TYPE_KIND_OBJECT";
        case LILY_AST_DATA_TYPE_KIND_OPTIONAL:
            return "LILY_AST_DATA_TYPE_KIND_OPTIONAL";
        case LILY_AST_DATA_TYPE_KIND_PTR:
            return "LILY_AST_DATA_TYPE_KIND_PTR";
        case LILY_AST_DATA_TYPE_KIND_REF:
            return "LILY_AST_DATA_TYPE_KIND_REF";
        case LILY_AST_DATA_TYPE_KIND_SELF:
            return "LILY_AST_DATA_TYPE_KIND_SELF";
        case LILY_AST_DATA_TYPE_KIND_STR:
            return "LILY_AST_DATA_TYPE_KIND_STR";
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            return "LILY_AST_DATA_TYPE_KIND_TRACE";
        case LILY_AST_DATA_TYPE_KIND_TUPLE:
            return "LILY_AST_DATA_TYPE_KIND_TUPLE";
        case LILY_AST_DATA_TYPE_KIND_UINT16:
            return "LILY_AST_DATA_TYPE_KIND_UINT16";
        case LILY_AST_DATA_TYPE_KIND_UINT32:
            return "LILY_AST_DATA_TYPE_KIND_UINT32";
        case LILY_AST_DATA_TYPE_KIND_UINT64:
            return "LILY_AST_DATA_TYPE_KIND_UINT64";
        case LILY_AST_DATA_TYPE_KIND_UINT8:
            return "LILY_AST_DATA_TYPE_KIND_UINT8";
        case LILY_AST_DATA_TYPE_KIND_UNIT:
            return "LILY_AST_DATA_TYPE_KIND_UNIT";
        case LILY_AST_DATA_TYPE_KIND_USIZE:
            return "LILY_AST_DATA_TYPE_KIND_USIZE";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyAstDataType, const LilyAstDataType *self)
{
    switch (self->kind) {
        case LILY_AST_DATA_TYPE_KIND_ARRAY:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, array = {sa} }",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataTypeArray(&self->array));
        case LILY_AST_DATA_TYPE_KIND_CUSTOM:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, custom = {Sr} }",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataTypeCustom(&self->custom));
        case LILY_AST_DATA_TYPE_KIND_LAMBDA:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, lambda = {Sr} }",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataTypeLambda(&self->lambda));
        case LILY_AST_DATA_TYPE_KIND_LIST:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, list = {Sr} }",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataType(self->list));
        case LILY_AST_DATA_TYPE_KIND_MUT:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, mut = {Sr} "
              "}",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataType(self->mut));
        case LILY_AST_DATA_TYPE_KIND_OPTIONAL:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, optional = {Sr} "
              "}",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataType(self->optional));
        case LILY_AST_DATA_TYPE_KIND_PTR:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, ptr = {Sr} "
              "}",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataType(self->ptr));
        case LILY_AST_DATA_TYPE_KIND_REF:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, ref = {Sr} "
              "}",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataType(self->ref));
        case LILY_AST_DATA_TYPE_KIND_RESULT:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, result = {Sr} "
              "}",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataTypeResult(&self->result));
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, trace = {Sr} "
              "}",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDataType(self->trace));
        case LILY_AST_DATA_TYPE_KIND_TUPLE: {
            String *res = format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa}, tuple = {{ ",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location));

            for (Usize i = 0; i < self->tuple->len; i++) {
                String *s = format__String(
                  "{Sr}, ",
                  to_string__Debug__LilyAstDataType(get__Vec(self->tuple, i)));

                APPEND_AND_FREE(res, s);
            }

            push_str__String(res, "} }");

            return res;
        }
        default:
            return format__String(
              "LilyAstDataType{{ kind = {s}, location = {sa} }",
              to_string__Debug__LilyAstDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location));
    }
}

void
IMPL_FOR_DEBUG(debug, LilyAstDataType, const LilyAstDataType *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyAstDataType(self));
}
#endif

String *
to_string__LilyAstDataType(const LilyAstDataType *self)
{
    switch (self->kind) {
        case LILY_AST_DATA_TYPE_KIND_ANY:
            return from__String("Any");
        case LILY_AST_DATA_TYPE_KIND_ARRAY: {
            String *res = NEW(String);

            switch (self->array.kind) {
                case LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC:
                    push_str__String(res, "[_]");
                    break;
                case LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS:
                    push_str__String(res, "[*]");
                    break;
                case LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED: {
                    String *s = format__String("[{d}]", self->array.size);

                    APPEND_AND_FREE(res, s);

                    break;
                }
                case LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED:
                    push_str__String(res, "[?]");
                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            if (self->array.data_type) {
                String *s = to_string__LilyAstDataType(self);

                APPEND_AND_FREE(res, s);
            }

            return res;
        }
        case LILY_AST_DATA_TYPE_KIND_BOOL:
            return from__String("Bool");
        case LILY_AST_DATA_TYPE_KIND_BYTE:
            return from__String("Byte");
        case LILY_AST_DATA_TYPE_KIND_BYTES:
            return from__String("Bytes");
        case LILY_AST_DATA_TYPE_KIND_CHAR:
            return from__String("Char");
        case LILY_AST_DATA_TYPE_KIND_CSHORT:
            return from__String("CShort");
        case LILY_AST_DATA_TYPE_KIND_CUSHORT:
            return from__String("CUshort");
        case LILY_AST_DATA_TYPE_KIND_CINT:
            return from__String("CInt");
        case LILY_AST_DATA_TYPE_KIND_CUINT:
            return from__String("CUint");
        case LILY_AST_DATA_TYPE_KIND_CLONG:
            return from__String("CLong");
        case LILY_AST_DATA_TYPE_KIND_CLONGLONG:
            return from__String("CLonglong");
        case LILY_AST_DATA_TYPE_KIND_CULONGLONG:
            return from__String("CULonglong");
        case LILY_AST_DATA_TYPE_KIND_CFLOAT:
            return from__String("CFloat");
        case LILY_AST_DATA_TYPE_KIND_CDOUBLE:
            return from__String("CDouble");
        case LILY_AST_DATA_TYPE_KIND_CSTR:
            return from__String("CStr");
        case LILY_AST_DATA_TYPE_KIND_CVOID:
            return from__String("CVoid");
        case LILY_AST_DATA_TYPE_KIND_CUSTOM: {
            String *res = format__String("{S}", self->custom.name);

            if (self->custom.generics) {
                push__String(res, '[');

                for (Usize i = 0; i < self->custom.generics->len; i++) {
                    String *s = to_string__LilyAstDataType(
                      get__Vec(self->custom.generics, i));

                    APPEND_AND_FREE(res, s);

                    if (i != self->custom.generics->len - 1) {
                        push_str__String(s, ", ");
                    }
                }
            }

            return res;
        }
        case LILY_AST_DATA_TYPE_KIND_RESULT: {
            String *res = NEW(String);

            if (self->result.errs) {
                if (self->result.errs->len == 1) {
                    String *s =
                      to_string__LilyAstDataType(last__Vec(self->result.errs));

                    APPEND_AND_FREE(res, s);
                } else {
                    push_str__String(res, "<");

                    for (Usize i = 0; i < self->result.errs->len; ++i) {
                        String *s = to_string__LilyAstDataType(
                          get__Vec(self->result.errs, i));

                        APPEND_AND_FREE(res, s);

                        if (i + 1 != self->result.errs->len) {
                            push_str__String(res, ", ");
                        }
                    }

                    push_str__String(res, ">");
                }
            }

            {
                char *s =
                  format("!{Sr}", to_string__LilyAstDataType(self->result.ok));

                PUSH_STR_AND_FREE(res, s);
            }

            return res;
        }
        case LILY_AST_DATA_TYPE_KIND_FLOAT32:
            return from__String("Float32");
        case LILY_AST_DATA_TYPE_KIND_FLOAT64:
            return from__String("Float64");
        case LILY_AST_DATA_TYPE_KIND_INT16:
            return from__String("Int16");
        case LILY_AST_DATA_TYPE_KIND_INT32:
            return from__String("Int32");
        case LILY_AST_DATA_TYPE_KIND_INT64:
            return from__String("Int64");
        case LILY_AST_DATA_TYPE_KIND_INT8:
            return from__String("Int8");
        case LILY_AST_DATA_TYPE_KIND_ISIZE:
            return from__String("Isize");
        case LILY_AST_DATA_TYPE_KIND_LAMBDA: {
            String *res = from__String("fun(");

            if (self->lambda.params) {
                for (Usize i = 0; i < self->lambda.params->len; i++) {
                    String *s = to_string__LilyAstDataType(
                      get__Vec(self->lambda.params, i));

                    APPEND_AND_FREE(res, s);

                    if (i != self->lambda.params->len - 1) {
                        push_str__String(res, ", ");
                    }
                }
            }

            push__String(res, ')');

            if (self->lambda.return_type) {
                push__String(res, ' ');

                String *s =
                  to_string__LilyAstDataType(self->lambda.return_type);

                APPEND_AND_FREE(res, s);
            }

            return res;
        }
        case LILY_AST_DATA_TYPE_KIND_LIST:
            return format__String("{{{}}",
                                  to_string__LilyAstDataType(self->list));
        case LILY_AST_DATA_TYPE_KIND_MUT:
            return format__String("mut {Sr}",
                                  to_string__LilyAstDataType(self->mut));
        case LILY_AST_DATA_TYPE_KIND_NEVER:
            return from__String("Never");
        case LILY_AST_DATA_TYPE_KIND_OBJECT:
            return from__String("Object");
        case LILY_AST_DATA_TYPE_KIND_OPTIONAL:
            return format__String("?{Sr}",
                                  to_string__LilyAstDataType(self->optional));
        case LILY_AST_DATA_TYPE_KIND_PTR:
            return format__String("*{Sr}",
                                  to_string__LilyAstDataType(self->ptr));
        case LILY_AST_DATA_TYPE_KIND_REF:
            return format__String("ref {Sr}",
                                  to_string__LilyAstDataType(self->ref));
        case LILY_AST_DATA_TYPE_KIND_SELF:
            return from__String("Self");
        case LILY_AST_DATA_TYPE_KIND_STR:
            return from__String("Str");
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            return format__String("trace {Sr}",
                                  to_string__LilyAstDataType(self->trace));
        case LILY_AST_DATA_TYPE_KIND_TUPLE: {
            String *res = from__String("(");

            for (Usize i = 0; i < self->tuple->len; i++) {
                String *s =
                  to_string__LilyAstDataType(get__Vec(self->tuple, i));

                APPEND_AND_FREE(res, s);

                if (i != self->tuple->len - 1) {
                    push_str__String(res, ", ");
                }
            }

            push__String(res, ')');

            return res;
        }
        case LILY_AST_DATA_TYPE_KIND_UINT16:
            return from__String("Uint16");
        case LILY_AST_DATA_TYPE_KIND_UINT32:
            return from__String("Uint32");
        case LILY_AST_DATA_TYPE_KIND_UINT64:
            return from__String("Uint64");
        case LILY_AST_DATA_TYPE_KIND_UINT8:
            return from__String("Uint8");
        case LILY_AST_DATA_TYPE_KIND_UNIT:
            return from__String("Unit");
        case LILY_AST_DATA_TYPE_KIND_USIZE:
            return from__String("Usize");
        default:
            UNREACHABLE("unknown variant");
    }
}

VARIANT_DESTRUCTOR(LilyAstDataType, array, LilyAstDataType *self)
{
    FREE(LilyAstDataTypeArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, custom, LilyAstDataType *self)
{
    FREE(LilyAstDataTypeCustom, &self->custom);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, lambda, LilyAstDataType *self)
{
    FREE(LilyAstDataTypeLambda, &self->lambda);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, list, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->list);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, mut, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->mut);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, optional, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->optional);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, ptr, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, ref, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->ref);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, result, LilyAstDataType *self)
{
    FREE(LilyAstDataTypeResult, &self->result);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, trace, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->trace);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, tuple, LilyAstDataType *self)
{
    FREE_BUFFER_ITEMS(self->tuple->buffer, self->tuple->len, LilyAstDataType);
    FREE(Vec, self->tuple);
    lily_free(self);
}

DESTRUCTOR(LilyAstDataType, LilyAstDataType *self)
{
    switch (self->kind) {
        case LILY_AST_DATA_TYPE_KIND_ARRAY:
            FREE_VARIANT(LilyAstDataType, array, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_CUSTOM:
            FREE_VARIANT(LilyAstDataType, custom, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_LAMBDA:
            FREE_VARIANT(LilyAstDataType, lambda, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_LIST:
            FREE_VARIANT(LilyAstDataType, list, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_MUT:
            FREE_VARIANT(LilyAstDataType, mut, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_OPTIONAL:
            FREE_VARIANT(LilyAstDataType, optional, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_PTR:
            FREE_VARIANT(LilyAstDataType, ptr, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_REF:
            FREE_VARIANT(LilyAstDataType, ref, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_RESULT:
            FREE_VARIANT(LilyAstDataType, result, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            FREE_VARIANT(LilyAstDataType, trace, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_TUPLE:
            FREE_VARIANT(LilyAstDataType, tuple, self);
            break;
        default:
            lily_free(self);
    }
}
