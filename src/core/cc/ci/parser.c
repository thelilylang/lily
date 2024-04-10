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

// TODO: add a typecheck.

#include <base/assert.h>
#include <base/atof.h>
#include <base/atoi.h>
#include <base/format.h>

#include <core/cc/ci/diagnostic/error.h>
#include <core/cc/ci/include.h>
#include <core/cc/ci/parser.h>
#include <core/cc/ci/result.h>
#include <core/shared/diagnostic.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct CIParserContext
{
    const CIDecl *current_decl; // CIDecl*? (&)
    const CIExpr *current_expr; // CIExpr*? (&)
    const CIStmt *current_stmt; // CIStmt*? (&)
};

static inline CONSTRUCTOR(struct CIParserContext, CIParserContext);

static inline void
add_macro__CIParser(CIParser *self, CIParserMacroCall *macro_call);

static void
add_item_to_wait_for_visit_list__CIParser(CIParser *self,
                                          enum CIDeclKind kind,
                                          String *name,
                                          CIGenericParams *generic_params);

static String *
generate_name_error__CIParser();

/// @brief lhs + rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_add_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial);

/// @brief lhs - rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_sub_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial);

/// @brief lhs * rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_mul_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial);

/// @brief lhs / rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_div_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial);

/// @brief lhs % rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_mod_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial);

/// @brief lhs & rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_bit_and_expr__CIParser(CIParser *self,
                               CIExpr *lhs,
                               CIExpr *rhs,
                               bool is_partial);

/// @brief lhs | rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_bit_or_expr__CIParser(CIParser *self,
                              CIExpr *lhs,
                              CIExpr *rhs,
                              bool is_partial);

/// @brief lhs ^ rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_bit_xor_expr__CIParser(CIParser *self,
                               CIExpr *lhs,
                               CIExpr *rhs,
                               bool is_partial);

/// @brief lhs << rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_bit_lshift_expr__CIParser(CIParser *self,
                                  CIExpr *lhs,
                                  CIExpr *rhs,
                                  bool is_partial);

/// @brief lhs >> rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_bit_rshift_expr__CIParser(CIParser *self,
                                  CIExpr *lhs,
                                  CIExpr *rhs,
                                  bool is_partial);

/// @brief lhs && rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_logical_and_expr__CIParser(CIParser *self,
                                   CIExpr *lhs,
                                   CIExpr *rhs,
                                   bool is_partial);

/// @brief lhs || rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_logical_or_expr__CIParser(CIParser *self,
                                  CIExpr *lhs,
                                  CIExpr *rhs,
                                  bool is_partial);

/// @brief lhs == rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_eq_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial);

/// @brief lhs != rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_ne_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial);

/// @brief lhs < rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_lt_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial);

/// @brief lhs <= rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_le_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial);

/// @brief lhs > rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_gt_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial);

/// @brief lhs >= rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_ge_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial);

/// @brief Resolve: ++rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_pre_increment_expr__CIParser(CIParser *self,
                                     CIExpr *rhs,
                                     bool is_partial);

/// @brief Resolve: --rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_pre_decrement_expr__CIParser(CIParser *self,
                                     CIExpr *rhs,
                                     bool is_partial);

/// @brief Resolve: rhs++
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_post_increment_expr__CIParser(CIParser *self,
                                      CIExpr *rhs,
                                      bool is_partial);

/// @brief Resolve: rhs--
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_post_decrement_expr__CIParser(CIParser *self,
                                      CIExpr *rhs,
                                      bool is_partial);

/// @brief Resolve: +rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_positive_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial);

/// @brief Resolve: -rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_negative_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial);

/// @brief Resolve: ~rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_bit_not_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial);

/// @brief Resolve: !rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_not_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial);

/// @brief Resolve: *rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_dereference__CIParser(CIParser *self, CIExpr *rhs, bool is_partial);

/// @brief Resolve: &rhs
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_ref__CIParser(CIParser *self, CIExpr *rhs, bool is_partial);

/// @brief Resolve unary expresssion
static CIExpr *
resolve_unary_expr__CIParser(CIParser *self, CIExpr *expr, bool is_partial);

/// @brief Resolve (can be partially) expression at compile time.
/// @param is_partial This makes it possible to have expressions that are not
/// resolvable at compile time.
static CIExpr *
resolve_expr__CIParser(CIParser *self, CIExpr *expr, bool is_partial);

/// @brief Move to next conditional preprocessor.
static CIToken *
next_conditional_preprocessor__CIParser(CIParser *self);

/// @brief Determine if the expression is true.
static bool
check_if_resolved_expr_is_true__CIParser(CIParser *self, CIExpr *expr);

/// @brief Determine which branch of conditional preprocessor to jump in.
static CIToken *
select_conditional_preprocessor__CIParser(CIParser *self);

/// @brief Jump into token block such as macro call, conditional preprocessor,
/// etc.
static void
jump_in_token_block__CIParser(CIParser *self);

/// @brief Advance to one token on the current iterator.
static void
next_token__CIParser(CIParser *self);

/// @brief This function is used when a new iterator is pushed to the stack and
/// we want the current token to represent the first token obtained by the
/// previously added iterator.
/// @param advance Call either `current__VecIter` or `next__VecIter`.
static void
init_next_token__CIParser(CIParser *self, bool advance);

/// @brief Peek token at position + n.
static CIToken *
peek_token__CIParser(CIParser *self, Usize n);

/// @brief Check if the current token has the same kind than passed in
/// parameter.
static bool
expect__CIParser(CIParser *self, enum CITokenKind kind, bool emit_error);

/// @brief Check if the current token has the same kind than an element in the
/// list of kind.
static void
expect_with_list__CIParser(CIParser *self, Usize n, ...);

/// @brief Advance to n token(s).
// TODO: Perhaps implement it.
// static void
// jump__CIParser(CIParser *self, Usize n);

/// @brief Check if the current token can be a data type.
static bool
is_data_type__CIParser(CIParser *self);

/// @return the generated fields.
static Vec *
visit_struct_or_union__CIParser(CIParser *self,
                                const CIDecl *decl,
                                CIGenericParams *called_generic_params);

static void
visit_data_type__CIParser(CIParser *self,
                          const CIDataType *data_type,
                          CIGenericParams *called_generic_params);

static void
visit_function_decl__CIParser(CIParser *self,
                              const CIDecl *decl,
                              CIGenericParams *called_generic_params);

static void
visit_function_expr__CIParser(CIParser *self,
                              const CIExpr *expr,
                              CIGenericParams *called_generic_params);

static void
visit_function_stmt__CIParser(CIParser *self,
                              const CIStmt *stmt,
                              CIGenericParams *called_generic_params);

static void
visit_function__CIParser(CIParser *self,
                         const CIDecl *function_decl,
                         CIGenericParams *called_generic_params);

static void
generate_struct_or_union_gen__CIParser(
  CIParser *self,
  String *name,
  CIGenericParams *called_generic_params,
  CIDecl *(*search_decl)(const CIResultFile *, const String *));

static void
generate_struct_gen__CIParser(CIParser *self,
                              String *struct_name,
                              CIGenericParams *called_generic_params);

static void
generate_union_gen__CIParser(CIParser *self,
                             String *union_name,
                             CIGenericParams *called_generic_params);

/// @brief Parse data type.
static CIDataType *
parse_data_type__CIParser(CIParser *self);

/// @brief Parse enum variants.
/// @return Vec<CIDeclEnumVariant*>*
static Vec *
parse_enum_variants__CIParser(CIParser *self);

/// @brief Parse enum declaration.
static CIDecl *
parse_enum__CIParser(CIParser *self, int storage_class_flag, String *name);

/// @brief Parse function params.
/// @return Vec<CIDeclFunctionParam*>*?
static Vec *
parse_function_params__CIParser(CIParser *self);

/// @brief Parse generic params.
/// @return Vec<CIDataType*>*
static CIGenericParams *
parse_generic_params__CIParser(CIParser *self);

/// @brief Parser function call.
static CIExpr *
parse_function_call__CIParser(CIParser *self,
                              String *identifier,
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
parse_binary_expr__CIParser(CIParser *self, CIExpr *expr);

static CIDeclFunctionItem *
parse_case__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self, bool in_switch);

static CIDeclFunctionItem *
parse_for_stmt__CIParser(CIParser *self, bool in_switch);

static CIStmtIfBranch *
parse_if_branch__CIParser(CIParser *self, bool in_loop, bool in_switch);

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

/// @brief Parse function body.
/// @return Vec<CIDeclFunctionItem*>*
static Vec *
parse_function_body__CIParser(CIParser *self, bool in_loop, bool in_switch);

static CIDeclFunctionItem *
parse_function_body_item__CIParser(CIParser *self,
                                   bool in_loop,
                                   bool in_switch);

/// @brief Parse function declaration.
static CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         String *name,
                         CIGenericParams *generic_params);

/// @brief Parse struct (union) fields.
static Vec *
parse_struct_fields__CIParser(CIParser *self);

/// @brief Parse struct declaration.
static CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       String *name,
                       CIGenericParams *generic_params);

/// @brief Parse union declaration.
static CIDecl *
parse_union__CIParser(CIParser *self,
                      int storage_class_flag,
                      String *name,
                      CIGenericParams *generic_params);

/// @brief Parse variable declaration.
static CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         String *name,
                         bool is_prototype,
                         bool is_local);

/// @brief Resolve `#embed` preprocessor.
static void
resolve_preprocessor_embed__CIParser(CIParser *self,
                                     CIToken *preprocessor_embed_token);

/// @brief Resolve `#error` preprocessor.
static void
resolve_preprocessor_error__CIParser(CIParser *self,
                                     CIToken *preprocessor_error_token);

/// @brief Resolve `#include` preprocessor.
static void
resolve_preprocessor_include__CIParser(CIParser *self,
                                       CIToken *preprocessor_include_token);

/// @brief Resolve `#line` preprocessor.
static inline bool
resolve_preprocessor_line__CIParser(CIParser *self,
                                    CIToken *preprocessor_line_token);

/// @brief Resolve `#pragma` preprocessor.
static bool
resolve_preprocessor_pragma__CIParser(CIParser *self,
                                      CIToken *preprocessor_pragma_token);

/// @brief Resolve `#undef` preprocessor.
static inline void
resolve_preprocessor_undef__CIParser(CIParser *self,
                                     CIToken *preprocessor_undef_token);

/// @brief Resolve `#warning` preprocessor.
static void
resolve_preprocessor_warning__CIParser(CIParser *self,
                                       CIToken *preprocessor_warning_token);

/// @brief Resolve preprocessor (not conditional preprocessor).
/// @return Return true if a preprocessor has been resolved, otherwise return
/// false.
static bool
resolve_preprocessor__CIParser(CIParser *self);

/// @brief Parse declaration.
static CIDecl *
parse_decl__CIParser(CIParser *self, bool in_function_body);

/// @brief Add declaration to scope.
static void
add_decl_to_scope__CIParser(CIParser *self,
                            CIDecl **decl_ref,
                            bool in_function_body);

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
static Vec *names_error = NULL; // Vec<String*>*
static Usize name_error_count = 0;
// The `data_type_as_expression` variable is used to keep the `DataType`
// pointer, when in `parse_decl__CIParser`, when `in_function_body` is true is
// that the following (current) token is not a `=`, `;` or `(`. Then, in the
// `parse_expr__CIParser` function, if `data_type_as_expression` is not NULL,
// the data type is returned as an expression.
static CIDataType *data_type_as_expression = NULL;

static int storage_class_flag = CI_STORAGE_CLASS_NONE;

static struct CIParserContext ctx;

// The `in_label` variable checks whether you're in a label and emits an error
// if there's a variable declaration in a label (also in case and default
// statetement).
static bool in_label = false;

static bool parse_field = false;

static CIScope *current_scope = NULL;

#define ENABLE_IN_LABEL() in_label = true;
#define DISABLE_IN_LABEL() \
    if (in_label) {        \
        in_label = false;  \
    }

CONSTRUCTOR(struct CIParserContext, CIParserContext)
{
    return (struct CIParserContext){ .current_decl = NULL,
                                     .current_expr = NULL,
                                     .current_stmt = NULL };
}

void
add_macro__CIParser(CIParser *self, CIParserMacroCall *macro_call)
{
    ASSERT(macro_call);

    push__Stack(self->macros_call, macro_call);
}

void
add_item_to_wait_for_visit_list__CIParser(CIParser *self,
                                          enum CIDeclKind kind,
                                          String *name,
                                          CIGenericParams *generic_params)
{
    CIParserWaitForVisit *inserted_item =
      get__HashMap(self->wait_visit_list, name->buffer);

    if (inserted_item) {
        ASSERT(inserted_item->kind == kind);

        push__Vec(inserted_item->generic_params_list, generic_params);
    } else {
        insert__HashMap(self->wait_visit_list,
                        name->buffer,
                        NEW(CIParserWaitForVisit, kind, name, generic_params));
    }
}

CONSTRUCTOR(CIParserWaitForVisit *,
            CIParserWaitForVisit,
            enum CIDeclKind kind,
            String *name,
            CIGenericParams *generic_params)
{
    CIParserWaitForVisit *self = lily_malloc(sizeof(CIParserWaitForVisit));

    self->kind = kind;
    self->name = name;
    self->generic_params_list = init__Vec(1, generic_params);

    return self;
}

DESTRUCTOR(CIParserWaitForVisit, CIParserWaitForVisit *self)
{
    FREE(Vec, self->generic_params_list);
    lily_free(self);
}

CONSTRUCTOR(CIParserMacroCall *, CIParserMacroCall, Vec *params)
{
    CIParserMacroCall *self = lily_malloc(sizeof(CIParserMacroCall));

    self->params = params;

    return self;
}

CONSTRUCTOR(CIParser, CIParser, CIResultFile *file, const CIScanner *scanner)
{
    CITokensIters tokens_iters = NEW(CITokensIters);

    if (!names_error) {
        names_error = NEW(Vec);
    }

    ctx = NEW(CIParserContext);
    current_scope = file->scope_base;

    return (CIParser){ .file = file,
                       .scanner = scanner,
                       .count_error = &file->count_error,
                       .count_warning = &file->count_warning,
                       .tokens_iters = tokens_iters,
                       .macros_call =
                         NEW(Stack, CI_PARSER_MACROS_CALL_MAX_SIZE),
                       .wait_visit_list = NEW(HashMap) };
}

String *
generate_name_error__CIParser()
{
    ASSERT(names_error);

    String *name_error = format__String("%__error__{zu}", name_error_count++);

    push__Vec(names_error, name_error);

    return name_error;
}

// +, -, *, /
#define RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(op, binary_kind)                 \
    ASSERT(lhs &&rhs);                                                        \
                                                                              \
    switch (lhs->kind + rhs->kind) {                                          \
        case CI_EXPR_KIND_LITERAL + CI_EXPR_KIND_LITERAL: {                   \
            switch (lhs->literal.kind) {                                      \
                case CI_EXPR_LITERAL_KIND_BOOL:                               \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                signed_int,                                   \
                                lhs->literal.bool_ op rhs->literal.bool_));   \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                signed_int,                                   \
                                lhs->literal.bool_ op rhs->literal.char_));   \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                float,                                        \
                                lhs->literal.bool_ op rhs->literal.float_));  \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          signed_int,                         \
                                          lhs->literal.bool_ op               \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          unsigned_int,                       \
                                          lhs->literal.bool_ op               \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_CHAR:                               \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                signed_int,                                   \
                                lhs->literal.char_ op rhs->literal.bool_));   \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                signed_int,                                   \
                                lhs->literal.char_ op rhs->literal.char_));   \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                float,                                        \
                                lhs->literal.char_ op rhs->literal.float_));  \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          signed_int,                         \
                                          lhs->literal.char_ op               \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          unsigned_int,                       \
                                          lhs->literal.char_ op               \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_FLOAT:                              \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                float,                                        \
                                lhs->literal.float_ op rhs->literal.bool_));  \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                float,                                        \
                                lhs->literal.float_ op rhs->literal.char_));  \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                float,                                        \
                                lhs->literal.float_ op rhs->literal.float_)); \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          float,                              \
                                          lhs->literal.float_ op              \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          float,                              \
                                          lhs->literal.float_ op              \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:                         \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          signed_int,                         \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.bool_));             \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          signed_int,                         \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.char_));             \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          float,                              \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.float_));            \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          signed_int,                         \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          signed_int,                         \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_STRING:                             \
                    FAILED("this operation is unsure at compile-time");       \
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                       \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          unsigned_int,                       \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.bool_));             \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          unsigned_int,                       \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.char_));             \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          float,                              \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.float_));            \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          unsigned_int,                       \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          unsigned_int,                       \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
            }                                                                 \
        }                                                                     \
        default:                                                              \
            if (!is_partial) {                                                \
                FAILED(                                                       \
                  "these expressions are not resolvable at compile-time");    \
            }                                                                 \
    }                                                                         \
    return NEW_VARIANT(                                                       \
      CIExpr,                                                                 \
      binary,                                                                 \
      NEW(CIExprBinary, binary_kind, ref__CIExpr(lhs), ref__CIExpr(rhs)));

CIExpr *
resolve_add_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(+, CI_EXPR_BINARY_KIND_ADD);
}

CIExpr *
resolve_sub_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(-, CI_EXPR_BINARY_KIND_SUB);
}

CIExpr *
resolve_mul_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(*, CI_EXPR_BINARY_KIND_MUL);
}

CIExpr *
resolve_div_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(/, CI_EXPR_BINARY_KIND_DIV);
}

// &, |, ^, <<, >>
// Also %
#define RESOLVE_BASIC_BINARY_BIT_EXPR(op, binary_kind)                       \
    ASSERT(lhs &&rhs);                                                       \
                                                                             \
    switch (lhs->kind + rhs->kind) {                                         \
        case CI_EXPR_KIND_LITERAL + CI_EXPR_KIND_LITERAL: {                  \
            switch (lhs->literal.kind) {                                     \
                case CI_EXPR_LITERAL_KIND_BOOL:                              \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                signed_int,                                  \
                                lhs->literal.bool_ op rhs->literal.bool_));  \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                signed_int,                                  \
                                lhs->literal.bool_ op rhs->literal.char_));  \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          signed_int,                        \
                                          lhs->literal.bool_ op              \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          unsigned_int,                      \
                                          lhs->literal.bool_ op              \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
                case CI_EXPR_LITERAL_KIND_CHAR:                              \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                signed_int,                                  \
                                lhs->literal.char_ op rhs->literal.bool_));  \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                signed_int,                                  \
                                lhs->literal.char_ op rhs->literal.char_));  \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          signed_int,                        \
                                          lhs->literal.char_ op              \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          unsigned_int,                      \
                                          lhs->literal.char_ op              \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
                case CI_EXPR_LITERAL_KIND_FLOAT:                             \
                    FAILED("cannot perform this operation with no-castable " \
                           "to int types");                                  \
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:                        \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          signed_int,                        \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.bool_));            \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          signed_int,                        \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.char_));            \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          signed_int,                        \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          unsigned_int,                      \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
                case CI_EXPR_LITERAL_KIND_STRING:                            \
                    FAILED("this operation is unsure at compile-time");      \
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                      \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          unsigned_int,                      \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.bool_));            \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          unsigned_int,                      \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.char_));            \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          unsigned_int,                      \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          unsigned_int,                      \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
            }                                                                \
        }                                                                    \
        default:                                                             \
            if (!is_partial) {                                               \
                FAILED(                                                      \
                  "these expressions are not resolvable at compile-time");   \
            }                                                                \
    }                                                                        \
    return NEW_VARIANT(                                                      \
      CIExpr,                                                                \
      binary,                                                                \
      NEW(CIExprBinary, binary_kind, ref__CIExpr(lhs), ref__CIExpr(rhs)));

CIExpr *
resolve_mod_expr__CIParser(CIParser *self,
                           CIExpr *lhs,
                           CIExpr *rhs,
                           bool is_partial)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(%, CI_EXPR_BINARY_KIND_MOD);
}

CIExpr *
resolve_bit_and_expr__CIParser(CIParser *self,
                               CIExpr *lhs,
                               CIExpr *rhs,
                               bool is_partial)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(&, CI_EXPR_BINARY_KIND_BIT_AND);
}

CIExpr *
resolve_bit_or_expr__CIParser(CIParser *self,
                              CIExpr *lhs,
                              CIExpr *rhs,
                              bool is_partial)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(|, CI_EXPR_BINARY_KIND_BIT_OR);
}

CIExpr *
resolve_bit_xor_expr__CIParser(CIParser *self,
                               CIExpr *lhs,
                               CIExpr *rhs,
                               bool is_partial)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(^, CI_EXPR_BINARY_KIND_BIT_XOR);
}

CIExpr *
resolve_bit_lshift_expr__CIParser(CIParser *self,
                                  CIExpr *lhs,
                                  CIExpr *rhs,
                                  bool is_partial)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(<<, CI_EXPR_BINARY_KIND_BIT_LSHIFT);
}

CIExpr *
resolve_bit_rshift_expr__CIParser(CIParser *self,
                                  CIExpr *lhs,
                                  CIExpr *rhs,
                                  bool is_partial)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(>>, CI_EXPR_BINARY_KIND_BIT_RSHIFT);
}

// &&, ||
#define RESOLVE_BASIC_BINARY_LOGICAL_EXPR(op, binary_kind)                   \
    ASSERT(lhs &&rhs);                                                       \
                                                                             \
    switch (lhs->kind + rhs->kind) {                                         \
        case CI_EXPR_KIND_LITERAL + CI_EXPR_KIND_LITERAL: {                  \
            switch (lhs->literal.kind) {                                     \
                case CI_EXPR_LITERAL_KIND_BOOL:                              \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                bool,                                        \
                                lhs->literal.bool_ op rhs->literal.bool_));  \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                bool,                                        \
                                lhs->literal.bool_ op rhs->literal.char_));  \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.bool_ op              \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.bool_ op              \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
                case CI_EXPR_LITERAL_KIND_CHAR:                              \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                bool,                                        \
                                lhs->literal.char_ op rhs->literal.bool_));  \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(                                   \
                                CIExprLiteral,                               \
                                bool,                                        \
                                lhs->literal.char_ op rhs->literal.char_));  \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.char_ op              \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.char_ op              \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
                case CI_EXPR_LITERAL_KIND_FLOAT:                             \
                    FAILED("cannot perform this operation with no-castable " \
                           "to int types");                                  \
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:                        \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.bool_));            \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.char_));            \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.signed_int op         \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
                case CI_EXPR_LITERAL_KIND_STRING:                            \
                    FAILED("this operation is unsure at compile-time");      \
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                      \
                    switch (rhs->literal.kind) {                             \
                        case CI_EXPR_LITERAL_KIND_BOOL:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.bool_));            \
                        case CI_EXPR_LITERAL_KIND_CHAR:                      \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.char_));            \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                     \
                            FAILED("cannot perform this operation with "     \
                                   "no-castable to int types");              \
                        case CI_EXPR_LITERAL_KIND_STRING:                    \
                            FAILED(                                          \
                              "this operation is unsure at compile-time");   \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.signed_int));       \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:              \
                            return NEW_VARIANT(                              \
                              CIExpr,                                        \
                              literal,                                       \
                              NEW_VARIANT(CIExprLiteral,                     \
                                          bool,                              \
                                          lhs->literal.unsigned_int op       \
                                            rhs->literal.unsigned_int));     \
                        default:                                             \
                            UNREACHABLE("unknown variant");                  \
                    }                                                        \
            }                                                                \
        }                                                                    \
        default:                                                             \
            if (!is_partial) {                                               \
                FAILED(                                                      \
                  "these expressions are not resolvable at compile-time");   \
            }                                                                \
    }                                                                        \
    return NEW_VARIANT(                                                      \
      CIExpr,                                                                \
      binary,                                                                \
      NEW(CIExprBinary, binary_kind, ref__CIExpr(lhs), ref__CIExpr(rhs)));

CIExpr *
resolve_logical_and_expr__CIParser(CIParser *self,
                                   CIExpr *lhs,
                                   CIExpr *rhs,
                                   bool is_partial)
{
    RESOLVE_BASIC_BINARY_LOGICAL_EXPR(&&, CI_EXPR_BINARY_KIND_AND);
}

CIExpr *
resolve_logical_or_expr__CIParser(CIParser *self,
                                  CIExpr *lhs,
                                  CIExpr *rhs,
                                  bool is_partial)
{
    RESOLVE_BASIC_BINARY_LOGICAL_EXPR(||, CI_EXPR_BINARY_KIND_OR);
}

#define RESOLVE_BASIC_BINARY_COMPARISON_EXPR(op, binary_kind)                 \
    ASSERT(lhs &&rhs);                                                        \
                                                                              \
    switch (lhs->kind + rhs->kind) {                                          \
        case CI_EXPR_KIND_LITERAL + CI_EXPR_KIND_LITERAL: {                   \
            switch (lhs->literal.kind) {                                      \
                case CI_EXPR_LITERAL_KIND_BOOL:                               \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.bool_ op rhs->literal.bool_));   \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.bool_ op rhs->literal.char_));   \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.bool_ op rhs->literal.float_));  \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.bool_ op               \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.bool_ op               \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_CHAR:                               \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.char_ op rhs->literal.bool_));   \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.char_ op rhs->literal.char_));   \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.char_ op rhs->literal.float_));  \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.char_ op               \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.char_ op               \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_FLOAT:                              \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.float_ op rhs->literal.bool_));  \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.float_ op rhs->literal.char_));  \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(                                    \
                                CIExprLiteral,                                \
                                bool,                                         \
                                lhs->literal.float_ op rhs->literal.float_)); \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.float_ op              \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.float_ op              \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:                         \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.bool_));             \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.char_));             \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.float_));            \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.signed_int op          \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
                case CI_EXPR_LITERAL_KIND_STRING:                             \
                    FAILED("this operation is unsure at compile-time");       \
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                       \
                    switch (rhs->literal.kind) {                              \
                        case CI_EXPR_LITERAL_KIND_BOOL:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.bool_));             \
                        case CI_EXPR_LITERAL_KIND_CHAR:                       \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.char_));             \
                        case CI_EXPR_LITERAL_KIND_FLOAT:                      \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.float_));            \
                        case CI_EXPR_LITERAL_KIND_STRING:                     \
                            FAILED(                                           \
                              "this operation is unsure at compile-time");    \
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:                 \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.signed_int));        \
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:               \
                            return NEW_VARIANT(                               \
                              CIExpr,                                         \
                              literal,                                        \
                              NEW_VARIANT(CIExprLiteral,                      \
                                          bool,                               \
                                          lhs->literal.unsigned_int op        \
                                            rhs->literal.unsigned_int));      \
                        default:                                              \
                            UNREACHABLE("unknown variant");                   \
                    }                                                         \
            }                                                                 \
        }                                                                     \
        default:                                                              \
            if (!is_partial) {                                                \
                FAILED("these expressions are not resolvable at "             \
                       "compile-time");                                       \
            }                                                                 \
    }                                                                         \
    return NEW_VARIANT(                                                       \
      CIExpr,                                                                 \
      binary,                                                                 \
      NEW(CIExprBinary, binary_kind, ref__CIExpr(lhs), ref__CIExpr(rhs)));

CIExpr *
resolve_eq_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(==, CI_EXPR_BINARY_KIND_EQ);
}

CIExpr *
resolve_ne_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(!=, CI_EXPR_BINARY_KIND_NE);
}

CIExpr *
resolve_lt_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(<, CI_EXPR_BINARY_KIND_LESS);
}

CIExpr *
resolve_le_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(<=, CI_EXPR_BINARY_KIND_LESS_EQ);
}

CIExpr *
resolve_gt_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(>, CI_EXPR_BINARY_KIND_GREATER);
}

CIExpr *
resolve_ge_expr__CIParser(CIParser *self,
                          CIExpr *lhs,
                          CIExpr *rhs,
                          bool is_partial)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(>=, CI_EXPR_BINARY_KIND_GREATER_EQ);
}

CIExpr *
resolve_pre_increment_expr__CIParser(CIParser *self,
                                     CIExpr *rhs,
                                     bool is_partial)
{
    TODO("resolve ++rhs");
}

CIExpr *
resolve_pre_decrement_expr__CIParser(CIParser *self,
                                     CIExpr *rhs,
                                     bool is_partial)
{
    TODO("resolve --rhs");
}

CIExpr *
resolve_post_increment_expr__CIParser(CIParser *self,
                                      CIExpr *rhs,
                                      bool is_partial)
{
    TODO("resolve rhs++");
}

CIExpr *
resolve_post_decrement_expr__CIParser(CIParser *self,
                                      CIExpr *rhs,
                                      bool is_partial)
{
    TODO("resolve rhs--");
}

CIExpr *
resolve_positive_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial)
{
    TODO("resolve +rhs");
}

CIExpr *
resolve_negative_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial)
{
    TODO("resolve -rhs");
}

CIExpr *
resolve_bit_not_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial)
{
    TODO("resolve ~rhs");
}

CIExpr *
resolve_not_expr__CIParser(CIParser *self, CIExpr *rhs, bool is_partial)
{
    ASSERT(rhs);

    switch (rhs->kind) {
        case CI_EXPR_KIND_LITERAL:
            switch (rhs->literal.kind) {
                case CI_EXPR_LITERAL_KIND_BOOL:
                    return NEW_VARIANT(
                      CIExpr,
                      literal,
                      NEW_VARIANT(CIExprLiteral, bool, !rhs->literal.bool_));
                case CI_EXPR_LITERAL_KIND_CHAR:
                    return NEW_VARIANT(
                      CIExpr,
                      literal,
                      NEW_VARIANT(CIExprLiteral, char, !rhs->literal.char_));
                case CI_EXPR_LITERAL_KIND_FLOAT:
                    return NEW_VARIANT(
                      CIExpr,
                      literal,
                      NEW_VARIANT(CIExprLiteral, float, !rhs->literal.float_));
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:
                    return NEW_VARIANT(CIExpr,
                                       literal,
                                       NEW_VARIANT(CIExprLiteral,
                                                   signed_int,
                                                   !rhs->literal.signed_int));
                case CI_EXPR_LITERAL_KIND_STRING:
                    goto default_expr_case;
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
                    return NEW_VARIANT(CIExpr,
                                       literal,
                                       NEW_VARIANT(CIExprLiteral,
                                                   unsigned_int,
                                                   !rhs->literal.unsigned_int));
                default:
                    UNREACHABLE(
                      "this kind of literal is not expected with `!`");
            }
        default:
        default_expr_case:
            if (!is_partial) {
                FAILED("this expression are not resolvable at compile-time");
            }
    }

    return NEW_VARIANT(
      CIExpr,
      unary,
      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_NOT, ref__CIExpr(rhs)));
}

CIExpr *
resolve_dereference__CIParser(CIParser *self, CIExpr *rhs, bool is_partial)
{
    // The referencing operator doesn't seem to be resolvable at the moment.
    return NEW_VARIANT(
      CIExpr,
      unary,
      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_DEREFERENCE, ref__CIExpr(rhs)));
}

CIExpr *
resolve_ref__CIParser(CIParser *self, CIExpr *rhs, bool is_partial)
{
    TODO("resolve &rhs");
}

CIExpr *
resolve_unary_expr__CIParser(CIParser *self, CIExpr *expr, bool is_partial)
{
    CIExpr *rhs = resolve_expr__CIParser(self, expr->unary.expr, is_partial);
    CIExpr *res = NULL;

    switch (expr->unary.kind) {
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
            res = resolve_pre_increment_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            res = resolve_pre_decrement_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
            res = resolve_post_increment_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
            res = resolve_post_decrement_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_POSITIVE:
            res = resolve_positive_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_NEGATIVE:
            res = resolve_negative_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_BIT_NOT:
            res = resolve_bit_not_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_NOT:
            res = resolve_not_expr__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_DEREFERENCE:
            res = resolve_dereference__CIParser(self, rhs, is_partial);

            break;
        case CI_EXPR_UNARY_KIND_REF:
            res = resolve_ref__CIParser(self, rhs, is_partial);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    FREE(CIExpr, rhs);

    return res;
}

CIExpr *
resolve_expr__CIParser(CIParser *self, CIExpr *expr, bool is_partial)
{
    ASSERT(expr);

    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            TODO("resolve alignof");
        case CI_EXPR_KIND_BINARY: {
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
                case CI_EXPR_BINARY_KIND_DOT:
                case CI_EXPR_BINARY_KIND_ARROW:
                    if (!is_partial) {
                        FAILED("cannot resolve assigns binary expression");
                    }

                    return ref__CIExpr(expr);
                default:
                    break;
            }

            CIExpr *lhs =
              resolve_expr__CIParser(self, expr->binary.left, is_partial);
            CIExpr *rhs =
              resolve_expr__CIParser(self, expr->binary.right, is_partial);
            CIExpr *res = NULL;

            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_ADD:
                    res =
                      resolve_add_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_SUB:
                    res =
                      resolve_sub_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_MUL:
                    res =
                      resolve_mul_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_DIV:
                    res =
                      resolve_div_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_MOD:
                    res =
                      resolve_mod_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_AND:
                    res = resolve_bit_and_expr__CIParser(
                      self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_OR:
                    res =
                      resolve_bit_or_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_XOR:
                    res = resolve_bit_xor_expr__CIParser(
                      self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
                    res = resolve_bit_lshift_expr__CIParser(
                      self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_RSHIFT:
                    res = resolve_bit_rshift_expr__CIParser(
                      self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_AND:
                    res = resolve_logical_and_expr__CIParser(
                      self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_OR:
                    res = resolve_logical_or_expr__CIParser(
                      self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_EQ:
                    res = resolve_eq_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_NE:
                    res = resolve_ne_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_LESS:
                    res = resolve_lt_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_LESS_EQ:
                    res = resolve_le_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_GREATER:
                    res = resolve_gt_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                case CI_EXPR_BINARY_KIND_GREATER_EQ:
                    res = resolve_ge_expr__CIParser(self, lhs, rhs, is_partial);

                    break;
                default:
                    UNREACHABLE("unknown binary kind");
            }

            FREE(CIExpr, lhs);
            FREE(CIExpr, rhs);

            return res;
        }
        case CI_EXPR_KIND_GROUPING:
            return resolve_expr__CIParser(self, expr->grouping, is_partial);
        case CI_EXPR_KIND_IDENTIFIER:
            TODO("resolve identifier");
        case CI_EXPR_KIND_LITERAL:
            return ref__CIExpr(expr);
        case CI_EXPR_KIND_SIZEOF:
            TODO("resolve sizeof");
        case CI_EXPR_KIND_STRUCT_CALL:
            TODO("resolve struct");
        case CI_EXPR_KIND_TERNARY:
            TODO("resolve ternary");
        case CI_EXPR_KIND_UNARY:
            return resolve_unary_expr__CIParser(self, expr, is_partial);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIToken *
next_conditional_preprocessor__CIParser(CIParser *self)
{
    CITokensIter *tokens_iter = peek__Stack(self->tokens_iters.iters);

    ASSERT(tokens_iter);

    // Move onto the next conditional preprocessor.
    while (
      self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF &&
      (self->tokens_iters.previous_token = self->tokens_iters.current_token) &&
      (self->tokens_iters.current_token = next__VecIter(&tokens_iter->iter))) {
        if (is_conditional_preprocessor__CITokenKind(
              self->tokens_iters.current_token->kind)) {
            return self->tokens_iters.current_token;
        }
    }

    pop_iter__CITokensIters(&self->tokens_iters);

    return NULL;
}

bool
check_if_resolved_expr_is_true__CIParser(CIParser *self, CIExpr *expr)
{
    ASSERT(expr);

    switch (expr->kind) {
        case CI_EXPR_KIND_LITERAL:
            switch (expr->literal.kind) {
                case CI_EXPR_LITERAL_KIND_BOOL:
                    return expr->literal.bool_;
                case CI_EXPR_LITERAL_KIND_CHAR:
                    return expr->literal.char_;
                case CI_EXPR_LITERAL_KIND_FLOAT:
                    return expr->literal.float_;
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:
                    return expr->literal.signed_int;
                case CI_EXPR_LITERAL_KIND_STRING:
                    return expr->literal.string;
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
                    return expr->literal.unsigned_int;
                default:
                    UNREACHABLE("unknown variant");
            }
        default:
            return false;
    }
}

#define PREPROCESSOR_PARSE_EXPR(n)                                          \
    CIExpr *n = parse_expr__CIParser(self);                                 \
                                                                            \
    /* NOTE: The EOPC token is used to signify the end of a preprocessor    \
    condition.                                                              \
                                                                          \ \
    See: include/core/cc/ci/token.h */                                      \
    if (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOPC) {     \
        FAILED("expected only one expression");                             \
    } else {                                                                \
        pop_iter__CITokensIters(&self->tokens_iters);                       \
        init_next_token__CIParser(self, false);                             \
    }

#define SELECT_IF_CONDITIONAL_PREPROCESSOR(k)                                \
    {                                                                        \
        CIToken *preprocessor = self->tokens_iters.current_token;            \
                                                                             \
        /* Add condition to the stack. */                                    \
        add_iter__CITokensIters(                                             \
          &self->tokens_iters,                                               \
          NEW(CITokensIter, preprocessor->preprocessor_##k.cond));           \
        init_next_token__CIParser(self, true);                               \
                                                                             \
        PREPROCESSOR_PARSE_EXPR(cond);                                       \
                                                                             \
        CIExpr *resolved_cond = resolve_expr__CIParser(self, cond, false);   \
                                                                             \
        FREE(CIExpr, cond);                                                  \
                                                                             \
        bool content_is_add = add_iter__CITokensIters(                       \
          &self->tokens_iters,                                               \
          NEW(CITokensIter, preprocessor->preprocessor_##k.content));        \
                                                                             \
        if (content_is_add) {                                                \
            init_next_token__CIParser(self, false);                          \
        }                                                                    \
                                                                             \
        if (check_if_resolved_expr_is_true__CIParser(self, resolved_cond)) { \
            FREE(CIExpr, resolved_cond);                                     \
                                                                             \
            return self->tokens_iters.current_token;                         \
        }                                                                    \
                                                                             \
        FREE(CIExpr, resolved_cond);                                         \
                                                                             \
        if (content_is_add) {                                                \
            next_conditional_preprocessor__CIParser(self);                   \
        }                                                                    \
                                                                             \
        return select_conditional_preprocessor__CIParser(self);              \
    }

// The `inverse` parameter is used for `#ifndef`, `#elifndef` cases, to
// invert the condition.
#define SELECT_IFDEF_CONDITIONAL_PREPROCESSOR(k, reverse)                   \
    {                                                                       \
        const CIResultDefine *is_def = get_define__CIResultFile(            \
          self->file,                                                       \
          self->tokens_iters.current_token->preprocessor_##k.identifier);   \
                                                                            \
        bool content_is_add = add_iter__CITokensIters(                      \
          &self->tokens_iters,                                              \
          NEW(CITokensIter,                                                 \
              self->tokens_iters.current_token->preprocessor_##k.content)); \
                                                                            \
        if (is_def || reverse) {                                            \
            init_next_token__CIParser(self, false);                         \
                                                                            \
            return self->tokens_iters.current_token;                        \
        }                                                                   \
                                                                            \
        if (content_is_add) {                                               \
            next_conditional_preprocessor__CIParser(self);                  \
        }                                                                   \
                                                                            \
        return select_conditional_preprocessor__CIParser(self);             \
    }

CIToken *
select_conditional_preprocessor__CIParser(CIParser *self)
{
    if (self->tokens_iters.current_token) {
        switch (self->tokens_iters.current_token->kind) {
            case CI_TOKEN_KIND_PREPROCESSOR_IF:
                SELECT_IF_CONDITIONAL_PREPROCESSOR(if);
            case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
                SELECT_IFDEF_CONDITIONAL_PREPROCESSOR(ifdef, false);
            case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
                SELECT_IFDEF_CONDITIONAL_PREPROCESSOR(ifndef, true);
            case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
                SELECT_IF_CONDITIONAL_PREPROCESSOR(elif);
            case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
                SELECT_IFDEF_CONDITIONAL_PREPROCESSOR(elifdef, false);
            case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
                SELECT_IFDEF_CONDITIONAL_PREPROCESSOR(elifndef, true);
            case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
                add_iter__CITokensIters(&self->tokens_iters,
                                        NEW(CITokensIter,
                                            self->tokens_iters.current_token
                                              ->preprocessor_else.content));

                return self->tokens_iters.current_token;
            default:
                return NULL;
        }
    }

    return NULL;
}

void
jump_in_token_block__CIParser(CIParser *self)
{
    ASSERT(self->tokens_iters.current_token);

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_PREPROCESSOR_ENDIF:
            UNREACHABLE("#endif is not expected at this point");
        case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
            add_define__CIResultFile(
              self->file,
              NEW(CIResultDefine,
                  &self->tokens_iters.current_token->preprocessor_define));
            add_macro__CIParser(
              self,
              NEW(
                CIParserMacroCall,
                self->tokens_iters.current_token->preprocessor_define.params));

            break;
        case CI_TOKEN_KIND_MACRO_PARAM: {
            ASSERT(!empty__Stack(self->macros_call));

            CIParserMacroCall *macro_top = peek__Stack(self->macros_call);
            Usize macro_param_id =
              self->tokens_iters.current_token->macro_param;

            ASSERT(macro_param_id < macro_top->params->len);

            add_iter__CITokensIters(
              &self->tokens_iters,
              NEW(CITokensIter, get__Vec(macro_top->params, macro_param_id)));
            init_next_token__CIParser(self, true);

            break;
        }
        case CI_TOKEN_KIND_PAREN_CALL:
            TODO("jump in define call");
        case CI_TOKEN_KIND_PREPROCESSOR_IF:
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF: {
            CIToken *conditional_preprocessor =
              select_conditional_preprocessor__CIParser(self);

            if (!conditional_preprocessor &&
                has_reach_end__CITokensIters(&self->tokens_iters) &&
                self->tokens_iters.iters->len > 1) {
                pop_iter__CITokensIters(&self->tokens_iters);
            }

            init_next_token__CIParser(self, true);

            return;
        }
        default:
            break;
    }
}

void
next_token__CIParser(CIParser *self)
{
    while (true) {
        ASSERT(!empty__Stack(self->tokens_iters.iters));

        if (self->tokens_iters.current_token) {
            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_EOF:
                    return;
                case CI_TOKEN_KIND_EOPC:
                    pop_iter__CITokensIters(&self->tokens_iters);
                    continue;
                default:
                    break;
            }
        }

        CITokensIter *top = peek__Stack(self->tokens_iters.iters);

        if (top->iter.count == 0) {
            init_next_token__CIParser(self, true);
        } else {
            CIToken *next_token = next__VecIter(&top->iter);

            // If the `next_token` is `NULL`, that means we have reached the
            // end of the current iter (top). So we pop the current iter from
            // the stack and call `next_token__CITokensIters` again.
            if (next_token) {
                self->tokens_iters.previous_token =
                  self->tokens_iters.current_token;
                self->tokens_iters.current_token = next_token;
            } else {
                if (self->tokens_iters.iters->len > 1) {
                    pop_iter__CITokensIters(&self->tokens_iters);
                } else {
                    // NOTE: We assign the last token of the first push
                    // iterator, and this token should normally be EOF.
                    self->tokens_iters.previous_token =
                      self->tokens_iters.current_token;
                    self->tokens_iters.current_token =
                      last__Vec(top->iter.vec); // Should be EOF.

                    break;
                }

                continue;
            }
        }

        // Resolve preprocessor like #error, #warning, #embed, #include, ...
        // (not conditional preprocessor).
        //
        // If we know that a preprocessor has been solved (when
        // `resolve_preprocessor__CIParser(self)` == true), we want to move one
        // token forward again, to skip it.
        if (resolve_preprocessor__CIParser(self)) {
            continue;
        }

        // Jump in token block such as #if, #ifdef, macro call, ...
        jump_in_token_block__CIParser(self);

        if (is_preprocessor__CITokenKind(
              self->tokens_iters.current_token->kind)) {
            continue;
        }

        break;
    }
}

static void
init_next_token__CIParser(CIParser *self, bool advance)
{
    while (true) {
        ASSERT(!empty__Stack(self->tokens_iters.iters));

        CITokensIter *top = peek__Stack(self->tokens_iters.iters);

        if (has_reach_end__CITokensIters(&self->tokens_iters)) {
            return;
        }

        self->tokens_iters.previous_token =
          advance ? self->tokens_iters.current_token
                  : self->tokens_iters.previous_token;
        self->tokens_iters.current_token =
          advance ? next__VecIter(&top->iter) : current__VecIter(&top->iter);

        // If the `previous_token` is `NULL`, we assign the `current_token`
        // to it. Otherwise, we assign nothing because that means we keep
        // the last token of the previous iterator.
        if (!self->tokens_iters.previous_token) {
            self->tokens_iters.previous_token =
              self->tokens_iters.current_token;
        } else if (!self->tokens_iters.current_token &&
                   self->tokens_iters.iters->len > 1) {
            pop_iter__CITokensIters(&self->tokens_iters);
            continue;
        }

        break;
    }
}

CIToken *
peek_token__CIParser(CIParser *self, Usize n)
{
    CIToken *current_token = self->tokens_iters.current_token;
    Vec *iters_vec = NEW(Vec);  // Vec<CITokensIter*>*
    Vec *macros_vec = NEW(Vec); // Vec<CIParserMacro*>*

    for (Usize i = self->tokens_iters.iters->len; i--;) {
        push__Vec(iters_vec, visit__Stack(self->tokens_iters.iters, i));
    }

    CITokensIter *current_iter =
      pop__Vec(iters_vec); // CITokensIter*? (&) | CITokensIter*?

    current_iter->peek.count = current_iter->iter.count;
    current_iter->peek.in_use = true;

    for (Usize i = 0; i < n && current_iter && current_token &&
                      current_token->kind != CI_TOKEN_KIND_EOF;) {
        current_token =
          safe_get__Vec(current_iter->iter.vec, current_iter->peek.count);

        if (current_token) {
            switch (current_token->kind) {
                case CI_TOKEN_KIND_MACRO_PARAM:
                    push__Vec(iters_vec, current_iter);
                    current_iter =
                      NEW(CITokensIter, peek__Stack(self->macros_call));

                    continue;
                case CI_TOKEN_KIND_IDENTIFIER:
                    TODO("macro");
                case CI_TOKEN_KIND_PAREN_CALL:
                    TODO("paren call");
                default:
                    ++current_iter->peek.count;
                    ++i;
            }
        } else {
            if (iters_vec->len > 0) {
                // Check if check if the current iterator is in the stack.
                if (iters_vec->len + 1 > self->tokens_iters.iters->len) {
                    FREE(CITokensIter, current_iter);
                } else {
                    current_iter->peek.in_use = false;
                }

                current_iter = pop__Vec(iters_vec);

                if (!current_iter->peek.in_use) {
                    current_iter->peek.count = current_iter->iter.count;
                    current_iter->peek.in_use = true;
                } else {
                    ++current_iter->peek.count;
                }
            } else {
                break;
            }
        }
    }

    if (current_iter) {
        current_iter->peek.in_use = false;
    }

    FREE(Vec, iters_vec);
    FREE(Vec, macros_vec);

    return current_token;
}

bool
expect__CIParser(CIParser *self, enum CITokenKind kind, bool emit_error)
{
    if (self->tokens_iters.current_token->kind == kind) {
        next_token__CIParser(self);

        return true;
    }

    if (emit_error) {
        FAILED("expected: ...");
    }

    return false;
}

void
expect_with_list__CIParser(CIParser *self, Usize n, ...)
{
    ASSERT(n > 0);

    va_list vl;

    va_start(vl);

    while (n-- && !expect__CIParser(self, va_arg(vl, enum CITokenKind), false))
        ;

    va_end(vl);

    if (n == 0) {
        FAILED("expected ...");
    }
}

bool
is_data_type__CIParser(CIParser *self)
{
    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return search_typedef__CIResultFile(
              self->file, self->tokens_iters.current_token->identifier);
        case CI_TOKEN_KIND_AT: // TODO: check if the next token is an identifier
                               // (not needed for the moment)
        case CI_TOKEN_KIND_KEYWORD_BOOL:
        case CI_TOKEN_KIND_KEYWORD_CHAR:
        case CI_TOKEN_KIND_KEYWORD_CONST:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY:
        case CI_TOKEN_KIND_KEYWORD_ENUM:
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
        case CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY:
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY:
        case CI_TOKEN_KIND_KEYWORD_INT:
        case CI_TOKEN_KIND_KEYWORD_SHORT_INT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR:
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
        case CI_TOKEN_KIND_KEYWORD_UNION:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT:
        case CI_TOKEN_KIND_KEYWORD_VOID:
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
        case CI_TOKEN_KIND_KEYWORD__BOOL:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
            return true;
        default:
            return false;
    }
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
                if (!strcmp(generic_param->generic->buffer,
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

CIDataType *
substitute_data_type__CIParser(CIDataType *data_type,
                               CIGenericParams *generic_params,
                               CIGenericParams *called_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY: {
            CIDataType *subs = substitute_data_type__CIParser(
              data_type, generic_params, called_generic_params);

            if (subs) {
                switch (data_type->array.kind) {
                    case CI_DATA_TYPE_ARRAY_KIND_SIZED:
                        return NEW_VARIANT(CIDataType,
                                           array,
                                           NEW_VARIANT(CIDataTypeArray,
                                                       sized,
                                                       subs,
                                                       data_type->array.name,
                                                       data_type->array.size));
                    case CI_DATA_TYPE_ARRAY_KIND_NONE:
                        return NEW_VARIANT(CIDataType,
                                           array,
                                           NEW_VARIANT(CIDataTypeArray,
                                                       none,
                                                       subs,
                                                       data_type->array.name));
                    default:
                        UNREACHABLE("unknown variant");
                }
            }

            return ref__CIDataType(data_type);
        }
        case CI_DATA_TYPE_KIND__ATOMIC: {
#define SUBSTITUTE_DATA_TYPE_WITH_MAX_ONE_GENERIC(data_type_name)        \
    CIDataType *subs = substitute_data_type__CIParser(                   \
      data_type->data_type_name, generic_params, called_generic_params); \
                                                                         \
    if (subs) {                                                          \
        return NEW_VARIANT(CIDataType, data_type_name, subs);            \
    }                                                                    \
                                                                         \
    return ref__CIDataType(data_type);

            SUBSTITUTE_DATA_TYPE_WITH_MAX_ONE_GENERIC(_atomic);
        }
        case CI_DATA_TYPE_KIND_FUNCTION: {
            Vec *subs_params = NEW(Vec);

            for (Usize i = 0; i < data_type->function.params->len; ++i) {
                CIDataType *subs_param = substitute_data_type__CIParser(
                  get__Vec(data_type->function.params, i),
                  generic_params,
                  called_generic_params);

                if (subs_param) {
                    push__Vec(subs_params, subs_param);
                }
            }

            CIDataType *subs_return_data_type = substitute_data_type__CIParser(
              data_type->function.return_data_type,
              generic_params,
              called_generic_params);

            if (subs_return_data_type) {
                return ref__CIDataType(data_type);
            } else {
                FREE_BUFFER_ITEMS(
                  subs_params->buffer, subs_params->len, CIDataType);
                FREE(Vec, subs_params);
            }

            return NEW_VARIANT(CIDataType,
                               function,
                               NEW(CIDataTypeFunction,
                                   data_type->function.name,
                                   subs_params,
                                   subs_return_data_type));
        }
        case CI_DATA_TYPE_KIND_GENERIC: {
            CIDataType *res = substitute_generic__CIParser(
              data_type->generic, generic_params, called_generic_params);

            if (res) {
                return ref__CIDataType(res);
            }

            return NULL;
        }
        case CI_DATA_TYPE_KIND_PRE_CONST: {
            SUBSTITUTE_DATA_TYPE_WITH_MAX_ONE_GENERIC(pre_const);
        }
        case CI_DATA_TYPE_KIND_POST_CONST: {
            SUBSTITUTE_DATA_TYPE_WITH_MAX_ONE_GENERIC(post_const);
        }
        case CI_DATA_TYPE_KIND_PTR: {
            SUBSTITUTE_DATA_TYPE_WITH_MAX_ONE_GENERIC(ptr);
        }
        case CI_DATA_TYPE_KIND_STRUCT: {
#define SUBSTITUTE_GENERIC_DECL_DATA_TYPE(decl_name, decl_ty, variant)         \
    if (data_type->decl_name.generic_params || data_type->decl_name.fields) {  \
        Vec *subs_params = NULL;                                               \
                                                                               \
        if (data_type->decl_name.generic_params) {                             \
            subs_params = NEW(Vec);                                            \
                                                                               \
            for (Usize i = 0;                                                  \
                 i < data_type->decl_name.generic_params->params->len;         \
                 ++i) {                                                        \
                CIDataType *subs_param = substitute_data_type__CIParser(       \
                  get__Vec(data_type->decl_name.generic_params->params, i),    \
                  generic_params,                                              \
                  called_generic_params);                                      \
                                                                               \
                if (subs_param) {                                              \
                    push__Vec(subs_params, subs_param);                        \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        Vec *subs_fields = NULL;                                               \
                                                                               \
        if (data_type->decl_name.fields) {                                     \
            subs_fields = NEW(Vec);                                            \
                                                                               \
            for (Usize i = 0; i < data_type->decl_name.fields->len; ++i) {     \
                CIDeclStructField *field =                                     \
                  get__Vec(data_type->decl_name.fields, i);                    \
                CIDataType *subs_field_dt = substitute_data_type__CIParser(    \
                  field->data_type, generic_params, called_generic_params);    \
                                                                               \
                push__Vec(subs_fields,                                         \
                          NEW(CIDeclStructField, field->name, subs_field_dt)); \
            }                                                                  \
        }                                                                      \
                                                                               \
        return NEW_VARIANT(                                                    \
          CIDataType,                                                          \
          variant,                                                             \
          NEW(decl_ty,                                                         \
              data_type->decl_name.name,                                       \
              subs_params ? NEW(CIGenericParams, subs_params) : NULL,          \
              subs_fields));                                                   \
    }                                                                          \
                                                                               \
    return ref__CIDataType(data_type);

            SUBSTITUTE_GENERIC_DECL_DATA_TYPE(
              struct_, CIDataTypeStruct, struct);
        }
        case CI_DATA_TYPE_KIND_TYPEDEF:
            // TODO: Maybe add generic on typedef.
            return ref__CIDataType(data_type);
        case CI_DATA_TYPE_KIND_UNION: {
            SUBSTITUTE_GENERIC_DECL_DATA_TYPE(union_, CIDataTypeUnion, union);
        }
        default:
            return ref__CIDataType(data_type);
    }
}

Vec *
visit_struct_or_union__CIParser(CIParser *self,
                                const CIDecl *decl,
                                CIGenericParams *called_generic_params)
{
    if (has_generic__CIDecl(decl)) {
        const Vec *fields = get_fields__CIDecl(decl);

        if (!fields) {
            return NULL;
        }

        CIGenericParams *generic_params = get_generic_params__CIDecl(decl);
        Vec *gen_fields = NEW(Vec);

        for (Usize i = 0; i < fields->len; ++i) {
            const CIDeclStructField *field = get__Vec(fields, i);
            CIDataType *subs_data_type = substitute_data_type__CIParser(
              field->data_type, generic_params, called_generic_params);

            switch (subs_data_type->kind) {
                case CI_DATA_TYPE_KIND_STRUCT:
                    if (subs_data_type->struct_.name) {
                        generate_struct_gen__CIParser(
                          self,
                          subs_data_type->struct_.name,
                          subs_data_type->struct_.generic_params);
                    }

                    break;
                case CI_DATA_TYPE_KIND_UNION:
                    if (subs_data_type->union_.name) {
                        generate_union_gen__CIParser(
                          self,
                          subs_data_type->union_.name,
                          subs_data_type->union_.generic_params);
                    }

                    break;
                default:
                    break;
            }

            if (subs_data_type) {
                push__Vec(gen_fields,
                          NEW(CIDeclStructField, field->name, subs_data_type));
            }
        }

        return gen_fields;
    }

    return NULL;
}

void
visit_data_type__CIParser(CIParser *self,
                          const CIDataType *data_type,
                          CIGenericParams *called_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_GENERIC:
            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            break;
        case CI_DATA_TYPE_KIND_UNION:
            break;
        default:
            break;
    }
}

void
visit_function_decl__CIParser(CIParser *self,
                              const CIDecl *decl,
                              CIGenericParams *called_generic_params)
{
    switch (decl->kind) {
        case CI_DECL_KIND_VARIABLE:
            break;
        default:
            break;
    }
}

void
visit_function_expr__CIParser(CIParser *self,
                              const CIExpr *expr,
                              CIGenericParams *called_generic_params)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            visit_function_expr__CIParser(
              self, expr->alignof_, called_generic_params);

            break;
        case CI_EXPR_KIND_BINARY:
            visit_function_expr__CIParser(
              self, expr->binary.left, called_generic_params);
            visit_function_expr__CIParser(
              self, expr->binary.right, called_generic_params);

            break;
        case CI_EXPR_KIND_CAST:
            visit_function_expr__CIParser(
              self, expr->cast.expr, called_generic_params);
            visit_data_type__CIParser(
              self, expr->cast.data_type, called_generic_params);

            break;
        case CI_EXPR_KIND_DATA_TYPE:
            visit_data_type__CIParser(
              self, expr->data_type, called_generic_params);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL:
            TODO("visit function call");

            break;
        case CI_EXPR_KIND_GROUPING:
            visit_function_expr__CIParser(
              self, expr->grouping, called_generic_params);

            break;
        case CI_EXPR_KIND_IDENTIFIER:
            TODO("visit identifier");

            break;
        case CI_EXPR_KIND_LITERAL:
        case CI_EXPR_KIND_STRUCT_CALL:
            break;
        case CI_EXPR_KIND_SIZEOF:
            visit_function_expr__CIParser(
              self, expr->sizeof_, called_generic_params);

            break;
        case CI_EXPR_KIND_TERNARY:
            visit_function_expr__CIParser(
              self, expr->ternary.cond, called_generic_params);
            visit_function_expr__CIParser(
              self, expr->ternary.if_, called_generic_params);
            visit_function_expr__CIParser(
              self, expr->ternary.else_, called_generic_params);

            break;
        case CI_EXPR_KIND_UNARY:
            visit_function_expr__CIParser(
              self, expr->unary.expr, called_generic_params);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
visit_function_stmt__CIParser(CIParser *self,
                              const CIStmt *stmt,
                              CIGenericParams *called_generic_params)
{
}

void
visit_function__CIParser(CIParser *self,
                         const CIDecl *function_decl,
                         CIGenericParams *called_generic_params)
{
    for (Usize i = 0; i < function_decl->function.body->len; ++i) {
        CIDeclFunctionItem *item = get__Vec(function_decl->function.body, i);

        switch (item->kind) {
            case CI_DECL_FUNCTION_ITEM_KIND_DECL:
                visit_function_decl__CIParser(
                  self, item->decl, called_generic_params);

                break;
            case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
                visit_function_expr__CIParser(
                  self, item->expr, called_generic_params);

                break;
            case CI_DECL_FUNCTION_ITEM_KIND_STMT:
                visit_function_stmt__CIParser(
                  self, &item->stmt, called_generic_params);

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }
}

void
generate_struct_or_union_gen__CIParser(
  CIParser *self,
  String *name,
  CIGenericParams *called_generic_params,
  CIDecl *(*search_decl)(const CIResultFile *, const String *))
{
    CIDecl *decl = search_decl(self->file, name);
    bool is_struct = decl->kind == CI_DECL_KIND_STRUCT ? true : false;

    if (!decl) {
        FAILED("struct or union not found");
    }

    if (called_generic_params &&
        !is_generic_params_contains_generic__CIDecl(called_generic_params)) {
        if (decl->is_prototype) {
            add_item_to_wait_for_visit_list__CIParser(
              self,
              decl->kind,
              is_struct ? decl->struct_.name : decl->union_.name,
              called_generic_params);
        } else {
            String *serialized_called_decl_name = NULL;

            if (is_struct) {
                serialized_called_decl_name = serialize_name__CIDeclStruct(
                  &decl->struct_, called_generic_params);
            } else {
                serialized_called_decl_name = serialize_name__CIDeclUnion(
                  &decl->union_, called_generic_params);
            }

            const CIDecl *decl_gen =
              search_decl(self->file, serialized_called_decl_name);

            if (!decl_gen) {
                Vec *fields = visit_struct_or_union__CIParser(
                  self, decl, called_generic_params);

                CIDecl *gen_decl =
                  is_struct
                    ? NEW_VARIANT(CIDecl,
                                  struct_gen,
                                  decl,
                                  ref__CIGenericParams(called_generic_params),
                                  serialized_called_decl_name,
                                  fields)
                    : NEW_VARIANT(CIDecl,
                                  union_gen,
                                  decl,
                                  ref__CIGenericParams(called_generic_params),
                                  serialized_called_decl_name,
                                  fields);
                add_decl_to_scope__CIParser(self, &gen_decl, false);
            } else {
                FREE(String, serialized_called_decl_name);
            }
        }
    }
}

void
generate_struct_gen__CIParser(CIParser *self,
                              String *struct_name_ref,
                              CIGenericParams *called_generic_params)
{
    generate_struct_or_union_gen__CIParser(self,
                                           struct_name_ref,
                                           called_generic_params,
                                           &search_struct__CIResultFile);
}

void
generate_union_gen__CIParser(CIParser *self,
                             String *union_name_ref,
                             CIGenericParams *called_generic_params)
{
    generate_struct_or_union_gen__CIParser(
      self, union_name_ref, called_generic_params, &search_union__CIResultFile);
}

CIDataType *
parse_data_type__CIParser(CIParser *self)
{
    CIDataType *res = NULL;

    next_token__CIParser(self);

    switch (self->tokens_iters.previous_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            res = NEW_VARIANT(CIDataType,
                              typedef,
                              self->tokens_iters.previous_token->identifier);

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LSHIFT:
                    TODO("parse generic params");
                default:
                    break;
            }

            break;
        case CI_TOKEN_KIND_AT: {
            String *generic = NULL;

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                generic = self->tokens_iters.previous_token->identifier;
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
        case CI_TOKEN_KIND_KEYWORD_CHAR:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR);

            break;
        case CI_TOKEN_KIND_KEYWORD_CONST:
            res = NEW_VARIANT(
              CIDataType, pre_const, parse_data_type__CIParser(self));

            break;
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_DOUBLE);

            break;
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_DOUBLE__COMPLEX);

            break;
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY);

            break;
        case CI_TOKEN_KIND_KEYWORD_ENUM: {
            String *name = NULL; // String* (&)

            // enum <name> ...;
            // enum <name> { ... } ...;
            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                name = self->tokens_iters.previous_token->identifier;
            } else {
                name = generate_name_error__CIParser();
            }

            res = NEW_VARIANT(CIDataType, enum, name);

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LBRACE:
                case CI_TOKEN_KIND_SEMICOLON: {
                    CIDecl *enum_decl =
                      parse_enum__CIParser(self, storage_class_flag, name);

                    if (add_enum__CIResultFile(self->file, enum_decl)) {
                        FREE(CIDecl, enum_decl);

                        FAILED("duplicate enum declaration");
                    }

                    break;
                }
                case CI_TOKEN_KIND_IDENTIFIER:
                    if (!parse_field) {
                        FAILED("the type is incomplete");
                    }

                    break;
                default:
                    break;
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT);

            break;
        case CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT__COMPLEX);

            break;
        case CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT__IMAGINARY);

            break;
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_DOUBLE);

            break;
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX);

            break;
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY);

            break;
        case CI_TOKEN_KIND_KEYWORD_INT:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_INT);

            break;
        case CI_TOKEN_KIND_KEYWORD_SHORT_INT:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_SHORT_INT);

            break;
        case CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_SIGNED_CHAR);

            break;
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
        case CI_TOKEN_KIND_KEYWORD_UNION: {
            enum CITokenKind previous_token_kind =
              self->tokens_iters.previous_token->kind;
            String *name = NULL; // String* (&)
            CIGenericParams *generic_params = NULL;

            // struct <name><generic_params, ...> ...;
            // struct <name><generic_params, ...> { ... } ...;
            // NOTE: A union or struct can be anonymous when you parse a field.
            if (!parse_field &&
                expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                name = self->tokens_iters.previous_token->identifier;
            } else if (!parse_field) {
                name = generate_name_error__CIParser();
            }

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LSHIFT:
                    generic_params = parse_generic_params__CIParser(self);

                    break;
                default:
                    break;
            }

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

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    switch (previous_token_kind) {
                        case CI_TOKEN_KIND_KEYWORD_STRUCT:
                            generate_struct_gen__CIParser(
                              self, name, generic_params);
                            break;
                        case CI_TOKEN_KIND_KEYWORD_UNION:
                            generate_union_gen__CIParser(
                              self, name, generic_params);
                            break;
                        default:
                            UNREACHABLE("this situation is impossible");
                    }

                    if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                        expect_with_list__CIParser(self,
                                                   2,
                                                   CI_TOKEN_KIND_LBRACE,
                                                   CI_TOKEN_KIND_SEMICOLON);

                        goto parse_struct_or_union;
                    }

                    break;
                case CI_TOKEN_KIND_LBRACE:
                case CI_TOKEN_KIND_SEMICOLON: {
                parse_struct_or_union: {
                    switch (previous_token_kind) {
                        case CI_TOKEN_KIND_KEYWORD_STRUCT: {
                            CIDecl *struct_decl = parse_struct__CIParser(
                              self,
                              storage_class_flag,
                              name,
                              generic_params
                                ? ref__CIGenericParams(generic_params)
                                : NULL);

                            if (struct_decl->struct_.name &&
                                add_struct__CIResultFile(self->file,
                                                         struct_decl)) {
                                FREE(CIDecl, struct_decl);

                                FAILED("struct name is already defined");
                            }

                            if (struct_decl->struct_.fields) {
                                res->struct_.fields =
                                  clone_fields__CIDeclStructField(
                                    struct_decl->struct_.fields);
                            }

                            if (!struct_decl->struct_.name) {
                                FREE(CIDecl, struct_decl);
                            }

                            break;
                        }
                        case CI_TOKEN_KIND_KEYWORD_UNION: {
                            CIDecl *union_decl = parse_union__CIParser(
                              self,
                              storage_class_flag,
                              name,
                              generic_params
                                ? ref__CIGenericParams(generic_params)
                                : NULL);

                            if (union_decl->union_.name &&
                                add_union__CIResultFile(self->file,
                                                        union_decl)) {
                                FREE(CIDecl, union_decl);
                                FAILED("union name is already defined");
                            }

                            if (union_decl->union_.fields) {
                                res->union_.fields =
                                  clone_fields__CIDeclStructField(
                                    union_decl->union_.fields);
                            }

                            if (!union_decl->union_.name) {
                                FREE(CIDecl, union_decl);
                            }

                            break;
                        }
                        default:
                            UNREACHABLE("unknown variant");
                    }

                    break;
                }
                }
                default:
                    break;
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_CHAR);

            break;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_INT);

            break;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT);

            break;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT);

            break;
        case CI_TOKEN_KIND_KEYWORD_VOID:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);

            break;
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
            TODO("volatile");
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL128);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL32);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL64);

            break;
        default:
            FAILED("expected data type");
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_STAR:
            next_token__CIParser(self);

            res = NEW_VARIANT(CIDataType, ptr, res);

            break;
        case CI_TOKEN_KIND_LPAREN:
            TODO("function data type");
        default:
            return res;
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_CONST:
            next_token__CIParser(self);

            res = NEW_VARIANT(CIDataType, post_const, res);

            break;
        default:
            return res;
    }

    return res;
}

Vec *
parse_enum_variants__CIParser(CIParser *self)
{
    Vec *variants = NEW(Vec); // Vec<CIDeclEnumVariant*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        String *name = NULL; // String* (&)

        if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
            name = self->tokens_iters.previous_token->identifier;
        } else {
            name = generate_name_error__CIParser();
        }

        switch (self->tokens_iters.current_token->kind) {
            case CI_TOKEN_KIND_EQ:
                next_token__CIParser(self);

                TODO("parse expression");

                break;
            default:
                push__Vec(variants,
                          NEW_VARIANT(CIDeclEnumVariant, default, name));
        }

        if (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE) {
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
parse_enum__CIParser(CIParser *self, int storage_class_flag, String *name)
{
    next_token__CIParser(self); // skip `{` or `;`

    switch (self->tokens_iters.previous_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                FAILED("expected identifier");
            }

            return NEW_VARIANT(CIDecl,
                               enum,
                               storage_class_flag,
                               true,
                               NULL,
                               NEW(CIDeclEnum, name, NULL));
        case CI_TOKEN_KIND_IDENTIFIER: {
            String *typedef_name =
              self->tokens_iters.previous_token->identifier;

            if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
            } else {
                FAILED("expected `;`, identifier is not expected");
            }

            return NEW_VARIANT(CIDecl,
                               enum,
                               storage_class_flag,
                               true,
                               typedef_name,
                               NEW(CIDeclEnum, name, NULL));
        }
        case CI_TOKEN_KIND_LBRACE:
            break;
        default:
            UNREACHABLE("expected `;`, `{` or identifier");
    }

    Vec *variants =
      parse_enum_variants__CIParser(self); // Vec<CIDeclEnumVariant*>*
    String *typedef_name = NULL;           // String*? (&)

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            typedef_name = self->tokens_iters.current_token->identifier;

            if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                next_token__CIParser(self);
                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
            } else {
                FAILED("expected `;`");
            }

            break;
        default:
            break;
    }

    return NEW_VARIANT(CIDecl,
                       enum,
                       storage_class_flag,
                       false,
                       typedef_name,
                       NEW(CIDeclEnum, name, variants));
}

Vec *
parse_function_params__CIParser(CIParser *self)
{
    next_token__CIParser(self); // skip `(`

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_RPAREN:
            next_token__CIParser(self);

            return NULL;
        default:
            break;
    }

    Vec *params = NEW(Vec); // Vec<CIDeclFunctionParam*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);
        String *name = NULL; // String*? (&)

        // TODO: ... parameter
        if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, false)) {
            name = self->tokens_iters.previous_token->identifier;
        }

        push__Vec(params, NEW(CIDeclFunctionParam, name, data_type));

        if (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RPAREN) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    if (self->tokens_iters.current_token->kind == CI_TOKEN_KIND_EOF) {
        FAILED("hit EOF");
    } else {
        next_token__CIParser(self); // skip `}`
    }

    return params;
}

CIGenericParams *
parse_generic_params__CIParser(CIParser *self)
{
    next_token__CIParser(self); // skip `<`

    Vec *params = NEW(Vec); // Vec<CIDataType*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RSHIFT &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);

        if (data_type) {
            push__Vec(params, data_type);
        }

        if (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RSHIFT) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    if (params->len == 0) {
        FAILED("expected at least one generic param");
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_RSHIFT:
            next_token__CIParser(self);

            break;
        case CI_TOKEN_KIND_EOF:
            FAILED("unexpected EOF");
        default:
            UNREACHABLE("expected `>` or `EOF`");
    }

    return params ? NEW(CIGenericParams, params) : NULL;
}

CIExpr *
parse_function_call__CIParser(CIParser *self,
                              String *identifier,
                              CIGenericParams *generic_params)
{
    const CIDecl *function_decl =
      search_function__CIResultFile(self->file, identifier);

    if (!function_decl) {
        FAILED("unknown function, impossible to call unknown function");
    }

    next_token__CIParser(self); // skip `(`

    Vec *params = NEW(Vec); // Vec<CIExpr*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIExpr *param = parse_expr__CIParser(self);

        if (param) {
            push__Vec(params, param);
        }

        if (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RPAREN) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    // Generate gen function declaration
    if (generic_params &&
        !is_generic_params_contains_generic__CIDecl(generic_params)) {
        if (function_decl->is_prototype) {
            add_item_to_wait_for_visit_list__CIParser(
              self,
              function_decl->kind,
              function_decl->function.name,
              generic_params);
        } else {
            String *serialized_called_function_name =
              serialize_name__CIDeclFunction(&function_decl->function,
                                             generic_params);
            const CIDecl *function_gen = search_function__CIResultFile(
              self->file, serialized_called_function_name);

            if (!function_gen) {
                CIDataType *subs_return_data_type =
                  substitute_data_type__CIParser(
                    function_decl->function.return_data_type,
                    function_decl->function.generic_params,
                    generic_params);

                visit_function__CIParser(self, function_decl, generic_params);

                CIDecl *function_gen_decl =
                  NEW_VARIANT(CIDecl,
                              function_gen,
                              (CIDecl *)function_decl,
                              ref__CIGenericParams(generic_params),
                              serialized_called_function_name,
                              subs_return_data_type ? subs_return_data_type
                                                    : ref__CIDataType(
                                                        function_decl->function
                                                          .return_data_type) /* Return a ref data type, when the substituted data type is NULL, to avoid an optional data type in the `return_data_type` field. */);

                add_decl_to_scope__CIParser(self, &function_gen_decl, false);
            }

            identifier = serialized_called_function_name;
        }
    }

    return NEW_VARIANT(
      CIExpr,
      function_call,
      NEW(CIExprFunctionCall, identifier, params, generic_params));
}

CIExpr *
parse_literal_expr__CIParser(CIParser *self)
{
    switch (self->tokens_iters.previous_token->kind) {
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

            switch (self->tokens_iters.previous_token->kind) {
                case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
                    int_s = self->tokens_iters.previous_token
                              ->literal_constant_int.value;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
                    int_s = self->tokens_iters.previous_token
                              ->literal_constant_octal.value;
                    base = 8;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
                    int_s = self->tokens_iters.previous_token
                              ->literal_constant_hex.value;
                    base = 16;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
                    int_s = self->tokens_iters.previous_token
                              ->literal_constant_bin.value;
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
                atof__Float64(self->tokens_iters.previous_token
                                ->literal_constant_float.value->buffer)));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(
                CIExprLiteral,
                char,
                self->tokens_iters.previous_token->literal_constant_character));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(
                CIExprLiteral,
                string,
                self->tokens_iters.previous_token->literal_constant_string));
        default:
            UNREACHABLE("unexpected token");
    }
}

CIExpr *
parse_struct_call__CIParser(CIParser *self)
{
    Vec *fields = NEW(Vec);

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        Vec *path = NEW(Vec);

        while (self->tokens_iters.current_token->kind == CI_TOKEN_KIND_DOT) {
            next_token__CIParser(self);

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                push__Vec(path, self->tokens_iters.previous_token->identifier);
            }
        }

        expect__CIParser(self, CI_TOKEN_KIND_EQ, true);

        CIExpr *value = parse_expr__CIParser(self);

        if (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE) {
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
parse_primary_expr__CIParser(CIParser *self)
{
    if (data_type_as_expression) {
        CIDataType *data_type = data_type_as_expression;

        data_type_as_expression = NULL;

        return NEW_VARIANT(CIExpr, data_type, data_type);
    }

    next_token__CIParser(self);

    switch (self->tokens_iters.previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_ALIGNOF: {
            CIExpr *alignof_expr = parse_expr__CIParser(self);

            if (alignof_expr) {
                return NEW_VARIANT(CIExpr, alignof, alignof_expr);
            }

            return NULL;
        }
        case CI_TOKEN_KIND_LPAREN: {
            if (is_data_type__CIParser(self)) {
                CIDataType *data_type = parse_data_type__CIParser(self);

                expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

                CIExpr *expr = parse_expr__CIParser(self);

                if (!data_type || !expr) {
                    if (data_type) {
                        FREE(CIDataType, data_type);
                    } else if (expr) {
                        FREE(CIExpr, expr);
                    }

                    return NULL;
                }

                return NEW_VARIANT(
                  CIExpr, cast, NEW(CIExprCast, data_type, expr));
            }

            CIExpr *expr = parse_expr__CIParser(self);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

            return NEW_VARIANT(CIExpr, grouping, expr);
        }
        case CI_TOKEN_KIND_LBRACE:
            return parse_struct_call__CIParser(self);
        case CI_TOKEN_KIND_HASHTAG:
            TODO("done Stringification");
        case CI_TOKEN_KIND_KEYWORD_SIZEOF: {
            CIExpr *sizeof_expr = parse_expr__CIParser(self);

            if (sizeof_expr) {
                return NEW_VARIANT(CIExpr, sizeof, sizeof_expr);
            }

            return NULL;
        }
        case CI_TOKEN_KIND_IDENTIFIER: {
            String *identifier = self->tokens_iters.previous_token->identifier;
            CIGenericParams *generic_params = NULL; // CIGenericParams*?

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LSHIFT:
                    generic_params = parse_generic_params__CIParser(self);

                    break;
                default:
                    break;
            }

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                    return parse_function_call__CIParser(
                      self, identifier, generic_params);
                case CI_TOKEN_KIND_LBRACE:
                    TODO("parse struct call");
                case CI_TOKEN_KIND_LHOOK:
                    TODO("parse indexed array");
                default:
                    return NEW_VARIANT(CIExpr, identifier, identifier);
            }
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
            return parse_literal_expr__CIParser(self);
        case CI_TOKEN_KIND_BANG:
        case CI_TOKEN_KIND_AMPERSAND:
        case CI_TOKEN_KIND_MINUS:
        case CI_TOKEN_KIND_PLUS:
        case CI_TOKEN_KIND_WAVE:
        case CI_TOKEN_KIND_STAR:
        case CI_TOKEN_KIND_PLUS_PLUS:
        case CI_TOKEN_KIND_MINUS_MINUS: {
            enum CITokenKind unary_token_kind =
              self->tokens_iters.previous_token->kind;
            CIExpr *expr = parse_expr__CIParser(self);

            if (!expr) {
                return NULL;
            }

            CIExpr *res = NULL;

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

            return res;
        }
        case CI_TOKEN_KIND_MACRO_DEFINED: {
            const CIResultDefine *is_def = get_define__CIResultFile(
              self->file, self->tokens_iters.previous_token->macro_defined);

            if (is_def) {
                return NEW_VARIANT(
                  CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, true));
            }

            return NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, false));
        }
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC__:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_VERSION__:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_HOSTED__:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__:
            TODO("predefined macro");
        default:
            FAILED("unexpected token");
    }
}

CIExpr *
parse_binary_expr__CIParser(CIParser *self, CIExpr *expr)
{
    Vec *stack = NEW(Vec); // Vec<CIExpr* | enum CIExprBinaryKind*>*
    Usize last_precedence = to_precedence__CIExpr(expr);

    push__Vec(stack, expr);

    while (
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_PLUS_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_MINUS_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_STAR_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_SLASH_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_PERCENTAGE_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_AMPERSAND_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_BAR_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_HAT_EQ ||
      self->tokens_iters.current_token->kind ==
        CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ ||
      self->tokens_iters.current_token->kind ==
        CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_PLUS ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_MINUS ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_STAR ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_SLASH ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_PERCENTAGE ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_AMPERSAND ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_BAR ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_HAT ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_LSHIFT_LSHIFT ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_RSHIFT_RSHIFT ||
      self->tokens_iters.current_token->kind ==
        CI_TOKEN_KIND_AMPERSAND_AMPERSAND ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_BAR_BAR ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_EQ_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_BANG_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_LSHIFT ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_RSHIFT ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_LSHIFT_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_RSHIFT_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_DOT ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_ARROW) {
        enum CIExprBinaryKind op =
          from_token__CIExprBinaryKind(self->tokens_iters.current_token);
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
parse_expr__CIParser(CIParser *self)
{
    CIExpr *expr = parse_primary_expr__CIParser(self);

    if (!expr) {
        return NULL;
    }

    // Parse binary expression
    switch (self->tokens_iters.current_token->kind) {
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
        case CI_TOKEN_KIND_DOT:
        case CI_TOKEN_KIND_ARROW:
            return parse_binary_expr__CIParser(self, expr);
        case CI_TOKEN_KIND_HASHTAG_HASHTAG:
            TODO("done <id>##<id>");
        default:
            return expr;
    }
}

CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self, bool in_switch)
{
    Vec *body = NULL;

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_body__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);

            if (item) {
                body = init__Vec(1, item);
            } else {
                body = NEW(Vec);
            }
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_KEYWORD_WHILE, true);

    CIExpr *cond = parse_expr__CIParser(self);

    if (!cond) {
        return NULL;
    }

    expect_with_list__CIParser(
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
    CIDeclFunctionItem *init_clause = NULL;
    CIExpr *expr1 = NULL;
    Vec *exprs2 = NULL;
    Vec *body = NULL;

    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
            break;
        default: {
            CIDecl *decl = parse_decl__CIParser(self, true); // => variable

            if (!decl) {
                FAILED("expected variable declaration");
            }

            init_clause = NEW_VARIANT(CIDeclFunctionItem, decl, decl);

            break;
        }
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
            break;
        default:
            expr1 = parse_expr__CIParser(self);

            if (!expr1) {
                FAILED("expected condition in for loop");
            }
    }

    switch (self->tokens_iters.current_token->kind) {
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

                if (self->tokens_iters.current_token->kind !=
                    CI_TOKEN_KIND_RPAREN) {
                    expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
                }
            } while (
              self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RPAREN &&
              self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_body__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);

            if (item) {
                body = init__Vec(1, item);
            } else {
                body = NEW(Vec);
            }
        }
    }

    return NEW_VARIANT(CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, for, NEW(CIStmtFor, body, init_clause, expr1, exprs2)));
}

CIStmtIfBranch *
parse_if_branch__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    CIExpr *cond = parse_expr__CIParser(self);
    Vec *body = NULL;

    if (!cond) {
        FAILED("expected if condition");
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_body__CIParser(self, in_loop, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, in_loop, in_switch);

            if (item) {
                body = init__Vec(1, item);
            } else {
                body = NEW(Vec);
            }
        }
    }

    return NEW(CIStmtIfBranch, cond, body);
}

CIDeclFunctionItem *
parse_if_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    CIStmtIfBranch *if_ = parse_if_branch__CIParser(self, in_loop, in_switch);
    Vec *else_ifs = NULL;
    Vec *else_ = NULL;

    if (!if_) {
        return NULL;
    }

    if (self->tokens_iters.current_token->kind ==
        CI_TOKEN_KIND_KEYWORD_ELSE_IF) {
        else_ifs = NEW(Vec);
    }

    for (; self->tokens_iters.current_token->kind ==
           CI_TOKEN_KIND_KEYWORD_ELSE_IF;) {
        next_token__CIParser(self);

        CIStmtIfBranch *else_if =
          parse_if_branch__CIParser(self, in_loop, in_switch);

        if (else_if) {
            push__Vec(else_ifs, else_if);
        }
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            if (expect__CIParser(self, CI_TOKEN_KIND_LBRACE, false)) {
                else_ = parse_function_body__CIParser(self, in_loop, in_switch);
            } else {
                CIDeclFunctionItem *item =
                  parse_function_body_item__CIParser(self, in_loop, false);

                if (item) {
                    else_ = init__Vec(1, item);
                } else {
                    else_ = NEW(Vec);
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
    Vec *body = NULL;

    if (!cond) {
        return NULL;
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_body__CIParser(self, true, in_switch);

            break;
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
            break;
        default:
            FAILED("expected `{` or `;`");
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

    Vec *body = NULL;

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);

            body = parse_function_body__CIParser(self, false, true);

            break;
        default:
            FAILED("expected `{`");
    }

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, switch, NEW(CIStmtSwitch, expr, body)));
}

CIDeclFunctionItem *
parse_block_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    Vec *body = parse_function_body__CIParser(self, in_loop, in_switch);

    return NEW_VARIANT(CIDeclFunctionItem,
                       stmt,
                       NEW_VARIANT(CIStmt, block, NEW(CIStmtBlock, body)));
}

CIDeclFunctionItem *
parse_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    next_token__CIParser(self);

    switch (self->tokens_iters.previous_token->kind) {
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
            String *label_identifier = NULL;

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                label_identifier =
                  self->tokens_iters.previous_token->identifier;
            } else {
                label_identifier = generate_name_error__CIParser();
            }

            return NEW_VARIANT(CIDeclFunctionItem,
                               stmt,
                               NEW_VARIANT(CIStmt, goto, label_identifier));
        }
        case CI_TOKEN_KIND_IDENTIFIER: {
            String *identifier = self->tokens_iters.previous_token->identifier;

            ASSERT(expect__CIParser(self, CI_TOKEN_KIND_COLON, false));

            return NEW_VARIANT(
              CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, label, identifier));
        }
        case CI_TOKEN_KIND_KEYWORD_IF:
            return parse_if_stmt__CIParser(self, in_loop, in_switch);
        case CI_TOKEN_KIND_KEYWORD_RETURN: {
            CIExpr *expr = parse_expr__CIParser(self);

            if (expr) {
                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, return, expr));
            }

            FAILED("expected expression");
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
    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            CIToken *peeked = peek_token__CIParser(self, 1);

            if (peeked && peeked->kind == CI_TOKEN_KIND_COLON) {
                goto parse_stmt;
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
        case CI_TOKEN_KIND_LBRACE:
        parse_stmt: {
            DISABLE_IN_LABEL();

            return parse_stmt__CIParser(self, in_loop, in_switch);
        }
        case CI_TOKEN_KIND_SEMICOLON:
            return NULL;
        default:
        default_case: {
            if (is_data_type__CIParser(self)) {
                CIDecl *decl = parse_decl__CIParser(self, true);

                DISABLE_IN_LABEL();

                if (decl) {
                    return NEW_VARIANT(CIDeclFunctionItem, decl, decl);
                } else if (!data_type_as_expression) {
                    return NULL;
                }
            }

            DISABLE_IN_LABEL();

            return NEW_VARIANT(
              CIDeclFunctionItem, expr, parse_expr__CIParser(self));
        }
    }
}

Vec *
parse_function_body__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    ASSERT(current_scope);

    CIScope *parent_scope = current_scope;

    current_scope =
      add_scope__CIResultFile(self->file, current_scope->scope_id, true);

    Vec *body = NEW(Vec); // Vec<CIDelcFunctionItem*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDeclFunctionItem *item =
          parse_function_body_item__CIParser(self, in_loop, in_switch);

        if (item) {
            push__Vec(body, item);
        }
    }

    current_scope = parent_scope;

    DISABLE_IN_LABEL();

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return body;
}

CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         String *name,
                         CIGenericParams *generic_params)
{
    Vec *params =
      parse_function_params__CIParser(self); // Vec<CIDeclFunctionParam*>*?

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);

            return NEW_VARIANT(CIDecl,
                               function,
                               storage_class_flag,
                               true,
                               NEW(CIDeclFunction,
                                   name,
                                   return_data_type,
                                   generic_params,
                                   params,
                                   NULL));
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);

            return NEW_VARIANT(
              CIDecl,
              function,
              storage_class_flag,
              false,
              NEW(CIDeclFunction,
                  name,
                  return_data_type,
                  generic_params,
                  params,
                  parse_function_body__CIParser(self, false, false)));
        default:
            FAILED("expected `;` or `{`");
    }
}

Vec *
parse_struct_fields__CIParser(CIParser *self)
{
    Vec *fields = NEW(Vec); // Vec<CIDeclStructField*>*
    bool old_parse_field = parse_field;

    parse_field = true;

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);
        String *name = NULL;

        switch (data_type->kind) {
            case CI_DATA_TYPE_KIND_STRUCT:
            case CI_DATA_TYPE_KIND_UNION:
                if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, false)) {
                    name = self->tokens_iters.previous_token->identifier;
                }

                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

                break;
            default:
                if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                    name = self->tokens_iters.previous_token->identifier;
                } else {
                    name = generate_name_error__CIParser();
                }

                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
        }

        // TODO: parse bits set

        push__Vec(fields, NEW(CIDeclStructField, name, data_type));
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    parse_field = old_parse_field;

    return fields;
}

CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       String *name,
                       CIGenericParams *generic_params)
{
    next_token__CIParser(self); // skip `{` or `;`

    switch (self->tokens_iters.previous_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                FAILED("expected identfiier");
            }

            return NEW_VARIANT(CIDecl,
                               struct,
                               storage_class_flag,
                               true,
                               NULL,
                               NEW(CIDeclStruct, name, generic_params, NULL));
        case CI_TOKEN_KIND_IDENTIFIER: {
            String *typedef_name =
              self->tokens_iters.previous_token->identifier;

            if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
            } else {
                FAILED("expected `;`, identifier is not expected");
            }

            return NEW_VARIANT(CIDecl,
                               struct,
                               storage_class_flag,
                               true,
                               typedef_name,
                               NEW(CIDeclStruct, name, generic_params, NULL));
        }
        case CI_TOKEN_KIND_LBRACE:
            break;
        default:
            UNREACHABLE("expected `;`, `{` or identifier");
    }

    Vec *fields =
      parse_struct_fields__CIParser(self); // Vec<CIDeclStructField*>*
    String *typedef_name = NULL;           // String*? (&)

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                typedef_name = self->tokens_iters.current_token->identifier;
                next_token__CIParser(self);
                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
            } else if (!parse_field) {
                FAILED("expected `;`");
            }

            break;
        default:
            break;
    }

    return NEW_VARIANT(CIDecl,
                       struct,
                       storage_class_flag,
                       false,
                       typedef_name,
                       NEW(CIDeclStruct, name, generic_params, fields));
}

CIDecl *
parse_union__CIParser(CIParser *self,
                      int storage_class_flag,
                      String *name,
                      CIGenericParams *generic_params)
{
    CIDecl *decl =
      parse_struct__CIParser(self, storage_class_flag, name, generic_params);

    if (decl) {
        decl->kind = CI_DECL_KIND_UNION;

        CIDeclUnion union_ = NEW(CIDeclUnion,
                                 decl->struct_.name,
                                 decl->struct_.generic_params,
                                 decl->struct_.fields);

        decl->union_ = union_;
    }

    return decl;
}

CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         String *name,
                         bool is_prototype,
                         bool is_local)
{
    if (in_label) {
        FAILED("Don't accept variable declaration in label");
    }

    next_token__CIParser(self); // skip `=` or `;`

    if (is_prototype) {
        return NEW_VARIANT(
          CIDecl,
          variable,
          storage_class_flag,
          true,
          NEW(CIDeclVariable, data_type, name, NULL, is_local));
    }

    CIExpr *expr = parse_expr__CIParser(self);

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
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

void
resolve_preprocessor_embed__CIParser(CIParser *self,
                                     CIToken *preprocessor_embed_token)
{
    TODO("resolve #embed preprocessor");
}

void
resolve_preprocessor_error__CIParser(CIParser *self,
                                     CIToken *preprocessor_error_token)
{
    emit__Diagnostic(
      NEW_VARIANT(
        Diagnostic,
        simple_ci_error,
        self->file->scanner.base.source.file,
        &preprocessor_error_token->location,
        NEW_VARIANT(CIError,
                    preprocessor_error,
                    preprocessor_error_token->preprocessor_error->buffer),
        NULL,
        NULL,
        NULL),
      self->count_error);
}

void
resolve_preprocessor_include__CIParser(CIParser *self,
                                       CIToken *preprocessor_include_token)
{
    const Vec *include_dirs = get_include_dirs__CIInclude();

    for (Usize i = 0; i < include_dirs->len; ++i) {
        const char *include_dir = get__Vec(include_dirs, i);
        // include_dir + '/' + preprocessor_include.value
        char *full_include_path =
          format("{s}/{S}",
                 include_dir,
                 preprocessor_include_token->preprocessor_include.value);

        if (exists__File(full_include_path)) {
            if (!add_and_run__CIResult(self->file->result,
                                       full_include_path,
                                       self->file->scanner.standard)) {
                lily_free(full_include_path);
            }

            goto exit;
        } else {
            continue;
        }
    }

    FAILED("the include file is not found");

exit:
}

bool
resolve_preprocessor_line__CIParser(CIParser *self,
                                    CIToken *preprocessor_line_token)
{
    // NOTE: We cannot resolve the `#line` preprocessor in the case of a
    // transpiler.
    return false;
}

bool
resolve_preprocessor_pragma__CIParser(CIParser *self,
                                      CIToken *preprocessor_pragma_token)
{
    // NOTE: In the case of a `#pragma`, we can only solve `#pragma once`, but
    // all other #pragma cases are not really possible to solve in the case of a
    // transpiler.
    TODO("resolve #pragma preprocessor");
}

void
resolve_preprocessor_undef__CIParser(CIParser *self,
                                     CIToken *preprocessor_undef_token)
{
    undef_define__CIResultFile(self->file,
                               preprocessor_undef_token->preprocessor_undef);
}

void
resolve_preprocessor_warning__CIParser(CIParser *self,
                                       CIToken *preprocessor_warning_token)
{
    emit_warning__Diagnostic(
      NEW_VARIANT(
        Diagnostic,
        simple_ci_warning,
        self->file->scanner.base.source.file,
        &preprocessor_warning_token->location,
        NEW_VARIANT(CIWarning,
                    preprocessor_warning,
                    preprocessor_warning_token->preprocessor_warning->buffer),
        NULL,
        NULL,
        NULL),
      NULL,
      self->count_warning);
}

bool
resolve_preprocessor__CIParser(CIParser *self)
{
    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
            resolve_preprocessor_embed__CIParser(
              self, self->tokens_iters.current_token);

            break;
        case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
            resolve_preprocessor_error__CIParser(
              self, self->tokens_iters.current_token);

            break;
        case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
            resolve_preprocessor_include__CIParser(
              self, self->tokens_iters.current_token);

            break;
        case CI_TOKEN_KIND_PREPROCESSOR_LINE:
            return resolve_preprocessor_line__CIParser(
              self, self->tokens_iters.current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_PRAGMA:
            return resolve_preprocessor_pragma__CIParser(
              self, self->tokens_iters.current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
            resolve_preprocessor_undef__CIParser(
              self, self->tokens_iters.current_token);

            break;
        case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
            resolve_preprocessor_warning__CIParser(
              self, self->tokens_iters.current_token);

            break;
        default:
            return false;
    }

    return true;
}

CIDecl *
parse_decl__CIParser(CIParser *self, bool in_function_body)
{
    storage_class_flag = CI_STORAGE_CLASS_NONE;

    parse_storage_class_specifiers__CIParser(self, &storage_class_flag);

    CIDataType *data_type = parse_data_type__CIParser(self);
    CIDecl *res = NULL;

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            String *name = self->tokens_iters.current_token->identifier;
            CIGenericParams *generic_params = NULL; // CIGenericParams*?

            next_token__CIParser(self);

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LSHIFT:
                    generic_params = parse_generic_params__CIParser(self);

                    break;
                default:
                    break;
            }

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_EQ:
                    if (generic_params) {
                        goto no_generic_params_expected;
                    }

                    res = parse_variable__CIParser(self,
                                                   storage_class_flag,
                                                   data_type,
                                                   name,
                                                   false,
                                                   in_function_body);

                    break;
                case CI_TOKEN_KIND_SEMICOLON:
                    if (generic_params) {
                    no_generic_params_expected: {
                        FAILED("no generic params are expected in variable "
                               "declaration");
                    }
                    }

                    res = parse_variable__CIParser(self,
                                                   storage_class_flag,
                                                   data_type,
                                                   name,
                                                   true,
                                                   in_function_body);

                    break;
                case CI_TOKEN_KIND_LPAREN:
                    res = parse_function__CIParser(self,
                                                   storage_class_flag,
                                                   data_type,
                                                   name,
                                                   generic_params);

                    break;
                default:
                    if (!in_function_body) {
                        FAILED("expected `=`, `;`, `(`");
                    }

                    data_type_as_expression = data_type;

                    return NULL;
            }

            break;
        }
        default:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            FREE(CIDataType, data_type);

            return NULL;
    }

    if (res) {
        add_decl_to_scope__CIParser(self, &res, in_function_body);
    }

    return res;
}

void
add_decl_to_scope__CIParser(CIParser *self,
                            CIDecl **decl_ref,
                            bool in_function_body)
{
    CIDecl *decl = *decl_ref;

    ASSERT(decl);

    switch (decl->kind) {
        case CI_DECL_KIND_ENUM:
            if (add_enum__CIResultFile(self->file, decl)) {
                FAILED("enum is already defined");

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_FUNCTION:
        case CI_DECL_KIND_FUNCTION_GEN:
            if (add_function__CIResultFile(self->file, decl)) {
                FAILED("function is already defined");

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_STRUCT:
        case CI_DECL_KIND_STRUCT_GEN:
            if (add_struct__CIResultFile(self->file, decl)) {
                FAILED("struct is already defined");

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_UNION:
        case CI_DECL_KIND_UNION_GEN:
            if (add_union__CIResultFile(self->file, decl)) {
                FAILED("union is already defined");

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_VARIABLE:
            if (add_variable__CIResultFile(self->file,
                                           current_scope,
                                           in_function_body ? ref__CIDecl(decl)
                                                            : decl)) {
                FAILED("variable is already defined");

                goto free;
            }

            goto exit;
        default:
            UNREACHABLE("impossible situation");
    }

exit:
    return;

free:
    FREE(CIDecl, decl);
    *decl_ref = NULL;
}

bool
parse_storage_class_specifier__CIParser(CIParser *self, int *storage_class_flag)
{
    switch (self->tokens_iters.current_token->kind) {
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
run__CIParser(CIParser *self)
{
    // Initialize the first iterator.
    add_iter__CITokensIters(&self->tokens_iters,
                            NEW(CITokensIter, self->scanner->tokens));

    // Initialize the first token.
    next_token__CIParser(self);

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        parse_decl__CIParser(self, false);
    }

    // TODO: Resolve all items in `wait_visit_list`

#ifdef ENV_DEBUG
    // TODO: Print debug
#endif
}

DESTRUCTOR(CIParser, const CIParser *self)
{
    if (names_error) {
        FREE_BUFFER_ITEMS(names_error->buffer, names_error->len, String);
        FREE(Vec, names_error);

        names_error = NULL;
    }

    FREE(CITokensIters, &self->tokens_iters);
    FREE_STACK_ITEMS(self->macros_call, CIParserMacroCall);
    FREE(Stack, self->macros_call);
    FREE_HASHMAP_VALUES(self->wait_visit_list, CIParserWaitForVisit);
    FREE(HashMap, self->wait_visit_list);
}
