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

#ifndef LILY_EX_LIB_LILYC_CLI_C
#define LILY_EX_LIB_LILYC_CLI_C

#include <cli/lilyc/config.h>

// #include "lily_base.c"

// <cli/lilyc/config.h>
extern inline CONSTRUCTOR(LilycConfig,
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
                          bool run);

extern inline DESTRUCTOR(LilycConfig, const LilycConfig *self);

#endif // LILY_EX_LIB_LILYC_CLI_C
