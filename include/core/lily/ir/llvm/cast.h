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
 * @brief Cast `i1` to `i8`.
 */
inline LLVMValueRef
i1_cast_i8__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i8__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i1` to `i16`.
 */
inline LLVMValueRef
i1_cast_i16__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i16__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i1` to `i32`.
 */
inline LLVMValueRef
i1_cast_i32__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i32__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i1` to `i64`.
 */
inline LLVMValueRef
i1_cast_i64__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i64__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i1` to `i128`.
 */
inline LLVMValueRef
i1_cast_i128__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i128__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i8` to `i1`.
 */
inline LLVMValueRef
i8_cast_i1__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i1__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i8` to `i16`.
 */
inline LLVMValueRef
i8_cast_i16__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i16__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i8` to `i32`.
 */
inline LLVMValueRef
i8_cast_i32__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i32__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i8` to `i64`.
 */
inline LLVMValueRef
i8_cast_i64__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i64__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i8` to `i128`.
 */
inline LLVMValueRef
i8_cast_i128__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i128__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i16` to `i1`.
 */
inline LLVMValueRef
i16_cast_i1__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i1__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i16` to `i8`.
 */
inline LLVMValueRef
i16_cast_i8__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i8__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i16` to `i32`.
 */
inline LLVMValueRef
i16_cast_i32__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i32__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i16` to `i64`.
 */
inline LLVMValueRef
i16_cast_i64__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i64__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i16` to `i128`.
 */
inline LLVMValueRef
i16_cast_i128__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i128__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i32` to `i1`.
 */
inline LLVMValueRef
i32_cast_i1__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i1__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i32` to `i8`.
 */
inline LLVMValueRef
i32_cast_i8__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i8__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i32` to `i16`.
 */
inline LLVMValueRef
i32_cast_i16__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i16__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i32` to `i64`.
 */
inline LLVMValueRef
i32_cast_i64__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i64__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i32` to `i128`.
 */
inline LLVMValueRef
i32_cast_i128__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i128__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i64` to `i1`.
 */
inline LLVMValueRef
i64_cast_i1__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i1__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i64` to `i8`.
 */
inline LLVMValueRef
i64_cast_i8__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i8__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i64` to `i16`.
 */
inline LLVMValueRef
i64_cast_i16__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i16__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i64` to `i32`.
 */
inline LLVMValueRef
i64_cast_i32__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i32__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i64` to `i128`.
 */
inline LLVMValueRef
i64_cast_i128__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i128__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i128` to `i1`.
 */
inline LLVMValueRef
i128_cast_i1__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i1__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i128` to `i8`.
 */
inline LLVMValueRef
i128_cast_i8__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i8__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i128` to `i16`.
 */
inline LLVMValueRef
i128_cast_i16__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i16__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i128` to `i32`.
 */
inline LLVMValueRef
i128_cast_i32__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i32__LilyIrLlvm(self), "");
}

/**
 *
 * @brief Cast `i128` to `i64`.
 */
inline LLVMValueRef
i128_cast_i64__LilyIrLlvm(const LilyIrLlvm *self, LLVMValueRef value)
{
    return LLVMBuildSExt(self->builder, value, i64__LilyIrLlvm(self), "");
}

#endif // LILY_CORE_LILY_IR_LLVM_CAST_H