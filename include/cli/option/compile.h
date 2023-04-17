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

#ifndef LILY_CLI_OPTION_COMPILE_H
#define LILY_CLI_OPTION_COMPILE_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

enum CompileOptionKind
{
    COMPILE_OPTION_KIND_CC_IR,          // --cc-ir
    COMPILE_OPTION_KIND_CPP_IR,         // --cpp-ir
    COMPILE_OPTION_KIND_DUMP_IR,        // --dump-ir
    COMPILE_OPTION_KIND_DUMP_PARSER,    // --dump-parser
    COMPILE_OPTION_KIND_DUMP_SCANNER,   // --dump-scanner
    COMPILE_OPTION_KIND_DUMP_TYPECHECK, // --dump-tc
    COMPILE_OPTION_KIND_ERROR,
    COMPILE_OPTION_KIND_FILENAME,
    COMPILE_OPTION_KIND_HELP,          // -h, --help
    COMPILE_OPTION_KIND_JS_IR,         // --js-ir
    COMPILE_OPTION_KIND_LLVM_IR,       // --llvm-ir
    COMPILE_OPTION_KIND_RUN_IR,        // --run-ir
    COMPILE_OPTION_KIND_RUN_PARSER,    // --run-parser
    COMPILE_OPTION_KIND_RUN_SCANNER,   // --run-scanner
    COMPILE_OPTION_KIND_RUN_TYPECHECK, // --run-tc
    COMPILE_OPTION_KIND_TARGET,        // --target=<os>-<arch>
    COMPILE_OPTION_KIND_WASM_IR,       // --wasm-ir
};

typedef struct CompileOption
{
    enum CompileOptionKind kind;
    union
    {
        const char *error;
        const char *filename;
        String *target;
    };
} CompileOption;

/**
 *
 * @brief Construct CompileOption type.
 */
CONSTRUCTOR(CompileOption *, CompileOption, enum CompileOptionKind kind);

/**
 *
 * @brief Construct CompileOption type (COMPILE_OPTION_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(CompileOption *, CompileOption, error, const char *error);

/**
 *
 * @brief Construct CompileOption type (COMPILE_OPTION_KIND_FILENAME).
 */
VARIANT_CONSTRUCTOR(CompileOption *,
                    CompileOption,
                    filename,
                    const char *filename);

/**
 *
 * @brief Construct CompileOption type (COMPILE_OPTION_KIND_TARGET).
 */
VARIANT_CONSTRUCTOR(CompileOption *, CompileOption, target, String *target);

/**
 *
 * @brief Convert each option in CompileOption struct.
 * @return Return a Vec<CompileOption*>*.
 */
Vec *
parse__CompileOption(const char **options, const Usize options_size);

/**
 *
 * @brief Free CompileOption type.
 */
DESTRUCTOR(CompileOption, CompileOption *self);

#endif // LILY_CLI_OPTION_COMPILE_H
