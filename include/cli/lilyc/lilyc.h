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

// Identifiers of the `lilyc` options. They are attached to each option at
// construction and come back untouched in the parse results, so this enum can
// be reordered freely, and options can gain or lose a short name, without
// affecting how the results are matched.
enum LilycOptionId
{
    LILYC_OPTION_ID_BUILD,
    LILYC_OPTION_ID_DUMP_SCANNER,
    LILYC_OPTION_ID_DUMP_PREPARSER,
    LILYC_OPTION_ID_DUMP_PRECOMPILER,
    LILYC_OPTION_ID_DUMP_PARSER,
    LILYC_OPTION_ID_DUMP_ANALYSIS,
    LILYC_OPTION_ID_DUMP_MIR,
    LILYC_OPTION_ID_DUMP_IR,
    LILYC_OPTION_ID_RUN_SCANNER,
    LILYC_OPTION_ID_RUN_PREPARSER,
    LILYC_OPTION_ID_RUN_PRECOMPILER,
    LILYC_OPTION_ID_RUN_PARSER,
    LILYC_OPTION_ID_RUN_ANALYSIS,
    LILYC_OPTION_ID_RUN_MIR,
    LILYC_OPTION_ID_RUN_IR,
    LILYC_OPTION_ID_LIB,
    LILYC_OPTION_ID_STATIC,
    LILYC_OPTION_ID_DYNAMIC,
    LILYC_OPTION_ID_LLVM_IR,
    LILYC_OPTION_ID_CC_IR,
    LILYC_OPTION_ID_CPP_IR,
    LILYC_OPTION_ID_JS_IR,
    LILYC_OPTION_ID_WASM_IR,
    LILYC_OPTION_ID_TARGET,
    LILYC_OPTION_ID_ODEBUG,
    LILYC_OPTION_ID_ORELEASE,
    LILYC_OPTION_ID_OSIZE,
    LILYC_OPTION_ID_O0,
    LILYC_OPTION_ID_O1,
    LILYC_OPTION_ID_O2,
    LILYC_OPTION_ID_O3,
    LILYC_OPTION_ID_OZ,
    LILYC_OPTION_ID_OUTPUT,
    LILYC_OPTION_ID_VERBOSE,
    LILYC_OPTION_ID_RUN
};

#define LILYC_OPTIONS(self, add_option)                                        \
    CliOption *build = NEW(CliOption, LILYC_OPTION_ID_BUILD, "--build");       \
    CliOption *dump_scanner =                                                  \
      NEW(CliOption, LILYC_OPTION_ID_DUMP_SCANNER, "--dump-scanner");          \
    CliOption *dump_preparser =                                                \
      NEW(CliOption, LILYC_OPTION_ID_DUMP_PREPARSER, "--dump-preparser");      \
    CliOption *dump_precompiler =                                              \
      NEW(CliOption, LILYC_OPTION_ID_DUMP_PRECOMPILER, "--dump-precompiler");  \
    CliOption *dump_parser =                                                   \
      NEW(CliOption, LILYC_OPTION_ID_DUMP_PARSER, "--dump-parser");            \
    CliOption *dump_analysis =                                                 \
      NEW(CliOption, LILYC_OPTION_ID_DUMP_ANALYSIS, "--dump-analysis");        \
    CliOption *dump_mir =                                                      \
      NEW(CliOption, LILYC_OPTION_ID_DUMP_MIR, "--dump-mir");                  \
    CliOption *dump_ir = NEW(CliOption, LILYC_OPTION_ID_DUMP_IR, "--dump-ir"); \
    CliOption *run_scanner =                                                   \
      NEW(CliOption, LILYC_OPTION_ID_RUN_SCANNER, "--run-scanner");            \
    CliOption *run_preparser =                                                 \
      NEW(CliOption, LILYC_OPTION_ID_RUN_PREPARSER, "--run-preparser");        \
    CliOption *run_precompiler =                                               \
      NEW(CliOption, LILYC_OPTION_ID_RUN_PRECOMPILER, "--run-precompiler");    \
    CliOption *run_parser =                                                    \
      NEW(CliOption, LILYC_OPTION_ID_RUN_PARSER, "--run-parser");              \
    CliOption *run_analysis =                                                  \
      NEW(CliOption, LILYC_OPTION_ID_RUN_ANALYSIS, "--run-analysis");          \
    CliOption *run_mir = NEW(CliOption, LILYC_OPTION_ID_RUN_MIR, "--run-mir"); \
    CliOption *run_ir = NEW(CliOption, LILYC_OPTION_ID_RUN_IR, "--run-ir");    \
    CliOption *lib = NEW(CliOption, LILYC_OPTION_ID_LIB, "--lib");             \
    CliOption *static_ = NEW(CliOption, LILYC_OPTION_ID_STATIC, "--static");   \
    CliOption *dynamic = NEW(CliOption, LILYC_OPTION_ID_DYNAMIC, "--dynamic"); \
    CliOption *llvm_ir = NEW(CliOption, LILYC_OPTION_ID_LLVM_IR, "--llvm-ir"); \
    CliOption *cc_ir = NEW(CliOption, LILYC_OPTION_ID_CC_IR, "--cc-ir");       \
    CliOption *cpp_ir = NEW(CliOption, LILYC_OPTION_ID_CPP_IR, "--cpp-ir");    \
    CliOption *js_ir = NEW(CliOption, LILYC_OPTION_ID_JS_IR, "--js-ir");       \
    CliOption *wasm_ir = NEW(CliOption, LILYC_OPTION_ID_WASM_IR, "--wasm-ir"); \
    CliOption *target = NEW(CliOption, LILYC_OPTION_ID_TARGET, "--target");    \
    CliOption *ODebug = NEW(CliOption, LILYC_OPTION_ID_ODEBUG, "-ODebug");     \
    CliOption *ORelease =                                                      \
      NEW(CliOption, LILYC_OPTION_ID_ORELEASE, "-ORelease");                   \
    CliOption *OSize = NEW(CliOption, LILYC_OPTION_ID_OSIZE, "-OSize");        \
    CliOption *O0 = NEW(CliOption, LILYC_OPTION_ID_O0, "-O0");                 \
    CliOption *O1 = NEW(CliOption, LILYC_OPTION_ID_O1, "-O1");                 \
    CliOption *O2 = NEW(CliOption, LILYC_OPTION_ID_O2, "-O2");                 \
    CliOption *O3 = NEW(CliOption, LILYC_OPTION_ID_O3, "-O3");                 \
    CliOption *Oz = NEW(CliOption, LILYC_OPTION_ID_OZ, "-Oz");                 \
    CliOption *output = NEW(CliOption, LILYC_OPTION_ID_OUTPUT, "--output");    \
    CliOption *verbose = NEW(CliOption, LILYC_OPTION_ID_VERBOSE, "--verbose"); \
    CliOption *run = NEW(CliOption, LILYC_OPTION_ID_RUN, "--run");             \
                                                                               \
    short_name__CliOption(                                                     \
      help__CliOption(build, "Build a package (exe, lib, ...)"), "-b");        \
    help__CliOption(dump_scanner, "Dump scanner output");                      \
    help__CliOption(dump_preparser, "Dump preparser output");                  \
    help__CliOption(dump_precompiler, "Dump precompiler output");              \
    help__CliOption(dump_parser, "Dump parser output");                        \
    help__CliOption(dump_analysis, "Dump analysis output");                    \
    help__CliOption(dump_mir, "Dump MIR output");                              \
    help__CliOption(dump_ir, "Dump IR output");                                \
    help__CliOption(run_scanner, "Run until the scanner");                     \
    help__CliOption(run_preparser, "Run until the preparser");                 \
    help__CliOption(run_precompiler, "Run until the precompiler");             \
    help__CliOption(run_parser, "Run until the parser");                       \
    help__CliOption(run_analysis, "Run until the analysis");                   \
    help__CliOption(run_mir, "Run until the MIR");                             \
    help__CliOption(run_ir, "Run until the IR");                               \
    short_name__CliOption(                                                     \
      help__CliOption(lib, "Compile a library (static library by default)"),   \
      "-l");                                                                   \
    help__CliOption(static_, "Force to compile a static library");             \
    help__CliOption(dynamic, "Force to compile a dynamic library");            \
    help__CliOption(llvm_ir, "Run LLVM as IR (by default)");                   \
    help__CliOption(cc_ir, "Use C as IR");                                     \
    help__CliOption(cpp_ir, "Use C++ as IR");                                  \
    help__CliOption(js_ir, "Use JS as IR");                                    \
    help__CliOption(wasm_ir, "Use WASM as IR");                                \
    value__CliOption(                                                          \
      help__CliOption(target,                                                  \
                      "Specify the target of the compilation (<TARGET>: "      \
                      "<arch>-<os>-<abi>)"),                                   \
      NEW(CliValue, CLI_VALUE_KIND_SINGLE, "TARGET", true));                   \
    help__CliOption(ODebug, "Debug optimization");                             \
    help__CliOption(ORelease, "Release optimization");                         \
    help__CliOption(OSize, "Size optimization");                               \
    help__CliOption(O0, "Level 0 of optimization (Debug)");                    \
    help__CliOption(O1, "Level 1 of optimization");                            \
    help__CliOption(O2, "Level 2 of optimization");                            \
    help__CliOption(O3, "Level 3 of optimization (Release)");                  \
    help__CliOption(Oz, "Size optimization");                                  \
    value__CliOption(                                                          \
      short_name__CliOption(                                                   \
        help__CliOption(output, "Write output to <FILENAME>"), "-o"),          \
      NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILENAME", true));                 \
    help__CliOption(verbose, "Enable log step of the compiler");               \
    help__CliOption(short_name__CliOption(run, "-r"),                          \
                    "Run the compiled file");                                  \
                                                                               \
    add_option(self, build);                                                   \
    add_option(self, dump_scanner);                                            \
    add_option(self, dump_preparser);                                          \
    add_option(self, dump_precompiler);                                        \
    add_option(self, dump_parser);                                             \
    add_option(self, dump_analysis);                                           \
    add_option(self, dump_mir);                                                \
    add_option(self, dump_ir);                                                 \
    add_option(self, run_scanner);                                             \
    add_option(self, run_preparser);                                           \
    add_option(self, run_precompiler);                                         \
    add_option(self, run_parser);                                              \
    add_option(self, run_analysis);                                            \
    add_option(self, run_mir);                                                 \
    add_option(self, run_ir);                                                  \
    add_option(self, lib);                                                     \
    add_option(self, static_);                                                 \
    add_option(self, dynamic);                                                 \
    add_option(self, llvm_ir);                                                 \
    add_option(self, cc_ir);                                                   \
    add_option(self, cpp_ir);                                                  \
    add_option(self, js_ir);                                                   \
    add_option(self, wasm_ir);                                                 \
    add_option(self, target);                                                  \
    add_option(self, ODebug);                                                  \
    add_option(self, ORelease);                                                \
    add_option(self, OSize);                                                   \
    add_option(self, O0);                                                      \
    add_option(self, O1);                                                      \
    add_option(self, O2);                                                      \
    add_option(self, O3);                                                      \
    add_option(self, Oz);                                                      \
    add_option(self, output);                                                  \
    add_option(self, verbose);                                                 \
    add_option(self, run);

Cli
build__CliLilyc(Vec *args);

#endif // LILY_CLI_LILYC_H
