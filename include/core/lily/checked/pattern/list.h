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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, PATTERNESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_CORE_LiLY_CHECKED_PATTERN_LIST_H
#define LILY_CORE_LiLY_CHECKED_PATTERN_LIST_H

#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct LilyCheckedPatternList
{
    Vec *patterns; // Vec<LilyCheckedPattern*>*
} LilyCheckedPatternList;

/**
 *
 * @brief Construct LilyCheckedPatternList type.
 */
inline CONSTRUCTOR(LilyCheckedPatternList, LilyCheckedPatternList, Vec *patterns)
{
    return (LilyCheckedPatternList){ .patterns = patterns };
}

/**
 *
 * @brief Convert LilyCheckedPatternList in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedPatternList, const LilyCheckedPatternList *self);
#endif

/**
 *
 * @brief Convert LilyCheckedPatternList in String.
 */
String *
to_string__LilyCheckedPatternList(const LilyCheckedPatternList *self);

/**
 *
 * @brief Free LilyCheckedPatternList type.
 */
DESTRUCTOR(LilyCheckedPatternList, LilyCheckedPatternList *self);

#endif // LILY_CORE_LiLY_CHECKED_PATTERN_ARRAY_H
