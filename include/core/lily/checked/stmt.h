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

#ifndef LILY_CORE_LILY_CHECKED_STMT_H
#define LILY_CORE_LILY_CHECKED_STMT_H

#include <base/macros.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

enum LilyCheckedStmtKind
{
    LILY_CHECKED_STMT_KIND_ASM,
    LILY_CHECKED_STMT_KIND_AWAIT,
    LILY_CHECKED_STMT_KIND_BLOCK,
    LILY_CHECKED_STMT_KIND_BREAK,
    LILY_CHECKED_STMT_KIND_DEFER,
    LILY_CHECKED_STMT_KIND_DROP,
    LILY_CHECKED_STMT_KIND_FOR,
    LILY_CHECKED_STMT_KIND_IF,
    LILY_CHECKED_STMT_KIND_MATCH,
    LILY_CHECKED_STMT_KIND_NEXT,
    LILY_CHECKED_STMT_KIND_RAISE,
    LILY_CHECKED_STMT_KIND_RETURN,
    LILY_CHECKED_STMT_KIND_TRY,
    LILY_CHECKED_STMT_KIND_UNSAFE,
    LILY_CHECKED_STMT_KIND_VARIABLE,
    LILY_CHECKED_STMT_KIND_WHILE
};

/**
 *
 * @brief Convert LilyCheckedStmtKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmtKind, enum LilyCheckedStmtKind self);
#endif

typedef struct LilyCheckedStmt {
    enum LilyAstStmtKind kind;
    Location location;
} LilyCheckedStmt;

/**
 *
 * @brief Convert LilyCheckedStmt in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmt, const LilyCheckedStmt *self);
#endif

/**
 *
 * @brief Free LilyCheckedStmt type.
 */
DESTRUCTOR(LilyCheckedStmt, const LilyCheckedStmt *self);

#endif // LILY_CORE_LILY_CHECKED_STMT_H