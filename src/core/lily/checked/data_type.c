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

#include <base/format.h>

#include <core/lily/checked/data_type.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/print.h>
#endif

// Free LilyCheckedDataTypeLambda type.
static DESTRUCTOR(LilyCheckedDataTypeLambda,
                  const LilyCheckedDataTypeLambda *self);

// Free LilyCheckedDataTypeArray type.
static DESTRUCTOR(LilyCheckedDataTypeArray,
                  const LilyCheckedDataTypeArray *self);

// Free LilyCheckedDataTypeCustom type.
static DESTRUCTOR(LilyCheckedDataTypeCustom,
                  const LilyCheckedDataTypeCustom *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_ARRAY).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          array,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_CUSTOM).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          custom,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          exception,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_LAMBDA).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          lambda,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_MUT).
static VARIANT_DESTRUCTOR(LilyCheckedDataType, mut, LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          optional,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(LilyCheckedDataType, ptr, LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_REF).
static VARIANT_DESTRUCTOR(LilyCheckedDataType, ref, LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_TRACE).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          trace,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          tuple,
                          LilyCheckedDataType *self);

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeLambda,
               const LilyCheckedDataTypeLambda *self)
{
    String *res = from__String("LilyCheckedDataTypeLambda{ params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", return_type = ");

    {
        String *s = to_string__Debug__LilyCheckedDataType(self->return_type);

        APPEND_AND_FREE(res, s);
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDataTypeLambda, const LilyCheckedDataTypeLambda *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedDataType);
        FREE(Vec, self->params);
    }

    FREE(LilyCheckedDataType, self->return_type);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeArrayKind,
               enum LilyCheckedDataTypeArrayKind self)
{
    switch (self) {
        case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_DYNAMIC:
            return "LILY_CHECKED_DATA_TYPE_ARRAY_KIND_DYNAMIC";
        case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS:
            return "LILY_CHECKED_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS";
        case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED:
            return "LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED";
        case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNDETERMINED:
            return "LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNDETERMINED";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

DESTRUCTOR(LilyCheckedDataTypeArray, const LilyCheckedDataTypeArray *self)
{
    FREE(LilyCheckedDataType, self->data_type);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeArray,
               const LilyCheckedDataTypeArray *self)
{
    if (self->kind == LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED) {
        return format(
          "LilyCheckedDataTypeArray{{ kind = {s}, data_type = {Sr}, size = {d} "
          "}",
          to_string__Debug__LilyCheckedDataTypeArrayKind(self->kind),
          to_string__Debug__LilyCheckedDataType(self->data_type),
          self->size);
    } else {
        return format(
          "LilyCheckedDataTypeArray{{ kind = {s}, data_type = {Sr} }",
          to_string__Debug__LilyCheckedDataTypeArrayKind(self->kind),
          to_string__Debug__LilyCheckedDataType(self->data_type));
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCustom,
               const LilyCheckedDataTypeCustom *self)
{
    String *res = format__String(
      "LilyCheckedDataTypeCustom{{ name = {S}, generics =", self->name);

    if (self->generics) {
        DEBUG_VEC_STRING(self->generics, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s =
          format(", kind = {s} }",
                 to_string__Debug__LilyCheckedDataTypeCustomKind(self->kind));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}

char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCustomKind,
               enum LilyCheckedDataTypeCustomKind self)
{
    switch (self) {
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_CLASS:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_CLASS";
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM";
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM_OBJECT:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM_OBJECT";
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD";
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD_OBJECT:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD_OBJECT";
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_TRAIT:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_TRAIT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

DESTRUCTOR(LilyCheckedDataTypeCustom, const LilyCheckedDataTypeCustom *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->generics) {
        FREE_BUFFER_ITEMS(
          self->generics->buffer, self->generics->len, LilyCheckedDataType);
        FREE(Vec, self->generics);
    }
}

CONSTRUCTOR(LilyCheckedDataType *,
            LilyCheckedDataType,
            enum LilyCheckedDataTypeKind kind,
            Location location)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = kind;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    array,
                    Location location,
                    LilyCheckedDataTypeArray array)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_ARRAY;
    self->location = location;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    custom,
                    Location location,
                    LilyCheckedDataTypeCustom custom)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_CUSTOM;
    self->location = location;
    self->custom = custom;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    exception,
                    Location location,
                    LilyCheckedDataType *exception)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION;
    self->location = location;
    self->exception = exception;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    lambda,
                    Location location,
                    LilyCheckedDataTypeLambda lambda)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_LAMBDA;
    self->location = location;
    self->lambda = lambda;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    mut,
                    Location location,
                    LilyCheckedDataType *mut)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_MUT;
    self->location = location;
    self->mut = mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    optional,
                    Location location,
                    LilyCheckedDataType *optional)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL;
    self->location = location;
    self->optional = optional;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ptr,
                    Location location,
                    LilyCheckedDataType *ptr)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_PTR;
    self->location = location;
    self->ptr = ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ref,
                    Location location,
                    LilyCheckedDataType *ref)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_REF;
    self->location = location;
    self->ref = ref;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    trace,
                    Location location,
                    LilyCheckedDataType *trace)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_TRACE;
    self->location = location;
    self->trace = trace;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    tuple,
                    Location location,
                    Vec *tuple)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_TUPLE;
    self->location = location;
    self->tuple = tuple;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeKind,
               enum LilyCheckedDataTypeKind self)
{
    switch (self) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
            return "LILY_CHECKED_DATA_TYPE_KIND_ANY";
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            return "LILY_CHECKED_DATA_TYPE_KIND_ARRAY";
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
            return "LILY_CHECKED_DATA_TYPE_KIND_BOOL";
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
            return "LILY_CHECKED_DATA_TYPE_KIND_BYTE";
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
            return "LILY_CHECKED_DATA_TYPE_KIND_BYTES";
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
            return "LILY_CHECKED_DATA_TYPE_KIND_CHAR";
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            return "LILY_CHECKED_DATA_TYPE_KIND_CUSTOM";
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            return "LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION";
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
            return "LILY_CHECKED_DATA_TYPE_KIND_FLOAT32";
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
            return "LILY_CHECKED_DATA_TYPE_KIND_FLOAT64";
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
            return "LILY_CHECKED_DATA_TYPE_KIND_INT16";
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
            return "LILY_CHECKED_DATA_TYPE_KIND_INT32";
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
            return "LILY_CHECKED_DATA_TYPE_KIND_INT64";
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
            return "LILY_CHECKED_DATA_TYPE_KIND_INT8";
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
            return "LILY_CHECKED_DATA_TYPE_KIND_ISIZE";
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            return "LILY_CHECKED_DATA_TYPE_KIND_LAMBDA";
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return "LILY_CHECKED_DATA_TYPE_KIND_MUT";
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
            return "LILY_CHECKED_DATA_TYPE_KIND_NEVER";
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return "LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL";
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return "LILY_CHECKED_DATA_TYPE_KIND_PTR";
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return "LILY_CHECKED_DATA_TYPE_KIND_REF";
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            return "LILY_CHECKED_DATA_TYPE_KIND_STR";
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return "LILY_CHECKED_DATA_TYPE_KIND_TRACE";
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
            return "LILY_CHECKED_DATA_TYPE_KIND_TUPLE";
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
            return "LILY_CHECKED_DATA_TYPE_KIND_UINT16";
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
            return "LILY_CHECKED_DATA_TYPE_KIND_UINT32";
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
            return "LILY_CHECKED_DATA_TYPE_KIND_UINT64";
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
            return "LILY_CHECKED_DATA_TYPE_KIND_UINT8";
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
            return "LILY_CHECKED_DATA_TYPE_KIND_UNIT";
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            return "LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN";
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return "LILY_CHECKED_DATA_TYPE_KIND_USIZE";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDataType, const LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, array = {Sr} "
              "}",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataTypeArray(&self->array));
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, custom = "
              "{Sr} }",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataTypeCustom(&self->custom));
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, exception = "
              "{Sr} "
              "}",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataType(self->exception));
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, lambda = "
              "{Sr} }",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataTypeLambda(&self->lambda));
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, mut = {Sr} "
              "}",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataType(self->mut));
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, optional = "
              "{Sr} "
              "}",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataType(self->optional));
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, ptr = {Sr} "
              "}",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataType(self->ptr));
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, ref = {Sr} "
              "}",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataType(self->ref));
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, trace = {Sr} "
              "}",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDataType(self->trace));
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            String *res = format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa}, tuple = {{ ",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location));

            for (Usize i = 0; i < self->tuple->len; i++) {
                String *s =
                  format__String("{Sr}, ",
                                 to_string__Debug__LilyCheckedDataType(
                                   get__Vec(self->tuple, i)));

                APPEND_AND_FREE(res, s);
            }

            push_str__String(res, "} }");

            return res;
        }
        default:
            return format__String(
              "LilyCheckedDataType{{ kind = {s}, location = {sa} }",
              to_string__Debug__LilyCheckedDataTypeKind(self->kind),
              to_string__Debug__Location(&self->location));
    }
}

void
IMPL_FOR_DEBUG(debug, LilyCheckedDataType, const LilyCheckedDataType *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyCheckedDataType(self));
}
#endif

String *
to_string__LilyCheckedDataType(const LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
            return from__String("Any");
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY: {
            String *res = NEW(String);

            switch (self->array.kind) {
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_DYNAMIC:
                    push_str__String(res, "[_]");
                    break;
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS:
                    push_str__String(res, "[*]");
                    break;
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED: {
                    String *s = format__String("[{d}]", self->array.size);

                    APPEND_AND_FREE(res, s);

                    break;
                }
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNDETERMINED:
                    push_str__String(res, "[?]");
                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            if (self->array.data_type) {
                String *s = to_string__LilyCheckedDataType(self);

                APPEND_AND_FREE(res, s);
            }

            return res;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
            return from__String("Bool");
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
            return from__String("Byte");
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
            return from__String("Bytes");
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
            return from__String("Char");
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM: {
            String *res = format__String("{S}", self->custom.name);

            if (self->custom.generics) {
                push__String(res, '[');

                for (Usize i = 0; i < self->custom.generics->len; i++) {
                    String *s = to_string__LilyCheckedDataType(
                      get__Vec(self->custom.generics, i));

                    APPEND_AND_FREE(res, s);

                    if (i != self->custom.generics->len - 1) {
                        push_str__String(s, ", ");
                    }
                }
            }

            return res;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            return format__String(
              "!{Sr}", to_string__LilyCheckedDataType(self->exception));
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
            return from__String("Float32");
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
            return from__String("Float64");
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
            return from__String("Int16");
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
            return from__String("Int32");
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
            return from__String("Int64");
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
            return from__String("Int8");
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
            return from__String("Isize");
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA: {
            String *res = from__String("fun(");

            if (self->lambda.params) {
                for (Usize i = 0; i < self->lambda.params->len; i++) {
                    String *s = to_string__LilyCheckedDataType(
                      get__Vec(self->lambda.params, i));

                    APPEND_AND_FREE(res, s);

                    if (i != self->lambda.params->len - 1) {
                        push_str__String(res, ", ");
                    }
                }
            }

            push__String(res, ')');
            push__String(res, ' ');

            {
                String *s =
                  to_string__LilyCheckedDataType(self->lambda.return_type);

                APPEND_AND_FREE(res, s);
            }

            return res;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return format__String("mut {Sr}",
                                  to_string__LilyCheckedDataType(self->mut));
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
            return from__String("Never");
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return format__String(
              "?{Sr}", to_string__LilyCheckedDataType(self->optional));
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return format__String("*{Sr}",
                                  to_string__LilyCheckedDataType(self->ptr));
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return format__String("ref {Sr}",
                                  to_string__LilyCheckedDataType(self->ref));
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            return from__String("Str");
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return format__String("trace {Sr}",
                                  to_string__LilyCheckedDataType(self->trace));
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            String *res = from__String("(");

            for (Usize i = 0; i < self->tuple->len; i++) {
                String *s =
                  to_string__LilyCheckedDataType(get__Vec(self->tuple, i));

                APPEND_AND_FREE(res, s);

                if (i != self->tuple->len - 1) {
                    push_str__String(res, ", ");
                }
            }

            push__String(res, ')');

            return res;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
            return from__String("Uint16");
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
            return from__String("Uint32");
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
            return from__String("Uint64");
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
            return from__String("Uint8");
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
            return from__String("Unit");
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            return from__String("Unknown");
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return from__String("Usize");
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
eq__LilyCheckedDataType(const LilyCheckedDataType *self,
                        const LilyCheckedDataType *other)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return other->kind == self->kind;
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
                    return self->array.kind == other->array.kind
                             ? self->array.kind ==
                                   LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED
                                 ? self->array.size == other->array.size &&
                                     eq__LilyCheckedDataType(
                                       self->array.data_type,
                                       other->array.data_type)
                                 : eq__LilyCheckedDataType(
                                     self->array.data_type,
                                     other->array.data_type)
                             : 0;
                default:
                    return false;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                    // TODO: compare generics
                    return !strcmp(self->custom.name->buffer,
                                   other->custom.name->buffer) &&
                           self->custom.kind == other->custom.kind;
                default:
                    return false;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->exception,
                                               other->exception)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            if (self->lambda.params->len == other->lambda.params->len) {
                for (Usize i = 0; i < self->lambda.params->len; ++i) {
                    if (!eq__LilyCheckedDataType(
                          get__Vec(self->lambda.params, i),
                          get__Vec(other->lambda.params, i))) {
                        return false;
                    }
                }
            } else {
                return false;
            }

            return eq__LilyCheckedDataType(self->lambda.return_type,
                                           other->lambda.return_type);
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->mut, other->mut)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->optional, other->optional)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->ptr, other->ptr)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->ref, other->ref)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->trace, other->trace)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
                    if (self->tuple->len == other->tuple->len) {
                        for (Usize i = 0; i < self->tuple->len; ++i) {
                            if (!eq__LilyCheckedDataType(
                                  get__Vec(self->tuple, i),
                                  get__Vec(other->tuple, i))) {
                                return false;
                            }
                        }

                        return true;
                    }

                    return false;
                default:
                    return false;
            }
        default:
            UNREACHABLE("unknown variant");
    }
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, array, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataTypeArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, custom, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataTypeCustom, &self->custom);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, exception, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->exception);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, lambda, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataTypeLambda, &self->lambda);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, mut, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->mut);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, optional, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->optional);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, ptr, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, ref, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->ref);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, trace, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->trace);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, tuple, LilyCheckedDataType *self)
{
    FREE_BUFFER_ITEMS(
      self->tuple->buffer, self->tuple->len, LilyCheckedDataType);
    FREE(Vec, self->tuple);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedDataType, LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            FREE_VARIANT(LilyCheckedDataType, array, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            FREE_VARIANT(LilyCheckedDataType, custom, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            FREE_VARIANT(LilyCheckedDataType, exception, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            FREE_VARIANT(LilyCheckedDataType, lambda, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            FREE_VARIANT(LilyCheckedDataType, mut, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            FREE_VARIANT(LilyCheckedDataType, optional, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            FREE_VARIANT(LilyCheckedDataType, ptr, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            FREE_VARIANT(LilyCheckedDataType, ref, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            FREE_VARIANT(LilyCheckedDataType, trace, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
            FREE_VARIANT(LilyCheckedDataType, tuple, self);
            break;
        default:
            lily_free(self);
    }
}
