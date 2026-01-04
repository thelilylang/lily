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

#ifndef LILY_EX_LIB_LILY_CORE_CC_CI_DIAGNOSTIC_C
#define LILY_EX_LIB_LILY_CORE_CC_CI_DIAGNOSTIC_C

#include <core/cc/ci/diagnostic/error.h>
#include <core/cc/ci/diagnostic/warning.h>

#include "lily_base.c"
#include "lily_core_shared.c"

// <core/cc/ci/diagnostic/error.h>
extern inline CONSTRUCTOR(CIError, CIError, enum CIErrorKind kind);

extern inline VARIANT_CONSTRUCTOR(CIError,
                                  CIError,
                                  preprocessor_error,
                                  char *preprocessor_error);

// <core/cc/ci/diagnostic/warning.h>
extern inline CONSTRUCTOR(CIWarning, CIWarning, enum CIWarningKind kind);

extern inline VARIANT_CONSTRUCTOR(CIWarning,
                                  CIWarning,
                                  preprocessor_warning,
                                  char *preprocessor_warning);

#endif // LILY_EX_LIB_LILY_CORE_CC_CI_DIAGNOSTIC_C
