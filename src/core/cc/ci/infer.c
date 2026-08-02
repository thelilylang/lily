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

#include <core/cc/ci/diagnostic/emit.h>
#include <core/cc/ci/infer.h>

// Emit a located error on `node` and count it, without stopping the inference:
// the caller carries on with a poisoned data type so that a single pass reports
// more than one error.
#define FAILED__CIInfer(file, node, error_kind)                        \
    EMIT_ERROR__CI(                                                    \
      get_file_from_location__CIResultFile((file), &(node)->location), \
      &(node)->location,                                               \
      NEW(CIError, error_kind),                                        \
      &(file)->file_analysis->count_error)

// Data type an inference function returns after having emitted an error.
#define POISONED_DATA_TYPE__CIInfer(node) \
    NEW(CIDataType, clone__Location(&(node)->location), CI_DATA_TYPE_KIND_ANY)
#include <core/cc/ci/resolver/data_type.h>
#include <core/cc/ci/resolver/data_type_access.h>

/// @param current_infer_dt CIDataType*?* (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
infer_expr_access_data_type_array_access__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const CIDeclStructFields *fields,
  CIDataType **current_infer_dt,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param current_expr_access CIExpr* (&)* (&)
/// @param current_fields CIDeclStructFields* (&)* (&)
/// @param current_infer_dt CIDataType*?* (&)
static void
infer_expr_access_data_type_arrow_or_dot__CIInfer(
  const CIResultFile *file,
  CIExpr **current_expr_access,
  CIDeclStructFields **current_fields,
  CIDataType **current_infer_dt,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param current_infer_dt CIDataType*?* (&)
static void
infer_expr_access_data_type_identifier__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const CIDeclStructFields *fields,
  CIDataType **current_infer_dt);

/// @param fields const CIDeclStructFields (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType*
static CIDataType *
infer_expr_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const CIDeclStructFields *fields,
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

void
infer_expr_access_data_type_array_access__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const CIDeclStructFields *fields,
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
        FAILED__CIInfer(
          file, expr_access, CI_ERROR_KIND_EXPECTED_ARRAY_OR_POINTER);

        return;
    }

    FREE(CIDataType, array_dt);
}

void
infer_expr_access_data_type_arrow_or_dot__CIInfer(
  const CIResultFile *file,
  CIExpr **current_expr_access,
  CIDeclStructFields **current_fields,
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
        *current_fields =
          (CIDeclStructFields *)get_fields_from_data_type__CIResolverDataType(
            file,
            *current_infer_dt,
            called_generic_params,
            decl_generic_params);
    }

    *current_expr_access = (*current_expr_access)->binary.right;
}

void
infer_expr_access_data_type_identifier__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const CIDeclStructFields *fields,
  CIDataType **current_infer_dt)
{
    if (*current_infer_dt) {
        FREE(CIDataType, *current_infer_dt);
        *current_infer_dt = NULL;
    }

    CIDeclStructField *field = get_field_from_name__CIDeclStructFields(
      fields, expr_access->identifier.value);

    if (!field) {
        FAILED__CIInfer(file, expr_access, CI_ERROR_KIND_FIELD_IS_NOT_FOUND);

        return;
    }

    *current_infer_dt =
      field ? build_data_type__CIDeclStructField(field) : NULL;
}

CIDataType *
infer_expr_access_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr_access,
  const CIDeclStructFields *fields,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIExpr *current_expr_access = (CIExpr *)expr_access;
    CIDeclStructFields *current_fields = (CIDeclStructFields *)fields;
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
        FAILED__CIInfer(file, expr_access, CI_ERROR_KIND_CANNOT_INFER_PATH);

        return POISONED_DATA_TYPE__CIInfer(expr_access);
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
    const CIDeclStructFields *struct_or_union_fields =
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
    const CIDeclStructFields *struct_or_union_fields =
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
infer_expr_binary_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT(expr->kind == CI_EXPR_KIND_BINARY);

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

    CIDataType *left_dt = infer_expr_data_type__CIInfer(file,
                                                        expr->binary.left,
                                                        current_scope_id,
                                                        called_generic_params,
                                                        decl_generic_params);

    switch (expr->binary.kind) {
        case CI_EXPR_BINARY_KIND_ASSIGN:
            return left_dt;
        // What is written on the left of a comma is read for what it does, and
        // what is written on its right is what it gives back.
        case CI_EXPR_BINARY_KIND_COMMA:
            FREE(CIDataType, left_dt);

            return infer_expr_data_type__CIInfer(file,
                                                 expr->binary.right,
                                                 current_scope_id,
                                                 called_generic_params,
                                                 decl_generic_params);
        // What a comparison and a logical operation return is whether it
        // holds, an int, whatever the operands are read as. Left to what
        // follows, an operand written as a float would give a float back,
        // which is not what a condition is written on.
        case CI_EXPR_BINARY_KIND_AND:
        case CI_EXPR_BINARY_KIND_OR:
        case CI_EXPR_BINARY_KIND_EQ:
        case CI_EXPR_BINARY_KIND_NE:
        case CI_EXPR_BINARY_KIND_LESS:
        case CI_EXPR_BINARY_KIND_GREATER:
        case CI_EXPR_BINARY_KIND_LESS_EQ:
        case CI_EXPR_BINARY_KIND_GREATER_EQ:
            FREE(CIDataType, left_dt);

            return NEW(CIDataType,
                       clone__Location(&expr->location),
                       CI_DATA_TYPE_KIND_INT);
        default:
            break;
    }

    CIDataType *right_dt = infer_expr_data_type__CIInfer(file,
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
        return NEW(CIDataType,
                   clone__Location(&expr->location),
                   CI_DATA_TYPE_KIND_FLOAT);
    } else if (left_dt_is_integer && right_dt_is_integer) {
        return NEW(
          CIDataType, clone__Location(&expr->location), CI_DATA_TYPE_KIND_INT);
    }

    FAILED__CIInfer(file, expr, CI_ERROR_KIND_OPERATION_IS_NOT_POSSIBLE);

    return POISONED_DATA_TYPE__CIInfer(expr);
}

CIDataType *
infer_expr_identifier_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT(expr->kind == CI_EXPR_KIND_IDENTIFIER);

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
              local_current_scope, GET_PTR_RC(String, expr->identifier.value));

            if (label) {
                ((CIExpr *)expr)->identifier.id =
                  NEW_VARIANT(CIExprIdentifierID, label, label);

                goto label;
            }

            FAILED__CIInfer(
              file, expr, CI_ERROR_KIND_CANNOT_INFER_ON_UNKNOWN_IDENTIFIER);

            return POISONED_DATA_TYPE__CIInfer(expr);

            break;
        }
        case CI_EXPR_IDENTIFIER_ID_KIND_ENUM_VARIANT: {
            CIDecl *enum_variant_decl = get_enum_variant_from_id__CIResultFile(
              file, expr->identifier.id.enum_variant);

            ASSERT(enum_variant_decl);

            CIDataType *enum_data_type =
              enum_variant_decl->enum_variant->enum_data_type;

            if (enum_data_type) {
                return ref__CIDataType(enum_data_type);
            }

            return NEW(CIDataType,
                       clone__Location(&expr->location),
                       CI_DATA_TYPE_KIND_INT);
        }
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
            CIDeclFunctionParams *function_params =
              (CIDeclFunctionParams *)get_function_params__CIDecl(decl);

            return NEW_VARIANT(
              CIDataType,
              function,
              clone__Location(&expr->location),
              NEW(CIDataTypeFunction,
                  NULL,
                  function_params ? ref__CIDeclFunctionParams(function_params)
                                  : NULL,
                  resolved_function_return_data_type,
                  NULL,
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
}

CIDataType *
resolve_called_function__CIInfer(const CIResultFile *file,
                                 CIDataType *callee_data_type,
                                 const CIGenericParams *called_generic_params,
                                 const CIGenericParams *decl_generic_params)
{
    CIDataType *res = run__CIResolverDataType(
      file, callee_data_type, called_generic_params, decl_generic_params);

    // A function is as often called through a pointer on it as on itself, so
    // the pointers it is reached behind are looked through. Each of them is
    // resolved in turn, as any of them is written behind a typedef as well.
    while (res && res->kind == CI_DATA_TYPE_KIND_PTR) {
        CIDataType *ptr = get_ptr__CIDataType(res);
        CIDataType *resolved_ptr =
          ptr ? run__CIResolverDataType(
                  file, ptr, called_generic_params, decl_generic_params)
              : NULL;

        FREE(CIDataType, res);

        res = resolved_ptr;
    }

    if (res && res->kind != CI_DATA_TYPE_KIND_FUNCTION) {
        FREE(CIDataType, res);

        return NULL;
    }

    return res;
}

CIDataType *
infer_expr_function_call_data_type__CIInfer(
  const CIResultFile *file,
  const CIExpr *expr,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT(expr->kind == CI_EXPR_KIND_FUNCTION_CALL);

    const CIExprIdentifier *callee_identifier =
      get_callee_identifier__CIExprFunctionCall(&expr->function_call);

    // A call made on anything else than the name of a function has no
    // declaration to be searched for, so what it returns is read from the type
    // of what it is made on.
    if (!callee_identifier) {
        CIDataType *callee_data_type =
          infer_expr_data_type__CIInfer(file,
                                        expr->function_call.callee,
                                        current_scope_id,
                                        called_generic_params,
                                        decl_generic_params);
        CIDataType *called_function = resolve_called_function__CIInfer(
          file, callee_data_type, called_generic_params, decl_generic_params);
        CIDataType *res =
          called_function
            ? ref__CIDataType(called_function->function.return_data_type)
            : NEW(CIDataType,
                  clone__Location(&expr->location),
                  CI_DATA_TYPE_KIND_INT);

        if (called_function) {
            FREE(CIDataType, called_function);
        }

        FREE(CIDataType, callee_data_type);

        return res;
    }

    CIDecl *function_decl = search_function_in_generic_context__CIResultFile(
      file,
      GET_PTR_RC(String, callee_identifier->value),
      callee_identifier->generic_params,
      called_generic_params,
      decl_generic_params);

    ASSERT(function_decl->kind & CI_DECL_KIND_FUNCTION);

    if (function_decl) {
        return ref__CIDataType(
          (CIDataType *)get_return_data_type__CIDecl(function_decl));
    }

    return NEW(
      CIDataType, clone__Location(&expr->location), CI_DATA_TYPE_KIND_INT);
}

CIDataType *
infer_expr_function_call_builtin_data_type__CIInfer(const CIExpr *expr)
{
    ASSERT(expr->kind == CI_EXPR_KIND_FUNCTION_CALL_BUILTIN);

    CIBuiltin *builtin = get_ref__CIBuiltin();
    const CIBuiltinFunction *builtin_function =
      get_builtin_function__CIBuiltin(builtin, expr->function_call_builtin.id);

    // A builtin written to give back the data type it is given holds
    // `TYPE_INFO` as the data type it returns, as `__builtin_va_arg` does
    // (C11 7.16.1.1). What it gives back is read from the argument written
    // where the parameters hold `TYPE_INFO`.
    if (builtin_function->return_data_type->kind ==
        CI_DATA_TYPE_KIND_TYPE_INFO) {
        const Vec *called_params = expr->function_call_builtin.params;

        for (Usize i = 0; i < builtin_function->params->len; ++i) {
            const CIDataType *builtin_param =
              get__Vec(builtin_function->params, i);

            if (builtin_param->kind != CI_DATA_TYPE_KIND_TYPE_INFO ||
                i >= called_params->len) {
                continue;
            }

            const CIExpr *called_param = get__Vec(called_params, i);

            // The argument is only a data type when the call is written the
            // way the builtin expects. The typechecker is what says so, and
            // it has not run yet when the size of a declaration is read, so
            // anything else is left to it to report.
            if (called_param->kind == CI_EXPR_KIND_DATA_TYPE) {
                return ref__CIDataType(called_param->data_type);
            }
        }
    }

    return ref__CIDataType(builtin_function->return_data_type);
}

CIDataType *
infer_expr_literal_data_type__CIInfer(const CIResultFile *file,
                                      const CIExprLiteral *literal)
{
    // TODO: Probably improve the inference of literal expression
    switch (literal->kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            return NEW(
              CIDataType, SYNTHETIC_LOCATION__CI(), CI_DATA_TYPE_KIND_BOOL);
        case CI_EXPR_LITERAL_KIND_CHAR:
            return NEW(
              CIDataType, SYNTHETIC_LOCATION__CI(), CI_DATA_TYPE_KIND_CHAR);
        case CI_EXPR_LITERAL_KIND_FLOAT:
            return NEW(
              CIDataType, SYNTHETIC_LOCATION__CI(), CI_DATA_TYPE_KIND_FLOAT);
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            return NEW(
              CIDataType, SYNTHETIC_LOCATION__CI(), CI_DATA_TYPE_KIND_INT);
        case CI_EXPR_LITERAL_KIND_STRING: {
            CIDataType *string_dt =
              NEW_VARIANT(CIDataType,
                          array,
                          SYNTHETIC_LOCATION__CI(),
                          NEW_VARIANT(CIDataTypeArray,
                                      sized,
                                      NEW(CIDataType,
                                          SYNTHETIC_LOCATION__CI(),
                                          CI_DATA_TYPE_KIND_CHAR),
                                      NULL,
                                      GET_PTR_RC(String, literal->string)->len,
                                      NULL,
                                      false,
                                      CI_DATA_TYPE_QUALIFIER_NONE));

            set_context__CIDataType(string_dt, CI_DATA_TYPE_CONTEXT_STACK);

            return string_dt;
        }
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            return NEW(CIDataType,
                       SYNTHETIC_LOCATION__CI(),
                       CI_DATA_TYPE_KIND_UNSIGNED_INT);
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
                CIDataType *res = ref__CIDataType(
                  unwrap_implicit_ptr_data_type__CIResolverDataType(
                    file,
                    unary_right_expr_data_type,
                    called_generic_params,
                    decl_generic_params));

                FREE(CIDataType, unary_right_expr_data_type);

                return res;
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
        case CI_EXPR_KIND_SIZEOF:
            // NOTE: Get 32 bits version of size_t
            return NEW(CIDataType,
                       clone__Location(&expr->location),
                       CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT);
        case CI_EXPR_KIND_ARRAY_ACCESS: {
            CIDataType *array_dt =
              infer_expr_data_type__CIInfer(file,
                                            expr->array_access.array,
                                            current_scope_id,
                                            called_generic_params,
                                            decl_generic_params);
            // What is held is taken a reference on before what holds it is
            // dropped, as it is part of it: a data type built for the access
            // alone, such as the one of a string literal, is otherwise dropped
            // with it.
            CIDataType *res =
              ref__CIDataType(unwrap_implicit_ptr_data_type__CIResolverDataType(
                file, array_dt, called_generic_params, decl_generic_params));

            FREE(CIDataType, array_dt);

            return res;
        }
        case CI_EXPR_KIND_BINARY:
            return infer_expr_binary_data_type__CIInfer(file,
                                                        expr,
                                                        current_scope_id,
                                                        called_generic_params,
                                                        decl_generic_params);
        case CI_EXPR_KIND_CAST:
            return ref__CIDataType(expr->cast.data_type);
        case CI_EXPR_KIND_DATA_TYPE:
            return ref__CIDataType(expr->data_type);
        case CI_EXPR_KIND_FUNCTION_CALL:
            return infer_expr_function_call_data_type__CIInfer(
              file,
              expr,
              current_scope_id,
              called_generic_params,
              decl_generic_params);
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            return infer_expr_function_call_builtin_data_type__CIInfer(expr);
        case CI_EXPR_KIND_GROUPING:
            return infer_expr_data_type__CIInfer(file,
                                                 expr->grouping,
                                                 current_scope_id,
                                                 called_generic_params,
                                                 decl_generic_params);
        case CI_EXPR_KIND_IDENTIFIER:
            return infer_expr_identifier_data_type__CIInfer(
              file,
              expr,
              current_scope_id,
              called_generic_params,
              decl_generic_params);
        case CI_EXPR_KIND_INITIALIZER:
            FAILED__CIInfer(file, expr, CI_ERROR_KIND_CANNOT_INFER_INITIALIZER);

            return POISONED_DATA_TYPE__CIInfer(expr);
        case CI_EXPR_KIND_LITERAL:
            return infer_expr_literal_data_type__CIInfer(file, &expr->literal);
        case CI_EXPR_KIND_NULLPTR:
            return NEW(CIDataType,
                       clone__Location(&expr->location),
                       CI_DATA_TYPE_KIND_NULLPTR_T);
        case CI_EXPR_KIND_TERNARY:
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

CIDataType *
perform_typeof__CIInfer(const CIResultFile *file,
                        const CIExpr *expr,
                        const CIScopeID *current_scope_id,
                        const CIGenericParams *called_generic_params,
                        const CIGenericParams *decl_generic_params)
{
    CIDataType *inferred_data_type = infer_expr_data_type__CIInfer(
      file, expr, current_scope_id, called_generic_params, decl_generic_params);
    // What is inferred is the data type of what `typeof` is written on, and is
    // shared with it. A copy is returned, as what is written around `typeof` is
    // set on what it stands for, and what it is written on is left as it is.
    CIDataType *res = clone__CIDataType(inferred_data_type);

    FREE(CIDataType, inferred_data_type);

    return res;
}

CIDataType *
perform_typeof_unqual__CIInfer(const CIResultFile *file,
                               const CIExpr *expr,
                               const CIScopeID *current_scope_id,
                               const CIGenericParams *called_generic_params,
                               const CIGenericParams *decl_generic_params)
{
    CIDataType *res = perform_typeof__CIInfer(
      file, expr, current_scope_id, called_generic_params, decl_generic_params);

    set_qualifier__CIDataType(res, CI_DATA_TYPE_QUALIFIER_NONE);

    return res;
}
