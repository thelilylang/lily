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
#include <base/vec.h>

#include <core/cc/ci/ast.h>
#include <core/cc/ci/typecheck.h>
#include <core/cc/ci/visitor.h>

inline bool
is_in_function_body__CIVisitor(CIVisitor *self);

static void
generate_function_gen__CIVisitor(CIVisitor *self,
                                 String *function_name,
                                 CIGenericParams *called_generic_params);

/// @brief Generate struct, union or typedef gen.
/// @param name String* (&)
static void
generate_type_gen__CIVisitor(CIVisitor *self,
                             enum CIDeclKind kind,
                             String *name,
                             CIGenericParams *called_generic_params,
                             CIDecl *(*search_decl)(const CIResultFile *,
                                                    const String *));

/// @param struct_name String* (&)
static inline void
generate_struct_gen__CIVisitor(CIVisitor *self,
                               String *struct_name,
                               CIGenericParams *called_generic_params);

/// @param union_name String* (&)
static inline void
generate_union_gen__CIVisitor(CIVisitor *self,
                              String *union_name,
                              CIGenericParams *called_generic_params);

/// @param typedef_name_ref String* (&)
static inline void
generate_typedef_gen__CIVisitor(CIVisitor *self,
                                String *typedef_name_ref,
                                CIGenericParams *called_generic_params);

static void
generate_from_data_type__CIVisitor(CIVisitor *self, CIDataType *data_type);

static CIDataType *
substitute_and_generate_from_data_type__CIVisitor(
  CIVisitor *self,
  CIDataType *data_type,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params);

/// @brief If the struct or union is not generic, this function will return
/// NULL.
/// @return the generated fields.
/// @return Vec<CIDataType*>*?
static Vec *
visit_struct_or_union__CIVisitor(CIVisitor *self,
                                 const CIDecl *decl,
                                 CIGenericParams *called_generic_params);

static void
visit_non_generic_struct_or_union__CIVisitor(CIVisitor *self,
                                             const CIDecl *decl);

/// @brief If the typedef is not generic, this function will return NULL.
/// @return the generated data type.
/// e.g. typedef <data_type> <name>;
/// @return CIDataType*?
static CIDataType *
visit_typedef__CIVisitor(CIVisitor *self,
                         const CIDecl *decl,
                         CIGenericParams *called_generic_params);

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
visit_function_expr_array__CIVisitor(CIVisitor *self,
                                     const CIExprArray *array,
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
visit_function_expr_struct_call__CIVisitor(
  CIVisitor *self,
  const CIExprStructCall *struct_call,
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

static void
visit_function_body__CIVisitor(CIVisitor *self,
                               const CIDeclFunctionBody *body,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params);

static void
visit_function__CIVisitor(CIVisitor *self,
                          const CIDecl *decl,
                          CIGenericParams *called_generic_params);

static void
visit_non_generic_function__CIVisitor(CIVisitor *self, const CIDecl *decl);

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

bool
is_in_function_body__CIVisitor(CIVisitor *self)
{
    ASSERT(self->file);

    return self->file->scope_base != self->current_scope;
}

void
generate_function_gen__CIVisitor(CIVisitor *self,
                                 String *function_name,
                                 CIGenericParams *called_generic_params)
{
    const CIDecl *function_decl =
      search_function__CIResultFile(self->file, function_name);

    if (!function_decl) {
        FAILED("unknown function, impossible to call unknown function");
    }

    // Generate gen function declaration
    if (called_generic_params &&
        !has_generic__CIGenericParams(called_generic_params)) {
        if (function_decl->is_prototype) {
            FAILED("expected to have the definition of the declaration at this "
                   "point");
        } else {
            String *serialized_called_function_name =
              serialize_name__CIDeclFunction(&function_decl->function,
                                             called_generic_params);
            const CIDecl *function_gen = search_function__CIResultFile(
              self->file, serialized_called_function_name);

            if (!function_gen) {
                CIDataType *subs_return_data_type =
                  substitute_data_type__CIParser(
                    function_decl->function.return_data_type,
                    function_decl->function.generic_params,
                    called_generic_params);

                visit_function__CIVisitor(
                  self, function_decl, called_generic_params);

                CIDecl *function_gen_decl =
                  NEW_VARIANT(CIDecl,
                              function_gen,
                              (CIDecl *)function_decl,
                              ref__CIGenericParams(called_generic_params),
                              serialized_called_function_name,
                              subs_return_data_type ? subs_return_data_type
                                                    : ref__CIDataType(
                                                        function_decl->function
                                                          .return_data_type) /* Return a ref data type, when the substituted data type is NULL, to avoid an optional data type in the `return_data_type` field. */);

                add_decl_to_scope__CIResultFile(
                  self->file,
                  &function_gen_decl,
                  self->current_scope,
                  true,
                  is_in_function_body__CIVisitor(self));
            }
        }
    }
}

void
generate_type_gen__CIVisitor(CIVisitor *self,
                             enum CIDeclKind kind,
                             String *name,
                             CIGenericParams *called_generic_params,
                             CIDecl *(*search_decl)(const CIResultFile *,
                                                    const String *))
{
    CIDecl *decl = search_decl(self->file, name);

    if (!decl) {
        FAILED("struct, typedef or union not found");
    }

    ASSERT(kind == decl->kind);

    if (called_generic_params &&
        !has_generic__CIGenericParams(called_generic_params)) {
        if (decl->is_prototype && !is_typedef__CIDecl(decl)) {
            FAILED("expected to have the definition of the declaration at this "
                   "point");
        } else {
            String *serialized_called_decl_name = NULL;

            switch (decl->kind) {
                case CI_DECL_KIND_STRUCT:
                    serialized_called_decl_name = serialize_name__CIDeclStruct(
                      &decl->struct_, called_generic_params);

                    break;
                case CI_DECL_KIND_TYPEDEF:
                    serialized_called_decl_name = serialize_name__CIDeclTypedef(
                      &decl->typedef_, called_generic_params);

                    break;
                case CI_DECL_KIND_UNION:
                    serialized_called_decl_name = serialize_name__CIDeclUnion(
                      &decl->union_, called_generic_params);

                    break;
                default:
                    UNREACHABLE("this kind of variant is not exepcted");
            }

            CIDecl *decl_gen =
              search_decl(self->file, serialized_called_decl_name);

            if (!decl_gen) {
                switch (decl->kind) {
                    case CI_DECL_KIND_STRUCT: {
                        Vec *fields = visit_struct_or_union__CIVisitor(
                          self, decl, called_generic_params);

                        if (fields) {
                            decl_gen = NEW_VARIANT(
                              CIDecl,
                              struct_gen,
                              decl,
                              ref__CIGenericParams(called_generic_params),
                              serialized_called_decl_name,
                              fields);

                            break;
                        }

                        FREE(String, serialized_called_decl_name);

                        return;
                    }
                    case CI_DECL_KIND_TYPEDEF: {
                        CIDataType *data_type = visit_typedef__CIVisitor(
                          self, decl, called_generic_params);

                        if (data_type) {
                            decl_gen = NEW_VARIANT(
                              CIDecl,
                              typedef_gen,
                              decl,
                              ref__CIGenericParams(called_generic_params),
                              serialized_called_decl_name,
                              data_type);

                            break;
                        }

                        FREE(String, serialized_called_decl_name);

                        return;
                    }
                    case CI_DECL_KIND_UNION: {
                        Vec *fields = visit_struct_or_union__CIVisitor(
                          self, decl, called_generic_params);

                        if (fields) {
                            decl_gen = NEW_VARIANT(
                              CIDecl,
                              union_gen,
                              decl,
                              ref__CIGenericParams(called_generic_params),
                              serialized_called_decl_name,
                              fields);

                            break;
                        }

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
        }
    }
}

void
generate_struct_gen__CIVisitor(CIVisitor *self,
                               String *struct_name_ref,
                               CIGenericParams *called_generic_params)
{
    generate_type_gen__CIVisitor(self,
                                 CI_DECL_KIND_STRUCT,
                                 struct_name_ref,
                                 called_generic_params,
                                 &search_struct__CIResultFile);
}

void
generate_union_gen__CIVisitor(CIVisitor *self,
                              String *union_name_ref,
                              CIGenericParams *called_generic_params)
{
    generate_type_gen__CIVisitor(self,
                                 CI_DECL_KIND_UNION,
                                 union_name_ref,
                                 called_generic_params,
                                 &search_union__CIResultFile);
}

void
generate_typedef_gen__CIVisitor(CIVisitor *self,
                                String *typedef_name_ref,
                                CIGenericParams *called_generic_params)
{
    generate_type_gen__CIVisitor(self,
                                 CI_DECL_KIND_TYPEDEF,
                                 typedef_name_ref,
                                 called_generic_params,
                                 &search_typedef__CIResultFile);
}

void
generate_from_data_type__CIVisitor(CIVisitor *self, CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
            if (data_type->struct_.name) {
                generate_struct_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->struct_.name),
                  data_type->struct_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_UNION:
            if (data_type->union_.name) {
                generate_union_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->union_.name),
                  data_type->union_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            generate_typedef_gen__CIVisitor(
              self,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params);

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
  CIGenericParams *called_generic_params)
{
    CIDataType *subs_data_type = substitute_data_type__CIParser(
      data_type, generic_params, called_generic_params);

    generate_from_data_type__CIVisitor(self, subs_data_type);

    return subs_data_type;
}

Vec *
visit_struct_or_union__CIVisitor(CIVisitor *self,
                                 const CIDecl *decl,
                                 CIGenericParams *called_generic_params)
{
    ASSERT(decl->kind == CI_DECL_KIND_STRUCT ||
           decl->kind == CI_DECL_KIND_UNION);

    if (has_generic__CIDecl(decl)) {
        const Vec *fields = get_fields__CIDecl(decl);

        if (!fields) {
            return NULL;
        }

        CIGenericParams *generic_params = get_generic_params__CIDecl(decl);
        Vec *gen_fields = NEW(Vec);

        for (Usize i = 0; i < fields->len; ++i) {
            const CIDeclStructField *field = get__Vec(fields, i);
            CIDataType *subs_data_type =
              substitute_and_generate_from_data_type__CIVisitor(
                self, field->data_type, generic_params, called_generic_params);

            if (subs_data_type) {
                push__Vec(gen_fields,
                          NEW(CIDeclStructField,
                              field->name,
                              subs_data_type,
                              field->bit));
            }
        }

        return gen_fields;
    }

    return NULL;
}

void
visit_non_generic_struct_or_union__CIVisitor(CIVisitor *self,
                                             const CIDecl *decl)
{
    ASSERT(decl->kind == CI_DECL_KIND_STRUCT ||
           decl->kind == CI_DECL_KIND_UNION);

    const Vec *fields = get_fields__CIDecl(decl);

    if (!fields) {
        return;
    }

    for (Usize i = 0; i < fields->len; ++i) {
        const CIDeclStructField *field = get__Vec(fields, i);

        generate_from_data_type__CIVisitor(self, field->data_type);
    }
}

CIDataType *
visit_typedef__CIVisitor(CIVisitor *self,
                         const CIDecl *decl,
                         CIGenericParams *called_generic_params)
{
    ASSERT(decl->kind == CI_DECL_KIND_TYPEDEF);

    return has_generic__CIDecl(decl)
             ? substitute_and_generate_from_data_type__CIVisitor(
                 self,
                 decl->typedef_.data_type,
                 decl->typedef_.generic_params,
                 called_generic_params)
             : NULL;
}

void
visit_non_generic_typedef__CIVisitor(CIVisitor *self, const CIDecl *decl)
{
    ASSERT(decl->kind == CI_DECL_KIND_TYPEDEF);

    generate_from_data_type__CIVisitor(self, decl->typedef_.data_type);
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
        for (Usize i = 0; i < function->params->len; ++i) {
            CIDeclFunctionParam *param = get__Vec(function->params, i);

            visit_data_type__CIVisitor(self,
                                       param->data_type,
                                       called_generic_params,
                                       decl_generic_params);
        }
    }

    if (function->function_data_type) {
        visit_data_type__CIVisitor(self,
                                   function->function_data_type,
                                   called_generic_params,
                                   decl_generic_params);
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
            if (data_type->ptr) {
                visit_data_type__CIVisitor(self,
                                           data_type->ptr,
                                           called_generic_params,
                                           decl_generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            if (data_type->struct_.name) {
                generate_struct_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->struct_.name),
                  data_type->struct_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            generate_typedef_gen__CIVisitor(
              self,
              GET_PTR_RC(String, data_type->typedef_.name),
              called_generic_params);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            if (data_type->union_.name) {
                generate_union_gen__CIVisitor(
                  self,
                  GET_PTR_RC(String, data_type->union_.name),
                  data_type->union_.generic_params);
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
visit_function_expr_array__CIVisitor(CIVisitor *self,
                                     const CIExprArray *array,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < array->array->len; ++i) {
        visit_function_expr__CIVisitor(self,
                                       get__Vec(array->array, i),
                                       called_generic_params,
                                       decl_generic_params);
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
    if (function_call->generic_params) {
        generate_function_gen__CIVisitor(
          self,
          GET_PTR_RC(String, function_call->identifier),
          function_call->generic_params);
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
visit_function_expr_struct_call__CIVisitor(
  CIVisitor *self,
  const CIExprStructCall *struct_call,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < struct_call->fields->len; ++i) {
        CIExprStructFieldCall *field_call = get__Vec(struct_call->fields, i);

        visit_function_expr__CIVisitor(
          self, field_call->value, called_generic_params, decl_generic_params);
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
        case CI_EXPR_KIND_ARRAY:
            visit_function_expr_array__CIVisitor(
              self, &expr->array, called_generic_params, decl_generic_params);

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
        case CI_EXPR_KIND_LITERAL:
            break;
        case CI_EXPR_KIND_STRUCT_CALL:
            visit_function_expr_struct_call__CIVisitor(self,
                                                       &expr->struct_call,
                                                       called_generic_params,
                                                       decl_generic_params);

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
        case CI_STMT_KIND_GOTO:
            break;
        case CI_STMT_KIND_IF:
            visit_function_stmt_if__CIVisitor(
              self, &stmt->if_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_RETURN:
            visit_function_expr__CIVisitor(
              self, stmt->return_, called_generic_params, decl_generic_params);

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
visit_function_body__CIVisitor(CIVisitor *self,
                               const CIDeclFunctionBody *body,
                               CIGenericParams *called_generic_params,
                               CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < body->content->len; ++i) {
        visit_function_item__CIVisitor(self,
                                       get__Vec(body->content, i),
                                       called_generic_params,
                                       decl_generic_params);
    }
}

void
visit_function__CIVisitor(CIVisitor *self,
                          const CIDecl *decl,
                          CIGenericParams *called_generic_params)
{
    ASSERT(decl->kind == CI_DECL_KIND_FUNCTION);

    visit_function_body__CIVisitor(self,
                                   decl->function.body,
                                   called_generic_params,
                                   decl->function.generic_params);
}

void
visit_non_generic_function__CIVisitor(CIVisitor *self, const CIDecl *decl)
{
    ASSERT(decl->kind == CI_DECL_KIND_FUNCTION);

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
run__CIVisitor(CIVisitor *self)
{
    // TODO: Merge that code in one step with the generator
    OrderedHashMapIter iter_libs = NEW(OrderedHashMapIter, self->result->libs);
    OrderedHashMapIter iter_bins = NEW(OrderedHashMapIter, self->result->bins);
    CIResultLib *current_lib = NULL;
    CIResultBin *current_bin = NULL;

    while ((current_lib = next__OrderedHashMapIter(&iter_libs))) {
        run_file__CIVisitor(self, current_lib->file);
    }

    while ((current_bin = next__OrderedHashMapIter(&iter_bins))) {
        run_file__CIVisitor(self, current_bin->file);
    }
}