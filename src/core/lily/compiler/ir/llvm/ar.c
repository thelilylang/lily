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

#include <base/alloc.h>
#include <base/file.h>

#include <core/lily/compiler/driver/llvm-ar.h>
#include <core/lily/compiler/ir/llvm/ar.h>
#include <core/lily/compiler/package/library.h>
#include <core/lily/compiler/package/package.h>

#include <cli/emit.h>

#include <stdio.h>
#include <string.h>

void
add_object_files__LilyIrLlvmAr(LilyPackage *self, Vec *args);

// TODO: add a system to avoid duplicate object files.
void
add_object_files__LilyIrLlvmAr(LilyPackage *self, Vec *args)
{
    // TODO: add option to add all object files (for unused package case). e.g.
    // `--full-lib`
    for (Usize i = 0; i < self->package_dependencies->len; ++i) {
        LilyPackage *package_dependency =
          get__Vec(self->package_dependencies, i);

        push__Vec(args, strdup(package_dependency->output_path));
        add_object_files__LilyIrLlvmAr(package_dependency, args);
    }
}

void
compile_lib__LilyIrLlvmAr(LilyLibrary *self)
{
    Vec *args = NEW(Vec); // Vec<char*>*

    push__Vec(args, strdup("--format=default"));
    push__Vec(args, strdup("rcs"));
    // TODO: generate shared library
#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
    push__Vec(args, format("lib{S}.a", self->name));
#elifdef LILY_WINDOWS_OS
    push__Vec(args, format("{S}.lib", self->name));
#else
#error "unknown OS"
#endif

    add_object_files__LilyIrLlvmAr(self->package, args);

#ifdef ENV_DEBUG
    printf("====LLVM-Ar(%s)====\n", self->name->buffer);

    printf("llvm-ar");

    for (Usize i = 0; i < args->len; ++i) {
        printf("%s", (char *)get__Vec(args, i));

        if (i + 1 != args->len) {
            printf(" ");
        }
    }

    puts("");
#endif

    if (run__LlvmAr(args->len, (char **)args->buffer)) {
        EMIT_ERROR("ar error");
        exit(1);
    }

    // Clean up

    for (Usize i = 0; i < args->len; ++i) {
        lily_free(get__Vec(args, i));
    }

    FREE(Vec, args);
}
