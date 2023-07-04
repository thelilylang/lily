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

#ifndef LILY_CORE_LILY_COMPILER_LINKER_H
#define LILY_CORE_LILY_COMPILER_LINKER_H

#include <core/lily/compiler/ir/llvm/linker.h>

typedef struct LilyPackage LilyPackage;

enum LilyLinkerKind
{
    LILY_LINKER_KIND_CC,
    LILY_LINKER_KIND_CPP,
    LILY_LINKER_KIND_LLVM
};

typedef struct LilyLinker
{
    enum LilyLinkerKind kind;
    union
    {
        LilyIrLlvmLinker llvm;
    };
} LilyLinker;

/**
 *
 * @brief Construct LilyLinker type (LILY_LINKER_KIND_LLVM).
 */
inline VARIANT_CONSTRUCTOR(LilyLinker, LilyLinker, llvm, LilyIrLlvmLinker llvm)
{
    return (LilyLinker){ .kind = LILY_LINKER_KIND_LLVM, .llvm = llvm };
}

/**
 *
 * @brief Run the linker.
 */
void
run__LilyLinker(LilyPackage *self);

/**
 *
 * @brief Free LilyLinker type.
 */
DESTRUCTOR(LilyLinker, const LilyLinker *self);

#endif // LILY_CORE_LILY_COMPILER_LINKER_H
