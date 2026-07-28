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

#include <base/assert.h>
#include <base/cli/result.h>

#include <cli/emit.h>
#include <cli/lilyc/lilyc.h>
#include <cli/lilyc/parse_config.h>

#include <stdio.h>
#include <stdlib.h>

LilycConfig
run__LilycParseConfig(const Vec *results)
{
    char *filename = NULL;
    bool build = false;
    bool dump_scanner = false, dump_preparser = false, dump_precompiler = false,
         dump_parser = false, dump_analysis = false, dump_mir = false,
         dump_ir = false;
    bool run_scanner = false, run_preparser = false, run_precompiler = false,
         run_parser = false, run_analysis = false, run_mir = false,
         run_ir = false;
    bool lib = false, static_ = false, dynamic = false;
    bool cc_ir = false, cpp_ir = false, js_ir = false, llvm_ir = false,
         wasm_ir = false;
    bool o0 = false, o1 = false, o2 = false, o3 = false, oz = false;
    bool verbose = false;
    bool run = false;
    const char *target = NULL;
    const char *output = NULL;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_VALUE:
                ASSERT(current->value);
                ASSERT(current->value->kind == CLI_RESULT_VALUE_KIND_SINGLE);

                filename = current->value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    case LILYC_OPTION_ID_BUILD:
                        build = true;
                        break;
                    case LILYC_OPTION_ID_DUMP_SCANNER:
                        dump_scanner = true;
                        break;
                    case LILYC_OPTION_ID_DUMP_PREPARSER:
                        dump_preparser = true;
                        break;
                    case LILYC_OPTION_ID_DUMP_PRECOMPILER:
                        dump_precompiler = true;
                        break;
                    case LILYC_OPTION_ID_DUMP_PARSER:
                        dump_parser = true;
                        break;
                    case LILYC_OPTION_ID_DUMP_ANALYSIS:
                        dump_analysis = true;
                        break;
                    case LILYC_OPTION_ID_DUMP_MIR:
                        dump_mir = true;
                        break;
                    case LILYC_OPTION_ID_DUMP_IR:
                        dump_ir = true;
                        break;
                    case LILYC_OPTION_ID_RUN_SCANNER:
                        run_scanner = true;
                        break;
                    case LILYC_OPTION_ID_RUN_PREPARSER:
                        run_preparser = true;
                        break;
                    case LILYC_OPTION_ID_RUN_PRECOMPILER:
                        run_precompiler = true;
                        break;
                    case LILYC_OPTION_ID_RUN_PARSER:
                        run_parser = true;
                        break;
                    case LILYC_OPTION_ID_RUN_ANALYSIS:
                        run_analysis = true;
                        break;
                    case LILYC_OPTION_ID_RUN_MIR:
                        run_mir = true;
                        break;
                    case LILYC_OPTION_ID_RUN_IR:
                        run_ir = true;
                        break;
                    case LILYC_OPTION_ID_LIB:
                        lib = true;
                        break;
                    case LILYC_OPTION_ID_STATIC:
                        static_ = true;
                        break;
                    case LILYC_OPTION_ID_DYNAMIC:
                        dynamic = true;
                        break;
                    case LILYC_OPTION_ID_LLVM_IR:
                        llvm_ir = true;
                        break;
                    case LILYC_OPTION_ID_CC_IR:
                        cc_ir = true;
                        break;
                    case LILYC_OPTION_ID_CPP_IR:
                        cpp_ir = true;
                        break;
                    case LILYC_OPTION_ID_JS_IR:
                        js_ir = true;
                        break;
                    case LILYC_OPTION_ID_WASM_IR:
                        wasm_ir = true;
                        break;
                    case LILYC_OPTION_ID_TARGET:
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        target = current->option->value->single;

                        break;
                    case LILYC_OPTION_ID_ODEBUG:
                    case LILYC_OPTION_ID_O0:
                        o0 = true;
                        break;
                    case LILYC_OPTION_ID_ORELEASE:
                    case LILYC_OPTION_ID_O3:
                        o3 = true;
                        break;
                    case LILYC_OPTION_ID_OSIZE:
                    case LILYC_OPTION_ID_OZ:
                        oz = true;
                        break;
                    case LILYC_OPTION_ID_O1:
                        o1 = true;
                        break;
                    case LILYC_OPTION_ID_O2:
                        o2 = true;
                        break;
                    case LILYC_OPTION_ID_OUTPUT:
                        output = current->option->value->single;
                        break;
                    case LILYC_OPTION_ID_VERBOSE:
                        verbose = true;
                        break;
                    case LILYC_OPTION_ID_RUN:
                        run = true;
                        break;
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    // Check if some options are incompatible.

    if (run && lib) {
        EMIT_ERROR(
          "you cannot use `-r` or `--run` option with `-l` or `--lib` option");
        exit(1);
    }

    return NEW(LilycConfig,
               filename,
               target,
               output,
               build,
               run_scanner,
               run_preparser,
               run_precompiler,
               run_parser,
               run_analysis,
               run_mir,
               run_ir,
               lib,
               static_,
               dynamic,
               dump_scanner,
               dump_preparser,
               dump_precompiler,
               dump_parser,
               dump_analysis,
               dump_mir,
               dump_ir,
               cc_ir,
               cpp_ir,
               js_ir,
               llvm_ir,
               wasm_ir,
               o0,
               o1,
               o2,
               o3,
               oz,
               verbose,
               run);
}
