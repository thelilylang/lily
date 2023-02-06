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

#ifndef LILY_CORE_LILY_SCANNER_H
#define LILY_CORE_LILY_SCANNER_H

#include <base/format.h>
#include <base/new.h>
#include <base/vec.h>

#include <core/lily/token.h>
#include <core/shared/diagnostic.h>
#include <core/shared/source.h>

typedef struct Scanner
{
    Vec *tokens; // Vec<LilyToken*>*
    Location location;
    Source source;
    Usize count_error;
} Scanner;

/**
 *
 * @brief Construct Scanner type.
 */
inline CONSTRUCTOR(Scanner, Scanner, Source source)
{
    return (Scanner){ .tokens = NEW(Vec),
                      .location = default__Location(source.file->name),
                      .source = source,
                      .count_error = 0 };
}

/**
 *
 * @brief Run the scanner.
 */
void
run__Scanner(Scanner *self, bool dump_scanner);

/**
 *
 * @brief Free Scanner type.
 */
DESTRUCTOR(Scanner, const Scanner *self);

#endif // LILY_CORE_LILY_SCANNER_H