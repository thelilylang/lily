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

#include <base/assert.h>
#include <base/format.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/expr/call.h>
#include <core/lily/analysis/checked/field.h>
#include <core/lily/analysis/checked/generic_param.h>
#include <core/lily/analysis/checked/scope.h>

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

// Free LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_RESULT).
static VARIANT_DESTRUCTOR(LilyCheckedDataType,
                          result,
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

static LilyCheckedDataType *
generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
  LilyCheckedDataType *self,
  const String *name,
  LilyCheckedDataType *original);

static void
remove_choice__LilyCheckedDataType(LilyCheckedDataType *self, Usize id);

#define GUARANTEE_COMPILER_DEFINED_DATA_TYPE(dt, min_choices, max_choices) \
    for (Usize i = 0; i < max_choices->len;) {                             \
        LilyCheckedDataType *data_type = get__Vec(max_choices, i);         \
        bool is_match = false;                                             \
                                                                           \
        for (Usize j = 0; j < min_choices->len; ++j) {                     \
            if (eq__LilyCheckedDataType(data_type,                         \
                                        get__Vec(min_choices, j))) {       \
                is_match = true;                                           \
                break;                                                     \
            }                                                              \
        }                                                                  \
                                                                           \
        if (!is_match) {                                                   \
            remove_choice__LilyCheckedDataType(dt, i);                     \
        } else {                                                           \
            ++i;                                                           \
        }                                                                  \
    }

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
        case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN:
            return "LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN";
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
          "LilyCheckedDataTypeArray{{ kind = {s}, data_type = {Sr}, sized = "
          "{d} "
          "}",
          to_string__Debug__LilyCheckedDataTypeArrayKind(self->kind),
          to_string__Debug__LilyCheckedDataType(self->data_type),
          self->sized);
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
        case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC:
            return "LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC";
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

CONSTRUCTOR(LilyCheckedDataTypeCondition *,
            LilyCheckedDataTypeCondition,
            Vec *params,
            Usize return_data_type_id)
{
    LilyCheckedDataTypeCondition *self =
      lily_malloc(sizeof(LilyCheckedDataTypeCondition));

    self->params = params;
    self->return_data_type_id = return_data_type_id;
    self->ref_count = 0;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCondition,
               const LilyCheckedDataTypeCondition *self)
{
    String *res = format__String("LilyCheckedDataTypeCondition{{ params =");

    DEBUG_VEC_STRING(self->params, res, LilyCheckedDataType);

    {
        char *s =
          format(", return_data_type_id = {d} }", self->return_data_type_id);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDataTypeCondition, LilyCheckedDataTypeCondition *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyCheckedDataType);
    FREE(Vec, self->params);
    lily_free(self);
}

Usize
add_choice__LilyCheckedDataTypeConditionalCompilerChoice(
  const LilyCheckedDataTypeConditionalCompilerChoice *self,
  LilyCheckedDataType *choice)
{
    for (Usize i = 0; i < self->choices->len; ++i) {
        if (eq__LilyCheckedDataType(get__Vec(self->choices, i), choice)) {
            return i;
        }
    }

    push__Vec(self->choices, choice);

    return self->choices->len - 1;
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

    DEBUG_VEC_STRING(self->conds, res, LilyCheckedDataTypeCondition);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDataTypeConditionalCompilerChoice,
           const LilyCheckedDataTypeConditionalCompilerChoice *self)
{
    FREE_BUFFER_ITEMS(
      self->conds->buffer, self->conds->len, LilyCheckedDataTypeCondition);
    FREE(Vec, self->conds);
    FREE(Vec, self->choices);
}

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

CONSTRUCTOR(LilyCheckedDataTypeResult *,
            LilyCheckedDataTypeResult,
            LilyCheckedDataType *ok,
            Vec *errs)
{
    LilyCheckedDataTypeResult *self =
      lily_malloc(sizeof(LilyCheckedDataTypeResult));

    self->ok = ok;
    self->errs = errs;
    self->ref_count = 0;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeResult,
               const LilyCheckedDataTypeResult *self)
{
    String *res =
      format__String("LilyCheckedDataTypeResult{{ ok = {Sr}, errs =",
                     to_string__Debug__LilyCheckedDataType(self->ok));

    if (self->errs) {
        DEBUG_VEC_STRING(self->errs, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDataTypeResult, LilyCheckedDataTypeResult *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    FREE(LilyCheckedDataType, self->ok);

    if (self->errs) {
        FREE_BUFFER_ITEMS(
          self->errs->buffer, self->errs->len, LilyCheckedDataType);
        FREE(Vec, self->errs);
    }

    lily_free(self);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeLen,
               const LilyCheckedDataTypeLen *self)
{
    if (self->is_undef) {
        return format(
          "LilyCheckedDataTypeLen{{ len = undef, is_undef = true }");
    }

    return format("LilyCheckedDataTypeLen{{ len = {zu}, is_undef = false }",
                  self->len);
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
    self->is_lock = true;
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
    self->is_lock = true;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    bytes,
                    const Location *location,
                    LilyCheckedDataTypeLen bytes)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_BYTES;
    self->location = location;
    self->ref_count = 0;
    self->is_lock = true;
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
    self->is_lock = true;
    self->custom = custom;

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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
    self->ref_mut = ref_mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    result,
                    const Location *location,
                    LilyCheckedDataTypeResult *result)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_RESULT;
    self->location = location;
    self->ref_count = 0;
    self->is_lock = true;
    self->result = result;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    str,
                    const Location *location,
                    LilyCheckedDataTypeLen str)
{
    LilyCheckedDataType *self = lily_malloc(sizeof(LilyCheckedDataType));

    self->kind = LILY_CHECKED_DATA_TYPE_KIND_STR;
    self->location = location;
    self->ref_count = 0;
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = true;
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
    self->is_lock = false;
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
    self->is_lock = false;
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
    self->is_lock = true;
    self->compiler_generic = compiler_generic;

    return self;
}

bool
eq__LilyCheckedDataType(LilyCheckedDataType *self, LilyCheckedDataType *other)
{
    if ((self->kind != other->kind &&
         self->kind !=
           LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE) &&
        other->kind == LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE) {
        return eq__LilyCheckedDataType(other, self);
    } else if ((self->kind != other->kind &&
                self->kind != LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE) &&
               other->kind ==
                 LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE) {
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
                    if (self->array.kind == other->array.kind) {
                        if (self->array.kind ==
                            LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED) {
                            return self->array.sized == other->array.sized &&
                                   eq__LilyCheckedDataType(
                                     self->array.data_type,
                                     other->array.data_type);
                        }

                        return eq__LilyCheckedDataType(self->array.data_type,
                                                       other->array.data_type);
                    } else if (self->array.kind != other->array.kind &&
                               self->array.kind ==
                                 LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN) {
                        self->array.kind = other->array.kind;

                        if (self->array.kind ==
                            LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED) {
                            self->array.sized = self->array.unknown;
                        }

                        return true;
                    } else if (self->array.kind != other->array.kind &&
                               other->array.kind ==
                                 LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
                        other->array.kind = self->array.kind;

                        if (other->array.kind ==
                            LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED) {
                            other->array.sized = other->array.unknown;
                        }

                        return true;
                    }

                    return false;
                default:
                    return false;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                    // TODO: compare the constraint data type for generic custom
                    // data type
                    if (self->custom.kind ==
                          LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC &&
                        other->custom.kind ==
                          LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC) {
                        return true;
                    }

                    // TODO: compare generics params of data type
                    return !strcmp(self->custom.global_name->buffer,
                                   other->custom.global_name->buffer) &&
                           self->custom.kind == other->custom.kind;
                default:
                    return false;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT: {
            if (self->kind == other->kind) {
                if (self->result->errs && other->result->errs) {
                    if (self->result->errs->len != other->result->errs->len) {
                        return false;
                    }

                    for (Usize i = 0; i < self->result->errs->len; ++i) {
                        if (!eq__LilyCheckedDataType(
                              get__Vec(self->result->errs, i),
                              get__Vec(other->result->errs, i))) {
                            return false;
                        }
                    }
                }

                return eq__LilyCheckedDataType(self->result->ok,
                                               other->result->ok);
            }

            return false;
        }
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
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL: {
            if (self->kind == other->kind) {
                return eq__LilyCheckedDataType(self->optional, other->optional);
            }

            if (other->ref_count == 0) {
                // Update to optional
                LilyCheckedDataType *tmp = NEW_VARIANT(
                  LilyCheckedDataType, optional, other->location, other);
                other = tmp;

                return eq__LilyCheckedDataType(self->optional, other->optional);
            }

            return false;
        }
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
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
                case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
                    Vec *self_choices = get_choices__LilyCheckedDataType(self);
                    Vec *other_choices =
                      get_choices__LilyCheckedDataType(other);

                    // Check if the data types are lock.
                    if (self->is_lock && other->is_lock) {
                        // There are no duplicate in each choices, so if the
                        // lenght of the choices are not equal, then the data
                        // types are not equal.
                        if (self_choices->len != other_choices->len) {
                            return false;
                        }

                        for (Usize i = 0; i < self_choices->len; ++i) {
                            LilyCheckedDataType *data_type =
                              get__Vec(self_choices, i);
                            bool is_match = false;

                            for (Usize j = 0; j < other_choices->len; ++j) {
                                if (eq__LilyCheckedDataType(
                                      data_type, get__Vec(other_choices, j))) {
                                    is_match = true;
                                    break;
                                }
                            }

                            if (!is_match) {
                                return false;
                            }
                        }

                        return true;
                    } else if (!self->is_lock && !other->is_lock) {
                        if (self_choices->len >= other_choices->len) {
                            GUARANTEE_COMPILER_DEFINED_DATA_TYPE(
                              self, self_choices, other_choices);
                        } else {
                            GUARANTEE_COMPILER_DEFINED_DATA_TYPE(
                              other, other_choices, self_choices);
                        }

                        if (self_choices->len != other_choices->len) {
                            return false;
                        }

                        return true;
                    } else if (!self->is_lock) {
                        GUARANTEE_COMPILER_DEFINED_DATA_TYPE(
                          other, other_choices, self_choices);

                        if (self_choices->len == other_choices->len &&
                            other_choices->len != 0) {
                            return true;
                        }

                        return false;
                    }

                    GUARANTEE_COMPILER_DEFINED_DATA_TYPE(
                      self, self_choices, other_choices);

                    if (self_choices->len == other_choices->len &&
                        self_choices->len != 0) {
                        return true;
                    }

                    return false;
                }
                default: {
                    Vec *self_choices = get_choices__LilyCheckedDataType(self);

                    if (self->is_lock) {
                        bool is_match = false;

                        for (Usize i = 0; i < self_choices->len; ++i) {
                            if (eq__LilyCheckedDataType(
                                  get__Vec(self_choices, i), other)) {
                                is_match = true;
                                break;
                            }
                        }

                        return is_match;
                    }

                    for (Usize i = 0; i < self_choices->len;) {
                        if (eq__LilyCheckedDataType(get__Vec(self_choices, i),
                                                    other)) {
                            ++i;
                        } else {
                            remove_choice__LilyCheckedDataType(self, i);
                        }
                    }

                    return self_choices->len > 0 ? true : false;
                }
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
eq_return_data_type__LilyCheckedDataType(LilyCheckedDataType *self,
                                         LilyCheckedDataType *other)
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
        LilyCheckedDataTypeCondition *self_cond =
          get__Vec(self->conditional_compiler_choice.conds, i);
        bool is_match = true;

        ASSERT(self_cond->params->len == cond->len);

        for (Usize j = 0; j < self_cond->params->len; ++j) {
            if (!eq__LilyCheckedDataType(get__Vec(self_cond->params, j),
                                         get__Vec(cond, j))) {
                is_match = false;
                break;
            }
        }

        if (is_match) {
            ASSERT(self_cond->return_data_type_id <
                   self->conditional_compiler_choice.choices->len);

            return get__Vec(self->conditional_compiler_choice.choices,
                            self_cond->return_data_type_id);
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
                                                   self->array.sized));
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
            return NEW_VARIANT(LilyCheckedDataType,
                               bytes,
                               self->location,
                               clone__LilyCheckedDataTypeLen(&self->bytes));
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
            return NEW_VARIANT(LilyCheckedDataType,
                               str,
                               self->location,
                               clone__LilyCheckedDataTypeLen(&self->str));
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
            Vec *choices = NEW(Vec); // Vec<LilyCheckedDataType* (&)>*
            Vec *conds = NEW(Vec);   // Vec<LilyCheckedDataTypeCondition*>*

            for (Usize i = 0;
                 i < self->conditional_compiler_choice.choices->len;
                 ++i) {
                push__Vec(
                  choices,
                  get__Vec(self->conditional_compiler_choice.choices, i));
            }

            for (Usize i = 0; i < self->conditional_compiler_choice.conds->len;
                 ++i) {
                LilyCheckedDataTypeCondition *cond =
                  get__Vec(self->conditional_compiler_choice.conds, i);
                Vec *params = NEW(Vec); // Vec<LilyCheckedDataType* (&)>*

                for (Usize i = 0; i < cond->params->len; ++i) {
                    push__Vec(
                      params,
                      ref__LilyCheckedDataType(get__Vec(cond->params, i)));
                }

                push__Vec(conds,
                          NEW(LilyCheckedDataTypeCondition,
                              params,
                              cond->return_data_type_id));
            }

            LilyCheckedDataType *res = NEW_VARIANT(
              LilyCheckedDataType,
              conditional_compiler_choice,
              self->location,
              NEW(
                LilyCheckedDataTypeConditionalCompilerChoice, choices, conds));

            res->is_lock = self->is_lock;

            return res;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE: {
            Vec *choices = NEW(Vec); // Vec<LilyCheckedDataType* (&)>*

            for (Usize i = 0; i < self->compiler_choice->len; ++i) {
                push__Vec(choices, get__Vec(self->compiler_choice, i));
            }

            LilyCheckedDataType *res = NEW_VARIANT(
              LilyCheckedDataType, compiler_choice, self->location, choices);

            res->is_lock = self->is_lock;

            return res;
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
is_signed_integer_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
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
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            return contains_direct_custom_data_type__LilyCheckedDataType(
              self->result->ok);
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
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            return get_direct_custom_data_type__LilyCheckedDataType(
              self->result->ok);
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
    if (!can_update__LilyCheckedDataType(self) ||
        other->kind == LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
        return;
    }

    // Update array data type (unknown)
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            switch (other->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
                    self->array.kind = other->array.kind;

                    if (self->array.kind ==
                        LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED) {
                        self->array.sized = self->array.unknown;
                    }

                    return;
                default:
                    return;
            }
        default:
            break;
    }

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
            self->bytes = NEW_VARIANT(LilyCheckedDataTypeLen, undef);

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
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            self->result = ref__LilyCheckedDataTypeResult(self->result);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            self->str = NEW_VARIANT(LilyCheckedDataTypeLen, undef);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            self->trace = clone__LilyCheckedDataType(other->trace);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            self->trace_mut = clone__LilyCheckedDataType(other->trace_mut);

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            Vec *tuple = NEW(Vec); // Vec<LilyCheckedDataType*>*

            for (Usize i = 0; i < other->tuple->len; ++i) {
                push__Vec(
                  tuple, clone__LilyCheckedDataType(get__Vec(other->tuple, i)));
            }

            self->tuple = tuple;

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
            Vec *choices = NEW(Vec); // Vec<LilyCheckedDataType* (&)>*
            Vec *conds = NEW(Vec);   // Vec<LilyCheckedDataTypeCondition*>*

            for (Usize i = 0;
                 i < other->conditional_compiler_choice.choices->len;
                 ++i) {
                push__Vec(
                  choices,
                  get__Vec(other->conditional_compiler_choice.choices, i));
            }

            for (Usize i = 0; i < other->conditional_compiler_choice.conds->len;
                 ++i) {
                push__Vec(conds,
                          ref__LilyCheckedDataTypeCondition(get__Vec(
                            other->conditional_compiler_choice.conds, i)));
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
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT: {
            char *s = format("{d}", self->kind);

            PUSH_STR_AND_FREE(ser, s);

            if (self->result->errs) {
                for (Usize i = 0; i < self->result->errs->len; ++i) {
                    serialize__LilyCheckedDataType(
                      get__Vec(self->result->errs, i), ser);
                }
            }

            serialize__LilyCheckedDataType(self->result->ok, ser);

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

#define RESOLVE_GENERIC_DATA_TYPE(fname, type, get)                            \
    LilyCheckedDataType *fname(LilyCheckedDataType *self,                      \
                               type *generic_params)                           \
    {                                                                          \
        ASSERT(self);                                                          \
                                                                               \
        switch (self->kind) {                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_ARRAY: {                          \
                LilyCheckedDataType *array_data_type =                         \
                  fname(self->array.data_type, generic_params);                \
                                                                               \
                if (!array_data_type) {                                        \
                    return NULL;                                               \
                }                                                              \
                                                                               \
                switch (self->array.kind) {                                    \
                    case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED:              \
                        return NEW_VARIANT(                                    \
                          LilyCheckedDataType,                                 \
                          array,                                               \
                          self->location,                                      \
                          NEW_VARIANT(LilyCheckedDataTypeArray,                \
                                      sized,                                   \
                                      array_data_type,                         \
                                      self->array.sized));                     \
                    default: {                                                 \
                        return NEW_VARIANT(LilyCheckedDataType,                \
                                           array,                              \
                                           self->location,                     \
                                           NEW(LilyCheckedDataTypeArray,       \
                                               self->array.kind,               \
                                               array_data_type));              \
                    }                                                          \
                }                                                              \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:                           \
                switch (self->custom.kind) {                                   \
                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC: {         \
                        LilyCheckedDataType *resolve =                         \
                          get(generic_params, self->custom.name->buffer);      \
                                                                               \
                        if (resolve) {                                         \
                            return clone__LilyCheckedDataType(resolve);        \
                        }                                                      \
                                                                               \
                        return NULL;                                           \
                    }                                                          \
                    default:                                                   \
                        if (self->custom.generics) {                           \
                            Vec *generics = NEW(Vec);                          \
                                                                               \
                            for (Usize i = 0; i < self->custom.generics->len;  \
                                 ++i) {                                        \
                                LilyCheckedDataType *generic =                 \
                                  fname(self, generic_params);                 \
                                                                               \
                                if (!generic) {                                \
                                    return NULL;                               \
                                }                                              \
                                                                               \
                                push__Vec(generics, generic);                  \
                            }                                                  \
                                                                               \
                            return NEW_VARIANT(                                \
                              LilyCheckedDataType,                             \
                              custom,                                          \
                              self->location,                                  \
                              NEW(LilyCheckedDataTypeCustom,                   \
                                  self->custom.scope_id,                       \
                                  (LilyCheckedAccessScope){                    \
                                    .id = self->custom.scope.id },             \
                                  self->custom.name,                           \
                                  self->custom.global_name,                    \
                                  generics,                                    \
                                  self->custom.kind,                           \
                                  self->custom.is_recursive));                 \
                        }                                                      \
                                                                               \
                        return clone__LilyCheckedDataType(self);               \
                }                                                              \
            case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA: {                         \
                Vec *params = NULL;                                            \
                LilyCheckedDataType *return_data_type =                        \
                  fname(self->lambda.return_type, generic_params);             \
                                                                               \
                if (!return_data_type) {                                       \
                    return NULL;                                               \
                }                                                              \
                                                                               \
                if (self->lambda.params) {                                     \
                    params = NEW(Vec);                                         \
                                                                               \
                    for (Usize i = 0; i < self->lambda.params->len; ++i) {     \
                        LilyCheckedDataType *param = fname(                    \
                          get__Vec(self->lambda.params, i), generic_params);   \
                                                                               \
                        if (param) {                                           \
                            push__Vec(params, param);                          \
                        } else {                                               \
                            return NULL;                                       \
                        }                                                      \
                    }                                                          \
                }                                                              \
                                                                               \
                return NEW_VARIANT(                                            \
                  LilyCheckedDataType,                                         \
                  lambda,                                                      \
                  self->location,                                              \
                  NEW(LilyCheckedDataTypeLambda, params, return_data_type));   \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_LIST: {                           \
                LilyCheckedDataType *list = fname(self->list, generic_params); \
                                                                               \
                return list                                                    \
                         ? NEW_VARIANT(                                        \
                             LilyCheckedDataType, list, self->location, list)  \
                         : NULL;                                               \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_MUT: {                            \
                LilyCheckedDataType *mut = fname(self->mut, generic_params);   \
                                                                               \
                return mut ? NEW_VARIANT(                                      \
                               LilyCheckedDataType, mut, self->location, mut)  \
                           : NULL;                                             \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL: {                       \
                LilyCheckedDataType *optional =                                \
                  fname(self->optional, generic_params);                       \
                                                                               \
                return optional ? NEW_VARIANT(LilyCheckedDataType,             \
                                              optional,                        \
                                              self->location,                  \
                                              optional)                        \
                                : NULL;                                        \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_PTR: {                            \
                LilyCheckedDataType *ptr = fname(self->ptr, generic_params);   \
                                                                               \
                return ptr ? NEW_VARIANT(                                      \
                               LilyCheckedDataType, ptr, self->location, ptr)  \
                           : NULL;                                             \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT: {                        \
                LilyCheckedDataType *ptr_mut =                                 \
                  fname(self->ptr_mut, generic_params);                        \
                                                                               \
                return ptr_mut ? NEW_VARIANT(LilyCheckedDataType,              \
                                             ptr_mut,                          \
                                             self->location,                   \
                                             ptr_mut)                          \
                               : NULL;                                         \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_REF: {                            \
                LilyCheckedDataType *ref = fname(self->ref, generic_params);   \
                                                                               \
                return ref ? NEW_VARIANT(                                      \
                               LilyCheckedDataType, ref, self->location, ref)  \
                           : NULL;                                             \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT: {                        \
                LilyCheckedDataType *ref_mut =                                 \
                  fname(self->ref_mut, generic_params);                        \
                                                                               \
                return ref_mut ? NEW_VARIANT(LilyCheckedDataType,              \
                                             ref_mut,                          \
                                             self->location,                   \
                                             ref_mut)                          \
                               : NULL;                                         \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_RESULT: {                         \
                Vec *errs = NULL; /* Vec<LilyCheckedDataType*>*? */            \
                                                                               \
                if (self->result->errs) {                                      \
                    errs = NEW(Vec);                                           \
                                                                               \
                    for (Usize i = 0; i < self->result->errs->len; ++i) {      \
                        LilyCheckedDataType *err = fname(                      \
                          get__Vec(self->result->errs, i), generic_params);    \
                                                                               \
                        if (!err) {                                            \
                            return NULL;                                       \
                        }                                                      \
                                                                               \
                        push__Vec(errs, err);                                  \
                    }                                                          \
                }                                                              \
                                                                               \
                LilyCheckedDataType *ok =                                      \
                  fname(self->result->ok, generic_params);                     \
                                                                               \
                return ok ? NEW_VARIANT(                                       \
                              LilyCheckedDataType,                             \
                              result,                                          \
                              self->location,                                  \
                              NEW(LilyCheckedDataTypeResult, ok, errs))        \
                          : NULL;                                              \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_TRACE: {                          \
                LilyCheckedDataType *trace =                                   \
                  fname(self->trace, generic_params);                          \
                                                                               \
                return trace ? NEW_VARIANT(LilyCheckedDataType,                \
                                           trace,                              \
                                           self->location,                     \
                                           trace)                              \
                             : NULL;                                           \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT: {                      \
                LilyCheckedDataType *trace_mut =                               \
                  fname(self->trace_mut, generic_params);                      \
                                                                               \
                return trace_mut ? NEW_VARIANT(LilyCheckedDataType,            \
                                               trace_mut,                      \
                                               self->location,                 \
                                               trace_mut)                      \
                                 : NULL;                                       \
            }                                                                  \
            case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {                          \
                Vec *tuple = NEW(Vec);                                         \
                                                                               \
                for (Usize i = 0; i < self->tuple->len; ++i) {                 \
                    LilyCheckedDataType *item =                                \
                      fname(get__Vec(self->tuple, i), generic_params);         \
                                                                               \
                    if (item) {                                                \
                        push__Vec(tuple, item);                                \
                    } else {                                                   \
                        return NULL;                                           \
                    }                                                          \
                }                                                              \
                                                                               \
                return NEW_VARIANT(                                            \
                  LilyCheckedDataType, tuple, self->location, tuple);          \
            }                                                                  \
            default:                                                           \
                return clone__LilyCheckedDataType(self);                       \
        }                                                                      \
    }

RESOLVE_GENERIC_DATA_TYPE(
  resolve_generic_data_type_with_ordered_hash_map__LilyCheckedDataType,
  OrderedHashMap,
  get__OrderedHashMap);

RESOLVE_GENERIC_DATA_TYPE(
  resolve_generic_data_type_with_hash_map__LilyCheckedDataType,
  HashMap,
  get__HashMap);

bool
contains_generic_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    ASSERT(self);

    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            return contains_generic_data_type__LilyCheckedDataType(
              self->array.data_type);
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            switch (self->custom.kind) {
                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC:
                    return true;
                default:
                    return false;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            if (self->result->errs) {
                for (Usize i = 0; i < self->result->errs->len; ++i) {
                    if (contains_generic_data_type__LilyCheckedDataType(
                          get__Vec(self->result->errs, i))) {
                        return true;
                    }
                }
            }

            return contains_generic_data_type__LilyCheckedDataType(
              self->result->ok);
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA: {
            if (self->lambda.params) {
                for (Usize i = 0; i < self->lambda.params->len; ++i) {
                    bool res = contains_generic_data_type__LilyCheckedDataType(
                      get__Vec(self->lambda.params, i));

                    if (res) {
                        return true;
                    }
                }
            }

            return contains_generic_data_type__LilyCheckedDataType(
              self->lambda.return_type);
        }
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            return contains_generic_data_type__LilyCheckedDataType(self->list);
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return contains_generic_data_type__LilyCheckedDataType(self->mut);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return contains_generic_data_type__LilyCheckedDataType(
              self->optional);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return contains_generic_data_type__LilyCheckedDataType(self->ptr);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return contains_generic_data_type__LilyCheckedDataType(
              self->ptr_mut);
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return contains_generic_data_type__LilyCheckedDataType(self->ref);
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return contains_generic_data_type__LilyCheckedDataType(
              self->ref_mut);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return contains_generic_data_type__LilyCheckedDataType(self->trace);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return contains_generic_data_type__LilyCheckedDataType(
              self->trace_mut);
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
            for (Usize i = 0; i < self->tuple->len; ++i) {
                bool res = contains_generic_data_type__LilyCheckedDataType(
                  get__Vec(self->tuple, i));

                if (res) {
                    return res;
                }
            }

            return false;
        default:
            return false;
    }
}

LilyCheckedDataType *
generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
  LilyCheckedDataType *self,
  const String *name,
  LilyCheckedDataType *original)
{
    if (self->kind == original->kind ||
        (self->kind == LILY_CHECKED_DATA_TYPE_KIND_CUSTOM
           ? self->custom.kind == LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC
           : 0)) {
        return NULL;
    }

    switch (original->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->array.data_type)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->array.data_type, name, original->array.data_type);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            switch (original->custom.kind) {
                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC:
                    if (!strcmp(original->custom.name->buffer, name->buffer)) {
                        return clone__LilyCheckedDataType(self);
                    }

                    return NULL;
                default:
                    return NULL;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            if (original->lambda.params) {
                ASSERT(self->lambda.params);
                ASSERT(original->lambda.params->len ==
                       self->lambda.params->len);

                for (Usize i = 0; i < original->lambda.params->len; ++i) {
                    LilyCheckedDataType *param =
                      get__Vec(original->lambda.params, i);

                    if (contains_generic_data_type__LilyCheckedDataType(
                          param)) {
                        LilyCheckedDataType *res =
                          generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                            get__Vec(self->lambda.params, i), name, param);

                        if (res) {
                            return res;
                        }
                    }
                }
            }

            if (contains_generic_data_type__LilyCheckedDataType(
                  original->lambda.return_type)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->lambda.return_type, name, original->lambda.return_type);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->list)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->list, name, original->list);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->mut)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->mut, name, original->mut);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->optional)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->optional, name, original->optional);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->ptr)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->ptr, name, original->ptr);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->ptr_mut)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->ptr_mut, name, original->ptr_mut);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->ref)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->ref, name, original->ref);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->ref_mut)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->ref_mut, name, original->ref_mut);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            if (contains_generic_data_type__LilyCheckedDataType(original)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self, name, original);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->trace)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->trace, name, original->trace);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            if (contains_generic_data_type__LilyCheckedDataType(
                  original->trace_mut)) {
                return generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                  self->trace_mut, name, original->trace_mut);
            }

            return NULL;
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
            ASSERT(original->tuple->len == self->tuple->len);

            for (Usize i = 0; i < original->tuple->len; ++i) {
                LilyCheckedDataType *item = get__Vec(original->tuple, i);

                if (contains_generic_data_type__LilyCheckedDataType(item)) {
                    LilyCheckedDataType *res =
                      generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                        get__Vec(self->tuple, i), name, item);

                    if (res) {
                        return res;
                    }
                }
            }

            return NULL;
        default:
            return NULL;
    }
}

void
remove_choice__LilyCheckedDataType(LilyCheckedDataType *self, Usize id)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            remove__Vec(self->conditional_compiler_choice.choices, id);

            for (Usize i = 0;
                 i < self->conditional_compiler_choice.conds->len;) {
                LilyCheckedDataTypeCondition *cond =
                  get__Vec(self->conditional_compiler_choice.conds, i);

                if (cond->return_data_type_id == id) {
                    FREE(
                      LilyCheckedDataTypeCondition,
                      remove__Vec(self->conditional_compiler_choice.conds, i));
                } else if (cond->return_data_type_id > id) {
                    --cond->return_data_type_id;
                    ++i;
                } else {
                    ++i;
                }
            }

            break;
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
            remove__Vec(self->compiler_choice, id);
            break;
        default:
            UNREACHABLE("not expected in this context");
    }
}

OrderedHashMap *
generate_generic_params_from_resolved_data_type__LilyCheckedDataType(
  LilyCheckedDataType *self,
  Vec *generic_params,
  LilyCheckedDataType *original)
{
    ASSERT(self && original);

    OrderedHashMap *resolved_generic_params = NEW(OrderedHashMap);

    for (Usize i = 0; i < generic_params->len; ++i) {
        const LilyCheckedGenericParam *generic_param =
          get__Vec(generic_params, i);
        const String *generic_param_name =
          get_name__LilyCheckedGenericParam(generic_param);
        LilyCheckedDataType *data_type_generic_param =
          generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
            self, generic_param_name, original);

        if (data_type_generic_param) {
            insert__OrderedHashMap(resolved_generic_params,
                                   generic_param_name->buffer,
                                   data_type_generic_param);
        } else {
            insert__OrderedHashMap(resolved_generic_params,
                                   generic_param_name->buffer,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                       self->location));
        }
    }

    return resolved_generic_params;
}

OrderedHashMap *
generate_generic_params_from_resolved_fields__LilyCheckedDataType(
  Vec *params,
  Vec *generic_params,
  Vec *fields)
{
    ASSERT(params && fields);
    ASSERT(params->len == fields->len);

    OrderedHashMap *resolved_generic_params = NEW(OrderedHashMap);

    for (Usize i = 0; i < fields->len; ++i) {
        LilyCheckedDataType *resolved_data_type =
          CAST(LilyCheckedExprCallRecordParam *, get__Vec(params, i))
            ->value->data_type;
        LilyCheckedDataType *original_data_type =
          CAST(LilyCheckedField *, get__Vec(fields, i))->data_type;

        for (Usize j = 0; j < generic_params->len; ++j) {
            const LilyCheckedGenericParam *generic_param =
              get__Vec(generic_params, j);
            const String *generic_param_name =
              get_name__LilyCheckedGenericParam(generic_param);
            OrderedHashMapPair *inserted_pair =
              get_pair_from_id__OrderedHashMap(resolved_generic_params, j);

            if (inserted_pair) {
                if (CAST(LilyCheckedDataType *, inserted_pair->value)->kind !=
                    LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
                    continue;
                }
            }

            LilyCheckedDataType *data_type_generic_param =
              generate_generic_param_from_resolved_data_type__LilyCheckedDataType(
                resolved_data_type, generic_param_name, original_data_type);

            if (inserted_pair && data_type_generic_param) {
                FREE(LilyCheckedDataType, inserted_pair->value);
                inserted_pair->value = data_type_generic_param;
            } else if (data_type_generic_param) {
                insert__OrderedHashMap(resolved_generic_params,
                                       generic_param_name->buffer,
                                       data_type_generic_param);
            } else {
                insert__OrderedHashMap(resolved_generic_params,
                                       generic_param_name->buffer,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                           resolved_data_type->location));
            }
        }
    }

    return resolved_generic_params;
}

bool
is_guarantee__LilyCheckedDataType(LilyCheckedDataType *self,
                                  enum LilyCheckedDataTypeKind guarantee)
{
    ASSERT(guarantee != LILY_CHECKED_DATA_TYPE_KIND_CUSTOM &&
           guarantee != LILY_CHECKED_DATA_TYPE_KIND_LAMBDA &&
           guarantee != LILY_CHECKED_DATA_TYPE_KIND_TUPLE &&
           guarantee != LILY_CHECKED_DATA_TYPE_KIND_ARRAY);

    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            for (Usize i = 0; self->conditional_compiler_choice.choices->len;
                 ++i) {
                if (!is_guarantee__LilyCheckedDataType(
                      get__Vec(self->conditional_compiler_choice.choices, i),
                      guarantee)) {
                    return false;
                }
            }

            return true;
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
            for (Usize i = 0; self->compiler_choice->len; ++i) {
                if (!is_guarantee__LilyCheckedDataType(
                      get__Vec(self->compiler_choice, i), guarantee)) {
                    return false;
                }
            }

            return true;
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC: {
            LilyCheckedDataType *update = NULL;

            switch (guarantee) {
                case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
                    update =
                      NEW_VARIANT(LilyCheckedDataType,
                                  result,
                                  self->location,
                                  NEW(LilyCheckedDataTypeResult, self, NULL));
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_LIST:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, list, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_MUT:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, mut, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, optional, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, ptr, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, ptr_mut, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_REF:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, ref, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, ref_mut, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, trace, self->location, self);
                    break;
                case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
                    update = NEW_VARIANT(
                      LilyCheckedDataType, trace_mut, self->location, self);
                    break;
                default:
                    self->kind = guarantee;
            }

            if (update) {
                self = update;
            }

            return true;
        }
        default:
            return self->kind == guarantee;
    }
}

Vec *
get_choices__LilyCheckedDataType(const LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
            return self->compiler_choice;
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
            return self->conditional_compiler_choice.choices;
        default:
            return NULL;
    }
}

void
add_choice__LilyCheckedDataType(Vec *choices, LilyCheckedDataType *choice)
{
    for (Usize i = 0; i < choices->len; ++i) {
        if (eq__LilyCheckedDataType(get__Vec(choices, i), choice)) {
            return;
        }
    }

    push__Vec(choices, choice);
}

bool
can_update__LilyCheckedDataType(LilyCheckedDataType *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            return self->array.kind ==
                   LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN;
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
            return true;
        default:
            return false;
    }
}

bool
is_contains_kind__LilyCheckedDataType(LilyCheckedDataType *self,
                                      enum LilyCheckedDataTypeKind kind)
{
    if (self->kind == kind) {
        return true;
    }

    // Look for traversable types.
    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return is_contains_kind__LilyCheckedDataType(self->mut, kind);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return is_contains_kind__LilyCheckedDataType(self->optional, kind);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return is_contains_kind__LilyCheckedDataType(self->ptr, kind);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return is_contains_kind__LilyCheckedDataType(self->ptr_mut, kind);
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return is_contains_kind__LilyCheckedDataType(self->ref, kind);
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return is_contains_kind__LilyCheckedDataType(self->ref_mut, kind);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return is_contains_kind__LilyCheckedDataType(self->trace, kind);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return is_contains_kind__LilyCheckedDataType(self->trace_mut, kind);
        default:
            return false;
    }
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
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            return "LILY_CHECKED_DATA_TYPE_KIND_RESULT";
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
        res = format__String(
          "LilyCheckedDataType{{ kind = {s}, location = {sa}, is_lock = {b}",
          to_string__Debug__LilyCheckedDataTypeKind(self->kind),
          to_string__Debug__Location(self->location),
          self->is_lock);
    } else {
        res = format__String(
          "LilyCheckedDataType{{ kind = {s}, location = NULL, is_lock = {b}",
          to_string__Debug__LilyCheckedDataTypeKind(self->kind),
          self->is_lock);
    }

    switch (self->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY: {
            char *s =
              format(", array = {sa} }",
                     to_string__Debug__LilyCheckedDataTypeArray(&self->array));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES: {
            char *s =
              format(", bytes = {sa} }",
                     to_string__Debug__LilyCheckedDataTypeLen(&self->bytes));

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
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT: {
            char *s =
              format(", result = {Sr} }",
                     to_string__Debug__LilyCheckedDataTypeResult(self->result));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_DATA_TYPE_KIND_STR: {
            char *s =
              format(", str = {sa} }",
                     to_string__Debug__LilyCheckedDataTypeLen(&self->str));

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

VARIANT_DESTRUCTOR(LilyCheckedDataType, result, LilyCheckedDataType *self)
{
    FREE(LilyCheckedDataTypeResult, self->result);
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
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            FREE_VARIANT(LilyCheckedDataType, result, self);
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
