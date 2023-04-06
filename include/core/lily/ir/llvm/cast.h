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

#ifndef LILY_CORE_LILY_IR_LLVM_CAST_H
#define LILY_CORE_LILY_IR_LLVM_CAST_H

#include <core/lily/ir/llvm/primary.h>

/**
 *
 * @brief Cast to `i1`.
 */
inline LLVMValueRef
cast_to_i1__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i1__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast to `i8`.
 */
inline LLVMValueRef
cast_to_i8__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i8__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast to `i16`.
 */
inline LLVMValueRef
cast_to_i16__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i16__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast to `i32`.
 */
inline LLVMValueRef
cast_to_i32__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i32__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast to `i64`.
 */
inline LLVMValueRef
cast_to_i64__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i64__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast to `i128`.
 */
inline LLVMValueRef
cast_to_i128__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i128__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast to `float`.
 */
inline LLVMValueRef
cast_to_float__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, float__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast to `double`.
 */
inline LLVMValueRef
cast_to_double__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, double__LilyIrLlvm(self), "");
}

#endif // LILY_CORE_LILY_IR_LLVM_CAST_H