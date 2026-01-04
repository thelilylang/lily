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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_LIST_TAIL_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_LIST_TAIL_H

#include <base/macros.h>
#include <base/string.h>

typedef struct LilyCheckedPattern LilyCheckedPattern;

typedef struct LilyCheckedPatternListTail
{
    LilyCheckedPattern *left;  // LilyCheckedPattern*?
    LilyCheckedPattern *right; // LilyCheckedPattern*?
} LilyCheckedPatternListTail;

/**
 *
 * @brief Construct LilyCheckedPatternListTail type.
 */
inline CONSTRUCTOR(LilyCheckedPatternListTail,
                   LilyCheckedPatternListTail,
                   LilyCheckedPattern *left,
                   LilyCheckedPattern *right)
{
    return (LilyCheckedPatternListTail){ .left = left, .right = right };
}

/**
 *
 * @brief Check if the both pattern list tail are equal.
 */
bool
eq__LilyCheckedPatternListTail(const LilyCheckedPatternListTail *self,
                               const LilyCheckedPatternListTail *other);

/**
 *
 * @brief Convert LilyCheckedPatternListTail in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternListTail,
               const LilyCheckedPatternListTail *self);
#endif

/**
 *
 * @brief Free LilyCheckedPatternListTail type.
 */
DESTRUCTOR(LilyCheckedPatternListTail, const LilyCheckedPatternListTail *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_LIST_TAIL_H
