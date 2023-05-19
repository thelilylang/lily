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

#include <core/lily/checked/operator_register.h>

#include <string.h>

LilyCheckedOperator *
search_operator__LilyCheckedOperatorRegister(
  const LilyCheckedOperatorRegister *self,
  String *name,
  LilyCheckedDataType *return_data_type,
  Vec *params)
{
    for (Usize i = 0; i < self->operators->len; ++i) {
        LilyCheckedOperator *operator= get__Vec(self->operators, i);

        if (!strcmp(name->buffer, operator->name->buffer)) {
            if (eq__LilyCheckedDataType(
                  return_data_type, operator->return_data_type)) {
                if (params->len != operator->params->len) {
                    continue;
                }

                bool is_match = true;

                for (Usize j = 0; j < operator->params->len; ++j) {
                    if (!eq__LilyCheckedDataType(get__Vec(operator->params, j),
                                                 get__Vec(params, j))) {
                        is_match = false;
                        break;
                    }
                }

                if (is_match) {
                    return operator;
                }
            }
        }
    }

    return NULL;
}

DESTRUCTOR(LilyCheckedOperatorRegister, const LilyCheckedOperatorRegister *self)
{
    FREE_BUFFER_ITEMS(
      self->operators->buffer, self->operators->len, LilyCheckedOperator);
    FREE(Vec, self->operators);
}
