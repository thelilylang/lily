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

#include <base/assert.h>

#include <core/cc/ci/resolver/data_type.h>

#include <stdio.h>
#include <stdlib.h>

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType*
static CIDataType *
resolve_generic_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @brief This function returns the given data type if it is not struct,
/// otherwise it attempts to resolve the struct data type.
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType* (&)
static CIDataType *
resolve_struct_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @brief This function returns the given data type if it is not typedef,
/// otherwise it attempts to resolve the typedef data type.
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType*
static CIDataType *
resolve_typedef_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
resolve_union_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

bool
is_array_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return true;
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = run__CIResolverDataType(
              file, data_type, called_generic_params, decl_generic_params);
            bool res = is_array_data_type__CIResolverDataType(
              file, resolved_dt, called_generic_params, decl_generic_params);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        default:
            return false;
    }
}

bool
is_integer_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  bool allow_implicit_cast,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = run__CIResolverDataType(
              file, data_type, called_generic_params, decl_generic_params);
            bool res = is_integer__CIDataType(resolved_dt);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        case CI_DATA_TYPE_KIND_ARRAY:
        case CI_DATA_TYPE_KIND_PTR: // via implicit cast
            return allow_implicit_cast;
        default:
            return is_integer__CIDataType(data_type);
    }
}

bool
is_float_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = run__CIResolverDataType(
              file, data_type, called_generic_params, decl_generic_params);
            bool res = is_float__CIDataType(resolved_dt);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        default:
            return is_float__CIDataType(data_type);
    }
}

bool
is_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = run__CIResolverDataType(
              file, data_type, called_generic_params, decl_generic_params);
            bool res = is_ptr_data_type__CIResolverDataType(
              file, resolved_dt, called_generic_params, decl_generic_params);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        case CI_DATA_TYPE_KIND_PTR:
        case CI_DATA_TYPE_KIND_ARRAY:
            return true;
        default:
            return false;
    }
}

bool
is_void_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *current_dt = (CIDataType *)data_type;

    while (is_ptr_data_type__CIResolverDataType(
      file, current_dt, called_generic_params, decl_generic_params)) {
        current_dt = unwrap_implicit_ptr_data_type__CIResolverDataType(
          file, current_dt, called_generic_params, decl_generic_params);
    }

    return current_dt->kind == CI_DATA_TYPE_KIND_VOID;
}

bool
is_struct_or_union_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
        case CI_DATA_TYPE_KIND_UNION:
            return true;
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = run__CIResolverDataType(
              file, data_type, called_generic_params, decl_generic_params);
            bool res = is_struct_or_union_data_type__CIResolverDataType(
              file, resolved_dt, called_generic_params, decl_generic_params);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        default:
            FAILED("expected struct or union");
    }
}

Usize
count_compatible_pointer_depth__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *current_dt = (CIDataType *)data_type;
    Usize depth_count = 0;

    while (current_dt &&
           is_ptr_data_type__CIResolverDataType(
             file, current_dt, called_generic_params, decl_generic_params)) {
        ++depth_count;
        current_dt = unwrap_implicit_ptr_data_type__CIResolverDataType(
          file, current_dt, called_generic_params, decl_generic_params);
    }

    return depth_count;
}

bool
is_compatible_with_void_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *left,
  const CIDataType *right,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    if (is_void_ptr_data_type__CIResolverDataType(
          file, left, called_generic_params, decl_generic_params) ||
        is_void_ptr_data_type__CIResolverDataType(
          file, right, called_generic_params, decl_generic_params)) {
        return count_compatible_pointer_depth__CIResolverDataType(
                 file, left, called_generic_params, decl_generic_params) ==
               count_compatible_pointer_depth__CIResolverDataType(
                 file, right, called_generic_params, decl_generic_params);
    }

    return false;
}

CIDataType *
unwrap_implicit_ptr_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT(is_ptr_data_type__CIResolverDataType(
      file, data_type, called_generic_params, decl_generic_params));

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDecl *decl = search_typedef_in_generic_context__CIResultFile(
              file,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params,
              called_generic_params,
              decl_generic_params);
            CIDataType *typedef_data_type =
              (CIDataType *)get_typedef_data_type__CIDecl(decl);

            ASSERT(is_ptr_data_type__CIResolverDataType(file,
                                                        typedef_data_type,
                                                        called_generic_params,
                                                        decl_generic_params));

            return unwrap_implicit_ptr_data_type__CIResolverDataType(
              file,
              typedef_data_type,
              called_generic_params,
              decl_generic_params);
        }
        case CI_DATA_TYPE_KIND_PTR:
            return data_type->ptr;
        case CI_DATA_TYPE_KIND_ARRAY:
            return data_type->array.data_type;
        default:
            UNREACHABLE("this kind of data type is not expected, expected "
                        "pointer compatible data type");
    }
}

CIDataType *
resolve_generic_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    bool has_generic = called_generic_params && decl_generic_params;

    if (has_generic) {
        Isize generic_params_index = find_generic__CIGenericParams(
          decl_generic_params, GET_PTR_RC(String, data_type->generic));

        if (generic_params_index == -1) {
            FAILED("generic params is not found");
        }

        return ref__CIDataType(
          get__Vec(called_generic_params->params, generic_params_index));
    }

    FAILED("expected generic param, to use generic data type");
}

CIDataType *
resolve_struct_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    if (data_type->struct_.name && data_type->struct_.generic_params) {
        CIDecl *struct_decl = search_struct_in_generic_context__CIResultFile(
          file,
          GET_PTR_RC(String, data_type->struct_.name),
          data_type->struct_.generic_params,
          called_generic_params,
          decl_generic_params);

        if (!struct_decl) {
            FAILED("struct is not found");
        }

        Rc *struct_name = NEW(Rc, clone__String(struct_decl->struct_gen.name));
        CIDataType *res = NEW_VARIANT(
          CIDataType, struct, NEW(CIDataTypeStruct, struct_name, NULL, NULL));

        FREE_RC(String, struct_name);

        return res;
    }

    return ref__CIDataType(data_type);
}

CIDataType *
resolve_typedef_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDecl *typedef_decl =
              search_typedef_in_generic_context__CIResultFile(
                file,
                GET_PTR_RC(String, data_type->typedef_.name),
                data_type->typedef_.generic_params,
                called_generic_params,
                decl_generic_params);

            switch (typedef_decl->kind) {
                case CI_DECL_KIND_TYPEDEF:
                    return resolve_typedef_data_type__CIResolverDataType(
                      file,
                      typedef_decl->typedef_.data_type,
                      called_generic_params,
                      decl_generic_params);
                case CI_DECL_KIND_TYPEDEF_GEN:
                    return resolve_typedef_data_type__CIResolverDataType(
                      file,
                      typedef_decl->typedef_gen.data_type,
                      called_generic_params,
                      decl_generic_params);
                default:
                    UNREACHABLE("expected typedef");
            }
        }
        default:
            return ref__CIDataType(data_type);
    }
}

CIDataType *
resolve_union_data_type__CIResolverDataType(
  const CIResultFile *file,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    if (data_type->union_.name && data_type->union_.generic_params) {
        CIDecl *union_decl = search_union_in_generic_context__CIResultFile(
          file,
          GET_PTR_RC(String, data_type->union_.name),
          data_type->union_.generic_params,
          called_generic_params,
          decl_generic_params);

        if (!union_decl) {
            FAILED("union declaration is not found");
        }

        Rc *union_name = NEW(Rc, clone__String(union_decl->union_gen.name));
        CIDataType *res = NEW_VARIANT(
          CIDataType, union, NEW(CIDataTypeUnion, union_name, NULL, NULL));

        FREE_RC(String, union_name);

        return res;
    }

    return ref__CIDataType(data_type);
}

const Vec *
get_fields_from_data_type__CIResolverDataType(
  const CIResultFile *file,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT(data_type->kind == CI_DATA_TYPE_KIND_STRUCT ||
           data_type->kind == CI_DATA_TYPE_KIND_UNION);

    const Vec *dt_fields = get_fields__CIDataType(data_type);

    if (dt_fields) {
        return dt_fields;
    }

    String *name = get_name__CIDataType(data_type);
    CIGenericParams *data_type_generic_params =
      (CIGenericParams *)get_generic_params__CIDataType(data_type);
    CIDecl *decl = NULL;

    ASSERT(name);

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF:
            UNREACHABLE("data type is resolved at this point");
        case CI_DATA_TYPE_KIND_STRUCT:
            decl = search_struct_in_generic_context__CIResultFile(
              file,
              name,
              data_type_generic_params,
              called_generic_params,
              decl_generic_params);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            decl = search_union_in_generic_context__CIResultFile(
              file,
              name,
              data_type_generic_params,
              called_generic_params,
              decl_generic_params);

            break;
        default:
            UNREACHABLE("unexpected data type");
    }

    if (!decl) {
        FAILED("struct or union is not found");
    }

    return get_fields__CIDecl(decl);
}

CIDataType *
run__CIResolverDataType(const CIResultFile *file,
                        CIDataType *data_type,
                        const CIGenericParams *called_generic_params,
                        const CIGenericParams *decl_generic_params)
{
    CIDataType *res = ref__CIDataType(data_type);
    CIDataType *prev_res = NULL;

    while (true) {
        prev_res = res;

        switch (res->kind) {
            case CI_DATA_TYPE_KIND_GENERIC:
                res = resolve_generic_data_type__CIResolverDataType(
                  file, res, called_generic_params, decl_generic_params);

                goto exit;
            case CI_DATA_TYPE_KIND_STRUCT:
                res = resolve_struct_data_type__CIResolverDataType(
                  file, res, called_generic_params, decl_generic_params);

                goto exit;
            case CI_DATA_TYPE_KIND_TYPEDEF:
                res = resolve_typedef_data_type__CIResolverDataType(
                  file, res, called_generic_params, decl_generic_params);

                break;
            case CI_DATA_TYPE_KIND_UNION:
                res = resolve_union_data_type__CIResolverDataType(
                  file, res, called_generic_params, decl_generic_params);

                goto exit;
            case CI_DATA_TYPE_KIND_PTR:
                res = NEW_VARIANT(CIDataType,
                                  ptr,
                                  run__CIResolverDataType(file,
                                                          res->ptr,
                                                          called_generic_params,
                                                          decl_generic_params));

                goto exit;
            default:
                res = ref__CIDataType(res);

                goto exit;
        }

        FREE(CIDataType, prev_res);
        prev_res = NULL;
    }

exit:
    FREE(CIDataType, prev_res);

    return res;
}
