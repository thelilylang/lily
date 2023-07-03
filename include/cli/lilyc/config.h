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

#ifndef LILY_CLI_LILYC_CONFIG_H
#define LILY_CLI_LILYC_CONFIG_H

#include <base/macros.h>

#include <stdbool.h>

typedef struct LilycConfig
{
    const char *filename;
    const char *target; // const char*?
    bool run_scanner;
    bool run_parser;
    bool run_typecheck;
    bool run_ir;
    bool dump_scanner;
    bool dump_parser;
    bool dump_typecheck;
    bool dump_mir;
    bool dump_ir;
    bool cc_ir;
    bool cpp_ir;
    bool js_ir;
    bool llvm_ir;
    bool wasm_ir;
} LilycConfig;

/**
 *
 * @brief Construct LilycConfig type.
 */
inline CONSTRUCTOR(LilycConfig,
                   LilycConfig,
                   const char *filename,
                   const char *target,
                   bool run_scanner,
                   bool run_parser,
                   bool run_typecheck,
                   bool run_ir,
                   bool dump_scanner,
                   bool dump_parser,
                   bool dump_typecheck,
                   bool dump_mir,
                   bool dump_ir,
                   bool cc_ir,
                   bool cpp_ir,
                   bool js_ir,
                   bool llvm_ir,
                   bool wasm_ir)
{
    return (LilycConfig){ .filename = filename,
                          .target = target,
                          .run_scanner = run_scanner,
                          .run_parser = run_parser,
                          .run_typecheck = run_typecheck,
                          .run_ir = run_ir,
                          .dump_scanner = dump_scanner,
                          .dump_parser = dump_parser,
                          .dump_typecheck = dump_typecheck,
                          .dump_mir = dump_mir,
                          .dump_ir = dump_ir,
                          .cc_ir = cc_ir,
                          .cpp_ir = cpp_ir,
                          .js_ir = js_ir,
                          .llvm_ir = llvm_ir,
                          .wasm_ir = wasm_ir };
}

#endif // LILY_CLI_LILYC_CONFIG_H
