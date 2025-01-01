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

#include <base/file.h>
#include <base/macros.h>

#include <cli/emit.h>
#include <core/lily/compiler/driver/lld.h>
#include <core/lily/compiler/ir/llvm/crt.h>
#include <core/lily/compiler/ir/llvm/dl.h>
#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/linker.h>
#include <core/lily/compiler/ir/llvm/utils.h>
#include <core/lily/compiler/output/cache.h>
#include <core/lily/package/package.h>

#ifdef ENV_LOCAL
#define LIB_DIR_BUILD "build"
#define LIB_DIR_BUILD_DEBUG "build/Debug"
#endif

void
compile_exe__LilyIrLlvmLinker(LilyPackage *self)
{
    ASSERT(self->kind == LILY_PACKAGE_KIND_COMPILER);

    Vec *args = NEW(Vec); // Vec<char*>*
    String *output_name =
      self->compiler.config->output
        ? from__String((char *)self->compiler.config->output)
        : get_filename__File(self->file.name);

    // Link all lib dependencies
    {
        Vec *lib_dependencies = NEW(Vec); // Vec<char*>*

        add_lib_dependencies__LilyCompilerIrLlvmUtils(self, lib_dependencies);
        append__Vec(args, lib_dependencies);

        FREE(Vec, lib_dependencies);
    }

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
    push__Vec(args, strdup("--build-id=sha1"));
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
    // Link crt1, crti, crtn and libc
    {
        const char *crt0 = get_crt0_library_path__LilyIrLlvmLinker();
        const char *crt1 = get_crt1_library_path__LilyIrLlvmLinker();

        if (crt0) {
            push__Vec(args, strdup(crt0));
        } else if (crt1) {
            push__Vec(args, strdup(crt1));
        } else {
            FAILED("crt0.o and crt1.o is not found");
        }
    }

    push__Vec(args, strdup(get_crti_library_path__LilyIrLlvmLinker()));
    push__Vec(args, strdup(get_crtn_library_path__LilyIrLlvmLinker()));
    push__Vec(args, strdup(get_library_path__LilyIrLlvmLinker("libc.so.6")));

    // Add dynamic linker option
    push__Vec(args, strdup("-dynamic-linker"));
    push__Vec(args, strdup(DYNAMIC_LINKER));
#elifdef LILY_APPLE_OS
    // Link libc
    push__Vec(args, strdup("-lc"));
#elifdef LILY_WINDOWS_OS
    push__Vec(args, strdup("/subsystem:console"));
    push__Vec(args, strdup("/defaultlib:libc.lib"));
    push__Vec(args, strdup("/dynamicbase"));
    push__Vec(args, strdup("/nxcompat"));
#else
#error "unknown OS"
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

    // Link all sub-packages
    {
        Vec *package_dependencies = NEW(Vec); // Vec<char*>*

        add_object_files__LilyCompilerIrLlvmUtils(self, package_dependencies);
        append__Vec(args, package_dependencies);

        FREE(Vec, package_dependencies);
    }

    push__Vec(args, strdup(self->compiler.output_path));

    // Default library link.
    // Link @sys.
    if (self->sys_is_loaded) {
#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
        push__Vec(args, strdup("-llily_sys"));
#elifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/defaultlib:lily_sys.lib"));
#else
#error "unknown OS"
#endif
    }

    // Link @builtin.
    if (self->builtin_is_loaded) {
#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS) || defined(LILY_APPLE_OS)
        push__Vec(args, strdup("-llily_builtin"));
#elifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/defaultlib:lily_builtin.lib"));
#else
#error "unknown OS"
#endif
    }

#ifdef LILY_APPLE_OS
    push__Vec(args, strdup("-e"));
    push__Vec(args, strdup("main"));

    // Add arch
    push__Vec(args, strdup("-arch"));

#ifdef LILY_X86_ARCH
    push__Vec(args, strdup("i386"));
#elifdef LILY_X86_64_ARCH
    push__Vec(args, strdup("x86_64"));
#elifdef LILY_ARM64_ARCH
    push__Vec(args, strdup("arm64"));
#else
#error "unknown arch to MacOS"
#endif

    // Add platform version
    push__Vec(args, strdup("-platform_version"));
    push__Vec(args, strdup("macOS"));
    push__Vec(args, strdup(MIN_MACOS_VERSION));
    push__Vec(args, strdup(MACOS_SDK_VERSION));
#endif

    // Add output option
    // TODO: Check there is passed `-o` option
    self->compiler.output_exe_path =
      format("{s}{Sr}", DIR_CACHE_BIN, output_name);

#ifdef LILY_WINDOWS_OS
    push__Vec(args, format("/out:{s}", self->compiler.output_exe_path));
#else
    push__Vec(args, strdup("-o"));
    push__Vec(args, strdup(self->compiler.output_exe_path));
#endif

    // Add optimization options
    if (self->compiler.config->o3) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:3"));
#else
        push__Vec(args, strdup("-O3"));
        push__Vec(args, strdup("--lto-O3"));
#endif
    } else if (self->compiler.config->o2) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:2"));
#else
        push__Vec(args, strdup("-O2"));
        push__Vec(args, strdup("--lto-O2"));
#endif
    } else if (self->compiler.config->o1) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:1"));
#else
        push__Vec(args, strdup("-O1"));
        push__Vec(args, strdup("--lto-O1"));
#endif
    } else if (self->compiler.config->o0) {
#ifdef LILY_WINDOWS_OS
        push__Vec(args, strdup("/opt:0"));
#else
        push__Vec(args, strdup("-O0"));
        push__Vec(args, strdup("--lto-O0"));
#endif
    }

    if (self->compiler.config->oz) {
#ifdef LILY_WINDOWS_OS
        TODO("optimize size");
#else
        push__Vec(args, strdup("-Oz"));
#endif
    }

#ifdef ENV_DEBUG
    printf("====Link(%s)====\n", self->name->buffer);
    print_cmd_args__LilyCompilerIrLlvmUtils(LINKER_CMD, args);
#endif

    if (!link_exe__LilyLLD(
          OBJ_FORMAT, (const char **)args->buffer, args->len)) {
        EMIT_ERROR("link error");
        exit(1);
    }

    // Clean up

    for (Usize i = 0; i < args->len; ++i) {
        lily_free(get__Vec(args, i));
    }

    FREE(Vec, args);
}
