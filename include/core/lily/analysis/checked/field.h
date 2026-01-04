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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_FIELD_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_FIELD_H

#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/expr.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyCheckedField
{
    String *name; // String* (&)
    LilyCheckedDataType *data_type;
    LilyCheckedExpr *optional_expr; // LilyCheckedExpr*?
    enum LilyVisibility visibility;
    bool is_mut;
    const Location *location;
} LilyCheckedField;

/**
 *
 * @brief Construct LilyCheckedField type.
 */
CONSTRUCTOR(LilyCheckedField *,
            LilyCheckedField,
            String *name,
            LilyCheckedDataType *data_type,
            LilyCheckedExpr *optional_expr,
            bool is_mut,
            const Location *location);

/**
 *
 * @brief Convert LilyCheckedField in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedField, const LilyCheckedField *self);
#endif

/**
 *
 * @brief Free LilyCheckedField type.
 */
DESTRUCTOR(LilyCheckedField, LilyCheckedField *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_FIELD_H
