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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_ATTRIBUTE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_ATTRIBUTE_H

#include <base/string.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/expr.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyCheckedDeclAttribute
{
    String *name; // String* (&)
    String *global_name;
    LilyCheckedDataType *data_type;
    LilyCheckedExpr *optional_expr; // LilyCheckedExpr*?
    bool is_set;
    bool is_get;
    enum LilyVisibility visibility;
    bool is_checked;
} LilyCheckedDeclAttribute;

/**
 *
 * @brief Construct LilyCheckedDeclAttribute type.
 */
inline CONSTRUCTOR(LilyCheckedDeclAttribute,
                   LilyCheckedDeclAttribute,
                   String *name,
                   String *global_name,
                   LilyCheckedDataType *data_type,
                   LilyCheckedExpr *optional_expr,
                   bool is_set,
                   bool is_get,
                   enum LilyVisibility visibility)
{
    return (LilyCheckedDeclAttribute){ .name = name,
                                       .global_name = global_name,
                                       .data_type = data_type,
                                       .optional_expr = optional_expr,
                                       .is_set = is_set,
                                       .is_get = is_get,
                                       .visibility = visibility,
                                       .is_checked = false };
}

/**
 *
 * @brief Convert LilyCheckedDeclAttribute in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclAttribute,
               const LilyCheckedDeclAttribute *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclAttribute type.
 */
DESTRUCTOR(LilyCheckedDeclAttribute, const LilyCheckedDeclAttribute *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_ATTRIBUTE_H
