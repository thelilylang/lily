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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_SCOPE_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_SCOPE_H

#include <base/hash_map.h>
#include <base/vec.h>

#include <llvm-c/Core.h>

typedef struct LilyIrLlvmScopeItem
{
    HashMap *values; // HashMap<LLVMValueRef>*
    Usize limit_block_id;
} LilyIrLlvmScopeItem;

/**
 *
 * @brief Construct LilyIrLlvmScopeItem type.
 */
CONSTRUCTOR(LilyIrLlvmScopeItem *, LilyIrLlvmScopeItem, Usize limit_block_id);

/**
 *
 * @brief Get value from scope item.
 */
inline LLVMValueRef
get__LilyIrLlvmScopeItem(const LilyIrLlvmScopeItem *self, const char *name)
{
    return get__HashMap(self->values, (char *)name);
}

/**
 *
 * @brief Add value to the scope item.
 */
void
add__LilyIrLlvmScopeItem(const LilyIrLlvmScopeItem *self,
                         char *name,
                         LLVMValueRef value);

/**
 *
 * @brief Free LilyIrLlvmScopeItem type.
 */
DESTRUCTOR(LilyIrLlvmScopeItem, LilyIrLlvmScopeItem *self);

typedef struct LilyIrLlvmScope
{
    Vec *items; // Vec<LilyIrLlvmScopeItem*>*
} LilyIrLlvmScope;

/**
 *
 * @brief Construct LilyIrLlvmScope type.
 */
inline CONSTRUCTOR(LilyIrLlvmScope, LilyIrLlvmScope)
{
    return (LilyIrLlvmScope){ .items = NEW(Vec) };
}

/**
 *
 * @brief Get value from the scope.
 */
LLVMValueRef
get__LilyIrLlvmScope(const LilyIrLlvmScope *self, char *name);

/**
 *
 * @brief Get value from the scope, but doesn't emit unreachable.
 * @return LLVMValueRef?
 */
LLVMValueRef
safe_get__LilyIrLlvmScope(const LilyIrLlvmScope *self, char *name);

/**
 *
 * @brief Add a new scope item to the scope.
 */
inline void
add_scope_item__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                                Usize limit_block_id)
{
    return push__Vec(self->items, NEW(LilyIrLlvmScopeItem, limit_block_id));
}

/**
 *
 * @brief Add a new value to the scope.
 */
void
add__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                     char *name,
                     LLVMValueRef value);

/**
 *
 * @brief Destroy all scopes with the same `limit_block_id` value.
 */
void
destroy_scope_items__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                                     Usize limit_block_id);

/**
 *
 * @brief Free LilyIrLlvmScope type.
 */
DESTRUCTOR(LilyIrLlvmScope, const LilyIrLlvmScope *self);

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_SCOPE_H
