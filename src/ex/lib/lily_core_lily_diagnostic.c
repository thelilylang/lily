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

#ifndef LILY_EX_LIB_LILY_CORE_LILY_DIAGNOSTIC_C
#define LILY_EX_LIB_LILY_CORE_LILY_DIAGNOSTIC_C

#include <core/lily/diagnostic/error.h>
#include <core/lily/diagnostic/warning.h>

#include "lily_base.c"
#include "lily_core_shared.c"

// <core/lily/diagnostic/error.h>
extern inline CONSTRUCTOR(LilyError, LilyError, enum LilyErrorKind kind);

extern inline VARIANT_CONSTRUCTOR(LilyError,
                                  LilyError,
                                  unexpected_token,
                                  char *unexpected_token);

// <core/lily/diagnostic/warning.h>
extern inline CONSTRUCTOR(LilyWarning, LilyWarning, enum LilyWarningKind kind);

#endif // LILY_EX_LIB_LILY_CORE_LILY_DIAGNOSTIC_C