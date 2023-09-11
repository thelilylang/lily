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

#ifndef LILY_CORE_LILY_COMPILER_PACKAGE_CONFIG
#define LILY_CORE_LILY_COMPILER_PACKAGE_CONFIG

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/platform.h>
#include <base/str.h>

#include <cli/emit.h>
#include <cli/lilyc/config.h>

#include <core/shared/target/arch.h>
#include <core/shared/target/os.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct LilyPackageConfig
{
    const char *output; // const char*?
    bool build;
    bool dump_scanner;
    bool dump_parser;
    bool dump_tc; // Dump typecheck
    bool dump_ir; // Dump IR (Intermediate Representation)
    bool run_scanner;
    bool run_parser;
    bool run_tc; // Run typecheck
    bool run_ir; // Run IR (Intermediate Representation)
    bool cc_ir;  // Use CC IR instead of LLVM IR.
    bool cpp_ir; // Use CPP IR instead of LLVM IR.
    bool js_ir;  // Use JS IR instead of LLVM IR.
    bool llvm_ir;
    bool wasm_ir; // Use WASM IR instead of LLVM IR.
    enum Arch arch_target;
    enum Os os_target;
    bool o0;
    bool o1;
    bool o2;
    bool o3;
    bool oz;
    bool verbose;
} LilyPackageConfig;

/**
 *
 * @brief Construct LilyPackageConfig type.
 */
CONSTRUCTOR(LilyPackageConfig,
            LilyPackageConfig,
            const char *target,
            const char *output,
            bool build,
            bool dump_scanner,
            bool dump_parser,
            bool dump_tc,
            bool dump_ir,
            bool run_scanner,
            bool run_parser,
            bool run_tc,
            bool run_ir,
            bool cc_ir,
            bool cpp_ir,
            bool js_ir,
            bool llvm_ir,
            bool wasm_ir,
            bool o0,
            bool o1,
            bool o2,
            bool o3,
            bool oz,
            bool verbose);

/**
 *
 * @brief Construct LilyPackageConfig type from LilycConfig type.
 */
inline LilyPackageConfig
from_CompileConfig__LilyPackageConfig(const LilycConfig *lilyc_config)
{
    return NEW(LilyPackageConfig,
               lilyc_config->target,
               lilyc_config->output,
               lilyc_config->build,
               lilyc_config->dump_scanner,
               lilyc_config->dump_parser,
               lilyc_config->dump_typecheck,
               lilyc_config->dump_ir,
               lilyc_config->run_scanner,
               lilyc_config->run_parser,
               lilyc_config->run_typecheck,
               lilyc_config->run_ir,
               lilyc_config->cc_ir,
               lilyc_config->cpp_ir,
               lilyc_config->js_ir,
               lilyc_config->llvm_ir,
               lilyc_config->wasm_ir,
               lilyc_config->o0,
               lilyc_config->o1,
               lilyc_config->o2,
               lilyc_config->o3,
               lilyc_config->oz,
               lilyc_config->verbose);
}

#endif // LILY_CORE_LILY_COMPILER_PACKAGE_CONFIG
