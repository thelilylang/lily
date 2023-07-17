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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_SCOPE_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_SCOPE_H

#include <base/hash_map.h>

#include <llvm-c/Core.h>

typedef struct LilyIrLlvmScope
{
    HashMap *values;                // HashMap<LLVMValueRef>*
    struct LilyIrLlvmScope *parent; // struct LilyIrLLvmScope*?
} LilyIrLlvmScope;

/**
 *
 * @brief Construct LilyIrLlvmScope type.
 */
CONSTRUCTOR(LilyIrLlvmScope *, LilyIrLlvmScope, LilyIrLlvmScope *parent);

/**
 *
 * @brief Get value from the scope.
 */
LLVMValueRef
get__LilyIrLlvmScope(const LilyIrLlvmScope *self, char *name);

/**
 *
 * @brief Add value to the scope.
 */
void
add__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                     char *name,
                     LLVMValueRef value);

/**
 *
 * @brief Free LilyIrLlvmScope type.
 */
DESTRUCTOR(LilyIrLlvmScope, LilyIrLlvmScope *self);

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_SCOPE_H
