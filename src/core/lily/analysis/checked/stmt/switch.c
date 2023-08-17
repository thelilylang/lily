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

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/stmt/switch.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    int,
                    Int64 int_)
{
    LilyCheckedStmtSwitchCaseValue *self =
      lily_malloc(sizeof(LilyCheckedStmtSwitchCaseValue));

    self->kind = LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT;
    self->int_ = int_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    float,
                    Float64 float_)
{
    LilyCheckedStmtSwitchCaseValue *self =
      lily_malloc(sizeof(LilyCheckedStmtSwitchCaseValue));

    self->kind = LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT;
    self->float_ = float_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    uint,
                    Uint64 uint)
{
    LilyCheckedStmtSwitchCaseValue *self =
      lily_malloc(sizeof(LilyCheckedStmtSwitchCaseValue));

    self->kind = LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT;
    self->uint = uint;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    else)
{
    LilyCheckedStmtSwitchCaseValue *self =
      lily_malloc(sizeof(LilyCheckedStmtSwitchCaseValue));

    self->kind = LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtSwitchCaseValueKind,
               enum LilyCheckedStmtSwitchCaseValueKind self)
{
    switch (self) {
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT:
            return "LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT";
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT:
            return "LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT";
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT:
            return "LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT";
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE:
            return "LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

bool
eq__LilyCheckedStmtSwitchCaseValue(const LilyCheckedStmtSwitchCaseValue *self,
                                   const LilyCheckedStmtSwitchCaseValue *other)
{
    if (self->kind != other->kind) {
        if (self->kind == LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE ||
            other->kind == LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE) {
            return false;
        }

        UNREACHABLE("value must be the same");
    }

    switch (self->kind) {
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT:
            return self->int_ == other->int_;
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT:
            return self->float_ == other->float_;
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT:
            return self->uint == other->uint;
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE:
            return true;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtSwitchCaseValue,
               const LilyCheckedStmtSwitchCaseValue *self)
{
    switch (self->kind) {
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT:
            return format(
              "LilyCheckedStmtSwitchCase{{ kind = {s}, int_ = {d} }",
              to_string__Debug__LilyCheckedStmtSwitchCaseValueKind(self->kind),
              self->int_);
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT:
            return format(
              "LilyCheckedStmtSwitchCase{{ kind = {s}, float_ = {f} }",
              to_string__Debug__LilyCheckedStmtSwitchCaseValueKind(self->kind),
              self->float_);
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT:
            return format(
              "LilyCheckedStmtSwitchCase{{ kind = {s}, float_ = {u} }",
              to_string__Debug__LilyCheckedStmtSwitchCaseValueKind(self->kind),
              self->uint);
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE:
            return format(
              "LilyCheckedStmtSwitchCase{{ kind = {s} }",
              to_string__Debug__LilyCheckedStmtSwitchCaseValueKind(self->kind));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(LilyCheckedStmtSwitchCase *,
            LilyCheckedStmtSwitchCase,
            LilyCheckedStmtSwitchCaseValue *value,
            LilyCheckedExpr *cond,
            LilyCheckedBodyFunItem *body_item)
{
    LilyCheckedStmtSwitchCase *self =
      lily_malloc(sizeof(LilyCheckedStmtSwitchCase));

    self->values = init__Vec(1, value);
    self->body_item = body_item;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtSwitchCase,
               const LilyCheckedStmtSwitchCase *self)
{
    String *res = format__String("LilyCheckedStmtSwitchCase{{ values =");

    DEBUG_VEC_STR(self->values, res, LilyCheckedStmtSwitchCaseValue);

    {
        char *s =
          format(", body_item = {Sr} }",
                 to_string__Debug__LilyCheckedBodyFunItem(self->body_item));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtSwitchCase, LilyCheckedStmtSwitchCase *self)
{
    FREE_BUFFER_ITEMS(
      self->values->buffer, self->values->len, LilyCheckedStmtSwitchCaseValue);
    FREE(Vec, self->values);
    FREE(LilyCheckedBodyFunItem, self->body_item);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtSwitch,
               const LilyCheckedStmtSwitch *self)
{
    String *res =
      format__String("LilyCheckedStmtSwitch{{ switched_expr = {Sr}, cases =",
                     to_string__Debug__LilyCheckedExpr(self->switched_expr));

    DEBUG_VEC_STRING(self->cases, res, LilyCheckedStmtSwitchCase);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtSwitch, const LilyCheckedStmtSwitch *self)
{
    FREE(LilyCheckedExpr, self->switched_expr);
    FREE_BUFFER_ITEMS(
      self->cases->buffer, self->cases->len, LilyCheckedStmtSwitchCase);
    FREE(Vec, self->cases);
}
