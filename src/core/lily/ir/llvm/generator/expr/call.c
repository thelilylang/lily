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

#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/generator/expr.h>
#include <core/lily/ir/llvm/generator/expr/call.h>
#include <core/lily/ir/llvm/primary.h>
#include <core/lily/ir/llvm/store.h>

#include <stdio.h>
#include <stdlib.h>

static LLVMValueRef
generate_fun_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                   const LilyCheckedExpr *expr,
                                   LilyLlvmScope *scope,
                                   LLVMValueRef fun,
                                   LLVMValueRef ptr);

static LLVMValueRef
generate_sys_fun_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                       const LilyCheckedExpr *expr,
                                       LilyLlvmScope *scope,
                                       LLVMValueRef fun,
                                       LLVMValueRef ptr);

static LLVMValueRef
generate_record_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                      const LilyCheckedExpr *expr,
                                      LilyLlvmScope *scope,
                                      LLVMValueRef fun,
                                      LLVMValueRef ptr);

static LLVMValueRef
generate_field_access_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                       const LilyCheckedExpr *expr,
                                       LilyLlvmScope *scope,
                                       LLVMValueRef fun,
                                       LLVMValueRef ptr);

LLVMValueRef
generate_fun_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                   const LilyCheckedExpr *expr,
                                   LilyLlvmScope *scope,
                                   LLVMValueRef fun,
                                   LLVMValueRef ptr)
{
    LilyLlvmFun *called_fun =
      search_fun__LilyLlvmScope(scope, expr->call.global_name);
    LLVMValueRef params[252] = { 0 };
    Usize params_len = 0;

    if (expr->call.fun.params) {
        params_len = expr->call.fun.params->len;

        for (Usize i = 0; i < expr->call.fun.params->len; ++i) {
            LilyCheckedExprCallFunParam *param =
              get__Vec(expr->call.fun.params, i);

            params[i] =
              generate_expr__LilyIrLlvm(self, param->value, scope, fun, ptr);
        }
    }

    return LLVMBuildCall2(self->builder,
                          called_fun->fun_type,
                          called_fun->fun,
                          params,
                          params_len,
                          "");
}

LLVMValueRef
generate_sys_fun_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                       const LilyCheckedExpr *expr,
                                       LilyLlvmScope *scope,
                                       LLVMValueRef fun,
                                       LLVMValueRef ptr)
{
    LilyLlvmFun *called_fun = search_fun__LilyLlvmScope(
      scope, expr->call.fun_sys.sys_fun_signature->real_name);
    LLVMValueRef params[252] = {};
    Usize params_len = 0;

    if (expr->call.fun_sys.params) {
        params_len = expr->call.fun_sys.params->len;

        for (Usize i = 0; i < expr->call.fun_sys.params->len; ++i) {
            LilyCheckedExprCallFunParam *param =
              get__Vec(expr->call.fun_sys.params, i);

            params[i] =
              generate_expr__LilyIrLlvm(self, param->value, scope, fun, ptr);
        }
    }

    return LLVMBuildCall2(self->builder,
                          called_fun->fun_type,
                          called_fun->fun,
                          params,
                          params_len,
                          "");
}

LLVMValueRef
generate_record_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                      const LilyCheckedExpr *expr,
                                      LilyLlvmScope *scope,
                                      LLVMValueRef fun,
                                      LLVMValueRef ptr)
{
    LLVMTypeRef record_type =
      generate_data_type__LilyIrLlvm(self, expr->data_type, scope);
    LLVMValueRef record_value =
      ptr ? ptr : LLVMBuildAlloca(self->builder, record_type, "");

    if (expr->call.record.params->len > 0) {
        for (Usize i = 0; i < expr->call.record.params->len; ++i) {
            LilyCheckedExprCallRecordParam *param =
              get__Vec(expr->call.record.params, i);

            LLVMValueRef value = generate_expr__LilyIrLlvm(
              self, param->value, scope, fun, record_value);

            if (value) {
                LLVMValueRef ptr_param = LLVMBuildStructGEP2(self->builder,
                                                             record_type,
                                                             record_value,
                                                             param->field_index,
                                                             "");

                LLVMBuildStore(self->builder, value, ptr_param);
            }
        }
    }

    return ptr ? NULL : record_value;
}

LLVMValueRef
generate_field_access_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                       const LilyCheckedExpr *expr,
                                       LilyLlvmScope *scope,
                                       LLVMValueRef fun,
                                       LLVMValueRef ptr)
{
    LilyCheckedExpr *first =
      get__Vec(expr->call.record_field_access.accesses, 0);

    ASSERT(first->kind == LILY_CHECKED_EXPR_KIND_CALL);

    LLVMValueRef value_ptr =
      generate_ref_call_expr__LilyIrLlvm(self, first, scope, fun);

    LLVMValueRef indices[252] = { 0 };
    bool first_is_ptr = false;

    if (first->data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_PTR ||
        first->data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_REF) {
        indices[0] = LLVMConstInt(i32__LilyIrLlvm(self), 0, false);
        first_is_ptr = true;
    }

    for (Usize i = 1; i < expr->call.record_field_access.accesses->len; ++i) {
        LilyCheckedExpr *field =
          get__Vec(expr->call.record_field_access.accesses, i);

        indices[first_is_ptr ? i : i - 1] = LLVMConstInt(
          i32__LilyIrLlvm(self), field->call.record_field_single.index, false);
    }

    LilyCheckedDataType *custom =
      get_direct_custom_data_type__LilyCheckedDataType(first->data_type);

    ASSERT(custom);

    LLVMTypeRef ptr_type = generate_data_type__LilyIrLlvm(self, custom, scope);

    LLVMValueRef field_access = LLVMBuildGEP2(
      self->builder,
      ptr_type,
      value_ptr,
      indices,
      first_is_ptr ? expr->call.record_field_access.accesses->len
                   : expr->call.record_field_access.accesses->len - 1,
      "");

    LLVMSetIsInBounds(field_access, true);

    // This is the type of the expression.
    LLVMTypeRef field_access_type =
      generate_data_type__LilyIrLlvm(self, expr->data_type, scope);

    switch (expr->data_type->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
            return field_access;
        default:
            return LLVMBuildLoad2(
              self->builder, field_access_type, field_access, "");
    }
}

LLVMValueRef
generate_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                               const LilyCheckedExpr *expr,
                               LilyLlvmScope *scope,
                               LLVMValueRef fun,
                               LLVMValueRef ptr)
{
    switch (expr->call.kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_CONSTANT:
            return search_value__LilyLlvmScope(scope, expr->call.global_name)
              ->value;
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE: {
            LLVMTypeRef type =
              generate_data_type__LilyIrLlvm(self, expr->data_type, scope);

            // switch (expr->data_type->kind) {
            //     case LILY_CHECKED_DATA_TYPE_KIND_STR: {
            //         LLVMValueRef str_ptr =
            //           search_value__LilyLlvmScope(scope,
            //           expr->call.global_name)
            //             ->value;
            //         LLVMValueRef str_value = LLVMBuildLoad2(
            //           self->builder,
            //           LLVMStructType(
            //             (LLVMTypeRef[]){
            //               ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self)),
            //               intptr__LilyIrLlvm(self) },
            //             2,
            //             false),
            //           str_ptr,
            //           "");

            //         return LLVMBuildExtractValue(
            //           self->builder, str_value, 0, "");
            //     }
            //     default:
            //         break;
            // }

            if (LLVMGetTypeKind(type) == LLVMPointerTypeKind) {
                return search_value__LilyLlvmScope(scope,
                                                   expr->call.global_name)
                  ->value;
            } else {
                return load_value__LilyLlvmScope(
                  scope, self, type, expr->call.global_name);
            }
        }
        case LILY_CHECKED_EXPR_CALL_KIND_FUN:
            return generate_fun_call_expr__LilyIrLlvm(
              self, expr, scope, fun, ptr);
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
            return LLVMGetParam(fun, expr->call.fun_param);
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS:
            return generate_sys_fun_call_expr__LilyIrLlvm(
              self, expr, scope, fun, ptr);
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD:
            return generate_record_call_expr__LilyIrLlvm(
              self, expr, scope, fun, ptr);
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS:
            return generate_field_access_expr__LilyIrLlvm(
              self, expr, scope, fun, ptr);
        default:
            TODO("generate_call_expr__LilyIrLlvm");
    }
}

LLVMValueRef
generate_ref_call_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                   const LilyCheckedExpr *expr,
                                   LilyLlvmScope *scope,
                                   LLVMValueRef fun)
{
    switch (expr->call.kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
            return search_value__LilyLlvmScope(scope, expr->call.global_name)
              ->value;
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
            return LLVMGetParam(fun, expr->call.fun_param);
        default:
            UNREACHABLE("this kind of call expression cannot be a ref");
    }
}
