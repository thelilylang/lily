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

#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/generator.h>
#include <core/lily/compiler/ir/llvm/generator/builtin.h>
#include <core/lily/compiler/ir/llvm/generator/constant.h>
#include <core/lily/compiler/ir/llvm/generator/function.h>
#include <core/lily/compiler/ir/llvm/generator/sys.h>
#include <core/lily/compiler/ir/llvm/generator/type/enum.h>
#include <core/lily/compiler/ir/llvm/generator/type/record.h>
#include <core/lily/compiler/package.h>

#include <llvm-c/DebugInfo.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Transforms/Utils.h>
#include <llvm-c/Transforms/Vectorize.h>

#include <stdio.h>

void
run__LilyIrLlvmGenerator(LilyPackage *self)
{
    // 1. Generate LLVM IR code

    String *filename = get_filename_from_path(self->file.name);
    Usize dir_len = strlen(self->file.name) - filename->len;
    char *dir = get_slice__Str(self->file.name, 0, dir_len);

    self->ir.llvm.file = LLVMDIBuilderCreateFile(
      self->ir.llvm.di_builder, filename->buffer, filename->len, dir, dir_len);

    LilyLlvmScope *scope = NEW(LilyLlvmScope, NULL);

    declare_builtin_function__LilyIrLlvm(&self->ir.llvm, scope, self);
    declare_sys_function__LilyIrLlvm(&self->ir.llvm, scope, self);

    for (Usize i = 0; i < self->analysis.module.decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(self->analysis.module.decls, i);

        switch (decl->kind) {
            case LILY_CHECKED_DECL_KIND_CONSTANT:
                generate_constant__LilyIrLlvm(
                  &self->ir.llvm, &decl->constant, scope);

                break;
            case LILY_CHECKED_DECL_KIND_ERROR:
                TODO("generate error");
            case LILY_CHECKED_DECL_KIND_FUN:
                generate_function__LilyIrLlvm(
                  &self->ir.llvm, &decl->fun, scope, decl->location);

                break;
            case LILY_CHECKED_DECL_KIND_METHOD:
                TODO("generate method");
            case LILY_CHECKED_DECL_KIND_MODULE:
                TODO("generate module");
            case LILY_CHECKED_DECL_KIND_OBJECT:
                TODO("generate object");
            case LILY_CHECKED_DECL_KIND_TYPE:
                switch (decl->type.kind) {
                    case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                        generate_record__LilyIrLlvm(
                          &self->ir.llvm, &decl->type.record, scope);

                        break;
                    case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                        generate_enum__LilyIrLlvm(
                          &self->ir.llvm, &decl->type.enum_, scope);

                        break;
                    default:
                        TODO("generate type");
                }

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    // 2. Create a pass manager
    LLVMPassManagerRef pass_manager = LLVMCreatePassManager();

    // 2.1 Add passes to the pass manager
    LLVMAddInstructionCombiningPass(pass_manager);
    LLVMAddPromoteMemoryToRegisterPass(pass_manager);
    LLVMAddLoopVectorizePass(pass_manager);

    // 3. Run pass manager
    LLVMRunPassManager(pass_manager, self->ir.llvm.module);

#ifdef ENV_DEBUG
    printf("====LLVM IR Generator(%s)====\n", self->global_name->buffer);

    dump__LilyIrLlvm(&self->ir.llvm);
#endif

    FREE(String, filename);
    lily_free(dir);
    FREE(LilyLlvmScope, scope);
    LLVMDisposePassManager(pass_manager);
}