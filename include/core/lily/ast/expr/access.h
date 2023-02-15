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

#ifndef LILY_CORE_LILY_AST_EXPR_ACCESS_H
#define LILY_CORE_LILY_AST_EXPR_ACCESS_H

#include <base/vec.h>

typedef struct LilyAstExpr LilyAstExpr;

enum LilyAstAccessKind
{
    LILY_AST_ACCESS_KIND_GLOBAL,
    LILY_AST_ACCESS_KIND_HOOK,
    LILY_AST_ACCESS_KIND_OBJECT,
    LILY_AST_ACCESS_KIND_PATH,
    LILY_AST_ACCESS_KIND_PROPERTY_INIT,
    LILY_AST_ACCESS_KIND_SELF,
};

typedef struct LilyAstAccessHook
{
    LilyAstExpr *access;
    LilyAstExpr *id;
} LilyAstAccessHook;

typedef struct LilyAstAccess
{
    enum LilyAstAccessKind kind;
    union
    {
        LilyAstExpr *global;
        LilyAstAccessHook hook;
        Vec *object; // Vec<LilyAstDataType*>*
        Vec *path;   // Vec<LilyAstExpr*>*
        LilyAstExpr *property_init;
        LilyAstExpr *self;
    };
} LilyAstAccess;

#endif // LILY_CORE_LILY_AST_EXPR_ACCESS_H