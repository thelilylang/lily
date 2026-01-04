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

#ifndef LILY_CORE_CC_CI_RESOLVER_DATA_TYPE_H
#define LILY_CORE_CC_CI_RESOLVER_DATA_TYPE_H

#include <core/cc/ci/ast.h>
#include <core/cc/ci/result.h>

/**
 *
 * @brief Check if the data type is array compatible.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_array_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  bool allow_implicit_cast,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Check if the data type is integer compatible.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_integer_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  bool allow_implicit_cast,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Check if the data type is float compatible.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_float_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Check if the data type is numeric compatible.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_numeric_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  bool allow_implicit_cast,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Check if the data type is pointer compatible.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Check if the data type is void pointer compatible.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_void_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Check if the data type is struct or union compatible.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_struct_or_union_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Count the number of level of pointers.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 * @return The depth of pointer compatible data type.
 */
Usize
count_compatible_pointer_depth__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Check if the data type is compatible with void pointer data type.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
bool
is_compatible_with_void_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *left,
  const CIDataType *right,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Get integer rank.
 */
Uint8
get_integer_rank__CIResolverDataType(const CIResultFile *file,
                                     CIDataType *data_type,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params);

/**
 *
 * @brief Get wrapped data type in implicit pointer.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 * @return CIDataType* (&)
 */
CIDataType *
unwrap_implicit_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @brief Get fields from struct or union data type.
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 * @return const CIDeclStructFields*? (&)
 */
const CIDeclStructFields *
get_fields_from_data_type__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/**
 *
 * @param file const CIResultFile* (&)
 * @param data_type_access CIDataType* (&)
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 * @return CIDataType*
 */
CIDataType *
run__CIResolverDataType(const CIResultFile *file,
                        CIDataType *data_type,
                        const CIGenericParams *called_generic_params,
                        const CIGenericParams *decl_generic_params);

#endif // LILY_CORE_CC_CI_RESOLVER_DATA_TYPE_H
