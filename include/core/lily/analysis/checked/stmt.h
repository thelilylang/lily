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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_H

#include <base/macros.h>

#include <core/lily/analysis/checked/stmt/asm.h>
#include <core/lily/analysis/checked/stmt/await.h>
#include <core/lily/analysis/checked/stmt/block.h>
#include <core/lily/analysis/checked/stmt/break.h>
#include <core/lily/analysis/checked/stmt/drop.h>
#include <core/lily/analysis/checked/stmt/for.h>
#include <core/lily/analysis/checked/stmt/if.h>
#include <core/lily/analysis/checked/stmt/match.h>
#include <core/lily/analysis/checked/stmt/next.h>
#include <core/lily/analysis/checked/stmt/raise.h>
#include <core/lily/analysis/checked/stmt/return.h>
#include <core/lily/analysis/checked/stmt/switch.h>
#include <core/lily/analysis/checked/stmt/try.h>
#include <core/lily/analysis/checked/stmt/unsafe.h>
#include <core/lily/analysis/checked/stmt/variable.h>
#include <core/lily/analysis/checked/stmt/while.h>
#include <core/lily/parser/ast/stmt.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

enum LilyCheckedStmtKind
{
    LILY_CHECKED_STMT_KIND_ASM,
    LILY_CHECKED_STMT_KIND_AWAIT,
    LILY_CHECKED_STMT_KIND_BLOCK,
    LILY_CHECKED_STMT_KIND_BREAK,
    LILY_CHECKED_STMT_KIND_DROP,
    LILY_CHECKED_STMT_KIND_FOR,
    LILY_CHECKED_STMT_KIND_IF,
    LILY_CHECKED_STMT_KIND_MATCH,
    LILY_CHECKED_STMT_KIND_NEXT,
    LILY_CHECKED_STMT_KIND_RAISE,
    LILY_CHECKED_STMT_KIND_RETURN,
    LILY_CHECKED_STMT_KIND_SWITCH, // only usable by the compiler
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

typedef struct LilyCheckedStmt
{
    enum LilyCheckedStmtKind kind;
    const Location *location;    // const Location* (&)
    const LilyAstStmt *ast_stmt; // const LilyAstStmt*? (&) e.g. optional in
                                 // the case of implicit return, ...
    union
    {
        LilyCheckedStmtAsm asm_;
        LilyCheckedStmtAwait await;
        LilyCheckedStmtBlock block;
        LilyCheckedStmtBreak break_;
        LilyCheckedStmtDrop drop;
        LilyCheckedStmtFor for_;
        LilyCheckedStmtIf if_;
        LilyCheckedStmtMatch match;
        LilyCheckedStmtNext next;
        LilyCheckedStmtRaise raise;
        LilyCheckedStmtReturn return_;
        LilyCheckedStmtSwitch switch_;
        LilyCheckedStmtTry try;
        LilyCheckedStmtUnsafe unsafe;
        LilyCheckedStmtVariable variable;
        LilyCheckedStmtWhile while_;
    };
} LilyCheckedStmt;

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_ASM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           asm,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtAsm asm_)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_ASM,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .asm_ = asm_ };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_AWAIT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           await,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtAwait await)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_AWAIT,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .await = await };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_BLOCK).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           block,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtBlock block)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_BLOCK,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .block = block };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_BREAK).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           break,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtBreak break_)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_BREAK,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .break_ = break_ };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_DROP).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           drop,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtDrop drop)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_DROP,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .drop = drop };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_FOR).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           for,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtFor for_)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_FOR,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .for_ = for_ };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_IF).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           if,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtIf if_)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_IF,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .if_ = if_ };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_MATCH).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           match,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtMatch match)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_MATCH,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .match = match };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_NEXT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           next,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtNext next)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_NEXT,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .next = next };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_RAISE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           raise,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtRaise raise)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_RAISE,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .raise = raise };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_RETURN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           return,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtReturn return_)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_RETURN,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .return_ = return_ };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_SWITCH).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           switch,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtSwitch switch_)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_SWITCH,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .switch_ = switch_ };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_TRY).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           try,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtTry try)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_TRY,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .try = try };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_UNSAFE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           unsafe,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtUnsafe unsafe)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_UNSAFE,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .unsafe = unsafe };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_VARIABLE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           variable,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtVariable variable)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_VARIABLE,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .variable = variable };
}

/**
 *
 * @brief Construct LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_WHILE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           while,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtWhile while_)
{
    return (LilyCheckedStmt){ .kind = LILY_CHECKED_STMT_KIND_WHILE,
                              .ast_stmt = ast_stmt,
                              .location = location,
                              .while_ = while_ };
}

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

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_H
