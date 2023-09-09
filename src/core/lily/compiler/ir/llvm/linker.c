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
#include <core/lily/compiler/driver/lld.h>
#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/linker.h>
#include <core/lily/compiler/ir/llvm/utils.h>
#include <core/lily/compiler/output/cache.h>
#include <core/lily/compiler/package.h>

#ifdef ENV_LOCAL
#define LIB_DIR_BUILD "build"
#define LIB_DIR_BUILD_DEBUG "build/Debug"
#endif

// Link all library dependencies
static void
link_lib_dependencies__LilyIrLlvmLinker(LilyPackage *self, Vec *args);

// Link all package dependencies
static void
link_package_dependencies__LilyIrLlvmLinker(LilyPackage *self, Vec *args);

void
link_lib_dependencies__LilyIrLlvmLinker(LilyPackage *self, Vec *args)
{
    for (Usize i = 0; i < self->lib_dependencies->len; ++i) {
        LilyLibrary *lib = get__Vec(self->lib_dependencies, i);

        ASSERT(lib->output_path);

        // Link direct library dependencies.
#ifdef LILY_WINDOWS_OS
        char *arg = format("/defaultlib:{s}", lib->output_path);
#else
        char *arg = strdup(lib->output_path);
#endif

        if (is_unique_arg__LilyCompilerIrLlvmUtils(args, arg)) {
            push__Vec(args, arg);
        } else {
            lily_free(arg);
        }

        // Link indirect library dependencies.
        link_lib_dependencies__LilyIrLlvmLinker(lib->package, args);
    }

    // Link indirect library dependencies.
    for (Usize i = 0; i < self->package_dependencies->len; ++i) {
        link_lib_dependencies__LilyIrLlvmLinker(
          get__Vec(self->package_dependencies, i), args);
    }
}

void
link_package_dependencies__LilyIrLlvmLinker(LilyPackage *self, Vec *args)
{
    for (Usize i = 0; i < self->package_dependencies->len; ++i) {
        LilyPackage *package_dependency =
          get__Vec(self->package_dependencies, i);

        ASSERT(package_dependency->output_path);

        // Link direct package dependencies.
        char *arg = strdup(package_dependency->output_path);

        if (is_unique_arg__LilyCompilerIrLlvmUtils(args, arg)) {
            push__Vec(args, arg);
        } else {
            lily_free(arg);
        }

        // Link indirect package dependencies.
        link_package_dependencies__LilyIrLlvmLinker(package_dependency, args);
    }
}

void
compile_exe__LilyIrLlvmLinker(LilyPackage *self)
{
    Vec *args = NEW(Vec); // Vec<char*>*
    String *output_name = self->config->output
                            ? from__String((char *)self->config->output)
                            : get_filename__File(self->file.name);

    // Default library link.
    // Link @sys and @builtin library.
    if (self->sys_is_loaded) {
#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
        push__Vec(args, strdup("-llily_sys"));
#elifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/defaultlib:lily_sys.lib"));
#else
#error "unknown OS"
#endif
    }

    if (self->builtin_is_loaded) {
#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
        push__Vec(args, strdup("-llily_builtin"));
#elifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/defaultlib:lily_builtin.lib"));
#else
#error "unknown OS"
#endif
    }

    // Link all lib dependencies
    {
        Vec *lib_dependencies = NEW(Vec); // Vec<char*>*

        link_lib_dependencies__LilyIrLlvmLinker(self, lib_dependencies);
        append__Vec(args, lib_dependencies);

        FREE(Vec, lib_dependencies);
    }

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
    push__Vec(args, strdup("--build-id=sha1"));
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
    // Link crt1, crti, crtn and libc
    push__Vec(args, strdup("/usr/lib/crt1.o"));
    push__Vec(args, strdup("/usr/lib/crti.o"));
    push__Vec(args, strdup("/usr/lib/crtn.o"));
    push__Vec(args, strdup("/usr/lib/libc.so"));

    // Add dynamic linker option
    push__Vec(args, strdup("-dynamic-linker"));
    push__Vec(args, strdup("/lib64/ld-linux-x86-64.so.2"));
#elifdef LILY_APPLE_OS
    // Link libc
    push__Vec(args, strdup("/usr/lib/libc.so"));

    // Add dynamic linker option
    push__Vec(args, strdup("-dynamic-linker"));
    push__Vec(args, strdup("/usr/lib/dylib"));
#elifdef LILY_WINDOWS_OS
    push__Vec(args, strdup("/subsystem:console"));
    push__Vec(args, strdup("/defaultlib:libc.lib"));
    push__Vec(args, strdup("/dynamicbase"));
    push__Vec(args, strdup("/nxcompat"));
#else
#error "unknown OS"
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
    push__Vec(args, strdup("-pie"));
#endif

#ifdef ENV_LOCAL
    if (self->builtin_is_loaded || self->sys_is_loaded) {
#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
        push__Vec(args, strdup("-L" LIB_DIR_BUILD));
        push__Vec(args, strdup("-L" LIB_DIR_BUILD_DEBUG));
#elifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/libpath:" LIB_DIR_BUILD));
        push__Vec(args, strdup("/libpath:" LIB_DIR_BUILD_DEBUG));
#else
#error "unknown OS"
#endif
    }
#endif

    // Link all package dependencies
    {
        Vec *package_dependencies = NEW(Vec); // Vec<char*>*

        link_package_dependencies__LilyIrLlvmLinker(self, package_dependencies);
        append__Vec(args, package_dependencies);

        FREE(Vec, package_dependencies);
    }

    push__Vec(args, strdup(self->output_path));

    // Add output option
    // TODO: Check there is passed `-o` option
#ifdef LILY_WINDOWS_OS
    push__Vec(args, format("/out:{s}{Sr}", BIN_DIR_PATH, output_name));
#else
    push__Vec(args, strdup("-o"));
    push__Vec(args, format("{s}{Sr}", DIR_CACHE_BIN, output_name));
#endif

    // Add optimization options
    if (self->config->o3) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:3"));
#else
        push__Vec(args, strdup("-O3"));
        push__Vec(args, strdup("--lto-O3"));
#endif
    } else if (self->config->o2) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:2"));
#else
        push__Vec(args, strdup("-O2"));
        push__Vec(args, strdup("--lto-O2"));
#endif
    } else if (self->config->o1) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:1"));
#else
        push__Vec(args, strdup("-O1"));
        push__Vec(args, strdup("--lto-O1"));
#endif
    } else if (self->config->o0) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:0"));
#else
        push__Vec(args, strdup("-O0"));
        push__Vec(args, strdup("--lto-O0"));
#endif
    }

    if (self->config->oz) {
#ifdef LILY_WINDOWS_OS
        TODO("optimize size");
#else
        push__Vec(args, strdup("-Oz"));
#endif
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

    for (Usize i = 0; i < args->len; ++i) {
        printf("%s", (char *)get__Vec(args, i));

        if (i + 1 != args->len) {
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

    if (!LilyLLDLink(obj_format, (const char **)args->buffer, args->len)) {
        EMIT_ERROR("link error");
        exit(1);
    }

    // Clean up

    for (Usize i = 0; i < args->len; ++i) {
        lily_free(get__Vec(args, i));
    }

    FREE(Vec, args);
}
