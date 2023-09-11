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

#include <base/assert.h>
#include <base/cli/result.h>

#include <cli/lilyc/parse_config.h>

#include <stdio.h>
#include <stdlib.h>

#define B_OPTION 4
#define BUILD_OPTION 5
#define DUMP_SCANNER_OPTION 6
#define DUMP_PARSER_OPTION 7
#define DUMP_TYPECHECK_OPTION 8
#define DUMP_MIR_OPTION 9
#define DUMP_IR_OPTION 10
#define RUN_SCANNER_OPTION 11
#define RUN_PARSER_OPTION 12
#define RUN_TYPECHECK_OPTION 13
#define RUN_IR_OPTION 14
#define L_OPTION 15
#define LIB_OPTION 16
#define STATIC_OPTION 17
#define DYNAMIC_OPTION 18
#define LLVM_IR_OPTION 19
#define CC_IR_OPTION 20
#define CPP_IR_OPTION 21
#define JS_IR_OPTION 22
#define WASM_IR_OPTION 23
#define TARGET_OPTION 24
#define ODEBUG_OPTION 25
#define ORELEASE_OPTION 26
#define OSIZE_OPTION 27
#define O0_OPTION 28
#define O1_OPTION 29
#define O2_OPTION 30
#define O3_OPTION 31
#define OZ_OPTION 32
#define O_OPTION 33
#define OUTPUT_OPTION 34
#define V_OPTION 35
#define VERBOSE_OPTION 36

LilycConfig
run__LilycParseConfig(const Vec *results)
{
    char *filename = NULL;
    bool build = false;
    bool dump_scanner = false, dump_parser = false, dump_typecheck = false,
         dump_mir = false, dump_ir = false;
    bool run_scanner = false, run_parser = false, run_typecheck = false,
         run_ir = false;
    bool lib = false, static_ = false, dynamic = false;
    bool cc_ir = false, cpp_ir = false, js_ir = false, llvm_ir = false,
         wasm_ir = false;
    bool o0 = false, o1 = false, o2 = false, o3 = false, oz = false;
    bool verbose = false;
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
                    // 0, 1, 2 and 3 id are used by help and version option
                    case B_OPTION:
                    case BUILD_OPTION:
                        build = true;
                        break;
                    case DUMP_SCANNER_OPTION:
                        dump_scanner = true;
                        break;
                    case DUMP_PARSER_OPTION:
                        dump_parser = true;
                        break;
                    case DUMP_TYPECHECK_OPTION:
                        dump_typecheck = true;
                        break;
                    case DUMP_MIR_OPTION:
                        dump_mir = true;
                        break;
                    case DUMP_IR_OPTION:
                        dump_ir = true;
                        break;
                    case RUN_SCANNER_OPTION:
                        run_scanner = true;
                        break;
                    case RUN_PARSER_OPTION:
                        run_parser = true;
                        break;
                    case RUN_TYPECHECK_OPTION:
                        run_typecheck = true;
                        break;
                    case RUN_IR_OPTION:
                        run_ir = true;
                        break;
                    case L_OPTION:
                    case LIB_OPTION:
                        lib = true;
                        break;
                    case STATIC_OPTION:
                        static_ = true;
                        break;
                    case DYNAMIC_OPTION:
                        dynamic = true;
                        break;
                    case LLVM_IR_OPTION:
                        llvm_ir = true;
                        break;
                    case CC_IR_OPTION:
                        cc_ir = true;
                        break;
                    case CPP_IR_OPTION:
                        cpp_ir = true;
                        break;
                    case JS_IR_OPTION:
                        js_ir = true;
                        break;
                    case WASM_IR_OPTION:
                        wasm_ir = true;
                        break;
                    case TARGET_OPTION:
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        target = current->option->value->single;

                        break;
                    case ODEBUG_OPTION:
                    case O0_OPTION:
                        o0 = true;
                        break;
                    case ORELEASE_OPTION:
                    case O3_OPTION:
                        o3 = true;
                        break;
                    case OSIZE_OPTION:
                    case OZ_OPTION:
                        oz = true;
                        break;
                    case O1_OPTION:
                        o1 = true;
                        break;
                    case O2_OPTION:
                        o2 = true;
                        break;
                    case O_OPTION:
                    case OUTPUT_OPTION:
                        output = current->option->value->single;
                        break;
                    case V_OPTION:
                    case VERBOSE_OPTION:
                        verbose = true;
                        break;
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW(LilycConfig,
               filename,
               target,
               output,
               build,
               run_scanner,
               run_parser,
               run_typecheck,
               run_ir,
               lib,
               static_,
               dynamic,
               dump_scanner,
               dump_parser,
               dump_typecheck,
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
               verbose);
}
