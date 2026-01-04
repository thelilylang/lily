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

#include <base/assert.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/operator_register.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
binary_update_data_type_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  LilyCheckedDataType *data_type,
  Usize position);

static void
binary_update_return_data_type_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *expr_location,
  LilyCheckedDataType *left,
  LilyCheckedDataType *right,
  LilyCheckedDataType **return_data_type);

int
add_operator__LilyCheckedOperatorRegister(LilyCheckedOperatorRegister *self,
                                          LilyCheckedOperator *operator)
{
    // Look for duplicate operator
    LilyCheckedOperator *duplicate_op =
      search_operator__LilyCheckedOperatorRegister(
        self, operator->name->buffer, operator->signature);

    if (duplicate_op) {
        return 1;
    }

    push__Vec(self->operators, operator);

    return 0;
}

LilyCheckedOperator *
search_operator__LilyCheckedOperatorRegister(
  const LilyCheckedOperatorRegister *self,
  char *name,
  Vec *signature)
{
    for (Usize i = 0; i < self->operators->len; ++i) {
        LilyCheckedOperator *operator= get__Vec(self->operators, i);

        if (!strcmp(name, operator->name->buffer)) {
            if (signature->len != operator->signature->len) {
                continue;
            }

            bool is_match = true;

            for (Usize j = 0; j < operator->signature->len; ++j) {
                if (!eq__LilyCheckedDataType(get__Vec(operator->signature, j),
                                             get__Vec(signature, j))) {
                    is_match = false;
                    break;
                }
            }

            if (is_match) {
                return operator;
            }
        }
    }

    return NULL;
}

Vec *
collect_all_operators__LilyCheckedOperatorRegister(
  const LilyCheckedOperatorRegister *self,
  char *name,
  Usize signature_len)
{
    Vec *operators = NEW(Vec); // Vec<LilyCheckedOperator* (&)>*

    for (Usize i = 0; i < self->operators->len; ++i) {
        LilyCheckedOperator *operator= get__Vec(self->operators, i);

        if (!strcmp(name, operator->name->buffer) &&
            signature_len == operator->signature->len) {
            push__Vec(operators, operator);
        }
    }

    return operators;
}

Vec *
generate_compiler_choice_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *location,
  Usize signature_index)
{
    if (operators->len > 0) {
        ASSERT(
          signature_index <
          CAST(LilyCheckedOperator *, last__Vec(operators))->signature->len -
            1);

        Vec *compiler_choice = NEW(Vec); // Vec<LilyCheckedDataType* (&)>*

        for (Usize i = 0; i < operators->len; ++i) {
            add_choice__LilyCheckedDataType(
              compiler_choice,
              get__Vec(
                CAST(LilyCheckedOperator *, get__Vec(operators, i))->signature,
                signature_index));
        }

        return compiler_choice;
    }

    return NULL;
}

LilyCheckedDataType *
generate_conditional_compiler_choice_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *location)
{
    Vec *choices = NEW(Vec); // Vec<LilyCheckedDataType* (&)>*
    Vec *conds = NEW(Vec);   // Vec<LilyCheckedDataTypeCondition*>*

    LilyCheckedDataTypeConditionalCompilerChoice cond_cc =
      NEW(LilyCheckedDataTypeConditionalCompilerChoice, choices, conds);

    for (Usize i = 0; i < operators->len; ++i) {
        LilyCheckedOperator *operator= get__Vec(operators, i);

        ASSERT(operator->signature->len> 0);

        LilyCheckedDataTypeCondition *condition =
          NEW(LilyCheckedDataTypeCondition,
              NEW(Vec),
              add_choice__LilyCheckedDataTypeConditionalCompilerChoice(
                &cond_cc, last__Vec(operator->signature)));

        for (Usize j = 0; j < operator->signature->len - 1; ++j) {
            push__Vec(
              condition->params,
              ref__LilyCheckedDataType(get__Vec(operator->signature, j)));
        }

        push__Vec(conds, condition);
    }

    return NEW_VARIANT(
      LilyCheckedDataType, conditional_compiler_choice, location, cond_cc);
}

#define FILTER_OPERATOR(dt_op, dt)                                             \
    switch (dt->kind) {                                                        \
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:                     \
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:                              \
            break;                                                             \
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE: {                    \
            bool is_match = false;                                             \
                                                                               \
            for (Usize j = 0; j < dt->compiler_choice->len; ++j) {             \
                if (eq__LilyCheckedDataType(                                   \
                      dt_op, get__Vec(dt->compiler_choice, j))) {              \
                    is_match = true;                                           \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            if (!is_match) {                                                   \
                remove__Vec(operators, i);                                     \
                continue;                                                      \
            }                                                                  \
                                                                               \
            break;                                                             \
        }                                                                      \
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {        \
            bool is_match = false;                                             \
                                                                               \
            for (Usize j = 0;                                                  \
                 j < dt->conditional_compiler_choice.choices->len;             \
                 ++j) {                                                        \
                if (eq__LilyCheckedDataType(                                   \
                      dt_op,                                                   \
                      get__Vec(dt->conditional_compiler_choice.choices, j))) { \
                    is_match = true;                                           \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            if (!is_match) {                                                   \
                remove__Vec(operators, i);                                     \
                continue;                                                      \
            }                                                                  \
                                                                               \
            break;                                                             \
        }                                                                      \
        default:                                                               \
            if (!eq__LilyCheckedDataType(dt_op, dt)) {                         \
                remove__Vec(operators, i);                                     \
                continue;                                                      \
            }                                                                  \
    }

#define BINARY_UPDATE_DATA_TYPE()                                          \
    if (position == 0 || position == 1) {                                  \
        Vec *choices = NEW(Vec); /* Vec<LilyCheckedDataType* (&)>* */      \
                                                                           \
        for (Usize i = 0; i < operators->len; ++i) {                       \
            add_choice__LilyCheckedDataType(                               \
              choices,                                                     \
              get__Vec(CAST(LilyCheckedOperator *, get__Vec(operators, i)) \
                         ->signature,                                      \
                       position));                                         \
        }                                                                  \
                                                                           \
        data_type->kind = LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE;     \
        data_type->compiler_choice = choices;                              \
    }

void
binary_update_data_type_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  LilyCheckedDataType *data_type,
  Usize position)
{
    if (data_type) {
        switch (data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
                BINARY_UPDATE_DATA_TYPE();
                break;
            default:
                break;
        }
    } else {
        BINARY_UPDATE_DATA_TYPE();
    }
}

void
binary_update_return_data_type_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *expr_location,
  LilyCheckedDataType *left,
  LilyCheckedDataType *right,
  LilyCheckedDataType **return_data_type)
{
    if (is_compiler_defined__LilyCheckedDataType(left) ||
        is_compiler_defined__LilyCheckedDataType(right)) {
        Vec *choices = NEW(Vec); /* Vec<LilyCheckedDataType* (&)>* */
        Vec *conds = NEW(Vec);   /* Vec<LilyCheckedDataTypeCondition*>* */
        LilyCheckedDataTypeConditionalCompilerChoice cond_cc =
          NEW(LilyCheckedDataTypeConditionalCompilerChoice, choices, conds);

        for (Usize i = 0; i < operators->len; ++i) {
            LilyCheckedOperator *operator= get__Vec(operators, i);

            push__Vec(
              conds,
              NEW(LilyCheckedDataTypeCondition,
                  init__Vec(
                    2,
                    ref__LilyCheckedDataType(get__Vec(operator->signature, 0)),
                    ref__LilyCheckedDataType(get__Vec(operator->signature, 1))),
                  add_choice__LilyCheckedDataTypeConditionalCompilerChoice(
                    &cond_cc, last__Vec(operator->signature))));
        }

        LilyCheckedDataType *update_return_data_type =
          NEW_VARIANT(LilyCheckedDataType,
                      conditional_compiler_choice,
                      expr_location,
                      cond_cc);

        if (*return_data_type) {
            update_data_type__LilyCheckedDataType(*return_data_type,
                                                  update_return_data_type);

            if (!eq__LilyCheckedDataType(update_return_data_type,
                                         *return_data_type)) {
                FAILED("return data type doesn't match");
            }

            FREE(LilyCheckedDataType, update_return_data_type);
        } else {
            *return_data_type = update_return_data_type;
        }
    } else {
        if (operators->len == 1) {
            if (*return_data_type) {
                update_data_type__LilyCheckedDataType(
                  *return_data_type,
                  last__Vec(CAST(LilyCheckedOperator *, last__Vec(operators))
                              ->signature));
            } else {
                *return_data_type = ref__LilyCheckedDataType(
                  last__Vec(CAST(LilyCheckedOperator *, last__Vec(operators))
                              ->signature));
            }
        } else {
            TODO("...");
        }
    }
}

void
typecheck_binary__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *expr_location,
  LilyCheckedDataType *left,
  LilyCheckedDataType *right,
  LilyCheckedDataType **defined_data_type)
{
    // 1. Filter operators
    for (Usize i = 0; i < operators->len;) {
        LilyCheckedOperator *operator= get__Vec(operators, i);
        LilyCheckedDataType *left_op_data_type =
          get__Vec(operator->signature, 0);

        FILTER_OPERATOR(left_op_data_type, left);

        LilyCheckedDataType *right_op_data_type =
          get__Vec(operator->signature, 1);

        FILTER_OPERATOR(right_op_data_type, right);

        if (*defined_data_type) {
            LilyCheckedDataType *return_op_data_type =
              last__Vec(operator->signature);

            FILTER_OPERATOR(return_op_data_type, (*defined_data_type));
        }

        ++i;
    }

    if (operators->len == 0) {
        FAILED("no operator signature matched");
    }

    // 2. Update data type
    binary_update_data_type_according_operator_collection__LilyCheckedOperatorRegister(
      operators, left, 0);
    binary_update_data_type_according_operator_collection__LilyCheckedOperatorRegister(
      operators, right, 1);
    binary_update_return_data_type_according_operator_collection__LilyCheckedOperatorRegister(
      operators, expr_location, left, right, defined_data_type);
}

DESTRUCTOR(LilyCheckedOperatorRegister, const LilyCheckedOperatorRegister *self)
{
    FREE_BUFFER_ITEMS(
      self->operators->buffer, self->operators->len, LilyCheckedOperator);
    FREE(Vec, self->operators);
}
