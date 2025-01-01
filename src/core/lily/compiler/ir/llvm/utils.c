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

#include <core/lily/compiler/ir/llvm/utils.h>
#include <core/lily/package/package.h>

#include <stdio.h>
#include <string.h>

bool
is_unique_arg__LilyCompilerIrLlvmUtils(Vec *args, char *arg)
{
    for (Usize i = 0; i < args->len; ++i) {
        if (!strcmp(get__Vec(args, i), arg)) {
            return false;
        }
    }

    return true;
}

void
print_cmd_args__LilyCompilerIrLlvmUtils(const char *cmd, Vec *args)
{
    printf("%s ", cmd);

    for (Usize i = 0; i < args->len; ++i) {
        printf("%s", (char *)get__Vec(args, i));

        if (i + 1 != args->len) {
            printf(" ");
        }
    }

    puts("");
}

void
add_lib_dependencies__LilyCompilerIrLlvmUtils(LilyPackage *self, Vec *args)
{
    for (Usize i = 0; i < self->lib_dependencies->len; ++i) {
        LilyLibrary *lib = get__Vec(self->lib_dependencies, i);

        // NOTE: In this case we not wait for thread finish, because the thread
        // is finish before the begin of this current thread.
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
        add_lib_dependencies__LilyCompilerIrLlvmUtils(lib->package, args);
    }

    // Link indirect library dependencies.
    for (Usize i = 0; i < self->package_dependencies->len; ++i) {
        add_lib_dependencies__LilyCompilerIrLlvmUtils(
          get__Vec(self->package_dependencies, i), args);
    }
}

void
add_object_files__LilyCompilerIrLlvmUtils(LilyPackage *self, Vec *args)
{
    ASSERT(self->kind == LILY_PACKAGE_KIND_COMPILER);

    for (Usize i = 0; i < self->sub_packages->len; ++i) {
        LilyPackage *sub_package = get__Vec(self->sub_packages, i);

        // NOTE: In this case we not wait for thread finish, because the thread
        // is finish before the begin of this current thread.
        ASSERT(sub_package->kind == LILY_PACKAGE_KIND_COMPILER);
        ASSERT(sub_package->compiler.output_path);

        char *arg = strdup(sub_package->compiler.output_path);

        ASSERT(is_unique_arg__LilyCompilerIrLlvmUtils(args, arg));
        push__Vec(args, arg);

        add_object_files__LilyCompilerIrLlvmUtils(sub_package, args);
    }
}
