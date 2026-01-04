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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_PRIMARY_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_PRIMARY_H

#include <core/lily/compiler/ir/llvm.h>

/**
 *
 * @brief Return `i1` as LLVMTypRef.
 */
inline LLVMTypeRef
i1__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMInt1TypeInContext(self->context);
}

/**
 *
 * @brief Return `i8` as LLVMTypRef.
 */
inline LLVMTypeRef
i8__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMInt8TypeInContext(self->context);
}

/**
 *
 * @brief Return `i16` as LLVMTypRef.
 */
inline LLVMTypeRef
i16__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMInt16TypeInContext(self->context);
}

/**
 *
 * @brief Return `i32` as LLVMTypRef.
 */
inline LLVMTypeRef
i32__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMInt32TypeInContext(self->context);
}

/**
 *
 * @brief Return `i64` as LLVMTypRef.
 */
inline LLVMTypeRef
i64__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMInt64TypeInContext(self->context);
}

/**
 *
 * @brief Return `i128` as LLVMTypRef.
 */
inline LLVMTypeRef
i128__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMInt128TypeInContext(self->context);
}

/**
 *
 * @brief Return `intptr` as LLVMTypRef.
 */
inline LLVMTypeRef
intptr__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMIntPtrTypeInContext(self->context, self->target_data);
}

/**
 *
 * @brief Return `float` as LLVMTypRef.
 */
inline LLVMTypeRef
float__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMFloatTypeInContext(self->context);
}

/**
 *
 * @brief Return `double` as LLVMTypRef.
 */
inline LLVMTypeRef
double__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMDoubleTypeInContext(self->context);
}

/**
 *
 * @brief Return `void` as LLVMTypRef.
 */
inline LLVMTypeRef
void__LilyIrLlvm(const LilyIrLlvm *self)
{
    return LLVMVoidTypeInContext(self->context);
}

/**
 *
 * @brief Return `*T` as LLVMTypeRef.
 */
inline LLVMTypeRef
ptr__LilyIrLlvm(const LilyIrLlvm *self, LLVMTypeRef ptr_type)
{
    return LLVMPointerType(ptr_type, 0);
}

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_PRIMARY_H
