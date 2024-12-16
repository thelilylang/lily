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

#include <core/cc/ci/infer.h>
#include <core/cc/ci/resolver/data_type.h>
#include <core/cc/ci/resolver/data_type_access.h>

#include <stdio.h>
#include <stdlib.h>

/// @return CIDataType*
static CIDataType *
perform_implicit_cast_on_array__CIInfer(const CIResultFile *file,
                                        CIDataType *data_type);

/// @param current_infer_dt CIDataType*?* (&)
static void
infer_expr_access_data_type_array_access__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const Vec *fields,
  CIDataType **current_infer_dt,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param current_expr_access CIExpr* (&)* (&)
/// @param current_fields Vec<CIDeclStructField*>* (&)* (&)
/// @param current_infer_dt CIDataType*?* (&)
static void
infer_expr_access_data_type_arrow_or_dot__CIInfer(
  const CIResultFile *file,
  CIExpr **current_expr_access,
  Vec **current_fields,
  CIDataType **current_infer_dt,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param current_infer_dt CIDataType*?* (&)
static void
infer_expr_access_data_type_identifier__CIInfer(const CIResultFile *file,
                                                const CIExpr *expr_access,
                                                const Vec *fields,
                                                CIDataType **current_infer_dt);

/// @param fields const Vec<CIDeclStructField*>* (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType*
static CIDataType *
infer_expr_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const Vec *fields,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
infer_expr_dot_binary_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExprBinary *dot_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
infer_expr_arrow_binary_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExprBinary *arrow_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

static CIDataType *
infer_expr_literal_data_type__CIInfer(const CIResultFile *file,
                                      const CIExprLiteral *literal);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
infer_expr_unary_data_type__CIInfer(
  const CIResultFile *file,
  const CIExprUnary *unary,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

CIDataType *
perform_implicit_cast_on_array__CIInfer(const CIResultFile *file,
                                        CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return NEW_VARIANT(CIDataType,
                               ptr,
                               perform_implicit_cast_on_array__CIInfer(
                                 file, data_type->array.data_type));
        default:
            return ref__CIDataType(data_type);
    }
}

void
infer_expr_access_data_type_array_access__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const Vec *fields,
  CIDataType **current_infer_dt,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *array_dt =
      infer_expr_access_data_type__CIInfer(file,
                                           expr_access->array_access.array,
                                           fields,
                                           current_scope_id,
                                           called_generic_params,
                                           decl_generic_params);

    if (*current_infer_dt) {
        FREE(CIDataType, *current_infer_dt);
    }

    if (is_ptr_data_type__CIResolverDataType(
          file, array_dt, called_generic_params, decl_generic_params)) {
        *current_infer_dt =
          ref__CIDataType(unwrap_implicit_ptr_data_type__CIResolverDataType(
            file, array_dt, called_generic_params, decl_generic_params));
    } else {
        *current_infer_dt = NULL;
        FAILED("expected array or pointer");
    }

    FREE(CIDataType, array_dt);
}

void
infer_expr_access_data_type_arrow_or_dot__CIInfer(
  const CIResultFile *file,
  CIExpr **current_expr_access,
  Vec **current_fields,
  CIDataType **current_infer_dt,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT((*current_expr_access)->binary.left->kind ==
           CI_EXPR_KIND_IDENTIFIER);

    infer_expr_access_data_type_identifier__CIInfer(
      file,
      (*current_expr_access)->binary.left,
      *current_fields,
      current_infer_dt);

    CIDataType *resolved_field_data_type = run__CIResolverDataType(
      file, *current_infer_dt, called_generic_params, decl_generic_params);
    CIDataType *resolved_access_data_type =
      run__CIResolverDataTypeAccess(file,
                                    resolved_field_data_type,
                                    (*current_expr_access)->binary.kind,
                                    called_generic_params,
                                    decl_generic_params);

    FREE(CIDataType, resolved_field_data_type);

    if (*current_infer_dt) {
        FREE(CIDataType, *current_infer_dt);
    }

    *current_infer_dt = resolved_access_data_type;

    if (is_struct_or_union_data_type__CIResolverDataType(file,
                                                         *current_infer_dt,
                                                         called_generic_params,
                                                         decl_generic_params)) {
        *current_fields = (Vec *)get_fields_from_data_type__CIResolverDataType(
          file, *current_infer_dt, called_generic_params, decl_generic_params);
    }

    *current_expr_access = (*current_expr_access)->binary.right;
}

void
infer_expr_access_data_type_identifier__CIInfer(const CIResultFile *file,
                                                const CIExpr *expr_access,
                                                const Vec *fields,
                                                CIDataType **current_infer_dt)
{
    if (*current_infer_dt) {
        FREE(CIDataType, *current_infer_dt);
    }

    CIDataType *field_data_type = get__CIDeclStructField(
      fields, GET_PTR_RC(String, expr_access->identifier.value));

    if (!field_data_type) {
        FAILED("the field is not found");
    }

    *current_infer_dt =
      field_data_type ? ref__CIDataType(field_data_type) : NULL;
}

CIDataType *
infer_expr_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const Vec *fields,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIExpr *current_expr_access = (CIExpr *)expr_access;
    Vec *current_fields = (Vec *)fields;
    CIDataType *current_infer_dt = NULL; // CIDataType*?

    while (true) {
        switch (current_expr_access->kind) {
            case CI_EXPR_KIND_ARRAY_ACCESS:
                infer_expr_access_data_type_array_access__CIInfer(
                  file,
                  current_expr_access,
                  current_fields,
                  &current_infer_dt,
                  current_scope_id,
                  called_generic_params,
                  decl_generic_params);

                goto exit;
            case CI_EXPR_KIND_BINARY:
                switch (expr_access->binary.kind) {
                    case CI_EXPR_BINARY_KIND_ARROW:
                    case CI_EXPR_BINARY_KIND_DOT:
                        infer_expr_access_data_type_arrow_or_dot__CIInfer(
                          file,
                          &current_expr_access,
                          &current_fields,
                          &current_infer_dt,
                          called_generic_params,
                          decl_generic_params);

                        break;
                    default:
                        goto exit;
                }

                break;
            case CI_EXPR_KIND_IDENTIFIER:
                infer_expr_access_data_type_identifier__CIInfer(
                  file, current_expr_access, current_fields, &current_infer_dt);

                goto exit;
            default:
                goto exit;
        }
    }

exit:
    if (!current_infer_dt) {
        FAILED("there are something wrong during the inference of path");
    }

    return current_infer_dt;
}

CIDataType *
infer_expr_dot_binary_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExprBinary *dot_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *left_dt = infer_expr_data_type__CIInfer(file,
                                                        dot_access->left,
                                                        current_scope_id,
                                                        called_generic_params,
                                                        decl_generic_params);
    CIDataType *resolved_left_dt =
      run__CIResolverDataTypeAccess(file,
                                    left_dt,
                                    CI_EXPR_BINARY_KIND_DOT,
                                    called_generic_params,
                                    decl_generic_params);
    CIDataType *struct_or_union_dt = run__CIResolverDataType(
      file, resolved_left_dt, called_generic_params, decl_generic_params);
    const Vec *struct_or_union_fields =
      get_fields_from_data_type__CIResolverDataType(
        file, struct_or_union_dt, called_generic_params, decl_generic_params);

    FREE(CIDataType, left_dt);
    FREE(CIDataType, resolved_left_dt);
    FREE(CIDataType, struct_or_union_dt);

    return infer_expr_access_data_type__CIInfer(file,
                                                dot_access->right,
                                                struct_or_union_fields,
                                                current_scope_id,
                                                called_generic_params,
                                                decl_generic_params);
}

CIDataType *
infer_expr_arrow_binary_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExprBinary *arrow_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *left_dt = infer_expr_data_type__CIInfer(file,
                                                        arrow_access->left,
                                                        current_scope_id,
                                                        called_generic_params,
                                                        decl_generic_params);
    CIDataType *resolved_left_dt =
      run__CIResolverDataTypeAccess(file,
                                    left_dt,
                                    CI_EXPR_BINARY_KIND_ARROW,
                                    called_generic_params,
                                    decl_generic_params);
    CIDataType *struct_or_union_dt = run__CIResolverDataType(
      file, resolved_left_dt, called_generic_params, decl_generic_params);
    const Vec *struct_or_union_fields =
      get_fields_from_data_type__CIResolverDataType(
        file, struct_or_union_dt, called_generic_params, decl_generic_params);

    FREE(CIDataType, left_dt);
    FREE(CIDataType, resolved_left_dt);
    FREE(CIDataType, struct_or_union_dt);

    return infer_expr_access_data_type__CIInfer(file,
                                                arrow_access->right,
                                                struct_or_union_fields,
                                                current_scope_id,
                                                called_generic_params,
                                                decl_generic_params);
}

CIDataType *
infer_expr_literal_data_type__CIInfer(const CIResultFile *file,
                                      const CIExprLiteral *literal)
{
    switch (literal->kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_BOOL);
        case CI_EXPR_LITERAL_KIND_CHAR:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR);
        case CI_EXPR_LITERAL_KIND_FLOAT:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT);
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
        case CI_EXPR_LITERAL_KIND_STRING: {
            CIDataType *string_dt = NEW_VARIANT(
              CIDataType,
              array,
              NEW_VARIANT(CIDataTypeArray,
                          sized,
                          NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR),
                          NULL,
                          GET_PTR_RC(String, literal->string)->len));

            set_context__CIDataType(string_dt, CI_DATA_TYPE_CONTEXT_STACK);

            return string_dt;
        }
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_INT);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIDataType *
infer_expr_unary_data_type__CIInfer(
  const CIResultFile *file,
  const CIExprUnary *unary,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *unary_right_expr_data_type =
      infer_expr_data_type__CIInfer(file,
                                    unary->expr,
                                    current_scope_id,
                                    called_generic_params,
                                    decl_generic_params);

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_REF:
            return wrap_ptr__CIDataType(unary_right_expr_data_type,
                                        CI_DATA_TYPE_CONTEXT_NONE);
        case CI_EXPR_UNARY_KIND_DEREFERENCE: {
            if (is_ptr_data_type__CIResolverDataType(file,
                                                     unary_right_expr_data_type,
                                                     called_generic_params,
                                                     decl_generic_params)) {
                TODO("get the type after dereferencing");
            }

            return unary_right_expr_data_type;
        }
        default:
            return unary_right_expr_data_type;
    }
}

CIDataType *
infer_expr_data_type__CIInfer(const CIResultFile *file,
                              const CIExpr *expr,
                              const CIScopeID *current_scope_id,
                              const CIGenericParams *called_generic_params,
                              const CIGenericParams *decl_generic_params)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            // FIXME: Get the size_t data type for 32 bits.
            return NEW(CIDataType,
                       CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT); // size_t
        case CI_EXPR_KIND_ARRAY: {
            if (expr->array.array->len == 0) {
                FAILED("cannot infer on array");
            }

            Vec *expr_dts = NEW(Vec); // Vec<CIDataType*>*
            bool is_struct = false;

            for (Usize i = 0; i < expr->array.array->len; ++i) {
                CIExpr *expr_item = get__Vec(expr->array.array, i);
                CIDataType *expr_item_dt =
                  infer_expr_data_type__CIInfer(file,
                                                expr_item,
                                                current_scope_id,
                                                called_generic_params,
                                                decl_generic_params);
                CIDataType *last_dt = safe_last__Vec(expr_dts);

                if (last_dt && !is_struct) {
                    if (!eq__CIDataType(expr_item_dt, last_dt)) {
                        is_struct = true;
                    }
                }

                push__Vec(expr_dts, expr_item_dt);
            }

            if (is_struct) {
                Vec *fields = expr_dts;

                for (Usize i = 0; i < fields->len; ++i) {
                    CIDataType *expr_dt = get__Vec(fields, i);

                    replace__Vec(
                      fields, i, NEW(CIDeclStructField, NULL, expr_dt, 0));
                }

                return NEW_VARIANT(CIDataType,
                                   struct,
                                   NEW(CIDataTypeStruct, NULL, NULL, fields));
            }

            CIDataType *array_data_type = get__Vec(expr_dts, 0);
            Usize expr_dts_len = expr_dts->len;

            // NOTE: We don't free the first item contains in this buffer.
            FREE_BUFFER_ITEMS(
              (expr_dts->buffer + 1), expr_dts->len - 1, CIDataType);
            FREE(Vec, expr_dts);

            return NEW_VARIANT(
              CIDataType,
              array,
              NEW_VARIANT(
                CIDataTypeArray, sized, array_data_type, NULL, expr_dts_len));
        }
        case CI_EXPR_KIND_ARRAY_ACCESS: {
            CIDataType *array_data_type =
              infer_expr_data_type__CIInfer(file,
                                            expr->array_access.array,
                                            current_scope_id,
                                            called_generic_params,
                                            decl_generic_params);
            CIDataType *ptr_array_data_type =
              perform_implicit_cast_on_array__CIInfer(file, array_data_type);

            switch (ptr_array_data_type->kind) {
                case CI_DATA_TYPE_KIND_PTR: {
                    CIDataType *res = ref__CIDataType(ptr_array_data_type->ptr);

                    FREE(CIDataType, ptr_array_data_type);
                    FREE(CIDataType, array_data_type);

                    return res;
                }
                default:
                    FAILED("expected pointer after implicit cast on array");
            }
        }
        case CI_EXPR_KIND_BINARY: {
            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_DOT:
                    return infer_expr_dot_binary_access_data_type__CIInfer(
                      file,
                      &expr->binary,
                      current_scope_id,
                      called_generic_params,
                      decl_generic_params);
                case CI_EXPR_BINARY_KIND_ARROW:
                    return infer_expr_arrow_binary_access_data_type__CIInfer(
                      file,
                      &expr->binary,
                      current_scope_id,
                      called_generic_params,
                      decl_generic_params);
                default:
                    break;
            }

            CIDataType *left_dt =
              infer_expr_data_type__CIInfer(file,
                                            expr->binary.left,
                                            current_scope_id,
                                            called_generic_params,
                                            decl_generic_params);

            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_ASSIGN:
                    return left_dt;
                default:
                    break;
            }

            CIDataType *right_dt =
              infer_expr_data_type__CIInfer(file,
                                            expr->binary.right,
                                            current_scope_id,
                                            called_generic_params,
                                            decl_generic_params);

            bool left_dt_is_integer = is_integer_data_type__CIResolverDataType(
              file, left_dt, true, called_generic_params, decl_generic_params);
            bool left_dt_is_float = is_float_data_type__CIResolverDataType(
              file, left_dt, called_generic_params, decl_generic_params);
            bool right_dt_is_integer = is_integer_data_type__CIResolverDataType(
              file, right_dt, true, called_generic_params, decl_generic_params);
            bool right_dt_is_float = is_float_data_type__CIResolverDataType(
              file, right_dt, called_generic_params, decl_generic_params);

            FREE(CIDataType, left_dt);
            FREE(CIDataType, right_dt);

            if (left_dt_is_float || right_dt_is_float) {
                return NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT);
            } else if (left_dt_is_integer && right_dt_is_integer) {
                return NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
            }

            FAILED("this kind of operation is not possible");
        }
        case CI_EXPR_KIND_CAST:
            return ref__CIDataType(expr->cast.data_type);
        case CI_EXPR_KIND_DATA_TYPE:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_TYPE_INFO);
        case CI_EXPR_KIND_FUNCTION_CALL: {
            CIDecl *function_decl =
              search_function_in_generic_context__CIResultFile(
                file,
                GET_PTR_RC(String, expr->function_call.identifier),
                expr->function_call.generic_params,
                called_generic_params,
                decl_generic_params);

            ASSERT(function_decl->kind & CI_DECL_KIND_FUNCTION);

            if (function_decl) {
                return ref__CIDataType(
                  (CIDataType *)get_return_data_type__CIDecl(function_decl));
            }

            return NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
        }
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN: {
            CIBuiltin *builtin = get_ref__CIBuiltin();
            const CIBuiltinFunction *builtin_function =
              get_builtin_function__CIBuiltin(builtin,
                                              expr->function_call_builtin.id);

            return ref__CIDataType(builtin_function->return_data_type);
        }
        case CI_EXPR_KIND_GROUPING:
            return infer_expr_data_type__CIInfer(file,
                                                 expr->grouping,
                                                 current_scope_id,
                                                 called_generic_params,
                                                 decl_generic_params);
        case CI_EXPR_KIND_IDENTIFIER:
            switch (expr->identifier.id.kind) {
                // NOTE: In the case that the identifier was not found in the
                // parser, the only case that could not be found in the parser
                // is the label.
                //
                // e.g.
                //
                // goto b;
                //
                // b: {}
                case CI_EXPR_IDENTIFIER_ID_KIND_NONE: {
                    CIScope *local_current_scope =
                      get_scope_from_id__CIResultFile(file, current_scope_id);

                    ASSERT(local_current_scope);

                    const CILabelID *label = search_label__CIScope(
                      local_current_scope,
                      GET_PTR_RC(String, expr->identifier.value));

                    if (label) {
                        ((CIExpr *)expr)->identifier.id =
                          NEW_VARIANT(CIExprIdentifierID, label, label);

                        goto label;
                    }

                    FAILED("cannot infer on unknown identifier");

                    break;
                }
                case CI_EXPR_IDENTIFIER_ID_KIND_ENUM_VARIANT:
                    TODO("enum variant");
                case CI_EXPR_IDENTIFIER_ID_KIND_FUNCTION: {
                    // TODO: Call generic function is not yet implemented
                    CIDecl *decl = get_function_from_id__CIResultFile(
                      file, expr->identifier.id.function);

                    ASSERT(decl);

                    CIDataType *function_return_data_type =
                      (CIDataType *)get_return_data_type__CIDecl(decl);
                    CIDataType *resolved_function_return_data_type =
                      run__CIResolverDataType(file,
                                              function_return_data_type,
                                              called_generic_params,
                                              decl_generic_params);
                    const Vec *function_params =
                      get_function_params__CIDecl(decl);
                    // NOTE: Maybe use ref count instead of.
                    Vec *cloned_function_params =
                      function_params
                        ? clone_params__CIDeclFunctionParam(function_params)
                        : NULL;

                    return NEW_VARIANT(CIDataType,
                                       function,
                                       NEW(CIDataTypeFunction,
                                           NULL,
                                           cloned_function_params,
                                           resolved_function_return_data_type,
                                           NULL));
                }
                case CI_EXPR_IDENTIFIER_ID_KIND_LABEL:
                label:
                    TODO("label");
                case CI_EXPR_IDENTIFIER_ID_KIND_VARIABLE: {
                    CIDecl *decl = get_variable_from_id__CIResultFile(
                      file, expr->identifier.id.variable);

                    ASSERT(decl);

                    return ref__CIDataType(decl->variable.data_type);
                }
                default:
                    UNREACHABLE("unknown variant");
            }
        case CI_EXPR_KIND_LITERAL:
            return infer_expr_literal_data_type__CIInfer(file, &expr->literal);
        case CI_EXPR_KIND_NULLPTR:
            TODO("infer nullptr");
        case CI_EXPR_KIND_SIZEOF:
            // FIXME: Get the size_t data type for 32 bits.
            return NEW(CIDataType,
                       CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT); // size_t
        case CI_EXPR_KIND_STRUCT_CALL: {
            Vec *fields = NEW(Vec); // Vec<CIDeclStructField*>*

            for (Usize i = 0; i < expr->struct_call.fields->len; ++i) {
                CIExprStructFieldCall *field =
                  get__Vec(expr->struct_call.fields, i);

                push__Vec(
                  fields,
                  NEW(CIDeclStructField,
                      NULL,
                      infer_expr_data_type__CIInfer(file,
                                                    field->value,
                                                    current_scope_id,
                                                    called_generic_params,
                                                    decl_generic_params),
                      0));
            }

            return NEW_VARIANT(
              CIDataType, struct, NEW(CIDataTypeStruct, NULL, NULL, fields));
        }
        case CI_EXPR_KIND_TERNARY:
            // NOTE: We only need to infer one condition branch.
            return infer_expr_data_type__CIInfer(file,
                                                 expr->ternary.if_,
                                                 current_scope_id,
                                                 called_generic_params,
                                                 decl_generic_params);
        case CI_EXPR_KIND_UNARY:
            return infer_expr_unary_data_type__CIInfer(file,
                                                       &expr->unary,
                                                       current_scope_id,
                                                       called_generic_params,
                                                       decl_generic_params);
        default:
            UNREACHABLE("unknown variant");
    }
}
