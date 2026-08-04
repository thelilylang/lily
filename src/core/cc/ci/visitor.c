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
#include <base/vec.h>

#include <core/cc/ci/ast.h>
#include <core/cc/ci/diagnostic/emit.h>
#include <core/cc/ci/resolver/expr.h>
#include <core/cc/ci/typecheck.h>
#include <core/cc/ci/visitor.h>

// Emit a located error and count it, without stopping the visitor: the caller
// returns from the current check so that the rest of the pass still runs.
#define FAILED__CIVisitor(self, node, error_kind)                            \
    EMIT_ERROR__CI(                                                          \
      get_file_from_location__CIResultFile((self)->file, &(node)->location), \
      &(node)->location,                                                     \
      NEW(CIError, error_kind),                                              \
      &(self)->file->file_analysis->count_error)

inline bool
is_in_function_body__CIVisitor(CIVisitor *self);

static void
generate_function_gen__CIVisitor(CIVisitor *self,
                                 String *function_name,
                                 CIGenericParams *unresolved_generic_params,
                                 CIGenericParams *called_generic_params,
                                 CIGenericParams *decl_generic_params);

/// @brief Generate struct, union or typedef gen.
/// @param name String* (&)
/// @param unresolved_generic_params CIGenericParams*? (&)
/// @param called_generic_params CIGenericParams*? (&)
/// @param decl_generic_params CIGenericParams*? (&)
static void
generate_type_gen__CIVisitor(CIVisitor *self,
                             enum CIDeclKind kind,
                             String *name,
                             CIGenericParams *unresolved_generic_params,
                             CIGenericParams *called_generic_params,
                             CIGenericParams *decl_generic_params,
                             CIDecl *(*search_decl)(const CIResultFile *,
                                                    const String *));

/// @param struct_name String* (&)
static inline void
generate_struct_gen__CIVisitor(CIVisitor *self,
                               String *struct_name,
                               CIGenericParams *unresolved_generic_params,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params);

/// @param union_name String* (&)
static inline void
generate_union_gen__CIVisitor(CIVisitor *self,
                              String *union_name,
                              CIGenericParams *unresolved_generic_params,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params);

/// @param typedef_name_ref String* (&)
static inline void
generate_typedef_gen__CIVisitor(CIVisitor *self,
                                String *typedef_name_ref,
                                CIGenericParams *unresolved_generic_params,
                                CIGenericParams *called_generic_params,
                                CIGenericParams *decl_generic_params);

static void
generate_from_data_type__CIVisitor(CIVisitor *self,
                                   CIDataType *data_type,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params);

/// @param serialized_name String*? (&)
static CIDataType *
substitute_and_generate_from_data_type__CIVisitor(
  CIVisitor *self,
  CIDataType *data_type,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params,
  String *serialized_name);

/// @param current_field CIDeclStructField*? (&)* (&)
/// @param prev_gen_field CIDeclStructField*? (&)* (&)
/// @param parent_gen_field CIDeclStructField*? (&)
static void
visit_struct_or_union_field__CIVisitor(CIVisitor *self,
                                       CIDeclStructField **current_field_ref,
                                       CIDeclStructFields *gen_fields,
                                       CIDeclStructField **prev_gen_field,
                                       CIDeclStructField *parent_gen_field,
                                       CIGenericParams *generic_params,
                                       CIGenericParams *called_generic_params);

/// @brief Write the members a field written on a pack stands for, one per data
/// type the pack is left.
/// @param current_field const CIDeclStructField* (&)
static void
visit_struct_or_union_pack_field__CIVisitor(
  CIVisitor *self,
  const CIDeclStructField *current_field,
  CIDeclStructFields *gen_fields,
  CIDeclStructField **prev_gen_field,
  CIDeclStructField *parent_gen_field,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params);

/// @brief If the struct or union is not generic, this function will return
/// NULL.
/// @return the generated fields.
/// @return CIDeclStructFields*?
static CIDeclStructFields *
visit_struct_or_union__CIVisitor(CIVisitor *self,
                                 const CIDecl *decl,
                                 CIGenericParams *called_generic_params);

static void
visit_non_generic_struct_or_union_field__CIVisitor(
  CIVisitor *self,
  CIDeclStructField *current_field);

static void
visit_non_generic_struct_or_union__CIVisitor(CIVisitor *self,
                                             const CIDecl *decl);

/// @brief If the typedef is not generic, this function will return NULL.
/// @param serialized_name String*? (&)
/// @return the generated data type.
/// e.g. typedef <data_type> <name>;
/// @return CIDataType*?
static CIDataType *
visit_typedef__CIVisitor(CIVisitor *self,
                         const CIDecl *decl,
                         CIGenericParams *called_generic_params,
                         String *serialized_name);

static inline void
visit_non_generic_typedef__CIVisitor(CIVisitor *self, const CIDecl *decl);

static void
visit_function_data_type__CIVisitor(CIVisitor *self,
                                    const CIDataTypeFunction *function,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params);

static void
visit_data_type__CIVisitor(CIVisitor *self,
                           const CIDataType *data_type,
                           CIGenericParams *called_generic_params,
                           CIGenericParams *decl_generic_params);

static void
visit_variable__CIVisitor(CIVisitor *self,
                          const CIDecl *decl,
                          CIGenericParams *called_generic_params,
                          CIGenericParams *decl_generic_params);

static inline void
visit_non_generic_variable__CIVisitor(CIVisitor *self, const CIDecl *decl);

static void
visit_function_decl__CIVisitor(CIVisitor *self,
                               const CIDecl *decl,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params);

static void
visit_function_expr_array_access__CIVisitor(
  CIVisitor *self,
  const CIExprArrayAccess *array_access,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr_binary__CIVisitor(CIVisitor *self,
                                      const CIExprBinary *binary,
                                      CIGenericParams *called_generic_params,
                                      CIGenericParams *decl_generic_params);

static void
visit_function_expr_cast__CIVisitor(CIVisitor *self,
                                    const CIExprCast *cast,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params);

static void
visit_function_expr_function_call__CIVisitor(
  CIVisitor *self,
  const CIExprFunctionCall *function_call,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr_function_call_builtin__CIVisitor(
  CIVisitor *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr_initializer__CIVisitor(
  CIVisitor *self,
  const CIExprInitializer *initializer,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr_ternary__CIVisitor(CIVisitor *self,
                                       const CIExprTernary *ternary,
                                       CIGenericParams *called_generic_params,
                                       CIGenericParams *decl_generic_params);

static void
visit_function_expr__CIVisitor(CIVisitor *self,
                               const CIExpr *expr,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_block__CIVisitor(CIVisitor *self,
                                     const CIStmtBlock *block,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_do_while__CIVisitor(CIVisitor *self,
                                        const CIStmtDoWhile *do_while,
                                        CIGenericParams *called_generic_params,
                                        CIGenericParams *decl_generic_params);

static void
visit_function_stmt_for__CIVisitor(CIVisitor *self,
                                   const CIStmtFor *for_,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params);

static void
visit_function_stmt_if_branch__CIVisitor(CIVisitor *self,
                                         const CIStmtIfBranch *if_branch,
                                         CIGenericParams *called_generic_params,
                                         CIGenericParams *decl_generic_params);

static void
visit_function_stmt_if__CIVisitor(CIVisitor *self,
                                  const CIStmtIf *if_,
                                  CIGenericParams *called_generic_params,
                                  CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_switch__CIVisitor(CIVisitor *self,
                                      const CIStmtSwitch *switch_,
                                      CIGenericParams *called_generic_params,
                                      CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_while__CIVisitor(CIVisitor *self,
                                     const CIStmtWhile *while_,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params);

static void
visit_function_stmt__CIVisitor(CIVisitor *self,
                               const CIStmt *stmt,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params);

static void
visit_function_item__CIVisitor(CIVisitor *self,
                               const CIDeclFunctionItem *item,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params);

/// @param prev_scope CIScope* (&)*? (&)
static void
set_current_scope__CIVisitor(CIVisitor *self,
                             CIScope *scope,
                             CIScope **prev_scope);

static void
visit_function_body__CIVisitor(CIVisitor *self,
                               const CIDeclFunctionBody *body,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params);

/// @param called_generic_params CIGenericParams*? (&)
static void
visit_function_return_data_type__CIVisitor(
  CIVisitor *self,
  const CIDecl *decl,
  CIGenericParams *called_generic_params);

/// @param called_generic_params CIGenericParams*? (&)
static void
visit_function_params__CIVisitor(CIVisitor *self,
                                 const CIDecl *decl,
                                 CIGenericParams *called_generic_params);

/// @brief Keep only the paths the declaration holds, of the statements
/// written on a condition known once the types it is called on are.
static void
select_comptime_paths__CIVisitor(CIVisitor *self,
                                 CIDeclFunctionBody *body,
                                 CIGenericParams *decl_generic_params,
                                 CIGenericParams *called_generic_params);

static void
visit_function__CIVisitor(CIVisitor *self,
                          const CIDecl *decl,
                          CIGenericParams *called_generic_params,
                          const CIDeclFunctionBody *body);

static void
visit_non_generic_function__CIVisitor(CIVisitor *self, CIDecl *decl);

static void
visit_global_decl__CIVisitor(CIVisitor *self, CIDecl *decl);

/// @param file const CIResultFile* (&)
static inline void
set_file__CIVisitor(CIVisitor *self, const CIResultFile *file);

static inline void
unset_file__CIVisitor(CIVisitor *self);

static void
visit_global_decls__CIVisitor(CIVisitor *self);

static void
run_file__CIVisitor(CIVisitor *self, const CIResultFile *file);

/// @param other_args void* (CIVisitor*)
static void
handler__CIVisitor([[maybe_unused]] void *entity,
                   const CIResultFile *file,
                   void *other_args);

bool
is_in_function_body__CIVisitor(CIVisitor *self)
{
    ASSERT(self->file);

    return self->file->scope_base != self->current_scope;
}

void
generate_function_gen__CIVisitor(CIVisitor *self,
                                 String *function_name,
                                 CIGenericParams *unresolved_generic_params,
                                 CIGenericParams *called_generic_params,
                                 CIGenericParams *decl_generic_params)
{
    CIDecl *function_decl =
      search_function__CIResultFile(self->file, function_name);

    if (!function_decl) {
        FAILED__CIVisitor(
          self, self->current_decl, CI_ERROR_KIND_CALL_TO_UNKNOWN_FUNCTION);

        return;
    }

    // Generate gen function declaration
    if (unresolved_generic_params) {
        if (is_prototype__CIDecl(function_decl)) {
            FAILED__CIVisitor(self,
                              function_decl,
                              CI_ERROR_KIND_EXPECTED_DECLARATION_DEFINITION);

            return;
        } else {
            CIGenericParams *resolved_generic_params =
              has_generic__CIGenericParams(unresolved_generic_params)
                ? substitute_generic_params__CIParser(self->file,
                                                      unresolved_generic_params,
                                                      decl_generic_params,
                                                      called_generic_params)
                : ref__CIGenericParams(unresolved_generic_params);
            String *serialized_called_function_name =
              serialize_name__CIDeclFunction(&function_decl->function,
                                             resolved_generic_params);
            const CIDecl *function_gen = search_function__CIResultFile(
              self->file, serialized_called_function_name);

            if (!function_gen) {
                CIDataType *subs_return_data_type =
                  substitute_data_type__CIParser(
                    self->file,
                    function_decl->function.return_data_type,
                    function_decl->function.generic_params,
                    resolved_generic_params,
                    NULL);

                // The body is written for the types the function is called
                // on before it is read, so a path the declaration does not
                // hold is one nothing is instantiated from.
                CIDeclFunctionBody *instantiated_body =
                  function_decl->function.body
                    ? clone__CIDeclFunctionBody(function_decl->function.body)
                    : NULL;

                if (instantiated_body) {
                    // The body is read as the declaration it is written in,
                    // since what is written on the params of that declaration
                    // - which param is written on a pack, and how many data
                    // types it is left - is what is read of it.
                    const CIDecl *parent_decl = self->current_decl;

                    self->current_decl = function_decl;

                    select_comptime_paths__CIVisitor(
                      self,
                      instantiated_body,
                      function_decl->function.generic_params,
                      resolved_generic_params);

                    self->current_decl = parent_decl;
                }

                visit_function__CIVisitor(self,
                                          function_decl,
                                          resolved_generic_params,
                                          instantiated_body);

                CIDecl *function_gen_decl = NEW_VARIANT(
                  CIDecl,
                  function_gen,
                  clone__Location(&function_decl->location),
                  (CIDecl *)function_decl,
                  ref__CIGenericParams(resolved_generic_params),
                  serialized_called_function_name,
                  subs_return_data_type
                    ? subs_return_data_type
                    : ref__CIDataType(
                        function_decl->function
                          .return_data_type) /* Return a ref data type, when the
                                                substituted data type is NULL,
                                                to avoid an optional data type
                                                in the `return_data_type` field.
                                              */
                  ,
                  instantiated_body);

                add_decl_to_scope__CIResultFile(
                  self->file,
                  &function_gen_decl,
                  self->current_scope,
                  true,
                  is_in_function_body__CIVisitor(self));
            } else {
                FREE(String, serialized_called_function_name);
            }

            FREE(CIGenericParams, resolved_generic_params);
        }
    }
}

void
generate_type_gen__CIVisitor(CIVisitor *self,
                             enum CIDeclKind kind,
                             String *name,
                             CIGenericParams *unresolved_generic_params,
                             CIGenericParams *called_generic_params,
                             CIGenericParams *decl_generic_params,
                             CIDecl *(*search_decl)(const CIResultFile *,
                                                    const String *))
{
    CIDecl *decl = search_decl(self->file, name);

    if (!decl) {
        FAILED__CIVisitor(self,
                          self->current_decl,
                          CI_ERROR_KIND_STRUCT_TYPEDEF_OR_UNION_NOT_FOUND);

        return;
    }

    ASSERT(kind == decl->kind);

    if (unresolved_generic_params) {
        if (is_prototype__CIDecl(decl)) {
            FAILED__CIVisitor(
              self, decl, CI_ERROR_KIND_EXPECTED_DECLARATION_DEFINITION);

            return;
        } else {
            CIGenericParams *resolved_generic_params =
              has_generic__CIGenericParams(unresolved_generic_params)
                ? substitute_generic_params__CIParser(self->file,
                                                      unresolved_generic_params,
                                                      decl_generic_params,
                                                      called_generic_params)
                : ref__CIGenericParams(unresolved_generic_params);
            String *serialized_called_decl_name = NULL;

            switch (decl->kind) {
                case CI_DECL_KIND_STRUCT:
                    serialized_called_decl_name = serialize_name__CIDeclStruct(
                      &decl->struct_, resolved_generic_params);

                    break;
                case CI_DECL_KIND_TYPEDEF:
                    serialized_called_decl_name = serialize_name__CIDeclTypedef(
                      &decl->typedef_, resolved_generic_params);

                    break;
                case CI_DECL_KIND_UNION:
                    serialized_called_decl_name = serialize_name__CIDeclUnion(
                      &decl->union_, resolved_generic_params);

                    break;
                default:
                    UNREACHABLE("this kind of variant is not exepcted");
            }

            CIDecl *decl_gen =
              search_decl(self->file, serialized_called_decl_name);

            if (!decl_gen) {
                switch (decl->kind) {
                    case CI_DECL_KIND_STRUCT: {
                        CIDeclStructFields *fields =
                          visit_struct_or_union__CIVisitor(
                            self, decl, resolved_generic_params);

                        if (fields) {
                            decl_gen = NEW_VARIANT(
                              CIDecl,
                              struct_gen,
                              clone__Location(&decl->location),
                              decl,
                              ref__CIGenericParams(resolved_generic_params),
                              serialized_called_decl_name,
                              fields);

                            break;
                        }

                        FREE(CIGenericParams, resolved_generic_params);
                        FREE(String, serialized_called_decl_name);

                        return;
                    }
                    case CI_DECL_KIND_TYPEDEF: {
                        CIDataType *data_type =
                          visit_typedef__CIVisitor(self,
                                                   decl,
                                                   resolved_generic_params,
                                                   serialized_called_decl_name);

                        if (data_type) {
                            decl_gen = NEW_VARIANT(
                              CIDecl,
                              typedef_gen,
                              clone__Location(&decl->location),
                              decl,
                              ref__CIGenericParams(resolved_generic_params),
                              serialized_called_decl_name,
                              data_type);

                            break;
                        }

                        FREE(CIGenericParams, resolved_generic_params);
                        FREE(String, serialized_called_decl_name);

                        return;
                    }
                    case CI_DECL_KIND_UNION: {
                        CIDeclStructFields *fields =
                          visit_struct_or_union__CIVisitor(
                            self, decl, resolved_generic_params);

                        if (fields) {
                            decl_gen = NEW_VARIANT(
                              CIDecl,
                              union_gen,
                              clone__Location(&decl->location),
                              decl,
                              ref__CIGenericParams(resolved_generic_params),
                              serialized_called_decl_name,
                              fields);

                            break;
                        }

                        FREE(CIGenericParams, resolved_generic_params);
                        FREE(String, serialized_called_decl_name);

                        return;
                    }
                    default:
                        UNREACHABLE("this kind of variant is not expected");
                }

                ASSERT(decl_gen->kind & CI_DECL_KIND_GEN);

                add_decl_to_scope__CIResultFile(
                  self->file,
                  &decl_gen,
                  self->current_scope,
                  true,
                  is_in_function_body__CIVisitor(self));
            } else {
                FREE(String, serialized_called_decl_name);
            }

            FREE(CIGenericParams, resolved_generic_params);
        }
    }
}

void
generate_struct_gen__CIVisitor(CIVisitor *self,
                               String *struct_name_ref,
                               CIGenericParams *unresolved_generic_params,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params)
{
    generate_type_gen__CIVisitor(self,
                                 CI_DECL_KIND_STRUCT,
                                 struct_name_ref,
                                 unresolved_generic_params,
                                 called_generic_params,
                                 decl_generic_params,
                                 &search_struct__CIResultFile);
}

void
generate_union_gen__CIVisitor(CIVisitor *self,
                              String *union_name_ref,
                              CIGenericParams *unresolved_generic_params,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params)
{
    generate_type_gen__CIVisitor(self,
                                 CI_DECL_KIND_UNION,
                                 union_name_ref,
                                 unresolved_generic_params,
                                 called_generic_params,
                                 decl_generic_params,
                                 &search_union__CIResultFile);
}

void
generate_typedef_gen__CIVisitor(CIVisitor *self,
                                String *typedef_name_ref,
                                CIGenericParams *unresolved_generic_params,
                                CIGenericParams *called_generic_params,
                                CIGenericParams *decl_generic_params)
{
    generate_type_gen__CIVisitor(self,
                                 CI_DECL_KIND_TYPEDEF,
                                 typedef_name_ref,
                                 unresolved_generic_params,
                                 called_generic_params,
                                 decl_generic_params,
                                 &search_typedef__CIResultFile);
}

void
generate_from_data_type__CIVisitor(CIVisitor *self,
                                   CIDataType *data_type,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
            if (data_type->struct_.name) {
                generate_struct_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->struct_.name),
                  data_type->struct_.generic_params,
                  called_generic_params,
                  decl_generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_UNION:
            if (data_type->union_.name) {
                generate_union_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->union_.name),
                  data_type->union_.generic_params,
                  called_generic_params,
                  decl_generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            generate_typedef_gen__CIVisitor(
              self,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params,
              called_generic_params,
              decl_generic_params);

            break;
        default:
            break;
    }
}

CIDataType *
substitute_and_generate_from_data_type__CIVisitor(
  CIVisitor *self,
  CIDataType *data_type,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params,
  String *serialized_name)
{
    CIDataType *subs_data_type =
      substitute_data_type__CIParser(self->file,
                                     data_type,
                                     generic_params,
                                     called_generic_params,
                                     serialized_name);

    // The substitution is allowed to fail, having already reported why. There
    // is nothing to generate from a data type that could not be built, and the
    // caller skips it in the same way.
    if (subs_data_type) {
        generate_from_data_type__CIVisitor(
          self, subs_data_type, called_generic_params, generic_params);
    }

    return subs_data_type;
}

void
visit_struct_or_union_pack_field__CIVisitor(
  CIVisitor *self,
  const CIDeclStructField *current_field,
  CIDeclStructFields *gen_fields,
  CIDeclStructField **prev_gen_field,
  CIDeclStructField *parent_gen_field,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params)
{
    const CIDataType *field_dt = current_field->member.data_type;
    CIGenericParamsRange range;

    if (find_generic_range__CIGenericParams(
          generic_params,
          called_generic_params,
          GET_PTR_RC(String, field_dt->generic),
          &range) != CI_GENERIC_PARAMS_RANGE_RESULT_OK) {
        // The declaration is one nothing can be instantiated from, which the
        // parser has already reported on.
        return;
    }

    for (Usize i = 0; i < range.len; ++i) {
        CIDataType *gen_field_dt = clone__CIDataType(
          get__Vec(called_generic_params->params, range.start + i));

        generate_from_data_type__CIVisitor(
          self, gen_field_dt, called_generic_params, generic_params);

        // The members a pack stands for are told apart by the rank they are
        // written at, since the source only ever names the pack once.
        Rc *gen_field_name =
          NEW(Rc,
              format__String(
                "{S}_{zu}", GET_PTR_RC(String, current_field->name), i));
        CIDeclStructField *gen_field = NEW_VARIANT(
          CIDeclStructField,
          member,
          gen_field_name,
          parent_gen_field,
          *prev_gen_field,
          NEW(
            CIDeclStructFieldMember, gen_field_dt, current_field->member.bit));

        FREE_RC(String, gen_field_name);

        ASSERT(add__CIDeclStructFields(gen_fields, gen_field, *prev_gen_field));

        *prev_gen_field = gen_field;
    }
}

void
visit_struct_or_union_field__CIVisitor(CIVisitor *self,
                                       CIDeclStructField **current_field_ref,
                                       CIDeclStructFields *gen_fields,
                                       CIDeclStructField **prev_gen_field,
                                       CIDeclStructField *parent_gen_field,
                                       CIGenericParams *generic_params,
                                       CIGenericParams *called_generic_params)
{
    CIDeclStructField *current_field = *current_field_ref;

    switch (current_field->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER: {
            CIDataType *field_dt = current_field->member.data_type;

            // A member written on a pack stands for one member per data type
            // the pack is left, rather than for one.
            if (is_pack__CIDataType(field_dt)) {
                visit_struct_or_union_pack_field__CIVisitor(
                  self,
                  current_field,
                  gen_fields,
                  prev_gen_field,
                  parent_gen_field,
                  generic_params,
                  called_generic_params);

                break;
            }

            CIDataType *subs_field_dt =
              substitute_and_generate_from_data_type__CIVisitor(
                self, field_dt, generic_params, called_generic_params, NULL);

            if (subs_field_dt) {
                CIDeclStructField *gen_field =
                  NEW_VARIANT(CIDeclStructField,
                              member,
                              current_field->name,
                              parent_gen_field,
                              *prev_gen_field,
                              NEW(CIDeclStructFieldMember,
                                  subs_field_dt,
                                  current_field->member.bit));

                ASSERT(add__CIDeclStructFields(
                  gen_fields, gen_field, *prev_gen_field));

                *prev_gen_field = gen_field;
            }

            break;
        }
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION: {
            CIDeclStructField *parent_field = current_field;
            CIDeclStructField *new_parent_gen_field =
              clone__CIDeclStructField(current_field);

            new_parent_gen_field->prev = *prev_gen_field;
            new_parent_gen_field->parent = parent_gen_field;

            ASSERT(add__CIDeclStructFields(
              gen_fields, new_parent_gen_field, *prev_gen_field));

            *prev_gen_field = new_parent_gen_field;

            while (*current_field_ref) {
                if ((*current_field_ref)->next &&
                    has_parent_by_addr__CIDeclStructField(
                      (*current_field_ref)->next, parent_field)) {
                    *current_field_ref = (*current_field_ref)->next;
                } else {
                    break;
                }

                visit_struct_or_union_field__CIVisitor(self,
                                                       current_field_ref,
                                                       gen_fields,
                                                       prev_gen_field,
                                                       new_parent_gen_field,
                                                       generic_params,
                                                       called_generic_params);
            }

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

CIDeclStructFields *
visit_struct_or_union__CIVisitor(CIVisitor *self,
                                 const CIDecl *decl,
                                 CIGenericParams *called_generic_params)
{
    ASSERT(decl->kind == CI_DECL_KIND_STRUCT ||
           decl->kind == CI_DECL_KIND_UNION);

    if (has_generic__CIDecl(decl)) {
        const CIDeclStructFields *fields = get_fields__CIDecl(decl);

        if (!fields) {
            return NULL;
        }

        CIGenericParams *generic_params = get_generic_params__CIDecl(decl);
        CIDeclStructField *current_field = fields->first;
        CIDeclStructFields *gen_fields = NEW(CIDeclStructFields);
        CIDeclStructField *prev_gen_field = NULL;

        while (current_field) {
            visit_struct_or_union_field__CIVisitor(self,
                                                   &current_field,
                                                   gen_fields,
                                                   &prev_gen_field,
                                                   NULL,
                                                   generic_params,
                                                   called_generic_params);

            if (current_field) {
                current_field = current_field->next;
            }
        }

        return gen_fields;
    }

    return NULL;
}

void
visit_non_generic_struct_or_union_field__CIVisitor(
  CIVisitor *self,
  CIDeclStructField *current_field)
{
    switch (current_field->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER: {
            generate_from_data_type__CIVisitor(
              self, current_field->member.data_type, NULL, NULL);

            break;
        }
        default:
            break;
    }
}

void
visit_non_generic_struct_or_union__CIVisitor(CIVisitor *self,
                                             const CIDecl *decl)
{
    ASSERT(decl->kind == CI_DECL_KIND_STRUCT ||
           decl->kind == CI_DECL_KIND_UNION);

    const CIDeclStructFields *fields = get_fields__CIDecl(decl);

    if (!fields) {
        return;
    }

    CIDeclStructField *current_field = fields->first;

    while (current_field) {
        visit_non_generic_struct_or_union_field__CIVisitor(self, current_field);

        current_field = current_field->next;
    }
}

CIDataType *
visit_typedef__CIVisitor(CIVisitor *self,
                         const CIDecl *decl,
                         CIGenericParams *called_generic_params,
                         String *serialized_name)
{
    ASSERT(decl->kind == CI_DECL_KIND_TYPEDEF);

    return has_generic__CIDecl(decl)
             ? substitute_and_generate_from_data_type__CIVisitor(
                 self,
                 decl->typedef_.data_type,
                 decl->typedef_.generic_params,
                 called_generic_params,
                 serialized_name)
             : NULL;
}

void
visit_non_generic_typedef__CIVisitor(CIVisitor *self, const CIDecl *decl)
{
    self->current_decl = decl;

    ASSERT(decl->kind == CI_DECL_KIND_TYPEDEF);

    generate_from_data_type__CIVisitor(
      self, decl->typedef_.data_type, NULL, NULL);
}

void
visit_function_data_type__CIVisitor(CIVisitor *self,
                                    const CIDataTypeFunction *function,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params)
{
    visit_data_type__CIVisitor(self,
                               function->return_data_type,
                               called_generic_params,
                               decl_generic_params);

    if (function->params) {
        for (Usize i = 0; i < function->params->content->len; ++i) {
            CIDeclFunctionParam *param = get__Vec(function->params->content, i);

            visit_data_type__CIVisitor(self,
                                       param->data_type,
                                       called_generic_params,
                                       decl_generic_params);
        }
    }
}

void
visit_data_type__CIVisitor(CIVisitor *self,
                           const CIDataType *data_type,
                           CIGenericParams *called_generic_params,
                           CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            visit_data_type__CIVisitor(self,
                                       data_type->array.data_type,
                                       called_generic_params,
                                       decl_generic_params);

            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            visit_function_data_type__CIVisitor(self,
                                                &data_type->function,
                                                called_generic_params,
                                                decl_generic_params);

            break;
        case CI_DATA_TYPE_KIND_PTR:
            if (data_type->ptr.data_type) {
                visit_data_type__CIVisitor(self,
                                           data_type->ptr.data_type,
                                           called_generic_params,
                                           decl_generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            if (data_type->struct_.name) {
                generate_struct_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->struct_.name),
                  data_type->struct_.generic_params,
                  called_generic_params,
                  decl_generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            generate_typedef_gen__CIVisitor(
              self,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params,
              called_generic_params,
              decl_generic_params);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            if (data_type->union_.name) {
                generate_union_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->union_.name),
                  data_type->union_.generic_params,
                  called_generic_params,
                  decl_generic_params);
            }

            break;
        default:
            break;
    }
}

void
visit_variable__CIVisitor(CIVisitor *self,
                          const CIDecl *decl,
                          CIGenericParams *called_generic_params,
                          CIGenericParams *decl_generic_params)
{
    visit_data_type__CIVisitor(self,
                               decl->variable.data_type,
                               called_generic_params,
                               decl_generic_params);

    if (decl->variable.expr) {
        visit_function_expr__CIVisitor(self,
                                       decl->variable.expr,
                                       called_generic_params,
                                       decl_generic_params);
    }
}

void
visit_non_generic_variable__CIVisitor(CIVisitor *self, const CIDecl *decl)
{
    self->current_decl = decl;

    visit_variable__CIVisitor(self, decl, NULL, NULL);
}

void
visit_function_decl__CIVisitor(CIVisitor *self,
                               const CIDecl *decl,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params)
{
    switch (decl->kind) {
        case CI_DECL_KIND_VARIABLE:
            visit_variable__CIVisitor(
              self, decl, called_generic_params, decl_generic_params);

            break;
        default:
            // TODO: visit other declarations, but for the moment we cannot
            // declare struct, union in function scope.
            break;
    }
}

void
visit_function_expr_array_access__CIVisitor(
  CIVisitor *self,
  const CIExprArrayAccess *array_access,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, array_access->array, called_generic_params, decl_generic_params);
    visit_function_expr__CIVisitor(
      self, array_access->access, called_generic_params, decl_generic_params);
}

void
visit_function_expr_binary__CIVisitor(CIVisitor *self,
                                      const CIExprBinary *binary,
                                      CIGenericParams *called_generic_params,
                                      CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, binary->left, called_generic_params, decl_generic_params);
    visit_function_expr__CIVisitor(
      self, binary->right, called_generic_params, decl_generic_params);
}

void
visit_function_expr_cast__CIVisitor(CIVisitor *self,
                                    const CIExprCast *cast,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, cast->expr, called_generic_params, decl_generic_params);
    visit_data_type__CIVisitor(
      self, cast->data_type, called_generic_params, decl_generic_params);
}

void
visit_function_expr_function_call__CIVisitor(
  CIVisitor *self,
  const CIExprFunctionCall *function_call,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    const CIExprIdentifier *callee_identifier =
      get_callee_identifier__CIExprFunctionCall(function_call);

    if (callee_identifier && callee_identifier->generic_params) {
        generate_function_gen__CIVisitor(
          self,
          GET_PTR_RC(String, callee_identifier->value),
          callee_identifier->generic_params,
          called_generic_params,
          decl_generic_params);
    } else if (!callee_identifier) {
        visit_function_expr__CIVisitor(self,
                                       function_call->callee,
                                       called_generic_params,
                                       decl_generic_params);
    }

    for (Usize i = 0; i < function_call->params->len; ++i) {
        visit_function_expr__CIVisitor(self,
                                       get__Vec(function_call->params, i),
                                       called_generic_params,
                                       decl_generic_params);
    }
}

void
visit_function_expr_function_call_builtin__CIVisitor(
  CIVisitor *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < function_call_builtin->params->len; ++i) {
        visit_function_expr__CIVisitor(
          self,
          get__Vec(function_call_builtin->params, i),
          called_generic_params,
          decl_generic_params);
    }
}

void
visit_function_expr_initializer__CIVisitor(
  CIVisitor *self,
  const CIExprInitializer *initializer,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < initializer->items->len; ++i) {
        CIExprInitializerItem *initializer_item =
          get__Vec(initializer->items, i);

        visit_function_expr__CIVisitor(self,
                                       initializer_item->value,
                                       called_generic_params,
                                       decl_generic_params);
    }
}

void
visit_function_expr_ternary__CIVisitor(CIVisitor *self,
                                       const CIExprTernary *ternary,
                                       CIGenericParams *called_generic_params,
                                       CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, ternary->cond, called_generic_params, decl_generic_params);
    visit_function_expr__CIVisitor(
      self, ternary->if_, called_generic_params, decl_generic_params);
    visit_function_expr__CIVisitor(
      self, ternary->else_, called_generic_params, decl_generic_params);
}

void
visit_function_expr__CIVisitor(CIVisitor *self,
                               const CIExpr *expr,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            visit_function_expr__CIVisitor(
              self, expr->alignof_, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_ARRAY_ACCESS:
            visit_function_expr_array_access__CIVisitor(self,
                                                        &expr->array_access,
                                                        called_generic_params,
                                                        decl_generic_params);

            break;
        case CI_EXPR_KIND_BINARY:
            visit_function_expr_binary__CIVisitor(
              self, &expr->binary, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_CAST:
            visit_function_expr_cast__CIVisitor(
              self, &expr->cast, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_DATA_TYPE:
            visit_data_type__CIVisitor(self,
                                       expr->data_type,
                                       called_generic_params,
                                       decl_generic_params);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL:
            visit_function_expr_function_call__CIVisitor(self,
                                                         &expr->function_call,
                                                         called_generic_params,
                                                         decl_generic_params);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            visit_function_expr_function_call_builtin__CIVisitor(
              self,
              &expr->function_call_builtin,
              called_generic_params,
              decl_generic_params);

            break;
        case CI_EXPR_KIND_GROUPING:
            visit_function_expr__CIVisitor(
              self, expr->grouping, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_IDENTIFIER:
            // TODO: Handling the case of generic pass-by-reference functions.
            break;
        case CI_EXPR_KIND_INITIALIZER:
            visit_function_expr_initializer__CIVisitor(self,
                                                       &expr->initializer,
                                                       called_generic_params,
                                                       decl_generic_params);

            break;
        case CI_EXPR_KIND_LITERAL:
        case CI_EXPR_KIND_NULLPTR:
            break;
        case CI_EXPR_KIND_SIZEOF:
            visit_function_expr__CIVisitor(
              self, expr->sizeof_, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_TERNARY:
            visit_function_expr_ternary__CIVisitor(
              self, &expr->ternary, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_UNARY:
            visit_function_expr__CIVisitor(self,
                                           expr->unary.expr,
                                           called_generic_params,
                                           decl_generic_params);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
visit_function_stmt_block__CIVisitor(CIVisitor *self,
                                     const CIStmtBlock *block,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params)
{
    visit_function_body__CIVisitor(
      self, block->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_do_while__CIVisitor(CIVisitor *self,
                                        const CIStmtDoWhile *do_while,
                                        CIGenericParams *called_generic_params,
                                        CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, do_while->cond, called_generic_params, decl_generic_params);
    visit_function_body__CIVisitor(
      self, do_while->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_for__CIVisitor(CIVisitor *self,
                                   const CIStmtFor *for_,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params)
{
    if (for_->init_clauses) {
        for (Usize i = 0; i < for_->init_clauses->len; ++i) {
            visit_function_item__CIVisitor(self,
                                           get__Vec(for_->init_clauses, i),
                                           called_generic_params,
                                           decl_generic_params);
        }
    }

    if (for_->expr1) {
        visit_function_expr__CIVisitor(
          self, for_->expr1, called_generic_params, decl_generic_params);
    }

    if (for_->exprs2) {
        for (Usize i = 0; i < for_->exprs2->len; ++i) {
            visit_function_expr__CIVisitor(self,
                                           get__Vec(for_->exprs2, i),
                                           called_generic_params,
                                           decl_generic_params);
        }
    }

    visit_function_body__CIVisitor(
      self, for_->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_if_branch__CIVisitor(CIVisitor *self,
                                         const CIStmtIfBranch *if_branch,
                                         CIGenericParams *called_generic_params,
                                         CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, if_branch->cond, called_generic_params, decl_generic_params);
    visit_function_body__CIVisitor(
      self, if_branch->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_if__CIVisitor(CIVisitor *self,
                                  const CIStmtIf *if_,
                                  CIGenericParams *called_generic_params,
                                  CIGenericParams *decl_generic_params)
{
    visit_function_stmt_if_branch__CIVisitor(
      self, if_->if_, called_generic_params, decl_generic_params);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            visit_function_stmt_if_branch__CIVisitor(self,
                                                     get__Vec(if_->else_ifs, i),
                                                     called_generic_params,
                                                     decl_generic_params);
        }
    }

    if (if_->else_) {
        visit_function_body__CIVisitor(
          self, if_->else_, called_generic_params, decl_generic_params);
    }
}

void
visit_function_stmt_switch__CIVisitor(CIVisitor *self,
                                      const CIStmtSwitch *switch_,
                                      CIGenericParams *called_generic_params,
                                      CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, switch_->expr, called_generic_params, decl_generic_params);
    visit_function_body__CIVisitor(
      self, switch_->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_while__CIVisitor(CIVisitor *self,
                                     const CIStmtWhile *while_,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIVisitor(
      self, while_->cond, called_generic_params, decl_generic_params);
    visit_function_body__CIVisitor(
      self, while_->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt__CIVisitor(CIVisitor *self,
                               const CIStmt *stmt,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params)
{
    switch (stmt->kind) {
        case CI_STMT_KIND_BLOCK:
            visit_function_stmt_block__CIVisitor(
              self, &stmt->block, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_BREAK:
        case CI_STMT_KIND_CASE: // NOTE: We skip this case, can only contain
                                // constant expressions.
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_DEFAULT:
            break;
        case CI_STMT_KIND_DO_WHILE:
            return visit_function_stmt_do_while__CIVisitor(
              self,
              &stmt->do_while,
              called_generic_params,
              decl_generic_params);
        case CI_STMT_KIND_FOR:
            visit_function_stmt_for__CIVisitor(
              self, &stmt->for_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_ASM:
            // The operands of an `asm` are expressions like any other, and
            // are visited so that what they name is instantiated.
            for (Usize i = 0; stmt->asm_.outputs && i < stmt->asm_.outputs->len;
                 ++i) {
                CIStmtAsmOperand *operand = get__Vec(stmt->asm_.outputs, i);

                visit_function_expr__CIVisitor(self,
                                               operand->value,
                                               called_generic_params,
                                               decl_generic_params);
            }

            for (Usize i = 0; stmt->asm_.inputs && i < stmt->asm_.inputs->len;
                 ++i) {
                CIStmtAsmOperand *operand = get__Vec(stmt->asm_.inputs, i);

                visit_function_expr__CIVisitor(self,
                                               operand->value,
                                               called_generic_params,
                                               decl_generic_params);
            }

            break;
        case CI_STMT_KIND_GOTO:
            break;
        case CI_STMT_KIND_IF:
            visit_function_stmt_if__CIVisitor(
              self, &stmt->if_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_RETURN:
            if (stmt->return_) {
                visit_function_expr__CIVisitor(self,
                                               stmt->return_,
                                               called_generic_params,
                                               decl_generic_params);
            }

            break;
        case CI_STMT_KIND_SWITCH:
            visit_function_stmt_switch__CIVisitor(
              self, &stmt->switch_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_WHILE:
            visit_function_stmt_while__CIVisitor(
              self, &stmt->while_, called_generic_params, decl_generic_params);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
visit_function_item__CIVisitor(CIVisitor *self,
                               const CIDeclFunctionItem *item,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params)
{
    switch (item->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            visit_function_decl__CIVisitor(
              self, item->decl, called_generic_params, decl_generic_params);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            visit_function_expr__CIVisitor(
              self, item->expr, called_generic_params, decl_generic_params);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            visit_function_stmt__CIVisitor(
              self, &item->stmt, called_generic_params, decl_generic_params);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
set_current_scope__CIVisitor(CIVisitor *self,
                             CIScope *scope,
                             CIScope **prev_scope)
{
    if (prev_scope) {
        *prev_scope = (CIScope *)self->current_scope;
    }

    self->current_scope = scope;
}

void
visit_function_body__CIVisitor(CIVisitor *self,
                               const CIDeclFunctionBody *body,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params)
{
    CIScope *prev_scope = NULL;
    CIScope *new_current_scope =
      get_scope_from_id__CIResultFile(self->file, body->scope_id);

    ASSERT(new_current_scope);

    set_current_scope__CIVisitor(self, new_current_scope, &prev_scope);

    for (Usize i = 0; i < body->content->len; ++i) {
        visit_function_item__CIVisitor(self,
                                       get__Vec(body->content, i),
                                       called_generic_params,
                                       decl_generic_params);
    }

    set_current_scope__CIVisitor(self, prev_scope, NULL);
}

void
visit_function_return_data_type__CIVisitor(
  CIVisitor *self,
  const CIDecl *decl,
  CIGenericParams *called_generic_params)
{
    const CIDataType *return_data_type = get_return_data_type__CIDecl(decl);
    CIGenericParams *decl_generic_params = get_generic_params__CIDecl(decl);

    visit_data_type__CIVisitor(
      self, return_data_type, called_generic_params, decl_generic_params);
}

void
visit_function_params__CIVisitor(CIVisitor *self,
                                 const CIDecl *decl,
                                 CIGenericParams *called_generic_params)
{
    const CIDeclFunctionParams *params = get_function_params__CIDecl(decl);
    CIGenericParams *decl_generic_params = get_generic_params__CIDecl(decl);

    if (params) {
        for (Usize i = 0; i < params->content->len; ++i) {
            const CIDeclFunctionParam *param = get__Vec(params->content, i);

            if (param->data_type) {
                visit_data_type__CIVisitor(self,
                                           param->data_type,
                                           called_generic_params,
                                           decl_generic_params);
            }
        }
    }
}

/// @brief Read what a data type written as an expression stands for, with the
/// types the declaration is written on read in place of the generics.
/// @return CIDataType*? The caller takes it over.
static CIDataType *
resolve_comptime_data_type__CIVisitor(CIVisitor *self,
                                      const CIExpr *expr,
                                      CIGenericParams *decl_generic_params,
                                      CIGenericParams *called_generic_params)
{
    // What is written between parentheses says the same thing as what it
    // holds.
    while (expr->kind == CI_EXPR_KIND_GROUPING) {
        expr = expr->grouping;
    }

    if (expr->kind != CI_EXPR_KIND_DATA_TYPE) {
        return NULL;
    }

    // A generic written with a rank is one read on a pack, and what it stands
    // for is the data type that rank holds rather than the one the generic
    // itself is left. The rank is known before the program runs, since it is
    // what an unrolled loop binds its counter to.
    if (expr->data_type->kind == CI_DATA_TYPE_KIND_GENERIC &&
        expr->data_type->generic_index && decl_generic_params &&
        called_generic_params) {
        CIResolverExpr resolver = NEW(CIResolverExpr,
                                      NULL,
                                      NULL,
                                      self->file,
                                      &self->file->file_analysis->count_error,
                                      false);

        set_comptime_env__CIResolverExpr(&resolver, self->comptime_env);

        CIExpr *resolved =
          run__CIResolverExpr(&resolver, expr->data_type->generic_index);

        if (!resolved) {
            return NULL;
        }

        Isize rank =
          to_literal_integer_value__CIResolverExpr(&resolver, resolved);

        FREE(CIExpr, resolved);

        CIGenericParamsRange range;

        if (find_generic_range__CIGenericParams(
              decl_generic_params,
              called_generic_params,
              GET_PTR_RC(String, expr->data_type->generic),
              &range) != CI_GENERIC_PARAMS_RANGE_RESULT_OK) {
            return NULL;
        }

        if (rank < 0 || (Usize)rank >= range.len) {
            FAILED__CIVisitor(
              self, expr, CI_ERROR_KIND_PACK_ACCESS_IS_OUT_OF_RANGE);

            return NULL;
        }

        return ref__CIDataType(
          get__Vec(called_generic_params->params, range.start + (Usize)rank));
    }

    // A data type is only written in place of a generic where a declaration
    // is written on generics and is called on types. Written anywhere else it
    // stands for itself, and there is nothing to read in its place: the
    // substitution is what reads the types a call is made on, and it is only
    // written where both are.
    if (decl_generic_params && called_generic_params) {
        CIDataType *res = substitute_data_type__CIParser(self->file,
                                                         expr->data_type,
                                                         decl_generic_params,
                                                         called_generic_params,
                                                         NULL);

        if (res) {
            return res;
        }
    }

    return ref__CIDataType(expr->data_type);
}

/// @brief Read what a condition written on data types stands for, which is
/// known once the types the declaration is written on are.
///
/// A comparison of data types is known wherever it is written, and not only
/// where a generic is written in it: what a declaration written on no generic
/// compares is already the two types themselves. C has nothing to read of such
/// a comparison, so what it stands for is read here in every declaration
/// rather than left to the C compiler. Only where a generic is written in it
/// are the types the declaration is called on read in its place.
///
/// @param is_true bool* (&) What the condition stands for, written only when
/// true is returned.
/// @return true when the condition is known before the program runs.
static bool
resolve_comptime_cond__CIVisitor(CIVisitor *self,
                                 const CIExpr *cond,
                                 CIGenericParams *decl_generic_params,
                                 CIGenericParams *called_generic_params,
                                 bool *is_true)
{
    while (cond->kind == CI_EXPR_KIND_GROUPING) {
        cond = cond->grouping;
    }

    // What is written on either side of `&&` and `||` is read from the left,
    // and what is written on the right of them is only read when the left
    // does not say what the whole of it stands for. A condition that is only
    // known while the program runs is left as it is written, since reading it
    // is what the program does, and what it does may be more than say whether
    // it holds.
    if (cond->kind == CI_EXPR_KIND_UNARY &&
        cond->unary.kind == CI_EXPR_UNARY_KIND_NOT) {
        if (resolve_comptime_cond__CIVisitor(self,
                                             cond->unary.expr,
                                             decl_generic_params,
                                             called_generic_params,
                                             is_true)) {
            *is_true = !*is_true;

            return true;
        }

        return false;
    }

    if (cond->kind != CI_EXPR_KIND_BINARY) {
        return false;
    }

    switch (cond->binary.kind) {
        case CI_EXPR_BINARY_KIND_AND:
        case CI_EXPR_BINARY_KIND_OR: {
            bool left_is_true = false;

            if (!resolve_comptime_cond__CIVisitor(self,
                                                  cond->binary.left,
                                                  decl_generic_params,
                                                  called_generic_params,
                                                  &left_is_true)) {
                return false;
            }

            // `&&` says nothing more once the left of it does not hold, and
            // `||` says nothing more once it does: what is written on the
            // right is not read at all then, so the whole of it is known.
            if (left_is_true == (cond->binary.kind == CI_EXPR_BINARY_KIND_OR)) {
                *is_true = left_is_true;

                return true;
            }

            // What the whole of it stands for is what is written on the right
            // of it, which is read the same way.
            return resolve_comptime_cond__CIVisitor(self,
                                                    cond->binary.right,
                                                    decl_generic_params,
                                                    called_generic_params,
                                                    is_true);
        }
        case CI_EXPR_BINARY_KIND_EQ:
        case CI_EXPR_BINARY_KIND_NE:
            break;
        default:
            return false;
    }

    CIDataType *left = resolve_comptime_data_type__CIVisitor(
      self, cond->binary.left, decl_generic_params, called_generic_params);

    if (!left) {
        return false;
    }

    CIDataType *right = resolve_comptime_data_type__CIVisitor(
      self, cond->binary.right, decl_generic_params, called_generic_params);

    if (!right) {
        FREE(CIDataType, left);

        return false;
    }

    bool eq = eq__CIDataType(left, right);

    FREE(CIDataType, left);
    FREE(CIDataType, right);

    *is_true = cond->binary.kind == CI_EXPR_BINARY_KIND_EQ ? eq : !eq;

    return true;
}

/// @brief Read what a comparison of data types stands for, wherever it is
/// written, and write it as what it stands for.
///
/// A comparison of data types is known once the types the declaration is
/// called on are, whether it is written as the whole of a condition or as a
/// part of one. What is written as a part of one is not a path the
/// declaration is written on, but it is still known, so it is written as the
/// 1 or the 0 it stands for rather than left as a comparison C cannot read.
///
/// @return CIExpr*? What is written in place of the expression, which the
/// caller takes over, or NULL when nothing in it is written on data types.
/// @brief Look for the param written on a pack the given name is written on.
/// @return CIDataType*? (&) NULL when the name is written on no param, or on
/// a param that is no pack.
static const CIDataType *
get_pack_param_data_type__CIVisitor(const CIVisitor *self, const String *name)
{
    if (!self->current_decl ||
        self->current_decl->kind != CI_DECL_KIND_FUNCTION) {
        return NULL;
    }

    const CIDeclFunctionParams *params = self->current_decl->function.params;

    if (!params) {
        return NULL;
    }

    for (Usize i = 0; i < params->content->len; ++i) {
        const CIDeclFunctionParam *param = get__Vec(params->content, i);

        if (param->kind == CI_DECL_FUNCTION_PARAM_KIND_NORMAL && param->name &&
            !strcmp(GET_PTR_RC(String, param->name)->buffer, name->buffer) &&
            param->data_type && is_pack__CIDataType(param->data_type)) {
            return param->data_type;
        }
    }

    return NULL;
}

/// @brief Read how many data types the pack the given name is written on is
/// left.
/// @return Whether the name is written on a pack the call site says the
/// length of.
static bool
get_pack_len__CIVisitor(const CIVisitor *self,
                        const String *name,
                        CIGenericParams *decl_generic_params,
                        CIGenericParams *called_generic_params,
                        Usize *res)
{
    const CIDataType *pack_data_type =
      get_pack_param_data_type__CIVisitor(self, name);

    if (!pack_data_type || !decl_generic_params || !called_generic_params) {
        return false;
    }

    CIGenericParamsRange range;

    if (find_generic_range__CIGenericParams(
          decl_generic_params,
          called_generic_params,
          GET_PTR_RC(String, pack_data_type->generic),
          &range) != CI_GENERIC_PARAMS_RANGE_RESULT_OK) {
        return false;
    }

    *res = range.len;

    return true;
}

/// @brief Say whether the declaration holds a param written `constexpr`.
/// @param self const CIDeclFunctionParams*? (&)
static bool
holds_comptime_param__CIVisitor(const CIDeclFunctionParams *self)
{
    if (!self) {
        return false;
    }

    for (Usize i = 0; i < self->content->len; ++i) {
        const CIDeclFunctionParam *param = get__Vec(self->content, i);

        if (param->is_comptime) {
            return true;
        }
    }

    return false;
}

/// @brief Write a value into the name a declaration is instantiated under.
///
/// What is written has to say the value and nothing else, since two values
/// written the same would be the same instance: a kind is written ahead of
/// every one of them, and what cannot be written as a name is written as the
/// bytes it holds.
static void
put_comptime_value_into_name__CIVisitor(String *res, const CIExpr *value)
{
    if (value->kind != CI_EXPR_KIND_LITERAL) {
        // Nothing but a literal is written here: what a value stands for has
        // been read before it is written, and reading it is what gives one.
        push_str__String(res, "u");

        return;
    }

    switch (value->literal.kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            push_str__String(res, value->literal.bool_ ? "b1" : "b0");

            break;
        case CI_EXPR_LITERAL_KIND_CHAR: {
            String *code =
              format__String("c{zu}", (Usize)(Uint8)value->literal.char_);

            APPEND_AND_FREE(res, code);

            break;
        }
        case CI_EXPR_LITERAL_KIND_FLOAT: {
            // The bytes a floating value holds are what say it apart from
            // every other one, which is what writing it as a number would
            // not: two values written the same to a few figures are not the
            // same value.
            Uint64 bits = 0;

            memcpy(&bits, &value->literal.float_, sizeof(bits));

            String *hex = format__String("f{zu:x}", (Usize)bits);

            APPEND_AND_FREE(res, hex);

            break;
        }
        case CI_EXPR_LITERAL_KIND_SIGNED_INT: {
            // A value written below zero is written `n` rather than `-`,
            // which is no letter a name is written with.
            String *digits =
              value->literal.signed_int < 0
                ? format__String("in{zu}", (Usize)-value->literal.signed_int)
                : format__String("i{zu}", (Usize)value->literal.signed_int);

            APPEND_AND_FREE(res, digits);

            break;
        }
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT: {
            String *digits =
              format__String("u{zu}", value->literal.unsigned_int);

            APPEND_AND_FREE(res, digits);

            break;
        }
        case CI_EXPR_LITERAL_KIND_STRING: {
            // Every byte is written as what it holds, so two strings written
            // the same are the same instance and no two others are.
            const String *str = GET_PTR_RC(String, value->literal.string);
            String *len = format__String("s{zu}_", str->len);

            APPEND_AND_FREE(res, len);

            for (Usize i = 0; i < str->len; ++i) {
                String *byte =
                  format__String("{zu:x}", (Usize)(Uint8)str->buffer[i]);

                APPEND_AND_FREE(res, byte);
            }

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

/// @brief Write the name a declaration is instantiated on the given values
/// under, which is the name it is written with and the values it is called
/// on.
/// @return String* The caller takes it over.
static String *
serialize_comptime_name__CIVisitor(const String *name, const Vec *values)
{
    String *res = format__String("{S}__c", name);

    for (Usize i = 0; i < values->len; ++i) {
        const CIComptimeBinding *binding = get__Vec((Vec *)values, i);

        push_str__String(res, "_");
        put_comptime_value_into_name__CIVisitor(res, binding->value);
    }

    return res;
}

/// @brief Read what `xs[i]` written on a pack stands for, which is the param
/// of the rank `i` says.
/// @return CIExpr*? NULL when it is no access made on a pack, or when the
/// index is only known while the program runs, which is left as it is
/// written.
static CIExpr *
fold_pack_access__CIVisitor(CIVisitor *self,
                            const CIExpr *expr,
                            CIGenericParams *decl_generic_params,
                            CIGenericParams *called_generic_params)
{
    const CIExpr *array = expr->array_access.array;

    while (array->kind == CI_EXPR_KIND_GROUPING) {
        array = array->grouping;
    }

    if (array->kind != CI_EXPR_KIND_IDENTIFIER) {
        return NULL;
    }

    const String *pack_name = GET_PTR_RC(String, array->identifier.value);
    Usize pack_len = 0;

    if (!get_pack_len__CIVisitor(self,
                                 pack_name,
                                 decl_generic_params,
                                 called_generic_params,
                                 &pack_len)) {
        return NULL;
    }

    // The index is read on what is known before the program runs, which is
    // what an unrolled loop binds its counter in.
    CIResolverExpr resolver = NEW(CIResolverExpr,
                                  NULL,
                                  NULL,
                                  self->file,
                                  &self->file->file_analysis->count_error,
                                  false);

    set_comptime_env__CIResolverExpr(&resolver, self->comptime_env);

    // A pack is written out as one param per data type it is left, so it is
    // no object anything is indexed on while the program runs: an index that
    // is not known here is one nothing can be written for, which the resolver
    // reports on.
    CIExpr *resolved =
      run__CIResolverExpr(&resolver, expr->array_access.access);

    if (!resolved) {
        return NULL;
    }

    Isize rank = to_literal_integer_value__CIResolverExpr(&resolver, resolved);

    FREE(CIExpr, resolved);

    // A pack is left as many data types as the call site writes, so a rank
    // outside of them is one no param is written on.
    if (rank < 0 || (Usize)rank >= pack_len) {
        FAILED__CIVisitor(
          self, expr, CI_ERROR_KIND_PACK_ACCESS_IS_OUT_OF_RANGE);

        return NULL;
    }

    Rc *name = NEW(Rc, format__String("{S}_{zu}", pack_name, (Usize)rank));
    CIExpr *res = NEW_VARIANT(
      CIExpr,
      identifier,
      clone__Location(&expr->location),
      NEW(CIExprIdentifier, name, NEW_VARIANT(CIExprIdentifierID, none), NULL));

    FREE_RC(String, name);

    return res;
}

/// @brief Write what a comparison of data types stands for, and what a param
/// of a pack read at a rank stands for, in place of the expression.
static CIExpr *
fold_comptime_exprs__CIVisitor(CIVisitor *self,
                               CIExpr *expr,
                               CIGenericParams *decl_generic_params,
                               CIGenericParams *called_generic_params);

/// @brief Write a declaration instantiated on the values its params written
/// `constexpr` are called with.
static void
generate_comptime_function__CIVisitor(CIVisitor *self,
                                      const CIDecl *function_decl,
                                      const String *name,
                                      Vec *values,
                                      CIGenericParams *decl_generic_params,
                                      CIGenericParams *called_generic_params)
{
    if (!function_decl->function.body) {
        FAILED__CIVisitor(
          self, function_decl, CI_ERROR_KIND_EXPECTED_DECLARATION_DEFINITION);

        return;
    }

    CIDeclFunctionBody *body =
      clone__CIDeclFunctionBody(function_decl->function.body);

    // The body is read on the values the params written `constexpr` hold, so
    // those names are known while it is read, and what is written on them is
    // written as the value it stands for.
    Vec *parent_env = self->comptime_env;
    const CIDecl *parent_decl = self->current_decl;
    Vec *env = NEW(Vec);

    if (parent_env) {
        for (Usize i = 0; i < parent_env->len; ++i) {
            push__Vec(env, get__Vec(parent_env, i));
        }
    }

    for (Usize i = 0; i < values->len; ++i) {
        push__Vec(env, get__Vec(values, i));
    }

    self->comptime_env = env;
    self->current_decl = function_decl;

    select_comptime_paths__CIVisitor(
      self, body, decl_generic_params, called_generic_params);

    self->comptime_env = parent_env;
    self->current_decl = parent_decl;

    FREE(Vec, env);

    // A param written `constexpr` is written nowhere the program reads, and
    // the instance is written on the value rather than given it: it holds the
    // params that are left, which is also what says the instance is written
    // out as it stands rather than instantiated again.
    CIDeclFunctionParams *params = NULL;

    if (function_decl->function.params) {
        Vec *content = NEW(Vec); // Vec<CIDeclFunctionParam*>*

        for (Usize i = 0; i < function_decl->function.params->content->len;
             ++i) {
            const CIDeclFunctionParam *param =
              get__Vec(function_decl->function.params->content, i);

            if (!param->is_comptime) {
                push__Vec(content, clone__CIDeclFunctionParam(param));
            }
        }

        params = NEW(CIDeclFunctionParams, content);
    }

    Rc *name_rc = NEW(Rc, clone__String((String *)name));
    CIDecl *instance =
      NEW_VARIANT(CIDecl,
                  function,
                  clone__Location(&function_decl->location),
                  function_decl->storage_class_flag,
                  false,
                  NEW(CIDeclFunction,
                      name_rc,
                      ref__CIDataType(function_decl->function.return_data_type),
                      NULL,
                      params,
                      body,
                      NULL));

    FREE_RC(String, name_rc);

    add_decl_to_scope__CIResultFile(self->file,
                                    &instance,
                                    self->current_scope,
                                    true,
                                    is_in_function_body__CIVisitor(self));
}

/// @brief Instantiate a declaration on the values its params written
/// `constexpr` are called with, and write the call as one made on it.
///
/// A param written `constexpr` holds a value the call site says, and the body
/// is read on that value: it is written nowhere the program reads, so the
/// declaration is instantiated on the value as it is on the types a generic
/// is called on. Each value the declaration is called on is written a
/// declaration of its own, named after the values it holds.
///
/// @return CIExpr*? The call as it is made on the instance, or NULL when the
/// declaration holds no param written `constexpr`.
static CIExpr *
fold_comptime_call__CIVisitor(CIVisitor *self,
                              const CIExpr *expr,
                              CIGenericParams *decl_generic_params,
                              CIGenericParams *called_generic_params)
{
    const CIExprIdentifier *callee =
      get_callee_identifier__CIExprFunctionCall(&expr->function_call);

    if (!callee) {
        return NULL;
    }

    CIDecl *function_decl = search_function__CIResultFile(
      self->file, GET_PTR_RC(String, callee->value));

    if (!function_decl || function_decl->kind != CI_DECL_KIND_FUNCTION ||
        !holds_comptime_param__CIVisitor(function_decl->function.params)) {
        return NULL;
    }

    // A declaration written on generics is instantiated on the types it is
    // called on, and one written with a param `constexpr` on the values it is
    // called with. Nothing is written yet to instantiate on both at once, so
    // the two are not written together rather than instantiated on one and
    // read on the other.
    if (function_decl->function.generic_params) {
        FAILED__CIVisitor(
          self, expr, CI_ERROR_KIND_COMPTIME_PARAM_ON_A_GENERIC_DECLARATION);

        return NULL;
    }

    const Vec *decl_params = function_decl->function.params->content;
    const Vec *args = expr->function_call.params;

    if (decl_params->len != args->len) {
        FAILED__CIVisitor(self, expr, CI_ERROR_KIND_PARAMS_COUNT_MISMATCH);

        return NULL;
    }

    CIResolverExpr resolver = NEW(CIResolverExpr,
                                  NULL,
                                  NULL,
                                  self->file,
                                  &self->file->file_analysis->count_error,
                                  false);

    set_comptime_env__CIResolverExpr(&resolver, self->comptime_env);

    Vec *values = NEW(Vec);    // Vec<CIComptimeBinding*>*
    Vec *left_args = NEW(Vec); // Vec<CIExpr*>*

    for (Usize i = 0; i < decl_params->len; ++i) {
        const CIDeclFunctionParam *param = get__Vec((Vec *)decl_params, i);
        CIExpr *arg = get__Vec((Vec *)args, i);

        if (!param->is_comptime) {
            CIExpr *folded_arg = fold_comptime_exprs__CIVisitor(
              self, arg, decl_generic_params, called_generic_params);

            push__Vec(left_args, folded_arg ? folded_arg : ref__CIExpr(arg));

            continue;
        }

        // What the call gives a param written `constexpr` has to be known
        // here, since the body is read on it rather than given it.
        CIExpr *resolved = run__CIResolverExpr(&resolver, arg);

        if (!resolved) {
            FREE_BUFFER_ITEMS(values->buffer, values->len, CIComptimeBinding);
            FREE(Vec, values);
            FREE_BUFFER_ITEMS(left_args->buffer, left_args->len, CIExpr);
            FREE(Vec, left_args);

            return NULL;
        }

        // The value is held as it was read, whichever kind of literal it
        // was written with, so nothing here is written on numbers alone.
        push__Vec(values, NEW(CIComptimeBinding, param->name, resolved));
    }

    String *name = serialize_comptime_name__CIVisitor(
      GET_PTR_RC(String, function_decl->function.name), values);

    // A declaration is instantiated once per set of values it is called on,
    // so one already written for these is the one the call is made on.
    if (!search_function__CIResultFile(self->file, name)) {
        generate_comptime_function__CIVisitor(self,
                                              function_decl,
                                              name,
                                              values,
                                              decl_generic_params,
                                              called_generic_params);
    }

    Rc *name_rc = NEW(Rc, clone__String(name));
    CIExpr *res =
      NEW_VARIANT(CIExpr,
                  function_call,
                  clone__Location(&expr->location),
                  NEW(CIExprFunctionCall,
                      NEW_VARIANT(CIExpr,
                                  identifier,
                                  clone__Location(&expr->location),
                                  NEW(CIExprIdentifier,
                                      name_rc,
                                      NEW_VARIANT(CIExprIdentifierID, none),
                                      NULL)),
                      left_args));

    FREE_RC(String, name_rc);
    FREE(String, name);
    FREE_BUFFER_ITEMS(values->buffer, values->len, CIComptimeBinding);
    FREE(Vec, values);

    return res;
}

static CIExpr *
fold_comptime_exprs__CIVisitor(CIVisitor *self,
                               CIExpr *expr,
                               CIGenericParams *decl_generic_params,
                               CIGenericParams *called_generic_params)
{
    bool is_true = false;

    if (resolve_comptime_cond__CIVisitor(
          self, expr, decl_generic_params, called_generic_params, &is_true)) {
        return NEW_VARIANT(CIExpr,
                           literal,
                           clone__Location(&expr->location),
                           NEW_VARIANT(CIExprLiteral, signed_int, is_true));
    }

    switch (expr->kind) {
        case CI_EXPR_KIND_ARRAY_ACCESS: {
            // `xs[i]` written on a pack is no access made while the program
            // runs: what it stands for is the param of that rank, which is
            // known once the index is. The rank is what says which data type
            // the param holds, so what is written on it is read on that type
            // rather than on one the whole of the pack shares.
            CIExpr *rank_access = fold_pack_access__CIVisitor(
              self, expr, decl_generic_params, called_generic_params);

            if (rank_access) {
                return rank_access;
            }

            // An access made on anything but a pack is one the program makes,
            // and what it is written on is read the same way as anything
            // else: an index written on the counter of an unrolled loop is
            // written as the value the turn holds.
            CIExpr *array =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->array_access.array,
                                             decl_generic_params,
                                             called_generic_params);
            CIExpr *access =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->array_access.access,
                                             decl_generic_params,
                                             called_generic_params);

            if (!array && !access) {
                return NULL;
            }

            return NEW_VARIANT(
              CIExpr,
              array_access,
              clone__Location(&expr->location),
              NEW(CIExprArrayAccess,
                  array ? array : ref__CIExpr(expr->array_access.array),
                  access ? access : ref__CIExpr(expr->array_access.access)));
        }
        case CI_EXPR_KIND_COUNTOF: {
            Usize len = 0;

            // How many data types the pack is left is said by the call site,
            // so it is known here and written as the number it stands for.
            if (!get_pack_len__CIVisitor(self,
                                         GET_PTR_RC(String, expr->countof),
                                         decl_generic_params,
                                         called_generic_params,
                                         &len)) {
                FAILED__CIVisitor(
                  self, expr, CI_ERROR_KIND_COUNTOF_IS_NOT_WRITTEN_ON_A_PACK);

                return NULL;
            }

            return NEW_VARIANT(CIExpr,
                               literal,
                               clone__Location(&expr->location),
                               NEW_VARIANT(CIExprLiteral, signed_int, len));
        }
        case CI_EXPR_KIND_IDENTIFIER: {
            // The counter of an unrolled loop is written nowhere the program
            // reads: the loop is run here, so what is written on the counter
            // is written as the value the turn holds.
            CIExpr *value = search_comptime_binding__CIResolverExpr(
              self->comptime_env, GET_PTR_RC(String, expr->identifier.value));

            return value ? ref__CIExpr(value) : NULL;
        }
        case CI_EXPR_KIND_FUNCTION_CALL: {
            // A call made on a declaration that holds a param written
            // `constexpr` is made on the instance written for the values it
            // gives them, rather than on the declaration itself.
            CIExpr *comptime_call = fold_comptime_call__CIVisitor(
              self, expr, decl_generic_params, called_generic_params);

            if (comptime_call) {
                return comptime_call;
            }

            // What a call is made on is read the same way as anything else
            // written as an expression: a param of a pack read at a rank is
            // written there as much as anywhere.
            Vec *params = NEW(Vec); // Vec<CIExpr*>*
            bool is_folded = false;

            for (Usize i = 0; i < expr->function_call.params->len; ++i) {
                CIExpr *param = get__Vec(expr->function_call.params, i);
                CIExpr *folded_param = fold_comptime_exprs__CIVisitor(
                  self, param, decl_generic_params, called_generic_params);

                is_folded = is_folded || folded_param;

                push__Vec(params,
                          folded_param ? folded_param : ref__CIExpr(param));
            }

            if (!is_folded) {
                FREE_BUFFER_ITEMS(params->buffer, params->len, CIExpr);
                FREE(Vec, params);

                return NULL;
            }

            return NEW_VARIANT(CIExpr,
                               function_call,
                               clone__Location(&expr->location),
                               NEW(CIExprFunctionCall,
                                   ref__CIExpr(expr->function_call.callee),
                                   params));
        }
        case CI_EXPR_KIND_SIZEOF:
        case CI_EXPR_KIND_ALIGNOF: {
            // What the size or the alignment is read of is written the same
            // way as anything else, so a value known before the program runs
            // is written there as what it stands for.
            CIExpr *operand = expr->kind == CI_EXPR_KIND_SIZEOF
                                ? expr->sizeof_
                                : expr->alignof_;
            CIExpr *folded = fold_comptime_exprs__CIVisitor(
              self, operand, decl_generic_params, called_generic_params);

            if (!folded) {
                return NULL;
            }

            return expr->kind == CI_EXPR_KIND_SIZEOF
                     ? NEW_VARIANT(CIExpr,
                                   sizeof,
                                   clone__Location(&expr->location),
                                   folded)
                     : NEW_VARIANT(CIExpr,
                                   alignof,
                                   clone__Location(&expr->location),
                                   folded);
        }
        case CI_EXPR_KIND_CAST: {
            // What a cast is written on is read the same way as anything
            // else: the data type says nothing that is read here, and the
            // expression is what a value known before the program runs is
            // written in.
            CIExpr *cast_expr =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->cast.expr,
                                             decl_generic_params,
                                             called_generic_params);

            return cast_expr
                     ? NEW_VARIANT(CIExpr,
                                   cast,
                                   clone__Location(&expr->location),
                                   NEW(CIExprCast,
                                       ref__CIDataType(expr->cast.data_type),
                                       cast_expr))
                     : NULL;
        }
        case CI_EXPR_KIND_TERNARY: {
            CIExpr *cond =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->ternary.cond,
                                             decl_generic_params,
                                             called_generic_params);
            CIExpr *if_ = fold_comptime_exprs__CIVisitor(self,
                                                         expr->ternary.if_,
                                                         decl_generic_params,
                                                         called_generic_params);
            CIExpr *else_ =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->ternary.else_,
                                             decl_generic_params,
                                             called_generic_params);

            if (!cond && !if_ && !else_) {
                return NULL;
            }

            return NEW_VARIANT(
              CIExpr,
              ternary,
              clone__Location(&expr->location),
              NEW(CIExprTernary,
                  cond ? cond : ref__CIExpr(expr->ternary.cond),
                  if_ ? if_ : ref__CIExpr(expr->ternary.if_),
                  else_ ? else_ : ref__CIExpr(expr->ternary.else_)));
        }
        case CI_EXPR_KIND_GROUPING: {
            CIExpr *grouping = fold_comptime_exprs__CIVisitor(
              self, expr->grouping, decl_generic_params, called_generic_params);

            return grouping ? NEW_VARIANT(CIExpr,
                                          grouping,
                                          clone__Location(&expr->location),
                                          grouping)
                            : NULL;
        }
        case CI_EXPR_KIND_BINARY: {
            CIExpr *left =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->binary.left,
                                             decl_generic_params,
                                             called_generic_params);
            CIExpr *right =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->binary.right,
                                             decl_generic_params,
                                             called_generic_params);

            if (!left && !right) {
                return NULL;
            }

            return NEW_VARIANT(
              CIExpr,
              binary,
              clone__Location(&expr->location),
              NEW(CIExprBinary,
                  expr->binary.kind,
                  left ? left : ref__CIExpr(expr->binary.left),
                  right ? right : ref__CIExpr(expr->binary.right)));
        }
        case CI_EXPR_KIND_UNARY: {
            CIExpr *unary_expr =
              fold_comptime_exprs__CIVisitor(self,
                                             expr->unary.expr,
                                             decl_generic_params,
                                             called_generic_params);

            return unary_expr
                     ? NEW_VARIANT(
                         CIExpr,
                         unary,
                         clone__Location(&expr->location),
                         NEW(CIExprUnary, expr->unary.kind, unary_expr))
                     : NULL;
        }
        default:
            return NULL;
    }
}

/// @brief Write what a comparison of data types stands for, in place of the
/// expression the body holds.
static void
fold_comptime_expr_slot__CIVisitor(CIVisitor *self,
                                   CIExpr **slot,
                                   CIGenericParams *decl_generic_params,
                                   CIGenericParams *called_generic_params)
{
    if (!*slot) {
        return;
    }

    CIExpr *folded = fold_comptime_exprs__CIVisitor(
      self, *slot, decl_generic_params, called_generic_params);

    if (folded) {
        FREE(CIExpr, *slot);
        *slot = folded;
    }
}

/// @brief Write how long an array is written to be as what it stands for,
/// where that is known before the program runs.
/// @param data_type CIDataType*? (&)
static void
fold_array_size__CIVisitor(CIVisitor *self,
                           CIDataType *data_type,
                           CIGenericParams *decl_generic_params,
                           CIGenericParams *called_generic_params)
{
    if (!data_type || data_type->kind != CI_DATA_TYPE_KIND_ARRAY) {
        return;
    }

    fold_comptime_expr_slot__CIVisitor(self,
                                       &data_type->array.size_expr,
                                       decl_generic_params,
                                       called_generic_params);

    // An array written of arrays is written a length per dimension it holds.
    fold_array_size__CIVisitor(self,
                               data_type->array.data_type,
                               decl_generic_params,
                               called_generic_params);
}

/// @brief Read which of the paths a statement is written with is the one the
/// declaration holds, and keep that one alone.
///
/// A condition known before the program runs is one the declaration is
/// written on either side of rather than one it reads while it runs. The path
/// it does not hold is not written, and nothing is read of it: that is what
/// makes a body written on a generic able to hold what only one of the types
/// it is called on can be written with.
///
/// @return CIDeclFunctionBody*? The path the statement holds, which the caller
/// takes over, or NULL when the statement holds none of them.
static CIDeclFunctionBody *
select_comptime_path__CIVisitor(CIVisitor *self,
                                const CIStmtIf *if_,
                                CIGenericParams *decl_generic_params,
                                CIGenericParams *called_generic_params,
                                bool *is_known)
{
    bool is_true = false;

    *is_known = false;

    if (!resolve_comptime_cond__CIVisitor(self,
                                          if_->if_->cond,
                                          decl_generic_params,
                                          called_generic_params,
                                          &is_true)) {
        return NULL;
    }

    if (is_true) {
        *is_known = true;

        return clone__CIDeclFunctionBody(if_->if_->body);
    }

    // What follows is read the same way, and is only known when each of the
    // conditions written before it is.
    for (Usize i = 0; if_->else_ifs && i < if_->else_ifs->len; ++i) {
        const CIStmtIfBranch *else_if = get__Vec(if_->else_ifs, i);

        if (!resolve_comptime_cond__CIVisitor(self,
                                              else_if->cond,
                                              decl_generic_params,
                                              called_generic_params,
                                              &is_true)) {
            return NULL;
        }

        if (is_true) {
            *is_known = true;

            return clone__CIDeclFunctionBody(else_if->body);
        }
    }

    *is_known = true;

    return if_->else_ ? clone__CIDeclFunctionBody(if_->else_) : NULL;
}

// A loop that is run before the program is has to end, and nothing written in
// it says that it does. What is written past this many turns is taken to be a
// loop that does not end, and is reported on rather than run.
#define CI_MAX_UNROLL_TURNS 4096

/// @brief Write the items an unrolled loop stands for where the loop was
/// written, and say how many were written.
///
/// `insert__Vec` is written on an index the body already holds, so what goes
/// past the end of it is pushed instead: the loop may be the only thing the
/// body is written with, and the body holds nothing at all once it is taken
/// out.
static Usize
splice_items__CIVisitor(Vec *content, Vec *items, Usize at)
{
    for (Usize i = 0; i < items->len; ++i) {
        void *item = get__Vec(items, i);

        if (at + i < content->len) {
            insert__Vec(content, item, at + i);
        } else {
            push__Vec(content, item);
        }
    }

    return items->len;
}

/// @brief Read the name and the value the init clause of an unrolled loop is
/// written with, which is what the counter is bound to on the first turn.
/// @return CIComptimeBinding*? The caller takes it over.
static CIComptimeBinding *
bind_unroll_counter__CIVisitor(CIVisitor *self,
                               const CIStmt *stmt,
                               CIResolverExpr *resolver)
{
    const Vec *init_clauses = stmt->for_.init_clauses;

    // The counter is what the turns of the loop are read from, so exactly one
    // is written, and it is written as a variable given a value.
    if (!init_clauses || init_clauses->len != 1) {
        FAILED__CIVisitor(
          self, stmt, CI_ERROR_KIND_UNROLLED_LOOP_HOLDS_NO_COUNTER);

        return NULL;
    }

    const CIDeclFunctionItem *init_clause = get__Vec((Vec *)init_clauses, 0);

    if (init_clause->kind != CI_DECL_FUNCTION_ITEM_KIND_DECL ||
        init_clause->decl->kind != CI_DECL_KIND_VARIABLE ||
        !init_clause->decl->variable.expr) {
        FAILED__CIVisitor(
          self, stmt, CI_ERROR_KIND_UNROLLED_LOOP_HOLDS_NO_COUNTER);

        return NULL;
    }

    CIExpr *resolved =
      run__CIResolverExpr(resolver, init_clause->decl->variable.expr);

    if (!resolved) {
        return NULL;
    }

    // The counter is what the turns of the loop are counted with, so it
    // holds a number whatever else a value known before the program runs may
    // be.
    Isize value = to_literal_integer_value__CIResolverExpr(resolver, resolved);

    FREE(CIExpr, resolved);

    return NEW(CIComptimeBinding,
               init_clause->decl->variable.name,
               NEW_VARIANT(CIExpr,
                           literal,
                           clone__Location(&stmt->location),
                           NEW_VARIANT(CIExprLiteral, signed_int, value)));
}

/// @brief Write what the counter holds a step further on, which is a number
/// however a value known before the program runs may be written.
static void
step_counter_value__CIVisitor(CIComptimeBinding *counter, Isize step)
{
    Isize value = counter->value->kind == CI_EXPR_KIND_LITERAL
                    ? counter->value->literal.signed_int
                    : 0;
    Location location = clone__Location(&counter->value->location);

    FREE(CIExpr, counter->value);

    counter->value =
      NEW_VARIANT(CIExpr,
                  literal,
                  location,
                  NEW_VARIANT(CIExprLiteral, signed_int, value + step));
}

/// @brief Run the increment of an unrolled loop, and say what the counter
/// holds on the turn that follows.
///
/// Only what is written on the counter is run: `++i`, `i++`, `--i`, `i--` and
/// `i = <what is known>` say the whole of what the next turn holds, and
/// anything else is written on more than the loop is run from.
///
/// @return Whether the next turn is one the counter holds a known value on.
static bool
step_unroll_counter__CIVisitor(CIVisitor *self,
                               const CIStmt *stmt,
                               CIResolverExpr *resolver,
                               CIComptimeBinding *counter)
{
    const CIExpr *incr = get__Vec(stmt->for_.exprs2, 0);

    while (incr->kind == CI_EXPR_KIND_GROUPING) {
        incr = incr->grouping;
    }

    switch (incr->kind) {
        case CI_EXPR_KIND_UNARY:
            switch (incr->unary.kind) {
                case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
                case CI_EXPR_UNARY_KIND_POST_INCREMENT:
                    step_counter_value__CIVisitor(counter, 1);

                    return true;
                case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
                case CI_EXPR_UNARY_KIND_POST_DECREMENT:
                    step_counter_value__CIVisitor(counter, -1);

                    return true;
                default:
                    break;
            }

            break;
        case CI_EXPR_KIND_BINARY:
            if (incr->binary.kind == CI_EXPR_BINARY_KIND_ASSIGN) {
                CIExpr *resolved =
                  run__CIResolverExpr(resolver, incr->binary.right);

                if (!resolved) {
                    return false;
                }

                FREE(CIExpr, counter->value);

                counter->value = resolved;

                return true;
            }

            break;
        default:
            break;
    }

    FAILED__CIVisitor(
      self, stmt, CI_ERROR_KIND_UNROLLED_LOOP_COUNTER_IS_NOT_STEPPED);

    return false;
}

/// @brief Run an unrolled loop, and write the body it holds once per turn it
/// is run for.
///
/// The loop is run here rather than left to C: the counter is known on every
/// turn, so what is written on it is known too, and a param of a pack read at
/// that rank is read on the data type that rank holds. What is written in
/// place of the loop is the body it holds, once per turn, and C is given no
/// loop at all.
///
/// @return Vec<CIDeclFunctionItem*>*? The caller takes it over.
static Vec *
expand_unrolled_for__CIVisitor(CIVisitor *self,
                               const CIStmt *stmt,
                               CIGenericParams *decl_generic_params,
                               CIGenericParams *called_generic_params)
{
    CIResolverExpr resolver = NEW(CIResolverExpr,
                                  NULL,
                                  NULL,
                                  self->file,
                                  &self->file->file_analysis->count_error,
                                  false);

    set_comptime_env__CIResolverExpr(&resolver, self->comptime_env);

    CIComptimeBinding *counter =
      bind_unroll_counter__CIVisitor(self, stmt, &resolver);

    if (!counter) {
        return NULL;
    }

    // The counter is written on the loop alone, so what is known while it is
    // run is what was known around it and the counter on top of it.
    Vec *parent_env = self->comptime_env;
    Vec *env = NEW(Vec);

    if (parent_env) {
        for (Usize i = 0; i < parent_env->len; ++i) {
            push__Vec(env, get__Vec(parent_env, i));
        }
    }

    Vec *res = NEW(Vec); // Vec<CIDeclFunctionItem*>*
    Usize turns = 0;

    // What the condition is written on is read here rather than by the
    // resolver: `_Countof` is known once the types the declaration is called
    // on are, and the resolver has nothing to read of it. It says the same
    // thing on every turn, so it is read once - and it is read before the
    // counter is one of the names that are known, since what the counter
    // holds is what every turn reads of it rather than what the first one
    // does.
    CIExpr *cond = NULL;

    if (stmt->for_.expr1) {
        cond = fold_comptime_exprs__CIVisitor(
          self, stmt->for_.expr1, decl_generic_params, called_generic_params);

        if (!cond) {
            cond = ref__CIExpr(stmt->for_.expr1);
        }
    }

    push__Vec(env, counter);

    self->comptime_env = env;
    set_comptime_env__CIResolverExpr(&resolver, env);

    while (true) {
        // A loop written with no condition is one that does not end, as C
        // reads it, and there is nothing to run it a known number of turns
        // for.
        if (!cond) {
            FAILED__CIVisitor(
              self, stmt, CI_ERROR_KIND_UNROLLED_LOOP_DOES_NOT_END);

            break;
        }

        CIExpr *resolved_cond = run__CIResolverExpr(&resolver, cond);

        if (!resolved_cond) {
            break;
        }

        bool holds = is_true__CIResolverExpr(&resolver, resolved_cond);

        FREE(CIExpr, resolved_cond);

        if (!holds) {
            break;
        }

        if (++turns > CI_MAX_UNROLL_TURNS) {
            FAILED__CIVisitor(
              self, stmt, CI_ERROR_KIND_UNROLLED_LOOP_DOES_NOT_END);

            break;
        }

        CIDeclFunctionBody *copy = clone__CIDeclFunctionBody(stmt->for_.body);

        // The body is written on the counter this turn holds, so what is
        // written on it is read on that value rather than left to C.
        select_comptime_paths__CIVisitor(
          self, copy, decl_generic_params, called_generic_params);

        push__Vec(res,
                  NEW_VARIANT(CIDeclFunctionItem,
                              stmt,
                              NEW_VARIANT(CIStmt,
                                          block,
                                          clone__Location(&stmt->location),
                                          NEW(CIStmtBlock, copy))));

        // What the increment is written on is the counter, so running it is
        // what says the value the next turn holds.
        if (!stmt->for_.exprs2 || stmt->for_.exprs2->len != 1 ||
            !step_unroll_counter__CIVisitor(self, stmt, &resolver, counter)) {
            break;
        }
    }

    self->comptime_env = parent_env;

    if (cond) {
        FREE(CIExpr, cond);
    }

    FREE(Vec, env);
    FREE(CIComptimeBinding, counter);

    return res;
}

void
select_comptime_paths__CIVisitor(CIVisitor *self,
                                 CIDeclFunctionBody *body,
                                 CIGenericParams *decl_generic_params,
                                 CIGenericParams *called_generic_params)
{
    for (Usize i = 0; i < body->content->len; ++i) {
        CIDeclFunctionItem *item = get__Vec(body->content, i);

        if (item->kind == CI_DECL_FUNCTION_ITEM_KIND_EXPR) {
            fold_comptime_expr_slot__CIVisitor(
              self, &item->expr, decl_generic_params, called_generic_params);

            continue;
        }

        // What a variable is given is read the same way as anything else
        // written as an expression: a comparison of data types written there
        // is known too, and is written as the 1 or the 0 it stands for.
        if (item->kind == CI_DECL_FUNCTION_ITEM_KIND_DECL) {
            if (item->decl->kind == CI_DECL_KIND_VARIABLE) {
                fold_comptime_expr_slot__CIVisitor(self,
                                                   &item->decl->variable.expr,
                                                   decl_generic_params,
                                                   called_generic_params);
                // How long an array is written to be is read the same way as
                // anything else written as an expression, so a length written
                // on a value that is known says the number it stands for
                // rather than a length the program reads.
                fold_array_size__CIVisitor(self,
                                           item->decl->variable.data_type,
                                           decl_generic_params,
                                           called_generic_params);
            }

            continue;
        }

        if (item->kind != CI_DECL_FUNCTION_ITEM_KIND_STMT) {
            continue;
        }

        switch (item->stmt.kind) {
            case CI_STMT_KIND_IF: {
                bool is_known = false;
                CIDeclFunctionBody *path =
                  select_comptime_path__CIVisitor(self,
                                                  &item->stmt.if_,
                                                  decl_generic_params,
                                                  called_generic_params,
                                                  &is_known);

                if (!is_known) {
                    // The condition is read while the program runs, so both
                    // paths are written and each is read on its own. What is
                    // written on data types in it is still known, so it is
                    // written as what it stands for.
                    fold_comptime_expr_slot__CIVisitor(
                      self,
                      &item->stmt.if_.if_->cond,
                      decl_generic_params,
                      called_generic_params);
                    select_comptime_paths__CIVisitor(self,
                                                     item->stmt.if_.if_->body,
                                                     decl_generic_params,
                                                     called_generic_params);

                    for (Usize j = 0; item->stmt.if_.else_ifs &&
                                      j < item->stmt.if_.else_ifs->len;
                         ++j) {
                        CIStmtIfBranch *else_if =
                          get__Vec(item->stmt.if_.else_ifs, j);

                        fold_comptime_expr_slot__CIVisitor(
                          self,
                          &else_if->cond,
                          decl_generic_params,
                          called_generic_params);
                        select_comptime_paths__CIVisitor(self,
                                                         else_if->body,
                                                         decl_generic_params,
                                                         called_generic_params);
                    }

                    if (item->stmt.if_.else_) {
                        select_comptime_paths__CIVisitor(self,
                                                         item->stmt.if_.else_,
                                                         decl_generic_params,
                                                         called_generic_params);
                    }

                    break;
                }

                Location location = clone__Location(&item->stmt.location);

                FREE(CIDeclFunctionItem, item);

                if (path) {
                    // The path the statement holds is written as a block, so
                    // what it declares is written where it was.
                    select_comptime_paths__CIVisitor(
                      self, path, decl_generic_params, called_generic_params);
                    replace__Vec(
                      body->content,
                      i,
                      NEW_VARIANT(
                        CIDeclFunctionItem,
                        stmt,
                        NEW_VARIANT(
                          CIStmt, block, location, NEW(CIStmtBlock, path))));
                } else {
                    // The statement holds no path at all, so nothing is
                    // written where it was.
                    remove__Vec(body->content, i);
                    --i;
                }

                break;
            }
            case CI_STMT_KIND_BLOCK:
                select_comptime_paths__CIVisitor(self,
                                                 item->stmt.block.body,
                                                 decl_generic_params,
                                                 called_generic_params);

                break;
            case CI_STMT_KIND_DO_WHILE:
                fold_comptime_expr_slot__CIVisitor(self,
                                                   &item->stmt.do_while.cond,
                                                   decl_generic_params,
                                                   called_generic_params);
                select_comptime_paths__CIVisitor(self,
                                                 item->stmt.do_while.body,
                                                 decl_generic_params,
                                                 called_generic_params);

                break;
            case CI_STMT_KIND_FOR: {
                // A loop written `inline` is run here rather than by the
                // program, and what is written in its place is the body it
                // holds, once per turn it is run for.
                if (item->stmt.for_.is_unrolled) {
                    Vec *expanded =
                      expand_unrolled_for__CIVisitor(self,
                                                     &item->stmt,
                                                     decl_generic_params,
                                                     called_generic_params);

                    remove__Vec(body->content, i);
                    FREE(CIDeclFunctionItem, item);

                    if (expanded) {
                        // The turns have been read as they were written, and
                        // what follows them is what is read next.
                        i +=
                          splice_items__CIVisitor(body->content, expanded, i);

                        FREE(Vec, expanded);
                    }

                    --i;

                    break;
                }

                fold_comptime_expr_slot__CIVisitor(self,
                                                   &item->stmt.for_.expr1,
                                                   decl_generic_params,
                                                   called_generic_params);
                select_comptime_paths__CIVisitor(self,
                                                 item->stmt.for_.body,
                                                 decl_generic_params,
                                                 called_generic_params);

                break;
            }
            case CI_STMT_KIND_RETURN:
                fold_comptime_expr_slot__CIVisitor(self,
                                                   &item->stmt.return_,
                                                   decl_generic_params,
                                                   called_generic_params);

                break;
            case CI_STMT_KIND_SWITCH:
                fold_comptime_expr_slot__CIVisitor(self,
                                                   &item->stmt.switch_.expr,
                                                   decl_generic_params,
                                                   called_generic_params);
                select_comptime_paths__CIVisitor(self,
                                                 item->stmt.switch_.body,
                                                 decl_generic_params,
                                                 called_generic_params);

                break;
            case CI_STMT_KIND_WHILE:
                fold_comptime_expr_slot__CIVisitor(self,
                                                   &item->stmt.while_.cond,
                                                   decl_generic_params,
                                                   called_generic_params);
                select_comptime_paths__CIVisitor(self,
                                                 item->stmt.while_.body,
                                                 decl_generic_params,
                                                 called_generic_params);

                break;
            default:
                break;
        }
    }
}

void
visit_function__CIVisitor(CIVisitor *self,
                          const CIDecl *decl,
                          CIGenericParams *called_generic_params,
                          const CIDeclFunctionBody *body)
{
    ASSERT(decl->kind == CI_DECL_KIND_FUNCTION);

    visit_function_return_data_type__CIVisitor(
      self, decl, called_generic_params);
    visit_function_params__CIVisitor(self, decl, called_generic_params);
    // The body the declaration holds is the one that is read, since a path
    // it does not hold is one nothing is read of.
    visit_function_body__CIVisitor(self,
                                   body ? body : decl->function.body,
                                   called_generic_params,
                                   decl->function.generic_params);
}

void
visit_non_generic_function__CIVisitor(CIVisitor *self, CIDecl *decl)
{
    self->current_decl = decl;

    ASSERT(decl->kind == CI_DECL_KIND_FUNCTION);

    // A comparison of data types is known before the program runs wherever it
    // is written, and C has nothing to read of it. A declaration written on no
    // generic has one body, which is the one that is written out, so the paths
    // it holds are read in it rather than in a body instantiated from it.
    if (decl->function.body) {
        select_comptime_paths__CIVisitor(self, decl->function.body, NULL, NULL);
    }

    visit_function_return_data_type__CIVisitor(self, decl, NULL);
    visit_function_params__CIVisitor(self, decl, NULL);
    visit_function_body__CIVisitor(self, decl->function.body, NULL, NULL);
}

void
visit_global_decl__CIVisitor(CIVisitor *self, CIDecl *decl)
{
    if (has_generic__CIDecl(decl) || is_prototype__CIDecl(decl) ||
        is_local__CIDecl(decl)) {
        return;
    }

    switch (decl->kind) {
        case CI_DECL_KIND_ENUM:
        case CI_DECL_KIND_ENUM_VARIANT:
        case CI_DECL_KIND_LABEL:
            // NOTE: Not have to visit this declaration.
            break;
        case CI_DECL_KIND_FUNCTION:
            visit_non_generic_function__CIVisitor(self, decl);

            break;
        case CI_DECL_KIND_STRUCT:
        case CI_DECL_KIND_UNION:
            visit_non_generic_struct_or_union__CIVisitor(self, decl);

            break;
        case CI_DECL_KIND_TYPEDEF:
            visit_non_generic_typedef__CIVisitor(self, decl);

            break;
        case CI_DECL_KIND_VARIABLE:
            visit_non_generic_variable__CIVisitor(self, decl);

            break;
        default:
            break;
    }
}

void
set_file__CIVisitor(CIVisitor *self, const CIResultFile *file)
{
    self->file = file;
    self->current_scope = self->file->scope_base;
}

void
unset_file__CIVisitor(CIVisitor *self)
{
    self->file = NULL;
    self->current_scope = NULL;
}

void
visit_global_decls__CIVisitor(CIVisitor *self)
{
    CIDecl *decl = NULL;
    VecIter iter_decl = NEW(VecIter, self->file->entity.decls);

    while ((decl = next__VecIter(&iter_decl))) {
        visit_global_decl__CIVisitor(self, decl);
    }
}

void
run_file__CIVisitor(CIVisitor *self, const CIResultFile *file)
{
    set_file__CIVisitor(self, file);
    visit_global_decls__CIVisitor(self);
    unset_file__CIVisitor(self);
}

void
handler__CIVisitor([[maybe_unused]] void *entity,
                   const CIResultFile *file,
                   void *other_args)
{
    CIVisitor *self = (CIVisitor *)other_args;

    run_file__CIVisitor(self, file);
}

void
run__CIVisitor(CIVisitor *self)
{
    pass_through_result__CIResult(self->result, &handler__CIVisitor, self);
}
