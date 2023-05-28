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

#include <base/assert.h>

#include <core/lily/checked/operator_register.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
add_operator__LilyCheckedOperatorRegister(LilyCheckedOperatorRegister *self,
                                          LilyCheckedOperator *
                                          operator)
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
collect_all_operator_with_name__LilyCheckedOperatorRegister(
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

LilyCheckedDataType *
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
            push__Vec(
              compiler_choice,
              get__Vec(
                CAST(LilyCheckedOperator *, get__Vec(operators, i))->signature,
                signature_index));
        }

        return NEW_VARIANT(
          LilyCheckedDataType, compiler_choice, location, compiler_choice);
    }

    return NULL;
}

DESTRUCTOR(LilyCheckedOperatorRegister, const LilyCheckedOperatorRegister *self)
{
    FREE_BUFFER_ITEMS(
      self->operators->buffer, self->operators->len, LilyCheckedOperator);
    FREE(Vec, self->operators);
}
