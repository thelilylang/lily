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

#include <base/hash/sip.h>

#include <core/lily/compiler/ir/llvm/compile.h>
#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/optimize.h>
#include <core/lily/compiler/output/cache.h>

#include <llvm-c/Core.h>

void
compile__LilyCompilerIrLlvm(const LilyPackage *package)
{
#ifdef PLATFORM_64
#ifdef LILY_WINDOWS_OS
    char *path = format("{s}{zu}.obj",
                        DIR_CACHE_OBJ,
                        hash_sip(package->global_name->buffer,
                                 package->global_name->len,
                                 0x0123456789abcdefULL,
                                 0xfedcba9876543210ULL));
#else
    char *path = format("{s}{zu}.o",
                        DIR_CACHE_OBJ,
                        hash_sip(package->global_name->buffer,
                                 package->global_name->len,
                                 0x0123456789abcdefULL,
                                 0xfedcba9876543210ULL));

#endif
#else
#ifdef LILY_WINDOWS_OS
    char *path = format("{s}{zu}.obj",
                        DIR_CACHE_OBJ,
                        hash_sip(package->global_name->buffer,
                                 package->global_name->len,
                                 0x01234567,
                                 0x89abcdef));
#else
    char *path = format("{s}{zu}.o",
                        DIR_CACHE_OBJ,
                        hash_sip(package->global_name->buffer,
                                 package->global_name->len,
                                 0x01234567,
                                 0x89abcdef));
#endif
#endif

    // TODO: Set optimization level (is_debug).
    LilyLLVMOptimize(&package->ir.llvm, LILY_OPT_LEVEL_DEBUG);

#ifdef ENV_DEBUG
    printf("====Optimized LLVM IR(%s)====\n", package->global_name->buffer);

    dump__LilyIrLlvm(&package->ir.llvm);
#endif

    //    char *error_msg = NULL;

    //    if (LLVMTargetMachineEmitToFile(package->ir.llvm.machine,
    //                                package->ir.llvm.module,
    //                                path,
    //                                LLVMObjectFile,
    //                                &error_msg) != 0) {
    //		EMIT_ERROR(error_msg);
    //        LLVMDisposeMessage(error_msg);
    //        exit(1);
    //	}

    lily_free(path);
}