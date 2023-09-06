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
#include <core/lily/compiler/linker/lld.h>
#include <core/lily/compiler/package.h>

#define EXTENSION ".lily"
#undef EXTENSION
#define EXTENSION_LEN 5

#ifdef ENV_LOCAL
#define LIB_DIR_BUILD "build"
#define LIB_DIR_BUILD_DEBUG "build/Debug"
#endif

#define OBJ_DIR_PATH "out.lily/obj/"
#define BIN_DIR_PATH "out.lily/bin/"

void
compile_exe__LilyIrLlvmLinker(LilyPackage *self)
{
    // Remove `.lily` from the path of the original filename.
    String *output_name = get_filename__File(self->file.name);
    String *path_base = get_filename__File(self->output_path);

    // Default library link.
    // Link @sys and @builtin library.
    push__Vec(self->linker.llvm.args, strdup("-llily_sys"));
    push__Vec(self->linker.llvm.args, strdup("-llily_builtin"));

    // Link crt1, crti, crtn and libc
    push__Vec(self->linker.llvm.args, strdup("/usr/lib/crt1.o"));
    push__Vec(self->linker.llvm.args, strdup("/usr/lib/crti.o"));
    push__Vec(self->linker.llvm.args, strdup("/usr/lib/crtn.o"));
    push__Vec(self->linker.llvm.args, strdup("/usr/lib/libc.so.6"));
    push__Vec(self->linker.llvm.args, strdup("-pie"));

    // Add dynamic linker option
    push__Vec(self->linker.llvm.args, strdup("-dynamic-linker"));
    push__Vec(self->linker.llvm.args, strdup("/lib64/ld-linux-x86-64.so.2"));

#ifdef ENV_LOCAL
    push__Vec(self->linker.llvm.args, strdup("-L" LIB_DIR_BUILD));
    push__Vec(self->linker.llvm.args, strdup("-L" LIB_DIR_BUILD_DEBUG));
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
    push__Vec(self->linker.llvm.args,
              format("{s}{Sr}.o", OBJ_DIR_PATH, path_base));
#elifdef LILY_WINDOWS_OS
    push__Vec(self->linker.llvm.args,
              format("{s}{Sr}.obj", OBJ_DIR_PATH, path_base));
#else
#error "unknown OS"
#endif

    // Add output option
    // TODO: Check there is passed `-o` option
    push__Vec(self->linker.llvm.args, strdup("-o"));
    push__Vec(self->linker.llvm.args,
              format("{s}{Sr}", BIN_DIR_PATH, output_name));

    // Add optimization options
    if (self->config->o3) {
        push__Vec(self->linker.llvm.args, strdup("-O3"));
    } else if (self->config->o2) {
        push__Vec(self->linker.llvm.args, strdup("-O2"));
    } else if (self->config->o1) {
        push__Vec(self->linker.llvm.args, strdup("-O1"));
    } else if (self->config->o0) {
        push__Vec(self->linker.llvm.args, strdup("-O0"));
    }

    if (self->config->oz) {
        push__Vec(self->linker.llvm.args, strdup("-Oz"));
    }

#ifdef ENV_DEBUG
    printf("====Link(%s)====\n", self->name->buffer);

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
    printf("ld.lld ");
#elifdef LILY_APPLE_OS
    printf("ld64.lld ");
#elifdef LILY_WINDOWS_OS
    printf("lld-link ");
#else
#error "unknown OS"
#endif

    for (Usize i = 0; i < self->linker.llvm.args->len; ++i) {
        printf("%s", (char *)get__Vec(self->linker.llvm.args, i));

        if (i + 1 != self->linker.llvm.args->len) {
            printf(" ");
        }
    }

    puts("");
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
    enum LilyCompilerLinkerObjectFormat obj_format =
      LILY_COMPILER_LINKER_OBJECT_FORMAT_ELF;
#elifdef LILY_APPLE_OS
    enum LilyCompilerLinkerObjectFormat obj_format =
      LILY_COMPILER_LINKER_OBJECT_FORMAT_MACHO;
#elifdef LILY_WINDOWS_OS
    enum LilyCompilerLinkerObjectFormat obj_format =
      LILY_COMPILER_LINKER_OBJECT_FORMAT_COFF;
#else
#error "unknown OS"
#endif

    if (!LilyLLDLink(obj_format,
                     (const char **)self->linker.llvm.args->buffer,
                     self->linker.llvm.args->len)) {
        EMIT_ERROR("link error");
        exit(1);
    }
}

void
compile_lib__LilyIrLlvmLinker(LilyPackage *self)
{
}

DESTRUCTOR(LilyIrLlvmLinker, const LilyIrLlvmLinker *self)
{
    for (Usize i = 0; i < self->args->len; ++i) {
        lily_free(get__Vec(self->args, i));
    }

    FREE(Vec, self->args);
}
