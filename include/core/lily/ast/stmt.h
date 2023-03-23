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

#ifndef LILY_CORE_LILY_AST_STMT_H
#define LILY_CORE_LILY_AST_STMT_H

#include <base/macros.h>

#include <core/lily/ast/stmt/asm.h>
#include <core/lily/ast/stmt/await.h>
#include <core/lily/ast/stmt/block.h>
#include <core/lily/ast/stmt/break.h>
#include <core/lily/ast/stmt/defer.h>
#include <core/lily/ast/stmt/drop.h>
#include <core/lily/ast/stmt/for.h>
#include <core/lily/ast/stmt/if.h>
#include <core/lily/ast/stmt/match.h>
#include <core/lily/ast/stmt/next.h>
#include <core/lily/ast/stmt/return.h>
#include <core/lily/ast/stmt/try.h>
#include <core/lily/ast/stmt/variable.h>
#include <core/lily/ast/stmt/while.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

enum LilyAstStmtKind
{
    LILY_AST_STMT_KIND_ASM,
    LILY_AST_STMT_KIND_AWAIT,
    LILY_AST_STMT_KIND_BLOCK,
    LILY_AST_STMT_KIND_BREAK,
    LILY_AST_STMT_KIND_DEFER,
    LILY_AST_STMT_KIND_DROP,
    LILY_AST_STMT_KIND_FOR,
    LILY_AST_STMT_KIND_IF,
    LILY_AST_STMT_KIND_MATCH,
    LILY_AST_STMT_KIND_NEXT,
    LILY_AST_STMT_KIND_RETURN,
    LILY_AST_STMT_KIND_TRY,
    LILY_AST_STMT_KIND_VARIABLE,
    LILY_AST_STMT_KIND_WHILE
};

/**
 *
 * @brief Convert LilyAstStmtKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstStmtKind, enum LilyAstStmtKind self);
#endif

typedef struct LilyAstStmt
{
    enum LilyAstStmtKind kind;
    union
    {
        LilyAstStmtAsm asm_;
        LilyAstStmtAwait await;
        LilyAstStmtBlock block;
        LilyAstStmtBreak break_;
        LilyAstStmtDefer defer;
        LilyAstStmtDrop drop;
        LilyAstStmtFor for_;
        LilyAstStmtIf if_;
        LilyAstStmtMatch match;
        LilyAstStmtNext next;
        LilyAstStmtReturn return_;
        LilyAstStmtTry try;
        LilyAstStmtVariable variable;
        LilyAstStmtWhile while_;
    };
} LilyAstStmt;

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_ASM).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt, LilyAstStmt, asm, LilyAstStmtAsm asm_)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_ASM, .asm_ = asm_ };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_AWAIT).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           await,
                           LilyAstStmtAwait await)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_AWAIT, .await = await };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_BLOCK).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           block,
                           LilyAstStmtBlock block)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_BLOCK, .block = block };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_BREAK).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           break,
                           LilyAstStmtBreak break_)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_BREAK, .break_ = break_ };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_DEFER).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           defer,
                           LilyAstStmtDefer defer)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_DEFER, .defer = defer };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_DROP).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt, LilyAstStmt, drop, LilyAstStmtDrop drop)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_DROP, .drop = drop };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_FOR).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           for,
                           LilyAstStmtFor for_)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_FOR, .for_ = for_ };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_IF).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt, LilyAstStmt, if, LilyAstStmtIf if_)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_IF, .if_ = if_ };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_MATCH).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           match,
                           LilyAstStmtMatch match)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_MATCH, .match = match };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_NEXT).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt, LilyAstStmt, next, LilyAstStmtNext next)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_NEXT, .next = next };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_RETURN).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           return,
                           LilyAstStmtReturn return_)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_RETURN,
                          .return_ = return_ };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_TRY).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt, LilyAstStmt, try, LilyAstStmtTry try)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_TRY, .try = try };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_VARIABLE).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           variable,
                           LilyAstStmtVariable variable)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_VARIABLE,
                          .variable = variable };
}

/**
 *
 * @brief Construct LilyAstStmt type (LILY_AST_STMT_KIND_WHILE).
 */
inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           while,
                           LilyAstStmtWhile while_)
{
    return (LilyAstStmt){ .kind = LILY_AST_STMT_KIND_WHILE, .while_ = while_ };
}

/**
 *
 * @brief Convert LilyAstStmt in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstStmt, const LilyAstStmt *self);
#endif

/**
 *
 * @brief Free LilyAstStmt type.
 */
DESTRUCTOR(LilyAstStmt, const LilyAstStmt *self);

#endif // LILY_CORE_LILY_AST_STMT_H
