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

#include <core/lily/compiler/ir/llvm/generator/data_type.h>
#include <core/lily/compiler/ir/llvm/generator/type/enum.h>

void
generate_enum__LilyIrLlvm(const LilyIrLlvm *self,
                          const LilyCheckedDeclEnum *enum_,
                          LilyLlvmScope *scope)
{
    // Generate all variants
    for (Usize i = 0; i < enum_->variants->len; ++i) {
        LilyCheckedVariant *variant = get__Vec(enum_->variants, i);
        LLVMTypeRef variant_llvm =
          LLVMStructCreateNamed(self->context, variant->global_name->buffer);

        if (variant->data_type) {
            LLVMStructSetBody(variant_llvm,
                              (LLVMTypeRef[]){ generate_data_type__LilyIrLlvm(
                                self, variant->data_type, scope) },
                              1,
                              false);
        } else {
            LLVMStructSetBody(variant_llvm, (LLVMTypeRef[]){ 0 }, 0, false);
        }
    }

    LLVMTypeRef enum_llvm =
      LLVMStructCreateNamed(self->context, enum_->global_name->buffer);
    LLVMTypeRef variants[252] = { 0 };

    for (Usize i = 0; i < enum_->variants->len; ++i) {
        LilyCheckedVariant *variant = get__Vec(enum_->variants, i);
        variants[i] =
          LLVMGetTypeByName2(self->context, variant->global_name->buffer);
    }

    LLVMStructSetBody(enum_llvm, variants, enum_->variants->len, false);
}
