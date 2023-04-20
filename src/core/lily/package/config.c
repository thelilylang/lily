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

#include <core/lily/package/config.h>

CONSTRUCTOR(LilyPackageConfig,
            LilyPackageConfig,
            const char *target,
            bool dump_scanner,
            bool dump_parser,
            bool dump_tc,
            bool dump_ir,
            bool run_scanner,
            bool run_parser,
            bool run_tc,
            bool run_ir,
            bool cc_ir,
            bool cpp_ir,
            bool js_ir,
            bool llvm_ir,
            bool wasm_ir)
{
    enum Os os = -1;
    enum Arch arch = -1;

    if (target) {
        Vec *target_split = split__Str(target, '-');

        if (target_split->len == 2) {
            os = from_str__Os(get__Vec(target_split, 0));
            arch = from_str__Arch(get__Vec(target_split, 1));

            // Clean up
            lily_free(target_split->buffer[0]);
            lily_free(target_split->buffer[1]);
            FREE(Vec, target_split);
        } else {
            EMIT_ERROR("expected `--target=<os>-<arch>`");

            // Clean up
            for (Usize i = 0; i < target_split->len; ++i) {
                lily_free(target_split->buffer[i]);
            }

            FREE(Vec, target_split);

            exit(1);
        }
    }

    return (LilyPackageConfig){ .dump_scanner = dump_scanner,
                                .dump_parser = dump_parser,
                                .dump_tc = dump_tc,
                                .dump_ir = dump_ir,
                                .run_scanner = run_scanner,
                                .run_parser = run_parser,
                                .run_tc = run_tc,
                                .run_ir = run_ir,
                                .cc_ir = cc_ir,
                                .cpp_ir = cpp_ir,
                                .js_ir = js_ir,
                                .llvm_ir = llvm_ir,
                                .wasm_ir = wasm_ir,
                                .arch_target = arch,
                                .os_target = os };
}
