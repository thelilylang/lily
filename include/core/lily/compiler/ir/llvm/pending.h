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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_PENDING_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_PENDING_H

#include <base/assert.h>
#include <base/hash_map.h>
#include <base/stack.h>

#include <llvm-c/Core.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LilyIrLlvmPending
{
    LLVMValueRef current_fun; // LLVMValueRef? (&)
    HashMap *blocks;          // HashMap<LLVMBasicBlockRef (&)>*
} LilyIrLlvmPending;

/**
 *
 * @brief Construct LilyIrLlvmPending type.
 */
inline CONSTRUCTOR(LilyIrLlvmPending, LilyIrLlvmPending)
{
    return (LilyIrLlvmPending){ .current_fun = NULL, .blocks = NEW(HashMap) };
}

/**
 *
 * @brief Get block from `blocks` field.
 */
inline LLVMBasicBlockRef
get_block__LilyIrLlvmPending(const LilyIrLlvmPending *self, const char *name)
{
    return get__HashMap(self->blocks, (char *)name);
}

/**
 *
 * @brief Add block to the `blocks` field.
 */
void
add_block__LilyIrLlvmPending(const LilyIrLlvmPending *self,
                             const char *name,
                             LLVMBasicBlockRef block);

/**
 *
 * @brief Free LilyIrLLvmPending type.
 */
inline DESTRUCTOR(LilyIrLlvmPending, const LilyIrLlvmPending *self)
{
    FREE(HashMap, self->blocks);
}

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_PENDING_H
