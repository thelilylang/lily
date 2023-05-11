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

#include <base/new.h>

#include <core/lily/ir/llvm.h>
#include <core/lily/ir/llvm/scope.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CONSTRUCTOR(LilyLlvmValue *, LilyLlvmValue, String *name, LLVMValueRef value)
{
    LilyLlvmValue *self = lily_malloc(sizeof(LilyLlvmValue));

    self->name = name;
    self->value = value;

    return self;
}

CONSTRUCTOR(LilyLlvmType *, LilyLlvmType, String *name, LLVMTypeRef type)
{
    LilyLlvmType *self = lily_malloc(sizeof(LilyLlvmType));

    self->name = name;
    self->type = type;

    return self;
}

CONSTRUCTOR(LilyLlvmScope *, LilyLlvmScope, LilyLlvmScope *parent)
{
    LilyLlvmScope *self = lily_malloc(sizeof(LilyLlvmScope));

    self->values = NEW(Vec);
    self->types = NEW(Vec);
    self->loads = NEW(Vec);
    self->parent = parent;

    return self;
}

LilyLlvmValue *
search_value__LilyLlvmScope(LilyLlvmScope *self, String *name)
{
    for (Usize i = 0; i < self->values->len; ++i) {
        LilyLlvmValue *value = get__Vec(self->values, i);

        if (!strcmp(value->name->buffer, name->buffer)) {
            return value;
        }
    }

    if (self->parent) {
        return search_value__LilyLlvmScope(self->parent, name);
    }

    UNREACHABLE("(search value) the analysis or the codegen have a bug!!");
}

LilyLlvmType *
search_type__LilyLlvmScope(LilyLlvmScope *self, String *name)
{
    for (Usize i = 0; i < self->types->len; ++i) {
        LilyLlvmType *type = get__Vec(self->types, i);

        if (!strcmp(type->name->buffer, name->buffer)) {
            return type;
        }
    }

    if (self->parent) {
        return search_type__LilyLlvmScope(self->parent, name);
    }

    UNREACHABLE("(search type) the analysis or the codegen have a bug!!");
}

LilyLlvmValue *
search_load__LilyLlvmScope(LilyLlvmScope *self, String *name)
{
    for (Usize i = 0; i < self->loads->len; ++i) {
        LilyLlvmValue *load = get__Vec(self->loads, i);

        if (!strcmp(load->name->buffer, name->buffer)) {
            return load;
        }
    }

    if (self->parent) {
        return search_load__LilyLlvmScope(self->parent, name);
    }

    return NULL;
}

LLVMValueRef
load_value__LilyLlvmScope(LilyLlvmScope *self,
                          const LilyIrLlvm *llvm,
                          LLVMTypeRef type,
                          String *name)
{
    LilyLlvmValue *load = search_load__LilyLlvmScope(self, name);

    if (load) {
        return load->value;
    }

    LLVMValueRef new_load = LLVMBuildLoad2(
      llvm->builder, type, search_value__LilyLlvmScope(self, name)->value, "");

    push__Vec(self->loads, NEW(LilyLlvmValue, name, new_load));

    return new_load;
}

DESTRUCTOR(LilyLlvmScope, LilyLlvmScope *self)
{
    FREE_BUFFER_ITEMS(self->values->buffer, self->values->len, LilyLlvmValue);
    FREE(Vec, self->values);

    FREE_BUFFER_ITEMS(self->types->buffer, self->types->len, LilyLlvmType);
    FREE(Vec, self->types);

    FREE_BUFFER_ITEMS(self->loads->buffer, self->loads->len, LilyLlvmValue);
    FREE(Vec, self->loads);

    lily_free(self);
}
