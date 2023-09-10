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

extern "C"
{
#include <base/macros.h>

#include <core/lily/compiler/driver/lld.h>
}

#include <lld/Common/Driver.h>

using namespace llvm;

bool
LilyLLDLink(enum LilyCompilerLinkerObjectFormat obj_format,
            const char **args,
            Usize arg_len)
{
    ArrayRef<const char *> array_ref_args(args, arg_len);

    switch (obj_format) {
        case LILY_COMPILER_LINKER_OBJECT_FORMAT_COFF:
            return lld::coff::link(
              array_ref_args, llvm::outs(), llvm::errs(), true, false);
        case LILY_COMPILER_LINKER_OBJECT_FORMAT_ELF:
            return lld::elf::link(
              array_ref_args, llvm::outs(), llvm::errs(), true, false);
        case LILY_COMPILER_LINKER_OBJECT_FORMAT_MACHO:
            return lld::macho::link(
              array_ref_args, llvm::outs(), llvm::errs(), true, false);
        case LILY_COMPILER_LINKER_OBJECT_FORMAT_MINGW:
            return lld::mingw::link(
              array_ref_args, llvm::outs(), llvm::errs(), true, false);
        case LILY_COMPILER_LINKER_OBJECT_FORMAT_UNKNOWN:
            UNREACHABLE("unknown object format");
        case LILY_COMPILER_LINKER_OBJECT_FORMAT_WASM:
            return lld::wasm::link(
              array_ref_args, llvm::outs(), llvm::errs(), true, false);
        default:
            UNREACHABLE("unknown variant");
    }
}
