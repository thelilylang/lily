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

#include <base/macros.h>

#include <core/lily/ir/llvm/dump.h>
#include <core/lily/ir/llvm/generator.h>
#include <core/lily/ir/llvm/generator/function.h>
#include <core/lily/package.h>

#include <stdio.h>

void
run__LilyIrLlvmGenerator(LilyPackage *self)
{
    for (Usize i = 0; i < self->analysis.module.decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(self->analysis.module.decls, i);

        switch (decl->kind) {
            case LILY_CHECKED_DECL_KIND_CONSTANT:
                TODO("generate constant");
            case LILY_CHECKED_DECL_KIND_ERROR:
                TODO("generate error");
            case LILY_CHECKED_DECL_KIND_FUN:
                generate_function__LilyIrLlvm(&self->ir.llvm, &decl->fun);
                break;
            case LILY_CHECKED_DECL_KIND_METHOD:
                TODO("generate method");
            case LILY_CHECKED_DECL_KIND_MODULE:
                TODO("generate module");
            case LILY_CHECKED_DECL_KIND_OBJECT:
                TODO("generate object");
            case LILY_CHECKED_DECL_KIND_TYPE:
                TODO("generate type");
            default:
                UNREACHABLE("unknown variant");
        }
    }

#ifdef ENV_DEBUG
    printf("====LLVM IR Generator(%s)====\n", self->global_name->buffer);

    dump__LilyIrLlvm(&self->ir.llvm);
#endif
}
