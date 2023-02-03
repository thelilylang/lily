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

#ifndef LILY_CLI_OPTION_TEST_H
#define LILY_CLI_OPTION_TEST_H

#include <base/macros.h>
#include <base/vec.h>

enum TestOptionKind
{
    TEST_OPTION_KIND_ERROR,
    TEST_OPTION_KIND_FILENAME,
    TEST_OPTION_KIND_HELP // -h, --help
};

typedef struct TestOption
{
  enum TestOptionKind kind;
  union {
    const char *error;
    const char *filename;
  };
} TestOption;

/**
 *
 * @brief Construct TestOption type.
 */
CONSTRUCTOR(TestOption *, TestOption, enum TestOptionKind kind);

/**
 *
 * @brief Construct CompileOption type (TEST_OPTION_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(TestOption *, TestOption, error, const char *error);

/**
 *
 * @brief Construct TestOption type (TEST_OPTION_KIND_FILENAME).
 */
VARIANT_CONSTRUCTOR(TestOption *, TestOption, filename, const char *filename);

/**
 *
 * @brief Convert each option in TestOption struct.
 * @return Return a Vec<TestOption*>*.
 */
Vec *
parse__TestOption(const char **options, const Usize options_size);

/**
 *
 * @brief Free TestOption type.
 */
DESTRUCTOR(TestOption, TestOption *self);

#endif // LILY_CLI_OPTION_TEST_H
