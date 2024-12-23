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
#include <base/atof.h>
#include <base/atoi.h>
#include <base/format.h>

#include <core/cc/ci/diagnostic/error.h>
#include <core/cc/ci/include.h>
#include <core/cc/ci/infer.h>
#include <core/cc/ci/parser.h>
#include <core/cc/ci/resolver/expr.h>
#include <core/cc/ci/result.h>
#include <core/shared/diagnostic.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum CIDataTypeCombination
{
    CI_DATA_TYPE_COMBINATION_NONE = 0,
    CI_DATA_TYPE_COMBINATION_CHAR = 1 << 0,
    CI_DATA_TYPE_COMBINATION_DOUBLE = 1 << 1,
    CI_DATA_TYPE_COMBINATION_FLOAT = 1 << 2,
    CI_DATA_TYPE_COMBINATION_INT = 1 << 3,
    CI_DATA_TYPE_COMBINATION_LONG = 1 << 4,
    CI_DATA_TYPE_COMBINATION_LONG_LONG = 1 << 5,
    CI_DATA_TYPE_COMBINATION_SHORT = 1 << 6,
    CI_DATA_TYPE_COMBINATION_SIGNED = 1 << 7,
    CI_DATA_TYPE_COMBINATION_UNSIGNED = 1 << 8,
    CI_DATA_TYPE_COMBINATION__COMPLEX = 1 << 9,
    CI_DATA_TYPE_COMBINATION__IMAGINARY = 1 << 10,
};

/// @return Rc<String*>* (&)
static Rc *
generate_name_error__CIParser();

static void
set_current_token__CIParser(CIParser *self, CIToken *next_token);

/// @brief Check that the token is available in accordance with the standard.
/// @note This function can modify the token type.
static void
check_standard__CIParser(CIParser *self, CIToken *token);

/// @brief Advance to one token on the current iterator.
static void
next_token__CIParser(CIParser *self);

static inline void
set_current_scope__CIParser(CIParser *self);

/// @brief Peek token at position + n.
static CIToken *
peek_token__CIParser(CIParser *self, Usize n);

/// @brief Check if the current token has the same kind than passed in
/// parameter.
static bool
expect__CIParser(CIParser *self, enum CITokenKind kind, bool emit_error);

/// @brief Check if the current token has the same type as the current element
/// in the type list.
static bool
expect_many__CIParser(CIParser *self, Usize n, ...);

/// @brief Check if the current token has the same kind than an element in the
/// list of kind (only one element needs to match for this function not to emit
/// an error).
static void
expect_with_list__CIParser(CIParser *self, Usize n, ...);

/// @brief Advance to n token(s).
static void
jump__CIParser(CIParser *self, Usize n);

/// @brief Check if the passed token is a data type.
static bool
token_is_data_type__CIParser(CIParser *self, const CIToken *token);

/// @brief Check if the current token can be a data type.
static inline bool
is_data_type__CIParser(CIParser *self);

/// @brief Check if the passed token is a storage class.
static bool
token_is_storage_class__CIParser(CIParser *self, const CIToken *token);

/// @brief Check if the current token can be a storage class.
static inline bool
is_storage_class__CIParser(CIParser *self);

/// @brief Check if the passed token is a data type qualifier.
static bool
token_is_data_type_qualifier__CIParser(CIParser *self, const CIToken *token);

/// @brief Check if the current token can be a data type qualifier.
static inline bool
is_data_type_qualifier__CIParser(CIParser *self);

/// @brief Parse data type context(s), such as: !heap, !non_null, !stack or
/// !trace.
static int
parse_data_type_contexts__CIParser(CIParser *self);

/// @brief Parse post data type, like pointer, ...
/// @param CIDataType*?
static CIDataType *
parse_post_data_type__CIParser(CIParser *self, CIDataType *data_type);

static inline const CIDecl *
add_decl_to_scope__CIParser(const CIParser *self,
                            CIDecl **decl_ref,
                            const CIScope *scope,
                            bool must_free,
                            bool in_function_body);

/// @brief Parse variable name (name + data type).
/// @param name_ref Rc<String*>* (&)* (&)
///
/// @example field, variable, parameter
///
/// variable:
///
/// int a[2];
/// //   ^^^
///
/// int *(*fn)(int, int);
/// //   ^^^^^^^^^^^^^^^
///
/// field:
///
/// struct {
/// 	int a[2];
/// 	//   ^^^
/// 	int *(*fn)(int, int);
/// 	//   ^^^^^^^^^^^^^^^
/// }
static void
parse_variable_name_and_data_type__CIParser(CIParser *self,
                                            CIDataType **data_type_ref,
                                            Rc **name_ref,
                                            bool name_is_required);

static void
parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
  CIParser *self,
  int *data_type_qualifier_flag,
  int *storage_class_flag);

/// @brief Check if the data type context is compatible with other data type
/// context contains in `context_flag`.
static void
valid_data_type_context__CIParser(CIParser *self, int context_flag);

static void
set_and_check_data_type_context__CIParser(CIParser *self,
                                          CIDataType *data_type,
                                          int context_flag);

/// @brief Add flag to multi part keyword.
static void
add_flag__CIDataTypeCombination(CIParser *self,
                                Uint32 *flags,
                                enum CIDataTypeCombination flag);

/// @brief Convert data type combination to token kind.
static enum CIDataTypeCombination
convert_token_kind_to_data_type_combination__CIDataTypeCombination(
  enum CITokenKind kind);

/// @brief Check if the token kind is multi part keyword.
bool
is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
  enum CITokenKind kind);

/// @return enum CIDataTypeKind | int (-1)
static int
convert_data_type_combination_to_data_type__CIParser(CIParser *self,
                                                     int combination);

/// @example signed int, long long int, ...
/// @return enum CIDataTypeKind | int (-1)
static int
parse_data_type_combination__CIParser(CIParser *self);

static CIDataType *
parse_pre_data_type__CIParser(CIParser *self);

/// @brief Parse enum variants.
/// @return Vec<CIDeclEnumVariant*>*
static Vec *
parse_enum_variants__CIParser(CIParser *self);

/// @brief Parse enum declaration.
/// @param name Rc<String*>* (&)
static CIDecl *
parse_enum__CIParser(CIParser *self, int storage_class_flag, Rc *name);

/// @brief Parse function params.
/// @param parent_function_scope CIScope*? (&)
/// @return Vec<CIDeclFunctionParam*>*?
static Vec *
parse_function_params__CIParser(CIParser *self, CIScope *parent_function_scope);

/// @brief Parse generic params.
/// @return Vec<CIDataType*>*?
/// @example
///
/// struct Vec.[@T] {
/// 	@T *buffer;
/// 	Usize len;
/// 	Usize capacity;
/// };
///
/// @T get.[@T](@T x) {
/// 	return x;
/// }
static CIGenericParams *
parse_generic_params__CIParser(CIParser *self);

/// @brief Parser function call.
/// @param identifier Rc<String*>* (&)
static CIExpr *
parse_function_call__CIParser(CIParser *self,
                              Rc *identifier,
                              CIGenericParams *generic_params);

/// @brief Parse literal expression.
/// @brief @return CIExpr*?
static CIExpr *
parse_literal_expr__CIParser(CIParser *self);

/// @brief Parse primary expression.
/// @return CIExpr*?
static CIExpr *
parse_primary_expr__CIParser(CIParser *self);

static CIExpr *
parse_struct_call__CIParser(CIParser *self);

static CIExpr *
parse_array__CIParser(CIParser *self);

static CIExpr *
parse_binary_expr__CIParser(CIParser *self, CIExpr *expr);

/// @example x++, x[0], ...
static CIExpr *
parse_post_expr__CIParser(CIParser *self, CIExpr *expr);

/// @param called_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static CIDecl *
search_decl__CIParser(const CIParser *self,
                      const String *name,
                      CIGenericParams *called_generic_params,
                      CIDecl *(*search_decl)(const CIResultFile *self,
                                             const String *name));

/// @param called_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static inline CIDecl *
search_function__CIParser(const CIParser *self,
                          const String *name,
                          CIGenericParams *called_generic_params);

static CIDeclFunctionItem *
parse_case__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self, bool in_switch);

static CIDeclFunctionItem *
parse_for_stmt__CIParser(CIParser *self, bool in_switch);

static CIStmtIfBranch *
parse_if_branch__CIParser(CIParser *self, bool in_loop, bool in_switch);

static bool
has_else_if__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_if_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch);

static CIDeclFunctionItem *
parse_while_stmt__CIParser(CIParser *self, bool in_switch);

static CIDeclFunctionItem *
parse_switch_stmt__CIParser(CIParser *self, bool in_loop);

static CIDeclFunctionItem *
parse_block_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch);

/// @brief Parse statement.
/// @return CIDeclFunctionItem*?
static CIDeclFunctionItem *
parse_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch);

/// @brief Parse function body (base).
/// @param parent_function_scope CIScope* (&)*? (&)
/// @return CIDeclFunctionBody*
static CIDeclFunctionBody *
parse_function_body_base__CIParser(CIParser *self,
                                   bool in_loop,
                                   bool in_switch,
                                   CIScope **parent_function_scope);

/// @brief Parse function body.
/// @param parent_function_scope CIScope* (&)*? (&)
/// @return CIDeclFunctionBody*
static inline CIDeclFunctionBody *
parse_function_body__CIParser(CIParser *self,
                              bool in_loop,
                              bool in_switch,
                              CIScope **parent_function_scope);

/// @brief Parse function block
/// @return CIDeclFunctionBody*
static inline CIDeclFunctionBody *
parse_function_block__CIParser(CIParser *self, bool in_loop, bool in_switch);

static CIDeclFunctionItem *
parse_function_body_item__CIParser(CIParser *self,
                                   bool in_loop,
                                   bool in_switch);

/// @brief Parse function declaration. Note that if the
/// "storage_class_flag" variable contains the storage class "typedef", this
/// function will parse and return a typedef declaration, but if it doesn't, it
/// will parse and return a function declaration.
/// @param name Rc<String*>* (&)
/// @param typedef_generic_params CIGenericParams*?
static CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         Rc *name,
                         CIGenericParams *generic_params,
                         Vec *attributes);

/// @brief Parse fields.
static Vec *
parse_fields__CIParser(CIParser *self);

/// @brief Parse struct or union fields.
/// @return Vec<CIDeclStructField*>*?
static Vec *
parse_struct_or_union_fields__CIParser(CIParser *self);

/// @brief Parse struct declaration.
/// @param name Rc<String*>* (&)
static CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       Rc *name,
                       CIGenericParams *generic_params);

/// @brief Parse typedef declaration.
/// @param aliased_decl CIDecl* - This can be a variable declaration or a
/// function.
/// @param generic_params CIGenericParams* (&)
/// @note Free aliased_decl.
static CIDecl *
parse_typedef__CIParser(CIParser *self,
                        CIDecl *aliased_decl,
                        CIGenericParams *generic_params);

/// @brief Parse union declaration.
/// @param name Rc<String*>* (&)
static CIDecl *
parse_union__CIParser(CIParser *self,
                      int storage_class_flag,
                      Rc *name,
                      CIGenericParams *generic_params);

static inline bool
is_only_initialization_compatible_data_type__CIParser(CIParser *self,
                                                      CIDataType *data_type);

static inline bool
is_initialization_expr__CIParser(CIParser *self, CIExpr *expr);

static void
check_for_initialization_expr__CIParser(CIParser *self,
                                        CIDataType *data_type,
                                        CIExpr *expr);

/// @brief Parse variable declaration.
/// @param name Rc<String*>*? (&)
static CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         Rc *name,
                         bool no_expr,
                         bool is_local);

/// @brief Parse list of variable declaration. Note that if the
/// "storage_class_flag" variable contains the storage class "typedef", this
/// function will parse and return a typedef declaration, but if it doesn't, it
/// will parse and return a variable declaration.
/// @param typedef_generic_params CIGenericParams*?
static CIDecl *
parse_variable_list__CIParser(CIParser *self,
                              CIDecl *current_var,
                              int storage_class_flag,
                              CIDataType *data_type,
                              CIGenericParams *typedef_generic_params);

static CIDecl *
parse_label__CIParser(CIParser *self);

/// @brief Parse declaration.
static CIDecl *
parse_decl__CIParser(CIParser *self);

static CIAttribute *
parse_attribute__CIParser(CIParser *self);

/// @param attributes Vec<CIAttribute*>** (&)
static void
parse_attributes__CIParser(CIParser *self, Vec **attributes);

/// @brief Parse data type qualifier.
/// @return true if the token is a data type qualifier, false otherwise.
static bool
parse_data_type_qualifier__CIParser(CIParser *self,
                                    int *data_type_qualifier_flag);

/// @brief Parse many data type qualifiers.
static void
parse_data_type_qualifiers__CIParser(CIParser *self,
                                     int *data_type_qualifier_flag);

/// @brief Parse storage class specifier.
/// @return true if the token is a storage class specifier, false otherwise.
static bool
parse_storage_class_specifier__CIParser(CIParser *self,
                                        int *storage_class_flag);

/// @brief Parse many storage class specifiers.
static void
parse_storage_class_specifiers__CIParser(CIParser *self,
                                         int *storage_class_flag);

// NOTE: If the program is multi-threaded, you'll need to adapt these variables
// to multi-threading.
// If a name (of a variable, a function, etc.) is expected by the compiler, but
// is not here, it will generate a name and therefore increment the
// `name_error_count` and store all `name_error` in the `names_error` vector, as
// declaration strucutures (CIDeclVariable, CIDeclFunction, ...), are not
// supposed to own the name (in other words not to be able to free it).
static Vec *names_error = NULL; // Vec<Rc<String*>*>*
static Usize name_error_count = 0;
// The `data_type_as_expression` variable is used to keep the `DataType`
// pointer, when in `parse_decl__CIParser`, when `in_function_body` is true is
// that the following (current) token is not a `=`, `;` or `(`. Then, in the
// `parse_expr__CIParser` function, if `data_type_as_expression` is not NULL,
// the data type is returned as an expression.
static CIDataType *data_type_as_expression = NULL;

static int storage_class_flag = CI_STORAGE_CLASS_NONE;

#define RESET_STORAGE_CLASS_FLAG() storage_class_flag = CI_STORAGE_CLASS_NONE;
#define HAS_TYPEDEF_STORAGE_CLASS_FLAG() \
    (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF)

static int data_type_qualifier_flag = CI_DATA_TYPE_QUALIFIER_NONE;

#define RESET_DATA_TYPE_QUALIFIER_FLAG() \
    data_type_qualifier_flag = CI_DATA_TYPE_QUALIFIER_NONE;

// The `in_label` variable checks whether you're in a label and emits an error
// if there's a variable declaration in a label (also in case and default
// statetement).
static bool in_label = false;

#define ENABLE_IN_LABEL() in_label = true;
#define DISABLE_IN_LABEL() \
    if (in_label) {        \
        in_label = false;  \
    }

// Represent the `scope` of the current block being analyzed.
static CIScope *current_scope = NULL; // CIScope*?

// Represent the `body` of the current block being analyzed.
static CIDeclFunctionBody *current_body = NULL; // CIDeclFunctionBody*?

// Allow initialization.
//
// e.g.
//
// Array: { 3, 2, 1, ... }
//
// Struct call: { .a = 1, .b = 2, ... } or { 1, 2, ... }
static bool allow_initialization = false;

#define ENABLE_ALLOW_INITIALIZATION() allow_initialization = true;
#define DISABLE_ALLOW_INITIALIZATION() allow_initialization = false;

static const CIFeature *tokens_feature = NULL; // CIFeature* (&)

#define HAS_REACH_EOF(token) (token->kind == CI_TOKEN_KIND_EOF)
#define HAS_REACH_EOT(token) (token->kind == CI_TOKEN_KIND_EOT)

// The function is set to true if the parse is currently parseing the body of a
// function.
static bool in_function_body = false;

#define SET_IN_FUNCTION_BODY() in_function_body = true;
#define UNSET_IN_FUNCTION_BODY() in_function_body = false;

// In some situations we don't want to eat the `;` token, for example when
// parsing variable declarations in a `for` loop.
//
// e.g.
//
// for (int i = 0, a = 20;;);
static bool eat_semicolon = true;

#define SET_EAT_SEMICOLON() eat_semicolon = true;
#define UNSET_EAT_SEMICOLON() eat_semicolon = false;

static const SizedStr ci_standard_attributes[CI_N_STANDARD_ATTRIBUTE] = {
    SIZED_STR_FROM_RAW("_Noreturn"),
    SIZED_STR_FROM_RAW("___Noreturn__"),
    SIZED_STR_FROM_RAW("__deprecated__"),
    SIZED_STR_FROM_RAW("__falthrough__"),
    SIZED_STR_FROM_RAW("__maybe_unused__"),
    SIZED_STR_FROM_RAW("__nodiscard__"),
    SIZED_STR_FROM_RAW("__noreturn__"),
    SIZED_STR_FROM_RAW("__unsequenced__"),
    SIZED_STR_FROM_RAW("__reproducible__"),
    SIZED_STR_FROM_RAW("deprecated"),
    SIZED_STR_FROM_RAW("fallthrough"),
    SIZED_STR_FROM_RAW("maybe_unused"),
    SIZED_STR_FROM_RAW("nodiscard"),
    SIZED_STR_FROM_RAW("noreturn"),
    SIZED_STR_FROM_RAW("unsequenced"),
    SIZED_STR_FROM_RAW("reproducible"),
};

static const enum CIAttributeStandardKind
  ci_standard_attribute_ids[CI_N_STANDARD_ATTRIBUTE] = {
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED,
      CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH,
      CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED,
      CI_ATTRIBUTE_STANDARD_KIND_NODISCARD,
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED,
      CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE,
      CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED,
      CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH,
      CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED,
      CI_ATTRIBUTE_STANDARD_KIND_NODISCARD,
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED,
      CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE
  };

CONSTRUCTOR(CIParser, CIParser, CIResultFile *file)
{
    if (!names_error) {
        names_error = NEW(Vec);
    }

    if (!tokens_feature) {
        tokens_feature = get_tokens_feature__CIScanner();
    }

    CIParserSpan current_span = default__CIParserSpan();

    return (CIParser){ .file = file,
                       .count_error = &file->file_analysis->count_error,
                       .count_warning = &file->file_analysis->count_warning,
                       .resolved_tokens = NULL,
                       .resolved_tokens_count = 0,
                       .current_token = NULL,
                       .previous_token = NULL,
                       .current_span = current_span,
                       .previous_span = current_span };
}

Rc *
generate_name_error__CIParser()
{
    ASSERT(names_error);

    String *name_error = format__String("%__error__{zu}", name_error_count++);
    Rc *rc_name_error = NEW(Rc, name_error);

    push__Vec(names_error, rc_name_error);

    return rc_name_error;
}

void
set_current_token__CIParser(CIParser *self, CIToken *next_token)
{
    self->previous_token =
      self->current_token ? self->current_token : next_token;
    self->current_token = next_token;

    if (is_same_filename__CIResultFile(
          self->file, self->current_token->location.filename)) {
        self->previous_span = self->current_span;
        self->current_span = from_token__CIParserSpan(self->current_token);
    }
}

void
init__CIParser(CIParser *self, const CIResolvedTokens *resolved_tokens)
{
    ASSERT(resolved_tokens);
    ASSERT(count__CIResolvedTokens(resolved_tokens) > 0);

    CIToken *first_token = get__CIResolvedTokens(resolved_tokens, 0);

    check_standard__CIParser(self, first_token);

    self->resolved_tokens = resolved_tokens;
    self->current_token = first_token;
    self->previous_token = self->current_token;
    self->current_span = from_token__CIParserSpan(self->current_token);
    self->previous_span = self->current_span;
    ++self->resolved_tokens_count;
}

void
set_current_scope__CIParser(CIParser *self)
{
    current_scope = self->file->scope_base;
}

void
check_standard__CIParser(CIParser *self, CIToken *token)
{
    ASSERT(token->kind < CI_TOKEN_KIND_MAX);

    Location location_error = clone__Location(&token->location);
    const CIFeature *feature = &tokens_feature[token->kind];

    CI_CHECK_STANDARD_SINCE(
      self->file->entity.result->config->standard, feature->since, {
          enum CIErrorKind error_kind;

          switch (feature->since) {
              case CI_STANDARD_NONE:
                  return;
              case CI_STANDARD_KR:
                  UNREACHABLE("since: no error with K&R standard");
              case CI_STANDARD_89:
                  error_kind = CI_ERROR_KIND_REQUIRED_C89_OR_LATER;

                  break;
              case CI_STANDARD_95:
                  error_kind = CI_ERROR_KIND_REQUIRED_C95_OR_LATER;

                  break;
              case CI_STANDARD_99:
                  error_kind = CI_ERROR_KIND_REQUIRED_C99_OR_LATER;

                  break;
              case CI_STANDARD_11:
                  error_kind = CI_ERROR_KIND_REQUIRED_C11_OR_LATER;

                  break;
              case CI_STANDARD_17:
                  error_kind = CI_ERROR_KIND_REQUIRED_C17_OR_LATER;

                  break;
              case CI_STANDARD_23:
                  error_kind = CI_ERROR_KIND_REQUIRED_C23_OR_LATER;

                  break;
              default:
                  UNREACHABLE("unknown standard");
          }

          emit__Diagnostic(NEW_VARIANT(Diagnostic,
                                       simple_ci_error,
                                       &self->file->file_input,
                                       &location_error,
                                       NEW(CIError, error_kind),
                                       NULL,
                                       NULL,
                                       NULL),
                           self->count_error);
      });

    CI_CHECK_STANDARD_UNTIL(
      self->file->entity.result->config->standard, feature->until, {
          String *note = NULL;

          switch (feature->until) {
              case CI_STANDARD_NONE:
                  return;
              case CI_STANDARD_KR:
                  UNREACHABLE("since: no error with K&R standard");
              case CI_STANDARD_89:
                  note =
                    from__String("this feature is no longer available in C89");

                  break;
              case CI_STANDARD_95:
                  note =
                    from__String("this feature is no longer available in C95");

                  break;
              case CI_STANDARD_99:
                  note =
                    from__String("this feature is no longer available in C99");

                  break;
              case CI_STANDARD_11:
                  note =
                    from__String("this feature is no longer available in C11");

                  break;
              case CI_STANDARD_17:
                  note =
                    from__String("this feature is no longer available in C17");

                  break;
              case CI_STANDARD_23:
                  note =
                    from__String("this feature is no longer available in C23");

                  break;
              default:
                  UNREACHABLE("unknown standard");
          }

          emit_note__Diagnostic(NEW_VARIANT(Diagnostic,
                                            simple_ci_note,
                                            &self->file->file_input,
                                            &location_error,
                                            note,
                                            NULL,
                                            NULL,
                                            NULL));
      });
}

void
next_token__CIParser(CIParser *self)
{
    if (self->resolved_tokens_count <
        count__CIResolvedTokens(self->resolved_tokens)) {
        CIToken *next_token = get__CIResolvedTokens(
          self->resolved_tokens, self->resolved_tokens_count++);

        ASSERT(next_token);

        check_standard__CIParser(self, next_token);
        set_current_token__CIParser(self, next_token);
    }
}

CIToken *
peek_token__CIParser(CIParser *self, Usize n)
{
    CIToken *peeked = self->current_token;
    Usize resolved_tokens_count = self->resolved_tokens_count;
    Usize resolved_tokens_total_count =
      count__CIResolvedTokens(self->resolved_tokens);

    while (resolved_tokens_count < resolved_tokens_total_count && n--) {
        peeked =
          get__CIResolvedTokens(self->resolved_tokens, resolved_tokens_count++);
    }

    return peeked;
}

bool
expect__CIParser(CIParser *self, enum CITokenKind kind, bool emit_error)
{
    if (self->current_token->kind == kind) {
        next_token__CIParser(self);

        return true;
    }

    if (emit_error) {
        FAILED("expected: ...");
    }

    return false;
}

bool
expect_many__CIParser(CIParser *self, Usize n, ...)
{
    ASSERT(n > 0);

    va_list vl;

    va_start(vl, n);

    while (n--) {
        if (!expect__CIParser(self, va_arg(vl, enum CITokenKind), true)) {
            return false;
        }
    }

    va_end(vl);

    return true;
}

void
expect_with_list__CIParser(CIParser *self, Usize n, ...)
{
    ASSERT(n > 0);

    va_list vl;

    va_start(vl, n);

    while (n--) {
        if (expect__CIParser(self, va_arg(vl, enum CITokenKind), false)) {
            return;
        }
    }

    va_end(vl);

    FAILED("expected ...");
}

void
jump__CIParser(CIParser *self, Usize n)
{
    while (n--) {
        next_token__CIParser(self);
    }
}

bool
token_is_data_type__CIParser(CIParser *self, const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return is__CIBuiltinType(GET_PTR_RC(String, token->identifier)) ||
                   search_typedef__CIResultFile(
                     self->file, GET_PTR_RC(String, token->identifier));
        case CI_TOKEN_KIND_AT: // TODO: check if the next token is an identifier
                               // (not needed for the moment)
        case CI_TOKEN_KIND_KEYWORD_BOOL:
        case CI_TOKEN_KIND_KEYWORD_CHAR:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_ENUM:
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
        case CI_TOKEN_KIND_KEYWORD_INT:
        case CI_TOKEN_KIND_KEYWORD_LONG:
        case CI_TOKEN_KIND_KEYWORD_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
        case CI_TOKEN_KIND_KEYWORD_TYPEOF:
        case CI_TOKEN_KIND_KEYWORD_UNION:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
        case CI_TOKEN_KIND_KEYWORD_VOID:
        case CI_TOKEN_KIND_KEYWORD__BOOL:
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return true;
        default:
            return token_is_data_type_qualifier__CIParser(self, token);
    }
}

bool
is_data_type__CIParser(CIParser *self)
{
    return token_is_data_type__CIParser(self, self->current_token);
}

bool
token_is_storage_class__CIParser(CIParser *self, const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_KEYWORD_AUTO:
        case CI_TOKEN_KIND_KEYWORD_CONSTEXPR:
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
        case CI_TOKEN_KIND_KEYWORD_INLINE:
        case CI_TOKEN_KIND_KEYWORD_REGISTER:
        case CI_TOKEN_KIND_KEYWORD_STATIC:
        case CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL:
        case CI_TOKEN_KIND_KEYWORD__THREAD_LOCAL:
        case CI_TOKEN_KIND_KEYWORD_TYPEDEF:
            return true;
        default:
            return false;
    }
}

bool
is_storage_class__CIParser(CIParser *self)
{
    return token_is_storage_class__CIParser(self, self->current_token);
}

bool
token_is_data_type_qualifier__CIParser(CIParser *self, const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_KEYWORD_CONST:
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
        case CI_TOKEN_KIND_KEYWORD_RESTRICT:
        case CI_TOKEN_KIND_KEYWORD__ATOMIC:
        case CI_TOKEN_KIND_KEYWORD__NORETURN:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT__:
            return true;
        default:
            return false;
    }
}

bool
is_data_type_qualifier__CIParser(CIParser *self)
{
    return token_is_data_type_qualifier__CIParser(self, self->current_token);
}

CIDataType *
substitute_generic__CIParser(const String *generic_name,
                             CIGenericParams *generic_params,
                             CIGenericParams *called_generic_params)
{
    Usize id = 0;

    for (Usize i = 0; i < generic_params->params->len; ++i) {
        CIDataType *generic_param = get__Vec(generic_params->params, i);

        switch (generic_param->kind) {
            case CI_DATA_TYPE_KIND_GENERIC:
                if (!strcmp(GET_PTR_RC(String, generic_param->generic)->buffer,
                            generic_name->buffer)) {
                    id = i;
                    goto exit_loop;
                }

                break;
            default:
                FAILED("expected only generic data type for the moment");
        }
    }

    FAILED("generic param name not found");

    return NULL;

exit_loop:
    return get__Vec(called_generic_params->params, id);
}

CIGenericParams *
substitute_generic_params__CIParser(
  CIGenericParams *unresolved_generic_params,
  const CIGenericParams *generic_params,
  const CIGenericParams *called_generic_params)
{
    if (unresolved_generic_params) {
        if (generic_params && called_generic_params &&
            has_generic__CIGenericParams(unresolved_generic_params)) {
            Vec *subs_params = NEW(Vec); // Vec<CIDataType*>*

            for (Usize i = 0; i < unresolved_generic_params->params->len; ++i) {
                CIDataType *subs_param = substitute_data_type__CIParser(
                  get__Vec(unresolved_generic_params->params, i),
                  (CIGenericParams *)generic_params,
                  (CIGenericParams *)called_generic_params);

                if (subs_param) {
                    push__Vec(subs_params, subs_param);
                }
            }

            return NEW(CIGenericParams, subs_params);
        }

        return ref__CIGenericParams(unresolved_generic_params);
    }

    return NULL;
}

CIDataType *
substitute_data_type__CIParser(CIDataType *data_type,
                               CIGenericParams *generic_params,
                               CIGenericParams *called_generic_params)
{
    CIDataType *res = NULL;

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY: {
            CIDataType *subs = substitute_data_type__CIParser(
              data_type, generic_params, called_generic_params);

            if (subs) {
                switch (data_type->array.kind) {
                    case CI_DATA_TYPE_ARRAY_KIND_SIZED:
                        res = NEW_VARIANT(CIDataType,
                                          array,
                                          NEW_VARIANT(CIDataTypeArray,
                                                      sized,
                                                      subs,
                                                      data_type->array.name,
                                                      data_type->array.size));

                        break;
                    case CI_DATA_TYPE_ARRAY_KIND_NONE:
                        res = NEW_VARIANT(CIDataType,
                                          array,
                                          NEW_VARIANT(CIDataTypeArray,
                                                      none,
                                                      subs,
                                                      data_type->array.name));

                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }
            } else {
                return ref__CIDataType(data_type);
            }

            break;
        }
        case CI_DATA_TYPE_KIND_FUNCTION: {
            Vec *subs_params = NEW(Vec);

            for (Usize i = 0; i < data_type->function.params->len; ++i) {
                CIDeclFunctionParam *param =
                  get__Vec(data_type->function.params, i);

                switch (param->kind) {
                    case CI_DECL_FUNCTION_PARAM_KIND_NORMAL: {
                        CIDataType *subs_data_type =
                          substitute_data_type__CIParser(param->data_type,
                                                         generic_params,
                                                         called_generic_params);

                        if (subs_data_type) {
                            push__Vec(subs_params,
                                      NEW_VARIANT(CIDeclFunctionParam,
                                                  normal,
                                                  param->name,
                                                  subs_data_type));
                        }

                        break;
                    }
                    case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
                        push__Vec(subs_params,
                                  NEW_VARIANT(CIDeclFunctionParam, variadic));

                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }
            }

            CIDataType *subs_return_data_type = substitute_data_type__CIParser(
              data_type->function.return_data_type,
              generic_params,
              called_generic_params);

            if (subs_return_data_type) {
                FREE_BUFFER_ITEMS(
                  subs_params->buffer, subs_params->len, CIDataType);
                FREE(Vec, subs_params);

                return ref__CIDataType(data_type);
            }

            res = NEW_VARIANT(
              CIDataType,
              function,
              NEW(CIDataTypeFunction,
                  data_type->function.name,
                  subs_params,
                  subs_return_data_type,
                  clone__CIDataType(data_type->function.function_data_type)));

            break;
        }
        case CI_DATA_TYPE_KIND_GENERIC: {
            CIDataType *res_generic = substitute_generic__CIParser(
              GET_PTR_RC(String, data_type->generic),
              generic_params,
              called_generic_params);

            if (res_generic) {
                return ref__CIDataType(res_generic);
            }

            return NULL;
        }
        case CI_DATA_TYPE_KIND_PTR: {
            if (data_type->ptr) {
                CIDataType *subs = substitute_data_type__CIParser(
                  data_type->ptr, generic_params, called_generic_params);

                if (subs) {
                    res = NEW_VARIANT(CIDataType, ptr, subs);
                } else {
                    return ref__CIDataType(data_type);
                }
            } else {
                return ref__CIDataType(data_type);
            }

            break;
        }
        case CI_DATA_TYPE_KIND_STRUCT: {
#define SUBSTITUTE_STRUCT_OR_UNION(decl_name, decl_ty, variant)               \
    if (data_type->decl_name.generic_params || data_type->decl_name.fields) { \
        Vec *subs_fields = NULL;                                              \
                                                                              \
        if (data_type->decl_name.fields) {                                    \
            subs_fields = NEW(Vec);                                           \
                                                                              \
            for (Usize i = 0; i < data_type->decl_name.fields->len; ++i) {    \
                CIDeclStructField *field =                                    \
                  get__Vec(data_type->decl_name.fields, i);                   \
                CIDataType *subs_field_dt = substitute_data_type__CIParser(   \
                  field->data_type, generic_params, called_generic_params);   \
                                                                              \
                push__Vec(subs_fields,                                        \
                          NEW(CIDeclStructField,                              \
                              field->name,                                    \
                              subs_field_dt,                                  \
                              field->bit));                                   \
            }                                                                 \
        }                                                                     \
                                                                              \
        res = NEW_VARIANT(CIDataType,                                         \
                          variant,                                            \
                          NEW(decl_ty,                                        \
                              data_type->decl_name.name,                      \
                              substitute_generic_params__CIParser(            \
                                data_type->decl_name.generic_params,          \
                                generic_params,                               \
                                called_generic_params),                       \
                              subs_fields));                                  \
                                                                              \
        break;                                                                \
    }                                                                         \
                                                                              \
    return ref__CIDataType(data_type);

            SUBSTITUTE_STRUCT_OR_UNION(struct_, CIDataTypeStruct, struct);
        }
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            if (data_type->typedef_.generic_params) {
                res = NEW_VARIANT(CIDataType,
                                  typedef,
                                  NEW(CIDataTypeTypedef,
                                      data_type->typedef_.name,
                                      substitute_generic_params__CIParser(
                                        data_type->typedef_.generic_params,
                                        generic_params,
                                        called_generic_params)));
            } else {
                return ref__CIDataType(data_type);
            }

            break;
        }
        case CI_DATA_TYPE_KIND_UNION: {
            SUBSTITUTE_STRUCT_OR_UNION(union_, CIDataTypeUnion, union);
        }
        default:
            return ref__CIDataType(data_type);
    }

    set_context__CIDataType(res, data_type->ctx);
    set_qualifier__CIDataType(res, data_type->qualifier);

    return res;
}

int
parse_data_type_contexts__CIParser(CIParser *self)
{
    int data_type_ctx = CI_DATA_TYPE_CONTEXT_NONE;

    while (self->current_token->kind == CI_TOKEN_KIND_BANG) {
        next_token__CIParser(self);

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_IDENTIFIER: {
#define CTX_IDS_LEN 4
                enum CIDataTypeContext ctx_ids[CTX_IDS_LEN] = {
                    CI_DATA_TYPE_CONTEXT_HEAP,
                    CI_DATA_TYPE_CONTEXT_NON_NULL,
                    CI_DATA_TYPE_CONTEXT_STACK,
                    CI_DATA_TYPE_CONTEXT_TRACE
                };
                static SizedStr ctx_ids_s[CTX_IDS_LEN] = {
                    SIZED_STR_FROM_RAW("heap"),
                    SIZED_STR_FROM_RAW("non_null"),
                    SIZED_STR_FROM_RAW("stack"),
                    SIZED_STR_FROM_RAW("trace")
                };
                int current_ctx = get_id__Search(
                  GET_PTR_RC(String, self->current_token->identifier),
                  ctx_ids_s,
                  (const Int32 *)ctx_ids,
                  CTX_IDS_LEN);

                if (current_ctx == -1) {
                    FAILED("expected only !heap, !non_null, !stack or !trace "
                           "as data type context");
                } else {
                    data_type_ctx |= current_ctx;
                }

#undef CTX_IDS_LEN

                next_token__CIParser(self);

                break;
            }
            default:
                FAILED(
                  "expected identifier after data type context, e.g. !<id>");
        }
    }

    return data_type_ctx;
}

CIDataType *
parse_post_data_type__CIParser(CIParser *self, CIDataType *data_type)
{
    bool is_data_type_qualifier = false;

    while (self->current_token->kind == CI_TOKEN_KIND_STAR ||
           (is_data_type_qualifier = is_data_type_qualifier__CIParser(self))) {
        if (is_data_type_qualifier) {
            parse_data_type_qualifiers__CIParser(self,
                                                 &data_type_qualifier_flag);
            set_qualifier__CIDataType(data_type, data_type_qualifier_flag);

            RESET_DATA_TYPE_QUALIFIER_FLAG();
            is_data_type_qualifier = false;

            continue;
        }

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_STAR:
                data_type = NEW_VARIANT(CIDataType, ptr, data_type);

                break;
            default:
                goto exit;
        }

        next_token__CIParser(self);
    }

exit:
    set_and_check_data_type_context__CIParser(
      self, data_type, parse_data_type_contexts__CIParser(self));

    return data_type;
}

const CIDecl *
add_decl_to_scope__CIParser(const CIParser *self,
                            CIDecl **decl_ref,
                            const CIScope *scope,
                            bool must_free,
                            bool in_function_body)
{
    return add_decl_to_scope__CIResultFile(
      self->file, decl_ref, scope, must_free, in_function_body);
}

void
parse_variable_name_and_data_type__CIParser(CIParser *self,
                                            CIDataType **data_type_ref,
                                            Rc **name_ref,
                                            bool name_is_required)
{
#define PARSE_VARIABLE_NAME(block)                           \
    switch (self->current_token->kind) {                     \
        case CI_TOKEN_KIND_IDENTIFIER:                       \
            if (*name_ref) {                                 \
                FAILED("identifier is already declared");    \
            } else {                                         \
                *name_ref = self->current_token->identifier; \
                                                             \
                next_token__CIParser(self);                  \
            }                                                \
                                                             \
            break;                                           \
            block;                                           \
        default:                                             \
            if (name_is_required) {                          \
                FAILED("expected identifier");               \
                /* return; */                                \
            }                                                \
    }

    ASSERT(data_type_ref);
    ASSERT(name_ref);

    PARSE_VARIABLE_NAME(case CI_TOKEN_KIND_LPAREN : break;);

    Usize i = 0;

loop:
    i += 1;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LHOOK: {
            next_token__CIParser(self); // skip `[`

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_RHOOK:
                    // <id>[n][p]
                    *data_type_ref =
                      NEW_VARIANT(CIDataType,
                                  array,
                                  NEW_VARIANT(CIDataTypeArray,
                                              none,
                                              *data_type_ref,
                                              i == 1 ? *name_ref : NULL));

                    next_token__CIParser(self);

                    goto loop;
                default:
                    break;
            }

            CIExpr *expr = parse_expr__CIParser(self);

            if (!expr) {
                return;
            }

            const CIResolverExpr resolver_expr =
              NEW(CIResolverExpr, self, current_scope, false);
            CIExpr *resolved_expr = run__CIResolverExpr(&resolver_expr, expr);

            Usize size = 0;

            switch (resolved_expr->kind) {
                case CI_EXPR_KIND_LITERAL:
                    switch (resolved_expr->literal.kind) {
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
                            if (resolved_expr->literal.signed_int < 0) {
                                FAILED("indexed integer must be positive");
                            } else {
                                size = resolved_expr->literal.signed_int;
                            }

                            break;
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
                            size = resolved_expr->literal.unsigned_int;

                            break;
                        default:
                            goto expected_integer_literal;
                    }

                    break;
                default:
                expected_integer_literal:
                    FAILED("expected integer literal expression");
            }

            FREE(CIExpr, expr);
            FREE(CIExpr, resolved_expr);

            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);

            *data_type_ref = NEW_VARIANT(CIDataType,
                                         array,
                                         NEW_VARIANT(CIDataTypeArray,
                                                     sized,
                                                     *data_type_ref,
                                                     i == 1 ? *name_ref : NULL,
                                                     size));

            goto loop;
        }
        case CI_TOKEN_KIND_LPAREN: {
            // NOTE: If the name is not NULL at this level, this is the
            // beginning of a function declaration and not a function data type.
            if (*name_ref) {
                goto exit;
            }

            next_token__CIParser(self); // `(`

            CIDataType *function_data_type = NULL; // CIDataType*?

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_STAR:
                    function_data_type =
                      parse_post_data_type__CIParser(self, NULL);

                    break;
                default:
                    break;
            }

            PARSE_VARIABLE_NAME({});

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

            Vec *params = NULL;

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_LPAREN: {
                    CIScope *parent_function_scope = add_scope__CIResultFile(
                      self->file, current_scope->scope_id, true);
                    params = parse_function_params__CIParser(
                      self, parent_function_scope);

                    break;
                }
                default:
                    break;
            }

            *data_type_ref = NEW_VARIANT(CIDataType,
                                         function,
                                         NEW(CIDataTypeFunction,
                                             *name_ref,
                                             params,
                                             *data_type_ref,
                                             function_data_type));

            goto loop;
        }
        default:
            goto exit;
    }

exit:
#undef PARSE_VARIABLE_NAME
}

void
parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
  CIParser *self,
  int *data_type_qualifier_flag,
  int *storage_class_flag)
{
    bool is_data_type_qualifier = false;
    bool is_storage_class_specifier = false;

    while ((is_data_type_qualifier = is_data_type_qualifier__CIParser(self)) ||
           (is_storage_class_specifier = is_storage_class__CIParser(self))) {
        if (is_data_type_qualifier) {
            parse_data_type_qualifiers__CIParser(self,
                                                 data_type_qualifier_flag);
        } else if (is_storage_class_specifier) {
            parse_storage_class_specifiers__CIParser(self, storage_class_flag);
        }
    }
}

void
valid_data_type_context__CIParser(CIParser *self, int context_flag)
{
    static enum CIDataTypeContext rejected_context_when_none_is_present[] = {};
    static enum CIDataTypeContext rejected_context_when_heap_is_present[] = {
        CI_DATA_TYPE_CONTEXT_STACK
    };
    static enum CIDataTypeContext
      rejected_context_when_non_null_is_present[] = {};
    static enum CIDataTypeContext rejected_context_when_stack_is_present[] = {
        CI_DATA_TYPE_CONTEXT_HEAP
    };
    static enum CIDataTypeContext rejected_context_when_trace_is_present[] = {};
    typedef struct CIDataTypeRejectedContexts
    {
        enum CIDataTypeContext *buffer;
        Usize len;
    } CIDataTypeRejectedContexts;
    static CIDataTypeRejectedContexts
      rejected_context_when_x_context_is_present[] = {
          { rejected_context_when_none_is_present,
            sizeof(rejected_context_when_none_is_present) /
              sizeof(*rejected_context_when_none_is_present) },
          { rejected_context_when_heap_is_present,
            sizeof(rejected_context_when_heap_is_present) /
              sizeof(*rejected_context_when_heap_is_present) },
          { rejected_context_when_non_null_is_present,
            sizeof(rejected_context_when_non_null_is_present) /
              sizeof(*rejected_context_when_non_null_is_present) },
          { rejected_context_when_stack_is_present,
            sizeof(rejected_context_when_stack_is_present) /
              sizeof(*rejected_context_when_stack_is_present) },
          { rejected_context_when_trace_is_present,
            sizeof(rejected_context_when_trace_is_present) /
              sizeof(*rejected_context_when_trace_is_present) }
      };
    CIDataTypeContextIds data_type_context_ids = get_ids__CIDataTypeContext();

    ASSERT(data_type_context_ids.len ==
           sizeof(rejected_context_when_x_context_is_present) /
             sizeof(*rejected_context_when_x_context_is_present));

    for (Usize i = 0; i < data_type_context_ids.len; ++i) {
        if (context_flag & data_type_context_ids.buffer[i]) {
            const CIDataTypeRejectedContexts *rejected_context_item =
              &rejected_context_when_x_context_is_present[i];

            for (Usize j = 0; j < rejected_context_item->len; ++j) {
                if (context_flag & rejected_context_item->buffer[j]) {
                    FAILED("incompatible data type context");
                }
            }
        }
    }
}

void
set_and_check_data_type_context__CIParser(CIParser *self,
                                          CIDataType *data_type,
                                          int context_flag)
{
    valid_data_type_context__CIParser(self, context_flag);
    set_context__CIDataType(data_type, context_flag);
}

void
add_flag__CIDataTypeCombination(CIParser *self,
                                Uint32 *flags,
                                enum CIDataTypeCombination flag)
{
    Uint32 old_flags = *flags;

    *flags |= flag;

    if (old_flags == *flags) {
        // Set `long long` flag if `long` flag is already set.
        if (flag == CI_DATA_TYPE_COMBINATION_LONG) {
            *flags &= ~CI_DATA_TYPE_COMBINATION_LONG;
            *flags |= CI_DATA_TYPE_COMBINATION_LONG_LONG;
        } else {
            FAILED("flag already set");
        }
    }
}

enum CIDataTypeCombination
convert_token_kind_to_data_type_combination__CIDataTypeCombination(
  enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_KEYWORD_CHAR:
            return CI_DATA_TYPE_COMBINATION_CHAR;
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
            return CI_DATA_TYPE_COMBINATION_DOUBLE;
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            return CI_DATA_TYPE_COMBINATION_FLOAT;
        case CI_TOKEN_KIND_KEYWORD_INT:
            return CI_DATA_TYPE_COMBINATION_INT;
        case CI_TOKEN_KIND_KEYWORD_LONG:
            return CI_DATA_TYPE_COMBINATION_LONG;
        case CI_TOKEN_KIND_KEYWORD_SHORT:
            return CI_DATA_TYPE_COMBINATION_SHORT;
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
            return CI_DATA_TYPE_COMBINATION_SIGNED;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
            return CI_DATA_TYPE_COMBINATION_UNSIGNED;
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
            return CI_DATA_TYPE_COMBINATION__COMPLEX;
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return CI_DATA_TYPE_COMBINATION__IMAGINARY;
        default:
            return CI_DATA_TYPE_COMBINATION_NONE;
    }
}

bool
is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_KEYWORD_CHAR:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
        case CI_TOKEN_KIND_KEYWORD_INT:
        case CI_TOKEN_KIND_KEYWORD_LONG:
        case CI_TOKEN_KIND_KEYWORD_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return true;
        default:
            return false;
    }
}

int
convert_data_type_combination_to_data_type__CIParser(CIParser *self,
                                                     int combination)
{
    switch (combination) {
        case CI_DATA_TYPE_COMBINATION_CHAR:
            return CI_DATA_TYPE_KIND_CHAR;
        case CI_DATA_TYPE_COMBINATION_DOUBLE:
            return CI_DATA_TYPE_KIND_DOUBLE;
        case CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__COMPLEX:
            return CI_DATA_TYPE_KIND_DOUBLE__COMPLEX;
        case CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__IMAGINARY:
            return CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY;
        case CI_DATA_TYPE_COMBINATION_FLOAT:
            return CI_DATA_TYPE_KIND_FLOAT;
        case CI_DATA_TYPE_COMBINATION_FLOAT | CI_DATA_TYPE_COMBINATION__COMPLEX:
            return CI_DATA_TYPE_KIND_FLOAT__COMPLEX;
        case CI_DATA_TYPE_COMBINATION_FLOAT |
          CI_DATA_TYPE_COMBINATION__IMAGINARY:
            return CI_DATA_TYPE_KIND_FLOAT__IMAGINARY;
        case CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_INT;
        case CI_DATA_TYPE_COMBINATION_LONG:
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_LONG:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_LONG |
          CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_DOUBLE:
            return CI_DATA_TYPE_KIND_LONG_DOUBLE;
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__COMPLEX:
            return CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX;
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__IMAGINARY:
            return CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY;
        case CI_DATA_TYPE_COMBINATION_LONG_LONG:
        case CI_DATA_TYPE_COMBINATION_LONG_LONG | CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG:
        case CI_DATA_TYPE_COMBINATION_SIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_LONG_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_SHORT:
        case CI_DATA_TYPE_COMBINATION_SHORT | CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_SHORT:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_SHORT |
          CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_SHORT_INT;
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_CHAR:
            return CI_DATA_TYPE_KIND_SIGNED_CHAR;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_INT;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_CHAR:
            return CI_DATA_TYPE_KIND_UNSIGNED_CHAR;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_LONG:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_LONG |
          CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_SHORT:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED |
          CI_DATA_TYPE_COMBINATION_SHORT | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT;
        default:
            return -1;
    }
}

int
parse_data_type_combination__CIParser(CIParser *self)
{
    Uint32 keyword_multi_part_flag = 0;

    while (is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
      self->previous_token->kind)) {
        enum CIDataTypeCombination data_type_combination =
          convert_token_kind_to_data_type_combination__CIDataTypeCombination(
            self->previous_token->kind);

        add_flag__CIDataTypeCombination(
          self, &keyword_multi_part_flag, data_type_combination);

        if (is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
              self->current_token->kind)) {
            next_token__CIParser(self);
        } else {
            break;
        }
    }

    return convert_data_type_combination_to_data_type__CIParser(
      self, keyword_multi_part_flag);
}

CIDataType *
parse_pre_data_type__CIParser(CIParser *self)
{
    CIDataType *res = NULL;

    // <storage_class_flag | dt_qualifier> <pre_dt>
    // e.g. static int, const int
    parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
      self, &data_type_qualifier_flag, &storage_class_flag);
    next_token__CIParser(self);

    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            if (is__CIBuiltinType(
                  GET_PTR_RC(String, self->previous_token->identifier))) {
                res = NEW_VARIANT(CIDataType,
                                  builtin,
                                  get_id__CIBuiltinType(GET_PTR_RC(
                                    String, self->previous_token->identifier)));

                break;
            } else if (!search_typedef__CIResultFile(
                         self->file,
                         GET_PTR_RC(String,
                                    self->previous_token->identifier))) {
                FAILED("expected type");
            }

            Rc *name = self->previous_token->identifier; // Rc<String*>* (&)
            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self);

            res = NEW_VARIANT(CIDataType,
                              typedef,
                              NEW(CIDataTypeTypedef, name, generic_params));

            break;
        }
        case CI_TOKEN_KIND_AT: {
            Rc *generic = NULL; // Rc<String*>* (&)

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                generic = self->previous_token->identifier;
            } else {
                generic = generate_name_error__CIParser();
            }

            res = NEW_VARIANT(CIDataType, generic, generic);

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_BOOL:
        case CI_TOKEN_KIND_KEYWORD__BOOL:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_BOOL);

            break;
        case CI_TOKEN_KIND_KEYWORD_ENUM: {
            Rc *name = NULL; // Rc<String*>*? (&)

            // enum <name> ...;
            // enum <name> : ...;
            // enum <name> { ... } ...;
            // enum { ... } ...;
            // NOTE: A enum can be anonymous.
            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    name = self->current_token->identifier;
                    next_token__CIParser(self);

                    break;
                default:
                    break;
            }

            res = NEW_VARIANT(
              CIDataType, enum, NEW(CIDataTypeEnum, name, NULL, NULL));

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_COLON:
                case CI_TOKEN_KIND_LBRACE:
                case CI_TOKEN_KIND_SEMICOLON: {
                    CIDecl *enum_decl =
                      parse_enum__CIParser(self, CI_STORAGE_CLASS_NONE, name);

                    if (add_decl_to_scope__CIParser(self,
                                                    &enum_decl,
                                                    current_scope,
                                                    true,
                                                    in_function_body)) {
                        break;
                    }

                    if (!enum_decl->enum_.name) {
                        res->enum_.variants =
                          enum_decl->enum_.variants
                            ? clone_variants__CIDeclEnumVariant(
                                enum_decl->enum_.variants)
                            : NULL;
                        res->enum_.data_type =
                          enum_decl->enum_.data_type
                            ? ref__CIDataType(enum_decl->enum_.data_type)
                            : NULL;

                        FREE(CIDecl, enum_decl);
                    }

                    break;
                }
                default:
                    break;
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
        case CI_TOKEN_KIND_KEYWORD_UNION: {
            enum CITokenKind previous_token_kind = self->previous_token->kind;
            Rc *name = NULL; // Rc<String*>*? (&)

            // struct <name><generic_params, ...> ...;
            // struct <name><generic_params, ...> { ... } ...;
            // struct { ... } ...;
            // NOTE: A union or struct can be anonymous.
            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    name = self->current_token->identifier;
                    next_token__CIParser(self);

                    break;
                default:
                    break;
            }

            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self); // CIGenericParams*?

            switch (previous_token_kind) {
                case CI_TOKEN_KIND_KEYWORD_STRUCT:
                    res = NEW_VARIANT(
                      CIDataType,
                      struct,
                      NEW(CIDataTypeStruct, name, generic_params, NULL));

                    break;
                case CI_TOKEN_KIND_KEYWORD_UNION:
                    res = NEW_VARIANT(
                      CIDataType,
                      union,
                      NEW(CIDataTypeUnion, name, generic_params, NULL));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            switch (previous_token_kind) {
                case CI_TOKEN_KIND_KEYWORD_STRUCT: {
                    CIDecl *struct_decl = parse_struct__CIParser(
                      self,
                      CI_STORAGE_CLASS_NONE,
                      name,
                      generic_params ? ref__CIGenericParams(generic_params)
                                     : NULL);

                    if (add_decl_to_scope__CIParser(self,
                                                    &struct_decl,
                                                    current_scope,
                                                    true,
                                                    in_function_body)) {
                        break;
                    }

                    if (struct_decl->struct_.fields &&
                        !struct_decl->struct_.name) {
                        res->struct_.fields = clone_fields__CIDeclStructField(
                          struct_decl->struct_.fields);
                        FREE(CIDecl, struct_decl);
                    }

                    break;
                }
                case CI_TOKEN_KIND_KEYWORD_UNION: {
                    CIDecl *union_decl = parse_union__CIParser(
                      self,
                      CI_STORAGE_CLASS_NONE,
                      name,
                      generic_params ? ref__CIGenericParams(generic_params)
                                     : NULL);

                    if (add_decl_to_scope__CIParser(self,
                                                    &union_decl,
                                                    current_scope,
                                                    true,
                                                    in_function_body)) {
                        break;
                    }

                    if (union_decl->union_.fields && !union_decl->union_.name) {
                        res->union_.fields = clone_fields__CIDeclStructField(
                          union_decl->union_.fields);
                        FREE(CIDecl, union_decl);
                    }

                    break;
                }
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_TYPEOF: {
            expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

            CIExpr *expr = parse_expr__CIParser(self);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

            res = infer_expr_data_type__CIInfer(
              self->file,
              expr,
              current_scope ? current_scope->scope_id : NULL,
              NULL,
              NULL);

            FREE(CIExpr, expr);

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_VOID:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL128);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL32);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL64);

            break;
        default: {
            int data_type_kind = parse_data_type_combination__CIParser(self);

            if (data_type_kind == -1) {
                FAILED("expected data type");
            }

            res = NEW(CIDataType, data_type_kind);
        }
    }

    // <pre_dt> <storage_class_flag | dt_qualifier>
    // e.g. int static, int const
    parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
      self, &data_type_qualifier_flag, &storage_class_flag);

    set_qualifier__CIDataType(res, data_type_qualifier_flag);

    RESET_DATA_TYPE_QUALIFIER_FLAG();

    return res;
}

CIDataType *
parse_data_type__CIParser(CIParser *self)
{
    RESET_DATA_TYPE_QUALIFIER_FLAG();

    CIDataType *pre = parse_pre_data_type__CIParser(self);

    return parse_post_data_type__CIParser(self, pre);
}

Vec *
parse_enum_variants__CIParser(CIParser *self)
{
    Vec *variants = NEW(Vec); // Vec<CIDeclEnumVariant*>*
    Isize precedent_value = -1;

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        Rc *name = NULL; // Rc<String*>* (&)

        if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
            name = self->previous_token->identifier;
        } else {
            name = generate_name_error__CIParser();
        }

        CIDeclEnumVariant *variant = NULL;

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_EQ: {
                next_token__CIParser(self);

                CIExpr *custom_expr = parse_expr__CIParser(self);
                CIResolverExpr resolver_expr =
                  NEW(CIResolverExpr, self, current_scope, false);
                CIExpr *resolved_custom_expr =
                  run__CIResolverExpr(&resolver_expr, custom_expr);
                Isize custom_value = 0;

                switch (resolved_custom_expr->kind) {
                    case CI_EXPR_KIND_LITERAL:
                        custom_value = to_literal_integer_value__CIResolverExpr(
                          resolved_custom_expr);

                        break;
                    default:
                        FAILED(
                          "expected literal expression (passing a constant or "
                          "enum variant is not yet supported).");
                }

                precedent_value = custom_value;
                variant = NEW(CIDeclEnumVariant, name, custom_value);

                push__Vec(variants, variant);

                FREE(CIExpr, custom_expr);
                FREE(CIExpr, resolved_custom_expr);

                break;
            }
            default:
                variant = NEW(CIDeclEnumVariant, name, ++precedent_value);

                push__Vec(variants, variant);
        }

        {
            CIDecl *variant_decl = NEW_VARIANT(CIDecl,
                                               enum_variant,
                                               CI_STORAGE_CLASS_NONE,
                                               ref__CIDeclEnumVariant(variant));

            add_decl_to_scope__CIParser(
              self, &variant_decl, current_scope, true, false);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RBRACE) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    if (variants->len == 0) {
        FAILED("expected one or many variants");
    }

    return variants;
}

CIDecl *
parse_enum__CIParser(CIParser *self, int storage_class_flag, Rc *name)
{
    ASSERT(self->current_token->kind == CI_TOKEN_KIND_COLON ||
           self->current_token->kind == CI_TOKEN_KIND_LBRACE ||
           self->current_token->kind == CI_TOKEN_KIND_SEMICOLON);

    CIDataType *data_type = NULL;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_COLON:
            next_token__CIParser(self);

            data_type = parse_data_type__CIParser(self);

            break;
        default:
            break;
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            break;
        case CI_TOKEN_KIND_SEMICOLON:
            return NEW_VARIANT(CIDecl,
                               enum,
                               storage_class_flag,
                               true,
                               NEW(CIDeclEnum, name, NULL, data_type));
        default:
            FAILED("expected `{` or `;`");
    }

    return NEW_VARIANT(
      CIDecl,
      enum,
      storage_class_flag,
      false,
      NEW(CIDeclEnum, name, parse_enum_variants__CIParser(self), data_type));
}

Vec *
parse_function_params__CIParser(CIParser *self, CIScope *parent_function_scope)
{
    next_token__CIParser(self); // skip `(`

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_RPAREN:
            next_token__CIParser(self);

            return NULL;
        default:
            break;
    }

    Vec *params = NEW(Vec); // Vec<CIDeclFunctionParam*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_DOT_DOT_DOT: {
                push__Vec(params, NEW_VARIANT(CIDeclFunctionParam, variadic));
                next_token__CIParser(self);

                break;
            }
            default: {
                CIDataType *data_type = parse_data_type__CIParser(self);
                Rc *name = NULL; // Rc<String*>*? (&)

                parse_variable_name_and_data_type__CIParser(
                  self, &data_type, &name, false);

                push__Vec(
                  params,
                  NEW_VARIANT(CIDeclFunctionParam, normal, name, data_type));

                if (parent_function_scope && name) {
                    CIDecl *param_decl =
                      NEW_VARIANT(CIDecl,
                                  variable,
                                  CI_STORAGE_CLASS_NONE,
                                  false,
                                  NEW(CIDeclVariable,
                                      ref__CIDataType(data_type),
                                      name,
                                      NULL,
                                      true));

                    add_variable__CIResultFile(
                      self->file, parent_function_scope, param_decl);
                }
            }
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RPAREN) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    if (self->current_token->kind == CI_TOKEN_KIND_EOF) {
        FAILED("hit EOF");
    } else {
        next_token__CIParser(self); // skip `)`
    }

    return params;
}

CIGenericParams *
parse_generic_params__CIParser(CIParser *self)
{

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_DOT: {
            CIToken *peeked = peek_token__CIParser(self, 1);

            switch (peeked->kind) {
                case CI_TOKEN_KIND_LHOOK:
                    break;
                default:
                    return NULL;
            }

            jump__CIParser(self, 2);

            break;
        }
        default:
            return NULL;
    }

    Vec *params = NEW(Vec); // Vec<CIDataType*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RHOOK &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);

        if (data_type) {
            push__Vec(params, data_type);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RHOOK) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    if (params->len == 0) {
        FAILED("expected at least one generic param");
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_RHOOK:
            next_token__CIParser(self);

            break;
        case CI_TOKEN_KIND_EOF:
            FAILED("unexpected EOF");
        default:
            UNREACHABLE("expected `]` or `EOF`");
    }

    return params ? NEW(CIGenericParams, params) : NULL;
}

CIExpr *
parse_function_call__CIParser(CIParser *self,
                              Rc *identifier,
                              CIGenericParams *generic_params)
{
    next_token__CIParser(self); // skip `(`

    Vec *params = NEW(Vec); // Vec<CIExpr*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIExpr *param = parse_expr__CIParser(self);

        if (param) {
            push__Vec(params, param);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RPAREN) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    if (is__CIBuiltinFunction(GET_PTR_RC(String, identifier))) {
        return NEW_VARIANT(
          CIExpr,
          function_call_builtin,
          NEW(CIExprFunctionCallBuiltin,
              get_id__CIBuiltinFunction(GET_PTR_RC(String, identifier)),
              params));
    }

    return NEW_VARIANT(
      CIExpr,
      function_call,
      NEW(CIExprFunctionCall, identifier, params, generic_params));
}

CIExpr *
parse_literal_expr__CIParser(CIParser *self)
{
    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_TRUE:
            return NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, true));
        case CI_TOKEN_KIND_KEYWORD_FALSE:
            return NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, false));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN: {
            String *int_s = NULL;
            int base = 10;

            switch (self->previous_token->kind) {
                case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
                    int_s = self->previous_token->literal_constant_int.value;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
                    int_s = self->previous_token->literal_constant_octal.value;
                    base = 8;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
                    int_s = self->previous_token->literal_constant_hex.value;
                    base = 16;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
                    int_s = self->previous_token->literal_constant_bin.value;
                    base = 2;

                    break;
                default:
                    UNREACHABLE("this kind of integer is not expected");
            }

            Optional *res_op = atoi_safe__Isize(int_s->buffer, base);

            if (is_none__Optional(res_op)) {
                FREE(Optional, res_op);

                FAILED("bad integer");
            }

            Isize res = (Isize)(Uptr)get__Optional(res_op);

            FREE(Optional, res_op);

            return NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, signed_int, res));
        }
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            // TODO: Check if the float is overflow/underflow.
            // Update `base/atof.h` header.
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(
                CIExprLiteral,
                float,
                atof__Float64(
                  self->previous_token->literal_constant_float.value->buffer)));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(CIExprLiteral,
                          char,
                          self->previous_token->literal_constant_character));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(CIExprLiteral,
                          string,
                          self->previous_token->literal_constant_string));
        default:
            UNREACHABLE("unexpected token");
    }
}

CIExpr *
parse_struct_call__CIParser(CIParser *self)
{
    Vec *fields = NEW(Vec);

    next_token__CIParser(self); // skip `{`

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        Vec *path = NULL; // Vec<Rc<String*>*>*?

        if (self->current_token->kind == CI_TOKEN_KIND_DOT) {
            path = NEW(Vec);

            while (self->current_token->kind == CI_TOKEN_KIND_DOT) {
                next_token__CIParser(self);

                if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                    push__Vec(path, ref__Rc(self->previous_token->identifier));
                }
            }

            expect__CIParser(self, CI_TOKEN_KIND_EQ, true);
        }

        CIExpr *value = parse_expr__CIParser(self);

        if (self->current_token->kind != CI_TOKEN_KIND_RBRACE) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }

        if (!value) {
            FREE(Vec, path);

            continue;
        }

        push__Vec(fields, NEW(CIExprStructFieldCall, path, value));
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return NEW_VARIANT(CIExpr, struct_call, NEW(CIExprStructCall, fields));
}

CIExpr *
parse_array__CIParser(CIParser *self)
{
    next_token__CIParser(self); // skip `{`

    Vec *array = NEW(Vec);

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIExpr *expr = parse_expr__CIParser(self);

        if (expr) {
            push__Vec(array, expr);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RBRACE) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return NEW_VARIANT(CIExpr, array, NEW(CIExprArray, array));
}

CIExpr *
parse_primary_expr__CIParser(CIParser *self)
{
    if (data_type_as_expression) {
        CIDataType *data_type = data_type_as_expression;

        data_type_as_expression = NULL;

        return NEW_VARIANT(CIExpr, data_type, data_type);
    } else if (is_data_type__CIParser(self)) {
        CIDataType *data_type = parse_data_type__CIParser(self);

        return NEW_VARIANT(CIExpr, data_type, data_type);
    }

    next_token__CIParser(self);

    CIExpr *res = NULL;

    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_ALIGNOF: {
            CIExpr *alignof_expr = parse_expr__CIParser(self);

            if (alignof_expr) {
                res = NEW_VARIANT(CIExpr, alignof, alignof_expr);
                break;
            }

            return NULL;
        }
        case CI_TOKEN_KIND_LPAREN: {
            if (is_data_type__CIParser(self)) {
                CIDataType *data_type = parse_data_type__CIParser(self);
                Rc *name = NULL; // Rc<String*>*?

                parse_variable_name_and_data_type__CIParser(
                  self, &data_type, &name, false);

                if (name) {
                    FAILED("name is not expected here");
                }

                expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

                ENABLE_ALLOW_INITIALIZATION();

                CIExpr *expr = parse_expr__CIParser(self);

                DISABLE_ALLOW_INITIALIZATION();

                if (!data_type || !expr) {
                    if (data_type) {
                        FREE(CIDataType, data_type);
                    } else if (expr) {
                        FREE(CIExpr, expr);
                    }

                    return NULL;
                }

                check_for_initialization_expr__CIParser(
                  (CIParser *)self, data_type, expr);

                res =
                  NEW_VARIANT(CIExpr, cast, NEW(CIExprCast, data_type, expr));

                break;
            }

            CIExpr *expr = parse_expr__CIParser(self);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

            res = NEW_VARIANT(CIExpr, grouping, expr);

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_SIZEOF: {
            bool has_open_paren = false;

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                    has_open_paren = true;

                    next_token__CIParser(self);

                    break;
                default:
                    break;
            }

            CIExpr *sizeof_expr = parse_expr__CIParser(self);

            if (has_open_paren) {
                expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);
            }

            if (sizeof_expr) {
                res = NEW_VARIANT(CIExpr, sizeof, sizeof_expr);

                break;
            }

            return NULL;
        }
        case CI_TOKEN_KIND_IDENTIFIER: {
            ASSERT(current_scope);

            Rc *identifier = self->previous_token->identifier;
            CIExprIdentifierID id = search_identifier__CIExprIdentifierID(
              GET_PTR_RC(String, identifier), self->file, current_scope);
            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self); // CIGenericParams*?

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                    res = parse_function_call__CIParser(
                      self, identifier, generic_params);

                    break;
                default:
                    res = NEW_VARIANT(CIExpr,
                                      identifier,
                                      NEW(CIExprIdentifier, identifier, id));
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_TRUE:
        case CI_TOKEN_KIND_KEYWORD_FALSE:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            res = parse_literal_expr__CIParser(self);

            break;
        case CI_TOKEN_KIND_BANG:
        case CI_TOKEN_KIND_AMPERSAND:
        case CI_TOKEN_KIND_MINUS:
        case CI_TOKEN_KIND_PLUS:
        case CI_TOKEN_KIND_WAVE:
        case CI_TOKEN_KIND_STAR:
        case CI_TOKEN_KIND_PLUS_PLUS:
        case CI_TOKEN_KIND_MINUS_MINUS: {
            enum CITokenKind unary_token_kind = self->previous_token->kind;
            CIExpr *expr = parse_primary_expr__CIParser(self);

            if (!expr) {
                return NULL;
            }

            switch (unary_token_kind) {
                case CI_TOKEN_KIND_BANG:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_NOT, expr));

                    break;
                case CI_TOKEN_KIND_AMPERSAND:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_REF, expr));

                    break;
                case CI_TOKEN_KIND_MINUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_NEGATIVE, expr));

                    break;
                case CI_TOKEN_KIND_PLUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_POSITIVE, expr));

                    break;
                case CI_TOKEN_KIND_WAVE:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_BIT_NOT, expr));

                    break;
                case CI_TOKEN_KIND_STAR:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_DEREFERENCE, expr));

                    break;
                case CI_TOKEN_KIND_PLUS_PLUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_PRE_INCREMENT, expr));

                    break;
                case CI_TOKEN_KIND_MINUS_MINUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_PRE_DECREMENT, expr));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_NULLPTR:
            return NEW(CIExpr, CI_EXPR_KIND_NULLPTR);
        case CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE: {
            bool has_feature = false;

            switch (self->previous_token->has_feature) {
                case CI_EXTENSIONS_HAS_FEATURE_C_ALIGNAS:
                case CI_EXTENSIONS_HAS_FEATURE_C_ALIGNOF:
                case CI_EXTENSIONS_HAS_FEATURE_C_ATOMIC:
                case CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTIONS:
                case CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTION_WITH_CONTROLLING_TYPE:
                case CI_EXTENSIONS_HAS_FEATURE_C_STATIC_ASSERT:
                case CI_EXTENSIONS_HAS_FEATURE_C_THREAD_LOCAL:
                    if (self->file->entity.result->config->standard ==
                        CI_STANDARD_11) {
                        has_feature = true;
                    }

                    break;
                case CI_EXTENSIONS_HAS_FEATURE_ADDRESS_SANITIZER:
                    if (self->file->entity.result->config->compiler.kind ==
                        CI_PROJECT_CONFIG_COMPILER_KIND_CLANG) {
                        has_feature = true;
                    }

                    break;
                case CI_EXTENSIONS_HAS_FEATURE_MODULES:
                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, has_feature));

            break;
        }
        // NOTE: Standard predefined macro cannot be redefined outside of
        // builtin file.
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__: {
            Rc *date_rc =
              NEW(Rc,
                  clone__String(
                    self->previous_token->standard_predefined_macro___date__));

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, string, date_rc));

            FREE_RC(String, date_rc);

            break;
        }
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__: {
            Rc *file_rc = NEW(
              Rc, from__String(self->file->file_input.name)); // Rc<String*>*

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, string, file_rc));

            FREE_RC(String, file_rc);

            break;
        }
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__:
            res = NEW_VARIANT(CIExpr,
                              literal,
                              NEW_VARIANT(CIExprLiteral,
                                          unsigned_int,
                                          self->previous_span.line));

            break;
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__: {
            Rc *time_rc =
              NEW(Rc,
                  clone__String(
                    self->previous_token
                      ->standard_predefined_macro___time__)); // Rc<String*>*

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, string, time_rc));

            FREE_RC(String, time_rc);

            break;
        }
        default:
            FAILED("unexpected token");
    }

    return parse_post_expr__CIParser(self, res);
}

CIExpr *
parse_binary_expr__CIParser(CIParser *self, CIExpr *expr)
{
    Vec *stack = NEW(Vec); // Vec<CIExpr* | enum CIExprBinaryKind*>*
    Usize last_precedence = to_precedence__CIExpr(expr);

    push__Vec(stack, expr);

    while (self->current_token->kind == CI_TOKEN_KIND_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_PLUS_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_MINUS_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_STAR_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_SLASH_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_PERCENTAGE_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_AMPERSAND_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_BAR_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_HAT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_PLUS ||
           self->current_token->kind == CI_TOKEN_KIND_MINUS ||
           self->current_token->kind == CI_TOKEN_KIND_STAR ||
           self->current_token->kind == CI_TOKEN_KIND_SLASH ||
           self->current_token->kind == CI_TOKEN_KIND_PERCENTAGE ||
           self->current_token->kind == CI_TOKEN_KIND_AMPERSAND ||
           self->current_token->kind == CI_TOKEN_KIND_BAR ||
           self->current_token->kind == CI_TOKEN_KIND_HAT ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT_LSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT_RSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_AMPERSAND_AMPERSAND ||
           self->current_token->kind == CI_TOKEN_KIND_BAR_BAR ||
           self->current_token->kind == CI_TOKEN_KIND_EQ_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_BANG_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT_EQ) {
        enum CIExprBinaryKind op =
          from_token__CIExprBinaryKind(self->current_token);
        Usize precedence = to_precedence__CIExprBinaryKind(op);

        next_token__CIParser(self);

        CIExpr *right = parse_primary_expr__CIParser(self);

        if (!right) {
            for (Usize i = 0; i < stack->len; i += 2) {
                FREE(CIExpr, get__Vec(stack, i));
            }

            FREE(Vec, stack);

            return NULL;
        }

        while (precedence <= last_precedence && stack->len > 1) {
            CIExpr *top_right = pop__Vec(stack);
            enum CIExprBinaryKind top_op =
              (enum CIExprBinaryKind)(Uptr)pop__Vec(stack);

            last_precedence = to_precedence__CIExprBinaryKind(top_op);

            if (last_precedence < precedence) {
                push__Vec(stack, (int *)top_op);
                push__Vec(stack, top_right);

                break;
            }

            CIExpr *top_left = pop__Vec(stack);

            push__Vec(
              stack,
              NEW_VARIANT(CIExpr,
                          binary,
                          NEW(CIExprBinary, top_op, top_left, top_right)));
        }

        push__Vec(stack, (int *)op);
        push__Vec(stack, right);

        last_precedence = precedence;
    }

    while (stack->len > 1) {
        CIExpr *rhs = pop__Vec(stack);
        enum CIExprBinaryKind op = (enum CIExprBinaryKind)(Uptr)pop__Vec(stack);
        CIExpr *lhs = pop__Vec(stack);

        push__Vec(stack,
                  NEW_VARIANT(CIExpr, binary, NEW(CIExprBinary, op, lhs, rhs)));
    }

    CIExpr *res = pop__Vec(stack);

    FREE(Vec, stack);

    return res;
}

CIExpr *
parse_post_expr__CIParser(CIParser *self, CIExpr *expr)
{
    if (!expr) {
        return NULL;
    }

loop:
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_PLUS_PLUS:
            next_token__CIParser(self);

            expr = NEW_VARIANT(
              CIExpr,
              unary,
              NEW(CIExprUnary, CI_EXPR_UNARY_KIND_POST_INCREMENT, expr));

            goto loop;
        case CI_TOKEN_KIND_MINUS_MINUS:
            next_token__CIParser(self);

            expr = NEW_VARIANT(
              CIExpr,
              unary,
              NEW(CIExprUnary, CI_EXPR_UNARY_KIND_POST_DECREMENT, expr));

            goto loop;
        // Parse array access
        case CI_TOKEN_KIND_LHOOK: {
            next_token__CIParser(self);

            CIExpr *access = parse_expr__CIParser(self);

            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);

            expr = NEW_VARIANT(
              CIExpr, array_access, NEW(CIExprArrayAccess, expr, access));

            goto loop;
        }
        case CI_TOKEN_KIND_DOT:
            next_token__CIParser(self);

            expr = NEW_VARIANT(CIExpr,
                               binary,
                               NEW(CIExprBinary,
                                   CI_EXPR_BINARY_KIND_DOT,
                                   expr,
                                   parse_primary_expr__CIParser(self)));

            goto loop;
        case CI_TOKEN_KIND_ARROW:
            next_token__CIParser(self);

            expr = NEW_VARIANT(CIExpr,
                               binary,
                               NEW(CIExprBinary,
                                   CI_EXPR_BINARY_KIND_ARROW,
                                   expr,
                                   parse_primary_expr__CIParser(self)));

            goto loop;
        default:
            return expr;
    }
}

CIExpr *
parse_expr__CIParser(CIParser *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE: {
            if (!allow_initialization) {
                FAILED("cannot declare array, struct call or union call "
                       "outside of variable initialization");
            }

            CIToken *peeked = peek_token__CIParser(self, 1);

            if (peeked && peeked->kind == CI_TOKEN_KIND_DOT) {
                return parse_struct_call__CIParser(self);
            } else {
                return parse_array__CIParser(self);
            }

            break;
        }
        default:
            break;
    }

    CIExpr *expr = parse_primary_expr__CIParser(self);

    if (!expr) {
        return NULL;
    }

    expr = parse_post_expr__CIParser(self, expr);

loop:
    switch (self->current_token->kind) {
        // Parse binary expression
        case CI_TOKEN_KIND_EQ:
        case CI_TOKEN_KIND_PLUS_EQ:
        case CI_TOKEN_KIND_MINUS_EQ:
        case CI_TOKEN_KIND_STAR_EQ:
        case CI_TOKEN_KIND_SLASH_EQ:
        case CI_TOKEN_KIND_PERCENTAGE_EQ:
        case CI_TOKEN_KIND_AMPERSAND_EQ:
        case CI_TOKEN_KIND_BAR_EQ:
        case CI_TOKEN_KIND_HAT_EQ:
        case CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ:
        case CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ:
        case CI_TOKEN_KIND_PLUS:
        case CI_TOKEN_KIND_MINUS:
        case CI_TOKEN_KIND_STAR:
        case CI_TOKEN_KIND_SLASH:
        case CI_TOKEN_KIND_PERCENTAGE:
        case CI_TOKEN_KIND_AMPERSAND:
        case CI_TOKEN_KIND_BAR:
        case CI_TOKEN_KIND_HAT:
        case CI_TOKEN_KIND_LSHIFT_LSHIFT:
        case CI_TOKEN_KIND_RSHIFT_RSHIFT:
        case CI_TOKEN_KIND_AMPERSAND_AMPERSAND:
        case CI_TOKEN_KIND_BAR_BAR:
        case CI_TOKEN_KIND_EQ_EQ:
        case CI_TOKEN_KIND_BANG_EQ:
        case CI_TOKEN_KIND_LSHIFT:
        case CI_TOKEN_KIND_RSHIFT:
        case CI_TOKEN_KIND_LSHIFT_EQ:
        case CI_TOKEN_KIND_RSHIFT_EQ:
            expr = parse_binary_expr__CIParser(self, expr);

            goto loop;
        case CI_TOKEN_KIND_INTERROGATION:
            next_token__CIParser(self); // skip `?`

            CIExpr *expr_if = parse_expr__CIParser(self);

            if (!expr_if) {
                return expr;
            }

            expect__CIParser(self, CI_TOKEN_KIND_COLON, true);

            CIExpr *expr_else = parse_expr__CIParser(self);

            if (!expr_else) {
                FREE(CIExpr, expr_if);

                return expr;
            }

            return NEW_VARIANT(
              CIExpr, ternary, NEW(CIExprTernary, expr, expr_if, expr_else));
        default:
            return parse_post_expr__CIParser(self, expr);
    }
}

CIDecl *
search_decl__CIParser(const CIParser *self,
                      const String *name,
                      CIGenericParams *called_generic_params,
                      CIDecl *(*search_decl)(const CIResultFile *self,
                                             const String *name))
{
    CIDecl *base_decl = search_decl(self->file, name);

    if (!base_decl) {
        FAILED("declaration is not found");
    }

    if (called_generic_params) {
        ASSERT(!has_generic__CIGenericParams(called_generic_params));

        String *serialized_name =
          serialize_name__CIGenericParams(called_generic_params, name);
        CIDecl *res = search_decl(self->file, serialized_name);

        FREE(String, serialized_name);

        return res;
    }

    return base_decl;
}

CIDecl *
search_enum__CIParser(const CIParser *self, const String *name)
{
    return search_decl__CIParser(self, name, NULL, &search_enum__CIResultFile);
}

CIDecl *
search_function__CIParser(const CIParser *self,
                          const String *name,
                          CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_function__CIResultFile);
}

CIDecl *
search_struct__CIParser(const CIParser *self,
                        const String *name,
                        CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_struct__CIResultFile);
}

CIDecl *
search_typedef__CIParser(const CIParser *self,
                         const String *name,
                         CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_typedef__CIResultFile);
}

CIDecl *
search_union__CIParser(const CIParser *self,
                       const String *name,
                       CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_union__CIResultFile);
}

CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self, bool in_switch)
{
    CIDeclFunctionBody *body = NULL;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    expect_many__CIParser(
      self, 2, CI_TOKEN_KIND_KEYWORD_WHILE, CI_TOKEN_KIND_LPAREN);

    CIExpr *cond = parse_expr__CIParser(self);

    if (!cond) {
        return NULL;
    }

    expect_many__CIParser(
      self, 2, CI_TOKEN_KIND_RPAREN, CI_TOKEN_KIND_SEMICOLON);

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, do_while, NEW(CIStmtDoWhile, body, cond)));
}

CIDeclFunctionItem *
parse_case__CIParser(CIParser *self)
{
    CIExpr *expr = parse_expr__CIParser(self);

    if (!expr) {
        return NULL;
    }

    expect__CIParser(self, CI_TOKEN_KIND_COLON, true);

    ENABLE_IN_LABEL();

    return NEW_VARIANT(CIDeclFunctionItem,
                       stmt,
                       NEW_VARIANT(CIStmt, case, NEW(CIStmtSwitchCase, expr)));
}

CIDeclFunctionItem *
parse_for_stmt__CIParser(CIParser *self, bool in_switch)
{
    Vec *init_clauses = NULL; // Vec<CIDeclFunctionItem*>*?
    CIExpr *expr1 = NULL;
    Vec *exprs2 = NULL; // Vec<CIExpr*>*?
    CIDeclFunctionBody *body = NULL;

    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
            break;
        default:
            init_clauses = NEW(Vec);

            UNSET_EAT_SEMICOLON();

            while (self->current_token->kind != CI_TOKEN_KIND_EOF) {
                CIDeclFunctionItem *init_clause =
                  parse_function_body_item__CIParser(self, false, false);

                if (!is_for_init_clause__CIDeclFunctionItem(init_clause)) {
                    FAILED("expected valid for init-clause");
                } else if (is_variable__CIDeclFunctionItem(init_clause) &&
                           self->file->config->standard < CI_STANDARD_99) {
                    FAILED("it is impossible to have a variable declaration in "
                           "an `init clause` before C99");
                }

                push__Vec(init_clauses, init_clause);

                switch (self->current_token->kind) {
                    case CI_TOKEN_KIND_COMMA:
                        next_token__CIParser(self);

                        continue;
                    default:
                        goto parse_init_clauses_exit;
                }
            }

        parse_init_clauses_exit:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            SET_EAT_SEMICOLON();
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
            break;
        default:
            expr1 = parse_expr__CIParser(self);
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_RPAREN:
            next_token__CIParser(self);
            break;
        default:
            exprs2 = NEW(Vec);

            do {
                CIExpr *expr = parse_expr__CIParser(self);

                if (expr) {
                    push__Vec(exprs2, expr);
                }

                if (self->current_token->kind != CI_TOKEN_KIND_RPAREN) {
                    expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
                }
            } while (self->current_token->kind != CI_TOKEN_KIND_RPAREN &&
                     self->current_token->kind != CI_TOKEN_KIND_EOF);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW_VARIANT(CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, for, NEW(CIStmtFor, body, init_clauses, expr1, exprs2)));
}

CIStmtIfBranch *
parse_if_branch__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    CIExpr *cond = parse_expr__CIParser(self);
    CIDeclFunctionBody *body = NULL;

    if (!cond) {
        FAILED("expected if condition");
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, in_loop, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, in_loop, in_switch);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW(CIStmtIfBranch, cond, body);
}

bool
has_else_if__CIParser(CIParser *self)
{
    if (self->current_token->kind == CI_TOKEN_KIND_KEYWORD_ELSE) {
        CIToken *peeked = peek_token__CIParser(self, 1);

        return peeked && peeked->kind == CI_TOKEN_KIND_KEYWORD_IF;
    }

    return false;
}

CIDeclFunctionItem *
parse_if_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    CIStmtIfBranch *if_ = parse_if_branch__CIParser(self, in_loop, in_switch);
    Vec *else_ifs = NULL;
    CIDeclFunctionBody *else_ = NULL;

    if (!if_) {
        return NULL;
    }

    for (; has_else_if__CIParser(self);) {
        jump__CIParser(self, 2);

        CIStmtIfBranch *else_if =
          parse_if_branch__CIParser(self, in_loop, in_switch);

        if (else_if) {
            if (!else_ifs) {
                else_ifs = NEW(Vec);
            }

            push__Vec(else_ifs, else_if);
        }
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            next_token__CIParser(self);

            if (expect__CIParser(self, CI_TOKEN_KIND_LBRACE, false)) {
                else_ =
                  parse_function_block__CIParser(self, in_loop, in_switch);
            } else {
                CIDeclFunctionItem *item =
                  parse_function_body_item__CIParser(self, in_loop, false);
                CIScope *scope = add_scope__CIResultFile(
                  self->file, current_scope->scope_id, true);

                else_ = NEW(CIDeclFunctionBody, scope->scope_id);

                if (item) {
                    add__CIDeclFunctionBody(else_, item);
                }
            }

            break;
        default:
            break;
    }

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, if, NEW(CIStmtIf, if_, else_ifs, else_)));
}

CIDeclFunctionItem *
parse_while_stmt__CIParser(CIParser *self, bool in_switch)
{
    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    CIExpr *cond = parse_expr__CIParser(self);
    CIDeclFunctionBody *body = NULL;

    if (!cond) {
        return NULL;
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);

            if (item) {
                CIScope *scope = add_scope__CIResultFile(
                  self->file, current_scope->scope_id, true);

                body = NEW(CIDeclFunctionBody, scope->scope_id);

                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, while, NEW(CIStmtWhile, cond, body)));
}

CIDeclFunctionItem *
parse_switch_stmt__CIParser(CIParser *self, bool in_loop)
{
    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    CIExpr *expr = parse_expr__CIParser(self);

    if (!expr) {
        return NULL;
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    CIDeclFunctionBody *body = NULL;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);

            body = parse_function_block__CIParser(self, in_loop, true);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, in_loop, true);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, switch, NEW(CIStmtSwitch, expr, body)));
}

CIDeclFunctionItem *
parse_block_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    CIDeclFunctionBody *body =
      parse_function_block__CIParser(self, in_loop, in_switch);

    return NEW_VARIANT(CIDeclFunctionItem,
                       stmt,
                       NEW_VARIANT(CIStmt, block, NEW(CIStmtBlock, body)));
}

CIDeclFunctionItem *
parse_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    next_token__CIParser(self);

    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_BREAK:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            if (in_loop || in_switch) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, break));
            } else {
                FAILED("break is not expected outside of a loop");
            }
        case CI_TOKEN_KIND_KEYWORD_CASE:
            if (in_switch) {
                return parse_case__CIParser(self);
            } else {
                FAILED("case is not expected outside of a switch");
            }
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            if (in_loop) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, continue));
            } else {
                FAILED("continue is not expected outside of a loop");
            }
        case CI_TOKEN_KIND_KEYWORD_DEFAULT:
            expect__CIParser(self, CI_TOKEN_KIND_COLON, true);

            ENABLE_IN_LABEL();

            if (in_switch) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, default));
            } else {
                FAILED("default is not expected outside of a switch");
            }
        case CI_TOKEN_KIND_KEYWORD_DO:
            return parse_do_while_stmt__CIParser(self, in_switch);
        case CI_TOKEN_KIND_KEYWORD_FOR:
            return parse_for_stmt__CIParser(self, in_switch);
        case CI_TOKEN_KIND_KEYWORD_GOTO: {
            Rc *label_identifier = NULL;

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                label_identifier = self->previous_token->identifier;
            } else {
                label_identifier = generate_name_error__CIParser();
            }

            return NEW_VARIANT(CIDeclFunctionItem,
                               stmt,
                               NEW_VARIANT(CIStmt, goto, label_identifier));
        }
        case CI_TOKEN_KIND_KEYWORD_IF:
            return parse_if_stmt__CIParser(self, in_loop, in_switch);
        case CI_TOKEN_KIND_KEYWORD_RETURN: {
            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_SEMICOLON:
                    return NEW_VARIANT(CIDeclFunctionItem,
                                       stmt,
                                       NEW_VARIANT(CIStmt, return, NULL));
                default: {
                    ENABLE_ALLOW_INITIALIZATION();

                    CIExpr *expr = parse_expr__CIParser(self);

                    DISABLE_ALLOW_INITIALIZATION();

                    if (expr) {
                        expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

                        return NEW_VARIANT(CIDeclFunctionItem,
                                           stmt,
                                           NEW_VARIANT(CIStmt, return, expr));
                    }

                    FAILED("expected expression");
                }
            }
        }
        case CI_TOKEN_KIND_KEYWORD_SWITCH:
            return parse_switch_stmt__CIParser(self, in_loop);
        case CI_TOKEN_KIND_KEYWORD_WHILE:
            return parse_while_stmt__CIParser(self, in_switch);
        case CI_TOKEN_KIND_LBRACE:
            return parse_block_stmt__CIParser(self, in_loop, in_switch);
        default:
            UNREACHABLE("not expected token");
    }
}

CIDeclFunctionItem *
parse_function_body_item__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            CIToken *peeked = peek_token__CIParser(self, 1);

            if (peeked && peeked->kind == CI_TOKEN_KIND_COLON) {
                goto parse_decl;
            }

            goto default_case;
        }
        case CI_TOKEN_KIND_KEYWORD_BREAK:
        case CI_TOKEN_KIND_KEYWORD_CASE:
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
        case CI_TOKEN_KIND_KEYWORD_DEFAULT:
        case CI_TOKEN_KIND_KEYWORD_DO:
        case CI_TOKEN_KIND_KEYWORD_FOR:
        case CI_TOKEN_KIND_KEYWORD_GOTO:
        case CI_TOKEN_KIND_KEYWORD_IF:
        case CI_TOKEN_KIND_KEYWORD_RETURN:
        case CI_TOKEN_KIND_KEYWORD_SWITCH:
        case CI_TOKEN_KIND_KEYWORD_WHILE:
        case CI_TOKEN_KIND_LBRACE: {
            DISABLE_IN_LABEL();

            return parse_stmt__CIParser(self, in_loop, in_switch);
        }
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);

            return NULL;
        default:
        default_case: {
            if (is_data_type__CIParser(self) ||
                is_storage_class__CIParser(self)) {
            parse_decl: {
                CIDecl *decl = parse_decl__CIParser(self);

                DISABLE_IN_LABEL();

                if (decl) {
                    return NEW_VARIANT(CIDeclFunctionItem, decl, decl);
                } else if (!data_type_as_expression) {
                    return NULL;
                }
            }
            }

            DISABLE_IN_LABEL();

            CIDeclFunctionItem *item =
              NEW_VARIANT(CIDeclFunctionItem, expr, parse_expr__CIParser(self));

            if (eat_semicolon) {
                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
            }

            return item;
        }
    }
}

CIDeclFunctionBody *
parse_function_body_base__CIParser(CIParser *self,
                                   bool in_loop,
                                   bool in_switch,
                                   CIScope **parent_function_scope)
{
    ASSERT(current_scope);

    // Save parent values.
    CIScope *parent_scope = current_scope;
    CIDeclFunctionBody *parent_body = current_body;

    current_scope =
      parent_function_scope
        ? *parent_function_scope
        : add_scope__CIResultFile(self->file, current_scope->scope_id, true);
    current_body = NEW(CIDeclFunctionBody, current_scope->scope_id);

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDeclFunctionItem *item =
          parse_function_body_item__CIParser(self, in_loop, in_switch);

        if (item) {
            add__CIDeclFunctionBody(current_body, item);
        }
    }

    CIDeclFunctionBody *res_body = current_body;

    // Restore parent values as current.
    current_scope = parent_scope;
    current_body = parent_body;

    DISABLE_IN_LABEL();

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return res_body;
}

CIDeclFunctionBody *
parse_function_body__CIParser(CIParser *self,
                              bool in_loop,
                              bool in_switch,
                              CIScope **parent_function_scope)
{
    return parse_function_body_base__CIParser(
      self, in_loop, in_switch, parent_function_scope);
}

CIDeclFunctionBody *
parse_function_block__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    return parse_function_body_base__CIParser(self, in_loop, in_switch, NULL);
}

CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         Rc *name,
                         CIGenericParams *generic_params,
                         Vec *attributes)
{
    CIScope *parent_function_scope =
      add_scope__CIResultFile(self->file, current_scope->scope_id, true);
    Vec *params = parse_function_params__CIParser(
      self, parent_function_scope); // Vec<CIDeclFunctionParam*>*?
    CIDecl *res = NEW_VARIANT(CIDecl,
                              function,
                              storage_class_flag,
                              true,
                              NEW(CIDeclFunction,
                                  name,
                                  return_data_type,
                                  generic_params,
                                  params,
                                  NULL,
                                  attributes));

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);

            if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                return parse_typedef__CIParser(self, res, generic_params);
            }

            add_decl_to_scope__CIParser(
              self, &res, current_scope, true, in_function_body);

            break;
        case CI_TOKEN_KIND_LBRACE:
            if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                FAILED("not expected to have a body when typedef is passed");
            } else if (is__CIBuiltinFunction(GET_PTR_RC(String, name))) {
                FAILED("cannot redefine a builtin function");
            }

            add_decl_to_scope__CIParser(
              self, &res, current_scope, true, in_function_body);

            next_token__CIParser(self);

            SET_IN_FUNCTION_BODY();

            CIDeclFunctionBody *body = parse_function_body__CIParser(
              self, false, false, &parent_function_scope);

            if (res) {
                set_function_body__CIDecl(res, body);
            } else {
                FREE(CIDeclFunctionBody, body);
            }

            UNSET_IN_FUNCTION_BODY();

            break;
        default:
            FAILED("expected `;` or `{`");
    }

    return res;
}

Vec *
parse_fields__CIParser(CIParser *self)
{
    Vec *fields = NEW(Vec); // Vec<CIDeclStructField*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        RESET_DATA_TYPE_QUALIFIER_FLAG();

        CIDataType *pre_data_type = parse_pre_data_type__CIParser(self);

    loop: {
        Rc *name = NULL;
        CIDataType *data_type = parse_post_data_type__CIParser(
          self, clone__CIDataType(pre_data_type));

        switch (data_type->kind) {
            case CI_DATA_TYPE_KIND_STRUCT:
            case CI_DATA_TYPE_KIND_UNION:
                if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, false)) {
                    name = self->previous_token->identifier;
                }

                break;
            default:
                if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                    name = self->previous_token->identifier;
                } else {
                    name = generate_name_error__CIParser();
                }
        }

        parse_variable_name_and_data_type__CIParser(
          self, &data_type, &name, false);

        Uint8 bit = 0;

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_COLON: {
                next_token__CIParser(self);

                CIExpr *bit_expr = parse_expr__CIParser(self);
                CIResolverExpr bit_expr_resolver =
                  NEW(CIResolverExpr, self, current_scope, false);
                CIExpr *resolved_bit_expr =
                  run__CIResolverExpr(&bit_expr_resolver, bit_expr);

                // TODO: Check that the bit field value does not
                // overflow/underflow.
                bit =
                  to_literal_integer_value__CIResolverExpr(resolved_bit_expr);

                if (bit < 0) {
                    FAILED("bit field cannot be negative");
                }

                FREE(CIExpr, bit_expr);
                FREE(CIExpr, resolved_bit_expr);

                break;
            }
            default:
                break;
        }

        push__Vec(fields, NEW(CIDeclStructField, name, data_type, bit));

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_COMMA:
                next_token__CIParser(self);

                goto loop;
            case CI_TOKEN_KIND_SEMICOLON:
                FREE(CIDataType, pre_data_type);

                next_token__CIParser(self);

                break;
            default:
                FAILED("expected `,` or `;`");
        }
    }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return fields;
}

Vec *
parse_struct_or_union_fields__CIParser(CIParser *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            break;
        default:
            return NULL;
    }

    return parse_fields__CIParser(self);
}

CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       Rc *name,
                       CIGenericParams *generic_params)
{
    Vec *fields = parse_struct_or_union_fields__CIParser(self);

    return NEW_VARIANT(CIDecl,
                       struct,
                       storage_class_flag,
                       fields ? false : true,
                       NEW(CIDeclStruct, name, generic_params, fields));
}

CIDecl *
parse_typedef__CIParser(CIParser *self,
                        CIDecl *aliased_decl,
                        CIGenericParams *generic_params)
{
    if ((storage_class_flag & ~CI_STORAGE_CLASS_TYPEDEF) !=
        CI_STORAGE_CLASS_NONE) {
        FAILED(
          "cannot combine other storage class specifier(s) with `typedef`");
    }

    String *decl_name = get_name__CIDecl(aliased_decl);
    CIDecl *res = NULL;

#define NEW_TYPEDEF(need_ref, dt)                                  \
    ({                                                             \
        Rc *_decl_name_rc = NEW(Rc, clone__String(decl_name));     \
        CIDecl *_res =                                             \
          NEW_VARIANT(CIDecl,                                      \
                      typedef,                                     \
                      NEW(CIDeclTypedef,                           \
                          _decl_name_rc,                           \
                          generic_params && need_ref               \
                            ? ref__CIGenericParams(generic_params) \
                            : generic_params,                      \
                          dt));                                    \
        FREE_RC(String, _decl_name_rc);                            \
        _res;                                                      \
    })

    switch (aliased_decl->kind) {
        case CI_DECL_KIND_VARIABLE:
            res = NEW_TYPEDEF(
              false, ref__CIDataType(aliased_decl->variable.data_type));

            break;
        case CI_DECL_KIND_FUNCTION:
            res = NEW_TYPEDEF(
              true,
              NEW_VARIANT(
                CIDataType,
                function,
                NEW(CIDataTypeFunction,
                    aliased_decl->function.name,
                    aliased_decl->function.params
                      ? clone_params__CIDeclFunctionParam(
                          aliased_decl->function.params)
                      : NULL,
                    ref__CIDataType(aliased_decl->function.return_data_type),
                    NULL)));

            break;
        default:
            UNREACHABLE("this kind of declaration is not exepcted");
    }

    FREE(CIDecl, aliased_decl);

    return res;

#undef NEW_TYPEDEF
}

CIDecl *
parse_union__CIParser(CIParser *self,
                      int storage_class_flag,
                      Rc *name,
                      CIGenericParams *generic_params)
{
    Vec *fields = parse_struct_or_union_fields__CIParser(self);

    return NEW_VARIANT(CIDecl,
                       union,
                       storage_class_flag,
                       fields ? false : true,
                       NEW(CIDeclUnion, name, generic_params, fields));
}

bool
is_only_initialization_compatible_data_type__CIParser(CIParser *self,
                                                      CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return true;
        default:
            return false;
    }
}

bool
is_initialization_expr__CIParser(CIParser *self, CIExpr *expr)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ARRAY:
            return true;
        case CI_EXPR_KIND_LITERAL:
            switch (expr->literal.kind) {
                case CI_EXPR_LITERAL_KIND_STRING:
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }
}

void
check_for_initialization_expr__CIParser(CIParser *self,
                                        CIDataType *data_type,
                                        CIExpr *expr)
{

    if (is_only_initialization_compatible_data_type__CIParser(self,
                                                              data_type) &&
        !is_initialization_expr__CIParser(self, expr)) {
        FAILED("need initialization expression for array data type");
    }
}

CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         Rc *name,
                         bool no_expr,
                         bool is_local)
{
    if (!name) {
        FAILED("expected name for variable");
    } else if (in_label) {
        FAILED("Don't accept variable declaration in label");
    }

    if (self->current_token->kind != CI_TOKEN_KIND_COMMA) {
        next_token__CIParser(self); // skip `=` or `;`
    }

    if (no_expr) {
        return NEW_VARIANT(
          CIDecl,
          variable,
          storage_class_flag,
          false,
          NEW(CIDeclVariable, data_type, name, NULL, is_local));
    } else if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
        FAILED("expression is not expected with typedef storage class");
    }

    ENABLE_ALLOW_INITIALIZATION();

    CIExpr *expr = parse_expr__CIParser(self);

    check_for_initialization_expr__CIParser(self, data_type, expr);

    DISABLE_ALLOW_INITIALIZATION();

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            if (eat_semicolon) {
                next_token__CIParser(self);
            }

            break;
        case CI_TOKEN_KIND_COMMA:
            break;
        default:
            FAILED("expected `;`");
    }

    return NEW_VARIANT(CIDecl,
                       variable,
                       storage_class_flag,
                       false,
                       NEW(CIDeclVariable, data_type, name, expr, is_local));
}

CIDecl *
parse_variable_list__CIParser(CIParser *self,
                              CIDecl *current_var,
                              int storage_class_flag,
                              CIDataType *data_type,
                              CIGenericParams *typedef_generic_params)
{
    // Parse list of variable.
    // first_variable, <var2>, <var3>;
    while (self->current_token->kind == CI_TOKEN_KIND_COMMA &&
           self->previous_token->kind != CI_TOKEN_KIND_SEMICOLON) {
        if (current_var) {
            if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                CIDecl *typedef_decl = parse_typedef__CIParser(
                  self, current_var, typedef_generic_params);

                // NOTE: "current_var" is not available after calling
                // "parse_typedef__CIParser"
                current_var = NULL;

                add_decl_to_scope__CIParser(
                  self, &typedef_decl, current_scope, true, in_function_body);
            } else {
                add_decl_to_scope__CIParser(
                  self, &current_var, current_scope, true, in_function_body);

                // Push to the current body of the function the current
                // variable.
                if (in_function_body) {
                    ASSERT(current_body);

                    add__CIDeclFunctionBody(
                      current_body,
                      NEW_VARIANT(CIDeclFunctionItem, decl, current_var));
                }
            }
        }

        next_token__CIParser(self); // skip `,`

        CIDataType *post_data_type =
          parse_post_data_type__CIParser(self, clone__CIDataType(data_type));
        Rc *name = NULL; // Rc<String*>*? (&)

        parse_variable_name_and_data_type__CIParser(
          self, &post_data_type, &name, true);

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_EQ:
                if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                    FAILED("not expected `=` when typedef storage class is "
                           "specified");
                }

                current_var = parse_variable__CIParser(self,
                                                       storage_class_flag,
                                                       post_data_type,
                                                       name,
                                                       false,
                                                       in_function_body);

                break;
            case CI_TOKEN_KIND_COMMA:
            case CI_TOKEN_KIND_SEMICOLON:
                current_var = parse_variable__CIParser(self,
                                                       storage_class_flag,
                                                       post_data_type,
                                                       name,
                                                       true,
                                                       in_function_body);

                break;
            default:
                FAILED("expected `=`, `*` or `;`");
        }
    }

    return HAS_TYPEDEF_STORAGE_CLASS_FLAG()
             ? parse_typedef__CIParser(
                 self, current_var, typedef_generic_params)
             : current_var;
}

CIDecl *
parse_label__CIParser(CIParser *self)
{
    if (in_function_body) {
        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_IDENTIFIER: {
                Rc *identifier = self->current_token->identifier;
                CIToken *peeked_token = peek_token__CIParser(self, 1);

                if (peeked_token && peeked_token->kind == CI_TOKEN_KIND_COLON) {
                    jump__CIParser(self, 2);

                    return NEW_VARIANT(
                      CIDecl, label, NEW(CIDeclLabel, identifier));
                }

                return NULL;
            }
            default:
                return NULL;
        }
    }

    return NULL;
}

CIDecl *
parse_decl__CIParser(CIParser *self)
{
    CIDecl *res = NULL;

    if ((res = parse_label__CIParser(self))) {
        goto exit;
    }

    Vec *attributes = NULL;

    parse_attributes__CIParser(self, &attributes);

    RESET_STORAGE_CLASS_FLAG();
    RESET_DATA_TYPE_QUALIFIER_FLAG();

    parse_storage_class_specifiers__CIParser(self, &storage_class_flag);

    CIDataType *pre_data_type = parse_pre_data_type__CIParser(self);
    CIDataType *data_type = parse_post_data_type__CIParser(self, pre_data_type);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LPAREN:
        case CI_TOKEN_KIND_IDENTIFIER: {
            Rc *name = NULL; // Rc<String*>*? (&)

            parse_variable_name_and_data_type__CIParser(
              self, &data_type, &name, false);

            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self); // CIGenericParams*?

            switch (self->current_token->kind) {
                // Parse first variable
                case CI_TOKEN_KIND_EQ:
                case CI_TOKEN_KIND_COMMA:
                case CI_TOKEN_KIND_SEMICOLON:
                    if (generic_params && !HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                        FAILED("no generic params are expected in variable "
                               "declaration");
                    }

                    res = parse_variable__CIParser(
                      self,
                      storage_class_flag,
                      data_type,
                      name,
                      self->current_token->kind == CI_TOKEN_KIND_EQ ? false
                                                                    : true,
                      in_function_body);

                    break;
                case CI_TOKEN_KIND_LPAREN:
                    res = parse_function__CIParser(self,
                                                   storage_class_flag,
                                                   data_type,
                                                   name,
                                                   generic_params,
                                                   attributes);

                    if (res && res->kind == CI_DECL_KIND_FUNCTION) {
                        return res;
                    }

                    goto exit;
                default:
                    if (!in_function_body) {
                        FAILED("expected `=`, `;`, `(`");
                    }

                    data_type_as_expression = data_type;

                    return NULL;
            }

            res = parse_variable_list__CIParser(
              self, res, storage_class_flag, pre_data_type, generic_params);

            break;
        }
        default:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            FREE(CIDataType, data_type);

            return NULL;
    }

exit:
    if (res) {
        add_decl_to_scope__CIParser(
          self, &res, current_scope, true, in_function_body);
    }

    return res;
}

CIAttribute *
parse_attribute__CIParser(CIParser *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            const String *attribute_identifier =
              GET_PTR_RC(String, self->current_token->identifier);
            const enum CIAttributeStandardKind attr_id =
              get_id__Search(attribute_identifier,
                             ci_standard_attributes,
                             (const Int32 *)ci_standard_attribute_ids,
                             CI_N_STANDARD_ATTRIBUTE);
            CIAttribute *res = NULL;

            next_token__CIParser(self);

            switch (attr_id) {
                case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
                case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD: {
                    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

                    Rc *reason = NULL; // Rc<String*>*? (&)

                    switch (self->current_token->kind) {
                        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
                            reason =
                              self->current_token->literal_constant_string;

                            break;
                        default:
                            FAILED("expected string value");
                    }

                    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

                    switch (attr_id) {
                        case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
                            res = NEW_VARIANT(CIAttribute,
                                              standard,
                                              NEW_VARIANT(CIAttributeStandard,
                                                          deprecated,
                                                          reason));

                            break;
                        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
                            res = NEW_VARIANT(CIAttribute,
                                              standard,
                                              NEW_VARIANT(CIAttributeStandard,
                                                          nodiscard,
                                                          reason));

                            break;
                        default:
                            UNREACHABLE("unknown variant");
                    }

                    break;
                }
                default:
                    res = NEW_VARIANT(
                      CIAttribute, standard, NEW(CIAttributeStandard, attr_id));
            }

            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);
            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);

            return res;
        }
        default:
            FAILED("expected attribute identifier");
    }

    return NULL;
}

void
parse_attributes__CIParser(CIParser *self, Vec **attributes)
{
    while (true) {
        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_LHOOK: {
                if (!(*attributes)) {
                    *attributes = NEW(Vec);
                }

                next_token__CIParser(self);
                expect__CIParser(self, CI_TOKEN_KIND_LHOOK, true);

                CIAttribute *attr = parse_attribute__CIParser(self);

                if (attr) {
                    push__Vec(*attributes, attr);
                }

                break;
            }
            default:
                return;
        }
    }
}

bool
parse_data_type_qualifier__CIParser(CIParser *self,
                                    int *data_type_qualifier_flag)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_CONST:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER_CONST;
            break;
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER_VOLATILE;
            break;
        case CI_TOKEN_KIND_KEYWORD_RESTRICT:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT__:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER_RESTRICT;
            break;
        case CI_TOKEN_KIND_KEYWORD__ATOMIC:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER__ATOMIC;
            break;
        case CI_TOKEN_KIND_KEYWORD__NORETURN:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER__NORETURN;
            break;
        default:
            return false;
    }

    return true;
}

void
parse_data_type_qualifiers__CIParser(CIParser *self,
                                     int *data_type_qualifier_flag)
{
    int old_data_type_qualifier_flag = *data_type_qualifier_flag;

    while (
      parse_data_type_qualifier__CIParser(self, data_type_qualifier_flag)) {
        next_token__CIParser(self);

        if (old_data_type_qualifier_flag == *data_type_qualifier_flag) {
            FAILED("warning: duplicate storage class specifier");
        }

        old_data_type_qualifier_flag = *data_type_qualifier_flag;
    }
}

bool
parse_storage_class_specifier__CIParser(CIParser *self, int *storage_class_flag)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_AUTO:
            *storage_class_flag |= CI_STORAGE_CLASS_AUTO;
            break;
        case CI_TOKEN_KIND_KEYWORD_CONSTEXPR:
            *storage_class_flag |= CI_STORAGE_CLASS_CONSTEXPR;
            break;
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
            *storage_class_flag |= CI_STORAGE_CLASS_EXTERN;
            break;
        case CI_TOKEN_KIND_KEYWORD_INLINE:
            *storage_class_flag |= CI_STORAGE_CLASS_INLINE;
            break;
        case CI_TOKEN_KIND_KEYWORD_REGISTER:
            *storage_class_flag |= CI_STORAGE_CLASS_REGISTER;
            break;
        case CI_TOKEN_KIND_KEYWORD_STATIC:
            *storage_class_flag |= CI_STORAGE_CLASS_STATIC;
            break;
        case CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL:
            *storage_class_flag |= CI_STORAGE_CLASS_THREAD_LOCAL;
            break;
        case CI_TOKEN_KIND_KEYWORD_TYPEDEF:
            *storage_class_flag |= CI_STORAGE_CLASS_TYPEDEF;
            break;
        default:
            return false;
    }

    return true;
}

void
parse_storage_class_specifiers__CIParser(CIParser *self,
                                         int *storage_class_flag)
{
    int old_storage_class_flag = *storage_class_flag;

    while (parse_storage_class_specifier__CIParser(self, storage_class_flag)) {
        next_token__CIParser(self);

        if (old_storage_class_flag == *storage_class_flag) {
            FAILED("warning: duplicate storage class specifier");
        }

        old_storage_class_flag = *storage_class_flag;
    }
}

void
run__CIParser(CIParser *self, const CIResolvedTokens *resolved_tokens)
{
    // Set the current scope.
    set_current_scope__CIParser(self);
    // Initialize the parser.
    init__CIParser(self, resolved_tokens);

    while (self->current_token->kind != CI_TOKEN_KIND_EOF) {
        parse_decl__CIParser(self);
    }

    if (*self->count_error > 0) {
        exit(1);
    }

#ifdef ENV_DEBUG
    // TODO: Print debug
#endif
}

DESTRUCTOR(CIParser, const CIParser *self)
{
    if (names_error) {
        FREE_BUFFER_RC_ITEMS(names_error->buffer, names_error->len, String);
        FREE(Vec, names_error);

        names_error = NULL;
    }
}
