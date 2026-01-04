/*
 *
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

#ifndef LILY_CLI_LILYC_H
#define LILY_CLI_LILYC_H

#include <base/cli.h>

#define LILYC_OPTIONS(self)                                                    \
    CliOption *build = NEW(CliOption, "--build");                              \
    CliOption *dump_scanner = NEW(CliOption, "--dump-scanner");                \
    CliOption *dump_preparser = NEW(CliOption, "--dump-preparser");            \
    CliOption *dump_precompiler = NEW(CliOption, "--dump-precompiler");        \
    CliOption *dump_parser = NEW(CliOption, "--dump-parser");                  \
    CliOption *dump_analysis = NEW(CliOption, "--dump-analysis");              \
    CliOption *dump_mir = NEW(CliOption, "--dump-mir");                        \
    CliOption *dump_ir = NEW(CliOption, "--dump-ir");                          \
    CliOption *run_scanner = NEW(CliOption, "--run-scanner");                  \
    CliOption *run_preparser = NEW(CliOption, "--run-preparser");              \
    CliOption *run_precompiler = NEW(CliOption, "--run-precompiler");          \
    CliOption *run_parser = NEW(CliOption, "--run-parser");                    \
    CliOption *run_analysis = NEW(CliOption, "--run-analysis");                \
    CliOption *run_mir = NEW(CliOption, "--run-mir");                          \
    CliOption *run_ir = NEW(CliOption, "--run-ir");                            \
    CliOption *lib = NEW(CliOption, "--lib");                                  \
    CliOption *static_ = NEW(CliOption, "--static");                           \
    CliOption *dynamic = NEW(CliOption, "--dynamic");                          \
    CliOption *llvm_ir = NEW(CliOption, "--llvm-ir");                          \
    CliOption *cc_ir = NEW(CliOption, "--cc-ir");                              \
    CliOption *cpp_ir = NEW(CliOption, "--cpp-ir");                            \
    CliOption *js_ir = NEW(CliOption, "--js-ir");                              \
    CliOption *wasm_ir = NEW(CliOption, "--wasm-ir");                          \
    CliOption *target = NEW(CliOption, "--target");                            \
    CliOption *ODebug = NEW(CliOption, "-ODebug");                             \
    CliOption *ORelease = NEW(CliOption, "-ORelease");                         \
    CliOption *OSize = NEW(CliOption, "-OSize");                               \
    CliOption *O0 = NEW(CliOption, "-O0");                                     \
    CliOption *O1 = NEW(CliOption, "-O1");                                     \
    CliOption *O2 = NEW(CliOption, "-O2");                                     \
    CliOption *O3 = NEW(CliOption, "-O3");                                     \
    CliOption *Oz = NEW(CliOption, "-Oz");                                     \
    CliOption *output = NEW(CliOption, "--output");                            \
    CliOption *verbose = NEW(CliOption, "--verbose");                          \
    CliOption *run = NEW(CliOption, "--run");                                  \
                                                                               \
    build->$help(build, "Build a package (exe, lib, ...)")                     \
      ->$short_name(build, "-b");                                              \
    dump_scanner->$help(dump_scanner, "Dump scanner output");                  \
    dump_preparser->$help(dump_preparser, "Dump preparser output");            \
    dump_precompiler->$help(dump_precompiler, "Dump precompiler output");      \
    dump_parser->$help(dump_parser, "Dump parser output");                     \
    dump_analysis->$help(dump_analysis, "Dump analysis output");               \
    dump_mir->$help(dump_mir, "Dump MIR output");                              \
    dump_ir->$help(dump_ir, "Dump IR output");                                 \
    run_scanner->$help(run_scanner, "Run until the scanner");                  \
    run_preparser->$help(run_preparser, "Run until the preparser");            \
    run_precompiler->$help(run_precompiler, "Run until the precompiler");      \
    run_parser->$help(run_parser, "Run until the parser");                     \
    run_analysis->$help(run_analysis, "Run until the analysis");               \
    run_mir->$help(run_mir, "Run until the MIR");                              \
    run_ir->$help(run_ir, "Run until the IR");                                 \
    lib->$help(lib, "Compile a library (static library by default)")           \
      ->$short_name(lib, "-l");                                                \
    static_->$help(static_, "Force to compile a static library");              \
    dynamic->$help(dynamic, "Force to compile a dynamic library");             \
    llvm_ir->$help(llvm_ir, "Run LLVM as IR (by default)");                    \
    cc_ir->$help(cc_ir, "Use C as IR");                                        \
    cpp_ir->$help(cpp_ir, "Use C++ as IR");                                    \
    js_ir->$help(js_ir, "Use JS as IR");                                       \
    wasm_ir->$help(wasm_ir, "Use WASM as IR");                                 \
    target                                                                     \
      ->$help(                                                                 \
        target,                                                                \
        "Specify the target of the compilation (<TARGET>: <arch>-<os>-<abi>)") \
      ->$value(target, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "TARGET", true));  \
    ODebug->$help(ODebug, "Debug optimization");                               \
    ORelease->$help(ORelease, "Release optimization");                         \
    OSize->$help(OSize, "Size optimization");                                  \
    O0->$help(O0, "Level 0 of optimization (Debug)");                          \
    O1->$help(O1, "Level 1 of optimization");                                  \
    O2->$help(O2, "Level 2 of optimization");                                  \
    O3->$help(O3, "Level 3 of optimization (Release)");                        \
    Oz->$help(Oz, "Size optimization");                                        \
    output->$help(output, "Write output to <FILENAME>")                        \
      ->$short_name(output, "-o")                                              \
      ->$value(output,                                                         \
               NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILENAME", true));        \
    verbose->$help(verbose, "Enable log step of the compiler");                \
    run->$short_name(run, "-r")->$help(run, "Run the compiled file");          \
                                                                               \
    self->$option(self, build)                                                 \
      ->$option(self, dump_scanner)                                            \
      ->$option(self, dump_preparser)                                          \
      ->$option(self, dump_precompiler)                                        \
      ->$option(self, dump_parser)                                             \
      ->$option(self, dump_analysis)                                           \
      ->$option(self, dump_mir)                                                \
      ->$option(self, dump_ir)                                                 \
      ->$option(self, run_scanner)                                             \
      ->$option(self, run_preparser)                                           \
      ->$option(self, run_precompiler)                                         \
      ->$option(self, run_parser)                                              \
      ->$option(self, run_analysis)                                            \
      ->$option(self, run_mir)                                                 \
      ->$option(self, run_ir)                                                  \
      ->$option(self, lib)                                                     \
      ->$option(self, static_)                                                 \
      ->$option(self, dynamic)                                                 \
      ->$option(self, llvm_ir)                                                 \
      ->$option(self, cc_ir)                                                   \
      ->$option(self, cpp_ir)                                                  \
      ->$option(self, js_ir)                                                   \
      ->$option(self, wasm_ir)                                                 \
      ->$option(self, target)                                                  \
      ->$option(self, ODebug)                                                  \
      ->$option(self, ORelease)                                                \
      ->$option(self, OSize)                                                   \
      ->$option(self, O0)                                                      \
      ->$option(self, O1)                                                      \
      ->$option(self, O2)                                                      \
      ->$option(self, O3)                                                      \
      ->$option(self, Oz)                                                      \
      ->$option(self, output)                                                  \
      ->$option(self, verbose)                                                 \
      ->$option(self, run);

Cli
build__CliLilyc(Vec *args);

#endif // LILY_CLI_LILYC_H
