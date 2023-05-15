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

#include <core/lily/ir/llvm/attr.h>
#include <core/lily/ir/llvm/generator/body/function.h>
#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/generator/expr.h>
#include <core/lily/ir/llvm/generator/function.h>
#include <core/lily/ir/llvm/generator/stmt.h>
#include <core/lily/ir/llvm/scope.h>

#include <stdio.h>
#include <stdlib.h>

#include <llvm-c/DebugInfo.h>

void
generate_function__LilyIrLlvm(const LilyIrLlvm *self,
                              const LilyCheckedDeclFun *fun,
                              LilyLlvmScope *scope,
                              const Location *location)
{
    LLVMTypeRef return_data_type =
      generate_data_type__LilyIrLlvm(self, fun->return_data_type, scope);
    LLVMTypeRef params[252] = {};
    Usize params_len = 0;

    if (fun->params) {
        params_len = fun->params->len;

        for (Usize i = 0; i < fun->params->len; ++i) {
            params[i] = generate_data_type__LilyIrLlvm(
              self,
              CAST(LilyCheckedDeclFunParam *, get__Vec(fun->params, i))
                ->data_type,
              scope);
        }
    }

    // TODO: check va_arg param
    LLVMTypeRef fun_data_type =
      LLVMFunctionType(return_data_type, params, params_len, false);

    char *name = NULL;

    if (fun->is_main) {
        name = fun->name->buffer;
    } else {
        name = fun->global_name->buffer;
    }

    LLVMValueRef fun_llvm = LLVMAddFunction(self->module, name, fun_data_type);

    push__Vec(scope->funs,
              NEW(LilyLlvmFun, fun->global_name, fun_llvm, fun_data_type));
    push__Vec(scope->values, NEW(LilyLlvmValue, fun->global_name, fun_llvm));

    LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(fun_llvm, "entry");
    LLVMPositionBuilderAtEnd(self->builder, entry_block);

    LilyLlvmScope *fun_scope = NEW(LilyLlvmScope, scope);

    if (fun->params) {
        for (Usize i = 0; i < fun->params->len; ++i) {
            LilyCheckedDeclFunParam *param = get__Vec(fun->params, i);

            if (param->data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_REF) {
                LLVMAddAttributeAtIndex(
                  fun_llvm, i, nonnull_attr__LilyIrLlvm(self));
            }
        }
    }

    GENERATE_FUNCTION_BODY(fun->body, fun_llvm, NULL, NULL, fun_scope);

    LLVMMetadataRef debug_location = LLVMDIBuilderCreateDebugLocation(
      self->context,
      location->start_line,
      location->start_column,
      LLVMDIBuilderCreateLexicalBlockFile(
        self->di_builder, self->file, self->file, 0),
      NULL);

    // Attach the debug location to the function
    LLVMSetSubprogram(fun_llvm, debug_location);

    FREE(LilyLlvmScope, fun_scope);
}
