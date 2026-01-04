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

#ifndef LILY_CORE_LILY_COMPILER_PACKAGE_INTERPRETER_CONFIG_H
#define LILY_CORE_LILY_COMPILER_PACKAGE_INTERPRETER_CONFIG_H

#include <base/macros.h>
#include <base/new.h>

#include <cli/lily/config.h>

typedef struct LilyPackageInterpreterConfig
{
    Vec *args;
    bool verbose;
    Usize max_heap;
    Usize max_stack;
} LilyPackageInterpreterConfig;

/**
 *
 * @brief Construct LilyPackageInterpreterConfig type.
 */
inline CONSTRUCTOR(LilyPackageInterpreterConfig,
                   LilyPackageInterpreterConfig,
                   Vec *args,
                   bool verbose,
                   Usize max_heap,
                   Usize max_stack)
{
    return (LilyPackageInterpreterConfig){ .args = args,
                                           .verbose = verbose,
                                           .max_heap = max_heap,
                                           .max_stack = max_stack };
}

/**
 *
 * @brief Construct LilyPackageInterpreterConfig type with default value.
 */
inline LilyPackageInterpreterConfig
default__LilyPackageInterpreterConfig()
{
    return (LilyPackageInterpreterConfig){
        .args = NULL, .verbose = false, .max_heap = 0, .max_stack = 0
    };
}

/**
 *
 * @brief Construct LilyPackageInterpreterConfig type from LilyConfig type (only
 * LILY_CONFIG_KIND_RUN variant is accepted).
 */
LilyPackageInterpreterConfig
from_RunConfig__LilyPackageInterpreterConfig(const LilyConfig *lily_config);

#endif // LILY_CORE_LILY_COMPILER_PACKAGE_INTERPRETER_CONFIG_H
