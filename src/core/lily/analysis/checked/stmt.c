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

#include <base/alloc.h>

#include <core/lily/analysis/checked/stmt.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_ASM).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 asm,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_AWAIT).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 await,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_BLOCK).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 block,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_DROP).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 drop,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_FOR).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt, for, const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_IF).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 if,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_MATCH).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 match,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_RAISE).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 raise,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_RETURN).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 return,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_SWITCH).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 switch,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_TRY).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 try,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_UNSAFE).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 unsafe,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_VARIABLE).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 variable,
                                 const LilyCheckedStmt *self);

// Free LilyCheckedStmt type (LILY_CHECKED_STMT_KIND_WHILE).
static inline VARIANT_DESTRUCTOR(LilyCheckedStmt,
                                 while,
                                 const LilyCheckedStmt *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmtKind, enum LilyCheckedStmtKind self)
{
    switch (self) {
        case LILY_CHECKED_STMT_KIND_ASM:
            return "LILY_CHECKED_STMT_KIND_ASM";
        case LILY_CHECKED_STMT_KIND_AWAIT:
            return "LILY_CHECKED_STMT_KIND_AWAIT";
        case LILY_CHECKED_STMT_KIND_BLOCK:
            return "LILY_CHECKED_STMT_KIND_BLOCK";
        case LILY_CHECKED_STMT_KIND_BREAK:
            return "LILY_CHECKED_STMT_KIND_BREAK";
        case LILY_CHECKED_STMT_KIND_DROP:
            return "LILY_CHECKED_STMT_KIND_DROP";
        case LILY_CHECKED_STMT_KIND_FOR:
            return "LILY_CHECKED_STMT_KIND_FOR";
        case LILY_CHECKED_STMT_KIND_IF:
            return "LILY_CHECKED_STMT_KIND_IF";
        case LILY_CHECKED_STMT_KIND_MATCH:
            return "LILY_CHECKED_STMT_KIND_MATCH";
        case LILY_CHECKED_STMT_KIND_NEXT:
            return "LILY_CHECKED_STMT_KIND_NEXT";
        case LILY_CHECKED_STMT_KIND_RAISE:
            return "LILY_CHECKED_STMT_KIND_RAISE";
        case LILY_CHECKED_STMT_KIND_RETURN:
            return "LILY_CHECKED_STMT_KIND_RETURN";
        case LILY_CHECKED_STMT_KIND_SWITCH:
            return "LILY_CHECKED_STMT_KIND_SWITCH";
        case LILY_CHECKED_STMT_KIND_TRY:
            return "LILY_CHECKED_STMT_KIND_TRY";
        case LILY_CHECKED_STMT_KIND_UNSAFE:
            return "LILY_CHECKED_STMT_KIND_UNSAFE";
        case LILY_CHECKED_STMT_KIND_VARIABLE:
            return "LILY_CHECKED_STMT_KIND_VARIABLE";
        case LILY_CHECKED_STMT_KIND_WHILE:
            return "LILY_CHECKED_STMT_KIND_WHILE";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmt, const LilyCheckedStmt *self)
{
    String *res = NULL;

    if (self->ast_stmt) {
        res = format__String(
          "LilyCheckedStmt{{ kind = {s}, location = {sa}, ast_stmt = {Sr}",
          to_string__Debug__LilyCheckedStmtKind(self->kind),
          to_string__Debug__Location(self->location),
          to_string__Debug__LilyAstStmt(self->ast_stmt));
    } else {
        res = format__String(
          "LilyCheckedStmt{{ kind = {s}, location = {sa}, ast_stmt = NULL",
          to_string__Debug__LilyCheckedStmtKind(self->kind),
          to_string__Debug__Location(self->location));
    }

    switch (self->kind) {
        case LILY_CHECKED_STMT_KIND_ASM: {
            char *s = format(", asm = {Sr} }",
                             to_string__Debug__LilyCheckedStmtAsm(&self->asm_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_AWAIT: {
            char *s =
              format(", await = {Sr} }",
                     to_string__Debug__LilyCheckedStmtAwait(&self->await));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_BLOCK: {
            char *s =
              format(", block = {Sr} }",
                     to_string__Debug__LilyCheckedStmtBlock(&self->block));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_BREAK: {
            char *s =
              format(", break = {Sr} }",
                     to_string__Debug__LilyCheckedStmtBreak(&self->break_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_DROP: {
            char *s =
              format(", drop = {Sr} }",
                     to_string__Debug__LilyCheckedStmtDrop(&self->drop));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_FOR: {
            char *s = format(", for = {Sr} }",
                             to_string__Debug__LilyCheckedStmtFor(&self->for_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_IF: {
            char *s = format(", if = {Sr} }",
                             to_string__Debug__LilyCheckedStmtIf(&self->if_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_MATCH: {
            char *s =
              format(", match = {Sr} }",
                     to_string__Debug__LilyCheckedStmtMatch(&self->match));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_NEXT: {
            char *s =
              format(", next = {Sr} }",
                     to_string__Debug__LilyCheckedStmtNext(&self->next));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_RAISE: {
            char *s =
              format(", raise = {Sr} }",
                     to_string__Debug__LilyCheckedStmtRaise(&self->raise));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_RETURN: {
            char *s =
              format(", return = {Sr} }",
                     to_string__Debug__LilyCheckedStmtReturn(&self->return_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_SWITCH: {
            char *s =
              format(", switch = {Sr} }",
                     to_string__Debug__LilyCheckedStmtSwitch(&self->switch_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_TRY: {
            char *s = format(", try = {Sr} }",
                             to_string__Debug__LilyCheckedStmtTry(&self->try));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_UNSAFE: {
            char *s =
              format(", unsafe = {Sr} }",
                     to_string__Debug__LilyCheckedStmtUnsafe(&self->unsafe));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_VARIABLE: {
            char *s = format(
              ", variable = {Sr} }",
              to_string__Debug__LilyCheckedStmtVariable(&self->variable));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_STMT_KIND_WHILE: {
            char *s =
              format(", while = {Sr} }",
                     to_string__Debug__LilyCheckedStmtWhile(&self->while_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedStmt, asm, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtAsm, &self->asm_);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, await, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtAwait, &self->await);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, block, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtBlock, &self->block);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, drop, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtDrop, &self->drop);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, for, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtFor, &self->for_);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, if, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtIf, &self->if_);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, match, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtMatch, &self->match);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, raise, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtRaise, &self->raise);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, return, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtReturn, &self->return_);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, switch, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtSwitch, &self->switch_);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, try, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtTry, &self->try);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, unsafe, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtUnsafe, &self->unsafe);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, variable, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtVariable, &self->variable);
}

VARIANT_DESTRUCTOR(LilyCheckedStmt, while, const LilyCheckedStmt *self)
{
    FREE(LilyCheckedStmtWhile, &self->while_);
}

DESTRUCTOR(LilyCheckedStmt, const LilyCheckedStmt *self)
{
    switch (self->kind) {
        case LILY_CHECKED_STMT_KIND_ASM:
            FREE_VARIANT(LilyCheckedStmt, asm, self);
            break;
        case LILY_CHECKED_STMT_KIND_AWAIT:
            FREE_VARIANT(LilyCheckedStmt, await, self);
            break;
        case LILY_CHECKED_STMT_KIND_BLOCK:
            FREE_VARIANT(LilyCheckedStmt, block, self);
            break;
        case LILY_CHECKED_STMT_KIND_DROP:
            FREE_VARIANT(LilyCheckedStmt, drop, self);
            break;
        case LILY_CHECKED_STMT_KIND_FOR:
            FREE_VARIANT(LilyCheckedStmt, for, self);
            break;
        case LILY_CHECKED_STMT_KIND_IF:
            FREE_VARIANT(LilyCheckedStmt, if, self);
            break;
        case LILY_CHECKED_STMT_KIND_MATCH:
            FREE_VARIANT(LilyCheckedStmt, match, self);
            break;
        case LILY_CHECKED_STMT_KIND_RAISE:
            FREE_VARIANT(LilyCheckedStmt, raise, self);
            break;
        case LILY_CHECKED_STMT_KIND_RETURN:
            FREE_VARIANT(LilyCheckedStmt, return, self);
            break;
        case LILY_CHECKED_STMT_KIND_SWITCH:
            FREE_VARIANT(LilyCheckedStmt, switch, self);
            break;
        case LILY_CHECKED_STMT_KIND_TRY:
            FREE_VARIANT(LilyCheckedStmt, try, self);
            break;
        case LILY_CHECKED_STMT_KIND_UNSAFE:
            FREE_VARIANT(LilyCheckedStmt, unsafe, self);
            break;
        case LILY_CHECKED_STMT_KIND_VARIABLE:
            FREE_VARIANT(LilyCheckedStmt, variable, self);
            break;
        case LILY_CHECKED_STMT_KIND_WHILE:
            FREE_VARIANT(LilyCheckedStmt, while, self);
            break;
        case LILY_CHECKED_STMT_KIND_BREAK:
        case LILY_CHECKED_STMT_KIND_NEXT:
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
