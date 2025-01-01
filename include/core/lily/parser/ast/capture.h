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

#ifndef LILY_CORE_LILY_PARSER_AST_CAPTURE_H
#define LILY_CORE_LILY_PARSER_AST_CAPTURE_H

#include <base/vec.h>

#include <core/lily/parser/ast/expr.h>

enum LilyAstCaptureKind
{
    LILY_AST_CAPTURE_KIND_SINGLE,
    LILY_AST_CAPTURE_KIND_MULTIPLE
};

/**
 *
 * @brief Convert LilyAstCaptureKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstCaptureKind, enum LilyAstCaptureKind self);
#endif

typedef struct LilyAstCapture
{
    enum LilyAstCaptureKind kind;
    union
    {
        // NOTE: all expressions are identifiers
        LilyAstExpr *single;
        Vec *multiple; // Vec<LilyAstExpr*>*
    };
} LilyAstCapture;

/**
 *
 * @brief Construct LilyAstCapture type (LILY_AST_CAPTURE_KIND_SINGLE).
 */
VARIANT_CONSTRUCTOR(LilyAstCapture *,
                    LilyAstCapture,
                    single,
                    LilyAstExpr *single);

/**
 *
 * @brief Construct LilyAstCapture type (LILY_AST_CAPTURE_KIND_MULTIPLE).
 */
VARIANT_CONSTRUCTOR(LilyAstCapture *, LilyAstCapture, multiple, Vec *multiple);

/**
 *
 * @brief Convert LilyAstCapture in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstCapture, const LilyAstCapture *self);
#endif

/**
 *
 * @brief Free LilyAstCapture type.
 */
DESTRUCTOR(LilyAstCapture, LilyAstCapture *self);

#endif // LILY_CORE_LILY_PARSER_AST_CAPTURE_H
