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

#ifndef LILY_CORE_LILY_COMPILER_IR_H
#define LILY_CORE_LILY_COMPILER_IR_H

#include <base/macros.h>

#include <core/lily/compiler/ir/cc.h>
#include <core/lily/compiler/ir/cpp.h>
#include <core/lily/compiler/ir/js.h>
#include <core/lily/compiler/ir/llvm.h>

typedef struct LilyPackage LilyPackage;

enum LilyIrKind
{
    LILY_IR_KIND_CC,
    LILY_IR_KIND_CPP,
    LILY_IR_KIND_JS,
    LILY_IR_KIND_LLVM,
};

typedef struct LilyIr
{
    enum LilyIrKind kind;
    union
    {
        LilyIrCc cc;
        LilyIrCpp cpp;
        LilyIrJs js;
        LilyIrLlvm llvm;
    };
} LilyIr;

/**
 *
 * @brief Construct LilyIr type (LILY_IR_KIND_CC).
 */
inline VARIANT_CONSTRUCTOR(LilyIr, LilyIr, cc, LilyIrCc cc)
{
    return (LilyIr){ .kind = LILY_IR_KIND_CC, .cc = cc };
}

/**
 *
 * @brief Construct LilyIr type (LILY_IR_KIND_CPP).
 */
inline VARIANT_CONSTRUCTOR(LilyIr, LilyIr, cpp, LilyIrCpp cpp)
{
    return (LilyIr){ .kind = LILY_IR_KIND_CPP, .cpp = cpp };
}

/**
 *
 * @brief Construct LilyIr type (LILY_IR_KIND_JS).
 */
inline VARIANT_CONSTRUCTOR(LilyIr, LilyIr, js, LilyIrJs js)
{
    return (LilyIr){ .kind = LILY_IR_KIND_JS, .js = js };
}

/**
 *
 * @brief Construct LilyIr type (LILY_IR_KIND_LLVM).
 */
inline VARIANT_CONSTRUCTOR(LilyIr, LilyIr, llvm, LilyIrLlvm llvm)
{
    return (LilyIr){ .kind = LILY_IR_KIND_LLVM, .llvm = llvm };
}

/**
 *
 * @brief Run the IR.
 */
void
run__LilyIr(LilyPackage *self);

/**
 *
 * @brief Free LilyIr type.
 */
DESTRUCTOR(LilyIr, const LilyIr *self);

#endif // LILY_CORE_LILY_COMPILER_IR_H
