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

#include <core/lily/compiler/ir/llvm/emit.h>

#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>

#include <llvm-c/Analysis.h>

using namespace llvm;

bool
LilyLLVMEmit(const LilyIrLlvm *self,
             char **error_msg,
             const char *filename,
             bool emit_obj,
             bool emit_asm,
             bool emit_ir,
             bool emit_bitcode)
{
    // if (LLVMVerifyModule(self->module, LLVMPrintMessageAction, error_msg)) {
    //     return 1;
    // }

    raw_fd_ostream *out_obj_ptr = nullptr;
    raw_fd_ostream *out_asm_ptr = nullptr;
    raw_fd_ostream *out_bc_ptr = nullptr;

    if (emit_asm) {
        std::error_code ec;
        out_asm_ptr =
          new (std::nothrow) raw_fd_ostream(filename, ec, sys::fs::OF_None);

        if (ec) {
            *error_msg =
              strdup((const char *)StringRef(ec.message()).bytes_begin());

            return 1;
        }
    }

    if (emit_obj) {
        std::error_code ec;
        out_obj_ptr =
          new (std::nothrow) raw_fd_ostream(filename, ec, sys::fs::OF_None);

        if (ec) {
            *error_msg =
              strdup((const char *)StringRef(ec.message()).bytes_begin());

            return 1;
        }
    }

    if (emit_bitcode) {
        std::error_code ec;
        out_bc_ptr =
          new (std::nothrow) raw_fd_ostream(filename, ec, sys::fs::OF_None);

        if (ec) {
            *error_msg =
              strdup((const char *)StringRef(ec.message()).bytes_begin());

            return 1;
        }
    }

    std::unique_ptr<raw_fd_ostream> out_asm(out_asm_ptr), out_obj(out_obj_ptr),
      out_bc(out_bc_ptr);
    auto &module = *unwrap(self->module);
    auto &machine = *reinterpret_cast<TargetMachine *>(self->machine);

    legacy::PassManager cpm;

    cpm.add(
      createTargetTransformInfoWrapperPass(machine.getTargetIRAnalysis()));
    cpm.run(module);

    if (emit_obj) {
        if (machine.addPassesToEmitFile(
              cpm, *out_obj, nullptr, CGFT_ObjectFile)) {
            *error_msg = strdup("Can't emit an object file");
            return 1;
        }
    }

    if (emit_asm) {
        if (machine.addPassesToEmitFile(
              cpm, *out_asm, nullptr, CGFT_AssemblyFile)) {
            *error_msg = strdup("Can't emit assembly file");
            return 1;
        }
    }

    if (emit_ir) {
        if (LLVMPrintModuleToFile(self->module, filename, error_msg)) {
            return 1;
        }
    }

    if (emit_obj) {
        WriteBitcodeToFile(module, *out_obj);
    }

    if (emit_bitcode) {
        WriteBitcodeToFile(module, *out_bc);
    }

    return 0;
}
