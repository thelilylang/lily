/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_CLI_LILYC_CONFIG_H
#define LILY_CLI_LILYC_CONFIG_H

#include <base/macros.h>

typedef struct LilycConfig
{
    const char *filename;
    const char *target; // const char*?
    const char *output; // const char*?
    bool build;
    bool run_scanner;
    bool run_preparser;
    bool run_precompiler;
    bool run_parser;
    bool run_analysis;
    bool run_mir;
    bool run_ir;
    bool lib;
    bool static_;
    bool dynamic;
    bool dump_scanner;
    bool dump_preparser;
    bool dump_precompiler;
    bool dump_parser;
    bool dump_analysis;
    bool dump_mir;
    bool dump_ir;
    bool cc_ir;
    bool cpp_ir;
    bool js_ir;
    bool llvm_ir;
    bool wasm_ir;
    bool o0; // Include -ODebug
    bool o1;
    bool o2;
    bool o3; // Include -ORelease
    bool oz; // Include -OSize
    bool verbose;
    bool run;
} LilycConfig;

/**
 *
 * @brief Construct LilycConfig type.
 */
inline CONSTRUCTOR(LilycConfig,
                   LilycConfig,
                   const char *filename,
                   const char *target,
                   const char *output,
                   bool build,
                   bool run_scanner,
                   bool run_preparser,
                   bool run_precompiler,
                   bool run_parser,
                   bool run_analysis,
                   bool run_mir,
                   bool run_ir,
                   bool lib,
                   bool static_,
                   bool dynamic,
                   bool dump_scanner,
                   bool dump_preparser,
                   bool dump_precompiler,
                   bool dump_parser,
                   bool dump_analysis,
                   bool dump_mir,
                   bool dump_ir,
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
                   bool verbose,
                   bool run)
{
    return (LilycConfig){ .filename = filename,
                          .target = target,
                          .output = output,
                          .build = build,
                          .run_scanner = run_scanner,
                          .run_preparser = run_preparser,
                          .run_precompiler = run_precompiler,
                          .run_parser = run_parser,
                          .run_analysis = run_analysis,
                          .run_mir = run_mir,
                          .run_ir = run_ir,
                          .lib = lib,
                          .static_ = static_,
                          .dynamic = dynamic,
                          .dump_scanner = dump_scanner,
                          .dump_preparser = dump_preparser,
                          .dump_precompiler = dump_precompiler,
                          .dump_parser = dump_parser,
                          .dump_analysis = dump_analysis,
                          .dump_mir = dump_mir,
                          .dump_ir = dump_ir,
                          .cc_ir = cc_ir,
                          .cpp_ir = cpp_ir,
                          .js_ir = js_ir,
                          .llvm_ir = llvm_ir,
                          .wasm_ir = wasm_ir,
                          .o0 = o0,
                          .o1 = o1,
                          .o2 = o2,
                          .o3 = o3,
                          .oz = oz,
                          .verbose = verbose,
                          .run = run };
}

#endif // LILY_CLI_LILYC_CONFIG_H
