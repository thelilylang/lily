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
#include <base/assert.h>

#include <core/lily/compiler/ir/llvm/scope.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(LilyIrLlvmScopeItem *, LilyIrLlvmScopeItem, Usize limit_block_id)
{
    LilyIrLlvmScopeItem *self = lily_malloc(sizeof(LilyIrLlvmScopeItem));

    self->values = NEW(HashMap);
    self->limit_block_id = limit_block_id;

    return self;
}

void
add__LilyIrLlvmScopeItem(const LilyIrLlvmScopeItem *self,
                         char *name,
                         LLVMValueRef value)
{
    if (insert__HashMap(self->values, name, value)) {
        UNREACHABLE("duplicate name in the scope");
    }
}

DESTRUCTOR(LilyIrLlvmScopeItem, LilyIrLlvmScopeItem *self)
{
    FREE(HashMap, self->values);
    lily_free(self);
}

LLVMValueRef
get__LilyIrLlvmScope(const LilyIrLlvmScope *self, char *name)
{
    for (Usize i = self->items->len; i--;) {
        LLVMValueRef value =
          get__LilyIrLlvmScopeItem(get__Vec(self->items, i), name);

        if (value) {
            return value;
        }
    }

    UNREACHABLE("impossible to found the value with the given name");
}

LLVMValueRef
safe_get__LilyIrLlvmScope(const LilyIrLlvmScope *self, char *name)
{
    for (Usize i = self->items->len; i--;) {
        LLVMValueRef value =
          get__LilyIrLlvmScopeItem(get__Vec(self->items, i), name);

        if (value) {
            return value;
        }
    }

    return NULL;
}

void
add__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                     char *name,
                     LLVMValueRef value)
{
    ASSERT(self->items->len > 0);

    LilyIrLlvmScopeItem *last_item = last__Vec(self->items);

    return add__LilyIrLlvmScopeItem(last_item, name, value);
}

void
destroy_scope_items__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                                     Usize limit_block_id)
{
    for (Usize i = 0; i < self->items->len;) {
        LilyIrLlvmScopeItem *item = get__Vec(self->items, i);

        if (item->limit_block_id == limit_block_id) {
            FREE(LilyIrLlvmScopeItem, remove__Vec(self->items, i));
        } else {
            ++i;
        }
    }
}

DESTRUCTOR(LilyIrLlvmScope, const LilyIrLlvmScope *self)
{
    FREE_BUFFER_ITEMS(
      self->items->buffer, self->items->len, LilyIrLlvmScopeItem);
    FREE(Vec, self->items);
}
