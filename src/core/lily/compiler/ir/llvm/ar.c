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

#include <base/alloc.h>
#include <base/file.h>

#include <core/lily/compiler/driver/lld.h>
#include <core/lily/compiler/driver/llvm-ar.h>
#include <core/lily/compiler/ir/llvm/ar.h>
#include <core/lily/compiler/ir/llvm/utils.h>
#include <core/lily/compiler/output/cache.h>
#include <core/lily/package/library.h>
#include <core/lily/package/package.h>

#include <cli/emit.h>

#include <stdio.h>
#include <string.h>

void
compile_lib__LilyIrLlvmAr(LilyLibrary *self)
{
    ASSERT(self->package->kind == LILY_PACKAGE_KIND_COMPILER);

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
    Vec *args = init__Vec(1, strdup("ar")); // Vec<char*>*
#elif defined(LILY_WINDOWS_OS)
    Vec *args = init__Vec(1, strdup("dlltool")); // Vec<char*>*
#else
#error "unknown OS"
#endif

#ifdef LILY_WINDOWS_OS
    char *static_lib_output_path =
      format("{s}{S}.lib", DIR_CACHE_LIB, self->name);
#else
    char *static_lib_output_path =
      format("{s}lib{S}.a", DIR_CACHE_LIB, self->name);
#endif

    push__Vec(args, strdup("--format=default"));
    push__Vec(args, strdup("rcs"));
    push__Vec(args, strdup(static_lib_output_path));
    push__Vec(args, strdup(self->package->compiler.output_path));

    // Add object files
    {
        Vec *objs = NEW(Vec); // Vec<char*>*

        add_object_files__LilyCompilerIrLlvmUtils(self->package, objs);
        append__Vec(args, objs);

        FREE(Vec, objs);
    }

#ifdef ENV_DEBUG
    printf("====LLVM-Ar(%s)====\n", self->name->buffer);
    print_cmd_args__LilyCompilerIrLlvmUtils("llvm-ar", args);
#endif

    if (run__LlvmAr(args->len, (char **)args->buffer)) {
        EMIT_ERROR("ar error");
        exit(1);
    }

    // We generate a dynamic library
    if (self->package->is_dynamic_lib) {
#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
        Vec *linker_args = init__Vec(1, strdup("ld.lld")); // Vec<char*>*
#elifdef LILY_APPLE_OS
        Vec *linker_args = init__Vec(1, strdup("ld64.lld")); // Vec<char*>*
#elif defined(LILY_WINDOWS_OS)
        Vec *linker_args = init__Vec(1, strdup("lld-link")); // Vec<char*>*
#else
#error "unknown OS"
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
        push__Vec(linker_args, strdup("--shared"));
        push__Vec(linker_args, strdup("--no-pie"));
#elif defined(LILY_APPLE_OS)
        push__Vec(linker_args, strdup("-dylib"));
#elif defined(LILY_WINDOWS_OS)
        push__Vec(linker_args, strdup("/dll"));
#endif

#ifdef LILY_WINDOWS_OS
        char *dynamic_lib_output_path =
          format("{s}{S}.dll", DIR_CACHE_LIB, self->name);
#elif defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
        char *dynamic_lib_output_path =
          format("{s}lib{S}.so", DIR_CACHE_LIB, self->name);
#elifdef LILY_APPLE_OS
        char *dynamic_lib_output_path =
          format("{s}lib{S}.dylib", DIR_CACHE_LIB, self->name);
#else
#error "unknown OS"
#endif

        // NOTE: Do not free `static_lib_output_path`
        push__Vec(linker_args, static_lib_output_path);

        // Link all lib dependencies
        {
            Vec *lib_dependencies = NEW(Vec); // Vec<char*>*

            add_lib_dependencies__LilyCompilerIrLlvmUtils(self->package,
                                                          lib_dependencies);
            append__Vec(linker_args, lib_dependencies);

            FREE(Vec, lib_dependencies);
        }

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
        push__Vec(linker_args, strdup("--build-id=sha1"));
#endif

#ifdef LILY_WINDOWS_OS
        push__Vec(linker_args, format("/out:{s}", dynamic_lib_output_path));
#else
        push__Vec(linker_args, strdup("-o"));
        push__Vec(linker_args, strdup(dynamic_lib_output_path));
#endif

#ifdef ENV_DEBUG
        printf("====Link Dynamic Library(%s)====\n", self->name->buffer);
        print_cmd_args__LilyCompilerIrLlvmUtils(LINKER_CMD, linker_args);
#endif

        if (run__LilyLLD((int)linker_args->len,
                         (const char **)linker_args->buffer)) {
            EMIT_ERROR("LLD error");
            exit(1);
        }

        // Clean up

        for (Usize i = 0; i < linker_args->len; ++i) {
            lily_free(get__Vec(linker_args, i));
        }

        FREE(Vec, linker_args);

        self->output_path = dynamic_lib_output_path;
    } else {
        self->output_path = static_lib_output_path;
    }

    // Clean up

    for (Usize i = 0; i < args->len; ++i) {
        lily_free(get__Vec(args, i));
    }

    FREE(Vec, args);
}
