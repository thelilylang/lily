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

#ifndef LILY_CLI_OPTION_RUN_H
#define LILY_CLI_OPTION_RUN_H

#include <base/macros.h>
#include <base/vec.h>

enum RunOptionKind
{
    RUN_OPTION_KIND_ERROR,
    RUN_OPTION_KIND_FILENAME,
    RUN_OPTION_KIND_HELP // -h, --help
};

typedef struct RunOption
{
  enum RunOptionKind kind;
  union {
    const char *error;
    const char *filename;
  };
} RunOption;

/**
 *
 * @brief Construct RunOption type.
 */
CONSTRUCTOR(RunOption *, RunOption, enum RunOptionKind kind);

/**
 *
 * @brief Construct CompileOption type (RUN_OPTION_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(RunOption *, RunOption, error, const char *error);

/**
 *
 * @brief Construct RunOption type (RUN_OPTION_KIND_FILENAME).
 */
VARIANT_CONSTRUCTOR(RunOption *, RunOption, filename, const char *filename);

/**
 *
 * @brief Convert each option in RunOption struct.
 * @return Return a Vec<RunOption*>*.
 */
Vec *
parse__RunOption(const char **options, const Usize options_size);

/**
 *
 * @brief Free RunOption type.
 */
DESTRUCTOR(RunOption, RunOption *self);

#endif // LILY_CLI_OPTION_RUN_H
