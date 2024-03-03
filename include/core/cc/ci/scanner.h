/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_CORE_CC_CI_SCANNER_H
#define LILY_CORE_CC_CI_SCANNER_H

#include <base/types.h>
#include <base/vec.h>

#include <core/cc/ci/features.h>
#include <core/cc/ci/token.h>
#include <core/shared/scanner.h>

typedef struct CIScanner
{
    Vec *tokens; // Vec<CIToken*>*
    Scanner base;
    enum CIStandard standard;
} CIScanner;

/**
 *
 * @brief Construct CIScanner type.
 */
inline CONSTRUCTOR(CIScanner,
                   CIScanner,
                   Source source,
                   Usize *count_error,
                   enum CIStandard standard)
{
    return (CIScanner){ .tokens = NEW(Vec),
                        .base = NEW(Scanner, source, count_error),
                        .standard = standard };
}

/**
 *
 * @brief Run the scanner.
 */
void
run__CIScanner(CIScanner *self, bool dump_scanner);

/**
 *
 * @brief Free CIScanner type.
 */
DESTRUCTOR(CIScanner, const CIScanner *self);

typedef struct CIScannerContext
{
    Vec *tokens; // Vec<CIToken*>*? (&)
    bool in_macro;
    bool in_prepro_cond;
} CIScannerContext;

/**
 *
 * @brief Construct CIScannerContext type.
 */
inline CONSTRUCTOR(CIScannerContext,
                   CIScannerContext,
                   Vec *tokens,
                   bool in_macro,
                   bool in_prepro_cond)
{
    return (CIScannerContext){ .tokens = tokens,
                               .in_macro = in_macro,
                               .in_prepro_cond = in_prepro_cond };
}

#endif // LILY_CORE_CC_CI_SCANNER_H
