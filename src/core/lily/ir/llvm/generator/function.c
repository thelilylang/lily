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

#include <core/lily/checked/signature.h>
#include <core/lily/ir/llvm/attr.h>
#include <core/lily/ir/llvm/generator/body/function.h>
#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/generator/expr.h>
#include <core/lily/ir/llvm/generator/function.h>
#include <core/lily/ir/llvm/generator/main.h>
#include <core/lily/ir/llvm/generator/stmt.h>
#include <core/lily/ir/llvm/scope.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <llvm-c/DebugInfo.h>

#define GENERATE_FUNCTION_DEBUG()                                      \
    /* Add debug location to the function */                           \
    LLVMMetadataRef debug_location = LLVMDIBuilderCreateDebugLocation( \
      self->context,                                                   \
      location->start_line,                                            \
      location->start_column,                                          \
      LLVMDIBuilderCreateLexicalBlockFile(                             \
        self->di_builder, self->file, self->file, 0),                  \
      NULL);                                                           \
                                                                       \
    LLVMSetSubprogram(fun_llvm, debug_location);

#define GENERATE_FUNCTION_ATTRS()                                        \
    /* Generate some function attributes */                              \
    if (!fun->is_recursive) {                                            \
        LLVM_ADD_FN_ATTR(fun_llvm, norecurse_attr__LilyIrLlvm(self));    \
    }                                                                    \
                                                                         \
    if (!fun->can_inline) {                                              \
        LLVM_ADD_FN_ATTR(fun_llvm, noinline_attr__LilyIrLlvm(self));     \
        LLVM_ADD_FN_ATTR(fun_llvm, optnone_attr__LilyIrLlvm(self));      \
    } else {                                                             \
        LLVM_ADD_FN_ATTR(fun_llvm, alwaysinline_attr__LilyIrLlvm(self)); \
    }                                                                    \
                                                                         \
    if (!fun->can_raise) {                                               \
        LLVM_ADD_FN_ATTR(fun_llvm, nounwind_attr__LilyIrLlvm(self));     \
    }                                                                    \
                                                                         \
    LLVM_ADD_FN_ATTR(fun_llvm, uwtable_attr__LilyIrLlvm(self));          \
    ADD_CUSTOM_HOST_ATTR(fun_llvm);

void
generate_function__LilyIrLlvm(const LilyIrLlvm *self,
                              const LilyCheckedDeclFun *fun,
                              LilyLlvmScope *scope,
                              const Location *location)
{
    if (fun->is_main) {
        LLVMValueRef fun_llvm =
          generate_main_function__LilyIrLlvm(self, fun, scope, location);

        GENERATE_FUNCTION_DEBUG();
        GENERATE_FUNCTION_ATTRS();

        return;
    }

    if (fun->signatures->len == 1 &&
        contains_uncertain_dt__LilyCheckedDeclFun(fun)) {
        return;
    }

    Usize i = 0;

    // To skip the first signature of the function with compiler defined data
    // type.
    if (contains_uncertain_dt__LilyCheckedDeclFun(fun)) {
        i = 1;
    }

    for (; i < fun->signatures->len; ++i) {
        LilyCheckedSignatureFun *signature = get__Vec(fun->signatures, i);

        ASSERT(signature->types->len >= 1);

        // Generate return data type of the function
        LLVMTypeRef return_data_type = generate_data_type__LilyIrLlvm(
          self, last__Vec(signature->types), scope);

        // Generate function parameters
        LLVMTypeRef params[252] = {};
        Usize params_len = signature->types->len - 1;

        for (Usize i = 0; i < signature->types->len - 1; ++i) {
            params[i] = generate_data_type__LilyIrLlvm(
              self, get__Vec(signature->types, i), scope);
        }

        // TODO: check va_arg param
        LLVMTypeRef fun_data_type =
          LLVMFunctionType(return_data_type, params, params_len, false);

        LLVMValueRef fun_llvm = LLVMAddFunction(
          self->module, signature->global_name->buffer, fun_data_type);

        // Set alignment on param (if applicable)
        for (Usize i = 0; i < signature->types->len - 1; ++i) {
            switch (CAST(LilyCheckedDataType *, get__Vec(signature->types, i))
                      ->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_REF: {
                    LLVMValueRef param = LLVMGetParam(fun_llvm, i);

                    LLVMSetParamAlignment(
                      param,
                      LLVMABIAlignmentOfType(self->target_data,
                                             LLVMTypeOf(param)));
                }
                default:
                    break;
            }
        }

        push__Vec(
          scope->funs,
          NEW(LilyLlvmFun, signature->global_name, fun_llvm, fun_data_type));
        push__Vec(scope->values,
                  NEW(LilyLlvmValue, signature->global_name, fun_llvm));

        LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(fun_llvm, "entry");
        LLVMPositionBuilderAtEnd(self->builder, entry_block);

        LilyLlvmScope *fun_scope = NEW(LilyLlvmScope, scope);

        GENERATE_FUNCTION_BODY(fun->body, fun_llvm, NULL, NULL, fun_scope);

        GENERATE_FUNCTION_DEBUG();
        GENERATE_FUNCTION_ATTRS();

        FREE(LilyLlvmScope, fun_scope);
    }
}
