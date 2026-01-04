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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_TABLE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_TABLE_H

#include <base/macros.h>
#include <base/types.h>
#include <base/vec.h>

typedef struct LilyCheckedPattern LilyCheckedPattern;

typedef struct LilyCheckedPatternTableItem
{
    Usize id;
    LilyCheckedPattern *value;
} LilyCheckedPatternTableItem;

/**
 *
 * @brief Construct LilyCheckedPatternTableItem type.
 */
CONSTRUCTOR(LilyCheckedPatternTableItem *,
            LilyCheckedPatternTableItem,
            Usize id,
            LilyCheckedPattern *value);

/**
 *
 * @brief Convert LilyCheckedPatternTableItem in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternTableItem,
               const LilyCheckedPatternTableItem *self);
#endif

/**
 *
 * @brief Free LilyCheckedPatternTableItem type.
 */
DESTRUCTOR(LilyCheckedPatternTableItem, LilyCheckedPatternTableItem *self);

typedef Vec * /* Vec<LilyCheckedPatternTableItem*>* */ LilyCheckedPatternTable;

/**
 *
 * @brief Add item to the table.
 */
inline void
add__LilyCheckedPatternTable(LilyCheckedPatternTable self,
                             LilyCheckedPatternTableItem *item)
{
    push__Vec(self, item);
}

/**
 *
 * @brief Get item from id.
 */
LilyCheckedPatternTableItem *
get__LilyCheckedPatternTableItem(LilyCheckedPatternTable self, Usize id);

#define FREE_PATTERN_TABLE(self)                                             \
    FREE_BUFFER_ITEMS(self->buffer, self->len, LilyCheckedPatternTableItem); \
    FREE(Vec, self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_TABLE_H
