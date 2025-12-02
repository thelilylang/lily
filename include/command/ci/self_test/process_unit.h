/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_COMMAND_CI_SELF_TEST_PROCESS_UNIT_H
#define LILY_COMMAND_CI_SELF_TEST_PROCESS_UNIT_H

#include <base/fork.h>
#include <base/string.h>

#include <command/ci/self_test/metadata.h>

#include <time.h>

typedef struct CISelfTestProcessUnit
{
    Fork pid;
    String *path; // String* (&)
    int read_out_fd;
    int read_diagnostic_fd;
    int read_compiler_error_fd;
    CISelfTestMetadata metadata;
    clock_t start;
} CISelfTestProcessUnit;

/**
 *
 * @brief Construct CISelfTestProcessUnit type.
 */
inline CONSTRUCTOR(CISelfTestProcessUnit,
                   CISelfTestProcessUnit,
                   Fork pid,
                   String *path,
                   int read_out_fd,
                   int read_diagnostic_fd,
                   int read_compiler_error_fd,
                   CISelfTestMetadata metadata,
                   clock_t start)
{
    return (CISelfTestProcessUnit){ .pid = pid,
                                    .path = path,
                                    .read_out_fd = read_out_fd,
                                    .read_diagnostic_fd = read_diagnostic_fd,
                                    .read_compiler_error_fd =
                                      read_compiler_error_fd,
                                    .metadata = metadata,
                                    .start = start };
}

/**
 *
 * @brief Free CISelfTestProcessUnit type.
 */
DESTRUCTOR(CISelfTestProcessUnit, const CISelfTestProcessUnit *self);

#endif // LILY_COMMAND_CI_SELF_TEST_PROCESS_UNIT_H
