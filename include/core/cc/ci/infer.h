/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_CORE_CC_CI_INFER_H
#define LILY_CORE_CC_CI_INFER_H

#include <core/cc/ci/ast.h>
#include <core/cc/ci/result.h>

/**
 *
 * @brief Infer data type of binary expression.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
CIDataType *
infer_expr_binary_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Infer data type of identifier expression.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 * @return CIDataType*
 */
CIDataType *
infer_expr_identifier_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Infer data type of function call expression.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 * @return CIDataType*
 */
CIDataType *
infer_expr_function_call_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Infer data type of function call builtin expression.
 * @return CIDataType*
 */
CIDataType *
infer_expr_function_call_builtin_data_type__CIInfer(const CIExpr *expr);

/**
 *
 * @brief Infer data type of expression.
 * @return CIDataType*
 */
CIDataType *
infer_expr_data_type__CIInfer(const CIResultFile *file,
                              const CIExpr *expr,
                              const CIScopeID *current_scope_id,
                              const CIGenericParams *called_generic_params,
                              const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Evaluate expression as typeof.
 * @return CIDataType*
 */
CIDataType *
perform_typeof__CIInfer(const CIResultFile *file,
                        const CIExpr *expr,
                        const CIScopeID *current_scope_id,
                        const CIGenericParams *called_generic_params,
                        const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Evaluate expression as typeof_unqual.
 * @return CIDataType*
 */
CIDataType *
perform_typeof_unqual__CIInfer(const CIResultFile *file,
                               const CIExpr *expr,
                               const CIScopeID *current_scope_id,
                               const CIGenericParams *called_generic_params,
                               const CIGenericParams *decl_generic_params);

#endif // LILY_CORE_CC_CI_INFER_H
