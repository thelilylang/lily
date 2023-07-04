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

#include <base/file.h>
#include <base/macros.h>

#include <cli/emit.h>
#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/linker.h>
#include <core/lily/compiler/package.h>

void
run__LilyIrLlvmLinker(LilyPackage *self)
{
#ifdef LILY_WINDOWS_OS
    TODO("add a support for windows");
#else
    // Remove `.lily` from the path of the original filename.
    char *path_base =
      get_slice__Str(self->file.name, 0, strlen(self->file.name) - 5);

    String *object_path = format__String("{sa}.o", path_base);
    char *error_msg = NULL;

    LLVMTargetMachineEmitToFile(self->ir.llvm.machine,
                                self->ir.llvm.module,
                                object_path->buffer,
                                LLVMObjectFile,
                                &error_msg);

    if (error_msg) {
        EMIT_ERROR(error_msg);
        LLVMDisposeMessage(error_msg);
        exit(1);
    }

    // append__String(self->linker.llvm.command, path);
    // push_str__String(self->linker.llvm.command, " -llily_sys
    // -llily_builtin");

    // char *content = LLVMPrintModuleToString(self->ir.llvm.module);

    // write_file__File(path->buffer, content);

    // system(self->linker.llvm.command->buffer);
    // remove(path->buffer);

    // lily_free(content);
    // FREE(String, path);
#endif
}
