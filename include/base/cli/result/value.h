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

#ifndef LILY_BASE_CLI_RESULT_VALUE_H
#define LILY_BASE_CLI_RESULT_VALUE_H

#include <base/vec.h>

enum CliResultValueKind
{
    CLI_RESULT_VALUE_KIND_SINGLE,
    CLI_RESULT_VALUE_KIND_MULTIPLE,
    CLI_RESULT_VALUE_KIND_MULTIPLE_INF
};

typedef struct CliResultValue
{
    enum CliResultValueKind kind;
    const char *name;
    union
    {
        char *single;      // char* (&)
        Vec *multiple;     // Vec<char* (&)>*
        Vec *multiple_inf; // Vec<char* (&)>*
    };
} CliResultValue;

/**
 *
 * @brief Construct CliResultValue type (CLI_RESULT_VALUE_KIND_SINGLE).
 */
VARIANT_CONSTRUCTOR(CliResultValue *,
                    CliResultValue,
                    single,
                    const char *name,
                    char *single);

/**
 *
 * @brief Construct CliResultValue type (CLI_RESULT_VALUE_KIND_MULTIPLE).
 */
VARIANT_CONSTRUCTOR(CliResultValue *,
                    CliResultValue,
                    multiple,
                    const char *name,
                    Vec *multiple);

/**
 *
 * @brief Construct CliResultValue type (CLI_RESULT_VALUE_KIND_MULTIPLE_INF).
 */
VARIANT_CONSTRUCTOR(CliResultValue *,
                    CliResultValue,
                    multiple_inf,
                    const char *name,
                    Vec *multiple_inf);

/**
 *
 * @brief Free CliResultValue type.
 */
DESTRUCTOR(CliResultValue, CliResultValue *self);

#endif // LILY_BASE_CLI_RESULT_VALUE_H
