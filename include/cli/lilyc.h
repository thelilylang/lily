/*
 *
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

#ifndef LILY_CLI_LILYC_H
#define LILY_CLI_LILYC_H

#include <base/cli.h>

#define LILYC_OPTIONS(self)                                                   \
    CliOption *dump_scanner = NEW(CliOption, "--dump-scanner");               \
    CliOption *dump_parser = NEW(CliOption, "--dump-parser");                 \
    CliOption *dump_tc = NEW(CliOption, "--dump-tc");                         \
    CliOption *dump_mir = NEW(CliOption, "--dump-mir");                       \
    CliOption *dump_ir = NEW(CliOption, "--dump-ir");                         \
    CliOption *llvm_ir = NEW(CliOption, "--llvm-ir");                         \
    CliOption *cc_ir = NEW(CliOption, "--cc-ir");                             \
    CliOption *cpp_ir = NEW(CliOption, "--cpp-ir");                           \
    CliOption *js_ir = NEW(CliOption, "--js-ir");                             \
    CliOption *wasm_ir = NEW(CliOption, "--wasm-ir");                         \
    CliOption *target = NEW(CliOption, "--target");                           \
                                                                              \
    dump_scanner->$help(dump_scanner, "Dump scanner output");                 \
    dump_parser->$help(dump_parser, "Dump parser output");                    \
    dump_tc->$help(dump_tc, "Dump typecheck output");                         \
    dump_mir->$help(dump_mir, "Dump MIR output");                             \
    dump_ir->$help(dump_ir, "Dump IR output");                                \
    llvm_ir->$help(llvm_ir, "Run LLVM as IR (by default)");                   \
    cc_ir->$help(cc_ir, "Use C as IR");                                       \
    cpp_ir->$help(cpp_ir, "Use C++ as IR");                                   \
    js_ir->$help(js_ir, "Use JS as IR");                                      \
    wasm_ir->$help(wasm_ir, "Use WASM as IR");                                \
    target                                                                    \
      ->$help(target,                                                         \
              "Specify the target of the compilation (<arch>-<os>-<abi>)")    \
      ->$value(target, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "TARGET", true)); \
                                                                              \
    self->$option(self, dump_scanner)                                         \
      ->$option(self, dump_parser)                                            \
      ->$option(self, dump_tc)                                                \
      ->$option(self, dump_mir)                                               \
      ->$option(self, dump_ir)                                                \
      ->$option(self, llvm_ir)                                                \
      ->$option(self, cc_ir)                                                  \
      ->$option(self, cpp_ir)                                                 \
      ->$option(self, js_ir)                                                  \
      ->$option(self, wasm_ir)                                                \
      ->$option(self, target);

Cli
build__CliLilyc(Vec *args);

#endif // LILY_CLI_LILYC_H
