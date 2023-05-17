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

#include <base/assert.h>

#include <core/lily/ir/llvm/generator/expr.h>
#include <core/lily/ir/llvm/generator/expr/call.h>
#include <core/lily/ir/llvm/generator/expr/unary.h>
#include <core/lily/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

static inline LLVMValueRef
generate_not__LilyIrLlvm(const LilyIrLlvm *self,
                         const LilyCheckedExpr *expr,
                         LilyLlvmScope *scope,
                         LLVMValueRef right,
                         LLVMValueRef fun,
                         LLVMValueRef ptr);

static inline LLVMValueRef
generate_dereference__LilyIrLlvm(const LilyIrLlvm *self,
                                 const LilyCheckedExpr *expr,
                                 LilyLlvmScope *scope,
                                 LLVMValueRef right,
                                 LLVMValueRef fun,
                                 LLVMValueRef ptr);

static inline LLVMValueRef
generate_neg__LilyIrLlvm(const LilyIrLlvm *self,
                         const LilyCheckedExpr *expr,
                         LilyLlvmScope *scope,
                         LLVMValueRef right,
                         LLVMValueRef fun,
                         LLVMValueRef ptr);

static inline LLVMValueRef
generate_ref__LilyIrLlvm(const LilyIrLlvm *self,
                         const LilyCheckedExpr *expr,
                         LilyLlvmScope *scope,
                         LLVMValueRef right,
                         LLVMValueRef fun,
                         LLVMValueRef ptr);

static inline LLVMValueRef
generate_not__LilyIrLlvm(const LilyIrLlvm *self,
                         const LilyCheckedExpr *expr,
                         LilyLlvmScope *scope,
                         LLVMValueRef right,
                         LLVMValueRef fun,
                         LLVMValueRef ptr)
{
    return LLVMBuildNot(self->builder, right, "");
}

LLVMValueRef
generate_dereference__LilyIrLlvm(const LilyIrLlvm *self,
                                 const LilyCheckedExpr *expr,
                                 LilyLlvmScope *scope,
                                 LLVMValueRef right,
                                 LLVMValueRef fun,
                                 LLVMValueRef ptr)
{
    return LLVMBuildLoad2(
      self->builder, LLVMGetElementType(LLVMTypeOf(right)), right, "");
}

LLVMValueRef
generate_neg__LilyIrLlvm(const LilyIrLlvm *self,
                         const LilyCheckedExpr *expr,
                         LilyLlvmScope *scope,
                         LLVMValueRef right,
                         LLVMValueRef fun,
                         LLVMValueRef ptr)
{
    return LLVMBuildNeg(self->builder, right, "");
}

LLVMValueRef
generate_ref__LilyIrLlvm(const LilyIrLlvm *self,
                         const LilyCheckedExpr *expr,
                         LilyLlvmScope *scope,
                         LLVMValueRef right,
                         LLVMValueRef fun,
                         LLVMValueRef ptr)
{
    switch (expr->data_type->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            switch (expr->unary.right->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                    LLVMSetAlignment(
                      LLVMBuildIntToPtr(
                        self->builder, right, LLVMTypeOf(right), ""),
                      LLVMABISizeOfType(self->target_data, LLVMTypeOf(right)));
                    return right;
                default: {
                    LLVMValueRef load =
                      LLVMBuildLoad2(self->builder,
                                     ptr__LilyIrLlvm(self, LLVMTypeOf(right)),
                                     right,
                                     "");
                    LLVMSetAlignment(
                      load,
                      LLVMABISizeOfType(self->target_data, LLVMTypeOf(load)));
                    return load;
                }
            }
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            switch (expr->unary.right->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                    return right;
                default:
                    return LLVMBuildLoad2(
                      self->builder,
                      ptr__LilyIrLlvm(self, LLVMTypeOf(right)),
                      right,
                      "");
            }
        default:
            UNREACHABLE("analysis have a bug!!");
    }
}

LLVMValueRef
generate_unary_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                const LilyCheckedExpr *expr,
                                LilyLlvmScope *scope,
                                LLVMValueRef fun,
                                LLVMValueRef ptr)
{
    switch (expr->unary.kind) {
        case LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE: {
            LLVMValueRef right = generate_call_expr__LilyIrLlvm(
              self, expr->unary.right, scope, fun, ptr);

            return generate_dereference__LilyIrLlvm(
              self, expr, scope, right, fun, ptr);
        }
        case LILY_CHECKED_EXPR_UNARY_KIND_NEG: {
            LLVMValueRef right = generate_expr__LilyIrLlvm(
              self, expr->unary.right, scope, fun, ptr);

            return generate_neg__LilyIrLlvm(self, expr, scope, right, fun, ptr);
        }
        case LILY_CHECKED_EXPR_UNARY_KIND_NOT: {
            LLVMValueRef right = generate_expr__LilyIrLlvm(
              self, expr->unary.right, scope, fun, ptr);

            return generate_not__LilyIrLlvm(self, expr, scope, right, fun, ptr);
        }
        case LILY_CHECKED_EXPR_UNARY_KIND_REF:
        case LILY_CHECKED_EXPR_UNARY_KIND_REF_MUT: {
            LLVMValueRef right = generate_ref_call_expr__LilyIrLlvm(
              self, expr->unary.right, scope, fun);

            return generate_ref__LilyIrLlvm(self, expr, scope, right, fun, ptr);
        }
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE:
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE_MUT:
            return generate_call_expr__LilyIrLlvm(
              self, expr->unary.right, scope, fun, ptr);
        default:
            TODO("unary operator");
    }
}
