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

#include <base/hash/sip.h>
#include <base/platform.h>

#include <core/lily/compiler/ir/llvm/compile.h>
#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/emit.h>
#include <core/lily/compiler/ir/llvm/optimize.h>
#include <core/lily/compiler/output/cache.h>
#include <core/lily/package/package.h>

#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>

#ifdef LILY_WINDOWS_OS
#define OBJ_EXT ".obj"
#else
#define OBJ_EXT ".o"
#endif

void
compile__LilyCompilerIrLlvm(LilyPackage *package)
{
    ASSERT(package->kind == LILY_PACKAGE_KIND_COMPILER);

#ifdef PLATFORM_64
    char *path = format("{s}{S}{zu}{zu}{s}",
                        DIR_CACHE_OBJ,
                        package->name,
                        hash_sip(package->global_name->buffer,
                                 package->global_name->len,
                                 0x0123456789abcdefULL,
                                 0xfedcba9876543210ULL),
                        hash_sip(package->file.name,
                                 strlen(package->file.name),
                                 0x0123456789abcdefULL,
                                 0xfedcba9876543210ULL),
                        OBJ_EXT);
#else
    char *path = format("{s}{S}{zu}{zu}{s}",
                        DIR_CACHE_OBJ,
                        package->name,
                        hash_sip(package->global_name->buffer,
                                 package->global_name->len,
                                 0x01234567,
                                 0x89abcdef),
                        hash_sip(package->file.name strlen(package->file.name),
                                 0x01234567,
                                 0x89abcdef),
                        OBJ_EXT);
#endif

    char *error_msg = NULL;
    enum LilyOptLevel lily_opt_level = LILY_OPT_LEVEL_O0;

    // TODO: -Oz

    if (package->compiler.config->o0) {
        lily_opt_level = LILY_OPT_LEVEL_O0;
    }

    if (package->compiler.config->o1) {
        lily_opt_level = LILY_OPT_LEVEL_O1;
    }

    if (package->compiler.config->o2) {
        lily_opt_level = LILY_OPT_LEVEL_O2;
    }

    if (package->compiler.config->o3) {
        lily_opt_level = LILY_OPT_LEVEL_O3;
    }

    // Verify LLVM module
    {
        char *error = NULL;

        if (LLVMVerifyModule(package->compiler.ir.llvm.module,
                             LLVMReturnStatusAction,
                             &error)) {
            EMIT_ERROR(error);
            LLVMDisposeMessage(error);
            exit(1);
        }

        LLVMDisposeMessage(error);
    }

    if (LilyLLVMOptimize(
          &package->compiler.ir.llvm, lily_opt_level, &error_msg, path)) {
        EMIT_ERROR(error_msg);
        LLVMDisposeMessage(error_msg);
        exit(1);
    }

    if (LilyLLVMEmit(&package->compiler.ir.llvm,
                     &error_msg,
                     path,
                     true,
                     false,
                     false,
                     false)) {
        EMIT_ERROR(error_msg);
        LLVMDisposeMessage(error_msg);
        exit(1);
    }

#ifdef ENV_DEBUG
    printf("====Optimized LLVM IR(%s)====\n", package->global_name->buffer);

    dump__LilyIrLlvm(&package->compiler.ir.llvm);
#endif

    package->compiler.output_path = path;
}
