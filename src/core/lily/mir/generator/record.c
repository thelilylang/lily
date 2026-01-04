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

#include <core/lily/mir/generator.h>
#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/record.h>

void
generate_record__LilyMir(LilyMirModule *module, LilyCheckedDecl *record)
{
    ASSERT(record->type.record.signatures->len > 0);

    for (Usize i = 0; i < record->type.record.signatures->len; ++i) {
        LilyCheckedSignatureType *signature =
          get__Vec(record->type.record.signatures, i);

        if (!has_only_known_dt__LilyCheckedSignatureType(signature)) {
            continue;
        } else if (!LilyMirKeyIsUnique(module,
                                       signature->ser_global_name->buffer)) {
            continue;
        }

        Vec *fields = NEW(Vec);

        LilyMirInstruction *inst =
          NEW_VARIANT(LilyMirInstruction,
                      struct,
                      NEW(LilyMirInstructionStruct,
                          get_linkage_from_visibility__LilyMirLinkage(
                            record->type.record.visibility),
                          signature->ser_global_name->buffer,
                          fields,
                          signature->generic_params));

        LilyMirAddInst(module, inst);

        for (Usize j = 0; j < record->type.record.fields->len; ++j) {
            LilyCheckedField *field = get__Vec(record->type.record.fields, j);

            push__Vec(fields, generate_dt__LilyMir(module, field->data_type));
        }

        LilyMirPopCurrent(module);
    }
}
