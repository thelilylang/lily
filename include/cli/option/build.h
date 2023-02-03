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

#ifndef LILY_CLI_OPTION_BUILD_H
#define LILY_CLI_OPTION_BUILD_H

#include <base/macros.h>
#include <base/vec.h>

enum BuildOptionKind
{
    BUILD_OPTION_KIND_ERROR,
    BUILD_OPTION_KIND_HELP,   // -h, --help
    BUILD_OPTION_KIND_VERBOSE // -v
};

typedef struct BuildOption
{
  enum BuildOptionKind kind;
  union {
    const char *error;
  };
} BuildOption;

/**
 *
 * @brief Construct BuildOption type.
 */
CONSTRUCTOR(BuildOption *, BuildOption, enum BuildOptionKind kind);

/**
 *
 * @brief Construct BuildOption type (BUILD_OPTION_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(BuildOption *, BuildOption, error, const char *error);

/**
 *
 * @brief Convert each option in BuildOption struct.
 * @return Return a Vec<BuildOption*>*.
 */
Vec *
parse__BuildOption(const char **options, const Usize options_size);

/**
 *
 * @brief Free BuildOption type.
 */
DESTRUCTOR(BuildOption, BuildOption *self);

#endif // LILY_CLI_OPTION_BUILD_H
