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

#ifndef LILY_CORE_LILY_CHECKED_OPERATOR_REGISTER_H
#define LILY_CORE_LILY_CHECKED_OPERATOR_REGISTER_H

#include <core/lily/checked/operator.h>

typedef struct LilyCheckedOperatorRegister
{
    // NOTE:
    // is_global == true: Vec<LilyCheckedOperator*>*
    // is_global == false: Vec<LilyCheckedOperator* (&)>*
    Vec *operators;
    bool is_global;
} LilyCheckedOperatorRegister;

/**
 *
 * @brief Construct LilyCheckedOperatorRegister type.
 */
inline CONSTRUCTOR(LilyCheckedOperatorRegister,
                   LilyCheckedOperatorRegister,
                   bool is_global)
{
    return (LilyCheckedOperatorRegister){ .operators = NEW(Vec),
                                          .is_global = is_global };
}

/**
 *
 * @brief Add operator to the register.
 * @return Return 1 if is failing otherwise return 0.
 */
int
add_operator__LilyCheckedOperatorRegister(LilyCheckedOperatorRegister *self,
                                          LilyCheckedOperator *
                                          operator);

/**
 *
 * @brief Search operator in the register.
 */
LilyCheckedOperator *
search_operator__LilyCheckedOperatorRegister(
  const LilyCheckedOperatorRegister *self,
  String *name,
  Vec *params,
  LilyCheckedDataType *return_data_type);

/**
 *
 * @brief Free LilyCheckedOperatorRegister type.
 */
DESTRUCTOR(LilyCheckedOperatorRegister,
           const LilyCheckedOperatorRegister *self);

#endif // LILY_CORE_LILY_CHECKED_OPERATOR_REGISTER_H
