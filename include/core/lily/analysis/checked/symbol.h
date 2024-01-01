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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SYMBOL_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SYMBOL_H

#include <base/alloc.h>
#include <base/types.h>

#include <core/lily/shared/visibility.h>

enum LilyCheckedSymbolKind
{
    LILY_CHECKED_SYMBOL_KIND_ALIAS,
    LILY_CHECKED_SYMBOL_KIND_CLASS,
    LILY_CHECKED_SYMBOL_KIND_CONSTANT,
    LILY_CHECKED_SYMBOL_KIND_ENUM,
    LILY_CHECKED_SYMBOL_KIND_ENUM_OBJECT,
    LILY_CHECKED_SYMBOL_KIND_ERROR,
    LILY_CHECKED_SYMBOL_KIND_FIELD,
    LILY_CHECKED_SYMBOL_KIND_FUN,
    LILY_CHECKED_SYMBOL_KIND_LABEL,
    LILY_CHECKED_SYMBOL_KIND_METHOD,
    LILY_CHECKED_SYMBOL_KIND_RECORD,
    LILY_CHECKED_SYMBOL_KIND_RECORD_OBJECT,
    LILY_CHECKED_SYMBOL_KIND_TRAIT,
    LILY_CHECKED_SYMBOL_KIND_VARIABLE,
};

typedef struct LilyCheckedSymbol
{
    Usize scope_id;
    Usize id;
    enum LilyCheckedSymbolKind kind;
    enum LilyVisibility visibility;
} LilyCheckedSymbol;

/**
 *
 * @brief Construct LilyCheckedSymbol type.
 */
CONSTRUCTOR(LilyCheckedSymbol *,
            LilyCheckedSymbol,
            Usize id,
            Usize scope_id,
            enum LilyCheckedSymbolKind kind,
            enum LilyVisibility visibility);

/**
 *
 * @brief Free LilyCheckedSymbol type.
 */
inline DESTRUCTOR(LilyCheckedSymbol, LilyCheckedSymbol *self)
{
    lily_free(self);
}

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SYMBOL_H
