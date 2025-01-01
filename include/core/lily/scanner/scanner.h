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

#ifndef LILY_CORE_LILY_SCANNER_H
#define LILY_CORE_LILY_SCANNER_H

#include <base/format.h>
#include <base/new.h>
#include <base/vec.h>

#include <core/lily/scanner/token.h>
#include <core/shared/diagnostic.h>
#include <core/shared/scanner.h>

typedef struct LilyScanner
{
    Vec *tokens; // Vec<LilyToken*>*
    Scanner base;
} LilyScanner;

/**
 *
 * @brief Construct LilyScanner type.
 */
inline CONSTRUCTOR(LilyScanner, LilyScanner, Source source, Usize *count_error)
{
    return (LilyScanner){ .tokens = NEW(Vec),
                          .base = NEW(Scanner, source, count_error) };
}

/**
 *
 * @brief Run the scanner.
 */
void
run__LilyScanner(LilyScanner *self, bool dump_scanner);

/**
 *
 * @brief Free LilyScanner type.
 */
DESTRUCTOR(LilyScanner, const LilyScanner *self);

#endif // LILY_CORE_LILY_SCANNER_H
