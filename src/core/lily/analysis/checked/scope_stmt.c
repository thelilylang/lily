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

#include <base/format.h>

#include <core/lily/analysis/checked/scope_stmt.h>
#include <core/lily/analysis/checked/stmt/for.h>
#include <core/lily/analysis/checked/stmt/if.h>
#include <core/lily/analysis/checked/stmt/match.h>
#include <core/lily/analysis/checked/stmt/while.h>

#include <stdio.h>
#include <stdlib.h>

OrderedHashMap *
get_captured_variables__LilyCheckedScopeStmt(const LilyCheckedScopeStmt *self)
{
    switch (self->kind) {
        case LILY_CHECKED_SCOPE_STMT_KIND_ELSE_BRANCH:
            TODO("get captured variables from else");
        case LILY_CHECKED_SCOPE_STMT_KIND_FOR:
            return self->for_->captured_variables;
        case LILY_CHECKED_SCOPE_STMT_KIND_IF_BRANCH:
            TODO("get captured variables from if");
        case LILY_CHECKED_SCOPE_STMT_KIND_MATCH_CASE:
            TODO("get captured variables from match case");
        case LILY_CHECKED_SCOPE_STMT_KIND_WHILE:
            TODO("get captured variables from while");
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeStmtKind,
               enum LilyCheckedScopeStmtKind self)
{
    switch (self) {
        case LILY_CHECKED_SCOPE_STMT_KIND_ELSE_BRANCH:
            return "LILY_CHECKED_SCOPE_STMT_KIND_ELSE_BRANCH";
        case LILY_CHECKED_SCOPE_STMT_KIND_FOR:
            return "LILY_CHECKED_SCOPE_STMT_KIND_FOR";
        case LILY_CHECKED_SCOPE_STMT_KIND_IF_BRANCH:
            return "LILY_CHECKED_SCOPE_STMT_KIND_IF_BRANCH";
        case LILY_CHECKED_SCOPE_STMT_KIND_MATCH_CASE:
            return "LILY_CHECKED_SCOPE_STMT_KIND_MATCH_CASE";
        case LILY_CHECKED_SCOPE_STMT_KIND_WHILE:
            return "LILY_CHECKED_SCOPE_STMT_KIND_WHILE";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeStmt,
               const LilyCheckedScopeStmt *self)
{
    String *res =
      format__String("LilyCheckedScopeStmt{{ kind = {s}",
                     to_string__Debug__LilyCheckedScopeStmtKind(self->kind));

    switch (self->kind) {
        case LILY_CHECKED_SCOPE_STMT_KIND_ELSE_BRANCH: {
            char *s = format(
              ", else_branch = {Sr} }",
              to_string__Debug__LilyCheckedStmtElseBranch(self->else_branch));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_SCOPE_STMT_KIND_FOR: {
            char *s = format(", for = {Sr} }",
                             to_string__Debug__LilyCheckedStmtFor(self->for_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_SCOPE_STMT_KIND_IF_BRANCH: {
            char *s = format(
              ", if_branch = {Sr} }",
              to_string__Debug__LilyCheckedStmtIfBranch(self->if_branch));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_SCOPE_STMT_KIND_MATCH_CASE: {
            char *s = format(
              ", match_case = {Sr} }",
              to_string__Debug__LilyCheckedStmtMatchCase(self->match_case));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_SCOPE_STMT_KIND_WHILE: {
            char *s =
              format(", while = {Sr} }",
                     to_string__Debug__LilyCheckedStmtWhile(self->while_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif
