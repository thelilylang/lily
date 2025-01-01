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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_OPERATOR_REGISTER_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_OPERATOR_REGISTER_H

#include <core/lily/analysis/checked/operator.h>

typedef struct LilyCheckedOperatorRegister
{
    Vec *operators; // Vec<LilyCheckedOperator*>*
} LilyCheckedOperatorRegister;

/**
 *
 * @brief Construct LilyCheckedOperatorRegister type.
 */
inline CONSTRUCTOR(LilyCheckedOperatorRegister, LilyCheckedOperatorRegister)
{
    return (LilyCheckedOperatorRegister){ .operators = NEW(Vec) };
}

/**
 *
 * @brief Add operator to the register.
 * @return Return 1 if is failing otherwise return 0.
 */
int
add_operator__LilyCheckedOperatorRegister(LilyCheckedOperatorRegister *self,
                                          LilyCheckedOperator *operator);

/**
 *
 * @brief Search operator in the register.
 */
LilyCheckedOperator *
search_operator__LilyCheckedOperatorRegister(
  const LilyCheckedOperatorRegister *self,
  char *name,
  Vec *signature);

/**
 *
 * @brief Collect all operators with the given name.
 * @return Vec<LilyCheckedOperator* (&)>*
 */
Vec *
collect_all_operators__LilyCheckedOperatorRegister(
  const LilyCheckedOperatorRegister *self,
  char *name,
  Usize signature_len);

/**
 *
 * @brief Generate compiler choice according to the operator collection.
 * @param location Location of the expression.
 * @return Vec<LilyCheckedDataType* (&)>*?
 */
Vec *
generate_compiler_choice_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *location,
  Usize signature_index);

/**
 *
 * @brief Generate conditional compiler choice according to the operator
 * collection.
 * @param location Location of the expression.
 */
LilyCheckedDataType *
generate_conditional_compiler_choice_according_operator_collection__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *location);

/**
 *
 * @brief Typecheck binary operator.
 * @param defined_data_type LilyCheckedDataType* (&)?
 */
void
typecheck_binary__LilyCheckedOperatorRegister(
  Vec *operators,
  const Location *expr_location,
  LilyCheckedDataType *left,
  LilyCheckedDataType *right,
  LilyCheckedDataType **defined_data_type);

/**
 *
 * @brief Free LilyCheckedOperatorRegister type.
 */
DESTRUCTOR(LilyCheckedOperatorRegister,
           const LilyCheckedOperatorRegister *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_OPERATOR_REGISTER_H
