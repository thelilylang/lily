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

#include <base/assert.h>
#include <base/dir.h>

#include <core/cc/ci/generator.h>

static void
start_session__CIGeneratorContent(CIGeneratorContent *self,
                                  CIScope *current_scope);

static void
end_session__CIGeneratorContent(CIGeneratorContent *self);

static void
end_session_by_cascade__CIGeneratorContent(CIGeneratorContent *self);

/// @param parent_scope CIScope* (&)* (&)
static void
set_new_scope__CIGeneratorContent(CIGeneratorContent *self,
                                  CIScope **parent_scope,
                                  CIScopeID *new_scope_id,
                                  const CIResultFile *file);

static void
restore_scope__CIGeneratorContent(CIGeneratorContent *self, CIScope *scope);

static inline void
set_must_inherit__CIGeneratorContent(CIGeneratorContent *self);

static inline void
unset_must_inherit__CIGeneratorContent(CIGeneratorContent *self);

static void
set_current_generic_params__CIGeneratorContent(
  CIGeneratorContent *self,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params);

static void
reset_current_generic_params__CIGeneratorContent(CIGeneratorContent *self);

static inline void
write__CIGeneratorContent(CIGeneratorContent *self, char c);

/// @param s char* (&)
static inline void
write_str__CIGeneratorContent(CIGeneratorContent *self, char *s);

/// @param s String*
static void
write_String__CIGeneratorContent(CIGeneratorContent *self, String *s);

static inline void
write_tab__CIGeneratorContent(CIGeneratorContent *self);

static inline void
inc_tab_count__CIGeneratorContent(CIGeneratorContent *self);

static inline void
dec_tab_count__CIGeneratorContent(CIGeneratorContent *self);

static inline void
set_write_semicolon__CIGeneratorContent(CIGeneratorContent *self);

static inline void
unset_write_semicolon__CIGeneratorContent(CIGeneratorContent *self);

static inline void
start_session_with_default_scope__CIGenerator(CIGenerator *self);

static inline void
end_session__CIGenerator(CIGenerator *self);

static inline void
end_session_by_cascade__CIGenerator(CIGenerator *self);

static void
end_session_by_decl__CIGenerator(CIGenerator *self, const CIDecl *decl);

static inline void
set_new_scope__CIGenerator(CIGenerator *self,
                           CIScope **parent_scope,
                           CIScopeID *new_scope_id);

static inline void
restore_scope__CIGenerator(CIGenerator *self, CIScope *scope);

static inline void
set_must_inherit__CIGenerator(CIGenerator *self);

static inline void
unset_must_inherit__CIGenerator(CIGenerator *self);

static inline void
set_current_generic_params__CIGenerator(CIGenerator *self,
                                        CIGenericParams *generic_params,
                                        CIGenericParams *called_generic_params);

static inline void
reset_current_generic_params__CIGenerator(CIGenerator *self);

static inline void
write__CIGenerator(CIGenerator *self, char c);

static inline void
write_str__CIGenerator(CIGenerator *self, char *s);

static inline void
write_String__CIGenerator(CIGenerator *self, String *s);

static inline void
inc_tab_count__CIGenerator(CIGenerator *self);

static inline void
dec_tab_count__CIGenerator(CIGenerator *self);

static inline void
write_tab__CIGenerator(CIGenerator *self);

static inline void
set_write_semicolon__CIGenerator(CIGenerator *self);

static inline void
unset_write_semicolon__CIGenerator(CIGenerator *self);

/// @param unresolved_generic_params CIGenericParams* (&)
/// @param name const String* (&)
/// @return String*
static String *
substitute_and_serialize_generic_params__CIGenerator(
  CIGenerator *self,
  CIGenericParams *unresolved_generic_params,
  const String *name);

static void
generate_data_type_qualifier__CIGenerator(CIGenerator *self,
                                          const int *data_type_qualifier);

/// @return CIDataType*?
static CIDataType *
substitute_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type);

/// @param name const String*? (&)
static void
search_decl_and_generate_base__CIGenerator(
  CIGenerator *self,
  const String *name,
  CIDecl *(*search)(const CIResultFile *self, const String *name));

/// @param name const String*? (&)
static inline void
search_enum_decl_and_generate__CIGenerator(CIGenerator *self,
                                           const String *name);

/// @param name const String*? (&)
static inline void
search_struct_decl_and_generate__CIGenerator(CIGenerator *self,
                                             const String *name);

/// @param name const String*? (&)
static inline void
search_union_decl_and_generate__CIGenerator(CIGenerator *self,
                                            const String *name);

/// @param name const String*? (&)
static inline void
search_typedef_decl_and_generate__CIGenerator(CIGenerator *self,
                                              const String *name);

static void
generate_pre_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type);

static void
generate_post_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type);

static void
generate_array_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type);

static void
generate_ptr_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type);

/// @param data_type CIDataType* (&)
/// @param function_data_type CIDataType* (&)
static void
generate_function_data_type__CIGenerator(CIGenerator *self,
                                         CIDataType *data_type,
                                         CIDataType *function_data_type);

static void
generate_data_type_base__CIGenerator(CIGenerator *self,
                                     CIDataType *data_type,
                                     bool is_on_top);

static void
generate_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type);

/// @param storage_class_flag const int* (&)
static void
generate_storage_class__CIGenerator(CIGenerator *self,
                                    const int *storage_class_flag);

static void
generate_attribute_standard__CIGenerator(
  CIGenerator *self,
  const CIAttributeStandard *attribute_standard);

static void
generate_attribute__CIGenerator(CIGenerator *self,
                                const CIAttribute *attribute);

/// @param attributes const Vec<CIAttribute*>*? (&)
static void
generate_attributes__CIGenerator(CIGenerator *self, const Vec *attributes);

static void
generate_attributes_by_decl__CIGenerator(CIGenerator *self, const CIDecl *decl);

static inline Usize
get_decl_id_from_decl__CIGenerator(CIGenerator *self, const CIDecl *decl);

/// @param const CIDeclEnumVariant* (&)
static void
generate_enum_variant__CIGenerator(CIGenerator *self,
                                   const CIDeclEnumVariant *enum_variant);

/// @param enum_variants Vec<CIDeclEnumVariant*>* (&)
static void
generate_enum_variants__CIGenerator(CIGenerator *self,
                                    const Vec *enum_variants);

/// @param name String*? (&)
/// @param data_type CIDataType*? (&)
/// @param variants Vec<CIDeclEnumVariant*>*? (&)
static void
generate_enum__CIGenerator(CIGenerator *self,
                           String *name,
                           CIDataType *data_type,
                           Vec *variants);

static void
generate_enum_prototype__CIGenerator(CIGenerator *self,
                                     const CIDeclEnum *enum_);

static inline void
generate_enum_decl__CIGenerator(CIGenerator *self, const CIDeclEnum *enum_);

static void
generate_function_params__CIGenerator(CIGenerator *self,
                                      const CIDeclFunctionParams *params);

static void
generate_function_prototype__CIGenerator(CIGenerator *self,
                                         const CIDeclFunction *function);

static void
generate_function_binary_expr__CIGenerator(CIGenerator *self,
                                           const CIExprBinary *binary);

/// @param params Vec<CIExpr*>* (&)
static void
generate_function_call_params_expr__CIGenerator(CIGenerator *self, Vec *params);

static void
generate_function_call_expr__CIGenerator(
  CIGenerator *self,
  const CIExprFunctionCall *function_call);

static void
generate_function_call_builtin_expr__CIGenerator(
  CIGenerator *self,
  const CIExprFunctionCallBuiltin *function_call_builtin);

static void
generate_function_literal_character_expr__CIGenerator(CIGenerator *self,
                                                      const char c);

static void
generate_function_literal_string_expr__CIGenerator(CIGenerator *self,
                                                   const String *string);

static void
generate_function_literal_expr__CIGenerator(CIGenerator *self,
                                            const CIExprLiteral *literal);

static void
generate_function_initializer_expr__CIGenerator(
  CIGenerator *self,
  const CIExprInitializer *initializer);

static void
generate_function_unary_expr__CIGenerator(CIGenerator *self,
                                          const CIExprUnary *unary);

static void
generate_function_expr__CIGenerator(CIGenerator *self, const CIExpr *expr);

static inline void
generate_function_block_stmt__CIGenerator(CIGenerator *self,
                                          const CIStmtBlock *block);

static void
generate_case_stmt__CIGenerator(CIGenerator *self,
                                const CIStmtSwitchCase *case_);

static void
generate_function_do_while_stmt__CIGenerator(CIGenerator *self,
                                             const CIStmtDoWhile *do_while);

static void
generate_function_for_stmt__CIGenerator(CIGenerator *self,
                                        const CIStmtFor *for_);

static void
generate_function_if_stmt_branch__CIGenerator(CIGenerator *self,
                                              const CIStmtIfBranch *if_branch);

static void
generate_function_if_stmt__CIGenerator(CIGenerator *self, const CIStmtIf *if_);

static void
generate_function_switch_stmt__CIGenerator(CIGenerator *self,
                                           const CIStmtSwitch *switch_);

static void
generate_function_while_stmt__CIGenerator(CIGenerator *self,
                                          const CIStmtWhile *while_);

static void
generate_function_stmt__CIGenerator(CIGenerator *self, const CIStmt *stmt);

static void
generate_function_body_item__CIGenerator(CIGenerator *self,
                                         const CIDeclFunctionItem *item);

static void
generate_function_body__CIGenerator(CIGenerator *self,
                                    const CIDeclFunctionBody *body);

static void
generate_function_decl__CIGenerator(CIGenerator *self,
                                    const CIDeclFunction *function);

static void
generate_function_gen_prototype__CIGenerator(
  CIGenerator *self,
  const CIDeclFunctionGen *function_gen);

static void
generate_function_gen_decl__CIGenerator(CIGenerator *self,
                                        const CIDeclFunctionGen *function_gen);

static void
generate_label_decl__CIGenerator(CIGenerator *self, const CIDeclLabel *label);

static void
generate_struct_prototype__CIGenerator(CIGenerator *self,
                                       const CIDeclStruct *struct_);

static void
generate_struct_field__CIGenerator(CIGenerator *self,
                                   const CIDeclStructField *field);

static void
generate_struct_fields__CIGenerator(CIGenerator *self,
                                    const CIDeclStructFields *fields);

/// @param name String*? (&)
/// @param fields CIDeclStructFields* (&)
static void
generate_struct__CIGenerator(CIGenerator *self,
                             String *name,
                             CIDeclStructFields *fields);

static inline void
generate_struct_decl__CIGenerator(CIGenerator *self,
                                  const CIDeclStruct *struct_);

static void
generate_struct_gen_prototype__CIGenerator(CIGenerator *self,
                                           const CIDeclStructGen *struct_gen);

static void
generate_struct_gen_decl__CIGenerator(CIGenerator *self,
                                      const CIDeclStructGen *struct_gen);

static void
generate_typedef_decl__CIGenerator(CIGenerator *self,
                                   const CIDeclTypedef *typedef_);

static void
generate_typedef_gen_decl__CIGenerator(CIGenerator *self,
                                       const CIDeclTypedefGen *typedef_gen);

static void
generate_union_prototype__CIGenerator(CIGenerator *self,
                                      const CIDeclUnion *union_);

static void
generate_union__CIGenerator(CIGenerator *self,
                            String *name,
                            CIDeclStructFields *fields);

static void
generate_union_decl__CIGenerator(CIGenerator *self, const CIDeclUnion *union_);

static void
generate_union_gen_prototype__CIGenerator(CIGenerator *self,
                                          const CIDeclUnionGen *union_gen);

static void
generate_union_gen_decl__CIGenerator(CIGenerator *self,
                                     const CIDeclUnionGen *union_gen);

static void
generate_variable_decl__CIGenerator(CIGenerator *self,
                                    const CIDeclVariable *variable);

static void
generate_decl__CIGenerator(CIGenerator *self, const CIDecl *decl);

static void
generate_decl_prototype__CIGenerator(CIGenerator *self, const CIDecl *decl);

static void
generate_global_decls__CIGenerator(CIGenerator *self);

static void
generate_global_decls_prototype__CIGenerator(CIGenerator *self);

static void
run_file__CIGenerator(CIGenerator *self);

static void
handler__CIGenerator([[maybe_unused]] void *entity,
                     const CIResultFile *file,
                     [[maybe_unused]] void *other_args);

CONSTRUCTOR(CIGeneratorContentSession *,
            CIGeneratorContentSession,
            CIScope *current_scope)
{
    CIGeneratorContentSession *self =
      lily_malloc(sizeof(CIGeneratorContentSession));

    self->buffer = NEW(String);
    self->must_inherit = false;
    self->inherit_props.current_generic_params = NULL;
    self->inherit_props.current_called_generic_params = NULL;
    self->inherit_props.tab_count = 0;
    self->inherit_props.write_semicolon = true;
    self->inherit_props.current_scope = current_scope;

    return self;
}

DESTRUCTOR(CIGeneratorContentSession, CIGeneratorContentSession *self)
{
    FREE(String, self->buffer);
    lily_free(self);
}

void
start_session__CIGeneratorContent(CIGeneratorContent *self,
                                  CIScope *current_scope)
{
    CIGeneratorContentSession *new_session =
      NEW(CIGeneratorContentSession, current_scope);

    if (self->last_session && self->last_session->must_inherit) {
        new_session->inherit_props = self->last_session->inherit_props;
    }

    self->last_session = new_session;

    push__Vec(self->sessions, self->last_session);
}

void
end_session__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    append__String(self->final, self->last_session->buffer);

    FREE(CIGeneratorContentSession, pop__Vec(self->sessions));

    self->last_session = safe_last__Vec(self->sessions);
}

void
end_session_by_cascade__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    CIGeneratorContentSession *next_session =
      self->sessions->len > 1
        ? safe_get__Vec(self->sessions, self->sessions->len - 2)
        : NULL;

    append__String(next_session ? next_session->buffer : self->final,
                   self->last_session->buffer);

    FREE(CIGeneratorContentSession, pop__Vec(self->sessions));

    self->last_session = next_session;
}

void
set_new_scope__CIGeneratorContent(CIGeneratorContent *self,
                                  CIScope **parent_scope,
                                  CIScopeID *new_scope_id,
                                  const CIResultFile *file)
{
    ASSERT(self->last_session);

    *parent_scope = self->last_session->inherit_props.current_scope;

    self->last_session->inherit_props.current_scope =
      get_scope_from_id__CIResultFile(file, new_scope_id);
}

void
restore_scope__CIGeneratorContent(CIGeneratorContent *self, CIScope *scope)
{
    ASSERT(self->last_session);

    self->last_session->inherit_props.current_scope = scope;
}

void
set_must_inherit__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    self->last_session->must_inherit = true;
}

void
unset_must_inherit__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    self->last_session->must_inherit = false;
}

void
set_current_generic_params__CIGeneratorContent(
  CIGeneratorContent *self,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params)
{
    ASSERT(self->last_session);

    self->last_session->inherit_props.current_generic_params = generic_params;
    self->last_session->inherit_props.current_called_generic_params =
      called_generic_params;
}

void
reset_current_generic_params__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    self->last_session->inherit_props.current_generic_params = NULL;
    self->last_session->inherit_props.current_called_generic_params = NULL;
}

void
start_session_with_default_scope__CIGenerator(CIGenerator *self)
{
    start_session__CIGeneratorContent(
      &self->content,
      self->content.last_session
        ? self->content.last_session->inherit_props.current_scope
        : self->file->scope_base);
}

void
end_session__CIGenerator(CIGenerator *self)
{
    end_session__CIGeneratorContent(&self->content);
}

void
end_session_by_cascade__CIGenerator(CIGenerator *self)
{
    end_session_by_cascade__CIGeneratorContent(&self->content);
}

void
end_session_by_decl__CIGenerator(CIGenerator *self, const CIDecl *decl)
{
    if (is_local__CIDecl(decl)) {
        end_session_by_cascade__CIGenerator(self);
    } else {
        end_session__CIGenerator(self);
    }
}

void
set_new_scope__CIGenerator(CIGenerator *self,
                           CIScope **parent_scope,
                           CIScopeID *new_scope_id)
{
    return set_new_scope__CIGeneratorContent(
      &self->content, parent_scope, new_scope_id, self->file);
}

void
restore_scope__CIGenerator(CIGenerator *self, CIScope *scope)
{
    return restore_scope__CIGeneratorContent(&self->content, scope);
}

void
set_must_inherit__CIGenerator(CIGenerator *self)
{
    set_must_inherit__CIGeneratorContent(&self->content);
}

void
unset_must_inherit__CIGenerator(CIGenerator *self)
{
    unset_must_inherit__CIGeneratorContent(&self->content);
}

void
write__CIGeneratorContent(CIGeneratorContent *self, char c)
{
    push__String(self->last_session ? self->last_session->buffer : self->final,
                 c);
}

void
write_str__CIGeneratorContent(CIGeneratorContent *self, char *s)
{
    push_str__String(
      self->last_session ? self->last_session->buffer : self->final, s);
}

void
write_String__CIGeneratorContent(CIGeneratorContent *self, String *s)
{
    push_str__String(
      self->last_session ? self->last_session->buffer : self->final, s->buffer);
    FREE(String, s);
}

void
write_tab__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    write_String__CIGeneratorContent(
      self, repeat__String("\t", self->last_session->inherit_props.tab_count));
}

void
inc_tab_count__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    ++self->last_session->inherit_props.tab_count;
}

void
dec_tab_count__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    --self->last_session->inherit_props.tab_count;
}

void
set_write_semicolon__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    self->last_session->inherit_props.write_semicolon = true;
}

void
unset_write_semicolon__CIGeneratorContent(CIGeneratorContent *self)
{
    ASSERT(self->last_session);

    self->last_session->inherit_props.write_semicolon = false;
}

DESTRUCTOR(CIGeneratorContent, const CIGeneratorContent *self)
{
    ASSERT(self->sessions->len == 0 && !self->last_session);

    FREE(String, self->final);
    FREE(Vec, self->sessions);
}

void
write__CIGenerator(CIGenerator *self, char c)
{
    write__CIGeneratorContent(&self->content, c);
}

void
write_str__CIGenerator(CIGenerator *self, char *s)
{
    write_str__CIGeneratorContent(&self->content, s);
}

void
write_String__CIGenerator(CIGenerator *self, String *s)
{
    write_String__CIGeneratorContent(&self->content, s);
}

void
inc_tab_count__CIGenerator(CIGenerator *self)
{
    inc_tab_count__CIGeneratorContent(&self->content);
}

void
dec_tab_count__CIGenerator(CIGenerator *self)
{
    dec_tab_count__CIGeneratorContent(&self->content);
}

void
write_tab__CIGenerator(CIGenerator *self)
{
    write_tab__CIGeneratorContent(&self->content);
}

void
set_write_semicolon__CIGenerator(CIGenerator *self)
{
    set_write_semicolon__CIGeneratorContent(&self->content);
}

void
unset_write_semicolon__CIGenerator(CIGenerator *self)
{
    unset_write_semicolon__CIGeneratorContent(&self->content);
}

void
set_current_generic_params__CIGenerator(CIGenerator *self,
                                        CIGenericParams *generic_params,
                                        CIGenericParams *called_generic_params)
{
    set_current_generic_params__CIGeneratorContent(
      &self->content, generic_params, called_generic_params);
}

void
reset_current_generic_params__CIGenerator(CIGenerator *self)
{
    reset_current_generic_params__CIGeneratorContent(&self->content);
}

String *
substitute_and_serialize_generic_params__CIGenerator(
  CIGenerator *self,
  CIGenericParams *unresolved_generic_params,
  const String *name)
{
    ASSERT(unresolved_generic_params);
    ASSERT(name);

    if (has_generic__CIGenericParams(unresolved_generic_params)) {
        ASSERT(
          self->content.last_session->inherit_props.current_generic_params);
        ASSERT(self->content.last_session->inherit_props
                 .current_called_generic_params);

        CIGenericParams *resolved_generic_params =
          substitute_generic_params__CIParser(
            unresolved_generic_params,
            self->content.last_session->inherit_props.current_generic_params,
            self->content.last_session->inherit_props
              .current_called_generic_params);

        ASSERT(resolved_generic_params);

        String *serialized_name =
          serialize_name__CIGenericParams(resolved_generic_params, name);

        FREE(CIGenericParams, resolved_generic_params);

        return serialized_name;
    }

    return serialize_name__CIGenericParams(unresolved_generic_params, name);
}

void
generate_data_type_qualifier__CIGenerator(CIGenerator *self,
                                          const int *data_type_qualifier)
{
    write_String__CIGenerator(
      self, to_string__CIDataTypeQualifier(*data_type_qualifier));
}

CIDataType *
substitute_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type)
{
    return substitute_data_type__CIParser(
      data_type,
      self->content.last_session->inherit_props.current_generic_params,
      self->content.last_session->inherit_props.current_called_generic_params);
}

void
search_decl_and_generate_base__CIGenerator(
  CIGenerator *self,
  const String *name,
  CIDecl *(*search)(const CIResultFile *self, const String *name))
{
    if (name) {
        CIDecl *decl = search(self->file, name);

        ASSERT(decl);

        generate_decl__CIGenerator(self, decl);
    }
}

void
search_enum_decl_and_generate__CIGenerator(CIGenerator *self,
                                           const String *name)
{
    search_decl_and_generate_base__CIGenerator(
      self, name, &search_enum__CIResultFile);
}

void
search_struct_decl_and_generate__CIGenerator(CIGenerator *self,
                                             const String *name)
{
    search_decl_and_generate_base__CIGenerator(
      self, name, &search_struct__CIResultFile);
}

void
search_union_decl_and_generate__CIGenerator(CIGenerator *self,
                                            const String *name)
{
    search_decl_and_generate_base__CIGenerator(
      self, name, &search_union__CIResultFile);
}

void
search_typedef_decl_and_generate__CIGenerator(CIGenerator *self,
                                              const String *name)
{
    search_decl_and_generate_base__CIGenerator(
      self, name, &search_typedef__CIResultFile);
}

void
generate_pre_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_FUNCTION:
            break;
        case CI_DATA_TYPE_KIND_ARRAY:
            return generate_pre_data_type__CIGenerator(
              self, data_type->array.data_type);
        default:
            generate_data_type_base__CIGenerator(self, data_type, false);
    }
}

void
generate_post_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return generate_array_data_type__CIGenerator(self, data_type);
        case CI_DATA_TYPE_KIND_FUNCTION:
            generate_data_type_base__CIGenerator(self, data_type, false);

            break;
        default:
            break;
    }
}

void
generate_array_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type)
{
#define GENERATE_DATA_TYPE_QUALIFIER(q)                        \
    if ((q) != CI_DATA_TYPE_QUALIFIER_NONE) {                  \
        write__CIGenerator(self, ' ');                         \
        generate_data_type_qualifier__CIGenerator(self, &(q)); \
    }

    if (data_type->kind != CI_DATA_TYPE_KIND_ARRAY) {
        return;
    }

    if (data_type->array.name) {
        write__CIGenerator(self, ' ');
        write_str__CIGenerator(
          self, GET_PTR_RC(String, data_type->array.name)->buffer);
    }

    write__CIGenerator(self, '[');

    if (data_type->array.is_static) {
        write_str__CIGenerator(self, "static ");
    }

    GENERATE_DATA_TYPE_QUALIFIER(data_type->array.qualifier);

    if (data_type->array.size_expr) {
        generate_function_expr__CIGenerator(self, data_type->array.size_expr);
    }

    write__CIGenerator(self, ']');

    generate_array_data_type__CIGenerator(self, data_type->array.data_type);
}

void
generate_ptr_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type)
{
    if (data_type->kind != CI_DATA_TYPE_KIND_PTR) {
        return;
    }

    generate_ptr_data_type__CIGenerator(self, data_type->ptr.data_type);

    write__CIGenerator(self, '*');

    GENERATE_DATA_TYPE_QUALIFIER(data_type->qualifier);

    if (data_type->ptr.name) {
        write__CIGenerator(self, ' ');
        write_str__CIGenerator(self,
                               GET_PTR_RC(String, data_type->ptr.name)->buffer);
    }
}

void
generate_function_data_type__CIGenerator(CIGenerator *self,
                                         CIDataType *data_type,
                                         CIDataType *function_data_type)
{
    ASSERT(function_data_type->kind == CI_DATA_TYPE_KIND_FUNCTION);

    generate_data_type_base__CIGenerator(
      self, function_data_type->function.return_data_type, false);

    if (function_data_type == data_type) {
        write__CIGenerator(self, ' ');
        write_str__CIGenerator(
          self, GET_PTR_RC(String, function_data_type->function.name)->buffer);
    } else {
        write__CIGenerator(self, '(');
        generate_data_type_base__CIGenerator(self, data_type, false);
        write__CIGenerator(self, ')');
    }

    if (function_data_type->function.params) {
        generate_function_params__CIGenerator(
          self, function_data_type->function.params);
    } else {
        write_str__CIGenerator(self, "()");
    }
}

void
generate_data_type_base__CIGenerator(CIGenerator *self,
                                     CIDataType *data_type,
                                     bool is_on_top)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ANY:
            UNREACHABLE("cannot generate this data type");
        case CI_DATA_TYPE_KIND_ARRAY: {
            CIDataType *wrapped_data_type = NULL;

            {
                CIDataType *current_data_type = data_type->array.data_type;

                while (current_data_type->kind == CI_DATA_TYPE_KIND_ARRAY &&
                       (current_data_type = current_data_type->array.data_type))
                    ;

                wrapped_data_type = current_data_type;
            }

            ASSERT(wrapped_data_type);

            generate_data_type_base__CIGenerator(
              self, wrapped_data_type, false);
            generate_array_data_type__CIGenerator(self, data_type);

            return;
        }
        case CI_DATA_TYPE_KIND_BOOL:
            if (self->file->config->standard < CI_STANDARD_23) {
                write_str__CIGenerator(self, "_Bool");
            } else {
                write_str__CIGenerator(self, "bool");
            }

            break;
        case CI_DATA_TYPE_KIND_BUILTIN: {
            const CIBuiltinType *builtin_type = get_builtin_type__CIBuiltin(
              self->file->entity.result->builtin, data_type->builtin);

            ASSERT(builtin_type);

            write_str__CIGenerator(self, (char *)builtin_type->name->buffer);

            break;
        }
        case CI_DATA_TYPE_KIND_CHAR:
            write_str__CIGenerator(self, "char");

            break;
        case CI_DATA_TYPE_KIND_DOUBLE:
            write_str__CIGenerator(self, "double");

            break;
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
            write_str__CIGenerator(self, "double _Complex");

            break;
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
            write_str__CIGenerator(self, "double _Imaginary");

            break;
        case CI_DATA_TYPE_KIND__DECIMAL32:
            write_str__CIGenerator(self, "_Decimal32");

            break;
        case CI_DATA_TYPE_KIND__DECIMAL64:
            write_str__CIGenerator(self, "_Decimal64");

            break;
        case CI_DATA_TYPE_KIND__DECIMAL128:
            write_str__CIGenerator(self, "_Decimal128");

            break;
        case CI_DATA_TYPE_KIND_ENUM: {
            search_enum_decl_and_generate__CIGenerator(
              self,
              data_type->enum_.name ? GET_PTR_RC(String, data_type->enum_.name)
                                    : NULL);
            generate_enum__CIGenerator(
              self,
              data_type->enum_.name ? GET_PTR_RC(String, data_type->enum_.name)
                                    : NULL,
              data_type->enum_.data_type,
              data_type->enum_.variants);

            break;
        }
        case CI_DATA_TYPE_KIND_FLOAT:
            write_str__CIGenerator(self, "float");

            break;
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
            write_str__CIGenerator(self, "float _Complex");

            break;
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
            write_str__CIGenerator(self, "float _Imaginary");

            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            // NOTE: The function data type is generated by the
            // `generate_function_data_type__CIGenerator` function.

            return;
        case CI_DATA_TYPE_KIND_GENERIC:
            UNREACHABLE("cannot get generic at this point");
        case CI_DATA_TYPE_KIND_INT:
            write_str__CIGenerator(self, "int");

            break;
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
            write_str__CIGenerator(self, "long double");

            break;
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
            write_str__CIGenerator(self, "long double _Complex");

            break;
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            write_str__CIGenerator(self, "long double _Imaginary");

            break;
        case CI_DATA_TYPE_KIND_LONG_INT:
            write_str__CIGenerator(self, "long int");

            break;
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
            write_str__CIGenerator(self, "long long int");

            break;
        case CI_DATA_TYPE_KIND_NULLPTR_T:
            // Clang (18.1.8) does not support the builtin use of `nullptr_t`,
            // it supports it only if defined as `typeof(nullptr)`.
            if (self->file->config->compiler.kind ==
                CI_PROJECT_CONFIG_COMPILER_KIND_CLANG) {
                write_str__CIGenerator(self, "typeof(nullptr)");

                break;
            }

            write_str__CIGenerator(self, "nullptr_t");

            break;
        case CI_DATA_TYPE_KIND_PTR: {
            CIDataType *wrapped_data_type = NULL;

            {
                CIDataType *current_data_type = data_type->ptr.data_type;

                while (current_data_type->kind == CI_DATA_TYPE_KIND_PTR &&
                       (current_data_type = current_data_type->ptr.data_type))
                    ;

                wrapped_data_type = current_data_type;
            }

            ASSERT(wrapped_data_type);

            generate_pre_data_type__CIGenerator(self, wrapped_data_type);

            bool need_to_generate_paren = is_on_top && data_type->ptr.name;

            if (need_to_generate_paren) {
                write__CIGenerator(self, '(');
            }

            generate_ptr_data_type__CIGenerator(self, data_type);

            if (need_to_generate_paren) {
                write__CIGenerator(self, ')');
            }

            generate_post_data_type__CIGenerator(self, wrapped_data_type);

            return;
        }
        case CI_DATA_TYPE_KIND_SHORT_INT:
            write_str__CIGenerator(self, "short int");

            break;
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            write_str__CIGenerator(self, "signed char");

            break;
        case CI_DATA_TYPE_KIND_STRUCT: {
#define GENERATE_STRUCT_OR_UNION_DT(dt_name)                                  \
    if (data_type->dt_name##_.generic_params) {                               \
        /* NOTE: Normally you can't declare a anonymous struct/union with     \
        generic parameters. This possibility is rejected in the               \
        parser. */                                                            \
        ASSERT(data_type->dt_name##_.name);                                   \
                                                                              \
        String *serialized_name =                                             \
          substitute_and_serialize_generic_params__CIGenerator(               \
            self,                                                             \
            data_type->dt_name##_.generic_params,                             \
            GET_PTR_RC(String, data_type->dt_name##_.name));                  \
                                                                              \
        search_##dt_name##_decl_and_generate__CIGenerator(self,               \
                                                          serialized_name);   \
        generate_##dt_name##__CIGenerator(                                    \
          self, serialized_name, data_type->dt_name##_.fields);               \
                                                                              \
        FREE(String, serialized_name);                                        \
    } else {                                                                  \
        String *struct_name =                                                 \
          data_type->dt_name##_.name                                          \
            ? GET_PTR_RC(String, data_type->dt_name##_.name)                  \
            : NULL;                                                           \
                                                                              \
        search_##dt_name##_decl_and_generate__CIGenerator(self, struct_name); \
        generate_##dt_name##__CIGenerator(                                    \
          self, struct_name, data_type->dt_name##_.fields);                   \
    }

            GENERATE_STRUCT_OR_UNION_DT(struct);

            break;
        }
        case CI_DATA_TYPE_KIND_TYPEDEF:
            if (data_type->typedef_.generic_params) {
                String *serialized_name =
                  substitute_and_serialize_generic_params__CIGenerator(
                    self,
                    data_type->typedef_.generic_params,
                    GET_PTR_RC(String, data_type->typedef_.name));

                search_typedef_decl_and_generate__CIGenerator(self,
                                                              serialized_name);
                write_String__CIGenerator(self, serialized_name);
            } else {
                String *typedef_name =
                  GET_PTR_RC(String, data_type->typedef_.name);

                search_typedef_decl_and_generate__CIGenerator(self,
                                                              typedef_name);
                write_str__CIGenerator(self, typedef_name->buffer);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPE_INFO:
            UNREACHABLE("cannot generate this data type");
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
            write_str__CIGenerator(self, "unsigned int");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
            write_str__CIGenerator(self, "unsigned char");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
            write_str__CIGenerator(self, "unsigned long int");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
            write_str__CIGenerator(self, "unsigned long long int");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            write_str__CIGenerator(self, "unsigned short int");

            break;
        case CI_DATA_TYPE_KIND_UNION: {
            GENERATE_STRUCT_OR_UNION_DT(union);

            break;
        }
        case CI_DATA_TYPE_KIND_VOID:
            write_str__CIGenerator(self, "void");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    GENERATE_DATA_TYPE_QUALIFIER(data_type->qualifier);

#undef GENERATE_DATA_TYPE_QUALIFIER
}

void
generate_data_type__CIGenerator(CIGenerator *self, CIDataType *data_type)
{
    CIDataType *subs_data_type = NULL;

    if (self->content.last_session->inherit_props.current_generic_params &&
        self->content.last_session->inherit_props
          .current_called_generic_params) {
        subs_data_type = substitute_data_type__CIGenerator(self, data_type);
    } else {
        subs_data_type = ref__CIDataType(data_type);
    }

    CIDataType *function_data_type = get_function__CIDataType(subs_data_type);

    if (function_data_type) {
        generate_function_data_type__CIGenerator(
          self, subs_data_type, function_data_type);
    } else {
        generate_data_type_base__CIGenerator(self, subs_data_type, true);
    }

    FREE(CIDataType, subs_data_type);
}

void
generate_storage_class__CIGenerator(CIGenerator *self,
                                    const int *storage_class_flag)
{
    write_String__CIGenerator(
      self,
      to_string__CIStorageClass(*storage_class_flag,
                                self->file->config->standard));

    if (*storage_class_flag != CI_STORAGE_CLASS_NONE) {
        write__CIGenerator(self, ' ');
    }
}

void
generate_attribute_standard__CIGenerator(
  CIGenerator *self,
  const CIAttributeStandard *attribute_standard)
{
    write_str__CIGenerator(self, "[[");

    switch (attribute_standard->kind) {
        case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
            write_String__CIGenerator(
              self,
              format__String(
                "deprecated({S})",
                GET_PTR_RC(String, attribute_standard->deprecated)));

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH:
            write_str__CIGenerator(self, "fallthrough");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED:
            write_str__CIGenerator(self, "maybe_unused");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
            write_String__CIGenerator(
              self,
              format__String(
                "nodiscard({S})",
                GET_PTR_RC(String, attribute_standard->nodiscard)));

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_NORETURN:
            write_str__CIGenerator(self, "noreturn");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED:
            write_str__CIGenerator(self, "unsequenced");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE:
            write_str__CIGenerator(self, "reproducible");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    write_str__CIGenerator(self, "]]");
}

void
generate_attribute__CIGenerator(CIGenerator *self, const CIAttribute *attribute)
{
    switch (attribute->kind) {
        case CI_ATTRIBUTE_KIND_CLANG:
            TODO("generate Clang attribute");
        case CI_ATTRIBUTE_KIND_GNU:
            TODO("generate GNU attribute");
        case CI_ATTRIBUTE_KIND_STANDARD:
            generate_attribute_standard__CIGenerator(self,
                                                     &attribute->standard);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_attributes__CIGenerator(CIGenerator *self, const Vec *attributes)
{
    if (attributes) {
        for (Usize i = 0; i < attributes->len; ++i) {
            generate_attribute__CIGenerator(self, get__Vec(attributes, i));
            write_str__CIGenerator(self, " ");
        }
    }
}

void
generate_attributes_by_decl__CIGenerator(CIGenerator *self, const CIDecl *decl)
{
    switch (decl->kind) {
        case CI_DECL_KIND_FUNCTION:
            generate_attributes__CIGenerator(self, decl->function.attributes);

            break;
        default:
            break;
    }
}

Usize
get_decl_id_from_decl__CIGenerator(CIGenerator *self, const CIDecl *decl)
{
    return get_decl_id_from_decl__CIScope(
      self->content.last_session->inherit_props.current_scope,
      self->file,
      decl);
}

void
generate_enum_variant__CIGenerator(CIGenerator *self,
                                   const CIDeclEnumVariant *enum_variant)
{
    write_String__CIGenerator(
      self,
      format__String("{S} = {zi},\n",
                     GET_PTR_RC(String, enum_variant->name),
                     enum_variant->value));
}

void
generate_enum_variants__CIGenerator(CIGenerator *self, const Vec *enum_variants)
{
    inc_tab_count__CIGenerator(self);

    for (Usize i = 0; i < enum_variants->len; ++i) {
        write_tab__CIGenerator(self);
        generate_enum_variant__CIGenerator(self, get__Vec(enum_variants, i));
    }

    dec_tab_count__CIGenerator(self);
}

void
generate_enum__CIGenerator(CIGenerator *self,
                           String *name,
                           CIDataType *data_type,
                           Vec *variants)
{
    write_String__CIGenerator(
      self,
      format__String("enum{s}{s}", name ? " " : "", name ? name->buffer : ""));

    if (data_type) {
        write_str__CIGenerator(self, " : ");
        generate_data_type__CIGenerator(self, data_type);
    }

    if (variants) {
        write_str__CIGenerator(self, " {\n");
        generate_enum_variants__CIGenerator(self, variants);
        write_str__CIGenerator(self, "}");
    }
}

void
generate_enum_prototype__CIGenerator(CIGenerator *self, const CIDeclEnum *enum_)
{
    write_String__CIGenerator(
      self, format__String("enum {S}", GET_PTR_RC(String, enum_->name)));

    if (enum_->data_type) {
        write_str__CIGenerator(self, " : ");
        generate_data_type__CIGenerator(self, enum_->data_type);
    }
}

void
generate_enum_decl__CIGenerator(CIGenerator *self, const CIDeclEnum *enum_)
{
    generate_enum__CIGenerator(self,
                               enum_->name ? GET_PTR_RC(String, enum_->name)
                                           : NULL,
                               enum_->data_type,
                               enum_->variants);
}

void
generate_function_params__CIGenerator(CIGenerator *self,
                                      const CIDeclFunctionParams *params)
{
    write_str__CIGenerator(self, "(");

    if (params) {
        for (Usize i = 0; i < params->content->len; ++i) {
            const CIDeclFunctionParam *param = get__Vec(params->content, i);

            switch (param->kind) {
                case CI_DECL_FUNCTION_PARAM_KIND_NORMAL:
                    ASSERT(param->data_type);

                    generate_data_type__CIGenerator(self, param->data_type);

                    if (param->name &&
                        !has_variable_name__CIDataType(param->data_type)) {
                        write_String__CIGenerator(
                          self,
                          format__String(" {S}",
                                         GET_PTR_RC(String, param->name)));
                    }

                    break;
                case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
                    write_str__CIGenerator(self, "...");

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            if (i + 1 != params->content->len) {
                write_str__CIGenerator(self, ", ");
            }
        }
    }

    write_str__CIGenerator(self, ")");
}

void
generate_function_prototype__CIGenerator(CIGenerator *self,
                                         const CIDeclFunction *function)
{
    generate_data_type__CIGenerator(self, function->return_data_type);
    write_String__CIGenerator(
      self, format__String(" {S}", GET_PTR_RC(String, function->name)));
    generate_function_params__CIGenerator(self, function->params);
}

void
generate_function_binary_expr__CIGenerator(CIGenerator *self,
                                           const CIExprBinary *binary)
{
    generate_function_expr__CIGenerator(self, binary->left);

    char *s_kind;

    switch (binary->kind) {
        case CI_EXPR_BINARY_KIND_ASSIGN:
            s_kind = "=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
            s_kind = "+=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
            s_kind = "-=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
            s_kind = "*=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
            s_kind = "/=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
            s_kind = "%=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            s_kind = "&=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            s_kind = "|=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
            s_kind = "^=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
            s_kind = "<<=";

            break;
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT:
            s_kind = ">>=";

            break;
        case CI_EXPR_BINARY_KIND_ADD:
            s_kind = "+";

            break;
        case CI_EXPR_BINARY_KIND_SUB:
            s_kind = "-";

            break;
        case CI_EXPR_BINARY_KIND_MUL:
            s_kind = "*";

            break;
        case CI_EXPR_BINARY_KIND_DIV:
            s_kind = "/";

            break;
        case CI_EXPR_BINARY_KIND_MOD:
            s_kind = "%";

            break;
        case CI_EXPR_BINARY_KIND_BIT_AND:
            s_kind = "&";

            break;
        case CI_EXPR_BINARY_KIND_BIT_OR:
            s_kind = "|";

            break;
        case CI_EXPR_BINARY_KIND_BIT_XOR:
            s_kind = "^";

            break;
        case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
            s_kind = "<<";

            break;
        case CI_EXPR_BINARY_KIND_BIT_RSHIFT:
            s_kind = ">>";

            break;
        case CI_EXPR_BINARY_KIND_AND:
            s_kind = "&&";

            break;
        case CI_EXPR_BINARY_KIND_OR:
            s_kind = "||";

            break;
        case CI_EXPR_BINARY_KIND_EQ:
            s_kind = "==";

            break;
        case CI_EXPR_BINARY_KIND_NE:
            s_kind = "!=";

            break;
        case CI_EXPR_BINARY_KIND_LESS:
            s_kind = "<";

            break;
        case CI_EXPR_BINARY_KIND_GREATER:
            s_kind = ">";

            break;
        case CI_EXPR_BINARY_KIND_LESS_EQ:
            s_kind = "<=";

            break;
        case CI_EXPR_BINARY_KIND_GREATER_EQ:
            s_kind = ">=";

            break;
        case CI_EXPR_BINARY_KIND_DOT:
            s_kind = ".";

            break;
        case CI_EXPR_BINARY_KIND_ARROW:
            s_kind = "->";

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    switch (binary->kind) {
        // Glue the operator to the expression on the left and right.
        case CI_EXPR_BINARY_KIND_ARROW:
        case CI_EXPR_BINARY_KIND_DOT:
            write_str__CIGenerator(self, s_kind);

            break;
        default:
            write_String__CIGenerator(self, format__String(" {s} ", s_kind));
    }

    generate_function_expr__CIGenerator(self, binary->right);
}

void
generate_function_call_params_expr__CIGenerator(CIGenerator *self, Vec *params)
{
    write_str__CIGenerator(self, "(");

    for (Usize i = 0; i < params->len; ++i) {
        generate_function_expr__CIGenerator(self, get__Vec(params, i));

        if (i + 1 != params->len) {
            write_str__CIGenerator(self, ", ");
        }
    }

    write_str__CIGenerator(self, ")");
}

void
generate_function_call_expr__CIGenerator(
  CIGenerator *self,
  const CIExprFunctionCall *function_call)
{
    if (function_call->generic_params) {
        write_String__CIGenerator(
          self,
          substitute_and_serialize_generic_params__CIGenerator(
            self,
            function_call->generic_params,
            GET_PTR_RC(String, function_call->identifier)));
    } else {
        write_str__CIGenerator(
          self, GET_PTR_RC(String, function_call->identifier)->buffer);
    }

    generate_function_call_params_expr__CIGenerator(self,
                                                    function_call->params);
}

void
generate_function_call_builtin_expr__CIGenerator(
  CIGenerator *self,
  const CIExprFunctionCallBuiltin *function_call_builtin)
{
    const CIBuiltinFunction *builtin_function = get_builtin_function__CIBuiltin(
      self->file->entity.result->builtin, function_call_builtin->id);

    ASSERT(builtin_function);

    write_str__CIGenerator(self, (char *)builtin_function->name->buffer);
    generate_function_call_params_expr__CIGenerator(
      self, function_call_builtin->params);
}

void
generate_function_literal_character_expr__CIGenerator(CIGenerator *self,
                                                      const char c)
{
    switch (c) {
        case '\"':
            write_str__CIGenerator(self, "\\\"");
            break;
        case '\'':
            write_str__CIGenerator(self, "\\'");
            break;
        case '\b':
            write_str__CIGenerator(self, "\\b");
            break;
        case '\f':
            write_str__CIGenerator(self, "\\f");
            break;
        case '\n':
            write_str__CIGenerator(self, "\\n");
            break;
        case '\t':
            write_str__CIGenerator(self, "\\t");
            break;
        case '\r':
            write_str__CIGenerator(self, "\\r");
            break;
        case '\v':
            write_str__CIGenerator(self, "\\v");
            break;
        case '\\':
            write_str__CIGenerator(self, "\\");
            break;
        case '\0':
            write_str__CIGenerator(self, "\\0");
            break;
        default:
            write__CIGenerator(self, c);
    }
}

void
generate_function_literal_string_expr__CIGenerator(CIGenerator *self,
                                                   const String *string)
{
    StringIter iter = NEW(StringIter, (String *)string);
    char current;

    while ((current = next__StringIter(&iter))) {
        generate_function_literal_character_expr__CIGenerator(self, current);
    }
}

void
generate_function_literal_expr__CIGenerator(CIGenerator *self,
                                            const CIExprLiteral *literal)
{
    switch (literal->kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            if (literal->bool_) {
                write_str__CIGenerator(self, "true");
            } else {
                write_str__CIGenerator(self, "false");
            }

            break;
        case CI_EXPR_LITERAL_KIND_CHAR:
            write__CIGenerator(self, '\'');
            generate_function_literal_character_expr__CIGenerator(
              self, literal->char_);
            write__CIGenerator(self, '\'');

            break;
        case CI_EXPR_LITERAL_KIND_FLOAT:
            write_String__CIGenerator(self,
                                      format__String("{f}", literal->float_));

            break;
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            write_String__CIGenerator(
              self, format__String("{zi}", literal->signed_int));

            break;
        case CI_EXPR_LITERAL_KIND_STRING:
            write__CIGenerator(self, '\"');
            generate_function_literal_string_expr__CIGenerator(
              self, GET_PTR_RC(String, literal->string));
            write__CIGenerator(self, '\"');

            break;
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            write_String__CIGenerator(
              self, format__String("{zu}", literal->unsigned_int));

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_initializer_expr__CIGenerator(
  CIGenerator *self,
  const CIExprInitializer *initializer)
{
    write_str__CIGenerator(self, "{");

    for (Usize i = 0; i < initializer->items->len; ++i) {
        CIExprInitializerItem *initializer_item =
          get__Vec(initializer->items, i);

        if (initializer_item->path) {
            for (Usize j = 0; j < initializer_item->path->len; ++j) {
                write_String__CIGenerator(
                  self,
                  format__String(
                    ".{S}",
                    GET_PTR_RC(
                      String,
                      CAST(Rc *, get__Vec(initializer_item->path, j)))));
            }

            write_str__CIGenerator(self, " = ");
        }

        generate_function_expr__CIGenerator(self, initializer_item->value);

        if (i + 1 != initializer->items->len) {
            write_str__CIGenerator(self, ", ");
        }
    }

    write_str__CIGenerator(self, "}");
}

void
generate_function_unary_expr__CIGenerator(CIGenerator *self,
                                          const CIExprUnary *unary)
{
    char *s_kind = NULL;

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
            s_kind = "++";

            break;
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
            s_kind = "--";

            break;
        default:
            break;
    }

    if (s_kind) {
        generate_function_expr__CIGenerator(self, unary->expr);
        write_str__CIGenerator(self, s_kind);

        s_kind = NULL;
    }

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            s_kind = "--";

            break;
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
            s_kind = "++";

            break;
        case CI_EXPR_UNARY_KIND_POSITIVE:
            s_kind = "+";

            break;
        case CI_EXPR_UNARY_KIND_NEGATIVE:
            s_kind = "-";

            break;
        case CI_EXPR_UNARY_KIND_BIT_NOT:
            s_kind = "~";

            break;
        case CI_EXPR_UNARY_KIND_NOT:
            s_kind = "!";

            break;
        case CI_EXPR_UNARY_KIND_DEREFERENCE:
            s_kind = "*";

            break;
        case CI_EXPR_UNARY_KIND_REF:
            s_kind = "&";

            break;
        default:
            break;
    }

    if (s_kind) {
        write_str__CIGenerator(self, s_kind);
        generate_function_expr__CIGenerator(self, unary->expr);
    }
}

void
generate_function_expr__CIGenerator(CIGenerator *self, const CIExpr *expr)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            write_str__CIGenerator(self, "alignof(");
            generate_function_expr__CIGenerator(self, expr->alignof_);
            write_str__CIGenerator(self, ")");

            break;
        case CI_EXPR_KIND_ARRAY_ACCESS:
            generate_function_expr__CIGenerator(self, expr->array_access.array);
            write_str__CIGenerator(self, "[");
            generate_function_expr__CIGenerator(self,
                                                expr->array_access.access);
            write_str__CIGenerator(self, "]");

            break;
        case CI_EXPR_KIND_BINARY:
            generate_function_binary_expr__CIGenerator(self, &expr->binary);

            break;
        case CI_EXPR_KIND_CAST:
            write_str__CIGenerator(self, "(");
            generate_data_type__CIGenerator(self, expr->cast.data_type);
            write_str__CIGenerator(self, ")");
            generate_function_expr__CIGenerator(self, expr->cast.expr);

            break;
        case CI_EXPR_KIND_DATA_TYPE:
            generate_data_type__CIGenerator(self, expr->data_type);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL:
            generate_function_call_expr__CIGenerator(self,
                                                     &expr->function_call);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            generate_function_call_builtin_expr__CIGenerator(
              self, &expr->function_call_builtin);

            break;
        case CI_EXPR_KIND_GROUPING:
            write_str__CIGenerator(self, "(");
            generate_function_expr__CIGenerator(self, expr->grouping);
            write_str__CIGenerator(self, ")");

            break;
        case CI_EXPR_KIND_IDENTIFIER:
            write_str__CIGenerator(
              self, GET_PTR_RC(String, expr->identifier.value)->buffer);

            break;
        case CI_EXPR_KIND_INITIALIZER:
            generate_function_initializer_expr__CIGenerator(self,
                                                            &expr->initializer);

            break;
        case CI_EXPR_KIND_LITERAL:
            generate_function_literal_expr__CIGenerator(self, &expr->literal);

            break;
        case CI_EXPR_KIND_NULLPTR:
            write_str__CIGenerator(self, "nullptr");

            break;
        case CI_EXPR_KIND_SIZEOF:
            write_str__CIGenerator(self, "sizeof(");
            generate_function_expr__CIGenerator(self, expr->sizeof_);
            write_str__CIGenerator(self, ")");

            break;
        case CI_EXPR_KIND_TERNARY:
            generate_function_expr__CIGenerator(self, expr->ternary.cond);
            write_str__CIGenerator(self, " ? ");
            generate_function_expr__CIGenerator(self, expr->ternary.if_);
            write_str__CIGenerator(self, " : ");
            generate_function_expr__CIGenerator(self, expr->ternary.else_);

            break;
        case CI_EXPR_KIND_UNARY:
            generate_function_unary_expr__CIGenerator(self, &expr->unary);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_block_stmt__CIGenerator(CIGenerator *self,
                                          const CIStmtBlock *block)
{
    generate_function_body__CIGenerator(self, block->body);
}

void
generate_case_stmt__CIGenerator(CIGenerator *self,
                                const CIStmtSwitchCase *case_)
{
    write_str__CIGenerator(self, "case ");
    generate_function_expr__CIGenerator(self, case_->value);
    write_str__CIGenerator(self, ":");
}

void
generate_function_do_while_stmt__CIGenerator(CIGenerator *self,
                                             const CIStmtDoWhile *do_while)
{
    write_str__CIGenerator(self, "do ");
    generate_function_body__CIGenerator(self, do_while->body);
    write_str__CIGenerator(self, "while (");
    generate_function_expr__CIGenerator(self, do_while->cond);
    write_str__CIGenerator(self, ");\n");
}

void
generate_function_for_stmt__CIGenerator(CIGenerator *self,
                                        const CIStmtFor *for_)
{
    write_str__CIGenerator(self, "for (");

    if (for_->init_clauses) {
        unset_write_semicolon__CIGenerator(self);

        for (Usize i = 0; i < for_->init_clauses->len; ++i) {
            generate_function_body_item__CIGenerator(
              self, get__Vec(for_->init_clauses, i));

            if (i + 1 != for_->init_clauses->len) {
                write__CIGenerator(self, ',');
            }
        }

        set_write_semicolon__CIGenerator(self);
    }

    write__CIGenerator(self, ';');

    if (for_->expr1) {
        generate_function_expr__CIGenerator(self, for_->expr1);
    }

    write__CIGenerator(self, ';');

    if (for_->exprs2) {
        for (Usize i = 0; i < for_->exprs2->len; ++i) {
            generate_function_expr__CIGenerator(self,
                                                get__Vec(for_->exprs2, i));

            if (i + 1 != for_->exprs2->len) {
                write_str__CIGenerator(self, ", ");
            }
        }
    }

    write_str__CIGenerator(self, ") ");
    generate_function_body__CIGenerator(self, for_->body);
}

void
generate_function_if_stmt_branch__CIGenerator(CIGenerator *self,
                                              const CIStmtIfBranch *if_branch)
{
    write_str__CIGenerator(self, " (");
    generate_function_expr__CIGenerator(self, if_branch->cond);
    write_str__CIGenerator(self, ") ");
    generate_function_body__CIGenerator(self, if_branch->body);
}

void
generate_function_if_stmt__CIGenerator(CIGenerator *self, const CIStmtIf *if_)
{
    write_str__CIGenerator(self, "if");
    generate_function_if_stmt_branch__CIGenerator(self, if_->if_);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            write_tab__CIGenerator(self);
            write_str__CIGenerator(self, "else if");
            generate_function_if_stmt_branch__CIGenerator(
              self, get__Vec(if_->else_ifs, i));
        }
    }

    if (if_->else_) {
        write_tab__CIGenerator(self);
        write_str__CIGenerator(self, "else ");
        generate_function_body__CIGenerator(self, if_->else_);
    }
}

void
generate_function_switch_stmt__CIGenerator(CIGenerator *self,
                                           const CIStmtSwitch *switch_)
{
    write_str__CIGenerator(self, "switch (");
    generate_function_expr__CIGenerator(self, switch_->expr);
    write_str__CIGenerator(self, ") ");
    generate_function_body__CIGenerator(self, switch_->body);
}

void
generate_function_while_stmt__CIGenerator(CIGenerator *self,
                                          const CIStmtWhile *while_)
{
    write_str__CIGenerator(self, "while (");
    generate_function_expr__CIGenerator(self, while_->cond);
    write_str__CIGenerator(self, ") ");
    generate_function_body__CIGenerator(self, while_->body);
}

void
generate_function_stmt__CIGenerator(CIGenerator *self, const CIStmt *stmt)
{
    switch (stmt->kind) {
        case CI_STMT_KIND_BLOCK:
            generate_function_block_stmt__CIGenerator(self, &stmt->block);

            break;
        case CI_STMT_KIND_BREAK:
            write_str__CIGenerator(self, "break;");

            break;
        case CI_STMT_KIND_CASE:
            generate_case_stmt__CIGenerator(self, &stmt->case_);

            break;
        case CI_STMT_KIND_CONTINUE:
            write_str__CIGenerator(self, "continue;");

            break;
        case CI_STMT_KIND_DEFAULT:
            write_str__CIGenerator(self, "default:");

            break;
        case CI_STMT_KIND_DO_WHILE:
            generate_function_do_while_stmt__CIGenerator(self, &stmt->do_while);

            break;
        case CI_STMT_KIND_FOR:
            generate_function_for_stmt__CIGenerator(self, &stmt->for_);

            break;
        case CI_STMT_KIND_GOTO:
            write_String__CIGenerator(
              self,
              format__String("goto {S};", GET_PTR_RC(String, stmt->goto_)));

            break;
        case CI_STMT_KIND_IF:
            generate_function_if_stmt__CIGenerator(self, &stmt->if_);

            break;
        case CI_STMT_KIND_RETURN:
            write_str__CIGenerator(self, "return ");

            if (stmt->return_) {
                generate_function_expr__CIGenerator(self, stmt->return_);
            }

            write_str__CIGenerator(self, ";");

            break;
        case CI_STMT_KIND_SWITCH:
            generate_function_switch_stmt__CIGenerator(self, &stmt->switch_);

            break;
        case CI_STMT_KIND_WHILE:
            generate_function_while_stmt__CIGenerator(self, &stmt->while_);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_body_item__CIGenerator(CIGenerator *self,
                                         const CIDeclFunctionItem *item)
{
    switch (item->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            set_must_inherit__CIGenerator(self);
            generate_decl__CIGenerator(self, item->decl);
            unset_must_inherit__CIGenerator(self);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            generate_function_expr__CIGenerator(self, item->expr);

            if (self->content.last_session->inherit_props.write_semicolon) {
                write_str__CIGenerator(self, ";\n");
            }

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            generate_function_stmt__CIGenerator(self, &item->stmt);
            write_str__CIGenerator(self, "\n");

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_body__CIGenerator(CIGenerator *self,
                                    const CIDeclFunctionBody *body)
{
    CIScope *parent_scope = NULL;

    write_str__CIGenerator(self, "{\n");
    inc_tab_count__CIGenerator(self);
    set_new_scope__CIGenerator(self, &parent_scope, body->scope_id);

    ASSERT(parent_scope);

    for (Usize i = 0; i < body->content->len; ++i) {
        write_tab__CIGenerator(self);
        generate_function_body_item__CIGenerator(self,
                                                 get__Vec(body->content, i));
    }

    restore_scope__CIGenerator(self, parent_scope);
    dec_tab_count__CIGenerator(self);
    write_tab__CIGenerator(self);
    write_str__CIGenerator(self, "}\n");
}

void
generate_function_decl__CIGenerator(CIGenerator *self,
                                    const CIDeclFunction *function)
{
    generate_function_prototype__CIGenerator(self, function);
    write_str__CIGenerator(self, " ");
    generate_function_body__CIGenerator(self, function->body);
}

void
generate_function_gen_prototype__CIGenerator(
  CIGenerator *self,
  const CIDeclFunctionGen *function_gen)
{
    set_current_generic_params__CIGenerator(
      self,
      function_gen->function->generic_params,
      function_gen->called_generic_params);
    generate_data_type__CIGenerator(self,
                                    function_gen->function->return_data_type);
    write_String__CIGenerator(
      self, format__String(" {S}", GET_PTR_RC(String, function_gen->name)));
    generate_function_params__CIGenerator(self, function_gen->function->params);
    reset_current_generic_params__CIGenerator(self);
}

void
generate_function_gen_decl__CIGenerator(CIGenerator *self,
                                        const CIDeclFunctionGen *function_gen)
{
    generate_function_gen_prototype__CIGenerator(self, function_gen);
    set_current_generic_params__CIGenerator(
      self,
      function_gen->function->generic_params,
      function_gen->called_generic_params);
    write_str__CIGenerator(self, " ");
    generate_function_body__CIGenerator(self, function_gen->function->body);
    reset_current_generic_params__CIGenerator(self);
}

void
generate_label_decl__CIGenerator(CIGenerator *self, const CIDeclLabel *label)
{
    write_String__CIGenerator(
      self, format__String("{S}:\n", GET_PTR_RC(String, label->name)));
}

void
generate_struct_prototype__CIGenerator(CIGenerator *self,
                                       const CIDeclStruct *struct_)
{
    write_String__CIGenerator(
      self, format__String("struct {S}", GET_PTR_RC(String, struct_->name)));
}

void
generate_struct_field__CIGenerator(CIGenerator *self,
                                   const CIDeclStructField *field)
{
    CIDataType *field_dt = build_data_type__CIDeclStructField(field);

    generate_data_type__CIGenerator(self, field_dt);

    if (field->name && !has_variable_name__CIDataType(field_dt)) {
        write_String__CIGenerator(
          self, format__String(" {S}", GET_PTR_RC(String, field->name)));
    }

    Usize field_bit = get_bit__CIDeclStructField(field);

    if (field_bit != 0) {
        write_String__CIGenerator(self, format__String(" : {zu}", field_bit));
    }

    write_str__CIGenerator(self, ";\n");

    FREE(CIDataType, field_dt);
}

void
generate_struct_fields__CIGenerator(CIGenerator *self,
                                    const CIDeclStructFields *fields)
{
    inc_tab_count__CIGenerator(self);

    CIDeclStructField *current_field = fields->first;

    while (current_field) {
        write_tab__CIGenerator(self);
        generate_struct_field__CIGenerator(self, current_field);

        current_field =
          get_next_field_with_no_parent__CIDeclStructField(current_field);
    }

    dec_tab_count__CIGenerator(self);
}

void
generate_struct__CIGenerator(CIGenerator *self,
                             String *name,
                             CIDeclStructFields *fields)
{
    write_String__CIGenerator(self,
                              format__String("struct{s}{s}",
                                             name ? " " : "",
                                             name ? name->buffer : ""));

    if (fields) {
        write_str__CIGenerator(self, " {\n");
        generate_struct_fields__CIGenerator(self, fields);
        write_str__CIGenerator(self, "}");
    }
}

void
generate_struct_decl__CIGenerator(CIGenerator *self,
                                  const CIDeclStruct *struct_)
{
    generate_struct__CIGenerator(
      self,
      struct_->name ? GET_PTR_RC(String, struct_->name) : NULL,
      struct_->fields);
}

void
generate_struct_gen_prototype__CIGenerator(CIGenerator *self,
                                           const CIDeclStructGen *struct_gen)
{
    write_String__CIGenerator(
      self, format__String("struct {S}", GET_PTR_RC(String, struct_gen->name)));
}

void
generate_struct_gen_decl__CIGenerator(CIGenerator *self,
                                      const CIDeclStructGen *struct_gen)
{
    set_current_generic_params__CIGenerator(self,
                                            struct_gen->struct_->generic_params,
                                            struct_gen->called_generic_params);
    generate_struct__CIGenerator(
      self, GET_PTR_RC(String, struct_gen->name), struct_gen->fields);
    reset_current_generic_params__CIGenerator(self);
}

void
generate_typedef_decl__CIGenerator(CIGenerator *self,
                                   const CIDeclTypedef *typedef_)
{
    write_str__CIGenerator(self, "typedef ");
    generate_data_type__CIGenerator(self, typedef_->data_type);

    if (!has_variable_name__CIDataType(typedef_->data_type)) {
        write_String__CIGenerator(
          self, format__String(" {S}", GET_PTR_RC(String, typedef_->name)));
    }
}

void
generate_typedef_gen_decl__CIGenerator(CIGenerator *self,
                                       const CIDeclTypedefGen *typedef_gen)
{
    set_current_generic_params__CIGenerator(
      self,
      typedef_gen->typedef_->generic_params,
      typedef_gen->called_generic_params);
    write_str__CIGenerator(self, "typedef ");
    generate_data_type__CIGenerator(self, typedef_gen->typedef_->data_type);

    if (!has_variable_name__CIDataType(typedef_gen->data_type)) {
        write_String__CIGenerator(
          self, format__String(" {S}", GET_PTR_RC(String, typedef_gen->name)));
    }

    reset_current_generic_params__CIGenerator(self);
}

void
generate_union_prototype__CIGenerator(CIGenerator *self,
                                      const CIDeclUnion *union_)
{
    write_String__CIGenerator(
      self, format__String("union {S}", GET_PTR_RC(String, union_->name)));
}

void
generate_union__CIGenerator(CIGenerator *self,
                            String *name,
                            CIDeclStructFields *fields)
{
    write_String__CIGenerator(
      self,
      format__String("union{s}{s}", name ? " " : "", name ? name->buffer : ""));

    if (fields) {
        write_str__CIGenerator(self, " {\n");
        generate_struct_fields__CIGenerator(self, fields);
        write_str__CIGenerator(self, "}");
    }
}

void
generate_union_decl__CIGenerator(CIGenerator *self, const CIDeclUnion *union_)
{
    generate_union__CIGenerator(self,
                                union_->name ? GET_PTR_RC(String, union_->name)
                                             : NULL,
                                union_->fields);
}

void
generate_union_gen_prototype__CIGenerator(CIGenerator *self,
                                          const CIDeclUnionGen *union_gen)
{
    write_String__CIGenerator(
      self, format__String("union {S}", GET_PTR_RC(String, union_gen->name)));
}

void
generate_union_gen_decl__CIGenerator(CIGenerator *self,
                                     const CIDeclUnionGen *union_gen)
{
    set_current_generic_params__CIGenerator(self,
                                            union_gen->union_->generic_params,
                                            union_gen->called_generic_params);
    generate_union__CIGenerator(
      self, GET_PTR_RC(String, union_gen->name), union_gen->fields);
    reset_current_generic_params__CIGenerator(self);
}

void
generate_variable_decl__CIGenerator(CIGenerator *self,
                                    const CIDeclVariable *variable)
{
    generate_data_type__CIGenerator(self, variable->data_type);

    if (!has_variable_name__CIDataType(variable->data_type)) {
        write_String__CIGenerator(
          self, format__String(" {S}", GET_PTR_RC(String, variable->name)));
    }

    if (variable->expr) {
        write_str__CIGenerator(self, " = ");
        generate_function_expr__CIGenerator(self, variable->expr);
    }

    if (self->content.last_session->inherit_props.write_semicolon) {
        write_str__CIGenerator(self, ";\n");
    }
}

void
generate_decl__CIGenerator(CIGenerator *self, const CIDecl *decl)
{
    if (!has_generic__CIDecl(decl)) {
        start_session_with_default_scope__CIGenerator(self);

        Usize decl_id = get_decl_id_from_decl__CIGenerator(self, decl);

        if (has__VecBit(self->generated_decls, decl_id)) {
            goto end_session;
        }

        add__VecBit(self->generated_decls, decl_id);

        if (is_prototype__CIDecl((CIDecl *)decl)) {
            goto end_session;
        }

        switch (decl->kind) {
            case CI_DECL_KIND_ENUM:
                generate_enum_decl__CIGenerator(self, &decl->enum_);

                break;
            case CI_DECL_KIND_ENUM_VARIANT:
                // NOTE: We don't want to generate enum variant here.
                goto end_session;
            case CI_DECL_KIND_FUNCTION:
                generate_function_decl__CIGenerator(self, &decl->function);

                goto end_session;
            case CI_DECL_KIND_FUNCTION_GEN:
                generate_function_gen_decl__CIGenerator(self,
                                                        &decl->function_gen);

                goto end_session;
            case CI_DECL_KIND_LABEL:
                generate_label_decl__CIGenerator(self, &decl->label);

                goto end_session;
            case CI_DECL_KIND_STRUCT:
                generate_struct_decl__CIGenerator(self, &decl->struct_);

                break;
            case CI_DECL_KIND_STRUCT_GEN:
                generate_struct_gen_decl__CIGenerator(self, &decl->struct_gen);

                break;
            case CI_DECL_KIND_TYPEDEF:
                generate_typedef_decl__CIGenerator(self, &decl->typedef_);

                break;
            case CI_DECL_KIND_TYPEDEF_GEN:
                generate_typedef_gen_decl__CIGenerator(self,
                                                       &decl->typedef_gen);

                break;
            case CI_DECL_KIND_UNION:
                generate_union_decl__CIGenerator(self, &decl->union_);

                break;
            case CI_DECL_KIND_UNION_GEN:
                generate_union_gen_decl__CIGenerator(self, &decl->union_gen);

                break;
            case CI_DECL_KIND_VARIABLE:
                generate_storage_class__CIGenerator(self,
                                                    &decl->storage_class_flag);
                generate_variable_decl__CIGenerator(self, &decl->variable);

                goto end_session;
            default:
                UNREACHABLE("unknown variant");
        }

        write_str__CIGenerator(self, ";\n");

    end_session:
        end_session_by_decl__CIGenerator(self, decl);
    }
}

void
generate_decl_prototype__CIGenerator(CIGenerator *self, const CIDecl *decl)
{
    if (!has_generic__CIDecl(decl) && can_have_prototype__CIDecl(decl)) {
        start_session_with_default_scope__CIGenerator(self);
        generate_attributes_by_decl__CIGenerator(self, decl);
        generate_storage_class__CIGenerator(self, &decl->storage_class_flag);

        switch (decl->kind) {
            case CI_DECL_KIND_ENUM:
                generate_enum_prototype__CIGenerator(self, &decl->enum_);

                break;
            case CI_DECL_KIND_ENUM_VARIANT:
            case CI_DECL_KIND_LABEL:
            case CI_DECL_KIND_VARIABLE:
                UNREACHABLE("those declarations can't have prototype");
            case CI_DECL_KIND_FUNCTION:
                generate_function_prototype__CIGenerator(self, &decl->function);

                break;
            case CI_DECL_KIND_FUNCTION_GEN:
                generate_function_gen_prototype__CIGenerator(
                  self, &decl->function_gen);

                break;
            case CI_DECL_KIND_STRUCT:
                generate_struct_prototype__CIGenerator(self, &decl->struct_);

                break;
            case CI_DECL_KIND_STRUCT_GEN:
                generate_struct_gen_prototype__CIGenerator(self,
                                                           &decl->struct_gen);

                break;
            case CI_DECL_KIND_TYPEDEF:
            case CI_DECL_KIND_TYPEDEF_GEN:
                // NOTE: We don't want to generate typedef here.
                goto end_session;
            case CI_DECL_KIND_UNION:
                generate_union_prototype__CIGenerator(self, &decl->union_);

                break;
            case CI_DECL_KIND_UNION_GEN:
                generate_union_gen_prototype__CIGenerator(self,
                                                          &decl->union_gen);

                break;
            default:
                UNREACHABLE("unknown variant");
        }

        write_str__CIGenerator(self, ";\n");

    end_session:
        end_session_by_decl__CIGenerator(self, decl);
    }
}

void
generate_global_decls__CIGenerator(CIGenerator *self)
{
    CIDecl *decl = NULL;
    VecIter iter_decls = NEW(VecIter, self->file->entity.decls);

    while ((decl = next__VecIter(&iter_decls))) {
        switch (decl->kind) {
            case CI_DECL_KIND_VARIABLE:
                // The expression `!decl->variable.is_local` is used to prevent
                // local variables from being generated in the global scope.
                if (!decl->variable.is_local) {
                    generate_decl__CIGenerator(self, decl);
                }

                break;
            case CI_DECL_KIND_LABEL:
                break;
            default:
                generate_decl__CIGenerator(self, decl);
        }
    }
}

void
generate_global_decls_prototype__CIGenerator(CIGenerator *self)
{
    CIDecl *decl = NULL;
    VecIter iter_decls = NEW(VecIter, self->file->entity.decls);

    while ((decl = next__VecIter(&iter_decls))) {
        generate_decl_prototype__CIGenerator(self, decl);
    }
}

void
run_file__CIGenerator(CIGenerator *self)
{
    String *dir_result =
      get_dir_result__CIResultFile(self->file, CI_DIR_RESULT_PURPOSE_C_GEN);
    String *path_result =
      format__String("{S}/{S}", dir_result, self->file->entity.filename_result);

    generate_global_decls_prototype__CIGenerator(self);
    generate_global_decls__CIGenerator(self);

    create_recursive_dir__Dir(dir_result->buffer,
                              DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    write_file__File(path_result->buffer,
                     self->content.final->buffer,
                     self->content.final->len);

    FREE(String, dir_result);
    FREE(String, path_result);
}

void
handler__CIGenerator([[maybe_unused]] void *entity,
                     const CIResultFile *file,
                     [[maybe_unused]] void *other_args)
{
    CIGenerator generator = NEW(CIGenerator, file);

    run_file__CIGenerator(&generator);

    FREE(CIGenerator, &generator);
}

void
run__CIGenerator(const CIResult *result)
{
    pass_through_result__CIResult(result, &handler__CIGenerator, NULL);
}

DESTRUCTOR(CIGenerator, const CIGenerator *self)
{
    FREE(CIGeneratorContent, &self->content);
    FREE(VecBit, self->generated_decls);
}
