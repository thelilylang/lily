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
#include <base/assert.h>

#include <core/lily/compiler/ir/llvm/scope.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(LilyIrLlvmScope *, LilyIrLlvmScope, LilyIrLlvmScope *parent)
{
    LilyIrLlvmScope *self = lily_malloc(sizeof(LilyIrLlvmScope));

    self->values = NEW(HashMap);
    self->parent = parent;

    return self;
}

LLVMValueRef
get__LilyIrLlvmScope(const LilyIrLlvmScope *self, char *name)
{
    LLVMValueRef value = get__HashMap(self->values, name);

    return value          ? value
           : self->parent ? get__LilyIrLlvmScope(self->parent, name)
                          : NULL;
}

void
add__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                     char *name,
                     LLVMValueRef value)
{
    if (insert__HashMap(self->values, name, value)) {
        UNREACHABLE("duplicate name in the scope");
    }
}

DESTRUCTOR(LilyIrLlvmScope, LilyIrLlvmScope *self)
{
    FREE(HashMap, self->values);
    lily_free(self);
}
