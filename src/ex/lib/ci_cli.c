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

#ifndef LILY_EX_LIB_CI_CLI_C
#define LILY_EX_LIB_CI_CLI_C

#include <cli/ci/config.h>
#include <cli/ci/config/compile.h>
#include <cli/ci/config/self_test.h>

// <cli/ci/config.h>
extern inline VARIANT_CONSTRUCTOR(CIConfig,
                                  CIConfig,
                                  compile,
                                  CIConfigCompile compile);

extern inline VARIANT_CONSTRUCTOR(CIConfig,
                                  CIConfig,
                                  self_test,
                                  CIConfigSelfTest self_test);

// <cli/ci/config/compile.h>
extern inline CONSTRUCTOR(CIConfigCompile,
                          CIConfigCompile,
                          const char *path,
                          enum CIcConfigMode mode,
                          bool file,
                          enum CIStandard standard,
                          Vec *includes,
                          Vec *includes0,
                          bool no_state_check);

extern inline DESTRUCTOR(CIConfigCompile, const CIConfigCompile *self);

// <cli/ci/config/self_test.h>
extern inline CONSTRUCTOR(CIConfigSelfTest, CIConfigSelfTest, const char *path);

#endif // LILY_EX_LIB_CI_CLI_C
