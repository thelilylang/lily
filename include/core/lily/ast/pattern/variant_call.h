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

#ifndef LILY_CORE_LILY_AST_PATTERN_VARIANT_CALL_H
#define LILY_CORE_LILY_AST_PATTERN_VARIANT_CALL_H

#include <core/lily/ast/expr.h>

typedef struct LilyAstPattern LilyAstPattern;

typedef struct LilyAstPatternVariantCall
{
    LilyAstExpr *id;
    LilyAstPattern *pattern;
} LilyAstPatternVariantCall;

/**
 *
 * @brief Construct LilyAstPatternVariantCall type.
 */
inline CONSTRUCTOR(LilyAstPatternVariantCall,
                   LilyAstPatternVariantCall,
                   LilyAstExpr *id,
                   LilyAstPattern *pattern)
{
    return (LilyAstPatternVariantCall){ .id = id, .pattern = pattern };
}

/**
 *
 * @brief Convert LilyAstPatternVariantCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstPatternVariantCall,
               const LilyAstPatternVariantCall *self);
#endif

/**
 *
 * @brief Free LilyAstPatternVariantCall type.
 */
DESTRUCTOR(LilyAstPatternVariantCall, const LilyAstPatternVariantCall *self);

#endif // LILY_CORE_LILY_AST_PATTERN_VARIANT_CALL_H
