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

#ifndef LILY_BASE_CLI_VALUE_H
#define LILY_BASE_CLI_VALUE_H

#include <base/vec.h>

enum CliValueKind
{
    CLI_VALUE_KIND_SINGLE,
    CLI_VALUE_KIND_MULTIPLE,
    CLI_VALUE_KIND_NONE
};

typedef struct CliValue
{
    enum CliValueKind kind;
    union
    {
        char *single;
        Vec *multiple; // Vec<char*>*
    };
} CliValue;

/**
 *
 * @brief Construct CliValue type (CLI_VALUE_KIND_SINGLE).
 */
inline VARIANT_CONSTRUCTOR(CliValue, CliValue, single, char *single)
{
    return (CliValue){ .kind = CLI_VALUE_KIND_SINGLE, .single = single };
}

/**
 *
 * @brief Construct CliValue type (CLI_VALUE_KIND_MULTIPLE).
 */
inline VARIANT_CONSTRUCTOR(CliValue, CliValue, multiple, Vec *multiple)
{
    return (CliValue){ .kind = CLI_VALUE_KIND_MULTIPLE, .multiple = multiple };
}

/**
 *
 * @brief Construct CliValue type (CLI_VALUE_KIND_NONE).
 */
inline VARIANT_CONSTRUCTOR(CliValue, CliValue, none)
{
    return (CliValue){ .kind = CLI_VALUE_KIND_NONE };
}

/**
 *
 * @brief Free CliValue type.
 */
DESTRUCTOR(CliValue, const CliValue *self);

#endif // LILY_BASE_CLI_VALUE_H
