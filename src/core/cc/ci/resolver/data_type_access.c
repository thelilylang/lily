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

#include <core/cc/ci/diagnostic/emit.h>
#include <core/cc/ci/resolver/data_type.h>
#include <core/cc/ci/resolver/data_type_access.h>

// Emit a located error on `data_type` and count it, without stopping the
// resolve: the caller carries on with a poisoned data type, so that a single
// run reports more than one error.
#define FAILED__CIResolverDataType(file, data_type, error_kind) \
    EMIT_ERROR__CI(&(file)->file_input,                         \
                   &(data_type)->location,                      \
                   NEW(CIError, error_kind),                    \
                   &(file)->file_analysis->count_error)

// Data type a resolve function returns after having emitted an error.
#define POISONED_DATA_TYPE__CIResolverDataType(data_type) \
    NEW(CIDataType,                                       \
        clone__Location(&(data_type)->location),          \
        CI_DATA_TYPE_KIND_ANY)

CIDataType *
run__CIResolverDataTypeAccess(const CIResultFile *file,
                              CIDataType *data_type_access,
                              enum CIExprBinaryKind binary_kind,
                              const CIGenericParams *called_generic_params,
                              const CIGenericParams *decl_generic_params)
{
    switch (binary_kind) {
        case CI_EXPR_BINARY_KIND_ARROW:
            if (!is_ptr_data_type__CIResolverDataType(file,
                                                      data_type_access,
                                                      called_generic_params,
                                                      decl_generic_params) &&
                count_compatible_pointer_depth__CIResolverDataType(
                  file,
                  data_type_access,
                  called_generic_params,
                  decl_generic_params) == 1) {
                FAILED__CIResolverDataType(
                  file,
                  data_type_access,
                  CI_ERROR_KIND_EXPECTED_POINTER_WITH_DEPTH_OF_ONE);

                return POISONED_DATA_TYPE__CIResolverDataType(data_type_access);
            }

            CIDataType *struct_or_union_dt =
              unwrap_implicit_ptr_data_type__CIResolverDataType(
                file,
                data_type_access,
                called_generic_params,
                decl_generic_params);

            if (!is_struct_or_union_data_type__CIResolverDataType(
                  file,
                  struct_or_union_dt,
                  called_generic_params,
                  decl_generic_params)) {
                FAILED__CIResolverDataType(
                  file,
                  data_type_access,
                  CI_ERROR_KIND_EXPECTED_STRUCT_OR_UNION_DATA_TYPE);

                return POISONED_DATA_TYPE__CIResolverDataType(data_type_access);
            }

            return ref__CIDataType(struct_or_union_dt);
        case CI_EXPR_BINARY_KIND_DOT:
            if (!is_struct_or_union_data_type__CIResolverDataType(
                  file,
                  data_type_access,
                  called_generic_params,
                  decl_generic_params)) {
                FAILED__CIResolverDataType(
                  file,
                  data_type_access,
                  CI_ERROR_KIND_EXPECTED_STRUCT_OR_UNION_DATA_TYPE);

                return POISONED_DATA_TYPE__CIResolverDataType(data_type_access);
            }

            return ref__CIDataType(data_type_access);
        default:
            UNREACHABLE("expected arrow or dot binary expression");
    }
}
