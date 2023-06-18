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

#include <base/print.h>

#include <core/lily/lily.h>
#include <core/lily/mir/generator.h>
#include <core/lily/mir/generator/constant.h>
#include <core/lily/mir/generator/fun.h>
#include <core/lily/package.h>

void
run__LilyMir(LilyPackage *self)
{
    for (Usize i = 0; i < self->analysis.module.decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(self->analysis.module.decls, i);

        switch (decl->kind) {
            case LILY_CHECKED_DECL_KIND_CONSTANT:
                LilyMirAddInst(
                  &self->mir_module,
                  generate_constant__LilyMir(&self->mir_module, decl));

                break;
            case LILY_CHECKED_DECL_KIND_ERROR:
                break;
            case LILY_CHECKED_DECL_KIND_FUN:
                generate_fun__LilyMir(&self->mir_module, decl);

                break;
            case LILY_CHECKED_DECL_KIND_METHOD:
                break;
            case LILY_CHECKED_DECL_KIND_MODULE:
                break;
            case LILY_CHECKED_DECL_KIND_OBJECT:
                break;
            case LILY_CHECKED_DECL_KIND_TYPE:
                break;
        }

        if (decl->kind != LILY_CHECKED_DECL_KIND_FUN) {
            LilyMirPopCurrent(&self->mir_module);
        }
    }

#ifdef DEBUG_MIR
    printf("====MIR Generator(%s)====\n", self->global_name->buffer);

    PRINT_STRING_ORD_HASH_MAP(self->mir_module.insts,
                              to_string__Debug__LilyMirInstruction);
#endif
}
