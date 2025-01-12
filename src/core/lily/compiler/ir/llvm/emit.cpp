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

#include <core/lily/compiler/ir/llvm/emit.h>

#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>

#include <llvm-c/Analysis.h>

using namespace llvm;

int
LilyLLVMEmit(const LilyIrLlvm *self,
             char **error_msg,
             const char *filename,
             bool emit_obj,
             bool emit_asm,
             bool emit_ir,
             bool emit_bitcode)
{
    if (emit_ir) {
        if (LLVMPrintModuleToFile(self->module, filename, error_msg)) {
            return 1;
        }

        return 0;
    }

    auto &module = *unwrap(self->module);
    auto &machine = *reinterpret_cast<TargetMachine *>(self->machine);
    std::error_code ec;
    legacy::PassManager code_gen_passes;

    if (emit_asm) {
        auto *out_asm =
          new (std::nothrow) raw_fd_ostream(filename, ec, sys::fs::OF_None);

        if (ec) {
            *error_msg =
              strdup((const char *)StringRef(ec.message()).bytes_begin());

            return 1;
        }

        if (machine.addPassesToEmitFile(code_gen_passes,
                                        *out_asm,
                                        nullptr,
                                        llvm::CodeGenFileType::AssemblyFile)) {
            *error_msg = strdup((const char *)"Can't emit file");

            return 1;
        }

        code_gen_passes.run(module);
        out_asm->flush();
        out_asm->close();

        delete out_asm;
    } else if (emit_obj) {
        auto *out_obj =
          new (std::nothrow) raw_fd_ostream(filename, ec, sys::fs::OF_None);

        if (ec) {
            *error_msg =
              strdup((const char *)StringRef(ec.message()).bytes_begin());

            return 1;
        }

        if (machine.addPassesToEmitFile(code_gen_passes,
                                        *out_obj,
                                        nullptr,
                                        llvm::CodeGenFileType::ObjectFile)) {
            *error_msg = strdup((const char *)"Can't emit file");

            return 1;
        }

        code_gen_passes.run(module);
        out_obj->flush();
        out_obj->close();

        delete out_obj;
    } else if (emit_bitcode) {
        auto *out_bc =
          new (std::nothrow) raw_fd_ostream(filename, ec, sys::fs::OF_None);

        if (ec) {
            *error_msg =
              strdup((const char *)StringRef(ec.message()).bytes_begin());

            return 1;
        }

        WriteBitcodeToFile(module, *out_bc);
        code_gen_passes.run(module);
        out_bc->flush();
        out_bc->close();

        delete out_bc;
    }

    return 0;
}
