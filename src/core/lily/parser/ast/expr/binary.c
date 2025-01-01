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

#include <base/format.h>

#include <core/lily/parser/ast/expr.h>
#include <core/lily/parser/ast/expr/binary.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprBinaryKind,
               enum LilyAstExprBinaryKind self)
{
    switch (self) {
        case LILY_AST_EXPR_BINARY_KIND_ADD:
            return "LILY_AST_EXPR_BINARY_KIND_ADD";
        case LILY_AST_EXPR_BINARY_KIND_AND:
            return "LILY_AST_EXPR_BINARY_KIND_AND";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_ADD:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_ADD";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_AND";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_OR";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_DIV:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_DIV";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_EXP:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_EXP";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MOD:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_MOD";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MUL:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_MUL";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_SUB:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN_SUB";
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN:
            return "LILY_AST_EXPR_BINARY_KIND_ASSIGN";
        case LILY_AST_EXPR_BINARY_KIND_BIT_AND:
            return "LILY_AST_EXPR_BINARY_KIND_BIT_AND";
        case LILY_AST_EXPR_BINARY_KIND_BIT_OR:
            return "LILY_AST_EXPR_BINARY_KIND_BIT_OR";
        case LILY_AST_EXPR_BINARY_KIND_CHAIN:
            return "LILY_AST_EXPR_BINARY_KIND_CHAIN";
        case LILY_AST_EXPR_BINARY_KIND_DIV:
            return "LILY_AST_EXPR_BINARY_KIND_DIV";
        case LILY_AST_EXPR_BINARY_KIND_EXP:
            return "LILY_AST_EXPR_BINARY_KIND_EXP";
        case LILY_AST_EXPR_BINARY_KIND_EQ:
            return "LILY_AST_EXPR_BINARY_KIND_EQ";
        case LILY_AST_EXPR_BINARY_KIND_GREATER_EQ:
            return "LILY_AST_EXPR_BINARY_KIND_GREATER_EQ";
        case LILY_AST_EXPR_BINARY_KIND_GREATER:
            return "LILY_AST_EXPR_BINARY_KIND_GREATER";
        case LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT:
            return "LILY_AST_EXPR_BINARY_KIND_L_SHIFT";
        case LILY_AST_EXPR_BINARY_KIND_LESS_EQ:
            return "LILY_AST_EXPR_BINARY_KIND_LESS_EQ";
        case LILY_AST_EXPR_BINARY_KIND_LESS:
            return "LILY_AST_EXPR_BINARY_KIND_LESS";
        case LILY_AST_EXPR_BINARY_KIND_MOD:
            return "LILY_AST_EXPR_BINARY_KIND_MOD";
        case LILY_AST_EXPR_BINARY_KIND_MUL:
            return "LILY_AST_EXPR_BINARY_KIND_MUL";
        case LILY_AST_EXPR_BINARY_KIND_NOT_EQ:
            return "LILY_AST_EXPR_BINARY_KIND_NOT_EQ";
        case LILY_AST_EXPR_BINARY_KIND_OR:
            return "LILY_AST_EXPR_BINARY_KIND_OR";
        case LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT:
            return "LILY_AST_EXPR_BINARY_KIND_R_SHIFT";
        case LILY_AST_EXPR_BINARY_KIND_RANGE:
            return "LILY_AST_EXPR_BINARY_KIND_RANGE";
        case LILY_AST_EXPR_BINARY_KIND_SUB:
            return "LILY_AST_EXPR_BINARY_KIND_SUB";
        case LILY_AST_EXPR_BINARY_KIND_XOR:
            return "LILY_AST_EXPR_BINARY_KIND_XOR";
        case LILY_AST_EXPR_BINARY_KIND_LIST_HEAD:
            return "LILY_AST_EXPR_BINARY_KIND_LIST_HEAD";
        case LILY_AST_EXPR_BINARY_KIND_LIST_TAIL:
            return "LILY_AST_EXPR_BINARY_KIND_LIST_TAIL";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
IMPL_FOR_DEBUG(to_string, LilyAstExprBinary, const LilyAstExprBinary *self)
{
    return format("LilyAstExprBinary{{ kind = {s}, left = {Sr}, right = {Sr} }",
                  to_string__Debug__LilyAstExprBinaryKind(self->kind),
                  to_string__Debug__LilyAstExpr(self->left),
                  to_string__Debug__LilyAstExpr(self->right));
}
#endif

String *
to_string__LilyAstExprBinary(const LilyAstExprBinary *self)
{
    String *res = to_string__LilyAstExpr(self->left);

    switch (self->kind) {
        case LILY_AST_EXPR_BINARY_KIND_ADD:
            push_str__String(res, " + ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_AND:
            push_str__String(res, " and ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_ADD:
            push_str__String(res, " += ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            push_str__String(res, " &= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
            push_str__String(res, " <<= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            push_str__String(res, " |= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
            push_str__String(res, " >>= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_DIV:
            push_str__String(res, " /= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_EXP:
            push_str__String(res, " **= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MOD:
            push_str__String(res, " %= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MUL:
            push_str__String(res, " *= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_SUB:
            push_str__String(res, " -= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_XOR:
            push_str__String(res, " xor= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN:
            push_str__String(res, " = ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_BIT_AND:
            push_str__String(res, " & ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_BIT_OR:
            push_str__String(res, " | ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_CHAIN:
            push_str__String(res, " |> ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_DIV:
            push_str__String(res, " / ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_EQ:
            push_str__String(res, " == ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_EXP:
            push_str__String(res, " ** ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_GREATER_EQ:
            push_str__String(res, " >= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_GREATER:
            push_str__String(res, " > ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT:
            push_str__String(res, " << ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_LESS_EQ:
            push_str__String(res, " <= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_LESS:
            push_str__String(res, " < ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_MOD:
            push_str__String(res, " % ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_MUL:
            push_str__String(res, " * ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_NOT_EQ:
            push_str__String(res, " not= ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_OR:
            push_str__String(res, " or ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT:
            push_str__String(res, " >> ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_RANGE:
            push_str__String(res, " .. ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_SUB:
            push_str__String(res, " - ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_XOR:
            push_str__String(res, " xor ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_LIST_HEAD:
            push_str__String(res, " -> ");
            break;
        case LILY_AST_EXPR_BINARY_KIND_LIST_TAIL:
            push_str__String(res, " <- ");
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    String *right = to_string__LilyAstExpr(self->right);

    APPEND_AND_FREE(res, right);

    return res;
}

Uint8
to_precedence__LilyAstExprBinaryKind(enum LilyAstExprBinaryKind self)
{
    switch (self) {
        case LILY_AST_EXPR_BINARY_KIND_EXP:
            return 100;
        case LILY_AST_EXPR_BINARY_KIND_MUL:
        case LILY_AST_EXPR_BINARY_KIND_DIV:
        case LILY_AST_EXPR_BINARY_KIND_MOD:
            return 95;
        case LILY_AST_EXPR_BINARY_KIND_ADD:
        case LILY_AST_EXPR_BINARY_KIND_SUB:
            return 90;
        case LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT:
        case LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT:
            return 85;
        case LILY_AST_EXPR_BINARY_KIND_LESS:
        case LILY_AST_EXPR_BINARY_KIND_LESS_EQ:
        case LILY_AST_EXPR_BINARY_KIND_GREATER:
        case LILY_AST_EXPR_BINARY_KIND_GREATER_EQ:
            return 80;
        case LILY_AST_EXPR_BINARY_KIND_EQ:
        case LILY_AST_EXPR_BINARY_KIND_NOT_EQ:
            return 75;
        case LILY_AST_EXPR_BINARY_KIND_BIT_AND:
            return 70;
        case LILY_AST_EXPR_BINARY_KIND_XOR:
            return 65;
        case LILY_AST_EXPR_BINARY_KIND_BIT_OR:
            return 60;
        case LILY_AST_EXPR_BINARY_KIND_AND:
            return 55;
        case LILY_AST_EXPR_BINARY_KIND_OR:
            return 50;
        case LILY_AST_EXPR_BINARY_KIND_CHAIN:
            return 45;
        case LILY_AST_EXPR_BINARY_KIND_RANGE:
            return 40;
        case LILY_AST_EXPR_BINARY_KIND_LIST_HEAD:
        case LILY_AST_EXPR_BINARY_KIND_LIST_TAIL:
            return 35;
        default:
            return 30;
    }
}

enum LilyAstExprBinaryKind
from_token__LilyAstExprBinary(const LilyToken *token)
{
    switch (token->kind) {
        case LILY_TOKEN_KIND_PLUS:
            return LILY_AST_EXPR_BINARY_KIND_ADD;
        case LILY_TOKEN_KIND_KEYWORD_AND:
            return LILY_AST_EXPR_BINARY_KIND_AND;
        case LILY_TOKEN_KIND_PLUS_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_ADD;
        case LILY_TOKEN_KIND_AMPERSAND_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_AND;
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT;
        case LILY_TOKEN_KIND_BAR_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_OR;
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT;
        case LILY_TOKEN_KIND_SLASH_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_DIV;
        case LILY_TOKEN_KIND_STAR_STAR_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_EXP;
        case LILY_TOKEN_KIND_PERCENTAGE_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_MOD;
        case LILY_TOKEN_KIND_STAR_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_MUL;
        case LILY_TOKEN_KIND_MINUS_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_SUB;
        case LILY_TOKEN_KIND_XOR_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN_XOR;
        case LILY_TOKEN_KIND_EQ:
            return LILY_AST_EXPR_BINARY_KIND_ASSIGN;
        case LILY_TOKEN_KIND_AMPERSAND:
            return LILY_AST_EXPR_BINARY_KIND_BIT_AND;
        case LILY_TOKEN_KIND_BAR:
            return LILY_AST_EXPR_BINARY_KIND_BIT_OR;
        case LILY_TOKEN_KIND_BAR_R_SHIFT:
            return LILY_AST_EXPR_BINARY_KIND_CHAIN;
        case LILY_TOKEN_KIND_SLASH:
            return LILY_AST_EXPR_BINARY_KIND_DIV;
        case LILY_TOKEN_KIND_EQ_EQ:
            return LILY_AST_EXPR_BINARY_KIND_EQ;
        case LILY_TOKEN_KIND_STAR_STAR:
            return LILY_AST_EXPR_BINARY_KIND_EXP;
        case LILY_TOKEN_KIND_R_SHIFT_EQ:
            return LILY_AST_EXPR_BINARY_KIND_GREATER_EQ;
        case LILY_TOKEN_KIND_R_SHIFT:
            return LILY_AST_EXPR_BINARY_KIND_GREATER;
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT:
            return LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT;
        case LILY_TOKEN_KIND_L_SHIFT_EQ:
            return LILY_AST_EXPR_BINARY_KIND_LESS_EQ;
        case LILY_TOKEN_KIND_L_SHIFT:
            return LILY_AST_EXPR_BINARY_KIND_LESS;
        case LILY_TOKEN_KIND_PERCENTAGE:
            return LILY_AST_EXPR_BINARY_KIND_MOD;
        case LILY_TOKEN_KIND_STAR:
            return LILY_AST_EXPR_BINARY_KIND_MUL;
        case LILY_TOKEN_KIND_NOT_EQ:
            return LILY_AST_EXPR_BINARY_KIND_NOT_EQ;
        case LILY_TOKEN_KIND_KEYWORD_OR:
            return LILY_AST_EXPR_BINARY_KIND_OR;
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT:
            return LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT;
        case LILY_TOKEN_KIND_DOT_DOT:
            return LILY_AST_EXPR_BINARY_KIND_RANGE;
        case LILY_TOKEN_KIND_MINUS:
            return LILY_AST_EXPR_BINARY_KIND_SUB;
        case LILY_TOKEN_KIND_KEYWORD_XOR:
            return LILY_AST_EXPR_BINARY_KIND_XOR;
        case LILY_TOKEN_KIND_ARROW:
            return LILY_AST_EXPR_BINARY_KIND_LIST_HEAD;
        case LILY_TOKEN_KIND_INVERSE_ARROW:
            return LILY_AST_EXPR_BINARY_KIND_LIST_TAIL;
        default:
            UNREACHABLE(
              "impossible to convert this token in LilyAstExprBinaryKind");
    }
}

DESTRUCTOR(LilyAstExprBinary, const LilyAstExprBinary *self)
{
    FREE(LilyAstExpr, self->left);
    FREE(LilyAstExpr, self->right);
}
