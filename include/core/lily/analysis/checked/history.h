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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_HISTORY_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_HISTORY_H

#include <base/new.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/decl.h>

typedef struct LilyCheckedHistory
{
    Vec *items; // Vec<LilyCheckedDecl* (&)>*
} LilyCheckedHistory;

/**
 *
 * @brief Construct LilyCheckedHistory type.
 */
CONSTRUCTOR(LilyCheckedHistory *, LilyCheckedHistory);

/**
 *
 * @brief Add item to the history.
 */
inline void
add__LilyCheckedHistory(LilyCheckedHistory *self, LilyCheckedDecl *item)
{
    push__Vec(self->items, item);
}

/**
 *
 * @brief Remove the last element to the history.
 */
inline void
pop__LilyCheckedHistory(LilyCheckedHistory *self)
{
    pop__Vec(self->items);
}

/**
 *
 * @brief Check if that contains the fun declaration with the given signature.
 * @param signature LilyCheckedSignatureFun*? (&)
 */
bool
contains_for_fun__LilyCheckedHistory(const LilyCheckedHistory *self,
                                     LilyCheckedSignatureFun *signature);

/**
 *
 * @brief Check if that contains the declaration with the given global name.
 */
bool
contains__LilyCheckedHistory(const LilyCheckedHistory *self,
                             String *global_name);

/**
 *
 * @brief Free LilyCheckedHistory type.
 */
DESTRUCTOR(LilyCheckedHistory, LilyCheckedHistory **self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_HISTORY_H
