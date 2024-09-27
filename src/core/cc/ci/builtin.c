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

#include <base/assert.h>
#include <base/new.h>

#include <core/cc/ci/builtin.h>
#include <core/cc/ci/primary_data_types.h>
#include <core/shared/search.h>

#include <stdio.h>
#include <stdlib.h>

// NOTE: Here's the minimum builtin requirement to get through the standard
// library. We'll be adding more in the future, but for now it's not a priority.

#define CI_BUILTIN_FUNCTION(n, rt, pn, ...)                         \
    (CIBuiltinFunction)                                             \
    {                                                               \
        .name = &builtin_function_names[n], .return_data_type = rt, \
        .params = init__Vec(pn, __VA_ARGS__)                        \
    }

#define CI_BUILTIN_TYPE_FROM_RAW(n)                                    \
    (CIBuiltinType)                                                    \
    {                                                                  \
        .name = &builtin_type_names[n], .size = builtin_type_sizes[n], \
        .alignment = builtin_type_alignments[n]                        \
    }

#define CI_BUILTIN_FUNCTION_MEMCPY 0
#define CI_BUILTIN_FUNCTION_VA_START 1
#define CI_BUILTIN_FUNCTION_VA_END 2
#define CI_BUILTIN_FUNCTION_VA_ARG 3
#define CI_BUILTIN_FUNCTION_VA_COPY 4

static SizedStr builtin_function_names[CI_BUILTIN_FUNCTION_COUNT] = {
    SIZED_STR_FROM_RAW("__builtin_memcpy"),
    SIZED_STR_FROM_RAW("__builtin_va_start"),
    SIZED_STR_FROM_RAW("__builtin_va_end"),
    SIZED_STR_FROM_RAW("__builtin_va_arg"),
    SIZED_STR_FROM_RAW("__builtin_va_copy")
};

static Int32 builtin_function_ids[CI_BUILTIN_FUNCTION_COUNT] = {
    CI_BUILTIN_FUNCTION_MEMCPY,
    CI_BUILTIN_FUNCTION_VA_START,
    CI_BUILTIN_FUNCTION_VA_END,
    CI_BUILTIN_FUNCTION_VA_ARG,
    CI_BUILTIN_FUNCTION_VA_COPY
};

#define CI_BUILTIN_TYPE_VA_LIST 0

static SizedStr builtin_type_names[CI_BUILTIN_TYPE_COUNT] = {
    SIZED_STR_FROM_RAW("__builtin_va_list")
};

static Int32 builtin_type_ids[CI_BUILTIN_TYPE_COUNT] = {
    CI_BUILTIN_TYPE_VA_LIST
};

static Usize builtin_type_sizes[CI_BUILTIN_TYPE_COUNT] = { sizeof(
  __builtin_va_list) };

static Usize builtin_type_alignments[CI_BUILTIN_TYPE_COUNT] = { alignof(
  __builtin_va_list) };

static CIBuiltin *builtin_ref = NULL; // CIBuiltin* (&)

CIBuiltinFunction *
load__CIBuiltinFunction()
{
    CIBuiltinFunction *builtins =
      lily_malloc(sizeof(CIBuiltinFunction) * CI_BUILTIN_FUNCTION_COUNT);

    builtins[CI_BUILTIN_FUNCTION_MEMCPY] = CI_BUILTIN_FUNCTION(
      CI_BUILTIN_FUNCTION_MEMCPY,
      NEW_VARIANT(CIDataType, ptr, void__PrimaryDataTypes()),
      2,
      NEW_VARIANT(CIDataType, ptr, void__PrimaryDataTypes()),
      unsigned_long_int__PrimaryDataTypes());
    builtins[CI_BUILTIN_FUNCTION_VA_START] = CI_BUILTIN_FUNCTION(
      CI_BUILTIN_FUNCTION_VA_START,
      void__PrimaryDataTypes(),
      2,
      NEW_VARIANT(CIDataType, builtin, CI_BUILTIN_TYPE_VA_LIST),
      any__PrimaryDataTypes());
    builtins[CI_BUILTIN_FUNCTION_VA_END] = CI_BUILTIN_FUNCTION(
      CI_BUILTIN_FUNCTION_VA_END,
      void__PrimaryDataTypes(),
      1,
      NEW_VARIANT(CIDataType, builtin, CI_BUILTIN_TYPE_VA_LIST));
    builtins[CI_BUILTIN_FUNCTION_VA_ARG] = CI_BUILTIN_FUNCTION(
      CI_BUILTIN_FUNCTION_VA_ARG,
      type_info__PrimaryDataTypes(),
      2,
      NEW_VARIANT(CIDataType, builtin, CI_BUILTIN_TYPE_VA_LIST),
      type_info__PrimaryDataTypes());
    builtins[CI_BUILTIN_FUNCTION_VA_COPY] = CI_BUILTIN_FUNCTION(
      CI_BUILTIN_FUNCTION_VA_COPY,
      void__PrimaryDataTypes(),
      2,
      NEW_VARIANT(CIDataType, builtin, CI_BUILTIN_TYPE_VA_LIST),
      NEW_VARIANT(CIDataType, builtin, CI_BUILTIN_TYPE_VA_LIST));

    return builtins;
}

bool
is__CIBuiltinFunction(String *name)
{
    return get_id__Search(name,
                          builtin_function_names,
                          builtin_function_ids,
                          CI_BUILTIN_FUNCTION_COUNT) != -1;
}

Usize
get_id__CIBuiltinFunction(String *name)
{
    Int32 id = get_id__Search(name,
                              builtin_function_names,
                              builtin_function_ids,
                              CI_BUILTIN_FUNCTION_COUNT);

    ASSERT(id != -1);

    return id;
}

DESTRUCTOR(CIBuiltinFunction, const CIBuiltinFunction *self)
{
    FREE(CIDataType, self->return_data_type);
    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, CIDataType);
    FREE(Vec, self->params);
}

CIBuiltinType *
load__CIBuiltinType()
{
    CIBuiltinType *builtins =
      lily_malloc(sizeof(CIBuiltinType) * CI_BUILTIN_TYPE_COUNT);

    builtins[0] = CI_BUILTIN_TYPE_FROM_RAW(0);

    return builtins;
}

bool
is__CIBuiltinType(String *name)
{
    return get_id__Search(name,
                          builtin_type_names,
                          builtin_type_ids,
                          CI_BUILTIN_TYPE_COUNT) != -1;
}

Usize
get_id__CIBuiltinType(String *name)
{
    Int32 id = get_id__Search(
      name, builtin_type_names, builtin_type_ids, CI_BUILTIN_TYPE_COUNT);

    ASSERT(id != -1);

    return id;
}

CONSTRUCTOR(CIBuiltin, CIBuiltin)
{
    return (CIBuiltin){ .functions = load__CIBuiltinFunction(),
                        .types = load__CIBuiltinType() };
}

const CIBuiltinType *
get_builtin_type__CIBuiltin(const CIBuiltin *self, Usize id)
{
    ASSERT(id < CI_BUILTIN_TYPE_COUNT);

    return &self->types[id];
}

const CIBuiltinFunction *
get_builtin_function__CIBuiltin(const CIBuiltin *self, Usize id)
{
    ASSERT(id < CI_BUILTIN_FUNCTION_COUNT);

    return &self->functions[id];
}

void
set__CIBuiltin(CIBuiltin *self)
{
    builtin_ref = self;
}

CIBuiltin *
get_ref__CIBuiltin()
{
    ASSERT(builtin_ref);

    return builtin_ref;
}

DESTRUCTOR(CIBuiltin, const CIBuiltin *self)
{
    FREE_BUFFER_ITEMS(
      &self->functions, CI_BUILTIN_FUNCTION_COUNT, CIBuiltinFunction);
    lily_free(self->functions);
    lily_free(self->types);
}
