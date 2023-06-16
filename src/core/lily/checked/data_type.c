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

#include <base/assert.h>
#include <base/format.h>

#include <core/lily/checked/data_type.h>
#include <core/lily/checked/scope.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_LIST).
static VARIANT_DESTRUCTOR(LilyCheckedDataType, list, LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_MUT).
static VARIANT_DESTRUCTOR(LilyCheckedDataType, mut, LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          optional,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(LilyCheckedDataType, ptr, LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          ptr_mut,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_REF).
static VARIANT_DESTRUCTOR(LilyCheckedDataType, ref, LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_REF_MUT).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          ref_mut,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_TRACE).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          trace,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          trace_mut,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          tuple,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type
// (LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          conditional_compiler_choice,
                          LilyCheckedDataType *self);

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          compiler_choice,
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
    String *res =
      format__String("LilyCheckedDataTypeCustom{{ scope_id = {d}, scope = "
                     "{sa}, name = {S}, global_name = {S}, generics =",
                     self->scope_id,
                     to_string__Debug__LilyCheckedAccessScope(&self->scope),
                     self->name,
                     self->global_name);

    if (self->generics) {
        DEBUG_VEC_STRING(self->generics, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s =
          format(", kind = {s}, is_recursive = {b} }",
                 to_string__Debug__LilyCheckedDataTypeCustomKind(self->kind),
                 self->is_recursive);

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
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ERROR:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ERROR";
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
    if (self->generics) {
        FREE_BUFFER_ITEMS(
          self->generics->buffer, self->generics->len, LilyCheckedDataType);
        FREE(Vec, self->generics);
    }
}

DESTRUCTOR(LilyCheckedDataTypeConditionalCompilerChoice,
           const LilyCheckedDataTypeConditionalCompilerChoice *self)
{
    FREE(Vec, self->choices);
    FREE_BUFFER_ITEMS(self->conds->buffer, self->conds->len, Vec);
    FREE(Vec, self->conds);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeConditionalCompilerChoice,
               const LilyCheckedDataTypeConditionalCompilerChoice *self)
{
    String *res = format__String(
      "LilyCheckedDataTypeConditionalCompilerChoice{{ choices =");

    DEBUG_VEC_STRING(self->choices, res, LilyCheckedDataType);
    push_str__String(res, ", conds =");

    DEBUG_VEC_STRING_2(self->conds, res, LilyCheckedDataType);
    push_str__String(res, " }");

    return res;
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCompilerGeneric,
               const LilyCheckedDataTypeCompilerGeneric *self)
{
    return format__String("LilyCheckedDataTypeCompilerGeneric{{ name = {S} }",
                          self->name);
}
#endif

CONSTRUCTOR(LilyCheckedDataType *,
            LilyCheckedDataType,
            enum LilyCheckedDataTypeKind kind,
            const Location *location)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = kind;
    self->location = location;
    self->ref_count = 0;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    array,
                    const Location *location,
                    LilyCheckedDataTypeArray array)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_ARRAY;
    self->location = location;
    self->ref_count = 0;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    bytes,
                    const Location *location,
                    Isize bytes)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_BYTES;
    self->location = location;
    self->ref_count = 0;
    self->bytes = bytes;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    custom,
                    const Location *location,
                    LilyCheckedDataTypeCustom custom)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_CUSTOM;
    self->location = location;
    self->ref_count = 0;
    self->custom = custom;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    exception,
                    const Location *location,
                    LilyCheckedDataType *exception)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION;
    self->location = location;
    self->ref_count = 0;
    self->exception = exception;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    lambda,
                    const Location *location,
                    LilyCheckedDataTypeLambda lambda)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_LAMBDA;
    self->location = location;
    self->ref_count = 0;
    self->lambda = lambda;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    list,
                    const Location *location,
                    LilyCheckedDataType *list)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_LIST;
    self->location = location;
    self->ref_count = 0;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    mut,
                    const Location *location,
                    LilyCheckedDataType *mut)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_MUT;
    self->location = location;
    self->ref_count = 0;
    self->mut = mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    optional,
                    const Location *location,
                    LilyCheckedDataType *optional)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL;
    self->location = location;
    self->ref_count = 0;
    self->optional = optional;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ptr,
                    const Location *location,
                    LilyCheckedDataType *ptr)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_PTR;
    self->location = location;
    self->ref_count = 0;
    self->ptr = ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ptr_mut,
                    const Location *location,
                    LilyCheckedDataType *ptr_mut)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT;
    self->location = location;
    self->ref_count = 0;
    self->ptr_mut = ptr_mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ref,
                    const Location *location,
                    LilyCheckedDataType *ref)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_REF;
    self->location = location;
    self->ref_count = 0;
    self->ref = ref;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ref_mut,
                    const Location *location,
                    LilyCheckedDataType *ref_mut)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_REF_MUT;
    self->location = location;
    self->ref_count = 0;
    self->ref_mut = ref_mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    str,
                    const Location *location,
                    Isize str)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_STR;
    self->location = location;
    self->ref_count = 0;
    self->str = str;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    trace,
                    const Location *location,
                    LilyCheckedDataType *trace)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_TRACE;
    self->location = location;
    self->ref_count = 0;
    self->trace = trace;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    trace_mut,
                    const Location *location,
                    LilyCheckedDataType *trace_mut)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT;
    self->location = location;
    self->ref_count = 0;
    self->trace_mut = trace_mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    tuple,
                    const Location *location,
                    Vec *tuple)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_TUPLE;
    self->location = location;
    self->ref_count = 0;
    self->tuple = tuple;

    return self;
}

VARIANT_CONSTRUCTOR(
  LilyCheckedDataType *,
  LilyCheckedDataType,
  conditional_compiler_choice,
  const Location *location,
  LilyCheckedDataTypeConditionalCompilerChoice conditional_compiler_choice)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE;
    self->location = location;
    self->ref_count = 0;
    self->conditional_compiler_choice = conditional_compiler_choice;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    compiler_choice,
                    const Location *location,
                    Vec *compiler_choice)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE;
    self->location = location;
    self->ref_count = 0;
    self->compiler_choice = compiler_choice;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    compiler_generic,
                    const Location *location,
                    LilyCheckedDataTypeCompilerGeneric compiler_generic)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC;
    self->location = location;
    self->ref_count = 0;
    self->compiler_generic = compiler_generic;

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
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
            return "LILY_CHECKED_DATA_TYPE_KIND_CSHORT";
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
            return "LILY_CHECKED_DATA_TYPE_KIND_CUSHORT";
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
            return "LILY_CHECKED_DATA_TYPE_KIND_CINT";
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
            return "LILY_CHECKED_DATA_TYPE_KIND_CUINT";
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
            return "LILY_CHECKED_DATA_TYPE_KIND_CLONG";
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
            return "LILY_CHECKED_DATA_TYPE_KIND_CULONG";
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
            return "LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG";
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
            return "LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG";
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
            return "LILY_CHECKED_DATA_TYPE_KIND_CFLOAT";
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
            return "LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE";
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
            return "LILY_CHECKED_DATA_TYPE_KIND_CSTR";
        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
            return "LILY_CHECKED_DATA_TYPE_KIND_CVOID";
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
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            return "LILY_CHECKED_DATA_TYPE_KIND_LIST";
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return "LILY_CHECKED_DATA_TYPE_KIND_MUT";
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
            return "LILY_CHECKED_DATA_TYPE_KIND_NEVER";
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return "LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL";
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return "LILY_CHECKED_DATA_TYPE_KIND_PTR";
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return "LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT";
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return "LILY_CHECKED_DATA_TYPE_KIND_REF";
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return "LILY_CHECKED_DATA_TYPE_KIND_REF_MUT";
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            return "LILY_CHECKED_DATA_TYPE_KIND_STR";
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return "LILY_CHECKED_DATA_TYPE_KIND_TRACE";
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return "LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT";
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
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            return "LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE";
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
            return "LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE";
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
            return "LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDataType, const LilyCheckedDataType *self)
{
    String *res = NULL;

    if (self->location) {
        res =
          format__String("LilyCheckedDataType{{ kind = {s}, location = {sa}",
                         to_string__Debug__LilyCheckedDataTypeKind(self->kind),
                         to_string__Debug__Location(self->location));
    } else {
        res =
          format__String("LilyCheckedDataType{{ kind = {s}, location = NULL",
                         to_string__Debug__LilyCheckedDataTypeKind(self->kind));
    }

    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY: {
            char *s =
              format(", array = {Sr} }",
                     to_string__Debug__LilyCheckedDataTypeArray(&self->array));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES: {
            char *s = format(", bytes = {d} }", self->bytes);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM: {
            char *s = format(
              ", custom = {Sr} }",
              to_string__Debug__LilyCheckedDataTypeCustom(&self->custom));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION: {
            char *s =
              format(", exception = {Sr} }",
                     to_string__Debug__LilyCheckedDataType(self->exception));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA: {
            char *s = format(
              ", lambda = {Sr} }",
              to_string__Debug__LilyCheckedDataTypeLambda(&self->lambda));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LIST: {
            char *s = format(", list = {Sr} }",
                             to_string__Debug__LilyCheckedDataType(self->list));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_MUT: {
            char *s = format(", mut = {Sr} }",
                             to_string__Debug__LilyCheckedDataType(self->mut));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL: {
            char *s =
              format(", optional = {Sr} }",
                     to_string__Debug__LilyCheckedDataType(self->optional));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_PTR: {
            char *s = format(", ptr = {Sr} }",
                             to_string__Debug__LilyCheckedDataType(self->ptr));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT: {
            char *s =
              format(", ptr_mut = {Sr} }",
                     to_string__Debug__LilyCheckedDataType(self->ptr_mut));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_REF: {
            char *s = format(", ref = {Sr} }",
                             to_string__Debug__LilyCheckedDataType(self->ref));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT: {
            char *s =
              format(", ref_mut = {Sr} }",
                     to_string__Debug__LilyCheckedDataType(self->ref_mut));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_STR: {
            char *s = format(", str = {d} }", self->str);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE: {
            char *s =
              format(", trace = {Sr} }",
                     to_string__Debug__LilyCheckedDataType(self->trace));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT: {
            char *s =
              format(", trace_mut = {Sr} }",
                     to_string__Debug__LilyCheckedDataType(self->trace_mut));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            push_str__String(res, ", tuple = { ");

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
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
            char *s = format(
              ", conditional_compiler_choice = {Sr} }",
              to_string__Debug__LilyCheckedDataTypeConditionalCompilerChoice(
                &self->conditional_compiler_choice));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE: {
            push_str__String(res, ", compiler_choice =");
            DEBUG_VEC_STRING(self->compiler_choice, res, LilyCheckedDataType);
            push_str__String(res, " }");

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC: {
            char *s =
              format(", compiler_generic = {Sr} }",
                     to_string__Debug__LilyCheckedDataTypeCompilerGeneric(
                       &self->compiler_generic));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            push_str__String(res, " }");
    }

    return res;
}

void
IMPL_FOR_DEBUG(debug, LilyCheckedDataType, const LilyCheckedDataType *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyCheckedDataType(self));
}
#endif

bool
eq__LilyCheckedDataType(const LilyCheckedDataType *self,
                        const LilyCheckedDataType *other)
{
    if (self->kind != other->kind &&
        other->kind == LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE) {
        return eq__LilyCheckedDataType(other, self);
    } else if (other->kind == LILY_CHECKED_DATA_TYPE_KIND_MUT &&
               self->kind != LILY_CHECKED_DATA_TYPE_KIND_MUT) {
        return eq__LilyCheckedDataType(other, self);
    }

    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
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
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return other->kind == self->kind;
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            return true;
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
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->list, other->list)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            // Mut isn't really a data type, it's just an alias to tell the
            // compiler that the data type is mutable. To take a concrete
            // example, it would be strange for the compiler to differentiate
            // between `mut Int8` and `Int8` types.
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->mut, other->mut)
                     : eq__LilyCheckedDataType(self->mut, other);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->optional, other->optional)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->ptr, other->ptr)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->ptr_mut, other->ptr_mut)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->ref, other->ref)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->ref_mut, other->ref_mut)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->trace, other->trace)
                     : false;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return self->kind == other->kind
                     ? eq__LilyCheckedDataType(self->trace_mut,
                                               other->trace_mut)
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
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
                    if (self->compiler_choice->len !=
                        other->compiler_choice->len) {
                        return false;
                    }

                    for (Usize i = 0; i < self->compiler_choice->len; ++i) {
                        if (!eq__LilyCheckedDataType(
                              get__Vec(self->compiler_choice, i),
                              get__Vec(other->compiler_choice, i))) {
                            return false;
                        }
                    }

                    return true;
                case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
                    if (self->compiler_choice->len !=
                        other->conditional_compiler_choice.choices->len) {
                        return false;
                    }

                    for (Usize i = 0; i < self->compiler_choice->len; ++i) {
                        LilyCheckedDataType *choice =
                          get__Vec(self->compiler_choice, i);
                        bool is_match = false;

                        for (Usize j = 0;
                             j <
                             other->conditional_compiler_choice.choices->len;
                             ++j) {
                            if (eq__LilyCheckedDataType(
                                  choice,
                                  get__Vec(
                                    other->conditional_compiler_choice.choices,
                                    j))) {
                                is_match = true;
                                break;
                            }
                        }

                        if (!is_match) {
                            return false;
                        }
                    }

                    return true;
                default:
                    return false;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
                    if (self->conditional_compiler_choice.choices->len !=
                        other->conditional_compiler_choice.choices->len) {
                        return false;
                    }

                    for (Usize i = 0;
                         i < self->conditional_compiler_choice.choices->len;
                         ++i) {
                        LilyCheckedDataType *choice = get__Vec(
                          self->conditional_compiler_choice.choices, i);
                        bool is_match = false;

                        for (Usize j = 0;
                             j <
                             other->conditional_compiler_choice.choices->len;
                             ++j) {
                            if (eq__LilyCheckedDataType(
                                  choice,
                                  get__Vec(
                                    other->conditional_compiler_choice.choices,
                                    j))) {
                                is_match = true;
                                break;
                            }
                        }

                        if (!is_match) {
                            return false;
                        }
                    }

                    return true;
                case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
                    if (self->conditional_compiler_choice.choices->len !=
                        other->compiler_choice->len) {
                        return false;
                    }

                    for (Usize i = 0;
                         i < self->conditional_compiler_choice.choices->len;
                         ++i) {
                        LilyCheckedDataType *choice = get__Vec(
                          self->conditional_compiler_choice.choices, i);
                        bool is_match = false;

                        for (Usize j = 0; j < other->compiler_choice->len;
                             ++j) {
                            if (eq__LilyCheckedDataType(
                                  choice,
                                  get__Vec(
                                    other->conditional_compiler_choice.choices,
                                    j))) {
                                is_match = true;
                                break;
                            }
                        }

                        if (!is_match) {
                            return false;
                        }
                    }

                    return true;
                default:
                    return false;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
                    return !strcmp(self->compiler_generic.name->buffer,
                                   other->compiler_generic.name->buffer);
                default:
                    return true;
            }
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
eq_return_data_type__LilyCheckedDataType(const LilyCheckedDataType *self,
                                         const LilyCheckedDataType *other)
{
    if (self->kind != other->kind &&
        other->kind ==
          LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE) {
        return eq_return_data_type__LilyCheckedDataType(other, self);
    }

    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            ASSERT(self->conditional_compiler_choice.conds->len ==
                   other->conditional_compiler_choice.conds->len);

            for (Usize i = 0; i < self->conditional_compiler_choice.conds->len;
                 ++i) {
                Vec *self_cond =
                  get__Vec(self->conditional_compiler_choice.conds, i);
                Vec *other_cond =
                  get__Vec(self->conditional_compiler_choice.conds, i);

                ASSERT(self_cond->len == other_cond->len);

                for (Usize j = 0; j < self_cond->len; ++j) {
                    if (!eq__LilyCheckedDataType(get__Vec(self_cond, j),
                                                 get__Vec(other_cond, j))) {
                        return false;
                    }
                }
            }

            return true;
        default:
            return eq__LilyCheckedDataType(self, other);
    }
}

LilyCheckedDataType *
get_return_data_type_of_conditional_compiler_choice(
  const LilyCheckedDataType *self,
  const Vec *cond)
{
    for (Usize i = 0; i < self->conditional_compiler_choice.conds->len; ++i) {
        Vec *self_cond = get__Vec(self->conditional_compiler_choice.conds, i);
        bool is_match = true;

        ASSERT(self_cond->len == cond->len);

        for (Usize j = 0; j < self_cond->len; ++j) {
            if (!eq__LilyCheckedDataType(get__Vec(self_cond, j),
                                         get__Vec(cond, j))) {
                is_match = false;
                break;
            }
        }

        if (is_match) {
            return get__Vec(self->conditional_compiler_choice.choices, i);
        }
    }

    return NULL;
}

LilyCheckedDataType *
clone__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return NEW(LilyCheckedDataType, self->kind, self->location);
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            switch (self->array.kind) {
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED:
                    return NEW_VARIANT(LilyCheckedDataType,
                                       array,
                                       self->location,
                                       NEW_VARIANT(LilyCheckedDataTypeArray,
                                                   sized,
                                                   clone__LilyCheckedDataType(
                                                     self->array.data_type),
                                                   self->array.size));
                default:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      array,
                      self->location,
                      NEW(LilyCheckedDataTypeArray,
                          self->array.kind,
                          clone__LilyCheckedDataType(self->array.data_type)));
            }
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
            return NEW_VARIANT(
              LilyCheckedDataType, bytes, self->location, self->bytes);
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM: {
            Vec *generics = NULL;

            if (self->custom.generics) {
                generics = NEW(Vec);

                for (Usize i = 0; i < self->custom.generics->len; ++i) {
                    push__Vec(generics,
                              clone__LilyCheckedDataType(
                                get__Vec(self->custom.generics, i)));
                }
            }

            return NEW_VARIANT(LilyCheckedDataType,
                               custom,
                               self->location,
                               NEW(LilyCheckedDataTypeCustom,
                                   self->custom.scope_id,
                                   self->custom.scope,
                                   self->custom.name,
                                   self->custom.global_name,
                                   generics,
                                   self->custom.kind,
                                   self->custom.is_recursive));
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA: {
            Vec *params = NULL;
            LilyCheckedDataType *return_type =
              clone__LilyCheckedDataType(self->lambda.return_type);

            if (self->lambda.params) {
                for (Usize i = 0; i < self->lambda.params->len; ++i) {
                    push__Vec(params,
                              clone__LilyCheckedDataType(
                                get__Vec(self->lambda.params, i)));
                }
            }

            return NEW_VARIANT(
              LilyCheckedDataType,
              lambda,
              self->location,
              NEW(LilyCheckedDataTypeLambda, params, return_type));
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            return NEW_VARIANT(LilyCheckedDataType,
                               list,
                               self->location,
                               clone__LilyCheckedDataType(self->list));
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return NEW_VARIANT(LilyCheckedDataType,
                               mut,
                               self->location,
                               clone__LilyCheckedDataType(self->mut));
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return NEW_VARIANT(LilyCheckedDataType,
                               optional,
                               self->location,
                               clone__LilyCheckedDataType(self->optional));
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return NEW_VARIANT(LilyCheckedDataType,
                               ptr,
                               self->location,
                               clone__LilyCheckedDataType(self->ptr));
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return NEW_VARIANT(LilyCheckedDataType,
                               ptr_mut,
                               self->location,
                               clone__LilyCheckedDataType(self->ptr_mut));
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return NEW_VARIANT(LilyCheckedDataType,
                               ref,
                               self->location,
                               clone__LilyCheckedDataType(self->ref));
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return NEW_VARIANT(LilyCheckedDataType,
                               ref_mut,
                               self->location,
                               clone__LilyCheckedDataType(self->ref_mut));
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            return NEW_VARIANT(
              LilyCheckedDataType, str, self->location, self->str);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return NEW_VARIANT(LilyCheckedDataType,
                               trace,
                               self->location,
                               clone__LilyCheckedDataType(self->trace));
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return NEW_VARIANT(LilyCheckedDataType,
                               trace_mut,
                               self->location,
                               clone__LilyCheckedDataType(self->trace_mut));
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            Vec *tuple = NEW(Vec);

            for (Usize i = 0; i < self->tuple->len; ++i) {
                push__Vec(tuple,
                          clone__LilyCheckedDataType(get__Vec(self->tuple, i)));
            }

            return NEW_VARIANT(
              LilyCheckedDataType, tuple, self->location, tuple);
        }
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
            Vec *choices = NEW(Vec);
            Vec *conds = NEW(Vec);

            for (Usize i = 0;
                 i < self->conditional_compiler_choice.choices->len;
                 ++i) {
                push__Vec(
                  choices,
                  get__Vec(self->conditional_compiler_choice.choices, i));
            }

            for (Usize i = 0; i < self->conditional_compiler_choice.conds->len;
                 ++i) {
                Vec *cond =
                  get__Vec(self->conditional_compiler_choice.conds, i);
                Vec *item = NEW(Vec);

                for (Usize j = 0; j < cond->len; ++j) {
                    push__Vec(item, get__Vec(cond, j));
                }

                push__Vec(conds, item);
            }

            return NEW_VARIANT(LilyCheckedDataType,
                               conditional_compiler_choice,
                               self->location,
                               NEW(LilyCheckedDataTypeConditionalCompilerChoice,
                                   choices,
                                   conds));
        }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE: {
            Vec *choices = NEW(Vec);

            for (Usize i = 0; i < self->compiler_choice->len; ++i) {
                push__Vec(choices, get__Vec(self->compiler_choice, i));
            }

            return NEW_VARIANT(
              LilyCheckedDataType, compiler_choice, self->location, choices);
        }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
            return NEW_VARIANT(LilyCheckedDataType,
                               compiler_generic,
                               self->location,
                               NEW(LilyCheckedDataTypeCompilerGeneric,
                                   self->compiler_generic.name));
        default:
            UNREACHABLE("unknown variant");
    }
}

Usize
get_integer_size__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
            return 1;
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
            return 2;
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
            return 4;
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
            return 8;
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return sizeof(Usize);
        default:
            UNREACHABLE("cannot get the integer size");
    }
}

bool
is_integer_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
            return true;
        default:
            return false;
    }
}

bool
is_float_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
            return true;
        default:
            return false;
    }
}

bool
contains_direct_custom_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            return true;
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->exception);
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->mut);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->optional);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->ptr);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->ptr_mut);
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->ref);
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->ref_mut);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->trace);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->trace_mut);
        default:
            return false;
    }
}

LilyCheckedDataType *
get_direct_custom_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            return self;
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            return get_direct_custom_data_type__LilyCheckedDataType(
              self->exception);
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return get_direct_custom_data_type__LilyCheckedDataType(self->mut);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return get_direct_custom_data_type__LilyCheckedDataType(
              self->optional);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return get_direct_custom_data_type__LilyCheckedDataType(self->ptr);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return get_direct_custom_data_type__LilyCheckedDataType(
              self->ptr_mut);
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return get_direct_custom_data_type__LilyCheckedDataType(self->ref);
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return get_direct_custom_data_type__LilyCheckedDataType(
              self->ref_mut);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return get_direct_custom_data_type__LilyCheckedDataType(
              self->trace);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return get_direct_custom_data_type__LilyCheckedDataType(
              self->trace_mut);
        default:
            return NULL;
    }
}

void
update_data_type__LilyCheckedDataType(LilyCheckedDataType *self,
                                      LilyCheckedDataType *other)
{
    // ASSERT((self->kind == LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN &&
    //         other->kind != LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) ||
    //        (self->kind == LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC &&
    //         other->kind != LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC));

    switch (other->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
            self->bytes = other->bytes;

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM: {
            Vec *generics = other->custom.generics ? NEW(Vec) : NULL;

            if (generics) {
                for (Usize i = 0; i < other->custom.generics->len; ++i) {
                    push__Vec(generics,
                              clone__LilyCheckedDataType(
                                get__Vec(other->custom.generics, i)));
                }
            }

            self->custom = NEW(LilyCheckedDataTypeCustom,
                               other->custom.scope_id,
                               other->custom.scope,
                               other->custom.name,
                               other->custom.global_name,
                               generics,
                               other->custom.kind,
                               other->custom.is_recursive);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA: {
            Vec *params = other->lambda.params ? NEW(Vec) : NULL;

            if (params) {
                for (Usize i = 0; i < other->lambda.params->len; ++i) {
                    push__Vec(params,
                              clone__LilyCheckedDataType(
                                get__Vec(other->lambda.params, i)));
                }
            }

            self->lambda =
              NEW(LilyCheckedDataTypeLambda,
                  params,
                  clone__LilyCheckedDataType(other->lambda.return_type));

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            self->list = clone__LilyCheckedDataType(other->list);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            self->mut = clone__LilyCheckedDataType(other->mut);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            self->optional = clone__LilyCheckedDataType(other->optional);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            self->ptr = clone__LilyCheckedDataType(other->ptr);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            self->ptr_mut = clone__LilyCheckedDataType(other->ptr_mut);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            self->ref = clone__LilyCheckedDataType(other->ref);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            self->ref_mut = clone__LilyCheckedDataType(other->ref_mut);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            self->str = other->str;

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            self->trace = clone__LilyCheckedDataType(other->trace);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            self->trace_mut = clone__LilyCheckedDataType(other->trace_mut);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            Vec *tuple = NEW(Vec);

            for (Usize i = 0; i < other->tuple->len; ++i) {
                push__Vec(
                  tuple, clone__LilyCheckedDataType(get__Vec(other->tuple, i)));
            }

            self->tuple = tuple;

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
            Vec *choices = NEW(Vec);
            Vec *conds = NEW(Vec);

            for (Usize i = 0;
                 i < other->conditional_compiler_choice.choices->len;
                 ++i) {
                push__Vec(
                  choices,
                  get__Vec(other->conditional_compiler_choice.choices, i));
            }

            for (Usize i = 0; i < other->conditional_compiler_choice.conds->len;
                 ++i) {
                Vec *cond =
                  get__Vec(other->conditional_compiler_choice.conds, i);
                Vec *item = NEW(Vec);

                for (Usize j = 0; j < cond->len; ++j) {
                    push__Vec(item, get__Vec(cond, j));
                }

                push__Vec(conds, item);
            }

            self->conditional_compiler_choice =
              NEW(LilyCheckedDataTypeConditionalCompilerChoice, choices, conds);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE: {
            Vec *compiler_choice = NEW(Vec);

            for (Usize i = 0; i < other->compiler_choice->len; ++i) {
                push__Vec(compiler_choice, get__Vec(other->compiler_choice, i));
            }

            self->compiler_choice = compiler_choice;

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
            self->compiler_generic = NEW(LilyCheckedDataTypeCompilerGeneric,
                                         other->compiler_generic.name);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    self->kind = other->kind;
}

bool
is_string_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
            return true;
        default:
            return false;
    }
}

void
serialize__LilyCheckedDataType(LilyCheckedDataType *self, String *ser)
{
    ASSERT(ser != NULL);

    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
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
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY: {
            char *s = format("{d}{d}", self->kind, self->array.kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->array.data_type, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM: {
            char *s = format("{d}{S}{d}",
                             self->kind,
                             self->custom.global_name,
                             self->custom.scope_id);

            PUSH_STR_AND_FREE(ser, s);

            if (self->custom.generics) {
                for (Usize i = 0; i < self->custom.generics->len; ++i) {
                    serialize__LilyCheckedDataType(
                      get__Vec(self->custom.generics, i), ser);
                }
            }

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            if (self->lambda.params) {
                for (Usize i = 0; i < self->lambda.params->len; ++i) {
                    serialize__LilyCheckedDataType(
                      get__Vec(self->lambda.params, i), ser);
                }
            }

            serialize__LilyCheckedDataType(self->lambda.return_type, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LIST: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->list, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_MUT: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->mut, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->optional, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_PTR: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->ptr, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->ptr_mut, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_REF: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->ref, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->ref_mut, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->trace, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            serialize__LilyCheckedDataType(self->trace_mut, ser);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            for (Usize i = 0; i < self->tuple->len; ++i) {
                serialize__LilyCheckedDataType(get__Vec(self->tuple, i), ser);
            }

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            for (Usize i = 0;
                 i < self->conditional_compiler_choice.choices->len;
                 ++i) {
                for (Usize j = 0;
                     j < self->conditional_compiler_choice.conds->len;
                     ++j) {
                    Vec *cond =
                      get__Vec(self->conditional_compiler_choice.conds, j);

                    for (Usize k = 0; k < cond->len; ++k) {
                        serialize__LilyCheckedDataType(get__Vec(cond, k), ser);
                    }
                }

                serialize__LilyCheckedDataType(
                  get__Vec(self->conditional_compiler_choice.choices, i), ser);
            }

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
            for (Usize i = 0; i < self->compiler_choice->len; ++i) {
                serialize__LilyCheckedDataType(
                  get__Vec(self->compiler_choice, i), ser);
            }

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC: {
            char *s = format("{d}{S}", self->kind, self->compiler_generic.name);

            PUSH_STR_AND_FREE(ser, s);

            break;
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

VARIANT_DESTRUCTOR(LilyCheckedDataType, list, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->list);
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

VARIANT_DESTRUCTOR(LilyCheckedDataType, ptr_mut, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->ptr_mut);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, ref, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->ref);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, ref_mut, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->ref_mut);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, trace, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->trace);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, trace_mut, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataType, self->trace_mut);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType, tuple, LilyCheckedDataType *self)
{
    FREE_BUFFER_ITEMS(
      self->tuple->buffer, self->tuple->len, LilyCheckedDataType);
    FREE(Vec, self->tuple);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType,
                   conditional_compiler_choice,
                   LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataTypeConditionalCompilerChoice,
         &self->conditional_compiler_choice);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDataType,
                   compiler_choice,
                   LilyCheckedDataType *self)
{
    FREE(Vec, self->compiler_choice);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedDataType, LilyCheckedDataType *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;

        return;
    }

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
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            FREE_VARIANT(LilyCheckedDataType, list, self);
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
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            FREE_VARIANT(LilyCheckedDataType, ptr_mut, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            FREE_VARIANT(LilyCheckedDataType, ref, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            FREE_VARIANT(LilyCheckedDataType, ref_mut, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            FREE_VARIANT(LilyCheckedDataType, trace, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            FREE_VARIANT(LilyCheckedDataType, trace_mut, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
            FREE_VARIANT(LilyCheckedDataType, tuple, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            FREE_VARIANT(
              LilyCheckedDataType, conditional_compiler_choice, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
            FREE_VARIANT(LilyCheckedDataType, compiler_choice, self);
            break;
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
            lily_free(self);
            break;
        default:
            lily_free(self);
    }
}
