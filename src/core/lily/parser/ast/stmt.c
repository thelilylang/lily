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

#include <base/alloc.h>

#include <core/lily/parser/ast/stmt.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyAstStmt type (LILY_AST_STMT_KIND_ASM).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, asm, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_AWAIT).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, await, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_BLOCK).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, block, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_BREAK).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, break, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_DEFER).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, defer, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_DROP).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, drop, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_FOR).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, for, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_IF).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, if, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_MATCH).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, match, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_NEXT).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, next, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_RAISE).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, raise, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_RETURN).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, return, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_TRY).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, try, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_UNSAFE).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, unsafe, const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_VARIABLE).
static inline VARIANT_DESTRUCTOR(LilyAstStmt,
                                 variable,
                                 const LilyAstStmt *self);

// Free LilyAstStmt type (LILY_AST_STMT_KIND_WHILE).
static inline VARIANT_DESTRUCTOR(LilyAstStmt, while, const LilyAstStmt *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstStmtKind, enum LilyAstStmtKind self)
{
    switch (self) {
        case LILY_AST_STMT_KIND_ASM:
            return "LILY_AST_STMT_KIND_ASM";
        case LILY_AST_STMT_KIND_AWAIT:
            return "LILY_AST_STMT_KIND_AWAIT";
        case LILY_AST_STMT_KIND_BLOCK:
            return "LILY_AST_STMT_KIND_BLOCK";
        case LILY_AST_STMT_KIND_BREAK:
            return "LILY_AST_STMT_KIND_BREAK";
        case LILY_AST_STMT_KIND_DEFER:
            return "LILY_AST_STMT_KIND_DEFER";
        case LILY_AST_STMT_KIND_DROP:
            return "LILY_AST_STMT_KIND_DROP";
        case LILY_AST_STMT_KIND_FOR:
            return "LILY_AST_STMT_KIND_FOR";
        case LILY_AST_STMT_KIND_IF:
            return "LILY_AST_STMT_KIND_IF";
        case LILY_AST_STMT_KIND_MATCH:
            return "LILY_AST_STMT_KIND_MATCH";
        case LILY_AST_STMT_KIND_NEXT:
            return "LILY_AST_STMT_KIND_NEXT";
        case LILY_AST_STMT_KIND_RAISE:
            return "LILY_AST_STMT_KIND_RAISE";
        case LILY_AST_STMT_KIND_RETURN:
            return "LILY_AST_STMT_KIND_RETURN";
        case LILY_AST_STMT_KIND_TRY:
            return "LILY_AST_STMT_KIND_TRY";
        case LILY_AST_STMT_KIND_UNSAFE:
            return "LILY_AST_STMT_KIND_UNSAFE";
        case LILY_AST_STMT_KIND_VARIABLE:
            return "LILY_AST_STMT_KIND_VARIABLE";
        case LILY_AST_STMT_KIND_WHILE:
            return "LILY_AST_STMT_KIND_WHILE";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyAstStmt, const LilyAstStmt *self)
{
    String *res = format__String("LilyAstStmt{{ kind = {s}, location = {sa}",
                                 to_string__Debug__LilyAstStmtKind(self->kind),
                                 to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_AST_STMT_KIND_ASM: {
            char *s = format(", asm = {Sr} }",
                             to_string__Debug__LilyAstStmtAsm(&self->asm_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_AWAIT: {
            char *s = format(", await = {Sr} }",
                             to_string__Debug__LilyAstStmtAwait(&self->await));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_BLOCK: {
            char *s = format(", block = {Sr} }",
                             to_string__Debug__LilyAstStmtBlock(&self->block));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_BREAK: {
            char *s = format(", break = {Sr} }",
                             to_string__Debug__LilyAstStmtBreak(&self->break_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_DEFER: {
            char *s = format(", defer = {Sr} }",
                             to_string__Debug__LilyAstStmtDefer(&self->defer));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_DROP: {
            char *s = format(", drop = {Sr} }",
                             to_string__Debug__LilyAstStmtDrop(&self->drop));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_FOR: {
            char *s = format(", for = {Sr} }",
                             to_string__Debug__LilyAstStmtFor(&self->for_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_IF: {
            char *s = format(", if = {Sr} }",
                             to_string__Debug__LilyAstStmtIf(&self->if_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_MATCH: {
            char *s = format(", match = {Sr} }",
                             to_string__Debug__LilyAstStmtMatch(&self->match));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_NEXT: {
            char *s = format(", next = {Sr} }",
                             to_string__Debug__LilyAstStmtNext(&self->next));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_RAISE: {
            char *s = format(", raise = {Sr} }",
                             to_string__Debug__LilyAstStmtRaise(&self->raise));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_RETURN: {
            char *s =
              format(", return = {Sr} }",
                     to_string__Debug__LilyAstStmtReturn(&self->return_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_TRY: {
            char *s = format(", try = {Sr} }",
                             to_string__Debug__LilyAstStmtTry(&self->try));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_UNSAFE: {
            char *s =
              format(", unsafe = {Sr} }",
                     to_string__Debug__LilyAstStmtUnsafe(&self->unsafe));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_VARIABLE: {
            char *s =
              format(", variable = {Sr} }",
                     to_string__Debug__LilyAstStmtVariable(&self->variable));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_STMT_KIND_WHILE: {
            char *s = format(", while = {Sr} }",
                             to_string__Debug__LilyAstStmtWhile(&self->while_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyAstStmt, asm, const LilyAstStmt *self)
{
    FREE(LilyAstStmtAsm, &self->asm_);
}

VARIANT_DESTRUCTOR(LilyAstStmt, await, const LilyAstStmt *self)
{
    FREE(LilyAstStmtAwait, &self->await);
}

VARIANT_DESTRUCTOR(LilyAstStmt, block, const LilyAstStmt *self)
{
    FREE(LilyAstStmtBlock, &self->block);
}

VARIANT_DESTRUCTOR(LilyAstStmt, break, const LilyAstStmt *self)
{
    FREE(LilyAstStmtBreak, &self->break_);
}

VARIANT_DESTRUCTOR(LilyAstStmt, defer, const LilyAstStmt *self)
{
    FREE(LilyAstStmtDefer, &self->defer);
}

VARIANT_DESTRUCTOR(LilyAstStmt, drop, const LilyAstStmt *self)
{
    FREE(LilyAstStmtDrop, &self->drop);
}

VARIANT_DESTRUCTOR(LilyAstStmt, for, const LilyAstStmt *self)
{
    FREE(LilyAstStmtFor, &self->for_);
}

VARIANT_DESTRUCTOR(LilyAstStmt, if, const LilyAstStmt *self)
{
    FREE(LilyAstStmtIf, &self->if_);
}

VARIANT_DESTRUCTOR(LilyAstStmt, match, const LilyAstStmt *self)
{
    FREE(LilyAstStmtMatch, &self->match);
}

VARIANT_DESTRUCTOR(LilyAstStmt, next, const LilyAstStmt *self)
{
    FREE(LilyAstStmtNext, &self->next);
}

VARIANT_DESTRUCTOR(LilyAstStmt, raise, const LilyAstStmt *self)
{
    FREE(LilyAstStmtRaise, &self->raise);
}

VARIANT_DESTRUCTOR(LilyAstStmt, return, const LilyAstStmt *self)
{
    FREE(LilyAstStmtReturn, &self->return_);
}

VARIANT_DESTRUCTOR(LilyAstStmt, try, const LilyAstStmt *self)
{
    FREE(LilyAstStmtTry, &self->try);
}

VARIANT_DESTRUCTOR(LilyAstStmt, unsafe, const LilyAstStmt *self)
{
    FREE(LilyAstStmtUnsafe, &self->unsafe);
}

VARIANT_DESTRUCTOR(LilyAstStmt, variable, const LilyAstStmt *self)
{
    FREE(LilyAstStmtVariable, &self->variable);
}

VARIANT_DESTRUCTOR(LilyAstStmt, while, const LilyAstStmt *self)
{
    FREE(LilyAstStmtWhile, &self->while_);
}

DESTRUCTOR(LilyAstStmt, const LilyAstStmt *self)
{
    switch (self->kind) {
        case LILY_AST_STMT_KIND_ASM:
            FREE_VARIANT(LilyAstStmt, asm, self);
            break;
        case LILY_AST_STMT_KIND_AWAIT:
            FREE_VARIANT(LilyAstStmt, await, self);
            break;
        case LILY_AST_STMT_KIND_BLOCK:
            FREE_VARIANT(LilyAstStmt, block, self);
            break;
        case LILY_AST_STMT_KIND_BREAK:
            FREE_VARIANT(LilyAstStmt, break, self);
            break;
        case LILY_AST_STMT_KIND_DEFER:
            FREE_VARIANT(LilyAstStmt, defer, self);
            break;
        case LILY_AST_STMT_KIND_DROP:
            FREE_VARIANT(LilyAstStmt, drop, self);
            break;
        case LILY_AST_STMT_KIND_FOR:
            FREE_VARIANT(LilyAstStmt, for, self);
            break;
        case LILY_AST_STMT_KIND_IF:
            FREE_VARIANT(LilyAstStmt, if, self);
            break;
        case LILY_AST_STMT_KIND_MATCH:
            FREE_VARIANT(LilyAstStmt, match, self);
            break;
        case LILY_AST_STMT_KIND_NEXT:
            FREE_VARIANT(LilyAstStmt, next, self);
            break;
        case LILY_AST_STMT_KIND_RAISE:
            FREE_VARIANT(LilyAstStmt, raise, self);
            break;
        case LILY_AST_STMT_KIND_RETURN:
            FREE_VARIANT(LilyAstStmt, return, self);
            break;
        case LILY_AST_STMT_KIND_TRY:
            FREE_VARIANT(LilyAstStmt, try, self);
            break;
        case LILY_AST_STMT_KIND_UNSAFE:
            FREE_VARIANT(LilyAstStmt, unsafe, self);
            break;
        case LILY_AST_STMT_KIND_VARIABLE:
            FREE_VARIANT(LilyAstStmt, variable, self);
            break;
        case LILY_AST_STMT_KIND_WHILE:
            FREE_VARIANT(LilyAstStmt, while, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
