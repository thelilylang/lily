/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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
#include <core/lily/mir/generator/enum.h>

void
generate_enum__LilyMir(LilyMirModule *module, LilyCheckedDecl *enum_)
{
    ASSERT(enum_->type.enum_.signatures->len > 0);

    for (Usize i = 0; i < enum_->type.enum_.signatures->len; ++i) {
        LilyCheckedSignatureType *signature =
          get__Vec(enum_->type.enum_.signatures, i);

        if (!has_only_known_dt__LilyCheckedSignatureType(signature)) {
            continue;
        } else if (!LilyMirKeyIsUnique(module,
                                       signature->ser_global_name->buffer)) {
            continue;
        }

        Vec *fields = init__Vec(1, NEW(LilyMirDt, LILY_MIR_DT_KIND_U8));
        LilyMirInstruction *inst =
          NEW_VARIANT(LilyMirInstruction,
                      struct,
                      NEW(LilyMirInstructionStruct,
                          get_linkage_from_visibility__LilyMirLinkage(
                            enum_->type.enum_.visibility),
                          signature->ser_global_name->buffer,
                          fields,
                          signature->generic_params));

        LilyMirAddInst(module, inst);

        for (Usize j = 0; j < enum_->type.enum_.variants->len; ++j) {
            LilyCheckedVariant *variant =
              get__Vec(enum_->type.enum_.variants, j);
            LilyCheckedSignatureVariant *variant_signature = NULL;

            if (variant->signatures->len == 1 && i > 0) {
                push__Vec(fields,
                          NEW_VARIANT(LilyMirDt,
                                      struct_name,
                                      CAST(LilyCheckedSignatureVariant *,
                                           get__Vec(variant->signatures, 0))
                                        ->ser_global_name->buffer));

                continue;
            } else {
                variant_signature = get__Vec(variant->signatures, i);
            }

            Vec *variant_fields = NEW(Vec);
            LilyMirInstruction *variant_inst =
              NEW_VARIANT(LilyMirInstruction,
                          struct,
                          NEW(LilyMirInstructionStruct,
                              get_linkage_from_visibility__LilyMirLinkage(
                                enum_->type.enum_.visibility),
                              variant_signature->ser_global_name->buffer,
                              variant_fields,
                              signature->generic_params));

            LilyMirAddInst(module, variant_inst);

            if (variant_signature->resolve_dt) {
                push__Vec(
                  variant_fields,
                  generate_dt__LilyMir(module, variant_signature->resolve_dt));
            }

            LilyMirPopCurrent(module);

            push__Vec(fields,
                      NEW_VARIANT(LilyMirDt,
                                  struct_name,
                                  variant_signature->ser_global_name->buffer));
        }

        LilyMirPopCurrent(module);
    }
}
