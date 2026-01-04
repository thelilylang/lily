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

#ifndef LILY_BASE_CLI_RESULT_H
#define LILY_BASE_CLI_RESULT_H

#include <base/cli/result/command.h>
#include <base/cli/result/option.h>

enum CliResultKind
{
    CLI_RESULT_KIND_COMMAND,
    CLI_RESULT_KIND_OPTION,
    CLI_RESULT_KIND_VALUE
};

typedef struct CliResult
{
    enum CliResultKind kind;
    union
    {
        CliResultCommand command;
        CliResultOption *option;
        CliResultValue *value;
    };
} CliResult;

/**
 *
 * @brief Construct CliResult type (CLI_RESULT_KIND_COMMAND).
 */
VARIANT_CONSTRUCTOR(CliResult *, CliResult, command, CliResultCommand command);

/**
 *
 * @brief Construct CliResult type (CLI_RESULT_KIND_OPTION).
 */
VARIANT_CONSTRUCTOR(CliResult *, CliResult, option, CliResultOption *option);

/**
 *
 * @brief Construct CliResult type (CLI_RESULT_KIND_VALUE).
 */
VARIANT_CONSTRUCTOR(CliResult *, CliResult, value, CliResultValue *value);

/**
 *
 * @brief Free CliResult type.
 */
DESTRUCTOR(CliResult, CliResult *self);

#endif // LILY_BASE_CLI_RESULT_H
