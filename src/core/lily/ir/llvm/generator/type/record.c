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

#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/generator/type/record.h>

void
generate_record__LilyIrLlvm(const LilyIrLlvm *self,
                            const LilyCheckedDeclRecord *record,
                            LilyLlvmScope *scope)
{
    LLVMTypeRef fields[252] = { 0 };

    for (Usize i = 0; i < record->fields->len; ++i) {
        LilyCheckedField *field = get__Vec(record->fields, i);

        fields[i] =
          generate_data_type__LilyIrLlvm(self, field->data_type, scope);
    }

    LLVMTypeRef record_llvm =
      LLVMStructCreateNamed(self->context, record->global_name->buffer);
    LLVMStructSetBody(record_llvm, fields, record->fields->len, false);

    push__Vec(scope->types,
              NEW(LilyLlvmType, record->global_name, record_llvm));
}
