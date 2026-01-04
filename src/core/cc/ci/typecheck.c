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

#include <core/cc/ci/infer.h>
#include <core/cc/ci/resolver/data_type.h>
#include <core/cc/ci/resolver/data_type_access.h>
#include <core/cc/ci/typecheck.h>

struct CurrentSwitch
{
    bool is_present;
};

struct CurrentGenericParams
{
    CIGenericParams *called; // CIGenericParams*? (&)
    CIGenericParams *decl;   // CIGenericParams*? (&)
};

static inline CONSTRUCTOR(struct CurrentGenericParams,
                          CurrentGenericParams,
                          CIGenericParams *called,
                          CIGenericParams *decl);

static struct CurrentGenericParams
default__CurrentGenericParams();

struct CITypecheckContext
{
    const CIDecl *current_decl; // const CIDecl*? (&)
    struct CurrentGenericParams current_generic_params;
    const CIScopeID *current_scope_id; // const CIScopeID*? (&)
    struct CurrentSwitch current_switch;
};

static inline CONSTRUCTOR(struct CITypecheckContext,
                          CITypecheckContext,
                          const CIDecl *current_decl,
                          CIGenericParams *called_generic_params,
                          CIGenericParams *decl_generic_params);

/// @return Old value of generic params
static inline struct CurrentGenericParams
set_current_generic_params__CITypecheckContext(
  struct CITypecheckContext *self,
  struct CurrentGenericParams new_current_generic_params);

static inline void
set_file__CITypecheck(CITypecheck *self, const CIResultFile *file);

static inline void
unset_file__CITypecheck(CITypecheck *self);

static void
typecheck_enum_decl__CITypecheck(CITypecheck *self,
                                 CIDecl *enum_decl,
                                 struct CITypecheckContext *typecheck_ctx);

static bool
is_valid_implicit_cast__CITypecheck(const CITypecheck *self,
                                    CIDataType *left,
                                    CIDataType *right,
                                    struct CITypecheckContext *typecheck_ctx);

static bool
perform_typecheck__CITypecheck(const CITypecheck *self,
                               CIDataType *expected_data_type,
                               CIDataType *given_data_type,
                               bool can_try,
                               struct CITypecheckContext *typecheck_ctx);

static void
typecheck_array_access_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *expected_data_type,
  const CIExprArrayAccess *array_access_expr,
  struct CITypecheckContext *typecheck_ctx);

static bool
typecheck_binary_integer_compatible_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx);

static bool
typecheck_binary_integer_or_float_compatible_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_binary_arithmetic_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_binary_bit_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_binary_logical_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_binary_comparison_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_binary_expr__CITypecheck(const CITypecheck *self,
                                   const CIExprBinary *binary,
                                   struct CITypecheckContext *typecheck_ctx);

static void
typecheck_initializer_expr_for_array_dt__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_struct_or_union_initializer_item__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializerItem *initializer_item,
  const CIDeclStructFields *fields,
  CIDeclStructField **current_field_ref,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_initializer_expr_for_struct_dt__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_initializer_expr_for_union_dt__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_initializer_expr__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx);

/// @param decl_params Vec<CIDeclFunctionParam*>* (&)
/// @param called_params Vec<CIExpr*>* (&)
static void
typecheck_function_call_expr_params__CITypecheck(
  const CITypecheck *self,
  const CIDecl *decl_function_call,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_function_call_expr__CITypecheck(
  const CITypecheck *self,
  const CIExprFunctionCall *function_call,
  struct CITypecheckContext *typecheck_ctx);

/// @param decl_params Vec<CIDataType*>* (&)
/// @param called_params Vec<CIExpr*>* (&)
static void
typecheck_function_call_builtin_expr_params__CITypecheck(
  const CITypecheck *self,
  const CIBuiltinFunction *decl_function_call_builtin,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_function_call_builtin_expr__CITypecheck(
  const CITypecheck *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_ternary_expr__CITypecheck(const CITypecheck *self,
                                    const CIExprTernary *ternary,
                                    struct CITypecheckContext *typecheck_ctx);

static void
typecheck_unary_expr__CITypecheck(const CITypecheck *self,
                                  const CIExprUnary *unary,
                                  struct CITypecheckContext *typecheck_ctx);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_expr__CITypecheck(const CITypecheck *self,
                            CIDataType *expected_data_type,
                            const CIExpr *given_expr,
                            struct CITypecheckContext *typecheck_ctx);

/// @brief Typecheck the expression and try to discard it whenever possible.
static void
typecheck_expr_and_try_discard__CITypecheck(
  const CITypecheck *self,
  CIExpr *expr,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_function_item_decl__CIParser(
  const CITypecheck *self,
  const CIDecl *decl,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_case_stmt__CITypecheck(const CITypecheck *self,
                                 const CIStmtSwitchCase *case_,
                                 struct CITypecheckContext *typecheck_ctx);

static void
typecheck_condition_expr__CITypecheck(const CITypecheck *self,
                                      const CIExpr *cond,
                                      struct CITypecheckContext *typecheck_ctx);

static void
typecheck_do_while_stmt__CITypecheck(const CITypecheck *self,
                                     const CIStmtDoWhile *do_while,
                                     struct CITypecheckContext *typecheck_ctx);

static void
typecheck_if_stmt_branch__CITypecheck(const CITypecheck *self,
                                      const CIStmtIfBranch *if_branch,
                                      struct CITypecheckContext *typecheck_ctx);

static void
typecheck_if_stmt__CITypecheck(const CITypecheck *self,
                               const CIStmtIf *if_,
                               struct CITypecheckContext *typecheck_ctx);

static void
typecheck_for_stmt__CITypecheck(const CITypecheck *self,
                                const CIStmtFor *for_,
                                struct CITypecheckContext *typecheck_ctx);

/// @param goto_ const String* (&)
static void
typecheck_goto_stmt__CITypecheck(const CITypecheck *self,
                                 const String *goto_,
                                 struct CITypecheckContext *typecheck_ctx);

/// @param return_ const CIExpr*? (&)
static void
typecheck_return_stmt__CITypecheck(const CITypecheck *self,
                                   const CIExpr *return_,
                                   struct CITypecheckContext *typecheck_ctx);

static void
typecheck_switch_stmt__CITypecheck(const CITypecheck *self,
                                   const CIStmtSwitch *switch_,
                                   struct CITypecheckContext *typecheck_ctx);

static void
typecheck_while_stmt__CITypecheck(const CITypecheck *self,
                                  const CIStmtWhile *while_,
                                  struct CITypecheckContext *typecheck_ctx);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_stmt__CITypecheck(const CITypecheck *self,
                            const CIStmt *given_stmt,
                            struct CITypecheckContext *typecheck_ctx);

/// @param body const CIDeclFunctionItem* (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_body_item__CITypecheck(const CITypecheck *self,
                                 const CIDeclFunctionItem *item,
                                 struct CITypecheckContext *typecheck_ctx);

/// @param body const Vec<CIDeclFunctionItem*>*? (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_body__CITypecheck(const CITypecheck *self,
                            const CIDeclFunctionBody *body,
                            struct CITypecheckContext *typecheck_ctx);

static void
typecheck_function_decl__CITypecheck(CITypecheck *self,
                                     CIDecl *function_decl,
                                     struct CITypecheckContext *typecheck_ctx);

static void
typecheck_variable_decl__CITypecheck(const CITypecheck *self,
                                     const CIDecl *variable_decl,
                                     struct CITypecheckContext *typecheck_ctx);

static void
typecheck_global_decl__CITypecheck(CITypecheck *self, CIDecl *decl);

static void
typecheck_global_decls__CITypecheck(CITypecheck *self);

/// @param other_args void* (CITypecheck* (&))
static void
handler__Typecheck([[maybe_unused]] void *entity,
                   const CIResultFile *file,
                   void *other_args);

CONSTRUCTOR(struct CurrentGenericParams,
            CurrentGenericParams,
            CIGenericParams *called,
            CIGenericParams *decl)
{
    return (struct CurrentGenericParams){ .called = called, .decl = decl };
}

struct CurrentGenericParams
default__CurrentGenericParams()
{
    return (struct CurrentGenericParams){ .called = NULL, .decl = NULL };
}

CONSTRUCTOR(struct CITypecheckContext,
            CITypecheckContext,
            const CIDecl *current_decl,
            CIGenericParams *called_generic_params,
            CIGenericParams *decl_generic_params)
{
    return (struct CITypecheckContext){
        .current_decl = current_decl,
        .current_generic_params =
          NEW(CurrentGenericParams, called_generic_params, decl_generic_params),
        .current_scope_id = NULL,
        .current_switch = { .is_present = false }
    };
}

struct CurrentGenericParams
set_current_generic_params__CITypecheckContext(
  struct CITypecheckContext *self,
  struct CurrentGenericParams new_current_generic_params)
{
    struct CurrentGenericParams old_current_generic_params =
      self->current_generic_params;

    self->current_generic_params = new_current_generic_params;

    return old_current_generic_params;
}

void
set_file__CITypecheck(CITypecheck *self, const CIResultFile *file)
{
    self->file = file;
    self->current_scope = self->file->scope_base;
}

void
unset_file__CITypecheck(CITypecheck *self)
{
    self->file = NULL;
    self->current_scope = NULL;
}

void
typecheck_enum_decl__CITypecheck(CITypecheck *self,
                                 CIDecl *enum_decl,
                                 struct CITypecheckContext *typecheck_ctx)
{
    if (enum_decl->enum_.data_type &&
        !is_integer_data_type__CIResolverDataType(
          self->file,
          enum_decl->enum_.data_type,
          false,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        FAILED("expected integer data type");
    }
}

bool
is_valid_implicit_cast__CITypecheck(const CITypecheck *self,
                                    CIDataType *left,
                                    CIDataType *right,
                                    struct CITypecheckContext *typecheck_ctx)
{
    switch (right->kind) {
        case CI_DATA_TYPE_KIND_BOOL:
        case CI_DATA_TYPE_KIND_CHAR:
        case CI_DATA_TYPE_KIND_INT:
        case CI_DATA_TYPE_KIND_LONG_INT:
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_SHORT_INT:
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            // NOTE: For the moment, we accept a cast from a float to an integer
            // without emitting a warning/error.
            return is_integer_data_type__CIResolverDataType(
                     self->file,
                     left,
                     true,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl) ||
                   is_float_data_type__CIResolverDataType(
                     self->file,
                     left,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl);
        case CI_DATA_TYPE_KIND_DOUBLE:
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
        case CI_DATA_TYPE_KIND__DECIMAL128:
        case CI_DATA_TYPE_KIND__DECIMAL32:
        case CI_DATA_TYPE_KIND__DECIMAL64:
        case CI_DATA_TYPE_KIND_FLOAT:
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            // NOTE: For the moment, we accept a cast from an integer to a float
            // without emitting a warning/error.
            return is_float_data_type__CIResolverDataType(
                     self->file,
                     left,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl) ||
                   is_integer_data_type__CIResolverDataType(
                     self->file,
                     left,
                     true,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl);
        case CI_DATA_TYPE_KIND_TYPEDEF:
            UNREACHABLE(
              "impossible to get typedef at this point (already resolved)");
        case CI_DATA_TYPE_KIND_ARRAY:
        case CI_DATA_TYPE_KIND_NULLPTR_T:
        case CI_DATA_TYPE_KIND_PTR:
            // In this case, we check whether the pointers being compared are
            // compatible. For example, we check whether a `void*` is compatible
            // with an `int*`.
            if (is_ptr_data_type__CIResolverDataType(
                  self->file,
                  left,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                if (is_compatible_with_void_ptr_data_type__CIResolverDataType(
                      self->file,
                      left,
                      right,
                      typecheck_ctx->current_generic_params.called,
                      typecheck_ctx->current_generic_params.decl)) {
                    return true;
                }

                CIDataType *left_ptr_dt =
                  unwrap_implicit_ptr_data_type__CIResolverDataType(
                    self->file,
                    left,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl);
                CIDataType *right_ptr_dt =
                  unwrap_implicit_ptr_data_type__CIResolverDataType(
                    self->file,
                    right,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl);

                return is_valid_implicit_cast__CITypecheck(
                  self, left_ptr_dt, right_ptr_dt, typecheck_ctx);
            }

            // This case is designed to ensure that a pointer or an array is
            // compatible with an integer.
            //
            // Valid: (void*)0, {1,2,3}, "hello"
            if (is_integer_data_type__CIResolverDataType(
                  self->file,
                  left,
                  true,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                return true;
            }

            return false;
        case CI_DATA_TYPE_KIND_ENUM:
            return is_integer_data_type__CIResolverDataType(
              self->file,
              right,
              true,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.decl);
        default:
            return false;
    }
}

bool
perform_typecheck__CITypecheck(const CITypecheck *self,
                               CIDataType *expected_data_type,
                               CIDataType *given_data_type,
                               bool can_try,
                               struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *resolved_expected_data_type =
      run__CIResolverDataType(self->file,
                              expected_data_type,
                              typecheck_ctx->current_generic_params.called,
                              typecheck_ctx->current_generic_params.decl);
    CIDataType *resolved_given_data_type =
      run__CIResolverDataType(self->file,
                              given_data_type,
                              typecheck_ctx->current_generic_params.called,
                              typecheck_ctx->current_generic_params.decl);

    if (!eq__CIDataType(resolved_expected_data_type,
                        resolved_given_data_type)) {
        if (!is_valid_implicit_cast__CITypecheck(self,
                                                 resolved_expected_data_type,
                                                 resolved_given_data_type,
                                                 typecheck_ctx) ||
            !is_valid_implicit_cast__CITypecheck(self,
                                                 resolved_given_data_type,
                                                 resolved_expected_data_type,
                                                 typecheck_ctx)) {
            FREE(CIDataType, resolved_expected_data_type);
            FREE(CIDataType, resolved_given_data_type);

            if (!can_try) {
                FAILED("data types don't match");
            }

            return false;
        }
    }

    FREE(CIDataType, resolved_expected_data_type);
    FREE(CIDataType, resolved_given_data_type);

    return true;
}

void
typecheck_array_access_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *expected_data_type,
  const CIExprArrayAccess *array_access_expr,
  struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *expected_array_expr_dt = infer_expr_data_type__CIInfer(
      self->file,
      array_access_expr->array,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    if (!is_array_data_type__CIResolverDataType(
          self->file,
          expected_array_expr_dt,
          true,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        FAILED("expected array compatible data type");
    }

    typecheck_expr__CITypecheck(
      self, expected_array_expr_dt, array_access_expr->array, typecheck_ctx);

    CIDataType *expected_access_expr_dt =
      NEW(CIDataType, CI_DATA_TYPE_KIND_INT);

    typecheck_expr__CITypecheck(
      self, expected_access_expr_dt, array_access_expr->access, typecheck_ctx);

    FREE(CIDataType, expected_access_expr_dt);
    FREE(CIDataType, expected_array_expr_dt);
}

bool
typecheck_binary_integer_compatible_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    return is_integer_data_type__CIResolverDataType(
             self->file,
             left_dt,
             true,
             typecheck_ctx->current_generic_params.called,
             typecheck_ctx->current_generic_params.decl) &&
           is_integer_data_type__CIResolverDataType(
             self->file,
             right_dt,
             true,
             typecheck_ctx->current_generic_params.called,
             typecheck_ctx->current_generic_params.decl);
}

bool
typecheck_binary_integer_or_float_compatible_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    return (is_integer_data_type__CIResolverDataType(
              self->file,
              left_dt,
              true,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.decl) ||
            is_float_data_type__CIResolverDataType(
              self->file,
              left_dt,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.decl)) &&
           (is_integer_data_type__CIResolverDataType(
              self->file,
              right_dt,
              true,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.decl) ||
            is_float_data_type__CIResolverDataType(
              self->file,
              right_dt,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.decl));
}

void
typecheck_binary_arithmetic_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    if (!typecheck_binary_integer_or_float_compatible_expr__CITypecheck(
          self, left_dt, right_dt, typecheck_ctx)) {
        FAILED("expected integer or float compatible data type for arithmetic "
               "operation");
    }
}

void
typecheck_binary_bit_expr__CITypecheck(const CITypecheck *self,
                                       CIDataType *left_dt,
                                       CIDataType *right_dt,
                                       struct CITypecheckContext *typecheck_ctx)
{
    if (!typecheck_binary_integer_compatible_expr__CITypecheck(
          self, left_dt, right_dt, typecheck_ctx)) {
        FAILED("expected integer compatible data type for bit operation");
    }
}

void
typecheck_binary_logical_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    if (!typecheck_binary_integer_compatible_expr__CITypecheck(
          self, left_dt, right_dt, typecheck_ctx)) {
        FAILED("expected integer compatible data type for logical operation");
    }
}

void
typecheck_binary_comparison_expr__CITypecheck(
  const CITypecheck *self,
  CIDataType *left_dt,
  CIDataType *right_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    if (!typecheck_binary_integer_or_float_compatible_expr__CITypecheck(
          self, left_dt, right_dt, typecheck_ctx)) {
        FAILED("expected integer or float compatible data type for comparison "
               "operation");
    }
}

void
typecheck_binary_expr__CITypecheck(const CITypecheck *self,
                                   const CIExprBinary *binary,
                                   struct CITypecheckContext *typecheck_ctx)
{
    if (binary->kind == CI_EXPR_BINARY_KIND_DOT ||
        binary->kind == CI_EXPR_BINARY_KIND_ARROW) {
        // NOTE: We perform no typecheck here on `.` or `->` binary operator.
        return;
    }

    CIDataType *left_dt = infer_expr_data_type__CIInfer(
      self->file,
      binary->left,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    CIDataType *right_dt = infer_expr_data_type__CIInfer(
      self->file,
      binary->right,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.called);

    switch (binary->kind) {
        case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
        case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
        case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
        case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
        case CI_EXPR_BINARY_KIND_ADD:
        case CI_EXPR_BINARY_KIND_SUB:
        case CI_EXPR_BINARY_KIND_MUL:
        case CI_EXPR_BINARY_KIND_DIV:
            typecheck_binary_arithmetic_expr__CITypecheck(
              self, left_dt, right_dt, typecheck_ctx);

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
        case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT:
        case CI_EXPR_BINARY_KIND_MOD:
        case CI_EXPR_BINARY_KIND_BIT_AND:
        case CI_EXPR_BINARY_KIND_BIT_OR:
        case CI_EXPR_BINARY_KIND_BIT_XOR:
        case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
        case CI_EXPR_BINARY_KIND_BIT_RSHIFT:
            typecheck_binary_bit_expr__CITypecheck(
              self, left_dt, right_dt, typecheck_ctx);

            break;
        case CI_EXPR_BINARY_KIND_AND:
        case CI_EXPR_BINARY_KIND_OR:
            typecheck_binary_logical_expr__CITypecheck(
              self, left_dt, right_dt, typecheck_ctx);

            break;
        case CI_EXPR_BINARY_KIND_EQ:
        case CI_EXPR_BINARY_KIND_NE:
        case CI_EXPR_BINARY_KIND_LESS:
        case CI_EXPR_BINARY_KIND_GREATER:
        case CI_EXPR_BINARY_KIND_LESS_EQ:
        case CI_EXPR_BINARY_KIND_GREATER_EQ:
            typecheck_binary_comparison_expr__CITypecheck(
              self, left_dt, right_dt, typecheck_ctx);

            break;
        default:
            break;
    }

    switch (binary->kind) {
        case CI_EXPR_BINARY_KIND_ASSIGN:
            if (is_array_data_type__CIResolverDataType(
                  self->file,
                  left_dt,
                  false,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                FAILED("cannot assign expression to array data type");
            }
        case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
        case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
        case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
        case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
        case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
        case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT:
            perform_typecheck__CITypecheck(
              self, left_dt, right_dt, false, typecheck_ctx);

            break;
        default:
            break;
    }

    FREE(CIDataType, left_dt);
    FREE(CIDataType, right_dt);
}

void
typecheck_initializer_expr_for_array_dt__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx)
{
    for (Usize i = 0; i < initializer->items->len; ++i) {
        const CIExprInitializerItem *initializer_item =
          get__Vec(initializer->items, i);

        if (initializer_item->path) {
            FAILED("path is not expected for the initialization of array");
        }

        typecheck_expr__CITypecheck(
          self, expected_data_type, initializer_item->value, typecheck_ctx);
    }
}

void
typecheck_struct_or_union_initializer_item__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializerItem *initializer_item,
  const CIDeclStructFields *fields,
  CIDeclStructField **current_field_ref,
  struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *current_field_dt = NULL;

    if (initializer_item->path) {
        *current_field_ref = get_field_from_path__CIDeclStructFields(
          fields,
          initializer_item->path,
          self->file,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl);

        if (!(*current_field_ref)) {
            FAILED("field not found");
        }

        current_field_dt =
          build_data_type__CIDeclStructField(*current_field_ref);

        typecheck_expr__CITypecheck(
          self, current_field_dt, initializer_item->value, typecheck_ctx);

        goto exit;
    }

    if (!(*current_field_ref)) {
        FAILED("excess elements in struct or union initializer");
    }

    if (initializer_item->value->kind != CI_EXPR_KIND_INITIALIZER) {
        *current_field_ref =
          (*current_field_ref)->kind != CI_DECL_STRUCT_FIELD_KIND_MEMBER
            ? get_next_member__CIDeclStructField(*current_field_ref)
            : *current_field_ref;

        if (!(*current_field_ref)) {
            FAILED("there are no fields");
        }
    }

    current_field_dt = build_data_type__CIDeclStructField(*current_field_ref);

    switch (initializer_item->value->kind) {
        case CI_EXPR_KIND_INITIALIZER:
            typecheck_initializer_expr__CITypecheck(
              self,
              &initializer_item->value->initializer,
              current_field_dt,
              typecheck_ctx);

            break;
        default:
            typecheck_expr__CITypecheck(
              self, current_field_dt, initializer_item->value, typecheck_ctx);
    }

exit:
    FREE(CIDataType, current_field_dt);
}

void
typecheck_initializer_expr_for_struct_dt__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx)
{
    const CIDeclStructFields *fields =
      get_fields_from_data_type__CIResolverDataType(
        self->file,
        expected_data_type,
        typecheck_ctx->current_generic_params.called,
        typecheck_ctx->current_generic_params.decl);

    ASSERT(fields);

    CIDeclStructField *current_field = fields->first; // CIDeclStructField*? (&)
    Vec *called_fields = NEW(Vec); // Vec<CIDeclStructField* (&)>*

    for (Usize i = 0; i < initializer->items->len; ++i) {
        const CIExprInitializerItem *initializer_item =
          get__Vec(initializer->items, i);

        typecheck_struct_or_union_initializer_item__CITypecheck(
          self, initializer_item, fields, &current_field, typecheck_ctx);

        current_field =
          skip_fields_with_given_parent__CIDeclStructField(current_field);
    }

    FREE(Vec, called_fields);
}

void
typecheck_initializer_expr_for_union_dt__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx)
{
    if (initializer->items->len > 1) {
        FAILED("excess elements in union initializer");
    }

    const CIDeclStructFields *fields =
      get_fields_from_data_type__CIResolverDataType(
        self->file,
        expected_data_type,
        typecheck_ctx->current_generic_params.called,
        typecheck_ctx->current_generic_params.decl);

    ASSERT(fields);

    CIExprInitializerItem *initializer_item =
      safe_get__Vec(initializer->items, 0);

    if (initializer_item) {
        CIDeclStructField *current_field_ref = fields->first;

        typecheck_struct_or_union_initializer_item__CITypecheck(
          self, initializer_item, fields, &current_field_ref, typecheck_ctx);
    }
}

void
typecheck_initializer_expr__CITypecheck(
  const CITypecheck *self,
  const CIExprInitializer *initializer,
  CIDataType *expected_data_type,
  struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *resolved_expected_data_type =
      run__CIResolverDataType(self->file,
                              expected_data_type,
                              typecheck_ctx->current_generic_params.called,
                              typecheck_ctx->current_generic_params.decl);

    switch (resolved_expected_data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            typecheck_initializer_expr_for_array_dt__CITypecheck(
              self,
              initializer,
              resolved_expected_data_type->array.data_type,
              typecheck_ctx);

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            typecheck_initializer_expr_for_struct_dt__CITypecheck(
              self, initializer, resolved_expected_data_type, typecheck_ctx);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            typecheck_initializer_expr_for_union_dt__CITypecheck(
              self, initializer, resolved_expected_data_type, typecheck_ctx);

            break;
        default:
            FAILED("this data type is not expected with initializer");
    }

    FREE(CIDataType, resolved_expected_data_type);
}

void
typecheck_function_call_expr_params__CITypecheck(
  const CITypecheck *self,
  const CIDecl *decl_function_call,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx)
{
    const CIDeclFunctionParams *decl_function_call_params =
      get_function_params__CIDecl(decl_function_call);
    bool is_variadic =
      decl_function_call_params
        ? is_variadic__CIDeclFunctionParams(decl_function_call_params)
        : false;
    Usize decl_function_call_params_len =
      decl_function_call_params ? decl_function_call_params->content->len : 0;
    Usize called_params_len = called_params->len;

    if (decl_function_call_params_len != called_params_len &&
        (is_variadic &&
         called_params_len < decl_function_call_params_len - 1)) {
        FAILED("number of params don't matched");
    }

    struct CurrentGenericParams old_current_generic_params =
      decl_function_call->kind == CI_DECL_KIND_FUNCTION_GEN
        ? set_current_generic_params__CITypecheckContext(
            typecheck_ctx,
            NEW(CurrentGenericParams,
                decl_function_call->function_gen.called_generic_params,
                decl_function_call->function_gen.function->generic_params))
        : default__CurrentGenericParams();

    for (Usize i = 0; i < called_params_len; ++i) {
        const CIDeclFunctionParam *decl_param = get__Vec(
          decl_function_call_params->content,
          i >= decl_function_call_params_len ? decl_function_call_params_len - 1
                                             : i);
        const CIExpr *called_param = get__Vec(called_params, i);

        switch (decl_param->kind) {
            case CI_DECL_FUNCTION_PARAM_KIND_NORMAL: {
                typecheck_expr__CITypecheck(
                  self, decl_param->data_type, called_param, typecheck_ctx);

                break;
            }
            case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC: {
                // NOTE: Here, we expect any data type because a variadic
                // parameter doesn't require a specific data type, but we do
                // need to perform a typecheck on the child expression of the
                // "called_param" expression.
                CIDataType *expected_data_type =
                  NEW(CIDataType, CI_DATA_TYPE_KIND_ANY);

                typecheck_expr__CITypecheck(
                  self, expected_data_type, called_param, typecheck_ctx);

                FREE(CIDataType, expected_data_type);

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }
    }

    if (decl_function_call->kind == CI_DECL_KIND_FUNCTION_GEN) {
        set_current_generic_params__CITypecheckContext(
          typecheck_ctx, old_current_generic_params);
    }
}

void
typecheck_function_call_expr__CITypecheck(
  const CITypecheck *self,
  const CIExprFunctionCall *function_call,
  struct CITypecheckContext *typecheck_ctx)
{
    CIDecl *function_decl = search_function_in_generic_context__CIResultFile(
      self->file,
      GET_PTR_RC(String, function_call->identifier),
      function_call->generic_params,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    if (!function_decl) {
        UNREACHABLE("the function has not been generated");
    }

    if (function_decl) {
        switch (function_decl->kind) {
            case CI_DECL_KIND_FUNCTION:
            case CI_DECL_KIND_FUNCTION_GEN:
                typecheck_function_call_expr_params__CITypecheck(
                  self, function_decl, function_call->params, typecheck_ctx);

                break;
            default:
                UNREACHABLE("expected function or function gen");
        }
    }
}

void
typecheck_function_call_builtin_expr_params__CITypecheck(
  const CITypecheck *self,
  const CIBuiltinFunction *decl_function_call_builtin,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx)
{
    if (called_params->len != decl_function_call_builtin->params->len) {
        FAILED("number of params don't matched");
    }

    for (Usize i = 0; i < decl_function_call_builtin->params->len; ++i) {
        CIDataType *builtin_param =
          get__Vec(decl_function_call_builtin->params, i);
        const CIExpr *called_param = get__Vec(called_params, i);

        typecheck_expr__CITypecheck(
          self, builtin_param, called_param, typecheck_ctx);
    }
}

void
typecheck_function_call_builtin_expr__CITypecheck(
  const CITypecheck *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  struct CITypecheckContext *typecheck_ctx)
{
    CIBuiltin *builtin = get_ref__CIBuiltin();
    const CIBuiltinFunction *builtin_function =
      get_builtin_function__CIBuiltin(builtin, function_call_builtin->id);

    ASSERT(builtin_function);

    typecheck_function_call_builtin_expr_params__CITypecheck(
      self, builtin_function, function_call_builtin->params, typecheck_ctx);
}

void
typecheck_ternary_expr__CITypecheck(const CITypecheck *self,
                                    const CIExprTernary *ternary,
                                    struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *cond_dt = infer_expr_data_type__CIInfer(
      self->file,
      ternary->cond,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    if (!is_integer_data_type__CIResolverDataType(
          self->file,
          cond_dt,
          true,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        FAILED("expected interger");
    }

    FREE(CIDataType, cond_dt);

    CIDataType *if_dt = infer_expr_data_type__CIInfer(
      self->file,
      ternary->if_,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    CIDataType *else_dt = infer_expr_data_type__CIInfer(
      self->file,
      ternary->else_,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    perform_typecheck__CITypecheck(self, if_dt, else_dt, false, typecheck_ctx);

    FREE(CIDataType, if_dt);
    FREE(CIDataType, else_dt);
}

void
typecheck_unary_expr__CITypecheck(const CITypecheck *self,
                                  const CIExprUnary *unary,
                                  struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *right_dt = infer_expr_data_type__CIInfer(
      self->file,
      unary->expr,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
        case CI_EXPR_UNARY_KIND_POSITIVE:
        case CI_EXPR_UNARY_KIND_NEGATIVE:
            if (!(is_integer_data_type__CIResolverDataType(
                    self->file,
                    right_dt,
                    true,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl) ||
                  is_float_data_type__CIResolverDataType(
                    self->file,
                    right_dt,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl))) {
                FAILED("this operation is not allowed for this data type, "
                       "expected float or integer");
            }

            break;
        case CI_EXPR_UNARY_KIND_BIT_NOT:
        case CI_EXPR_UNARY_KIND_NOT:
            if (!is_integer_data_type__CIResolverDataType(
                  self->file,
                  right_dt,
                  true,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                FAILED("this operation is not allowed for this data type, "
                       "expected integer");
            }

            break;
        case CI_EXPR_UNARY_KIND_DEREFERENCE:
            if (!is_ptr_data_type__CIResolverDataType(
                  self->file,
                  right_dt,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                FAILED("this operation is not allowed for this data type, "
                       "expected pointer");
            }

            break;
        case CI_EXPR_UNARY_KIND_REF:
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    FREE(CIDataType, right_dt);
}

void
typecheck_expr__CITypecheck(const CITypecheck *self,
                            CIDataType *expected_data_type,
                            const CIExpr *given_expr,
                            struct CITypecheckContext *typecheck_ctx)
{
    ASSERT((typecheck_ctx->current_generic_params.called &&
            typecheck_ctx->current_generic_params.decl) ||
           (!typecheck_ctx->current_generic_params.called &&
            !typecheck_ctx->current_generic_params.decl));

    // Typecheck expression content.
    switch (given_expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
        case CI_EXPR_KIND_IDENTIFIER:
        case CI_EXPR_KIND_LITERAL:
        case CI_EXPR_KIND_NULLPTR:
        case CI_EXPR_KIND_SIZEOF:
            // NOTE: No typecheck to perform
            break;
        case CI_EXPR_KIND_ARRAY_ACCESS:
            typecheck_array_access_expr__CITypecheck(self,
                                                     expected_data_type,
                                                     &given_expr->array_access,
                                                     typecheck_ctx);

            break;
        case CI_EXPR_KIND_BINARY:
            typecheck_binary_expr__CITypecheck(
              self, &given_expr->binary, typecheck_ctx);

            break;
        case CI_EXPR_KIND_CAST: {
            // To allow cast (void) from all types, we expect any data type.
            CIDataType *expected_dt =
              given_expr->cast.data_type->kind == CI_DATA_TYPE_KIND_VOID
                ? NEW(CIDataType, CI_DATA_TYPE_KIND_ANY)
                : ref__CIDataType(given_expr->cast.data_type);

            typecheck_expr__CITypecheck(
              self, expected_dt, given_expr->cast.expr, typecheck_ctx);

            // TODO: Implement a specific check to verify what can be cast
            // explicitly.

            FREE(CIDataType, expected_dt);

            break;
        }
        case CI_EXPR_KIND_INITIALIZER:
            return typecheck_initializer_expr__CITypecheck(
              self,
              &given_expr->initializer,
              expected_data_type,
              typecheck_ctx);
        case CI_EXPR_KIND_FUNCTION_CALL:
            typecheck_function_call_expr__CITypecheck(
              self, &given_expr->function_call, typecheck_ctx);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            typecheck_function_call_builtin_expr__CITypecheck(
              self, &given_expr->function_call_builtin, typecheck_ctx);

            break;
        case CI_EXPR_KIND_GROUPING:
            typecheck_expr__CITypecheck(
              self, expected_data_type, given_expr->grouping, typecheck_ctx);

            break;
        case CI_EXPR_KIND_TERNARY:
            typecheck_ternary_expr__CITypecheck(
              self, &given_expr->ternary, typecheck_ctx);

            break;
        case CI_EXPR_KIND_UNARY:
            typecheck_unary_expr__CITypecheck(
              self, &given_expr->unary, typecheck_ctx);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    CIDataType *given_expr_dt = infer_expr_data_type__CIInfer(
      self->file,
      given_expr,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    ASSERT(given_expr_dt);

    // NOTE: Maybe move that check in other place.
    // validate_expr_according_data_type_context__CITypecheck(
    //   self, expected_data_type, given_expr, typecheck_ctx);
    perform_typecheck__CITypecheck(
      self, expected_data_type, given_expr_dt, false, typecheck_ctx);

    FREE(CIDataType, given_expr_dt);
}

void
typecheck_expr_and_try_discard__CITypecheck(
  const CITypecheck *self,
  CIExpr *expr,
  struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *expected_dt = NULL;

    // Here are the expressions whose value can be discarded by default.
    switch (expr->kind) {
        case CI_EXPR_KIND_BINARY:
            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_ASSIGN:
                case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
                case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
                case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
                case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
                case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
                case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT:
                    goto expected_any;
                default:
                    goto expected_void;
            }
        case CI_EXPR_KIND_FUNCTION_CALL:
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
        expected_any:
            // NOTE: In order to discard the value of a function call,
            // we expect the any type, which is usually only used for
            // builtins, but which has the particularity of accepting
            // all types.
            expected_dt = NEW(CIDataType, CI_DATA_TYPE_KIND_ANY);

            break;
        case CI_EXPR_KIND_UNARY:
            switch (expr->unary.kind) {
                case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
                case CI_EXPR_UNARY_KIND_POST_INCREMENT:
                case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
                case CI_EXPR_UNARY_KIND_POST_DECREMENT:
                    goto expected_any;
                default:
                    goto expected_void;
            }
        default:
        expected_void:
            expected_dt = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);
    }

    typecheck_expr__CITypecheck(self, expected_dt, expr, typecheck_ctx);

    FREE(CIDataType, expected_dt);
}

void
typecheck_function_item_decl__CIParser(const CITypecheck *self,
                                       const CIDecl *decl,
                                       struct CITypecheckContext *typecheck_ctx)
{
    switch (decl->kind) {
        case CI_DECL_KIND_LABEL:
            break;
        case CI_DECL_KIND_VARIABLE:
            typecheck_variable_decl__CITypecheck(self, decl, typecheck_ctx);

            break;
        default:
            UNREACHABLE("this kind of declaration is not expected or not yet "
                        "implement in body function");
    }
}

void
typecheck_case_stmt__CITypecheck(const CITypecheck *self,
                                 const CIStmtSwitchCase *case_,
                                 struct CITypecheckContext *typecheck_ctx)
{
    if (typecheck_ctx->current_switch.is_present) {
        CIDataType *expr_data_type = infer_expr_data_type__CIInfer(
          self->file,
          case_->value,
          typecheck_ctx->current_scope_id,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl);

        if (!is_integer_data_type__CIResolverDataType(
              self->file,
              expr_data_type,
              false,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.decl)) {
            FAILED("expected integer compatible data type");
        }

        FREE(CIDataType, expr_data_type);

        return;
    }

    // NOTE: We don't do anything because we're outside the switch
    // statement.
}

void
typecheck_condition_expr__CITypecheck(const CITypecheck *self,
                                      const CIExpr *cond,
                                      struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *cond_expr_dt = infer_expr_data_type__CIInfer(
      self->file,
      cond,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    if (!is_integer_data_type__CIResolverDataType(
          self->file,
          cond_expr_dt,
          true,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        FAILED("expected boolean compatible expression data type");
    }

    FREE(CIDataType, cond_expr_dt);
}

void
typecheck_do_while_stmt__CITypecheck(const CITypecheck *self,
                                     const CIStmtDoWhile *do_while,
                                     struct CITypecheckContext *typecheck_ctx)
{
    typecheck_condition_expr__CITypecheck(self, do_while->cond, typecheck_ctx);
    typecheck_body__CITypecheck(self, do_while->body, typecheck_ctx);
}

void
typecheck_if_stmt_branch__CITypecheck(const CITypecheck *self,
                                      const CIStmtIfBranch *if_branch,
                                      struct CITypecheckContext *typecheck_ctx)
{
    typecheck_condition_expr__CITypecheck(self, if_branch->cond, typecheck_ctx);
    typecheck_body__CITypecheck(self, if_branch->body, typecheck_ctx);
}

void
typecheck_if_stmt__CITypecheck(const CITypecheck *self,
                               const CIStmtIf *if_,
                               struct CITypecheckContext *typecheck_ctx)
{
    typecheck_if_stmt_branch__CITypecheck(self, if_->if_, typecheck_ctx);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            const CIStmtIfBranch *else_if = get__Vec(if_->else_ifs, i);

            typecheck_if_stmt_branch__CITypecheck(self, else_if, typecheck_ctx);
        }
    }

    if (if_->else_) {
        typecheck_body__CITypecheck(self, if_->else_, typecheck_ctx);
    }
}

void
typecheck_for_stmt__CITypecheck(const CITypecheck *self,
                                const CIStmtFor *for_,
                                struct CITypecheckContext *typecheck_ctx)
{
    if (for_->init_clauses) {
        for (Usize i = 0; i < for_->init_clauses->len; ++i) {
            typecheck_body_item__CITypecheck(
              self, get__Vec(for_->init_clauses, i), typecheck_ctx);
        }
    }

    if (for_->expr1) {
        typecheck_condition_expr__CITypecheck(self, for_->expr1, typecheck_ctx);
    }

    if (for_->exprs2) {
        CIDataType *expected_expr2_dt = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);

        for (Usize i = 0; i < for_->exprs2->len; ++i) {
            CIExpr *expr2 = get__Vec(for_->exprs2, i);

            typecheck_expr_and_try_discard__CITypecheck(
              self, expr2, typecheck_ctx);
        }

        FREE(CIDataType, expected_expr2_dt);
    }

    typecheck_body__CITypecheck(self, for_->body, typecheck_ctx);
}

void
typecheck_goto_stmt__CITypecheck(const CITypecheck *self,
                                 const String *goto_,
                                 struct CITypecheckContext *typecheck_ctx)
{
    const CIScope *local_current_scope = get_scope_from_id__CIResultFile(
      self->file, typecheck_ctx->current_scope_id);

    CIDecl *label_decl =
      search_label__CIResultFile(self->file, local_current_scope, goto_);

    if (!label_decl) {
        FAILED("the specified label defined in goto doesn't exist");
    }
}

void
typecheck_return_stmt__CITypecheck(const CITypecheck *self,
                                   const CIExpr *return_,
                                   struct CITypecheckContext *typecheck_ctx)
{
    ASSERT(typecheck_ctx->current_decl);

    const CIDataType *given_return_data_type =
      get_return_data_type__CIDecl(typecheck_ctx->current_decl);

    if (return_) {
        typecheck_expr__CITypecheck(
          self, (CIDataType *)given_return_data_type, return_, typecheck_ctx);
    } else {
        CIDataType *expected_return_data_type =
          NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);

        if (!eq__CIDataType(expected_return_data_type,
                            given_return_data_type)) {
            FAILED("expected void return data type");
        }

        FREE(CIDataType, expected_return_data_type);
    }
}

void
typecheck_switch_stmt__CITypecheck(const CITypecheck *self,
                                   const CIStmtSwitch *switch_,
                                   struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *expr_data_type = infer_expr_data_type__CIInfer(
      self->file,
      switch_->expr,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    struct CurrentSwitch parent_current_switch = typecheck_ctx->current_switch;

    typecheck_ctx->current_switch.is_present = true;

    if (!is_integer_data_type__CIResolverDataType(
          self->file,
          expr_data_type,
          false,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        FAILED("expected integer compatible data type");
    }

    typecheck_body__CITypecheck(self, switch_->body, typecheck_ctx);

    typecheck_ctx->current_switch = parent_current_switch;

    FREE(CIDataType, expr_data_type);
}

void
typecheck_while_stmt__CITypecheck(const CITypecheck *self,
                                  const CIStmtWhile *while_,
                                  struct CITypecheckContext *typecheck_ctx)
{
    typecheck_condition_expr__CITypecheck(self, while_->cond, typecheck_ctx);
    typecheck_body__CITypecheck(self, while_->body, typecheck_ctx);
}

void
typecheck_stmt__CITypecheck(const CITypecheck *self,
                            const CIStmt *given_stmt,
                            struct CITypecheckContext *typecheck_ctx)
{
    ASSERT((typecheck_ctx->current_generic_params.called &&
            typecheck_ctx->current_generic_params.decl) ||
           (!typecheck_ctx->current_generic_params.called &&
            !typecheck_ctx->current_generic_params.decl));

    switch (given_stmt->kind) {
        case CI_STMT_KIND_BLOCK:
            return typecheck_body__CITypecheck(
              self, given_stmt->block.body, typecheck_ctx);
        case CI_STMT_KIND_BREAK:
            break;
        case CI_STMT_KIND_CASE:
            return typecheck_case_stmt__CITypecheck(
              self, &given_stmt->case_, typecheck_ctx);
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_DEFAULT:
            break;
        case CI_STMT_KIND_DO_WHILE:
            return typecheck_do_while_stmt__CITypecheck(
              self, &given_stmt->do_while, typecheck_ctx);
        case CI_STMT_KIND_FOR:
            return typecheck_for_stmt__CITypecheck(
              self, &given_stmt->for_, typecheck_ctx);
        case CI_STMT_KIND_GOTO:
            return typecheck_goto_stmt__CITypecheck(
              self, GET_PTR_RC(String, given_stmt->goto_), typecheck_ctx);
        case CI_STMT_KIND_IF:
            return typecheck_if_stmt__CITypecheck(
              self, &given_stmt->if_, typecheck_ctx);
        case CI_STMT_KIND_RETURN:
            return typecheck_return_stmt__CITypecheck(
              self, given_stmt->return_, typecheck_ctx);
        case CI_STMT_KIND_SWITCH:
            return typecheck_switch_stmt__CITypecheck(
              self, &given_stmt->switch_, typecheck_ctx);
        case CI_STMT_KIND_WHILE:
            return typecheck_while_stmt__CITypecheck(
              self, &given_stmt->while_, typecheck_ctx);
        default:
            UNREACHABLE("unknown variant");
    }
}

void
typecheck_body_item__CITypecheck(const CITypecheck *self,
                                 const CIDeclFunctionItem *item,
                                 struct CITypecheckContext *typecheck_ctx)
{
    switch (item->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            typecheck_function_item_decl__CIParser(
              self, item->decl, typecheck_ctx);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            typecheck_expr_and_try_discard__CITypecheck(
              self, item->expr, typecheck_ctx);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            typecheck_stmt__CITypecheck(self, &item->stmt, typecheck_ctx);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
typecheck_body__CITypecheck(const CITypecheck *self,
                            const CIDeclFunctionBody *body,
                            struct CITypecheckContext *typecheck_ctx)
{
    if (body) {
        VecIter body_content_iter = NEW(VecIter, body->content);
        CIDeclFunctionItem *body_content_item = NULL;
        const CIScopeID *parent_scope_id = typecheck_ctx->current_scope_id;

        typecheck_ctx->current_scope_id = body->scope_id;

        while ((body_content_item = next__VecIter(&body_content_iter))) {
            typecheck_body_item__CITypecheck(
              self, body_content_item, typecheck_ctx);
        }

        typecheck_ctx->current_scope_id = parent_scope_id;
    }
}

void
typecheck_function_decl__CITypecheck(CITypecheck *self,
                                     CIDecl *function_decl,
                                     struct CITypecheckContext *typecheck_ctx)
{
    // We can't check the types of a function that has as yet unknown generic
    // types.
    if (function_decl->function.generic_params) {
        return;
    }

    typecheck_body__CITypecheck(
      self, function_decl->function.body, typecheck_ctx);
}

void
typecheck_variable_decl__CITypecheck(const CITypecheck *self,
                                     const CIDecl *variable_decl,
                                     struct CITypecheckContext *typecheck_ctx)
{
    if (variable_decl->variable.expr) {
        typecheck_expr__CITypecheck(self,
                                    variable_decl->variable.data_type,
                                    variable_decl->variable.expr,
                                    typecheck_ctx);
    }
}

void
typecheck_global_decl__CITypecheck(CITypecheck *self, CIDecl *decl)
{
    if (has_generic__CIDecl(decl) || is_prototype__CIDecl(decl) ||
        is_local__CIDecl(decl)) {
        return;
    }

    struct CITypecheckContext typecheck_ctx =
      NEW(CITypecheckContext, decl, NULL, NULL);

    switch (decl->kind) {
        case CI_DECL_KIND_ENUM:
            typecheck_enum_decl__CITypecheck(self, decl, &typecheck_ctx);

            break;
        case CI_DECL_KIND_FUNCTION:
            typecheck_function_decl__CITypecheck(self, decl, &typecheck_ctx);

            break;
        case CI_DECL_KIND_VARIABLE:
            typecheck_variable_decl__CITypecheck(self, decl, &typecheck_ctx);

            break;
        default:
            break;
    }
}

void
typecheck_global_decls__CITypecheck(CITypecheck *self)
{
    CIDecl *decl = NULL;
    VecIter iter_decl = NEW(VecIter, self->file->entity.decls);

    while ((decl = next__VecIter(&iter_decl))) {
        typecheck_global_decl__CITypecheck(self, decl);
    }
}

void
handler__Typecheck([[maybe_unused]] void *entity,
                   const CIResultFile *file,
                   void *other_args)
{
    CITypecheck *self = (CITypecheck *)other_args;

    set_file__CITypecheck(self, file);
    typecheck_global_decls__CITypecheck(self);
    unset_file__CITypecheck(self);
}

void
run__CITypecheck(CITypecheck *self)
{
    pass_through_result__CIResult(self->result, &handler__Typecheck, self);
}
