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

#include <cli/version.h>

#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/generator.h>
#include <core/lily/compiler/ir/llvm/lily.h>
#include <core/lily/lily.h>
#include <core/lily/package/package.h>

#include <llvm-c/DebugInfo.h>

void
run__LilyIrLlvmGenerator(LilyPackage *self)
{
    ASSERT(self->kind == LILY_PACKAGE_KIND_COMPILER);

    // Init file (LLVMMetadataRef)
    // TODO: improve that later
    self->compiler.ir.llvm.file =
      LLVMDIBuilderCreateFile(self->compiler.ir.llvm.di_builder,
                              self->file.name,
                              strlen(self->file.name),
                              "",
                              0);
    self->compiler.ir.llvm.compile_unit = LLVMDIBuilderCreateCompileUnit(
      self->compiler.ir.llvm.di_builder,
      LLVMDWARFSourceLanguageC, // Source language (e.g., C, C++, etc.)
      self->compiler.ir.llvm.file,
      "lily-" VERSION, // Compiler version string
      strlen("lily-" VERSION),
      self->compiler.config->o0 ? false : true,
      "",
      0,
      0,
      "",
      0,
      LLVMDWARFEmissionFull,
      0,
      false,
      false,
      "",
      0,
      "",
      0);

    LilyLLVMPrepareModule(&self->compiler.ir.llvm, self->mir_module.insts);
    LilyLLVMRunModule(&self->compiler.ir.llvm, self->mir_module.insts);

#ifdef DEBUG_MIR
    printf("====LLVM IR Generator(%s)====\n", self->global_name->buffer);

    dump__LilyIrLlvm(&self->compiler.ir.llvm);
#endif
}
