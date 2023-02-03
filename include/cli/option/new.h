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

#ifndef LILY_CLI_OPTION_NEW_H
#define LILY_CLI_OPTION_NEW_H

#include <base/macros.h>
#include <base/vec.h>

enum NewOptionKind
{
    NEW_OPTION_KIND_ERROR,
    NEW_OPTION_KIND_NAME,
    NEW_OPTION_KIND_HELP // -h, --help
};

typedef struct NewOption
{
  enum NewOptionKind kind;
  union {
    const char *error;
    const char *name;
  };
} NewOption;

/**
 *
 * @brief Construct NewOption type.
 */
CONSTRUCTOR(NewOption *, NewOption, enum NewOptionKind kind);

/**
 *
 * @brief Construct CompileOption type (INIT_OPTION_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(NewOption *, NewOption, error, const char *error);

/**
 *
 * @brief Construct NewOption type (INIT_OPTION_KIND_NAME).
 */
VARIANT_CONSTRUCTOR(NewOption *, NewOption, name, const char *name);

/**
 *
 * @brief Convert each option in NewOption struct.
 * @return Return a Vec<NewOption*>*.
 */
Vec *
parse__NewOption(const char **options, const Usize options_size);

/**
 *
 * @brief Free NewOption type.
 */
DESTRUCTOR(NewOption, NewOption *self);

#endif // LILY_CLI_OPTION_NEW_H
