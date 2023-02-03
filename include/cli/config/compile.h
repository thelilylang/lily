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

#include <base/macros.h>

#include <stdbool.h>

typedef struct CompileConfig
{
    const char *filename;
    bool run_scanner;
    bool run_parser;
    bool run_typecheck;
    bool run_ir;
    bool dump_scanner;
    bool dump_parser;
    bool dump_typecheck;
    bool dump_ir;
} CompileConfig;

/**
 *
 * @brief Construct CompileConfig type.
 */
inline CONSTRUCTOR(CompileConfig,
                   CompileConfig,
                   const char *filename,
                   bool run_scanner,
                   bool run_parser,
                   bool run_typecheck,
                   bool run_ir,
                   bool dump_scanner,
                   bool dump_parser,
                   bool dump_typecheck,
                   bool dump_ir)
{
    return (CompileConfig){ .filename = filename,
                            .run_scanner = run_scanner,
                            .run_parser = run_parser,
                            .run_typecheck = run_typecheck,
                            .run_ir = run_ir,
                            .dump_scanner = dump_scanner,
                            .dump_parser = dump_parser,
                            .dump_typecheck = dump_typecheck,
                            .dump_ir = dump_ir };
}
