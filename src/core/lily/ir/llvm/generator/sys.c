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
#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/generator/sys.h>

void
declare_sys_function__LilyIrLlvm(LilyIrLlvm *self,
                                 LilyLlvmScope *scope,
                                 LilyPackage *package)
{
    for (Usize i = 0; i < package->sys_usage->len; ++i) {
        LilySysFun *sys_fun = get__Vec(package->sys_usage, i);

        LLVMTypeRef return_data_type = generate_data_type__LilyIrLlvm(
          self, sys_fun->return_data_type, scope);
        LLVMTypeRef params[252] = {};
        Usize params_len = 0;

        if (sys_fun->params) {
            params_len = sys_fun->params->len;

            for (Usize j = 0; j < sys_fun->params->len; ++j) {
                params[j] = generate_data_type__LilyIrLlvm(
                  self, get__Vec(sys_fun->params, j), scope);
            }
        }

        LLVMTypeRef sys_fun_type =
          LLVMFunctionType(return_data_type, params, params_len, false);
        LLVMValueRef sys_fun_llvm = LLVMAddFunction(
          self->module, sys_fun->real_name->buffer, sys_fun_type);
        LLVMSetFunctionCallConv(sys_fun_llvm, LLVMFastCallConv);

        LLVM_ADD_FN_ATTR(sys_fun_llvm, hot_attr__LilyIrLlvm(self));
        LLVM_ADD_FN_ATTR(sys_fun_llvm, nounwind_attr__LilyIrLlvm(self));
        LLVM_ADD_FN_ATTR(sys_fun_llvm, norecurse_attr__LilyIrLlvm(self));

        push__Vec(scope->values,
                  NEW(LilyLlvmValue, sys_fun->real_name, sys_fun_llvm));
        push__Vec(
          scope->funs,
          NEW(LilyLlvmFun, sys_fun->real_name, sys_fun_llvm, sys_fun_type));
    }
}
