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

#ifndef LILY_EX_LIB_CI_COMMAND_C
#define LILY_EX_LIB_CI_COMMAND_C

#include <command/ci/compile/compile.h>
#include <command/ci/self_test/metadata.h>
#include <command/ci/self_test/process_unit.h>

// <command/ci/compile.h>
extern inline void
run__CICompile(CIConfig *config);

// <command/ci/self_test/metadata.h>
extern inline CONSTRUCTOR(CISelfTestMetadata, CISelfTestMetadata);

// <command/ci/self_test/process_unit.h>
extern inline CONSTRUCTOR(CISelfTestProcessUnit,
                          CISelfTestProcessUnit,
                          Fork pid,
                          String *path,
                          int read_out_fd,
                          int read_diagnostic_fd,
                          int read_compiler_error_fd,
                          CISelfTestMetadata metadata,
                          clock_t start);

#endif // LILY_EX_LIB_CI_COMMAND_C
