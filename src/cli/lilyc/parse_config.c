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

LilycConfig
run__LilycParseConfig(const Vec *results)
{
    char *filename = NULL;
    bool dump_scanner = false, dump_parser = false, dump_typecheck = false,
         dump_mir = false, dump_ir = false;
    bool run_scanner = false, run_parser = false, run_typecheck = false,
         run_ir = false;
    bool cc_ir = false, cpp_ir = false, js_ir = false, llvm_ir = false,
         wasm_ir = false;
    bool o0 = false, o1 = false, o2 = false, o3 = false, oz = false;
    const char *target = NULL;
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
                    case 4:
                        dump_scanner = true;
                        break;
                    case 5:
                        dump_parser = true;
                        break;
                    case 6:
                        dump_typecheck = true;
                        break;
                    case 7:
                        dump_mir = true;
                        break;
                    case 8:
                        dump_ir = true;
                        break;
                    case 9:
                        run_scanner = true;
                        break;
                    case 10:
                        run_parser = true;
                        break;
                    case 11:
                        run_typecheck = true;
                        break;
                    case 12:
                        run_ir = true;
                        break;
                    case 13:
                        llvm_ir = true;
                        break;
                    case 14:
                        cc_ir = true;
                        break;
                    case 15:
                        cpp_ir = true;
                        break;
                    case 16:
                        js_ir = true;
                        break;
                    case 17:
                        wasm_ir = true;
                        break;
                    case 18:
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        target = current->option->value->single;

                        break;
                    case 19:
                    case 22:
                        o0 = true;
                        break;
                    case 20:
                    case 25:
                        o3 = true;
                        break;
                    case 21:
                    case 26:
                        oz = true;
                        break;
                    case 23:
                        o1 = true;
                        break;
                    case 24:
                        o2 = true;
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
               run_scanner,
               run_parser,
               run_typecheck,
               run_ir,
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
               oz);
}
