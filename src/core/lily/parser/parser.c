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

#include <base/atof.h>
#include <base/atoi.h>
#include <base/optional.h>

#include <core/lily/lily.h>
#include <core/lily/package/package.h>
#include <core/lily/parser/ast/decl.h>
#include <core/lily/parser/parser.h>

#include <stdio.h>

// Free LilyParseBlock type.
static inline DESTRUCTOR(LilyParseBlock, const LilyParseBlock *self);

// Advance to one declaration.
static void
next_decl__LilyParser(LilyParser *self);

// Advance to one token.
static void
next_token__LilyParseBlock(LilyParseBlock *self);

// Advance to n token(s).
static void
jump__LilyParseBlock(LilyParseBlock *self, Usize n);

// Peek token at position + n.
static LilyToken *
peek_token__LilyParseBlock(LilyParseBlock *self, Usize n);

// Check if it needs to parse a data type.
static inline bool
is_data_type__LilyParseBlock(LilyParseBlock *self);

// Parse data type
static LilyAstDataType *
parse_data_type__LilyParseBlock(LilyParseBlock *self);

// Parse dot access
// a.b.c
/// @param begin LilAstExpr*?
static LilyAstExpr *
parse_path_access__LilyParseBlock(LilyParseBlock *self,
                                  LilyAstExpr *begin,
                                  enum LilyAstExprAccessKind kind);

// Parse hook access
static LilyAstExpr *
parse_hook_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access);

// Parse object access
// A.@Object@Object2@Object3
/// @param access LilyAstExpr*?
static LilyAstExpr *
parse_at_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access);

// Parse access expression
static LilyAstExpr *
parse_access_expr__LilyParseBlock(LilyParseBlock *self);

static LilyAstExpr *
parse_only_access_expr__LilyParseBlock(LilyParseBlock *self);

// Parse array expression
static LilyAstExpr *
parse_array_expr__LilyParseBlock(LilyParseBlock *self);

// Parse tuple expression
static LilyAstExpr *
parse_tuple_expr__LilyParseBlock(LilyParseBlock *self);

// Parse binary expression
static LilyAstExpr *
parse_binary_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *expr);

// Parse fun/method call expression
static LilyAstExpr *
parse_fun_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

// Parse record call expression
static LilyAstExpr *
parse_record_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

// Parse variant call expression
static LilyAstExpr *
parse_variant_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

// Parse call expression
static LilyAstExpr *
parse_call_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

/// @return Vec<LilyAstExprLambdaParam*>*
static Vec *
parse_lambda_params__LilyParser(LilyParser *self, const Vec *pre_params);

/// @return Vec<LilyAstExprLambdaParamCall*>*
static Vec *
parse_lambda_params_call__LilyParser(LilyParser *self,
                                     const Vec *pre_params_call);

// Parse lambda expression.
static LilyAstExpr *
parse_lambda_expr__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *lambda);

// Parse list expression
static LilyAstExpr *
parse_list_expr__LilyParseBlock(LilyParseBlock *self);

// Parse literal expression
static LilyAstExpr *
parse_literal_expr__LilyParseBlock(LilyParseBlock *self);

static LilyAstExpr *
parse_only_identifier_expr__LilyParseBlock(LilyParseBlock *self);

static LilyAstExpr *
parse_access_expr_for_pattern__LilyParseBlock(LilyParseBlock *self);

static LilyAstExpr *
parse_path_access_for_pattern__LilyParseBlock(LilyParseBlock *self,
                                              LilyAstExpr *begin,
                                              enum LilyAstExprAccessKind kind);

static LilyAstExpr *
parse_primary_expr__LilyParseBlock(LilyParseBlock *self, bool not_parse_access);

static LilyAstExpr *
parse_postfix_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *expr);

// Parse expression
static LilyAstExpr *
parse_expr__LilyParseBlock(LilyParseBlock *self);

// Parse capture.
static LilyAstCapture *
parse_capture__LilyParseBlock(LilyParseBlock *self);

// Parse asm statement.
static LilyAstBodyFunItem *
parse_asm_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item);

// Parse await statement.
static LilyAstBodyFunItem *
parse_await_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse block statement.
static inline LilyAstBodyFunItem *
parse_block_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse break statement.
static LilyAstBodyFunItem *
parse_break_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse defer statement.
static LilyAstBodyFunItem *
parse_defer_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse drop statement.
static LilyAstBodyFunItem *
parse_drop_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item);

// Parse for statement.
static LilyAstBodyFunItem *
parse_for_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item);

// Parse if statement.
static LilyAstBodyFunItem *
parse_if_stmt__LilyParser(LilyParser *self,
                          const LilyPreparserFunBodyItem *item);

// Parse match statement.
static LilyAstBodyFunItem *
parse_match_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse next statement.
static LilyAstBodyFunItem *
parse_next_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item);

// Parse raise statement.
static LilyAstBodyFunItem *
parse_raise_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse return statement.
static LilyAstBodyFunItem *
parse_return_stmt__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *item);

// Parse try statement.
static LilyAstBodyFunItem *
parse_try_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item);

// Parse unsafe statement.
static inline LilyAstBodyFunItem *
parse_unsafe_stmt__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *item);

// Parse variable statement.
static LilyAstBodyFunItem *
parse_variable_stmt__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item);

// Parse while statement.
static LilyAstBodyFunItem *
parse_while_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse statement.
static LilyAstBodyFunItem *
parse_stmt__LilyParser(LilyParser *self, const LilyPreparserFunBodyItem *item);

// Parse item of the body of the fun/method.
// NOTE: It's used to parse expr/stmt in statement (e.g. defer).
static LilyAstBodyFunItem *
parse_fun_body_item_for_stmt__LilyParser(LilyParser *self,
                                         const LilyPreparserFunBodyItem *item);

// Parse item of the body of the fun/method.
/// @param body Vec<LilyAstBodyFunItem*>*
static void
parse_fun_body_item__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item,
                                Vec *body);

/// @return Vec<LilyAstBodyFunItem*>*
static Vec *
parse_fun_body__LilyParser(LilyParser *self, Vec *block);

// Parse array pattern
// [a, _, b, ..]
static LilyAstPattern *
parse_array_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse as pattern
// _ as a
static LilyAstPattern *
parse_as_pattern__LilyParseBlock(LilyParseBlock *self, LilyAstPattern *pattern);

// Parse exception pattern
// error InvalidFile:x
static LilyAstPattern *
parse_exception_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse list pattern
// {a, _, b, ..}
static LilyAstPattern *
parse_list_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse list head pattern
// <head> -> <list>
static LilyAstPattern *
parse_list_head_pattern__LilyParseBlock(LilyParseBlock *self,
                                        LilyAstPattern *left);

// Parse list tail pattern
// <list> <- <tail>
static LilyAstPattern *
parse_list_tail_pattern__LilyParseBlock(LilyParseBlock *self,
                                        LilyAstPattern *left);

// Parse literal pattern
// "Hello"
static LilyAstPattern *
parse_literal_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse range pattern
// 'a' .. 'z'
static LilyAstPattern *
parse_range_pattern__LilyParseBlock(LilyParseBlock *self, LilyAstPattern *left);

// Parse record call pattern
// Person { name, .. }
static LilyAstPattern *
parse_record_call_pattern__LilyParseBlock(LilyParseBlock *self,
                                          LilyAstExpr *id);

// Parse tuple pattern
// (1, 2, _)
static LilyAstPattern *
parse_tuple_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse variant call pattern
// Cat:cat
static LilyAstPattern *
parse_variant_call_pattern__LilyParseBlock(LilyParseBlock *self,
                                           LilyAstExpr *id);

// Parse all patterns.
static LilyAstPattern *
parse_pattern__LilyParseBlock(LilyParseBlock *self);

static LilyAstGenericParam *
parse_generic_param__LilyParseBlock(LilyParseBlock *self);

/// @param generic_params Vec<Vec<LilyToken* (&)>*>*
static Vec *
parse_generic_params__LilyParser(LilyParser *self, Vec *generic_params);

/// @param inherit_params Vec<Vec<LilyToken* (&)>*>*
static Vec *
parse_inherit_params__LilyParser(LilyParser *self, Vec *inherit_params);

/// @param impl_params Vec<Vec<LilyToken* (&)>*>*
static Vec *
parse_impl_params__LilyParser(LilyParser *self, Vec *impl_params);

// Parse alias declaration.
static LilyAstDecl *
parse_alias_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse attribute declaration.
static void
parse_attribute_decl__LilyParser(LilyParser *self,
                                 const LilyPreparserAttribute *pre_attribute,
                                 LilyAstDeclAttribute *attribute);

// Parse attribute declaration for class.
static LilyAstBodyClassItem *
parse_attribute_decl_for_class__LilyParser(
  LilyParser *self,
  const LilyPreparserClassBodyItem *item);

// Parse attribute declaration for trait.
static LilyAstBodyTraitItem *
parse_attribute_decl_for_trait__LilyParser(
  LilyParser *self,
  const LilyPreparserTraitBodyItem *item);

// Parse body of class.
/// @param body Vec<LilyPreparserClassBodyItem*>*
static Vec *
parse_class_body__LilyParser(LilyParser *self, Vec *pre_body);

// Parse class declaration.
static LilyAstDecl *
parse_class_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse constant declaration.
static LilyAstDecl *
parse_constant_decl__LilyParser(LilyParser *self,
                                LilyPreparserConstantInfo *info,
                                Location *location);

// Parse constant declaration for enum object.
static LilyAstBodyEnumObjectItem *
parse_constant_decl_for_enum_object__LilyParser(LilyParser *self,
                                                LilyPreparserConstantInfo *info,
                                                Location *location);

// Parse constant declaration for record object.
static LilyAstBodyRecordObjectItem *
parse_constant_decl_for_record_object__LilyParser(
  LilyParser *self,
  LilyPreparserConstantInfo *info,
  Location *location);

// Parse enum variant.
static LilyAstVariant *
parse_enum_variant__LilyParser(LilyParser *self,
                               LilyPreparserEnumBodyItem *item);

// Parse enum variant for enum object.
static LilyAstBodyEnumObjectItem *
parse_enum_variant_for_enum_object__LilyParser(
  LilyParser *self,
  LilyPreparserEnumObjectBodyItem *item);

// Parse enum declaration.
static LilyAstDecl *
parse_enum_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse body of enum object.
static Vec *
parse_enum_object_body__LilyParser(LilyParser *self, Vec *pre_body);

// Parse enum object declaration.
static LilyAstDecl *
parse_enum_object_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse error declaration.
static LilyAstDecl *
parse_error_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse use declaration.
static LilyAstDecl *
parse_use_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse include declaration.
static LilyAstDecl *
parse_include_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

static LilyAstDeclFunParam *
parse_fun_param__LilyParseBlock(LilyParseBlock *self);

// Parse fun params.
/// @param params Vec<Vec<LilyToken*>*>*
static Vec *
parse_fun_params__LilyParser(LilyParser *self, Vec *params);

// Parse fun declaration.
static LilyAstDecl *
parse_fun_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

static LilyAstDeclMethodParam *
parse_method_param__LilyParseBlock(LilyParseBlock *self);

// Parse method params.
/// @param params Vec<Vec<LilyToken*>*>*
static Vec *
parse_method_params__LilyParser(LilyParser *self, Vec *params);

// Parse method declaration.
static LilyAstBodyClassItem *
parse_method_decl__LilyParser(LilyParser *self,
                              LilyPreparserClassBodyItem *item);

// Parse method declaration for enum object.
static LilyAstBodyEnumObjectItem *
parse_method_decl_for_enum_object__LilyParser(
  LilyParser *self,
  LilyPreparserEnumObjectBodyItem *item);

// Parse method declaration.
static LilyAstBodyRecordObjectItem *
parse_method_decl_for_record_object__LilyParser(
  LilyParser *self,
  LilyPreparserRecordObjectBodyItem *item);

// Parse module declaration.
static LilyAstDecl *
parse_module_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse object declaration.
static LilyAstDecl *
parse_object_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse prototype declaration.
static LilyAstBodyTraitItem *
parse_prototype_decl__LilyParser(LilyParser *self,
                                 LilyPreparserTraitBodyItem *item);

// Parse record field.
static LilyAstField *
parse_record_field__LilyParser(LilyParser *self,
                               LilyPreparserRecordBodyItem *item);

// Parse record declaration.
static LilyAstDecl *
parse_record_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse record field object.
static LilyAstBodyRecordObjectItem *
parse_record_object_field__LilyParser(LilyParser *self,
                                      LilyPreparserRecordObjectBodyItem *item);

// Parse body of record object.
static Vec *
parse_record_object_body__LilyParser(LilyParser *self, Vec *pre_body);

// Parse record object declaration.
static LilyAstDecl *
parse_record_object_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse body of trait.
static Vec *
parse_trait_body__LilyParser(LilyParser *self, Vec *pre_body);

// Parse trait declaration.
static LilyAstDecl *
parse_trait_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

// Parse type declaration.
static LilyAstDecl *
parse_type_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

static LilyMacro *
search_private_macro__LilyParser(const LilyParser *self, const String *name);

static LilyMacro *
search_public_macro__LilyParser(const LilyParser *self, const String *name);

static LilyMacro *
search_macro__LilyParser(const LilyParser *self, const String *name);

// Check if the token is an identifier.
static bool
is_id__LilyParser(const Vec *tokens);

// Check if the first token can be a data type.
static bool
is_dt__LilyParser(const Vec *tokens);

// Check if there are one token.
static inline bool
is_tk__LilyParser(const Vec *tokens);

// Check if there are many tokens.
static inline bool
is_tks__LilyParser(const Vec *tokens);

// Check if the first token can be a stmt.
static bool
is_stmt__LilyParser(const Vec *tokens);

// Check if the first token can be an expr.
static bool
is_expr__LilyParser(const Vec *tokens);

// Check if is a path.
static bool
is_path__LilyParser(const Vec *tokens);

// Check if is a pattern.
static bool
is_patt__LilyParser(const Vec *tokens);

// Check if is a block.
static bool
is_block__LilyParser(const Vec *tokens);

/// @param body Body of record
static void
apply_macro_expansion_in_record__LilyParser(LilyParser *self,
                                            LilyPreparserRecordBodyItem *item,
                                            Vec *body);

/// @param body Body of enum
static void
apply_macro_expansion_in_enum__LilyParser(LilyParser *self,
                                          LilyPreparserEnumBodyItem *item,
                                          Vec *body);

static void
apply_macro_expansion_in_class__LilyParser(LilyParser *self,
                                           LilyPreparserClassBodyItem *item,
                                           Vec *body);

/// @param body Body of record object
static void
apply_macro_expansion_in_record_object__LilyParser(
  LilyParser *self,
  LilyPreparserRecordObjectBodyItem *item,
  Vec *body);

/// @param body Body of enum object
static void
apply_macro_expansion_in_enum_object__LilyParser(
  LilyParser *self,
  LilyPreparserEnumObjectBodyItem *item,
  Vec *body);

/// @param body Body of trait
static void
apply_macro_expansion_in_trait__LilyParser(LilyParser *self,
                                           LilyPreparserTraitBodyItem *item,
                                           Vec *body);

static inline bool
must_close_macro_expand__LilyPreparser(LilyPreparser *self);

/// @param body Body of fun
static void
apply_macro_expansion_in_fun__LilyParser(LilyParser *self,
                                         LilyPreparserFunBodyItem *item,
                                         Vec *body);

static void
apply_macro_expansion__LilyParser(LilyParser *self,
                                  LilyPreparserDecl *decl,
                                  Vec *decls);

// Parse declaration.
static LilyAstDecl *
parse_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl);

static void
parse_decls__LilyParser(LilyParser *self, Vec *decls, const Vec *pre_decls);

#define SKIP_TO_TOKEN(k)                                 \
    while (self->current->kind != k &&                   \
           self->current->kind != LILY_TOKEN_KIND_EOF) { \
        next_token__LilyParseBlock(self);                \
    }                                                    \
    next_token__LilyParseBlock(self);

#define CHECK_COMMA(closing)                                                  \
    if (self->current->kind != closing) {                                     \
        switch (self->current->kind) {                                        \
            case LILY_TOKEN_KIND_COMMA:                                       \
                next_token__LilyParseBlock(self);                             \
                break;                                                        \
            default:                                                          \
                emit__Diagnostic(                                             \
                  NEW_VARIANT(Diagnostic,                                     \
                              simple_lily_error,                              \
                              self->file,                                     \
                              &self->current->location,                       \
                              NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN), \
                              NULL,                                           \
                              NULL,                                           \
                              from__String("expected `,`")),                  \
                  self->count_error);                                         \
                break;                                                        \
        }                                                                     \
    }

// Check if the ParseBlock has reached the end.
#define HAS_REACHED_THE_END(block) (block.position == block.tokens->len - 1)

#define CHECK_EXPR(expr, expr_block, help, detail_msg, block_free)        \
    if (!expr) {                                                          \
        block_free;                                                       \
    }                                                                     \
                                                                          \
    if (!HAS_REACHED_THE_END(expr_block)) {                               \
        emit__Diagnostic(                                                 \
          NEW_VARIANT(                                                    \
            Diagnostic,                                                   \
            simple_lily_error,                                            \
            expr_block.file,                                              \
            &expr_block.current->location,                                \
            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION), \
            NULL,                                                         \
            help,                                                         \
            from__String(detail_msg)),                                    \
          expr_block.count_error);                                        \
    }

#define CHECK_PATTERN(pattern, pattern_block, help, detail_msg, block_free) \
    if (!pattern) {                                                         \
        block_free;                                                         \
    }                                                                       \
                                                                            \
    if (!HAS_REACHED_THE_END(pattern_block)) {                              \
        emit__Diagnostic(                                                   \
          NEW_VARIANT(                                                      \
            Diagnostic,                                                     \
            simple_lily_error,                                              \
            pattern_block.file,                                             \
            &pattern_block.current->location,                               \
            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_ONLY_ONE_PATTERN),      \
            NULL,                                                           \
            help,                                                           \
            from__String(detail_msg)),                                      \
          pattern_block.count_error);                                       \
    }

#define CHECK_DATA_TYPE(dt, dt_block, help, detail_msg, block_free)      \
    if (!dt) {                                                           \
        block_free;                                                      \
    }                                                                    \
                                                                         \
    if (!HAS_REACHED_THE_END(dt_block)) {                                \
        emit__Diagnostic(                                                \
          NEW_VARIANT(                                                   \
            Diagnostic,                                                  \
            simple_lily_error,                                           \
            dt_block.file,                                               \
            &dt_block.current->location,                                 \
            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_ONLY_ONE_DATA_TYPE), \
            NULL,                                                        \
            help,                                                        \
            from__String(detail_msg)),                                   \
          dt_block.count_error);                                         \
    }

CONSTRUCTOR(LilyParseBlock, LilyParseBlock, LilyParser *parser, Vec *tokens)
{
    Location location_eof =
      clone__Location(&CAST(LilyToken *, last__Vec(tokens))->location);
    start__Location(&location_eof,
                    location_eof.end_line,
                    location_eof.end_column,
                    location_eof.end_position);

    // This token is used to check if the parser has reached the end.
    push__Vec(tokens,
              NEW(LilyToken, LILY_TOKEN_KIND_EOF, location_eof)); // Add EOF.

    return (LilyParseBlock){ .parser = parser,
                             .tokens = tokens,
                             .current = get__Vec(tokens, 0),
                             .previous = get__Vec(tokens, 0),
                             .file = &parser->package->file,
                             .count_error = &parser->package->count_error,
                             .count_warning = &parser->package->count_warning,
                             .position = 0 };
}

DESTRUCTOR(LilyParseBlock, const LilyParseBlock *self)
{
    FREE(LilyToken, pop__Vec(self->tokens));
}

void
next_decl__LilyParser(LilyParser *self)
{
    if (self->position + 1 < self->package->preparser_info.decls->len) {
        ++self->position;
        self->current =
          get__Vec(self->package->preparser_info.decls, self->position);
    }
}

void
next_token__LilyParseBlock(LilyParseBlock *self)
{
    if (self->position + 1 < self->tokens->len) {
        self->previous = get__Vec(self->tokens, self->position++);
        self->current = get__Vec(self->tokens, self->position);
    } else {
        self->previous = last__Vec(self->tokens);
        self->current = self->previous;
    }
}

void
jump__LilyParseBlock(LilyParseBlock *self, Usize n)
{
    if (self->position + n < self->tokens->len) {
        self->position += n;
        self->current = get__Vec(self->tokens, self->position);

        return;
    }

    UNREACHABLE("cannot jump outside of the length of tokens");
}

LilyToken *
peek_token__LilyParseBlock(LilyParseBlock *self, Usize n)
{
    if (self->position + n < self->tokens->len) {
        return get__Vec(self->tokens, self->position + n);
    }

    return NULL;
}

#define IS_DATA_TYPE(token)                     \
    switch (token) {                            \
        case LILY_TOKEN_KIND_BANG:              \
        case LILY_TOKEN_KIND_STAR:              \
        case LILY_TOKEN_KIND_INTERROGATION:     \
        case LILY_TOKEN_KIND_KEYWORD_FUN:       \
        case LILY_TOKEN_KIND_KEYWORD_MUT:       \
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:    \
        case LILY_TOKEN_KIND_KEYWORD_REF:       \
        case LILY_TOKEN_KIND_KEYWORD_TRACE:     \
        case LILY_TOKEN_KIND_KEYWORD_SELF:      \
        case LILY_TOKEN_KIND_L_BRACE:           \
        case LILY_TOKEN_KIND_L_HOOK:            \
        case LILY_TOKEN_KIND_L_PAREN:           \
        case LILY_TOKEN_KIND_L_SHIFT:           \
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: \
            return true;                        \
        default:                                \
            return false;                       \
    }

bool
is_data_type__LilyParseBlock(LilyParseBlock *self)
{
    IS_DATA_TYPE(self->current->kind);
}

LilyAstDataType *
parse_data_type__LilyParseBlock(LilyParseBlock *self)
{
    // Any
    // [_]<dt>
    // [*]<dt>
    // [<size>]<dt>
    // [?]<dt>
    // Bool
    // Byte
    // Bytes
    // Char
    // CShort
    // CUshort
    // CInt
    // CUint
    // CLong
    // CUlong
    // CLonglong
    // CULonglong
    // CFloat
    // CDouble
    // CStr
    // CVoid
    // <<err|errs>>!<dt>
    // Float32
    // Float64
    // Int16
    // Int32
    // Int64
    // Int8
    // Isize
    // fun(<dt>, ...) <return_dt>
    // {<dt>}
    // mut <dt>
    // Never
    // Object
    // ?<dt>
    // *<dt>
    // ref <dt>
    // Self
    // Str
    // trace <dt>
    // (<dt>, ...)
    // Uint16
    // Uint32
    // Uint64
    // Uint8
    // Unit
    // Usize
    // <name>.<name>...[<dt>]
    next_token__LilyParseBlock(self);

    Location location = clone__Location(&self->previous->location);

    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            END_LOCATION(&location, self->previous->location);

            if (!strcmp(self->previous->identifier_normal->buffer, "Any")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_ANY, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Bool")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BOOL, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Byte")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BYTE, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Bytes")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BYTES, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Char")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CHAR, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CShort")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CSHORT, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CUshort")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CUSHORT, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CInt")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CINT, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CUint")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CUINT, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CLong")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CLONG, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CUlong")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CULONG, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CLonglong")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CLONGLONG, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CFloat")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CFLOAT, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CDouble")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CDOUBLE, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CStr")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CSTR, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "CVoid")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CVOID, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Float32")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_FLOAT32, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Float64")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_FLOAT64, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int16")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT16, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int32")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT32, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int64")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT64, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int8")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT8, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Isize")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_ISIZE, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Never")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_NEVER, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Str")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_STR, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint16")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT16, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint32")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT32, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint64")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT64, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint8")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT8, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Unit")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UNIT, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Usize")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_USIZE, location);
            } else {
                // 1. Parse name
                String *name = clone__String(self->previous->identifier_normal);

                while (self->current->kind == LILY_TOKEN_KIND_DOT) {
                    next_token__LilyParseBlock(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                            push__String(name, '.');
                            append__String(name,
                                           self->current->identifier_normal);
                            next_token__LilyParseBlock(self);

                            break;
                        default: {
                            String *token_s =
                              to_string__LilyToken(self->current);

                            emit__Diagnostic(
                              NEW_VARIANT(Diagnostic,
                                          simple_lily_error,
                                          self->file,
                                          &self->current->location,
                                          NEW_VARIANT(LilyError,
                                                      unexpected_token,
                                                      token_s->buffer),
                                          NULL,
                                          NULL,
                                          from__String("expected identifier")),
                              self->count_error);

                            FREE(String, token_s);
                            FREE(String, name);

                            return NULL;
                        }
                    }
                }

                // 2. Parse generics
                Vec *generics = NULL; // Vec<LilyAstDataType*>*?

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_L_HOOK:
                        generics = NEW(Vec);
                        next_token__LilyParseBlock(self);

                        while (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                            LilyAstDataType *dt =
                              parse_data_type__LilyParseBlock(self);

                            if (!dt) {
                                FREE_BUFFER_ITEMS(generics->buffer,
                                                  generics->len,
                                                  LilyAstDataType);
                                FREE(Vec, generics);

                                return NULL;
                            }

                            push__Vec(generics, dt);

                            if (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                                switch (self->current->kind) {
                                    case LILY_TOKEN_KIND_COMMA:
                                        next_token__LilyParseBlock(self);
                                        break;
                                    default:
                                        emit__Diagnostic(
                                          NEW_VARIANT(
                                            Diagnostic,
                                            simple_lily_error,
                                            self->file,
                                            &self->current->location,
                                            NEW(LilyError,
                                                LILY_ERROR_KIND_EXPECTED_TOKEN),
                                            NULL,
                                            NULL,
                                            from__String("expected `,`")),
                                          self->count_error);

                                        break;
                                }
                            }
                        }

                        END_LOCATION(&location, self->current->location);
                        next_token__LilyParseBlock(self);

                        break;
                    default: {
                        LilyToken *token =
                          get__Vec(self->tokens, self->position - 1);

                        END_LOCATION(&location, token->location);

                        break;
                    }
                }

                return NEW_VARIANT(LilyAstDataType,
                                   custom,
                                   location,
                                   NEW(LilyAstDataTypeCustom, name, generics));
            }

        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
        case LILY_TOKEN_KIND_KEYWORD_SELF:
            END_LOCATION(&location, self->previous->location);

            switch (self->previous->kind) {
                case LILY_TOKEN_KIND_KEYWORD_OBJECT:
                    return NEW(LilyAstDataType,
                               LILY_AST_DATA_TYPE_KIND_OBJECT,
                               location);
                case LILY_TOKEN_KIND_KEYWORD_SELF:
                    return NEW(
                      LilyAstDataType, LILY_AST_DATA_TYPE_KIND_SELF, location);
                default:
                    UNREACHABLE("this way is impossible");
            }

        case LILY_TOKEN_KIND_L_HOOK:
#define EXPECTED_R_HOOK()                                                 \
    switch (self->current->kind) {                                        \
        case LILY_TOKEN_KIND_R_HOOK:                                      \
            next_token__LilyParseBlock(self);                             \
            break;                                                        \
        default:                                                          \
            emit__Diagnostic(                                             \
              NEW_VARIANT(Diagnostic,                                     \
                          simple_lily_error,                              \
                          self->file,                                     \
                          &self->current->location,                       \
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN), \
                          NULL,                                           \
                          NULL,                                           \
                          from__String("expected `]`")),                  \
              self->count_error);                                         \
            return NULL;                                                  \
    }

#define PARSE_ARRAY_DATA_TYPE()                                           \
    LilyAstDataType *dt = NULL;                                           \
    if (is_data_type__LilyParseBlock(self)) {                             \
        dt = parse_data_type__LilyParseBlock(self);                       \
                                                                          \
        if (!dt) {                                                        \
            return NULL;                                                  \
        }                                                                 \
                                                                          \
        END_LOCATION(&location, dt->location);                            \
    } else {                                                              \
        LilyToken *previous = get__Vec(self->tokens, self->position - 1); \
                                                                          \
        END_LOCATION(&location, previous->location);                      \
    }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                    if (!strcmp(self->current->identifier_normal->buffer,
                                "_")) {
                        next_token__LilyParseBlock(self);

                        EXPECTED_R_HOOK();
                        PARSE_ARRAY_DATA_TYPE();

                        return NEW_VARIANT(
                          LilyAstDataType,
                          array,
                          location,
                          NEW(LilyAstDataTypeArray,
                              LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC,
                              dt));
                    } else {
                        String *token_s = to_string__LilyToken(self->current);

                        emit__Diagnostic(
                          NEW_VARIANT(Diagnostic,
                                      simple_lily_error,
                                      self->file,
                                      &self->current->location,
                                      NEW_VARIANT(LilyError,
                                                  unexpected_token,
                                                  token_s->buffer),
                                      NULL,
                                      NULL,
                                      from__String("expected `_`")),
                          self->count_error);

                        FREE(String, token_s);

                        return NULL;
                    }
                case LILY_TOKEN_KIND_STAR: {
                    next_token__LilyParseBlock(self);

                    EXPECTED_R_HOOK();
                    PARSE_ARRAY_DATA_TYPE();

                    return NEW_VARIANT(
                      LilyAstDataType,
                      array,
                      location,
                      NEW(LilyAstDataTypeArray,
                          LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS,
                          dt));
                }

                case LILY_TOKEN_KIND_LITERAL_INT_10: {
                    Optional *size_op = atoi_safe__Usize(
                      self->current->literal_int_10->buffer, 10);

                    if (is_none__Optional(size_op)) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_USIZE_OUT_OF_RANGE),
                            NULL,
                            NULL,
                            NULL),
                          self->count_error);

                        FREE(Optional, size_op);

                        return NULL;
                    }

                    Usize size = (Usize)(Uptr)get__Optional(size_op);

                    next_token__LilyParseBlock(self);

                    FREE(Optional, size_op);
                    EXPECTED_R_HOOK();
                    PARSE_ARRAY_DATA_TYPE();

                    return NEW_VARIANT(
                      LilyAstDataType,
                      array,
                      location,
                      NEW_VARIANT(LilyAstDataTypeArray, sized, dt, size));
                }

                case LILY_TOKEN_KIND_INTERROGATION: {
                    next_token__LilyParseBlock(self);

                    EXPECTED_R_HOOK();
                    PARSE_ARRAY_DATA_TYPE();

                    return NEW_VARIANT(
                      LilyAstDataType,
                      array,
                      location,
                      NEW(LilyAstDataTypeArray,
                          LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED,
                          dt));
                }

                default: {
                    String *token_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW_VARIANT(
                          LilyError, unexpected_token, token_s->buffer),
                        NULL,
                        NULL,
                        from__String(
                          "expected `_`, `*`, `?` or literal integer")),
                      self->count_error);

                    FREE(String, token_s);

                    return NULL;
                }
            }

            break;

        case LILY_TOKEN_KIND_L_SHIFT: {
            Vec *errs = NEW(Vec);

            while (self->current->kind != LILY_TOKEN_KIND_R_SHIFT &&
                   self->current->kind != LILY_TOKEN_KIND_EOF) {
                Location *location_error = &self->current->location;
                LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

                if (dt) {
                    switch (dt->kind) {
                        case LILY_AST_DATA_TYPE_KIND_CUSTOM:
                            push__Vec(errs, dt);
                            break;
                        default:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                location_error,
                                NEW(LilyError,
                                    LILY_ERROR_KIND_EXPECTED_ERROR_DATA_TYPE),
                                NULL,
                                NULL,
                                NULL),
                              self->count_error);
                    }
                }
            }

            if (self->current->kind == LILY_TOKEN_KIND_EOF) {
                String *token_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected `>`")),
                  self->count_error);

                FREE(String, token_s);
            } else {
                next_token__LilyParseBlock(self); // skip `>`
            }

            if (self->current->kind == LILY_TOKEN_KIND_BANG) {
                next_token__LilyParseBlock(self); // skip `!`
            } else {
                String *token_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected `!`")),
                  self->count_error);

                FREE(String, token_s);
            }

            LilyAstDataType *ok = parse_data_type__LilyParseBlock(self);

            END_LOCATION(&location, ok->location);

            if (ok) {
                return NEW_VARIANT(LilyAstDataType,
                                   result,
                                   location,
                                   NEW(LilyAstDataTypeResult, ok, errs));
            }

            return NULL;
        }
        case LILY_TOKEN_KIND_BANG:
        case LILY_TOKEN_KIND_INTERROGATION:
        case LILY_TOKEN_KIND_STAR:
        case LILY_TOKEN_KIND_KEYWORD_REF:
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
        case LILY_TOKEN_KIND_KEYWORD_MUT: {
            enum LilyTokenKind kind = self->previous->kind;
            LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

            if (!dt) {
                return NULL;
            }

            END_LOCATION(&location, dt->location);

            switch (kind) {
                case LILY_TOKEN_KIND_BANG:
                    return NEW_VARIANT(LilyAstDataType,
                                       result,
                                       location,
                                       NEW(LilyAstDataTypeResult, dt, NULL));
                case LILY_TOKEN_KIND_INTERROGATION:
                    return NEW_VARIANT(LilyAstDataType, optional, location, dt);
                case LILY_TOKEN_KIND_STAR:
                    return NEW_VARIANT(LilyAstDataType, ptr, location, dt);
                case LILY_TOKEN_KIND_KEYWORD_REF:
                    return NEW_VARIANT(LilyAstDataType, ref, location, dt);
                case LILY_TOKEN_KIND_KEYWORD_TRACE:
                    return NEW_VARIANT(LilyAstDataType, trace, location, dt);
                case LILY_TOKEN_KIND_KEYWORD_MUT:
                    return NEW_VARIANT(LilyAstDataType, mut, location, dt);
                default:
                    UNREACHABLE("this way is impossible");
            }
        }

        case LILY_TOKEN_KIND_KEYWORD_FUN: {
            Vec *params = NULL;                       // Vec<LilyAstDataType*>*?
            LilyAstDataType *return_data_type = NULL; // LilyAstDataType*?

            // 1. Parse params
            if (self->current->kind == LILY_TOKEN_KIND_L_PAREN) {
                next_token__LilyParseBlock(self);

                params = NEW(Vec);

                while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                    LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

                    if (!dt) {
                        // TODO: skip to next right paren.

                        FREE_BUFFER_ITEMS(
                          params->buffer, params->len, LilyAstDataType);
                        FREE(Vec, params);

                        return NULL;
                    }

                    push__Vec(params, dt);

                    if (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_COMMA:
                                next_token__LilyParseBlock(self);
                                break;
                            default:
                                emit__Diagnostic(
                                  NEW_VARIANT(
                                    Diagnostic,
                                    simple_lily_error,
                                    self->file,
                                    &self->current->location,
                                    NEW(LilyError,
                                        LILY_ERROR_KIND_EXPECTED_TOKEN),
                                    NULL,
                                    NULL,
                                    from__String("expected `,`")),
                                  self->count_error);

                                break;
                        }
                    }
                }

                next_token__LilyParseBlock(self);
            }

            // 2. Parse return data type
            if (is_data_type__LilyParseBlock(self)) {
                return_data_type = parse_data_type__LilyParseBlock(self);

                if (!return_data_type) {
                    if (params) {
                        FREE_BUFFER_ITEMS(
                          params->buffer, params->len, LilyAstDataType);
                        FREE(Vec, params);
                    }

                    return NULL;
                }

                END_LOCATION(&location, return_data_type->location);
            } else {
                if (params) {
                    LilyToken *last = last__Vec(params);

                    END_LOCATION(&location, last->location);
                } else {
                    LilyToken *token =
                      get__Vec(self->tokens, self->position - 1);

                    END_LOCATION(&location, token->location);
                }
            }

            return NEW_VARIANT(
              LilyAstDataType,
              lambda,
              location,
              NEW(LilyAstDataTypeLambda, params, return_data_type));
        }

        case LILY_TOKEN_KIND_L_BRACE: {
            LilyAstDataType *data_type = parse_data_type__LilyParseBlock(self);

            if (!data_type) {
                SKIP_TO_TOKEN(LILY_TOKEN_KIND_R_BRACE);

                return NULL;
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_R_BRACE:
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyParseBlock(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `}`")),
                      self->count_error);

                    SKIP_TO_TOKEN(LILY_TOKEN_KIND_R_BRACE);
            }

            return NEW_VARIANT(LilyAstDataType, list, location, data_type);
        }

        case LILY_TOKEN_KIND_L_PAREN: {
            Vec *tuple = NEW(Vec); // Vec<LilyAstDataType*>*

            while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

                if (!dt) {
                    FREE_BUFFER_ITEMS(
                      tuple->buffer, tuple->len, LilyAstDataType);
                    FREE(Vec, tuple);

                    return NULL;
                }

                push__Vec(tuple, dt);

                if (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_COMMA:
                            next_token__LilyParseBlock(self);
                            break;
                        default:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                                NULL,
                                NULL,
                                from__String("expected `,`")),
                              self->count_error);

                            break;
                    }
                }
            }

            END_LOCATION(&location, self->current->location);
            next_token__LilyParseBlock(self);

            return NEW_VARIANT(LilyAstDataType, tuple, location, tuple);
        }

        case LILY_TOKEN_KIND_EXPAND:
            switch (self->previous->expand.kind) {
                case LILY_TOKEN_EXPAND_KIND_DT: {
                    LilyParseBlock dt_block =
                      NEW(LilyParseBlock,
                          self->parser,
                          self->previous->expand.tokens);
                    LilyAstDataType *dt =
                      parse_data_type__LilyParseBlock(&dt_block);

                    CHECK_DATA_TYPE(
                      dt, dt_block, NULL, "expected only one data type", {});

                    FREE(LilyParseBlock, &dt_block);

                    return dt;
                }
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->previous->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_DATA_TYPE),
                        init__Vec(1,
                                  from__String("expected `dt` as the data type "
                                               "of the macro param")),
                        NULL,
                        NULL),
                      self->count_error);

                    return NULL;
            }

        default: {
            String *token_s = to_string__LilyToken(self->previous);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->previous->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String(
                  "expected `Any`, `[`, `Bool`, `Byte`, `Bytes`, `Char`, "
                  "`Float32`, `Float64`, `Int16`, `Int32`, `Int64`, `Int8`, "
                  "`Isize`, `fun`, `mut`, `Never`, `Object`, `?`, `*`, `ref`, "
                  "`Self`, `Str`, `trace`, `(`, `Uint16`, `Uint32`, `Uint64`, "
                  "`Uint8`, `Unit`, `Usize` or other identifier")),
              self->count_error);

            FREE(String, token_s);

            return NULL;
        }
    }

    return NULL;
}

LilyAstExpr *
parse_path_access__LilyParseBlock(LilyParseBlock *self,
                                  LilyAstExpr *begin,
                                  enum LilyAstExprAccessKind kind)
{
    LilyAstExpr *current_access = NULL;
    Location *current_location = NULL;

    if (begin) {
        if (self->current->kind != LILY_TOKEN_KIND_DOT) {
            return begin;
        }

        ASSERT(kind == LILY_AST_EXPR_ACCESS_KIND_PATH);

        current_access = NEW_VARIANT(
          LilyAstExpr,
          access,
          clone__Location(&begin->location),
          NEW_VARIANT(LilyAstExprAccess, path, init__Vec(1, begin)));
        current_location = &current_access->location;
    } else {
        ASSERT(kind == LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH ||
               kind == LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH ||
               kind == LILY_AST_EXPR_ACCESS_KIND_SELF_PATH ||
               kind == LILY_AST_EXPR_ACCESS_KIND_self_PATH ||
               kind == LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT);

        Location *prev_location = &self->previous->location;

        LilyAstExpr *first_expr =
          parse_primary_expr__LilyParseBlock(self, true);
        Location expr_location = clone__Location(&first_expr->location);

        START_LOCATION(&expr_location, (*prev_location));

        switch (kind) {
            case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, global_path, NEW(Vec)));

                break;
            case LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, object_path, NEW(Vec)));

                break;
            case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, Self_path, NEW(Vec)));

                break;
            case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, self_path, NEW(Vec)));

                break;
            case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, property_init, NEW(Vec)));

                break;
            default:
                UNREACHABLE("this situation is impossible");
        }

        if (add_item_to_path__LilyAstExprAccess(&current_access, &first_expr)) {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &first_expr->location,
                          NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_CALL_EXPR),
                          NULL,
                          NULL,
                          NULL),
              self->count_error);
        }

        if (self->current->kind != LILY_TOKEN_KIND_DOT) {
            return current_access;
        }

        current_location = &current_access->location;
    }

    while (self->current->kind == LILY_TOKEN_KIND_DOT) {
        if (current_access->kind != LILY_AST_EXPR_KIND_ACCESS) {
        update_current_access: {
            current_access = NEW_VARIANT(
              LilyAstExpr,
              access,
              clone__Location(&current_access->location),
              NEW_VARIANT(
                LilyAstExprAccess, path, init__Vec(1, current_access)));
            current_location = &current_access->location;
        }
        } else {
            if (current_access->access.kind == LILY_AST_EXPR_ACCESS_KIND_HOOK) {
                goto update_current_access;
            }
        }

        next_token__LilyParseBlock(self); // skip `.`

        LilyAstExpr *expr = parse_primary_expr__LilyParseBlock(self, true);

        if (!expr) {
            FREE(LilyAstExpr, current_access);

            return NULL;
        }

        if (add_item_to_path__LilyAstExprAccess(&current_access, &expr)) {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &expr->location,
                          NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_CALL_EXPR),
                          NULL,
                          NULL,
                          NULL),
              self->count_error);
        }
    }

    if (current_access->kind == LILY_AST_EXPR_KIND_ACCESS) {
        END_LOCATION(current_location, self->previous->location);
    }

    return current_access;
}

LilyAstExpr *
parse_hook_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access)
{
    // <access>[<expr>]

    next_token__LilyParseBlock(self); // skip `[`

    Location location = clone__Location(&access->location);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

    if (!expr) {
        FREE(LilyAstExpr, access);

        return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_R_HOOK: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyParseBlock(self);

            LilyAstExpr *res = NEW_VARIANT(
              LilyAstExpr,
              access,
              location,
              NEW_VARIANT(LilyAstExprAccess,
                          hook,
                          NEW(LilyAstExprAccessHook, access, expr)));

            return self->current->kind == LILY_TOKEN_KIND_L_HOOK
                     ? parse_hook_access__LilyParseBlock(self, res)
                     : res;
        }
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `]`")),
              self->count_error);

            FREE(LilyAstExpr, access);
            FREE(LilyAstExpr, expr);

            return NULL;
        }
    }
}

LilyAstExpr *
parse_at_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access)
{
    // <access>@Object@Object2
    Location location;

    if (access) {
        location = clone__Location(&access->location);
    } else {
        location = clone__Location(&self->current->location);
    }

    Vec *objects = NEW(Vec); // Vec<LilyAstDataType*>*

    while (self->previous->kind == LILY_TOKEN_KIND_AT) {
        LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

        if (!dt) {
            FREE(LilyAstExpr, access);
            FREE_BUFFER_ITEMS(objects->buffer, objects->len, LilyAstDataType);
            FREE(Vec, objects);

            return NULL;
        }

        push__Vec(objects, dt);
        next_token__LilyParseBlock(self);
    }

    {
        LilyAstDataType *last = last__Vec(objects);

        END_LOCATION(&location, last->location);
    }

    return NEW_VARIANT(LilyAstExpr,
                       access,
                       location,
                       NEW_VARIANT(LilyAstExprAccess,
                                   at,
                                   NEW(LilyAstExprAccessAt, access, objects)));
}

LilyAstExpr *
parse_access_expr__LilyParseBlock(LilyParseBlock *self)
{
    enum LilyTokenKind kind = self->previous->kind;
    enum LilyAstExprAccessKind access_kind;

    next_token__LilyParseBlock(self); // skip `.`

    switch (kind) {
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
            access_kind = LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH;
            break;
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
            access_kind = LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH;
            break;
        case LILY_TOKEN_KIND_KEYWORD_SELF:
            access_kind = LILY_AST_EXPR_ACCESS_KIND_SELF_PATH;
            break;
        case LILY_TOKEN_KIND_KEYWORD_self:
            access_kind = LILY_AST_EXPR_ACCESS_KIND_self_PATH;
            break;
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            access_kind = LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT;
            break;
        default:
            UNREACHABLE("this situation is impossible");
    }

    LilyAstExpr *path =
      parse_path_access__LilyParseBlock(self, NULL, access_kind);

    if (path) {
        return path;
    }

    return NULL;
}

LilyAstExpr *
parse_only_access_expr__LilyParseBlock(LilyParseBlock *self)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            return parse_only_identifier_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
        case LILY_TOKEN_KIND_KEYWORD_SELF:
        case LILY_TOKEN_KIND_KEYWORD_self:
        case LILY_TOKEN_KIND_AT:
            return parse_access_expr__LilyParseBlock(self);
        default: {
            String *token_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String("expected identifier normal, identifier string, "
                             "`global`, `object`, `Self`, `self` or `@`")),
              self->count_error);

            FREE(String, token_s);

            return NULL;
        }
    }
}

// NOTE: only used to parse expression between (), {}, [] by example.
#define EXPR_PARSE_CLOSING(exprs, closing, loc)                        \
    while (self->current->kind != closing) {                           \
        LilyAstExpr *expr = parse_expr__LilyParseBlock(self);          \
                                                                       \
        if (expr) {                                                    \
            push__Vec(exprs, expr);                                    \
        } else {                                                       \
            SKIP_TO_TOKEN(closing);                                    \
                                                                       \
            FREE_BUFFER_ITEMS(exprs->buffer, exprs->len, LilyAstExpr); \
            FREE(Vec, exprs);                                          \
                                                                       \
            return NULL;                                               \
        }                                                              \
                                                                       \
        CHECK_COMMA(closing)                                           \
    }                                                                  \
                                                                       \
    if (loc) {                                                         \
        END_LOCATION(loc, self->current->location);                    \
    }                                                                  \
                                                                       \
    next_token__LilyParseBlock(self); /* skip closing */

// NOTE: only used to parse data type between (), {}, [] by example.
#define DATA_TYPE_PARSE_CLOSING(dts, closing, loc)                     \
    while (self->current->kind != closing) {                           \
        LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);   \
                                                                       \
        if (dt) {                                                      \
            push__Vec(dts, dt);                                        \
        } else {                                                       \
            SKIP_TO_TOKEN(closing);                                    \
                                                                       \
            FREE_BUFFER_ITEMS(dts->buffer, dts->len, LilyAstDataType); \
            FREE(Vec, dts);                                            \
                                                                       \
            return NULL;                                               \
        }                                                              \
                                                                       \
        CHECK_COMMA(closing)                                           \
    }                                                                  \
                                                                       \
    if (loc) {                                                         \
        END_LOCATION(loc, self->current->location);                    \
    }                                                                  \
                                                                       \
    next_token__LilyParseBlock(self); /* skip closing */

LilyAstExpr *
parse_array_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);
    Vec *exprs = NEW(Vec);

    EXPR_PARSE_CLOSING(exprs, LILY_TOKEN_KIND_R_HOOK, (&location));

    return NEW_VARIANT(
      LilyAstExpr, array, location, NEW(LilyAstExprArray, exprs));
}

LilyAstExpr *
parse_tuple_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);
    Vec *exprs = NEW(Vec);

    EXPR_PARSE_CLOSING(exprs, LILY_TOKEN_KIND_R_PAREN, (&location));

    return NEW_VARIANT(
      LilyAstExpr, tuple, location, NEW(LilyAstExprTuple, exprs));
}

LilyAstExpr *
parse_binary_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *expr)
{
    // TODO: maybe use Stack instead of Vec.
    Vec *stack = NEW(Vec); // Vec<LilyAstExpr* | enum LilyAstExprBinaryKind*>*
    Usize last_precedence = MAX_LILY_AST_EXPR_BINARY_PRECEDENCE + 1;

    push__Vec(stack, expr);

    while (self->current->kind == LILY_TOKEN_KIND_PLUS ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_AND ||
           self->current->kind == LILY_TOKEN_KIND_PLUS_EQ ||
           self->current->kind == LILY_TOKEN_KIND_AMPERSAND_EQ ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_BAR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_SLASH_EQ ||
           self->current->kind == LILY_TOKEN_KIND_STAR_STAR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_PERCENTAGE_EQ ||
           self->current->kind == LILY_TOKEN_KIND_STAR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_MINUS_EQ ||
           self->current->kind == LILY_TOKEN_KIND_XOR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_EQ ||
           self->current->kind == LILY_TOKEN_KIND_AMPERSAND ||
           self->current->kind == LILY_TOKEN_KIND_BAR ||
           self->current->kind == LILY_TOKEN_KIND_BAR_R_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_SLASH ||
           self->current->kind == LILY_TOKEN_KIND_EQ_EQ ||
           self->current->kind == LILY_TOKEN_KIND_STAR_STAR ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT_L_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_PERCENTAGE ||
           self->current->kind == LILY_TOKEN_KIND_STAR ||
           self->current->kind == LILY_TOKEN_KIND_NOT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_OR ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT_R_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_DOT_DOT ||
           self->current->kind == LILY_TOKEN_KIND_MINUS ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_XOR ||
           self->current->kind == LILY_TOKEN_KIND_ARROW ||
           self->current->kind == LILY_TOKEN_KIND_INVERSE_ARROW) {
        enum LilyAstExprBinaryKind op =
          from_token__LilyAstExprBinary(self->current);
        Usize precedence = to_precedence__LilyAstExprBinaryKind(op);

        next_token__LilyParseBlock(self);

        LilyAstExpr *right = parse_primary_expr__LilyParseBlock(self, false);

        if (!right) {
            for (Usize i = 0; i < stack->len; i += 2) {
                FREE(LilyAstExpr, get__Vec(stack, i));
            }

            FREE(Vec, stack);

            return NULL;
        }

        while (precedence <= last_precedence && stack->len > 1) {
            LilyAstExpr *top_right = pop__Vec(stack);
            enum LilyAstExprBinaryKind top_op =
              (enum LilyAstExprBinaryKind)(Uptr)pop__Vec(stack);

            last_precedence = to_precedence__LilyAstExprBinaryKind(top_op);

            if (last_precedence < precedence) {
                push__Vec(stack, (int *)top_op);
                push__Vec(stack, top_right);

                break;
            }

            LilyAstExpr *top_left = pop__Vec(stack);

            push__Vec(
              stack,
              NEW_VARIANT(
                LilyAstExpr,
                binary,
                (Location){ .filename = self->current->location.filename,
                            .start_line = top_left->location.start_line,
                            .end_line = top_right->location.end_line,
                            .start_column = top_left->location.start_column,
                            .end_column = top_right->location.end_column },
                NEW(LilyAstExprBinary, top_op, top_left, top_right)));
        }

        push__Vec(stack, (int *)op);
        push__Vec(stack, right);

        last_precedence = precedence;
    }

    while (stack->len > 1) {
        LilyAstExpr *rhs = pop__Vec(stack);
        enum LilyAstExprBinaryKind op =
          (enum LilyAstExprBinaryKind)(Uptr)pop__Vec(stack);
        LilyAstExpr *lhs = pop__Vec(stack);

        push__Vec(
          stack,
          NEW_VARIANT(LilyAstExpr,
                      binary,
                      (Location){ .filename = self->current->location.filename,
                                  .start_line = lhs->location.start_line,
                                  .end_line = rhs->location.end_line,
                                  .start_column = lhs->location.start_column,
                                  .end_column = rhs->location.end_column },
                      NEW(LilyAstExprBinary, op, lhs, rhs)));
    }

    LilyAstExpr *res = pop__Vec(stack);

    FREE(Vec, stack);

    return res;
}

#define PARSE_FUN_PARAM_CALL(CLEAN_UP)                                         \
    Vec *params = NEW(Vec); /* Vec<LilyAstExprFunParamCall*>* */               \
                                                                               \
    while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {                   \
        switch (self->current->kind) {                                         \
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {                          \
                LilyToken *peeked = peek_token__LilyParseBlock(self, 1);       \
                                                                               \
                switch (peeked->kind) {                                        \
                    case LILY_TOKEN_KIND_COLON_EQ: {                           \
                        Location location_param =                              \
                          clone__Location(&self->current->location);           \
                        String *name =                                         \
                          clone__String(self->current->identifier_normal);     \
                                                                               \
                        jump__LilyParseBlock(self, 2);                         \
                                                                               \
                        /* Parse param expression */                           \
                        LilyAstExpr *value = parse_expr__LilyParseBlock(self); \
                                                                               \
                        if (!value) {                                          \
                            CLEAN_UP();                                        \
                                                                               \
                            return NULL;                                       \
                        }                                                      \
                                                                               \
                        END_LOCATION(&location_param, value->location);        \
                        push__Vec(params,                                      \
                                  NEW_VARIANT(LilyAstExprFunParamCall,         \
                                              default_,                        \
                                              value,                           \
                                              location_param,                  \
                                              name));                          \
                                                                               \
                        break;                                                 \
                    }                                                          \
                    default:                                                   \
                        goto parse_param_normal;                               \
                }                                                              \
                                                                               \
                break;                                                         \
            }                                                                  \
            default: {                                                         \
            parse_param_normal: {                                              \
                Location location_param =                                      \
                  clone__Location(&self->current->location);                   \
                LilyAstExpr *value = parse_expr__LilyParseBlock(self);         \
                                                                               \
                if (!value) {                                                  \
                    CLEAN_UP();                                                \
                                                                               \
                    return NULL;                                               \
                }                                                              \
                                                                               \
                END_LOCATION(&location_param, value->location);                \
                push__Vec(                                                     \
                  params,                                                      \
                  NEW_VARIANT(                                                 \
                    LilyAstExprFunParamCall, normal, value, location_param));  \
                                                                               \
                break;                                                         \
            }                                                                  \
            }                                                                  \
        }                                                                      \
                                                                               \
        CHECK_COMMA(LILY_TOKEN_KIND_R_PAREN);                                  \
    }

LilyAstExpr *
parse_fun_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
#define CLEAN_UP_FUN_CALL()                                                  \
    FREE(LilyAstExpr, id);                                                   \
    FREE_BUFFER_ITEMS(params->buffer, params->len, LilyAstExprFunParamCall); \
    FREE(Vec, params);

    next_token__LilyParseBlock(self); // skip `(` or `::`

    Location location = clone__Location(&id->location);

    PARSE_FUN_PARAM_CALL(CLEAN_UP_FUN_CALL);

    END_LOCATION(&location, self->current->location);
    next_token__LilyParseBlock(self); // skip `)`

    return NEW_VARIANT(
      LilyAstExpr,
      call,
      location,
      NEW_VARIANT(LilyAstExprCall, fun, NEW(LilyAstExprCallFun, id, params)));
}

LilyAstExpr *
parse_record_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
    next_token__LilyParseBlock(self); // skip `{`

    Vec *params = NEW(Vec); // Vec<LilyAstExprRecordParamCall*>*
    Location location = clone__Location(&id->location);

    while (self->current->kind != LILY_TOKEN_KIND_R_BRACE) {
        String *name = NULL;

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                name = clone__String(self->current->identifier_normal);
                next_token__LilyParseBlock(self);
                break;
            default:
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                    NULL,
                    NULL,
                    NULL),
                  self->count_error);

                name = from__String("__error__");
        }

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_COLON_EQ:
                next_token__LilyParseBlock(self);
                break;
            default:
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                              NULL,
                              NULL,
                              from__String("expected `:=`")),
                  self->count_error);

                break;
        }

        LilyAstExpr *value = parse_expr__LilyParseBlock(self);

        if (!value) {
            FREE(String, name);
            FREE_BUFFER_ITEMS(
              params->buffer, params->len, LilyAstExprRecordParamCall);
            FREE(Vec, params);

            return NULL;
        }

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_R_BRACE:
                break;
            default:
                if (self->current->kind != LILY_TOKEN_KIND_COMMA) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `,`")),
                      self->count_error);
                } else {
                    next_token__LilyParseBlock(self);
                }

                break;
        }

        push__Vec(params, NEW(LilyAstExprRecordParamCall, name, value));
    }

    END_LOCATION(&location, self->current->location);
    next_token__LilyParseBlock(self); // skip `}`

    return NEW_VARIANT(LilyAstExpr,
                       call,
                       location,
                       NEW_VARIANT(LilyAstExprCall,
                                   record,
                                   NEW(LilyAstExprCallRecord, id, params)));
}

LilyAstExpr *
parse_variant_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
    Location location = clone__Location(&id->location);

    next_token__LilyParseBlock(self); // skip `:`

    LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

    if (!expr) {
        FREE(LilyAstExpr, id);

        return NULL;
    }

    END_LOCATION(&location, expr->location);

    return NEW_VARIANT(LilyAstExpr,
                       call,
                       location,
                       NEW_VARIANT(LilyAstExprCall,
                                   variant,
                                   NEW(LilyAstExprCallVariant, id, expr)));
}

LilyAstExpr *
parse_call_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_BRACE:
            return parse_record_call__LilyParseBlock(self, id);
        case LILY_TOKEN_KIND_L_PAREN:
            return parse_fun_call__LilyParseBlock(self, id);
        case LILY_TOKEN_KIND_COLON:
            return parse_variant_call__LilyParseBlock(self, id);
        default: {
            String *token_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String("expected `{`, `(`, `::`, `:` or `:$`")),
              self->count_error);

            FREE(String, token_s);

            return NULL;
        }
    }
}

Vec *
parse_lambda_params__LilyParser(LilyParser *self, const Vec *pre_params)
{
    Vec *params = NEW(Vec);

    for (Usize i = 0; i < pre_params->len; ++i) {
        LilyParseBlock block =
          NEW(LilyParseBlock, self, get__Vec(pre_params, i));

        LilyAstExprLambdaParam *param = NULL;

        Location location = clone__Location(&block.current->location);

        // Get name of the param.
        String *name = NULL;

        switch (block.current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                name = clone__String(block.current->identifier_normal);
                next_token__LilyParseBlock(&block);

                break;
            case LILY_TOKEN_KIND_IDENTIFIER_STRING:
                name = clone__String(block.current->identifier_string);
                next_token__LilyParseBlock(&block);

                break;
            default:
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    block.file,
                    &block.current->location,
                    NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                    NULL,
                    NULL,
                    NULL),
                  block.count_error);

                name = from__String("__error__");
        }

        switch (block.current->kind) {
            case LILY_TOKEN_KIND_COLON_EQ: {
                next_token__LilyParseBlock(&block);

                LilyAstExpr *expr = parse_expr__LilyParseBlock(&block);

                CHECK_EXPR(expr, block, NULL, "expected `,`", {});

                END_LOCATION(&location, expr->location);

                param = NEW_VARIANT(
                  LilyAstExprLambdaParam, default, location, name, NULL, expr);

                break;
            }
            case LILY_TOKEN_KIND_EOF: {
                END_LOCATION(&location, block.current->location);

                param = NEW_VARIANT(
                  LilyAstExprLambdaParam, normal, location, name, NULL);

                break;
            }
            default: {
                LilyAstDataType *data_type =
                  parse_data_type__LilyParseBlock(&block);

                switch (block.current->kind) {
                    case LILY_TOKEN_KIND_COLON_EQ: {
                        next_token__LilyParseBlock(&block);

                        LilyAstExpr *expr = parse_expr__LilyParseBlock(&block);

                        CHECK_EXPR(expr, block, NULL, "expected `,`", {});

                        END_LOCATION(&location, block.current->location);

                        param = NEW_VARIANT(LilyAstExprLambdaParam,
                                            default,
                                            location,
                                            name,
                                            data_type,
                                            expr);

                        break;
                    }
                    case LILY_TOKEN_KIND_EOF: {
                        END_LOCATION(&location, block.current->location);

                        param = NEW_VARIANT(LilyAstExprLambdaParam,
                                            normal,
                                            location,
                                            name,
                                            data_type);

                        break;
                    }
                    default: {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            block.file,
                            &block.current->location,
                            NEW(LilyError,
                                LILY_ERROR_KIND_EXPECTED_ONLY_ONE_DATA_TYPE),
                            NULL,
                            NULL,
                            from__String("expected `,`")),
                          block.count_error);

                        END_LOCATION(&location, block.current->location);

                        param = NEW_VARIANT(LilyAstExprLambdaParam,
                                            normal,
                                            location,
                                            name,
                                            data_type);
                    }
                }
            }
        }

        push__Vec(params, param);

        FREE(LilyParseBlock, &block);
    }

    return params;
}

Vec *
parse_lambda_params_call__LilyParser(LilyParser *self,
                                     const Vec *pre_params_call)
{
    Vec *params = NEW(Vec);

    // 1. Parse params call.
    for (Usize i = 0; i < pre_params_call->len; ++i) {
        LilyParseBlock block =
          NEW(LilyParseBlock, self, get__Vec(pre_params_call, i));

        String *name = NULL;
        Location location = clone__Location(&block.current->location);
        LilyToken *peeked = peek_token__LilyParseBlock(&block, 1);

        if (peeked) {
            switch (peeked->kind) {
                case LILY_TOKEN_KIND_COLON_EQ:
                    switch (block.current->kind) {
                        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                            name =
                              clone__String(block.current->identifier_normal);
                            next_token__LilyParseBlock(&block);

                            break;
                        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
                            name =
                              clone__String(block.current->identifier_string);
                            next_token__LilyParseBlock(&block);

                            break;
                        default:
                            break;
                    }

                    break;
                default:
                    break;
            }
        }

        if (name) {
            switch (block.current->kind) {
                case LILY_TOKEN_KIND_COLON_EQ:
                    next_token__LilyParseBlock(&block);
                    break;
                default:
                    UNREACHABLE("this way is impossible");
            }

            LilyAstExpr *expr = parse_expr__LilyParseBlock(&block);

            CHECK_EXPR(expr, block, NULL, "expected `,`", {
                FREE(String, name);
                continue;
            });

            END_LOCATION(&location, block.current->location);

            push__Vec(
              params,
              NEW_VARIANT(
                LilyAstExprLambdaParamCall, default, expr, location, name));
        } else {
            LilyAstExpr *expr = parse_expr__LilyParseBlock(&block);

            CHECK_EXPR(expr, block, NULL, "expected `,`", { continue; });

            push__Vec(
              params,
              NEW_VARIANT(
                LilyAstExprLambdaParamCall, normal, expr, expr->location));
        }

        FREE(LilyParseBlock, &block);
    }

    return params;
}

LilyAstExpr *
parse_lambda_expr__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *lambda)
{
    // 1. Parse lambda params.
    Vec *params = NULL;

    if (lambda->lambda.params) {
        params = parse_lambda_params__LilyParser(self, lambda->lambda.params);
    }

    // 2. Parse return_data_type
    LilyAstDataType *return_data_type = NULL;

    if (lambda->lambda.return_data_type) {
        LilyParseBlock block_return_data_type =
          NEW(LilyParseBlock, self, lambda->lambda.return_data_type);

        return_data_type =
          parse_data_type__LilyParseBlock(&block_return_data_type);

        FREE(LilyParseBlock, &block_return_data_type);
    }

    // 3. Parse fun body item
    LilyAstBodyFunItem *item =
      parse_fun_body_item_for_stmt__LilyParser(self, lambda->lambda.item);

    // 4. Parse params call.
    Vec *params_call = NULL;

    if (lambda->lambda.params_call) {
        params_call = parse_lambda_params_call__LilyParser(
          self, lambda->lambda.params_call);
    }

    return NEW_VARIANT(LilyAstExpr,
                       lambda,
                       lambda->location,
                       NEW(LilyAstExprLambda,
                           lambda->lambda.name,
                           params,
                           return_data_type,
                           item,
                           params_call));
}

LilyAstExpr *
parse_list_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);
    Vec *exprs = NEW(Vec);

    EXPR_PARSE_CLOSING(exprs, LILY_TOKEN_KIND_R_BRACE, (&location));

    return NEW_VARIANT(
      LilyAstExpr, list, location, NEW(LilyAstExprList, exprs));
}

#define VARIANT_LITERAL(name, variant, value) \
    NEW_VARIANT(LilyAst##name,                \
                literal,                      \
                self->previous->location,     \
                NEW_VARIANT(LilyAst##name##Literal, variant, value))

#define LITERAL(name, kind)               \
    NEW_VARIANT(LilyAst##name,            \
                literal,                  \
                self->previous->location, \
                NEW(LilyAst##name##Literal, kind))

#define PARSE_LITERAL(name, name_u)                                           \
    switch (self->previous->kind) {                                           \
        case LILY_TOKEN_KIND_KEYWORD_TRUE:                                    \
            return VARIANT_LITERAL(name, bool_, true);                        \
        case LILY_TOKEN_KIND_KEYWORD_FALSE:                                   \
            return VARIANT_LITERAL(name, bool_, false);                       \
        case LILY_TOKEN_KIND_KEYWORD_NIL:                                     \
            return LITERAL(name, LILY_AST_##name_u##_LITERAL_KIND_NIL);       \
        case LILY_TOKEN_KIND_KEYWORD_NONE:                                    \
            return LITERAL(name, LILY_AST_##name_u##_LITERAL_KIND_NONE);      \
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:                                   \
            return LITERAL(name, LILY_AST_##name_u##_LITERAL_KIND_UNDEF);     \
        case LILY_TOKEN_KIND_LITERAL_BYTE:                                    \
            return VARIANT_LITERAL(name, byte, self->previous->literal_byte); \
        case LILY_TOKEN_KIND_LITERAL_BYTES: {                                 \
            Usize size =                                                      \
              strlen((const char *)self->previous->literal_bytes) + 1;        \
            Uint8 *bytes_copy = lily_malloc(size);                            \
                                                                              \
            memcpy(bytes_copy, self->previous->literal_bytes, size);          \
                                                                              \
            return VARIANT_LITERAL(name, bytes, bytes_copy);                  \
        }                                                                     \
        case LILY_TOKEN_KIND_LITERAL_CHAR:                                    \
            return VARIANT_LITERAL(name, char, self->previous->literal_char); \
        case LILY_TOKEN_KIND_LITERAL_CSTR: {                                  \
            Usize size = strlen(self->previous->literal_cstr) + 1;            \
            char *cstr_copy = lily_malloc(size);                              \
                                                                              \
            memcpy(cstr_copy, self->previous->literal_cstr, size);            \
                                                                              \
            return VARIANT_LITERAL(name, cstr, cstr_copy);                    \
        }                                                                     \
        case LILY_TOKEN_KIND_LITERAL_FLOAT:                                   \
            /* TODO: Check if the float is overflow/underflow.                \
            Update `base/atof.h` header. */                                   \
            return VARIANT_LITERAL(                                           \
              name,                                                           \
              float64,                                                        \
              atof__Float64(self->previous->literal_float->buffer));          \
        case LILY_TOKEN_KIND_LITERAL_INT_2:                                   \
        case LILY_TOKEN_KIND_LITERAL_INT_8:                                   \
        case LILY_TOKEN_KIND_LITERAL_INT_10:                                  \
        case LILY_TOKEN_KIND_LITERAL_INT_16: {                                \
            int base = 10;                                                    \
            String *literal_int = NULL;                                       \
                                                                              \
            switch (self->previous->kind) {                                   \
                case LILY_TOKEN_KIND_LITERAL_INT_2:                           \
                    base = 2;                                                 \
                    literal_int = self->previous->literal_int_2;              \
                    break;                                                    \
                case LILY_TOKEN_KIND_LITERAL_INT_8:                           \
                    base = 8;                                                 \
                    literal_int = self->previous->literal_int_8;              \
                    break;                                                    \
                case LILY_TOKEN_KIND_LITERAL_INT_16:                          \
                    base = 16;                                                \
                    literal_int = self->previous->literal_int_16;             \
                    break;                                                    \
                default:                                                      \
                    literal_int = self->previous->literal_int_10;             \
                    break;                                                    \
            }                                                                 \
                                                                              \
            /* try to cast to Int32 */                                        \
            Optional *op = atoi_safe__Int32(literal_int->buffer, base);       \
                                                                              \
            if (is_some__Optional(op)) {                                      \
                Int32 int32 = (Int32)(Uptr)get__Optional(op);                 \
                                                                              \
                FREE(Optional, op);                                           \
                                                                              \
                return VARIANT_LITERAL(name, int32, int32);                   \
            } else {                                                          \
                FREE(Optional, op);                                           \
                                                                              \
                /* try to cast to Int64 */                                    \
                op = atoi_safe__Int64(literal_int->buffer, base);             \
                                                                              \
                if (is_some__Optional(op)) {                                  \
                    Int64 int64 = (Int64)(Uptr)get__Optional(op);             \
                                                                              \
                    FREE(Optional, op);                                       \
                                                                              \
                    return VARIANT_LITERAL(name, int64, int64);               \
                } else {                                                      \
                    FREE(Optional, op);                                       \
                                                                              \
                    emit__Diagnostic(                                         \
                      NEW_VARIANT(                                            \
                        Diagnostic,                                           \
                        simple_lily_error,                                    \
                        self->file,                                           \
                        &self->previous->location,                            \
                        NEW(LilyError,                                        \
                            LILY_ERROR_KIND_FEATURE_NOT_YET_SUPPORTED),       \
                        NULL,                                                 \
                        NULL,                                                 \
                        from__String(                                         \
                          "Int128 and Uint128 data type not yet supported")), \
                      self->count_error);                                     \
                                                                              \
                    return NULL;                                              \
                }                                                             \
            }                                                                 \
        }                                                                     \
        case LILY_TOKEN_KIND_LITERAL_STR:                                     \
            return VARIANT_LITERAL(                                           \
              name, str, clone__String(self->previous->literal_str));         \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:                          \
            return VARIANT_LITERAL(                                           \
              name, suffix_float32, self->previous->literal_suffix_float32);  \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:                          \
            return VARIANT_LITERAL(                                           \
              name, suffix_float64, self->previous->literal_suffix_float64);  \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:                            \
            return VARIANT_LITERAL(                                           \
              name, suffix_int16, self->previous->literal_suffix_int16);      \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:                            \
            return VARIANT_LITERAL(                                           \
              name, suffix_int32, self->previous->literal_suffix_int32);      \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:                            \
            return VARIANT_LITERAL(                                           \
              name, suffix_int64, self->previous->literal_suffix_int64);      \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:                             \
            return VARIANT_LITERAL(                                           \
              name, suffix_int8, self->previous->literal_suffix_int8);        \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:                            \
            return VARIANT_LITERAL(                                           \
              name, suffix_isize, self->previous->literal_suffix_isize);      \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:                            \
            return VARIANT_LITERAL(                                           \
              name, suffix_uint8, self->previous->literal_suffix_uint8);      \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:                           \
            return VARIANT_LITERAL(                                           \
              name, suffix_uint16, self->previous->literal_suffix_uint16);    \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:                           \
            return VARIANT_LITERAL(                                           \
              name, suffix_uint32, self->previous->literal_suffix_uint32);    \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:                           \
            return VARIANT_LITERAL(                                           \
              name, suffix_uint64, self->previous->literal_suffix_uint64);    \
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:                            \
            return VARIANT_LITERAL(                                           \
              name, suffix_usize, self->previous->literal_suffix_usize);      \
        default:                                                              \
            UNREACHABLE("this way is impossible");                            \
    }                                                                         \
                                                                              \
    return NULL;

LilyAstExpr *
parse_literal_expr__LilyParseBlock(LilyParseBlock *self)
{
    PARSE_LITERAL(Expr, EXPR);
}

LilyAstExpr *
parse_only_identifier_expr__LilyParseBlock(LilyParseBlock *self)
{
#define PARSE_ONLY_IDENTIFIER(token_kind)                                      \
    {                                                                          \
        Location *prev_location = &self->previous->location;                   \
        String *prev_id = self->previous->token_kind;                          \
                                                                               \
        if (!strcmp(prev_id->buffer, "_")) {                                   \
            emit__Diagnostic(                                                  \
              NEW_VARIANT(Diagnostic,                                          \
                          simple_lily_error,                                   \
                          self->file,                                          \
                          prev_location,                                       \
                          NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_WILDCARD), \
                          NULL,                                                \
                          NULL,                                                \
                          from__String("expected identifier")),                \
              self->count_error);                                              \
        }                                                                      \
                                                                               \
        if (self->current->kind == LILY_TOKEN_KIND_COLON_COLON) {              \
            Vec *generic_params = NULL; /* Vec<LilyAstDataType*>*? */          \
                                                                               \
            next_token__LilyParseBlock(self); /* skip `::` */                  \
                                                                               \
            switch (self->current->kind) {                                     \
                case LILY_TOKEN_KIND_L_HOOK:                                   \
                    generic_params = NEW(Vec);                                 \
                                                                               \
                    next_token__LilyParseBlock(self); /* skip `[` */           \
                                                                               \
                    DATA_TYPE_PARSE_CLOSING(                                   \
                      generic_params, LILY_TOKEN_KIND_R_HOOK, NULL);           \
                                                                               \
                    break;                                                     \
                default:                                                       \
                    emit__Diagnostic(                                          \
                      NEW_VARIANT(                                             \
                        Diagnostic,                                            \
                        simple_lily_error,                                     \
                        self->file,                                            \
                        &self->current->location,                              \
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),        \
                        NULL,                                                  \
                        NULL,                                                  \
                        from__String("expected `[`, after `::`")),             \
                      self->count_error);                                      \
            }                                                                  \
                                                                               \
            return NEW_VARIANT(LilyAstExpr,                                    \
                               identifier,                                     \
                               clone__Location(prev_location),                 \
                               NEW(LilyAstExprIdentifier,                      \
                                   clone__String(prev_id),                     \
                                   generic_params));                           \
        }                                                                      \
                                                                               \
        return NEW_VARIANT(                                                    \
          LilyAstExpr,                                                         \
          identifier,                                                          \
          clone__Location(prev_location),                                      \
          NEW(LilyAstExprIdentifier, clone__String(prev_id), NULL));           \
    }

    next_token__LilyParseBlock(self);

    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            PARSE_ONLY_IDENTIFIER(identifier_normal);
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            PARSE_ONLY_IDENTIFIER(identifier_string);
        default:
            UNREACHABLE("this situation is impossible");
    }
}

LilyAstExpr *
parse_access_expr_for_pattern__LilyParseBlock(LilyParseBlock *self)
{
    enum LilyTokenKind kind = self->previous->kind;
    enum LilyAstExprAccessKind access_kind;

    next_token__LilyParseBlock(self); // skip `.`

    switch (kind) {
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
            access_kind = LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH;
            break;
        case LILY_TOKEN_KIND_KEYWORD_SELF:
            access_kind = LILY_AST_EXPR_ACCESS_KIND_SELF_PATH;
            break;
        default:
            UNREACHABLE("this situation is impossible");
    }

    LilyAstExpr *path =
      parse_path_access_for_pattern__LilyParseBlock(self, NULL, access_kind);

    if (path) {
        return path;
    }

    return NULL;
}

LilyAstExpr *
parse_path_access_for_pattern__LilyParseBlock(LilyParseBlock *self,
                                              LilyAstExpr *begin,
                                              enum LilyAstExprAccessKind kind)
{
    LilyAstExpr *current_access = NULL;
    Vec *current_path = NULL;
    Location *current_location = NULL;

    if (begin) {
        if (self->current->kind != LILY_TOKEN_KIND_DOT) {
            return begin;
        }

        ASSERT(kind == LILY_AST_EXPR_ACCESS_KIND_PATH);

        current_access = NEW_VARIANT(
          LilyAstExpr,
          access,
          clone__Location(&begin->location),
          NEW_VARIANT(LilyAstExprAccess, path, init__Vec(1, begin)));
        current_location = &current_access->location;
        current_path = current_access->access.path;
    } else {
        ASSERT(kind == LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH ||
               kind == LILY_AST_EXPR_ACCESS_KIND_SELF_PATH);

        Location *prev_location = &self->previous->location;

        LilyAstExpr *first_expr =
          parse_only_identifier_expr__LilyParseBlock(self);
        Location expr_location = clone__Location(&first_expr->location);

        START_LOCATION(&expr_location, (*prev_location));

        switch (kind) {
            case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, global_path, NEW(Vec)));

                break;
            case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, Self_path, NEW(Vec)));

                break;
            case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
                current_access = NEW_VARIANT(
                  LilyAstExpr,
                  access,
                  expr_location,
                  NEW_VARIANT(LilyAstExprAccess, self_path, NEW(Vec)));

                break;
            default:
                UNREACHABLE("this situation is impossible");
        }

        current_path = get_path__LilyAstExprAccess(&current_access->access);
        current_location = &current_access->location;

        push__Vec(current_path, first_expr);

        if (self->current->kind != LILY_TOKEN_KIND_DOT) {
            return current_access;
        }
    }

    while (self->current->kind == LILY_TOKEN_KIND_DOT) {
        next_token__LilyParseBlock(self); // skip `.`

        LilyAstExpr *expr = parse_only_identifier_expr__LilyParseBlock(self);

        if (!expr) {
            FREE(LilyAstExpr, current_access);

            return NULL;
        }

        push__Vec(current_path, expr);
    }

    END_LOCATION(current_location, self->previous->location);

    return current_access;
}

LilyAstExpr *
parse_primary_expr__LilyParseBlock(LilyParseBlock *self, bool not_parse_access)
{
    next_token__LilyParseBlock(self);

    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
        case LILY_TOKEN_KIND_KEYWORD_NIL:
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
        case LILY_TOKEN_KIND_KEYWORD_NONE:
        case LILY_TOKEN_KIND_LITERAL_BYTE:
        case LILY_TOKEN_KIND_LITERAL_BYTES:
        case LILY_TOKEN_KIND_LITERAL_CHAR:
        case LILY_TOKEN_KIND_LITERAL_CSTR:
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
        case LILY_TOKEN_KIND_LITERAL_INT_2:
        case LILY_TOKEN_KIND_LITERAL_INT_8:
        case LILY_TOKEN_KIND_LITERAL_INT_10:
        case LILY_TOKEN_KIND_LITERAL_INT_16:
        case LILY_TOKEN_KIND_LITERAL_STR:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            return parse_literal_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_L_HOOK:
            return parse_array_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_L_BRACE:
            return parse_list_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_L_PAREN: {
            LilyToken *peeked = self->current;

            switch (peeked->kind) {
                case LILY_TOKEN_KIND_R_PAREN: {
                    Location location =
                      clone__Location(&self->current->location);

                    next_token__LilyParseBlock(self);
                    END_LOCATION(&location, self->previous->location);

                    return NEW_VARIANT(
                      LilyAstExpr,
                      literal,
                      location,
                      NEW(LilyAstExprLiteral, LILY_AST_EXPR_LITERAL_KIND_UNIT));
                }
                default:
                    break;
            }

            for (Usize i = 1; peeked; ++i) {
                if (peeked->kind == LILY_TOKEN_KIND_COMMA) {
                    return parse_tuple_expr__LilyParseBlock(self);
                } else if (peeked->kind == LILY_TOKEN_KIND_R_PAREN) {
                    Location location =
                      clone__Location(&self->previous->location);
                    LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

                    if (!expr) {
                        return NULL;
                    }

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_R_PAREN:
                            END_LOCATION(&location, self->current->location);
                            next_token__LilyParseBlock(self);

                            return NEW_VARIANT(
                              LilyAstExpr, grouping, location, expr);
                        default:
                            // ERROR: expected `)`
                            return NULL;
                    }
                }

                peeked = peek_token__LilyParseBlock(self, i);
            }
        }
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
        case LILY_TOKEN_KIND_KEYWORD_self:
        case LILY_TOKEN_KIND_KEYWORD_SELF:
        case LILY_TOKEN_KIND_AT:
            if (self->previous->kind == LILY_TOKEN_KIND_KEYWORD_self &&
                self->current->kind != LILY_TOKEN_KIND_DOT) {
                return NEW(LilyAstExpr,
                           clone__Location(&self->previous->location),
                           LILY_AST_EXPR_KIND_SELF);
            }

            if (not_parse_access) {
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_PATH),
                              NULL,
                              NULL,
                              NULL),
                  self->count_error);

                return NULL;
            }

            if (self->previous->kind == LILY_TOKEN_KIND_AT &&
                self->current->kind == LILY_TOKEN_KIND_IDENTIFIER_NORMAL) {
                return parse_at_access__LilyParseBlock(self, NULL);
            }

            if (self->current->kind == LILY_TOKEN_KIND_DOT) {
                return parse_access_expr__LilyParseBlock(self);
            } else {
                goto unexpected_token;
            }
        case LILY_TOKEN_KIND_KEYWORD_AT_LEN: {
            Location location = clone__Location(&self->previous->location);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_PAREN:
                    next_token__LilyParseBlock(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `(`")),
                      self->count_error);

                    return NULL;
            }

            LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

            if (!expr) {
                return NULL;
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_R_PAREN:
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyParseBlock(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `)`")),
                      self->count_error);

                    FREE(LilyAstExpr, expr);

                    return NULL;
            }

            return NEW_VARIANT(LilyAstExpr,
                               call,
                               location,
                               NEW_VARIANT(LilyAstExprCall, len, expr));
        }
        case LILY_TOKEN_KIND_KEYWORD_AT_BUILTIN:
        case LILY_TOKEN_KIND_KEYWORD_AT_SYS: {
            enum LilyTokenKind special_fun_call_kind = self->previous->kind;
            String *name = NULL;
            Location location = clone__Location(&self->previous->location);

#define CLEAN_UP_SPECIAL_FUN_CALL()                                          \
    FREE_BUFFER_ITEMS(params->buffer, params->len, LilyAstExprFunParamCall); \
    FREE(Vec, params);                                                       \
    FREE(String, name);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_DOT:
                    next_token__LilyParseBlock(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `.`")),
                      self->count_error);

                    return NULL;
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                    name = clone__String(self->current->identifier_normal);
                    next_token__LilyParseBlock(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                        NULL,
                        NULL,
                        from__String("expected `sys` function identifier")),
                      self->count_error);

                    return NULL;
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_PAREN:
                    next_token__LilyParseBlock(self); // skip `(`

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `(`")),
                      self->count_error);

                    FREE(String, name);

                    return NULL;
            }

            PARSE_FUN_PARAM_CALL(CLEAN_UP_SPECIAL_FUN_CALL);

            END_LOCATION(&location, self->current->location);
            next_token__LilyParseBlock(self); // skip `)`

            switch (special_fun_call_kind) {
                case LILY_TOKEN_KIND_KEYWORD_AT_BUILTIN:
                    return NEW_VARIANT(
                      LilyAstExpr,
                      call,
                      location,
                      NEW_VARIANT(
                        LilyAstExprCall,
                        fun_builtin,
                        NEW(LilyAstExprCallFunBuiltin, name, params)));
                case LILY_TOKEN_KIND_KEYWORD_AT_SYS:
                    return NEW_VARIANT(
                      LilyAstExpr,
                      call,
                      location,
                      NEW_VARIANT(LilyAstExprCall,
                                  fun_sys,
                                  NEW(LilyAstExprCallFunSys, name, params)));
                default:
                    UNREACHABLE("this situation is impossible");
            }
        }
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
#define PARSE_IDENTIFIER(token_kind)                                           \
    {                                                                          \
        Location *prev_location = &self->previous->location;                   \
        String *prev_id = self->previous->token_kind;                          \
                                                                               \
        if (!strcmp(prev_id->buffer, "_")) {                                   \
            return NEW(LilyAstExpr,                                            \
                       clone__Location(prev_location),                         \
                       LILY_AST_EXPR_KIND_WILDCARD);                           \
        }                                                                      \
                                                                               \
        if (self->current->kind == LILY_TOKEN_KIND_DOT && !not_parse_access) { \
            return parse_path_access__LilyParseBlock(                          \
              self,                                                            \
              NEW_VARIANT(                                                     \
                LilyAstExpr,                                                   \
                identifier,                                                    \
                clone__Location(prev_location),                                \
                NEW(LilyAstExprIdentifier, clone__String(prev_id), NULL)),     \
              LILY_AST_EXPR_ACCESS_KIND_PATH);                                 \
        } else if (self->current->kind == LILY_TOKEN_KIND_COLON_COLON) {       \
            Vec *generic_params = NULL; /* Vec<LilyAstDataType*>*? */          \
                                                                               \
            next_token__LilyParseBlock(self); /* skip `::` */                  \
                                                                               \
            switch (self->current->kind) {                                     \
                case LILY_TOKEN_KIND_L_HOOK:                                   \
                    generic_params = NEW(Vec);                                 \
                                                                               \
                    next_token__LilyParseBlock(self); /* skip `[` */           \
                                                                               \
                    DATA_TYPE_PARSE_CLOSING(                                   \
                      generic_params, LILY_TOKEN_KIND_R_HOOK, NULL);           \
                                                                               \
                    break;                                                     \
                default:                                                       \
                    emit__Diagnostic(                                          \
                      NEW_VARIANT(                                             \
                        Diagnostic,                                            \
                        simple_lily_error,                                     \
                        self->file,                                            \
                        &self->current->location,                              \
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),        \
                        NULL,                                                  \
                        NULL,                                                  \
                        from__String("expected `[`, after `::`")),             \
                      self->count_error);                                      \
            }                                                                  \
                                                                               \
            LilyAstExpr *id = NEW_VARIANT(LilyAstExpr,                         \
                                          identifier,                          \
                                          clone__Location(prev_location),      \
                                          NEW(LilyAstExprIdentifier,           \
                                              clone__String(prev_id),          \
                                              generic_params));                \
                                                                               \
            switch (self->current->kind) {                                     \
                case LILY_TOKEN_KIND_L_PAREN:                                  \
                    return parse_fun_call__LilyParseBlock(self, id);           \
                case LILY_TOKEN_KIND_L_BRACE:                                  \
                    return parse_record_call__LilyParseBlock(self, id);        \
                default:                                                       \
                    return id;                                                 \
            }                                                                  \
        }                                                                      \
                                                                               \
        LilyAstExpr *id = NEW_VARIANT(                                         \
          LilyAstExpr,                                                         \
          identifier,                                                          \
          clone__Location(prev_location),                                      \
          NEW(LilyAstExprIdentifier, clone__String(prev_id), NULL));           \
                                                                               \
        switch (self->current->kind) {                                         \
            case LILY_TOKEN_KIND_L_PAREN:                                      \
                return parse_fun_call__LilyParseBlock(self, id);               \
            case LILY_TOKEN_KIND_L_BRACE:                                      \
                return parse_record_call__LilyParseBlock(self, id);            \
            case LILY_TOKEN_KIND_L_HOOK:                                       \
                return parse_hook_access__LilyParseBlock(self, id);            \
            case LILY_TOKEN_KIND_COLON:                                        \
                return parse_variant_call__LilyParseBlock(self, id);           \
            default:                                                           \
                return id;                                                     \
        }                                                                      \
    }
            PARSE_IDENTIFIER(identifier_string);
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            PARSE_IDENTIFIER(identifier_normal);
        case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            return NEW_VARIANT(
              LilyAstExpr,
              identifier_dollar,
              clone__Location(&self->previous->location),
              NEW(LilyAstExprIdentifierDollar,
                  clone__String(self->previous->identifier_dollar)));
        case LILY_TOKEN_KIND_KEYWORD_REF:
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
        case LILY_TOKEN_KIND_INTERROGATION:
        case LILY_TOKEN_KIND_MINUS:
        case LILY_TOKEN_KIND_KEYWORD_NOT: {
            enum LilyAstExprUnaryKind op = 0;

            switch (self->previous->kind) {
                case LILY_TOKEN_KIND_MINUS:
                    op = LILY_AST_EXPR_UNARY_KIND_NEG;
                    break;
                case LILY_TOKEN_KIND_KEYWORD_NOT:
                    op = LILY_AST_EXPR_UNARY_KIND_NOT;
                    break;
                case LILY_TOKEN_KIND_INTERROGATION:
                    op = LILY_AST_EXPR_UNARY_KIND_OPTIONAL;
                    break;
                case LILY_TOKEN_KIND_KEYWORD_REF: {
                    if (self->current->kind == LILY_TOKEN_KIND_KEYWORD_MUT) {
                        next_token__LilyParseBlock(self);

                        op = LILY_AST_EXPR_UNARY_KIND_REF_MUT;
                    } else {
                        op = LILY_AST_EXPR_UNARY_KIND_REF;
                    }

                    break;
                }
                case LILY_TOKEN_KIND_KEYWORD_TRACE: {
                    if (self->current->kind == LILY_TOKEN_KIND_KEYWORD_MUT) {
                        next_token__LilyParseBlock(self);

                        op = LILY_AST_EXPR_UNARY_KIND_TRACE_MUT;
                    } else {
                        op = LILY_AST_EXPR_UNARY_KIND_TRACE;
                    }

                    break;
                }
                default:
                    UNREACHABLE("this way is impossible");
            }

            Location location = clone__Location(&self->previous->location);
            LilyAstExpr *right = parse_expr__LilyParseBlock(self);

            if (!right) {
                return NULL;
            }

            END_LOCATION(&location, right->location);

            return NEW_VARIANT(
              LilyAstExpr, unary, location, NEW(LilyAstExprUnary, op, right));
        }
        case LILY_TOKEN_KIND_EXPAND: {
            switch (self->previous->expand.kind) {
                case LILY_TOKEN_EXPAND_KIND_EXPR: {
                    LilyParseBlock expr_block =
                      NEW(LilyParseBlock,
                          self->parser,
                          self->previous->expand.tokens);

                    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

                    CHECK_EXPR(
                      expr, expr_block, NULL, "expected only one expression", {
                          FREE(LilyParseBlock, &expr_block);
                          return NULL;
                      });

                    FREE(LilyParseBlock, &expr_block);

                    return expr;
                }
                case LILY_TOKEN_EXPAND_KIND_PATH: {
                    LilyParseBlock expr_block =
                      NEW(LilyParseBlock,
                          self->parser,
                          self->previous->expand.tokens);

                    LilyAstExpr *path = NULL;

                    switch (expr_block.current->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
                        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
                        case LILY_TOKEN_KIND_KEYWORD_SELF:
                        case LILY_TOKEN_KIND_KEYWORD_self:
                            path =
                              parse_access_expr__LilyParseBlock(&expr_block);

                            break;
                        default:
                            path = parse_path_access__LilyParseBlock(
                              self, NULL, LILY_AST_EXPR_ACCESS_KIND_PATH);
                    }

                    CHECK_EXPR(
                      path, expr_block, NULL, "expected only one path", {
                          FREE(LilyParseBlock, &expr_block);

                          return NULL;
                      });

                    FREE(LilyParseBlock, &expr_block);

                    // Verify if is a path
                    switch (path->kind) {
                        case LILY_AST_EXPR_KIND_ACCESS:
                            switch (path->access.kind) {
                                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                                case LILY_AST_EXPR_ACCESS_KIND_PATH:
                                case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
                                case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
                                    break;
                                default:
                                    goto expected_path;
                            }

                            break;
                        default:
                        expected_path: {
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &path->location,
                                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_PATH),
                                NULL,
                                NULL,
                                NULL),
                              self->count_error);
                        }
                    }

                    return path;
                }
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->previous->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_EXPRESSION),
                        init__Vec(
                          1,
                          from__String("expected `expr` or `path` as the "
                                       "data type of the macro param")),
                        NULL,
                        NULL),
                      self->count_error);

                    return NULL;
            }
        }
        case LILY_TOKEN_KIND_KEYWORD_TRY: {
            Location location = clone__Location(&self->previous->location);
            LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

            if (!expr) {
                return NULL;
            }

            END_LOCATION(&location, self->previous->location);

            return NEW_VARIANT(
              LilyAstExpr, try, location, NEW(LilyAstExprTry, expr));
        }
        case LILY_TOKEN_KIND_SEMICOLON:
            return NULL;
        default: {
        unexpected_token: {
            String *previous_s = to_string__LilyToken(self->previous);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->previous->location,
                NEW_VARIANT(LilyError, unexpected_token, previous_s->buffer),
                NULL,
                NULL,
                NULL),
              self->count_error);

            FREE(String, previous_s);

            return NULL;
        }
        }
    }
}

LilyAstExpr *
parse_postfix_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *expr)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_DOT_STAR: {
            Location location = clone__Location(&expr->location);

            next_token__LilyParseBlock(self);
            END_LOCATION(&location, self->current->location);

            return parse_postfix_expr__LilyParseBlock(
              self,
              NEW_VARIANT(LilyAstExpr,
                          unary,
                          location,
                          NEW(LilyAstExprUnary,
                              LILY_AST_EXPR_UNARY_KIND_DEREFERENCE,
                              expr)));
        }
        case LILY_TOKEN_KIND_DOT:
            expr = parse_path_access__LilyParseBlock(
              self, expr, LILY_AST_EXPR_ACCESS_KIND_PATH);

            if (!expr) {
                return NULL;
            }

            return parse_postfix_expr__LilyParseBlock(self, expr);
        case LILY_TOKEN_KIND_AT:
            switch (expr->kind) {
                case LILY_AST_EXPR_KIND_ACCESS:
                    expr = parse_at_access__LilyParseBlock(self, expr);

                    if (!expr) {
                        return NULL;
                    }
                    return parse_postfix_expr__LilyParseBlock(self, expr);
                default:
                    return expr;
            }
        case LILY_TOKEN_KIND_L_HOOK:
            switch (expr->kind) {
                case LILY_AST_EXPR_KIND_ACCESS:
                case LILY_AST_EXPR_KIND_IDENTIFIER:
                    expr = parse_hook_access__LilyParseBlock(self, expr);

                    return parse_postfix_expr__LilyParseBlock(self, expr);
                default:
                    return expr;
            }
        case LILY_TOKEN_KIND_KEYWORD_CAST: {
            Location location = clone__Location(&expr->location);

            next_token__LilyParseBlock(self);

            LilyAstDataType *dest_data_type =
              parse_data_type__LilyParseBlock(self);

            if (!dest_data_type) {
                FREE(LilyAstExpr, expr);

                return NULL;
            }

            END_LOCATION(&location, self->current->location);

            expr = NEW_VARIANT(LilyAstExpr,
                               cast,
                               location,
                               NEW(LilyAstExprCast, expr, dest_data_type));

            return parse_postfix_expr__LilyParseBlock(self, expr);
        }
        case LILY_TOKEN_KIND_L_PAREN:
        case LILY_TOKEN_KIND_L_BRACE:
        case LILY_TOKEN_KIND_COLON:
            switch (expr->kind) {
                case LILY_AST_EXPR_KIND_ACCESS:
                case LILY_AST_EXPR_KIND_IDENTIFIER:
                    expr = parse_call_expr__LilyParseBlock(self, expr);

                    if (!expr) {
                        return NULL;
                    }

                    return parse_postfix_expr__LilyParseBlock(self, expr);
                default:
                    return expr;
            }
        default:
            return expr;
    }
}

LilyAstExpr *
parse_expr__LilyParseBlock(LilyParseBlock *self)
{
    LilyAstExpr *expr = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN: {
            LilyToken *peeked = peek_token__LilyParseBlock(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_KEYWORD_FUN: {
                        next_token__LilyParseBlock(self);

                        // Remove (fun ...)
                        //                ^
                        {
                            Usize position_r_paren = self->position;

                            for (; position_r_paren < self->tokens->len;) {
                                if (CAST(
                                      LilyToken *,
                                      get__Vec(self->tokens, position_r_paren))
                                      ->kind == LILY_TOKEN_KIND_L_PAREN) {
                                    ++position_r_paren;

                                    int count = 0;

                                    do {
                                        switch (CAST(LilyToken *,
                                                     get__Vec(self->tokens,
                                                              position_r_paren))
                                                  ->kind) {
                                            case LILY_TOKEN_KIND_L_PAREN:
                                                ++count;
                                                ++position_r_paren;

                                                break;
                                            case LILY_TOKEN_KIND_R_PAREN:
                                                --count;
                                                ++position_r_paren;

                                                break;
                                            default:
                                                ++position_r_paren;
                                        }
                                    } while (CAST(LilyToken *,
                                                  get__Vec(self->tokens,
                                                           position_r_paren))
                                                   ->kind !=
                                                 LILY_TOKEN_KIND_R_PAREN
                                               ? 1
                                               : count > 0);
                                    ++position_r_paren;
                                } else if (CAST(LilyToken *,
                                                get__Vec(self->tokens,
                                                         position_r_paren))
                                             ->kind ==
                                           LILY_TOKEN_KIND_R_PAREN) {
                                    break;
                                } else {
                                    ++position_r_paren;
                                }
                            }

                            remove__Vec(self->tokens, position_r_paren);
                        }

                        LilyPreparser preparser = NEW(
                          LilyPreparser, self->file, self->tokens, NULL, false);

                        preparser.position = self->position;
                        preparser.current =
                          get__Vec(preparser.tokens, preparser.position);

                        LilyPreparserFunBodyItem *lambda =
                          preparse_lambda__LilyPreparser(&preparser);

                        if (lambda) {
                            self->position = preparser.position;
                            self->current =
                              get__Vec(self->tokens, self->position);
                            self->previous =
                              get__Vec(self->tokens, self->position - 1);
                            *self->count_error += preparser.count_error;
                            *self->count_warning += preparser.count_warning;

                            expr = parse_lambda_expr__LilyParser(self->parser,
                                                                 lambda);

                            FREE(LilyPreparserFunBodyItem, lambda);
                        } else {
                            return NULL;
                        }

                        break;
                    }
                    default:
                        expr = parse_primary_expr__LilyParseBlock(self, false);
                }
            }

            break;
        }
        default:
            expr = parse_primary_expr__LilyParseBlock(self, false);
    }

    if (!expr) {
        return NULL;
    }

    expr = parse_postfix_expr__LilyParseBlock(self, expr);

    // parse binary expression
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_PLUS:
        case LILY_TOKEN_KIND_KEYWORD_AND:
        case LILY_TOKEN_KIND_PLUS_EQ:
        case LILY_TOKEN_KIND_AMPERSAND_EQ:
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ:
        case LILY_TOKEN_KIND_BAR_EQ:
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ:
        case LILY_TOKEN_KIND_SLASH_EQ:
        case LILY_TOKEN_KIND_STAR_STAR_EQ:
        case LILY_TOKEN_KIND_PERCENTAGE_EQ:
        case LILY_TOKEN_KIND_STAR_EQ:
        case LILY_TOKEN_KIND_MINUS_EQ:
        case LILY_TOKEN_KIND_XOR_EQ:
        case LILY_TOKEN_KIND_EQ:
        case LILY_TOKEN_KIND_AMPERSAND:
        case LILY_TOKEN_KIND_BAR:
        case LILY_TOKEN_KIND_BAR_R_SHIFT:
        case LILY_TOKEN_KIND_SLASH:
        case LILY_TOKEN_KIND_EQ_EQ:
        case LILY_TOKEN_KIND_STAR_STAR:
        case LILY_TOKEN_KIND_R_SHIFT_EQ:
        case LILY_TOKEN_KIND_R_SHIFT:
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT:
        case LILY_TOKEN_KIND_L_SHIFT_EQ:
        case LILY_TOKEN_KIND_L_SHIFT:
        case LILY_TOKEN_KIND_PERCENTAGE:
        case LILY_TOKEN_KIND_STAR:
        case LILY_TOKEN_KIND_NOT_EQ:
        case LILY_TOKEN_KIND_KEYWORD_OR:
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT:
        case LILY_TOKEN_KIND_DOT_DOT:
        case LILY_TOKEN_KIND_MINUS:
        case LILY_TOKEN_KIND_KEYWORD_XOR:
        case LILY_TOKEN_KIND_ARROW:
        case LILY_TOKEN_KIND_INVERSE_ARROW:
            return parse_binary_expr__LilyParseBlock(self, expr);
        default:
            return expr;
    }
}

LilyAstCapture *
parse_capture__LilyParseBlock(LilyParseBlock *self)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK: {
            Vec *multiple = NEW(Vec); // Vec<LilyAstCapture*?>*

            next_token__LilyParseBlock(self); // skip `[`

            while (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                LilyAstExpr *single = parse_expr__LilyParseBlock(self);

                if (!single) {
                    for (Usize i = 0; i < multiple->len; ++i) {
                        LilyAstCapture *capture = get__Vec(multiple, i);

                        if (capture) {
                            FREE(LilyAstCapture, capture);
                        }
                    }

                    FREE(Vec, multiple);

                    return NULL;
                }

                push__Vec(multiple, single);

                if (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_COMMA:
                            next_token__LilyParseBlock(self);
                            break;
                        default:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->parser->package->file,
                                &self->current->location,
                                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                                NULL,
                                NULL,
                                from__String("expected `,`")),
                              &self->parser->package->count_error);
                    }
                }
            }

            next_token__LilyParseBlock(self); // skip `]`

            if (!HAS_REACHED_THE_END((*self))) {
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              &self->parser->package->file,
                              &self->current->location,
                              NEW(LilyError,
                                  LILY_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION),
                              NULL,
                              NULL,
                              from__String("expected `do`")),
                  &self->parser->package->count_error);
            }

            switch (multiple->len) {
                case 1:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_warning,
                        self->file,
                        &self->current->location,
                        NEW(LilyWarning,
                            LILY_WARNING_KIND_UNUSED_MULTIPLE_CAPTURE),
                        NULL,
                        NULL,
                        NULL),
                      self->count_warning);

                    return NULL;
                case 0:
                    FREE(Vec, multiple);

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_warning,
                        self->file,
                        &self->current->location,
                        NEW(LilyWarning, LILY_WARNING_KIND_UNUSED_CAPTURE),
                        NULL,
                        NULL,
                        NULL),
                      self->count_warning);

                    return NULL;
                default:
                    return NEW_VARIANT(LilyAstCapture, multiple, multiple);
            }
        }
        default: {
            LilyAstExpr *single = parse_expr__LilyParseBlock(self);

            CHECK_EXPR(
              single, (*self), NULL, "expected `do`", { return NULL; });

            return NEW_VARIANT(LilyAstCapture, single, single);
        }
    }
}

LilyAstBodyFunItem *
parse_asm_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item)
{
    if (item->stmt_asm.params->len > 0) {
        // 1. Parse first expression
        LilyAstExpr *value = NULL;

        {
            LilyParseBlock value_block =
              NEW(LilyParseBlock, self, get__Vec(item->stmt_asm.params, 0));
            value = parse_expr__LilyParseBlock(&value_block);

            FREE(LilyParseBlock, &value_block);

            if (!value) {
                return NULL;
            }
        }

        // 2. Parse all other params
        Vec *params = NEW(Vec); // Vec<LilyAstExpr*>*

        for (Usize i = 1; i < item->stmt_asm.params->len; ++i) {
            LilyParseBlock param_block =
              NEW(LilyParseBlock, self, get__Vec(item->stmt_asm.params, i));
            LilyAstExpr *param = parse_expr__LilyParseBlock(&param_block);

            CHECK_EXPR(param, param_block, NULL, "expected `,`", {
                FREE(LilyAstExpr, value);
                FREE_BUFFER_ITEMS(params->buffer, params->len, LilyAstExpr);
                FREE(Vec, params);
                FREE(LilyParseBlock, &param_block);

                return NULL;
            });

            FREE(LilyParseBlock, &param_block);

            push__Vec(params, param);
        }

        return NEW_VARIANT(LilyAstBodyFunItem,
                           stmt,
                           NEW_VARIANT(LilyAstStmt,
                                       asm,
                                       item->location,
                                       NEW(LilyAstStmtAsm, value, params)));
    } else {
        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      &self->package->file,
                      &item->location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_ASM_PARAM),
                      NULL,
                      NULL,
                      NULL),
          &self->package->count_error);

        return NULL;
    }
}

LilyAstBodyFunItem *
parse_await_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    LilyParseBlock expr_block =
      NEW(LilyParseBlock, self, item->stmt_await.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    FREE(LilyParseBlock, &expr_block);

    if (!expr) {
        return NULL;
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, await, item->location, NEW(LilyAstStmtAwait, expr)));
}

LilyAstBodyFunItem *
parse_block_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   block,
                                   item->location,
                                   NEW(LilyAstStmtBlock,
                                       parse_fun_body__LilyParser(
                                         self, item->stmt_block.block))));
}

LilyAstBodyFunItem *
parse_break_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    if (item->stmt_break.name) {
        return NEW_VARIANT(
          LilyAstBodyFunItem,
          stmt,
          NEW_VARIANT(LilyAstStmt,
                      break,
                      item->location,
                      NEW(LilyAstStmtBreak, item->stmt_break.name)));
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, break, item->location, NEW(LilyAstStmtBreak, NULL)));
}

LilyAstBodyFunItem *
parse_defer_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    LilyAstBodyFunItem *item_ast =
      parse_fun_body_item_for_stmt__LilyParser(self, item->stmt_defer.item);

    if (!item_ast) {
        return NULL;
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, defer, item->location, NEW(LilyAstStmtDefer, item_ast)));
}

LilyAstBodyFunItem *
parse_drop_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item)
{
    LilyParseBlock expr_block = NEW(LilyParseBlock, self, item->stmt_drop.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr, expr_block, NULL, "expected `;`", {
        FREE(LilyParseBlock, &expr_block);
        return NULL;
    });

    FREE(LilyParseBlock, &expr_block);

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, drop, item->location, NEW(LilyAstStmtDrop, expr)));
}

LilyAstBodyFunItem *
parse_for_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item)
{
    LilyParseBlock expr_block = NEW(LilyParseBlock, self, item->stmt_for.expr);
    LilyAstExpr *expr_left =
      parse_primary_expr__LilyParseBlock(&expr_block, false);
    LilyAstCapture *capture = NULL; // LilyAstCapture*?

    if (!expr_left) {
        return NULL;
    }

    switch (expr_block.current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_IN:
            next_token__LilyParseBlock(&expr_block);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          expr_block.file,
                          &expr_block.current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `in` keyword")),
              expr_block.count_error);
    }

    LilyAstExpr *expr_right = parse_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr_right,
               expr_block,
               init__Vec(1, from__String("<expr> in <expr>")),
               "expected `do` keyword",
               {
                   FREE(LilyAstExpr, expr_left);
                   FREE(LilyParseBlock, &expr_block);
                   return NULL;
               });

    FREE(LilyParseBlock, &expr_block);

    if (item->stmt_for.capture) {
        LilyParseBlock capture_block =
          NEW(LilyParseBlock, self, item->stmt_for.capture);

        capture = parse_capture__LilyParseBlock(&capture_block);

        FREE(LilyParseBlock, &capture_block);
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, for, item->location, NEW(LilyAstStmtFor, item->stmt_for.name, expr_left, expr_right, capture, parse_fun_body__LilyParser(self, item->stmt_for.block))));
}

LilyAstBodyFunItem *
parse_if_stmt__LilyParser(LilyParser *self,
                          const LilyPreparserFunBodyItem *item)
{
    // 1. Parse if expression
    LilyParseBlock if_expr_block =
      NEW(LilyParseBlock, self, item->stmt_if.if_expr);
    LilyAstExpr *if_expr = parse_expr__LilyParseBlock(&if_expr_block);
    LilyAstCapture *if_capture = NULL; // LilyAstCapture*?

    CHECK_EXPR(if_expr, if_expr_block, NULL, "expected `do` keyword", {});

    FREE(LilyParseBlock, &if_expr_block);

    if (!if_expr) {
        return NULL;
    }

    // 2. Parse if capture
    if (item->stmt_if.if_capture) {
        LilyParseBlock if_capture_block =
          NEW(LilyParseBlock, self, item->stmt_if.if_capture);

        if_capture = parse_capture__LilyParseBlock(&if_capture_block);

        FREE(LilyParseBlock, &if_capture_block);
    }

    // 3. Parse if block
    Vec *if_body = parse_fun_body__LilyParser(self, item->stmt_if.if_block);

    Vec *elif_exprs = NULL;    // Vec<LilyAstExpr*>*?
    Vec *elif_captures = NULL; // Vec<LilyAstCapture*?>*?
    Vec *elif_bodies = NULL;   // Vec<Vec<LilyAstBodyFunItem*>*>*?

    if (item->stmt_if.elif_exprs) {
        // 4. Parse elif expressions
        elif_exprs = NEW(Vec); // Vec<LilyAstExpr*>*

        for (Usize i = 0; i < item->stmt_if.elif_exprs->len; ++i) {
            LilyParseBlock expr_block =
              NEW(LilyParseBlock, self, get__Vec(item->stmt_if.elif_exprs, i));
            LilyAstExpr *elif_expr = parse_expr__LilyParseBlock(&expr_block);

            CHECK_EXPR(
              elif_expr, expr_block, NULL, "expected `do` keyword", {});

            FREE(LilyParseBlock, &expr_block);

            if (elif_expr) {
                push__Vec(elif_exprs, elif_expr);
            }
        }

        // 5. Parse elif captures
        elif_captures = NEW(Vec); // Vec<LilyAstCapture*?>*

        for (Usize i = 0; i < item->stmt_if.elif_captures->len; ++i) {
            Vec *elif_capture = get__Vec(item->stmt_if.elif_captures, i);

            if (elif_capture) {
                LilyParseBlock elif_capture_block =
                  NEW(LilyParseBlock, self, elif_capture);

                push__Vec(elif_captures,
                          parse_capture__LilyParseBlock(&elif_capture_block));

                FREE(LilyParseBlock, &elif_capture_block);
            } else {
                push__Vec(elif_captures, NULL);
            }
        }

        // 6. Parse elif blocks
        elif_bodies = NEW(Vec); // Vec<Vec<LilyAstBodyFunItem*>*>*

        for (Usize i = 0; i < item->stmt_if.elif_blocks->len; ++i) {
            push__Vec(elif_bodies,
                      parse_fun_body__LilyParser(
                        self, get__Vec(item->stmt_if.elif_blocks, i)));
        }
    }

    Vec *else_body = NULL;

    if (item->stmt_if.else_block) {
        // 7. Parse else block
        else_body = parse_fun_body__LilyParser(self, item->stmt_if.else_block);
    }

    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   if,
                                   item->location,
                                   NEW(LilyAstStmtIf,
                                       if_expr,
                                       if_capture,
                                       if_body,
                                       elif_exprs,
                                       elif_captures,
                                       elif_bodies,
                                       else_body)));
}

LilyAstBodyFunItem *
parse_match_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    // 1. Parse expression
    LilyParseBlock expr_block =
      NEW(LilyParseBlock, self, item->stmt_match.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr, expr_block, NULL, "expected `do` keyword", {
        FREE(LilyParseBlock, &expr_block);
        return NULL;
    });

    FREE(LilyParseBlock, &expr_block);

    // 2. Parse case(s)
    Vec *cases = NEW(Vec); // Vec<LilyAstStmtMatchCase*>*

    for (Usize i = 0; i < item->stmt_match.patterns->len; ++i) {
        // 3. Parse pattern
        LilyParseBlock pattern_block =
          NEW(LilyParseBlock, self, get__Vec(item->stmt_match.patterns, i));
        LilyAstPattern *pattern = parse_pattern__LilyParseBlock(&pattern_block);

        CHECK_PATTERN(pattern, pattern_block, NULL, "expected `=>` or `?`", {
            FREE(LilyParseBlock, &pattern_block);

            return NULL;
        });

        FREE(LilyParseBlock, &pattern_block);

        // 4. Parse expression
        LilyAstExpr *cond = NULL;
        {
            Vec *cond_preparser = get__Vec(item->stmt_match.pattern_conds, i);

            if (cond_preparser) {
                LilyParseBlock cond_block =
                  NEW(LilyParseBlock, self, cond_preparser);
                cond = parse_expr__LilyParseBlock(&cond_block);

                CHECK_EXPR(cond, cond_block, NULL, "expected `=>`", {});

                FREE(LilyParseBlock, &cond_block);
            }
        }

        // 5. Parse fun body item
        LilyAstBodyFunItem *body_item =
          parse_fun_body_item_for_stmt__LilyParser(
            self, get__Vec(item->stmt_match.blocks, i));

        if (body_item) {
            push__Vec(cases,
                      NEW(LilyAstStmtMatchCase, pattern, cond, body_item));
        } else {
            FREE(LilyAstPattern, pattern);

            if (cond) {
                FREE(LilyAstExpr, cond);
            }
        }
    }

    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   match,
                                   item->location,
                                   NEW(LilyAstStmtMatch, expr, cases)));
}

LilyAstBodyFunItem *
parse_next_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item)
{
    if (item->stmt_next.name) {
        return NEW_VARIANT(
          LilyAstBodyFunItem,
          stmt,
          NEW_VARIANT(LilyAstStmt,
                      next,
                      item->location,
                      NEW(LilyAstStmtNext, item->stmt_next.name)));
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, next, item->location, NEW(LilyAstStmtNext, NULL)));
}

LilyAstBodyFunItem *
parse_raise_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    if (item->stmt_raise.expr->len == 0) {
        Location location = clone__Location(&item->location);

        location.start_line = location.end_line;
        location.start_column = location.end_column;
        location.start_position = location.end_position;

        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      &self->package->file,
                      &location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_EXPRESSION),
                      NULL,
                      NULL,
                      NULL),
          &self->package->count_error);

        return NULL;
    }

    LilyParseBlock expr_block =
      NEW(LilyParseBlock, self, item->stmt_raise.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr, expr_block, NULL, "expected `;`", {
        FREE(LilyParseBlock, &expr_block);
        return NULL;
    });

    FREE(LilyParseBlock, &expr_block);

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, raise, item->location, NEW(LilyAstStmtRaise, expr)));
}

LilyAstBodyFunItem *
parse_return_stmt__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *item)
{
    if (item->stmt_return.expr) {
        LilyParseBlock expr_block =
          NEW(LilyParseBlock, self, item->stmt_return.expr);
        LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

        CHECK_EXPR(expr, expr_block, NULL, "expected `;`", {
            FREE(LilyParseBlock, &expr_block);
            return NULL;
        });

        FREE(LilyParseBlock, &expr_block);

        return NEW_VARIANT(
          LilyAstBodyFunItem,
          stmt,
          NEW_VARIANT(
            LilyAstStmt, return, item->location, NEW(LilyAstStmtReturn, expr)));
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, return, item->location, NEW(LilyAstStmtReturn, NULL)));
}

LilyAstBodyFunItem *
parse_try_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item)
{
    Vec *body = parse_fun_body__LilyParser(self, item->stmt_try.block);
    LilyAstExpr *catch_expr = NULL;
    Vec *catch_body = NULL;

    if (item->stmt_try.catch_expr) {
        LilyParseBlock catch_expr_block =
          NEW(LilyParseBlock, self, item->stmt_try.catch_expr);
        catch_expr = parse_expr__LilyParseBlock(&catch_expr_block);

        CHECK_EXPR(
          catch_expr, catch_expr_block, NULL, "expected `do` keyword", {
              FREE(LilyParseBlock, &catch_expr_block);
              return NULL;
          });

        FREE(LilyParseBlock, &catch_expr_block);

        catch_body =
          parse_fun_body__LilyParser(self, item->stmt_try.catch_block);
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(LilyAstStmt,
                  try,
                  item->location,
                  NEW(LilyAstStmtTry, body, catch_expr, catch_body)));
}

LilyAstBodyFunItem *
parse_unsafe_stmt__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *item)
{
    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   unsafe,
                                   item->location,
                                   NEW(LilyAstStmtUnsafe,
                                       parse_fun_body__LilyParser(
                                         self, item->stmt_unsafe.block))));
}

LilyAstBodyFunItem *
parse_variable_stmt__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item)
{
    // 1. Parse data type
    LilyAstDataType *data_type = NULL;

    if (item->stmt_var.data_type) {
        LilyParseBlock data_type_block =
          NEW(LilyParseBlock, self, item->stmt_var.data_type);
        data_type = parse_data_type__LilyParseBlock(&data_type_block);

        CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `:=`", {
            FREE(LilyParseBlock, &data_type_block);
            return NULL;
        });

        FREE(LilyParseBlock, &data_type_block);
    }

    // 2. Parse expression
    LilyParseBlock expr_block = NEW(LilyParseBlock, self, item->stmt_var.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr, expr_block, NULL, "expected `;`", {
        FREE(LilyParseBlock, &expr_block);
        return NULL;
    });

    FREE(LilyParseBlock, &expr_block);

    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   variable,
                                   item->location,
                                   NEW(LilyAstStmtVariable,
                                       item->stmt_var.name,
                                       data_type,
                                       expr,
                                       item->stmt_var.is_mut,
                                       item->stmt_var.is_trace,
                                       item->stmt_var.is_ref,
                                       item->stmt_var.is_drop)));
}

LilyAstBodyFunItem *
parse_while_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    // 1. Parse expression
    LilyParseBlock expr_block =
      NEW(LilyParseBlock, self, item->stmt_while.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr, expr_block, NULL, "expected `do` keyword", {
        FREE(LilyParseBlock, &expr_block);
        return NULL;
    });

    FREE(LilyParseBlock, &expr_block);

    // 2. Parse body
    Vec *body = parse_fun_body__LilyParser(self, item->stmt_while.block);

    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   while,
                                   item->location,
                                   NEW(LilyAstStmtWhile, NULL, expr, body)));
}

LilyAstBodyFunItem *
parse_stmt__LilyParser(LilyParser *self, const LilyPreparserFunBodyItem *item)
{
    switch (item->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM:
            return parse_asm_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT:
            return parse_await_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK:
            return parse_block_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK:
            return parse_break_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER:
            return parse_defer_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP:
            return parse_drop_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR:
            return parse_for_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF:
            return parse_if_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH:
            return parse_match_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT:
            return parse_next_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE:
            return parse_raise_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN:
            return parse_return_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY:
            return parse_try_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE:
            return parse_unsafe_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE:
            return parse_variable_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE:
            return parse_while_stmt__LilyParser(self, item);
        default:
            UNREACHABLE("no other statement");
    }

    return NULL;
}

LilyAstBodyFunItem *
parse_fun_body_item_for_stmt__LilyParser(LilyParser *self,
                                         const LilyPreparserFunBodyItem *item)
{
    switch (item->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS: {
            LilyParseBlock exprs_block =
              NEW(LilyParseBlock, self, item->exprs.tokens);
            LilyAstExpr *expr = parse_expr__LilyParseBlock(&exprs_block);

            CHECK_EXPR(expr, exprs_block, NULL, "expected `;`", {
                FREE(LilyParseBlock, &exprs_block);
                return NULL;
            });

            FREE(LilyParseBlock, &exprs_block);

            return NEW_VARIANT(LilyAstBodyFunItem, expr, expr);
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA: {
            LilyAstExpr *expr = parse_lambda_expr__LilyParser(self, item);

            if (!expr) {
                return NULL;
            }

            return NEW_VARIANT(LilyAstBodyFunItem, expr, expr);
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND: {
            Vec *body = NEW(Vec);

            apply_macro_expansion_in_fun__LilyParser(
              self, (LilyPreparserFunBodyItem *)item, body);

            switch (body->len) {
                case 0:
                    FREE(Vec, body);

                    return NULL;
                case 1: {
                    LilyAstBodyFunItem *item = get__Vec(body, 0);

                    FREE(Vec, body);

                    return item;
                }
                default: {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &item->location,
                        NEW(LilyError,
                            LILY_ERROR_KIND_TOO_MANY_ITEMS_IN_MACRO_EXPAND),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);

                    FREE_BUFFER_ITEMS(
                      body->buffer, body->len, LilyAstBodyFunItem);
                    FREE(Vec, body);

                    return NULL;
                }
            }
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                &self->package->file,
                &item->location,
                NEW(LilyError,
                    LILY_ERROR_KIND_VARIABLE_DECLARATION_IS_NOT_EXPECTED),
                NULL,
                NULL,
                NULL),
              &self->package->count_error);

            return NULL;
        default:
            return parse_stmt__LilyParser(self, item);
    }
}

void
parse_fun_body_item__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item,
                                Vec *body)
{
    switch (item->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS: {
            LilyParseBlock exprs_block =
              NEW(LilyParseBlock, self, item->exprs.tokens);

            do {
                LilyAstExpr *expr = parse_expr__LilyParseBlock(&exprs_block);

                if (expr) {
                    push__Vec(body,
                              NEW_VARIANT(LilyAstBodyFunItem, expr, expr));
                }
            } while (!HAS_REACHED_THE_END(exprs_block));

            FREE(LilyParseBlock, &exprs_block);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA: {
            LilyAstExpr *expr = parse_lambda_expr__LilyParser(self, item);

            if (expr) {
                push__Vec(body, NEW_VARIANT(LilyAstBodyFunItem, expr, expr));
            }

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND: {
            apply_macro_expansion_in_fun__LilyParser(
              self, (LilyPreparserFunBodyItem *)item, body);

            break;
        }
        default: {
            LilyAstBodyFunItem *stmt = parse_stmt__LilyParser(self, item);

            if (stmt) {
                push__Vec(body, stmt);
            }
        }
    }
}

Vec *
parse_fun_body__LilyParser(LilyParser *self, Vec *block)
{
    Vec *body = NEW(Vec); // Vec<LilyAstBodyFunItem*>*

    for (Usize i = 0; i < block->len; ++i) {
        parse_fun_body_item__LilyParser(self, get__Vec(block, i), body);
    }

    return body;
}

// NOTE: only used to parse expression between (), {}, [] by example.
#define PATTERN_PARSE_CLOSING(closing)                                 \
    Vec *patterns = NEW(Vec); /* Vec<LilyAstPattern*>* */              \
    while (self->current->kind != closing) {                           \
        LilyAstPattern *pattern = parse_pattern__LilyParseBlock(self); \
                                                                       \
        if (pattern) {                                                 \
            push__Vec(patterns, pattern);                              \
        } else {                                                       \
            SKIP_TO_TOKEN(closing);                                    \
                                                                       \
            FREE_BUFFER_ITEMS(                                         \
              patterns->buffer, patterns->len, LilyAstPattern);        \
            FREE(Vec, patterns);                                       \
                                                                       \
            return NULL;                                               \
        }                                                              \
                                                                       \
        CHECK_COMMA(closing)                                           \
    }                                                                  \
                                                                       \
    END_LOCATION(&location, self->current->location);                  \
    next_token__LilyParseBlock(self); /* skip closing */

LilyAstPattern *
parse_array_pattern__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);

    PATTERN_PARSE_CLOSING(LILY_TOKEN_KIND_R_HOOK);

    return NEW_VARIANT(
      LilyAstPattern, array, location, NEW(LilyAstPatternArray, patterns));
}

LilyAstPattern *
parse_as_pattern__LilyParseBlock(LilyParseBlock *self, LilyAstPattern *pattern)
{
    Location location = clone__Location(&pattern->location);

    next_token__LilyParseBlock(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            END_LOCATION(&location, self->current->location);
            next_token__LilyParseBlock(self);

            return NEW_VARIANT(
              LilyAstPattern,
              as,
              location,
              NEW(LilyAstPatternAs, pattern, self->current->identifier_normal));
        default: {
            String *token_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->previous->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String("expected identifier after `as` keyword")),
              self->count_error);

            FREE(String, token_s);

            return NULL;
        }
    }
}

LilyAstPattern *
parse_exception_pattern__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);
    LilyAstExpr *id = parse_only_access_expr__LilyParseBlock(self);

    if (!id) {
        return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON:
            next_token__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_COLON_DOLLAR:
            END_LOCATION(&location, self->current->location);

            next_token__LilyParseBlock(self);

            return NEW_VARIANT(LilyAstPattern,
                               error,
                               location,
                               NEW(LilyAstPatternError, id, NULL));
        default: {
            String *token_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->previous->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String("expected `:`")),
              self->count_error);

            FREE(String, token_s);

            break;
        }
    }

    LilyAstPattern *pattern = parse_pattern__LilyParseBlock(self);

    if (!pattern) {
        FREE(LilyAstExpr, id);

        return NULL;
    }

    END_LOCATION(&location, pattern->location);

    return NEW_VARIANT(
      LilyAstPattern, error, location, NEW(LilyAstPatternError, id, pattern));
}

LilyAstPattern *
parse_list_pattern__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);

    PATTERN_PARSE_CLOSING(LILY_TOKEN_KIND_R_BRACE);

    return NEW_VARIANT(
      LilyAstPattern, list, location, NEW(LilyAstPatternList, patterns));
}

LilyAstPattern *
parse_list_head_pattern__LilyParseBlock(LilyParseBlock *self,
                                        LilyAstPattern *left)
{
    next_token__LilyParseBlock(self);

    Location location = clone__Location(&left->location);
    LilyAstPattern *right = parse_pattern__LilyParseBlock(self);

    if (!right) {
        FREE(LilyAstPattern, left);

        return NULL;
    }

    END_LOCATION(&location, right->location);

    return NEW_VARIANT(LilyAstPattern,
                       list_head,
                       location,
                       NEW(LilyAstPatternListHead, left, right));
}

LilyAstPattern *
parse_list_tail_pattern__LilyParseBlock(LilyParseBlock *self,
                                        LilyAstPattern *left)
{
    next_token__LilyParseBlock(self);

    Location location = clone__Location(&left->location);
    LilyAstPattern *right = parse_pattern__LilyParseBlock(self);

    if (!right) {
        FREE(LilyAstPattern, left);

        return NULL;
    }

    END_LOCATION(&location, right->location);

    return NEW_VARIANT(LilyAstPattern,
                       list_tail,
                       location,
                       NEW(LilyAstPatternListTail, left, right));
}

LilyAstPattern *
parse_literal_pattern__LilyParseBlock(LilyParseBlock *self)
{
    PARSE_LITERAL(Pattern, PATTERN);
}

LilyAstPattern *
parse_range_pattern__LilyParseBlock(LilyParseBlock *self, LilyAstPattern *left)
{
    Location location = clone__Location(&left->location);

    next_token__LilyParseBlock(self); // skip `..`

    LilyAstPattern *right = parse_pattern__LilyParseBlock(self);

    if (!right) {
        FREE(LilyAstPattern, left);

        return NULL;
    }

    END_LOCATION(&location, right->location);

    return NEW_VARIANT(
      LilyAstPattern, range, location, NEW(LilyAstPatternRange, left, right));
}

LilyAstPattern *
parse_record_call_pattern__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
    Location location = clone__Location(&id->location);
    Vec *fields = NEW(Vec);

    next_token__LilyParseBlock(self); // skip `{`

    while (self->current->kind != LILY_TOKEN_KIND_R_BRACE) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyToken *peeked = peek_token__LilyParseBlock(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_EQ: {
                            String *name =
                              clone__String(self->current->identifier_normal);

                            jump__LilyParseBlock(self, 2);

                            LilyAstPattern *pattern =
                              parse_pattern__LilyParseBlock(self);

                            if (pattern) {

                                push__Vec(fields,
                                          NEW(LilyAstPatternRecordField,
                                              name,
                                              pattern));
                            } else {
                                FREE(String, name);
                                FREE(LilyAstExpr, id);
                                FREE_BUFFER_ITEMS(fields->buffer,
                                                  fields->len,
                                                  LilyAstPatternRecordField);
                                FREE(Vec, fields);

                                return NULL;
                            }

                            goto end_of_the_loop;
                        }
                        default:
                            break;
                    }
                }

                break;
            }
            default:
                break;
        }

        {
            LilyAstPattern *pattern = parse_pattern__LilyParseBlock(self);

            if (pattern) {
                push__Vec(fields,
                          NEW(LilyAstPatternRecordField, NULL, pattern));
            } else {
                FREE(LilyAstExpr, id);
                FREE_BUFFER_ITEMS(
                  fields->buffer, fields->len, LilyAstPatternRecordField);
                FREE(Vec, fields);

                return NULL;
            }
        }

    end_of_the_loop: {
        CHECK_COMMA(LILY_TOKEN_KIND_R_BRACE);
    }
    }

    END_LOCATION(&location, self->current->location);
    next_token__LilyParseBlock(self);

    return NEW_VARIANT(LilyAstPattern,
                       record_call,
                       location,
                       NEW(LilyAstPatternRecordCall, id, fields));
}

LilyAstPattern *
parse_tuple_pattern__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);

    PATTERN_PARSE_CLOSING(LILY_TOKEN_KIND_R_PAREN);

    return NEW_VARIANT(
      LilyAstPattern, tuple, location, NEW(LilyAstPatternTuple, patterns));
}

LilyAstPattern *
parse_variant_call_pattern__LilyParseBlock(LilyParseBlock *self,
                                           LilyAstExpr *id)
{
    Location location = clone__Location(&id->location);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_DOLLAR:
            END_LOCATION(&location, self->current->location);

            next_token__LilyParseBlock(self);

            return NEW_VARIANT(LilyAstPattern,
                               variant_call,
                               location,
                               NEW(LilyAstPatternVariantCall, id, NULL));
        default:
            break;
    }

    next_token__LilyParseBlock(self); // skip `:`

    LilyAstPattern *pattern = parse_pattern__LilyParseBlock(self);

    END_LOCATION(&location, pattern->location);

    return NEW_VARIANT(LilyAstPattern,
                       variant_call,
                       location,
                       NEW(LilyAstPatternVariantCall, id, pattern));
}

LilyAstPattern *
parse_pattern__LilyParseBlock(LilyParseBlock *self)
{
#define PARSE_PATTERN_IDENTIFIER(identifier_val)                               \
    /* Check if it's a path */                                                 \
    switch (self->current->kind) {                                             \
        case LILY_TOKEN_KIND_L_BRACE:                                          \
            pattern = parse_record_call_pattern__LilyParseBlock(               \
              self,                                                            \
              NEW_VARIANT(LilyAstExpr,                                         \
                          identifier,                                          \
                          self->previous->location,                            \
                          NEW(LilyAstExprIdentifier,                           \
                              clone__String(identifier_val),                   \
                              NULL)));                                         \
                                                                               \
            break;                                                             \
        case LILY_TOKEN_KIND_COLON:                                            \
        case LILY_TOKEN_KIND_COLON_DOLLAR:                                     \
            pattern = parse_variant_call_pattern__LilyParseBlock(              \
              self,                                                            \
              NEW_VARIANT(LilyAstExpr,                                         \
                          identifier,                                          \
                          self->previous->location,                            \
                          NEW(LilyAstExprIdentifier,                           \
                              clone__String(identifier_val),                   \
                              NULL)));                                         \
                                                                               \
            break;                                                             \
        case LILY_TOKEN_KIND_DOT: {                                            \
            /* Parse path access */                                            \
            LilyAstExpr *path = parse_path_access_for_pattern__LilyParseBlock( \
              self,                                                            \
              NEW_VARIANT(LilyAstExpr,                                         \
                          identifier,                                          \
                          self->previous->location,                            \
                          NEW(LilyAstExprIdentifier,                           \
                              clone__String(identifier_val),                   \
                              NULL)),                                          \
              LILY_AST_EXPR_ACCESS_KIND_PATH);                                 \
                                                                               \
            /* Parse variant call or record call */                            \
            switch (self->current->kind) {                                     \
                case LILY_TOKEN_KIND_L_BRACE:                                  \
                    pattern =                                                  \
                      parse_record_call_pattern__LilyParseBlock(self, path);   \
                                                                               \
                    break;                                                     \
                case LILY_TOKEN_KIND_COLON:                                    \
                case LILY_TOKEN_KIND_COLON_DOLLAR:                             \
                    pattern =                                                  \
                      parse_variant_call_pattern__LilyParseBlock(self, path);  \
                                                                               \
                    break;                                                     \
                default: {                                                     \
                    String *token_s = to_string__LilyToken(self->current);     \
                                                                               \
                    emit__Diagnostic(                                          \
                      NEW_VARIANT(Diagnostic,                                  \
                                  simple_lily_error,                           \
                                  self->file,                                  \
                                  &self->previous->location,                   \
                                  NEW_VARIANT(LilyError,                       \
                                              unexpected_token,                \
                                              token_s->buffer),                \
                                  NULL,                                        \
                                  NULL,                                        \
                                  from__String("expected `{`, `:`, or `.`")),  \
                      self->count_error);                                      \
                                                                               \
                    FREE(String, token_s);                                     \
                                                                               \
                    return NULL;                                               \
                }                                                              \
            }                                                                  \
                                                                               \
            break;                                                             \
        }                                                                      \
        default:                                                               \
            if (!strcmp(identifier_val->buffer, "_")) {                        \
                pattern = NEW(LilyAstPattern,                                  \
                              self->previous->location,                        \
                              LILY_AST_PATTERN_KIND_WILDCARD);                 \
            } else {                                                           \
                pattern = NEW_VARIANT(                                         \
                  LilyAstPattern,                                              \
                  name,                                                        \
                  self->previous->location,                                    \
                  NEW(LilyAstPatternName, clone__String(identifier_val)));     \
            }                                                                  \
            break;                                                             \
    }

    next_token__LilyParseBlock(self);

    LilyAstPattern *pattern = NULL;

    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
        case LILY_TOKEN_KIND_KEYWORD_NIL:
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
        case LILY_TOKEN_KIND_KEYWORD_NONE:
        case LILY_TOKEN_KIND_LITERAL_BYTE:
        case LILY_TOKEN_KIND_LITERAL_BYTES:
        case LILY_TOKEN_KIND_LITERAL_CHAR:
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
        case LILY_TOKEN_KIND_LITERAL_INT_2:
        case LILY_TOKEN_KIND_LITERAL_INT_8:
        case LILY_TOKEN_KIND_LITERAL_INT_10:
        case LILY_TOKEN_KIND_LITERAL_INT_16:
        case LILY_TOKEN_KIND_LITERAL_STR:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            pattern = parse_literal_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_L_HOOK:
            pattern = parse_array_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_L_BRACE:
            pattern = parse_list_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_KEYWORD_ERROR:
            pattern = parse_exception_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            PARSE_PATTERN_IDENTIFIER(self->previous->identifier_string);

            break;
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            PARSE_PATTERN_IDENTIFIER(self->previous->identifier_normal);

            break;
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
        case LILY_TOKEN_KIND_KEYWORD_SELF: {
            LilyAstExpr *access =
              parse_access_expr_for_pattern__LilyParseBlock(self);

            if (!access) {
                return NULL;
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_COLON:
                case LILY_TOKEN_KIND_COLON_DOLLAR:
                    pattern =
                      parse_variant_call_pattern__LilyParseBlock(self, access);

                    break;
                case LILY_TOKEN_KIND_L_BRACE:
                    pattern =
                      parse_record_call_pattern__LilyParseBlock(self, access);

                    break;
                default: {
                    String *token_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(
                      NEW_VARIANT(Diagnostic,
                                  simple_lily_error,
                                  self->file,
                                  &self->previous->location,
                                  NEW_VARIANT(LilyError,
                                              unexpected_token,
                                              token_s->buffer),
                                  NULL,
                                  NULL,
                                  from__String("expected `{`, `:`, or `.`")),
                      self->count_error);

                    FREE(String, token_s);
                    FREE(LilyAstExpr, access);

                    return NULL;
                }
            }

            break;
        }
        case LILY_TOKEN_KIND_L_PAREN:
            switch (self->current->kind) {
                case LILY_TOKEN_KIND_R_PAREN: {
                    Location location =
                      clone__Location(&self->current->location);

                    next_token__LilyParseBlock(self);
                    END_LOCATION(&location, self->previous->location);

                    pattern =
                      NEW_VARIANT(LilyAstPattern,
                                  literal,
                                  location,
                                  NEW(LilyAstPatternLiteral,
                                      LILY_AST_PATTERN_LITERAL_KIND_UNIT));

                    break;
                }
                default:
                    pattern = parse_tuple_pattern__LilyParseBlock(self);
            }

            break;
        case LILY_TOKEN_KIND_DOT_DOT:
            pattern = NEW(LilyAstPattern,
                          self->previous->location,
                          LILY_AST_PATTERN_KIND_AUTO_COMPLETE);

            break;
        case LILY_TOKEN_KIND_EXPAND:
            switch (self->previous->expand.kind) {
                case LILY_TOKEN_EXPAND_KIND_PATT: {
                    LilyParseBlock patt_block =
                      NEW(LilyParseBlock,
                          self->parser,
                          self->previous->expand.tokens);
                    LilyAstPattern *patt =
                      parse_pattern__LilyParseBlock(&patt_block);

                    CHECK_PATTERN(
                      patt, patt_block, NULL, "expected only one pattern", {});

                    FREE(LilyParseBlock, &patt_block);

                    return patt;
                }
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->previous->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_PATTERN),
                        init__Vec(
                          1,
                          from__String("expected `patt` as the data type "
                                       "of the macro param")),
                        NULL,
                        NULL),
                      self->count_error);

                    return NULL;
            }
        default: {
            String *token_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->previous->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String("expected literal value, `[`, `error`, "
                             "identifier, `(` or `..`")),
              self->count_error);

            FREE(String, token_s);

            return NULL;
        }
    }

    if (!pattern) {
        return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_AS:
            return parse_as_pattern__LilyParseBlock(self, pattern);
        case LILY_TOKEN_KIND_DOT_DOT:
            return parse_range_pattern__LilyParseBlock(self, pattern);
        case LILY_TOKEN_KIND_ARROW:
            return parse_list_head_pattern__LilyParseBlock(self, pattern);
        case LILY_TOKEN_KIND_INVERSE_ARROW:
            return parse_list_tail_pattern__LilyParseBlock(self, pattern);
        default:
            break;
    }

    return pattern;
}

LilyAstGenericParam *
parse_generic_param__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->current->location);

    // 1. Parse name
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);

            next_token__LilyParseBlock(self);

            break;
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->previous->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                          NULL,
                          NULL,
                          NULL),
              self->count_error);

            return NULL;
        }
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON: {
            // 2. Parse generic param constraint
            next_token__LilyParseBlock(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_HOOK: {
                    Vec *dts = NEW(Vec);

                    next_token__LilyParseBlock(self); // skip `[`
                    DATA_TYPE_PARSE_CLOSING(
                      dts, LILY_TOKEN_KIND_R_HOOK, (&location));

                    return NEW_VARIANT(
                      LilyAstGenericParam,
                      constraint,
                      location,
                      NEW(LilyAstGenericParamConstraint, name, dts));
                }
                default: {
                    LilyAstDataType *data_type =
                      parse_data_type__LilyParseBlock(self);

                    if (!data_type) {
                        return NULL;
                    }

                    END_LOCATION(&location, data_type->location);

                    return NEW_VARIANT(LilyAstGenericParam,
                                       constraint,
                                       location,
                                       NEW(LilyAstGenericParamConstraint,
                                           name,
                                           init__Vec(1, data_type)));
                }
            }
        }
        default:
            END_LOCATION(&location, self->current->location);

            return NEW_VARIANT(LilyAstGenericParam, normal, location, name);
    }
}

Vec *
parse_generic_params__LilyParser(LilyParser *self, Vec *generic_params)
{
    Vec *res = NEW(Vec); // Vec<LilyAstGenericParam*>*

    for (Usize i = 0; i < generic_params->len; ++i) {
        LilyParseBlock generic_param_block =
          NEW(LilyParseBlock, self, get__Vec(generic_params, i));
        LilyAstGenericParam *generic_param =
          parse_generic_param__LilyParseBlock(&generic_param_block);

        if (!HAS_REACHED_THE_END(generic_param_block)) {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                generic_param_block.file,
                &generic_param_block.current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_ONLY_ONE_GENERIC_PARAM),
                NULL,
                NULL,
                from__String("expected `,`")),
              generic_param_block.count_error);
        }

        FREE(LilyParseBlock, &generic_param_block);

        if (generic_param) {
            push__Vec(res, generic_param);
        }
    }

    return res;
}

Vec *
parse_inherit_params__LilyParser(LilyParser *self, Vec *inherit_params)
{
    Vec *res = NEW(Vec); // Vec<LilyAstInheritParam*>*

    for (Usize i = 0; i < inherit_params->len; ++i) {
        LilyParseBlock data_type_block =
          NEW(LilyParseBlock, self, get__Vec(inherit_params, i));
        LilyAstDataType *data_type =
          parse_data_type__LilyParseBlock(&data_type_block);

        CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `,`", {});

        FREE(LilyParseBlock, &data_type_block);

        push__Vec(res, NEW(LilyAstInheritParam, data_type));
    }

    return res;
}

Vec *
parse_impl_params__LilyParser(LilyParser *self, Vec *impl_params)
{
    Vec *res = NEW(Vec); // Vec<LilyAstImplParam*>*

    for (Usize i = 0; i < impl_params->len; ++i) {
        LilyParseBlock data_type_block =
          NEW(LilyParseBlock, self, get__Vec(impl_params, i));
        LilyAstDataType *data_type =
          parse_data_type__LilyParseBlock(&data_type_block);

        CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `,`", {});

        FREE(LilyParseBlock, &data_type_block);

        push__Vec(res, NEW(LilyAstImplParam, data_type));
    }

    return res;
}

LilyAstDecl *
parse_alias_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (decl->type.alias.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, decl->type.alias.generic_params);
    }

    // 2. Parse alias data type
    LilyParseBlock data_type_block =
      NEW(LilyParseBlock, self, decl->type.alias.data_type);
    LilyAstDataType *data_type =
      parse_data_type__LilyParseBlock(&data_type_block);

    CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `;`", {
        if (generic_params) {
            FREE_BUFFER_ITEMS(
              generic_params->buffer, generic_params->len, LilyAstGenericParam);
            FREE(Vec, generic_params);
        }

        FREE(LilyParseBlock, &data_type_block);

        return NULL;
    });

    FREE(LilyParseBlock, &data_type_block);

    return NEW_VARIANT(LilyAstDecl,
                       type,
                       decl->location,
                       NEW_VARIANT(LilyAstDeclType,
                                   alias,
                                   NEW(LilyAstDeclAlias,
                                       decl->type.alias.name,
                                       generic_params,
                                       data_type,
                                       decl->type.alias.visibility)));
}

void
parse_attribute_decl__LilyParser(LilyParser *self,
                                 const LilyPreparserAttribute *pre_attribute,
                                 LilyAstDeclAttribute *attribute)
{
    attribute->name = pre_attribute->name;
    attribute->is_set = pre_attribute->is_set;
    attribute->is_get = pre_attribute->is_get;
    attribute->visibility = pre_attribute->visibility;

    // 1. Parse data type
    LilyParseBlock data_type_block =
      NEW(LilyParseBlock, self, pre_attribute->data_type);
    attribute->data_type = parse_data_type__LilyParseBlock(&data_type_block);

    CHECK_DATA_TYPE(attribute->data_type,
                    data_type_block,
                    NULL,
                    "expected `:=`, `::` or `;`",
                    {
                        FREE(LilyParseBlock, &data_type_block);
                        return;
                    });

    FREE(LilyParseBlock, &data_type_block);

    // 2. Parse expression
    attribute->optional_expr = NULL;

    if (pre_attribute->default_expr) {
        LilyParseBlock expr_block =
          NEW(LilyParseBlock, self, pre_attribute->default_expr);
        attribute->optional_expr = parse_expr__LilyParseBlock(&expr_block);

        CHECK_EXPR(
          attribute->optional_expr, expr_block, NULL, "expected `::` or `;`", {
              FREE(LilyAstDataType, attribute->data_type);
              FREE(LilyParseBlock, &expr_block);

              return;
          });

        FREE(LilyParseBlock, &expr_block);
    }
}

LilyAstBodyClassItem *
parse_attribute_decl_for_class__LilyParser(
  LilyParser *self,
  const LilyPreparserClassBodyItem *item)
{
    LilyAstDeclAttribute attribute;
    parse_attribute_decl__LilyParser(self, &item->attribute, &attribute);

    // NOTE: The destructor doesn't expect a NULL pointer on
    // `attribute.data_type`.
    if (!attribute.data_type) {
        FREE(String, attribute.name);

        if (attribute.optional_expr) {
            FREE(LilyAstExpr, attribute.optional_expr);
        }

        return NULL;
    }

    return NEW_VARIANT(
      LilyAstBodyClassItem, attribute, item->location, attribute);
}

LilyAstBodyTraitItem *
parse_attribute_decl_for_trait__LilyParser(
  LilyParser *self,
  const LilyPreparserTraitBodyItem *item)
{
    LilyAstDeclAttribute attribute;
    parse_attribute_decl__LilyParser(self, &item->attribute, &attribute);

    // NOTE: The destructor doesn't expect a NULL pointer on
    // attribute.data_type.
    if (!attribute.data_type) {
        FREE(String, attribute.name);

        if (attribute.optional_expr) {
            FREE(LilyAstExpr, attribute.optional_expr);
        }

        return NULL;
    }

    return NEW_VARIANT(
      LilyAstBodyTraitItem, attribute, item->location, attribute);
}

Vec *
parse_class_body__LilyParser(LilyParser *self, Vec *pre_body)
{
    Vec *body = NEW(Vec); // Vec<LilyAstBodyClassItem*>*

    for (Usize i = 0; i < pre_body->len; ++i) {
        LilyPreparserClassBodyItem *item = get__Vec(pre_body, i);

        switch (item->kind) {
            case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUTE: {
                LilyAstBodyClassItem *res =
                  parse_attribute_decl_for_class__LilyParser(self, item);

                if (res) {
                    push__Vec(body, res);
                }

                break;
            }
            case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND:
                apply_macro_expansion_in_class__LilyParser(self, item, body);

                break;
            case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD: {
                LilyAstBodyClassItem *res =
                  parse_method_decl__LilyParser(self, item);

                if (res) {
                    push__Vec(body, res);
                }

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }
    }

    return body;
}

LilyAstDecl *
parse_class_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (decl->object.class.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, decl->object.class.generic_params);
    }

    // 2. Parse inherit params
    Vec *inherit_params = NULL;

    if (decl->object.class.inherits) {
        inherit_params =
          parse_inherit_params__LilyParser(self, decl->object.class.inherits);
    }

    // 3. Parse impl params
    Vec *impl_params = NULL;

    if (decl->object.class.implements) {
        impl_params =
          parse_impl_params__LilyParser(self, decl->object.class.implements);
    }

    return NEW_VARIANT(LilyAstDecl,
                       object,
                       decl->location,
                       NEW_VARIANT(LilyAstDeclObject,
                                   class,
                                   NEW(LilyAstDeclClass,
                                       decl->object.class.name,
                                       generic_params,
                                       inherit_params,
                                       impl_params,
                                       parse_class_body__LilyParser(
                                         self, decl->object.class.body),
                                       decl->object.class.visibility,
                                       decl->object.class.is_close)));
}

#define PARSE_CONSTANT_INFO(self, info, location, dt)                   \
    LilyParseBlock expr_block = NEW(LilyParseBlock, self, info->expr);  \
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);        \
                                                                        \
    CHECK_EXPR(expr, expr_block, NULL, "expected `:=`", {               \
        FREE(LilyParseBlock, &expr_block);                              \
        return NULL;                                                    \
    });                                                                 \
                                                                        \
    FREE(LilyParseBlock, &expr_block);                                  \
                                                                        \
    LilyParseBlock data_type_block =                                    \
      NEW(LilyParseBlock, self, info->data_type);                       \
    LilyAstDataType *data_type =                                        \
      parse_data_type__LilyParseBlock(&data_type_block);                \
                                                                        \
    CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `;`", { \
        FREE(LilyAstExpr, expr);                                        \
        FREE(LilyParseBlock, &expr_block);                              \
                                                                        \
        return NULL;                                                    \
    });                                                                 \
                                                                        \
    FREE(LilyParseBlock, &data_type_block);                             \
                                                                        \
    return NEW_VARIANT(                                                 \
      dt,                                                               \
      constant,                                                         \
      *location,                                                        \
      NEW(                                                              \
        LilyAstDeclConstant, info->name, data_type, expr, info->visibility));

LilyAstDecl *
parse_constant_decl__LilyParser(LilyParser *self,
                                LilyPreparserConstantInfo *info,
                                Location *location)
{
    PARSE_CONSTANT_INFO(self, info, location, LilyAstDecl);
}

LilyAstBodyEnumObjectItem *
parse_constant_decl_for_enum_object__LilyParser(LilyParser *self,
                                                LilyPreparserConstantInfo *info,
                                                Location *location)
{
    PARSE_CONSTANT_INFO(self, info, location, LilyAstBodyEnumObjectItem);
}

LilyAstBodyRecordObjectItem *
parse_constant_decl_for_record_object__LilyParser(
  LilyParser *self,
  LilyPreparserConstantInfo *info,
  Location *location)
{
    PARSE_CONSTANT_INFO(self, info, location, LilyAstBodyRecordObjectItem);
}

#define PARSE_ENUM_VARIANT(item)                                               \
    LilyAstDataType *data_type = NULL;                                         \
                                                                               \
    if (item->variant.data_type) {                                             \
        LilyParseBlock data_type_block =                                       \
          NEW(LilyParseBlock, self, item->variant.data_type);                  \
        data_type = parse_data_type__LilyParseBlock(&data_type_block);         \
                                                                               \
        CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `;`", {}); \
                                                                               \
        FREE(LilyParseBlock, &data_type_block);                                \
    }

LilyAstVariant *
parse_enum_variant__LilyParser(LilyParser *self,
                               LilyPreparserEnumBodyItem *item)
{
    PARSE_ENUM_VARIANT(item);

    return NEW(LilyAstVariant, item->variant.name, data_type, item->location);
}

LilyAstBodyEnumObjectItem *
parse_enum_variant_for_enum_object__LilyParser(
  LilyParser *self,
  LilyPreparserEnumObjectBodyItem *item)
{
    PARSE_ENUM_VARIANT(item);

    return NEW_VARIANT(
      LilyAstBodyEnumObjectItem,
      variant,
      item->location,
      NEW(LilyAstVariant, item->variant.name, data_type, item->location));
}

LilyAstDecl *
parse_enum_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (decl->type.enum_.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, decl->type.enum_.generic_params);
    }

    // 2. Parse body
    Vec *variants = NEW(Vec); // Vec<LilyAstVariant*>*

    for (Usize i = 0; i < decl->type.enum_.body->len; ++i) {
        LilyPreparserEnumBodyItem *item = get__Vec(decl->type.enum_.body, i);

        switch (item->kind) {
            case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND:
                apply_macro_expansion_in_enum__LilyParser(self, item, variants);

                break;
            case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT:
                push__Vec(variants, parse_enum_variant__LilyParser(self, item));

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    return NEW_VARIANT(LilyAstDecl,
                       type,
                       decl->location,
                       NEW_VARIANT(LilyAstDeclType,
                                   enum,
                                   NEW(LilyAstDeclEnum,
                                       decl->type.enum_.name,
                                       generic_params,
                                       variants,
                                       decl->type.enum_.visibility)));
}

Vec *
parse_enum_object_body__LilyParser(LilyParser *self, Vec *pre_body)
{
    Vec *body = NEW(Vec); // Vec<LilyAstBodyEnumObjectItem*>*

    for (Usize i = 0; i < pre_body->len; ++i) {
        LilyPreparserEnumObjectBodyItem *item = get__Vec(pre_body, i);

        switch (item->kind) {
            case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT:
                switch (item->constant.kind) {
                    case LILY_PREPARSER_CONSTANT_KIND_SIMPLE: {
                        LilyAstBodyEnumObjectItem *constant =
                          parse_constant_decl_for_enum_object__LilyParser(
                            self, item->constant.simple, &item->location);

                        if (constant) {
                            push__Vec(body, constant);
                        }

                        break;
                    }
                    case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE: {
                        for (Usize i = 0; i < item->constant.multiple->len;
                             ++i) {
                            LilyAstBodyEnumObjectItem *constant =
                              parse_constant_decl_for_enum_object__LilyParser(
                                self,
                                get__Vec(item->constant.multiple, i),
                                &item->location);

                            if (constant) {
                                push__Vec(body, constant);
                            }
                        }

                        break;
                    }
                }

                break;
            case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND:
                apply_macro_expansion_in_enum_object__LilyParser(
                  self, item, body);

                break;
            case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD: {
                LilyAstBodyEnumObjectItem *method =
                  parse_method_decl_for_enum_object__LilyParser(self, item);

                if (method) {
                    push__Vec(body, method);
                }

                break;
            }
            case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT: {
                push__Vec(
                  body,
                  parse_enum_variant_for_enum_object__LilyParser(self, item));

                break;
            }
        }
    }

    return body;
}

LilyAstDecl *
parse_enum_object_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (decl->object.enum_.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, decl->object.enum_.generic_params);
    }

    // 2. Parse implement params
    Vec *impl_params = NULL;

    if (decl->object.enum_.implements) {
        impl_params =
          parse_impl_params__LilyParser(self, decl->object.enum_.implements);
    }

    // 3. Parse body
    Vec *body =
      parse_enum_object_body__LilyParser(self, decl->object.enum_.body);

    return NEW_VARIANT(LilyAstDecl,
                       object,
                       decl->location,
                       NEW_VARIANT(LilyAstDeclObject,
                                   enum,
                                   NEW(LilyAstDeclEnumObject,
                                       decl->object.enum_.name,
                                       generic_params,
                                       impl_params,
                                       body,
                                       decl->object.enum_.visibility)));
}

LilyAstDecl *
parse_error_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse data type
    LilyAstDataType *data_type = NULL;

    if (decl->error.data_type) {
        LilyParseBlock data_type_block =
          NEW(LilyParseBlock, self, decl->error.data_type);
        data_type = parse_data_type__LilyParseBlock(&data_type_block);

        CHECK_DATA_TYPE(
          data_type, data_type_block, NULL, "expected only one data type", {});

        FREE(LilyParseBlock, &data_type_block);
    }

    // 2. Parse generic params
    Vec *generic_params = NULL;

    if (decl->error.generic_params) {
        generic_params =
          parse_generic_params__LilyParser(self, decl->error.generic_params);
    }

    return NEW_VARIANT(LilyAstDecl,
                       error,
                       decl->location,
                       NEW(LilyAstDeclError,
                           decl->error.name,
                           generic_params,
                           data_type,
                           decl->error.visibility));
}

LilyAstDecl *
parse_use_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    LilyParseBlock access_block = NEW(LilyParseBlock, self, decl->use.path);
    LilyAstExpr *access = parse_only_access_expr__LilyParseBlock(&access_block);

    if (!access) {
        return NULL;
    } else {
        access = parse_postfix_expr__LilyParseBlock(&access_block, access);

        CHECK_EXPR(access, access_block, NULL, "", {
            FREE(LilyParseBlock, &access_block);
            FREE(LilyAstExpr, access);
        });
    }

    return NEW_VARIANT(
      LilyAstDecl, use, decl->location, NEW(LilyAstDeclUse, access));
}

LilyAstDecl *
parse_include_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    LilyParseBlock access_block = NEW(LilyParseBlock, self, decl->include.path);
    LilyAstExpr *access = parse_only_access_expr__LilyParseBlock(&access_block);

    if (!access) {
        return NULL;
    } else {
        access = parse_postfix_expr__LilyParseBlock(&access_block, access);

        CHECK_EXPR(access, access_block, NULL, "", {
            FREE(LilyParseBlock, &access_block);
            FREE(LilyAstExpr, access);
        });
    }

    return NEW_VARIANT(
      LilyAstDecl, include, decl->location, NEW(LilyAstDeclInclude, access));
}

LilyAstDeclFunParam *
parse_fun_param__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->current->location);

    // 1. Parse name
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            next_token__LilyParseBlock(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                          NULL,
                          NULL,
                          NULL),
              self->count_error);

            name = from__String("__error__");

            break;
    }

    LilyAstExpr *expr = NULL;
    LilyAstDataType *data_type = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
        parse_expr: {
            next_token__LilyParseBlock(self);

            expr = parse_expr__LilyParseBlock(self);

            if (expr) {
                return NEW_VARIANT(LilyAstDeclFunParam,
                                   default,
                                   name,
                                   data_type,
                                   location,
                                   expr);
            }

            return NEW_VARIANT(
              LilyAstDeclFunParam, normal, name, data_type, location);
        }
        case LILY_TOKEN_KIND_EOF:
            END_LOCATION(&location, self->previous->location);

            return NEW_VARIANT(
              LilyAstDeclFunParam, normal, name, NULL, location);
        default:
            data_type = parse_data_type__LilyParseBlock(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_COLON_EQ:
                    goto parse_expr;
                default: {
                    CHECK_DATA_TYPE(
                      data_type, (*self), NULL, "expected `,`", {});

                    return NEW_VARIANT(
                      LilyAstDeclFunParam, normal, name, data_type, location);
                }
            }

            break;
    }

    return NULL;
}

Vec *
parse_fun_params__LilyParser(LilyParser *self, Vec *params)
{
    Vec *res = NEW(Vec); // Vec<LilyAstDeclFunParam*>*

    for (Usize i = 0; i < params->len; ++i) {
        LilyParseBlock param_block =
          NEW(LilyParseBlock, self, get__Vec(params, i));
        LilyAstDeclFunParam *param =
          parse_fun_param__LilyParseBlock(&param_block);

        FREE(LilyParseBlock, &param_block);

        if (param) {
            push__Vec(res, param);
        }
    }

    return res;
}

LilyAstDecl *
parse_fun_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL; // Vec<LilyAstGenericParam*>*?

    if (decl->fun.generic_params) {
        generic_params =
          parse_generic_params__LilyParser(self, decl->fun.generic_params);
    }

    // 2. Parse params
    Vec *params = NULL; // Vec<LilyAstDeclFunParam*|LilyAstDeclMethodParam*>*?

    if (decl->fun.params && decl->fun.object_impl) {
        params = parse_method_params__LilyParser(self, decl->fun.params);
    } else if (decl->fun.params && !decl->fun.object_impl) {
        params = parse_fun_params__LilyParser(self, decl->fun.params);
    }

    // 3. Parse return data type
    LilyAstDataType *return_data_type = NULL;

    if (decl->fun.return_data_type) {
        LilyParseBlock return_data_type_block =
          NEW(LilyParseBlock, self, decl->fun.return_data_type);
        return_data_type =
          parse_data_type__LilyParseBlock(&return_data_type_block);

        CHECK_DATA_TYPE(
          return_data_type, return_data_type_block, NULL, "expected `=`", {});

        FREE(LilyParseBlock, &return_data_type_block);
    }

    // 4. Parse body
    Vec *body = parse_fun_body__LilyParser(self, decl->fun.body);

    // 5. Parse req
    Vec *req = NULL; // Vec<LilyAstExpr*>*?

    if (decl->fun.req) {
        req = NEW(Vec);

        for (Usize i = 0; i < decl->fun.req->len; ++i) {
            LilyParseBlock expr_block =
              NEW(LilyParseBlock, self, get__Vec(decl->fun.req, i));
            LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

            CHECK_EXPR(expr, expr_block, NULL, "expected `]`", {});

            FREE(LilyParseBlock, &expr_block);

            if (expr) {
                push__Vec(req, expr);
            }
        }
    }

    // 6. Parse when
    Vec *when = NULL; // Vec<LilyAstExpr*>*?

    if (decl->fun.when) {
        when = NEW(Vec);

        for (Usize i = 0; i < decl->fun.when->len; ++i) {
            LilyParseBlock expr_block =
              NEW(LilyParseBlock, self, get__Vec(decl->fun.when, i));
            LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

            CHECK_EXPR(expr, expr_block, NULL, "expected `]`", {});

            FREE(LilyParseBlock, &expr_block);

            if (expr) {
                push__Vec(when, expr);
            }
        }
    }

    return NEW_VARIANT(LilyAstDecl,
                       fun,
                       decl->location,
                       NEW(LilyAstDeclFun,
                           decl->fun.name,
                           decl->fun.object_impl,
                           generic_params,
                           params,
                           return_data_type,
                           body,
                           req,
                           when,
                           decl->fun.visibility,
                           decl->fun.is_async,
                           decl->fun.is_operator,
                           decl->fun.req_is_comptime,
                           decl->fun.when_is_comptime));
}

LilyAstDeclMethodParam *
parse_method_param__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->current->location);

    // 1. Parse name
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            next_token__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_KEYWORD_MUT:
            next_token__LilyParseBlock(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_self:
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyParseBlock(self);

                    if (!HAS_REACHED_THE_END((*self))) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                            NULL,
                            NULL,
                            from__String("expected `,`")),
                          self->count_error);

                        return NULL;
                    }

                    return NEW(LilyAstDeclMethodParam,
                               LILY_AST_DECL_METHOD_PARAM_KIND_MUT_SELF,
                               location);
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `self`")),
                      self->count_error);

                    return NULL;
            }
        case LILY_TOKEN_KIND_KEYWORD_REF:
            next_token__LilyParseBlock(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_self:
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyParseBlock(self);

                    if (!HAS_REACHED_THE_END((*self))) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                            NULL,
                            NULL,
                            from__String("expected `,`")),
                          self->count_error);

                        return NULL;
                    }

                    return NEW(LilyAstDeclMethodParam,
                               LILY_AST_DECL_METHOD_PARAM_KIND_REF_SELF,
                               location);
                case LILY_TOKEN_KIND_KEYWORD_MUT:
                    next_token__LilyParseBlock(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_self:
                            END_LOCATION(&location, self->current->location);
                            next_token__LilyParseBlock(self);

                            if (!HAS_REACHED_THE_END((*self))) {
                                emit__Diagnostic(
                                  NEW_VARIANT(
                                    Diagnostic,
                                    simple_lily_error,
                                    self->file,
                                    &self->current->location,
                                    NEW(LilyError,
                                        LILY_ERROR_KIND_EXPECTED_TOKEN),
                                    NULL,
                                    NULL,
                                    from__String("expected `,`")),
                                  self->count_error);

                                return NULL;
                            }

                            return NEW(
                              LilyAstDeclMethodParam,
                              LILY_AST_DECL_METHOD_PARAM_KIND_REF_MUT_SELF,
                              location);
                        default:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                                NULL,
                                NULL,
                                from__String("expected `self`")),
                              self->count_error);

                            return NULL;
                    }
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `self` or `mut`")),
                      self->count_error);

                    return NULL;
            }
        case LILY_TOKEN_KIND_KEYWORD_self:
            END_LOCATION(&location, self->current->location);
            next_token__LilyParseBlock(self);

            if (!HAS_REACHED_THE_END((*self))) {
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                              NULL,
                              NULL,
                              from__String("expected `,`")),
                  self->count_error);

                return NULL;
            }

            return NEW(LilyAstDeclMethodParam,
                       LILY_AST_DECL_METHOD_PARAM_KIND_SELF,
                       location);
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                          NULL,
                          NULL,
                          NULL),
              self->count_error);

            name = from__String("__error__");

            break;
    }

    LilyAstExpr *expr = NULL;
    LilyAstDataType *data_type = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
        parse_expr: {
            next_token__LilyParseBlock(self);

            expr = parse_expr__LilyParseBlock(self);

            if (expr) {
                return NEW_VARIANT(LilyAstDeclMethodParam,
                                   default,
                                   name,
                                   data_type,
                                   location,
                                   expr);
            }

            return NEW_VARIANT(
              LilyAstDeclMethodParam, normal, name, data_type, location);
        }
        case LILY_TOKEN_KIND_EOF:
            END_LOCATION(&location, self->previous->location);

            return NEW_VARIANT(
              LilyAstDeclMethodParam, normal, name, NULL, location);
        default:
            data_type = parse_data_type__LilyParseBlock(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_COLON_EQ:
                    goto parse_expr;
                default: {
                    CHECK_DATA_TYPE(
                      data_type, (*self), NULL, "expected `,`", {});

                    return NEW_VARIANT(LilyAstDeclMethodParam,
                                       normal,
                                       name,
                                       data_type,
                                       location);
                }
            }

            break;
    }

    return NULL;
}

Vec *
parse_method_params__LilyParser(LilyParser *self, Vec *params)
{
    Vec *res = NEW(Vec); // Vec<LilyAstDeclMethodParam*>*

    for (Usize i = 0; i < params->len; ++i) {
        LilyParseBlock param_block =
          NEW(LilyParseBlock, self, get__Vec(params, i));
        LilyAstDeclMethodParam *param =
          parse_method_param__LilyParseBlock(&param_block);

        FREE(LilyParseBlock, &param_block);

        if (param) {
            push__Vec(res, param);
        }
    }

    return res;
}

#define PARSE_METHOD_DECL(self, item, dt)                                      \
    /* 1. Parse generic params */                                              \
    Vec *generic_params = NULL; /* Vec<LilyAstGenericParam*>*? */              \
                                                                               \
    if (item->method.generic_params) {                                         \
        generic_params =                                                       \
          parse_generic_params__LilyParser(self, item->method.generic_params); \
    }                                                                          \
                                                                               \
    /* 2. Parse params */                                                      \
    Vec *params = NULL; /* Vec<LilyAstDeclFunParam>*? */                       \
                                                                               \
    if (item->method.params) {                                                 \
        params = parse_method_params__LilyParser(self, item->method.params);   \
    }                                                                          \
                                                                               \
    /* 3. Parse return data type */                                            \
    LilyAstDataType *return_data_type = NULL;                                  \
                                                                               \
    if (item->method.return_data_type) {                                       \
        LilyParseBlock return_data_type_block =                                \
          NEW(LilyParseBlock, self, item->method.return_data_type);            \
        return_data_type =                                                     \
          parse_data_type__LilyParseBlock(&return_data_type_block);            \
                                                                               \
        CHECK_DATA_TYPE(                                                       \
          return_data_type, return_data_type_block, NULL, "expected `=`", {}); \
                                                                               \
        FREE(LilyParseBlock, &return_data_type_block);                         \
    }                                                                          \
                                                                               \
    /* 4. Parse body */                                                        \
    Vec *body = parse_fun_body__LilyParser(self, item->method.body);           \
                                                                               \
    /* 5. Parse req */                                                         \
    Vec *req = NULL; /* Vec<LilyAstExpr*>*? */                                 \
                                                                               \
    if (item->method.req) {                                                    \
        req = NEW(Vec);                                                        \
                                                                               \
        for (Usize i = 0; i < item->method.req->len; ++i) {                    \
            LilyParseBlock expr_block =                                        \
              NEW(LilyParseBlock, self, get__Vec(item->method.req, i));        \
            LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);       \
                                                                               \
            FREE(LilyParseBlock, &expr_block);                                 \
                                                                               \
            if (expr) {                                                        \
                push__Vec(req, expr);                                          \
            }                                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    /* 6. Parse when */                                                        \
    Vec *when = NULL; /* Vec<LilyAstExpr*>*? */                                \
                                                                               \
    if (item->method.when) {                                                   \
        when = NEW(Vec);                                                       \
                                                                               \
        for (Usize i = 0; i < item->method.when->len; ++i) {                   \
            LilyParseBlock expr_block =                                        \
              NEW(LilyParseBlock, self, get__Vec(item->method.when, i));       \
            LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);       \
                                                                               \
            FREE(LilyParseBlock, &expr_block);                                 \
                                                                               \
            if (expr) {                                                        \
                push__Vec(when, expr);                                         \
            }                                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    return NEW_VARIANT(dt,                                                     \
                       method,                                                 \
                       item->location,                                         \
                       NEW(LilyAstDeclMethod,                                  \
                           item->method.name,                                  \
                           item->method.object_impl,                           \
                           generic_params,                                     \
                           params,                                             \
                           return_data_type,                                   \
                           body,                                               \
                           req,                                                \
                           when,                                               \
                           item->method.visibility,                            \
                           item->method.is_async,                              \
                           item->method.is_operator,                           \
                           item->method.req_is_comptime,                       \
                           item->method.when_is_comptime));

LilyAstBodyEnumObjectItem *
parse_method_decl_for_enum_object__LilyParser(
  LilyParser *self,
  LilyPreparserEnumObjectBodyItem *item)
{
    PARSE_METHOD_DECL(self, item, LilyAstBodyEnumObjectItem);
}

LilyAstBodyRecordObjectItem *
parse_method_decl_for_record_object__LilyParser(
  LilyParser *self,
  LilyPreparserRecordObjectBodyItem *item)
{
    PARSE_METHOD_DECL(self, item, LilyAstBodyRecordObjectItem);
}

LilyAstBodyClassItem *
parse_method_decl__LilyParser(LilyParser *self,
                              LilyPreparserClassBodyItem *item)
{
    PARSE_METHOD_DECL(self, item, LilyAstBodyClassItem);
}

LilyAstDecl *
parse_module_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    Vec *body = NEW(Vec); // Vec<LilyAstDecl*>*
    parse_decls__LilyParser(self, body, decl->module.body);

    return NEW_VARIANT(
      LilyAstDecl,
      module,
      decl->location,
      NEW(LilyAstDeclModule, decl->module.name, body, decl->module.visibility));
}

LilyAstDecl *
parse_object_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    switch (decl->object.kind) {
        case LILY_PREPARSER_OBJECT_KIND_CLASS:
            return parse_class_decl__LilyParser(self, decl);
        case LILY_PREPARSER_OBJECT_KIND_ENUM:
            return parse_enum_object_decl__LilyParser(self, decl);
        case LILY_PREPARSER_OBJECT_KIND_RECORD:
            return parse_record_object_decl__LilyParser(self, decl);
        case LILY_PREPARSER_OBJECT_KIND_TRAIT:
            return parse_trait_decl__LilyParser(self, decl);
        default:
            UNREACHABLE("unknown variant");
    }

    return NULL;
}

LilyAstBodyTraitItem *
parse_prototype_decl__LilyParser(LilyParser *self,
                                 LilyPreparserTraitBodyItem *item)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (item->prototype.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, item->prototype.generic_params);
    }

    // 2. Parse params
    Vec *params = NULL;

    if (item->prototype.params) {
        params = NEW(Vec);

        for (Usize i = 0; i < item->prototype.params->len; ++i) {
            LilyParseBlock param_block =
              NEW(LilyParseBlock, self, get__Vec(item->prototype.params, i));
            LilyAstDataType *param =
              parse_data_type__LilyParseBlock(&param_block);

            CHECK_DATA_TYPE(param, param_block, NULL, "expected `,`", {});

            FREE(LilyParseBlock, &param_block);

            if (param) {
                push__Vec(params, param);
            }
        }
    }

    // 3. Parse return data type
    LilyAstDataType *return_data_type = NULL;

    if (item->prototype.return_data_type) {
        LilyParseBlock return_data_type_block =
          NEW(LilyParseBlock, self, item->prototype.return_data_type);
        return_data_type =
          parse_data_type__LilyParseBlock(&return_data_type_block);

        CHECK_DATA_TYPE(
          return_data_type, return_data_type_block, NULL, "expected `;`", {});

        FREE(LilyParseBlock, &return_data_type_block);
    }

    return NEW_VARIANT(LilyAstBodyTraitItem,
                       prototype,
                       item->location,
                       NEW(LilyAstDeclPrototype,
                           item->prototype.name,
                           generic_params,
                           params,
                           return_data_type));
}

LilyAstField *
parse_record_field__LilyParser(LilyParser *self,
                               LilyPreparserRecordBodyItem *item)
{
    LilyParseBlock data_type_block =
      NEW(LilyParseBlock, self, item->field.data_type);
    LilyAstDataType *data_type =
      parse_data_type__LilyParseBlock(&data_type_block);

    CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `:=` or `;`", {
        FREE(LilyParseBlock, &data_type_block);
        return NULL;
    });

    FREE(LilyParseBlock, &data_type_block);

    LilyAstExpr *expr = NULL;

    if (item->field.optional_expr) {
        LilyParseBlock expr_block =
          NEW(LilyParseBlock, self, item->field.optional_expr);
        expr = parse_expr__LilyParseBlock(&expr_block);

        CHECK_EXPR(expr, expr_block, NULL, "expected `;`", {});

        FREE(LilyParseBlock, &expr_block);
    }

    return NEW(LilyAstField,
               item->field.name,
               data_type,
               expr,
               item->field.is_mut,
               item->location);
}

LilyAstDecl *
parse_record_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (decl->type.record.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, decl->type.record.generic_params);
    }

    // 2. Parse body
    Vec *fields = NEW(Vec); // Vec<LilyAstField*>*

    for (Usize i = 0; i < decl->type.record.body->len; ++i) {
        LilyPreparserRecordBodyItem *item = get__Vec(decl->type.record.body, i);

        switch (item->kind) {
            case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND:
                apply_macro_expansion_in_record__LilyParser(self, item, fields);

                break;
            case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD: {
                LilyAstField *field =
                  parse_record_field__LilyParser(self, item);

                if (field) {
                    push__Vec(fields, field);
                }

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }
    }

    return NEW_VARIANT(LilyAstDecl,
                       type,
                       decl->location,
                       NEW_VARIANT(LilyAstDeclType,
                                   record,
                                   NEW(LilyAstDeclRecord,
                                       decl->type.record.name,
                                       generic_params,
                                       fields,
                                       decl->type.record.visibility)));
}

LilyAstBodyRecordObjectItem *
parse_record_object_field__LilyParser(LilyParser *self,
                                      LilyPreparserRecordObjectBodyItem *item)
{
    LilyParseBlock data_type_block =
      NEW(LilyParseBlock, self, item->field.data_type);
    LilyAstDataType *data_type =
      parse_data_type__LilyParseBlock(&data_type_block);

    CHECK_DATA_TYPE(data_type, data_type_block, NULL, "expected `:=` or `;`", {
        FREE(LilyParseBlock, &data_type_block);
        return NULL;
    });

    FREE(LilyParseBlock, &data_type_block);

    LilyAstExpr *expr = NULL;

    if (item->field.optional_expr) {
        LilyParseBlock expr_block =
          NEW(LilyParseBlock, self, item->field.optional_expr);
        expr = parse_expr__LilyParseBlock(&expr_block);

        CHECK_EXPR(expr, expr_block, NULL, "expected `;`", {});

        FREE(LilyParseBlock, &expr_block);
    }

    return NEW_VARIANT(LilyAstBodyRecordObjectItem,
                       field,
                       item->location,
                       NEW(LilyAstFieldObject,
                           item->field.name,
                           data_type,
                           expr,
                           item->field.visibility));
}

Vec *
parse_record_object_body__LilyParser(LilyParser *self, Vec *pre_body)
{
    Vec *body = NEW(Vec); // Vec<LilyAstBodyRecordObjectItem*>*

    for (Usize i = 0; i < pre_body->len; ++i) {
        LilyPreparserRecordObjectBodyItem *item = get__Vec(pre_body, i);

        switch (item->kind) {
            case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT:
                switch (item->constant.kind) {
                    case LILY_PREPARSER_CONSTANT_KIND_SIMPLE: {
                        LilyAstBodyEnumObjectItem *constant =
                          parse_constant_decl_for_enum_object__LilyParser(
                            self, item->constant.simple, &item->location);

                        if (constant) {
                            push__Vec(body, constant);
                        }

                        break;
                    }
                    case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE: {
                        for (Usize i = 0; i < item->constant.multiple->len;
                             ++i) {
                            LilyAstBodyRecordObjectItem *constant =
                              parse_constant_decl_for_record_object__LilyParser(
                                self,
                                get__Vec(item->constant.multiple, i),
                                &item->location);

                            if (constant) {
                                push__Vec(body, constant);
                            }
                        }

                        break;
                    }
                }

                break;
            case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND:
                apply_macro_expansion_in_record_object__LilyParser(
                  self, item, body);

                break;
            case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD: {
                LilyAstBodyRecordObjectItem *method =
                  parse_method_decl_for_record_object__LilyParser(self, item);

                if (method) {
                    push__Vec(body, method);
                }

                break;
            }
            case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD: {
                LilyAstBodyRecordObjectItem *field =
                  parse_record_object_field__LilyParser(self, item);

                if (field) {
                    push__Vec(body, field);
                }

                break;
            }
        }
    }

    return body;
}

LilyAstDecl *
parse_record_object_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (decl->object.record.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, decl->object.record.generic_params);
    }

    // 2. Parse implement params
    Vec *impl_params = NULL;

    if (decl->object.record.implements) {
        impl_params =
          parse_impl_params__LilyParser(self, decl->object.record.implements);
    }

    // 3. Parse body
    Vec *body =
      parse_record_object_body__LilyParser(self, decl->object.record.body);

    return NEW_VARIANT(LilyAstDecl,
                       object,
                       decl->location,
                       NEW_VARIANT(LilyAstDeclObject,
                                   record,
                                   NEW(LilyAstDeclRecordObject,
                                       decl->object.record.name,
                                       generic_params,
                                       impl_params,
                                       body,
                                       decl->object.record.visibility)));
}

Vec *
parse_trait_body__LilyParser(LilyParser *self, Vec *pre_body)
{
    Vec *body = NEW(Vec); // Vec<LilyAstBodyTraitItem*>*

    for (Usize i = 0; i < pre_body->len; ++i) {
        LilyPreparserTraitBodyItem *item = get__Vec(pre_body, i);

        switch (item->kind) {
            case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE: {
                LilyAstBodyTraitItem *res =
                  parse_attribute_decl_for_trait__LilyParser(self, item);

                if (res) {
                    push__Vec(body, res);
                }

                break;
            }
            case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND:
                apply_macro_expansion_in_trait__LilyParser(self, item, body);

                break;
            case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE: {
                LilyAstBodyTraitItem *res =
                  parse_prototype_decl__LilyParser(self, item);

                if (res) {
                    push__Vec(body, res);
                }

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }
    }

    return body;
}

LilyAstDecl *
parse_trait_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    // 1. Parse generic params
    Vec *generic_params = NULL;

    if (decl->object.trait.generic_params) {
        generic_params = parse_generic_params__LilyParser(
          self, decl->object.record.generic_params);
    }

    // 2. Parse implement params
    Vec *inherit_params = NULL;

    if (decl->object.trait.inherits) {
        inherit_params =
          parse_inherit_params__LilyParser(self, decl->object.trait.inherits);
    }

    // 3. Parse body
    Vec *body = parse_trait_body__LilyParser(self, decl->object.trait.body);

    return NEW_VARIANT(LilyAstDecl,
                       object,
                       decl->location,
                       NEW_VARIANT(LilyAstDeclObject,
                                   trait,
                                   NEW(LilyAstDeclTrait,
                                       decl->object.trait.name,
                                       generic_params,
                                       inherit_params,
                                       body,
                                       decl->object.trait.visibility,
                                       decl->object.trait.is_close)));
}

LilyAstDecl *
parse_type_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    switch (decl->type.kind) {
        case LILY_PREPARSER_TYPE_KIND_ALIAS:
            return parse_alias_decl__LilyParser(self, decl);
        case LILY_PREPARSER_TYPE_KIND_ENUM:
            return parse_enum_decl__LilyParser(self, decl);
        case LILY_PREPARSER_TYPE_KIND_RECORD:
            return parse_record_decl__LilyParser(self, decl);
        default:
            UNREACHABLE("unknown variant");
    }

    return NULL;
}

LilyMacro *
search_private_macro__LilyParser(const LilyParser *self, const String *name)
{
    for (Usize i = 0; i < self->package->private_macros->len; ++i) {
        LilyMacro *macro = get__Vec(self->package->private_macros, i);

        if (!strcmp(macro->name->buffer, name->buffer)) {
            return macro;
        }
    }

    return NULL;
}

LilyMacro *
search_public_macro__LilyParser(const LilyParser *self, const String *name)
{
    for (Usize i = 0; i < self->root_package->public_macros->len; ++i) {
        LilyMacro *macro = get__Vec(self->root_package->public_macros, i);

        if (!strcmp(macro->name->buffer, name->buffer)) {
            return macro;
        }
    }

    return NULL;
}

LilyMacro *
search_macro__LilyParser(const LilyParser *self, const String *name)
{
    LilyMacro *private_macro = search_private_macro__LilyParser(self, name);

    if (private_macro) {
        return private_macro;
    }

    LilyMacro *public_macro = search_public_macro__LilyParser(self, name);

    if (public_macro) {
        return public_macro;
    }

    return NULL;
}

bool
is_id__LilyParser(const Vec *tokens)
{
    if (tokens->len == 1) {
        switch (CAST(LilyToken *, get__Vec(tokens, 0))->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            case LILY_TOKEN_KIND_IDENTIFIER_MACRO:
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING:
                return true;
            default:
                return false;
        }
    }

    return false;
}

bool
is_dt__LilyParser(const Vec *tokens)
{
    if (tokens->len >= 1) {
        IS_DATA_TYPE(CAST(LilyToken *, get__Vec(tokens, 0))->kind);
    }

    return false;
}

bool
is_tk__LilyParser(const Vec *tokens)
{
    return tokens->len == 1;
}

bool
is_tks__LilyParser(const Vec *tokens)
{
    return tokens->len >= 3 ? CAST(LilyToken *, get__Vec(tokens, 0))->kind ==
                                  LILY_TOKEN_KIND_DOLLAR &&
                                CAST(LilyToken *, get__Vec(tokens, 1))->kind ==
                                  LILY_TOKEN_KIND_L_BRACE
                            : 0;
}

bool
is_stmt__LilyParser(const Vec *tokens)
{
    if (tokens->len >= 1) {
        switch (CAST(LilyToken *, get__Vec(tokens, 0))->kind) {
            case LILY_TOKEN_KIND_KEYWORD_ASM:
            case LILY_TOKEN_KIND_KEYWORD_AWAIT:
            case LILY_TOKEN_KIND_AT:
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
            case LILY_TOKEN_KIND_KEYWORD_BREAK:
            case LILY_TOKEN_KIND_KEYWORD_DEFER:
            case LILY_TOKEN_KIND_KEYWORD_DROP:
            case LILY_TOKEN_KIND_KEYWORD_FOR:
            case LILY_TOKEN_KIND_KEYWORD_IF:
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
            case LILY_TOKEN_KIND_KEYWORD_NEXT:
            case LILY_TOKEN_KIND_KEYWORD_RETURN:
            case LILY_TOKEN_KIND_KEYWORD_TRY:
            case LILY_TOKEN_KIND_KEYWORD_VAL:
            case LILY_TOKEN_KIND_KEYWORD_MUT:
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
                return true;
            default:
                return false;
        }
    }

    return false;
}

bool
is_expr__LilyParser(const Vec *tokens)
{
    if (tokens->len >= 1) {
        switch (CAST(LilyToken *, get__Vec(tokens, 0))->kind) {
            case LILY_TOKEN_KIND_KEYWORD_TRUE:
            case LILY_TOKEN_KIND_KEYWORD_FALSE:
            case LILY_TOKEN_KIND_KEYWORD_NIL:
            case LILY_TOKEN_KIND_KEYWORD_UNDEF:
            case LILY_TOKEN_KIND_KEYWORD_NONE:
            case LILY_TOKEN_KIND_LITERAL_BYTE:
            case LILY_TOKEN_KIND_LITERAL_BYTES:
            case LILY_TOKEN_KIND_LITERAL_CHAR:
            case LILY_TOKEN_KIND_LITERAL_CSTR:
            case LILY_TOKEN_KIND_LITERAL_FLOAT:
            case LILY_TOKEN_KIND_LITERAL_INT_2:
            case LILY_TOKEN_KIND_LITERAL_INT_8:
            case LILY_TOKEN_KIND_LITERAL_INT_10:
            case LILY_TOKEN_KIND_LITERAL_INT_16:
            case LILY_TOKEN_KIND_LITERAL_STR:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            case LILY_TOKEN_KIND_L_HOOK:
            case LILY_TOKEN_KIND_L_BRACE:
            case LILY_TOKEN_KIND_L_PAREN:
            case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
            case LILY_TOKEN_KIND_KEYWORD_self:
            case LILY_TOKEN_KIND_KEYWORD_SELF:
            case LILY_TOKEN_KIND_AT:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            case LILY_TOKEN_KIND_KEYWORD_FUN:
            case LILY_TOKEN_KIND_STAR:
            case LILY_TOKEN_KIND_KEYWORD_REF:
            case LILY_TOKEN_KIND_KEYWORD_TRACE:
            case LILY_TOKEN_KIND_MINUS:
            case LILY_TOKEN_KIND_KEYWORD_NOT:
            case LILY_TOKEN_KIND_KEYWORD_TRY:
                return true;
            default:
                return false;
        }
    }

    return false;
}

bool
is_path__LilyParser(const Vec *tokens)
{
    if (tokens->len >= 2) {
        switch (CAST(LilyToken *, get__Vec(tokens, 0))->kind) {
            case LILY_TOKEN_KIND_KEYWORD_self:
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
                switch (CAST(LilyToken *, get__Vec(tokens, 1))->kind) {
                    case LILY_TOKEN_KIND_DOT:
                        return true;
                    default:
                        return false;
                }
            default:
                return false;
        }
    }

    return false;
}

bool
is_patt__LilyParser(const Vec *tokens)
{
    if (tokens->len >= 1) {
        switch (CAST(LilyToken *, get__Vec(tokens, 0))->kind) {
            case LILY_TOKEN_KIND_KEYWORD_TRUE:
            case LILY_TOKEN_KIND_KEYWORD_FALSE:
            case LILY_TOKEN_KIND_KEYWORD_NIL:
            case LILY_TOKEN_KIND_KEYWORD_UNDEF:
            case LILY_TOKEN_KIND_KEYWORD_NONE:
            case LILY_TOKEN_KIND_LITERAL_BYTE:
            case LILY_TOKEN_KIND_LITERAL_BYTES:
            case LILY_TOKEN_KIND_LITERAL_CHAR:
            case LILY_TOKEN_KIND_LITERAL_CSTR:
            case LILY_TOKEN_KIND_LITERAL_FLOAT:
            case LILY_TOKEN_KIND_LITERAL_INT_2:
            case LILY_TOKEN_KIND_LITERAL_INT_8:
            case LILY_TOKEN_KIND_LITERAL_INT_10:
            case LILY_TOKEN_KIND_LITERAL_INT_16:
            case LILY_TOKEN_KIND_LITERAL_STR:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
            case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            case LILY_TOKEN_KIND_L_HOOK:
            case LILY_TOKEN_KIND_L_BRACE:
            case LILY_TOKEN_KIND_KEYWORD_ERROR:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
            case LILY_TOKEN_KIND_KEYWORD_SELF:
            case LILY_TOKEN_KIND_L_PAREN:
            case LILY_TOKEN_KIND_DOT_DOT:
                return true;
            default:
                return false;
        }
    }

    return false;
}

bool
is_block__LilyParser(const Vec *tokens)
{
    if (tokens->len >= 1) {
        switch (CAST(LilyToken *, get__Vec(tokens, 0))->kind) {
            case LILY_TOKEN_KIND_AT:
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
                return true;
            default:
                return false;
        }
    }

    return false;
}

#define CHECK_MACRO(decl)                                                          \
    /* 1. Looks for the macro. */                                                  \
    LilyMacro *macro =                                                             \
      search_macro__LilyParser(self, decl->macro_expand.name);                     \
                                                                                   \
    if (!macro) {                                                                  \
        emit__Diagnostic(                                                          \
          NEW_VARIANT(Diagnostic,                                                  \
                      simple_lily_error,                                           \
                      &self->package->file,                                        \
                      &decl->location,                                             \
                      NEW(LilyError, LILY_ERROR_KIND_MACRO_IS_NOT_FOUND),          \
                      NULL,                                                        \
                      NULL,                                                        \
                      NULL),                                                       \
          &self->package->count_error);                                            \
                                                                                   \
        return;                                                                    \
    }                                                                              \
                                                                                   \
    /* 2. Check params of macro. Then replace the macro identifier(s) by the       \
    value(s) passed in the parameters of the expansion macro. */                   \
    Vec macro_tokens_copy = { .buffer = lily_malloc(macro->tokens->capacity *      \
                                                    PTR_SIZE),                     \
                              .len = macro->tokens->len,                           \
                              .capacity = macro->tokens->capacity,                 \
                              .default_capacity = macro->tokens->capacity };       \
    Vec *expand_tokens = NULL; /* Vec<LilyToken*?>*? */                            \
                                                                                   \
    memcpy(macro_tokens_copy.buffer,                                               \
           macro->tokens->buffer,                                                  \
           macro->tokens->capacity *PTR_SIZE);                                     \
                                                                                   \
    if (decl->macro_expand.params && macro->params) {                              \
        if (decl->macro_expand.params->len > macro->params->len) {                 \
            emit__Diagnostic(                                                      \
              NEW_VARIANT(                                                         \
                Diagnostic,                                                        \
                simple_lily_error,                                                 \
                &self->package->file,                                              \
                &decl->location,                                                   \
                NEW(LilyError,                                                     \
                    LILY_ERROR_KIND_MACRO_EXPAND_HAVE_TOO_MANY_PARAMS),            \
                NULL,                                                              \
                NULL,                                                              \
                format__String(                                                    \
                  "expected {d} parameters, obtained {d} parameters",              \
                  macro->params->len,                                              \
                  decl->macro_expand.params->len)),                                \
              &self->package->count_error);                                        \
                                                                                   \
            return;                                                                \
        } else if (decl->macro_expand.params->len < macro->params->len) {          \
            emit__Diagnostic(                                                      \
              NEW_VARIANT(                                                         \
                Diagnostic,                                                        \
                simple_lily_error,                                                 \
                &self->package->file,                                              \
                &decl->location,                                                   \
                NEW(LilyError, LILY_ERROR_KIND_MACRO_EXPAND_MISS_FEW_PARAMS),      \
                NULL,                                                              \
                NULL,                                                              \
                format__String(                                                    \
                  "expected {d} parameters, obtained {d} parameters",              \
                  macro->params->len,                                              \
                  decl->macro_expand.params->len)),                                \
              &self->package->count_error);                                        \
                                                                                   \
            return;                                                                \
        } else {                                                                   \
            /* NOTE: macro->params and macro_expand.params have the same           \
             * length. */                                                          \
            expand_tokens = NEW(Vec);                                              \
                                                                                   \
            for (Usize i = 0; i < macro->params->len; ++i) {                       \
                LilyMacroParam *param = get__Vec(macro->params, i);                \
                                                                                   \
                /* Checks if the values passed as parameters are valid with        \
                respect to the type that is specified in the macro                 \
                declaration. */                                                    \
                switch (param->kind) {                                             \
                    case LILY_MACRO_PARAM_KIND_ID:                                 \
                        if (!is_id__LilyParser(                                    \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_ID),            \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_DT:                                 \
                        if (!is_dt__LilyParser(                                    \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_DT),            \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_TK:                                 \
                        if (!is_tk__LilyParser(                                    \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_TK),            \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_TKS:                                \
                        if (!is_tks__LilyParser(                                   \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_TKS),           \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_STMT:                               \
                        if (!is_stmt__LilyParser(                                  \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_STMT),          \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_EXPR:                               \
                        if (!is_expr__LilyParser(                                  \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_EXPR),          \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_PATH:                               \
                        if (!is_path__LilyParser(                                  \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_PATH),          \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_PATT:                               \
                        if (!is_patt__LilyParser(                                  \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_PATT),          \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    case LILY_MACRO_PARAM_KIND_BLOCK:                              \
                        if (!is_block__LilyParser(                                 \
                              get__Vec(decl->macro_expand.params, i))) {           \
                            emit__Diagnostic(                                      \
                              NEW_VARIANT(                                         \
                                Diagnostic,                                        \
                                simple_lily_error,                                 \
                                &self->package->file,                              \
                                &decl->location,                                   \
                                NEW(LilyError,                                     \
                                    LILY_ERROR_KIND_MACRO_EXPECTED_BLOCK),         \
                                NULL,                                              \
                                NULL,                                              \
                                format__String("at parameter number {d}",          \
                                               i + 1)),                            \
                              &self->package->count_error);                        \
                                                                                   \
                            continue;                                              \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                    default:                                                       \
                        UNREACHABLE("unknown variant");                            \
                }                                                                  \
                                                                                   \
                /* Fill in the vector of expansion tokens to be able to save       \
                 * them locally and free them at the end of the analysis of        \
                 * the macro. */                                                   \
                for (Usize i = 0; i < macro->params->len; ++i) {                   \
                    Location location = clone__Location(                           \
                      &CAST(LilyToken *,                                           \
                            CAST(Vec *, decl->macro_expand.params->buffer[i])      \
                              ->buffer[0])                                         \
                         ->location);                                              \
                    {                                                              \
                        LilyToken *last =                                          \
                          last__Vec(decl->macro_expand.params->buffer[i]);         \
                        end__Location(&location,                                   \
                                      last->location.end_line,                     \
                                      last->location.end_column,                   \
                                      last->location.end_position);                \
                    }                                                              \
                    switch (CAST(LilyMacroParam *, get__Vec(macro->params, i))     \
                              ->kind) {                                            \
                        case LILY_MACRO_PARAM_KIND_EXPR:                           \
                            push__Vec(                                             \
                              expand_tokens,                                       \
                              NEW_VARIANT(                                         \
                                LilyToken,                                         \
                                expand,                                            \
                                location,                                          \
                                NEW(LilyTokenExpand,                               \
                                    LILY_TOKEN_EXPAND_KIND_EXPR,                   \
                                    get__Vec(decl->macro_expand.params, i))));     \
                            break;                                                 \
                        case LILY_MACRO_PARAM_KIND_PATT:                           \
                            push__Vec(                                             \
                              expand_tokens,                                       \
                              NEW_VARIANT(                                         \
                                LilyToken,                                         \
                                expand,                                            \
                                location,                                          \
                                NEW(LilyTokenExpand,                               \
                                    LILY_TOKEN_EXPAND_KIND_PATT,                   \
                                    get__Vec(decl->macro_expand.params, i))));     \
                            break;                                                 \
                        case LILY_MACRO_PARAM_KIND_PATH:                           \
                            push__Vec(                                             \
                              expand_tokens,                                       \
                              NEW_VARIANT(                                         \
                                LilyToken,                                         \
                                expand,                                            \
                                location,                                          \
                                NEW(LilyTokenExpand,                               \
                                    LILY_TOKEN_EXPAND_KIND_PATH,                   \
                                    get__Vec(decl->macro_expand.params, i))));     \
                            break;                                                 \
                        case LILY_MACRO_PARAM_KIND_DT:                             \
                            push__Vec(                                             \
                              expand_tokens,                                       \
                              NEW_VARIANT(                                         \
                                LilyToken,                                         \
                                expand,                                            \
                                location,                                          \
                                NEW(LilyTokenExpand,                               \
                                    LILY_TOKEN_EXPAND_KIND_DT,                     \
                                    get__Vec(decl->macro_expand.params, i))));     \
                            break;                                                 \
                        default:                                                   \
                            push__Vec(expand_tokens, NULL);                        \
                    }                                                              \
                }                                                                  \
                                                                                   \
                /* Replaces all uses of the parameter in the macro with the        \
                value passed in the macro expansion. */                            \
                                                                                   \
                for (Usize i = 0; i < macro_tokens_copy.len;) {                    \
                    LilyToken *token = get__Vec(&macro_tokens_copy, i);            \
                                                                                   \
                    /* Looks for identifier macro. */                              \
                    switch (token->kind) {                                         \
                        case LILY_TOKEN_KIND_IDENTIFIER_MACRO:                     \
                            /* Checks if the macro identifier matches a            \
                            parameter in the macro declaration, otherwise          \
                            issues an error saying that `the macro identifier      \
                            is not found`. */                                      \
                            for (Usize j = 0; j < macro->params->len; ++j) {       \
                                LilyMacroParam *param =                            \
                                  get__Vec(macro->params, j);                      \
                                                                                   \
                                if (!strcmp(token->identifier_macro->buffer,       \
                                            param->name->buffer)) {                \
                                    Vec *macro_expand_param =                      \
                                      get__Vec(decl->macro_expand.params, j);      \
                                                                                   \
                                    remove__Vec(&macro_tokens_copy, i);            \
                                                                                   \
                                    /* See if it is possible to push otherwise     \
                                    inserted at the position of the                \
                                    `identifier_macro`. */                         \
                                    if (i > macro_tokens_copy.len) {               \
                                        switch (param->kind) {                     \
                                            case LILY_MACRO_PARAM_KIND_TKS:        \
                                                for (Usize k = 2;                  \
                                                     k <                           \
                                                     macro_expand_param->len -     \
                                                       1;                          \
                                                     ++k) {                        \
                                                    push__Vec(                     \
                                                      &macro_tokens_copy,          \
                                                      get__Vec(                    \
                                                        macro_expand_param,        \
                                                        k));                       \
                                                }                                  \
                                                break;                             \
                                            case LILY_MACRO_PARAM_KIND_EXPR:       \
                                            case LILY_MACRO_PARAM_KIND_PATT:       \
                                            case LILY_MACRO_PARAM_KIND_PATH:       \
                                            case LILY_MACRO_PARAM_KIND_DT:         \
                                                push__Vec(                         \
                                                  &macro_tokens_copy,              \
                                                  get__Vec(expand_tokens, j));     \
                                                break;                             \
                                            default:                               \
                                                for (Usize k = 0;                  \
                                                     k <                           \
                                                     macro_expand_param->len;      \
                                                     ++k) {                        \
                                                    push__Vec(                     \
                                                      &macro_tokens_copy,          \
                                                      get__Vec(                    \
                                                        macro_expand_param,        \
                                                        k));                       \
                                                }                                  \
                                        }                                          \
                                    } else {                                       \
                                        switch (param->kind) {                     \
                                            case LILY_MACRO_PARAM_KIND_TKS:        \
                                                for (Usize k = 2;                  \
                                                     k <                           \
                                                     macro_expand_param->len -     \
                                                       1;                          \
                                                     ++k) {                        \
                                                    insert__Vec(                   \
                                                      &macro_tokens_copy,          \
                                                      get__Vec(                    \
                                                        macro_expand_param,        \
                                                        k),                        \
                                                      i + (k - 2));                \
                                                }                                  \
                                                break;                             \
                                            case LILY_MACRO_PARAM_KIND_EXPR:       \
                                            case LILY_MACRO_PARAM_KIND_PATT:       \
                                            case LILY_MACRO_PARAM_KIND_PATH:       \
                                            case LILY_MACRO_PARAM_KIND_DT:         \
                                                insert__Vec(                       \
                                                  &macro_tokens_copy,              \
                                                  get__Vec(expand_tokens, j),      \
                                                  i);                              \
                                                break;                             \
                                            default:                               \
                                                for (Usize k = 0;                  \
                                                     k <                           \
                                                     macro_expand_param->len;      \
                                                     ++k) {                        \
                                                    insert__Vec(                   \
                                                      &macro_tokens_copy,          \
                                                      get__Vec(                    \
                                                        macro_expand_param,        \
                                                        k),                        \
                                                      i + k);                      \
                                                }                                  \
                                        }                                          \
                                    }                                              \
                                                                                   \
                                    i += macro_expand_param->len;                  \
                                                                                   \
                                    goto exit_loop;                                \
                                }                                                  \
                            }                                                      \
                                                                                   \
                            {                                                      \
                                const File *file =                                 \
                                  get_file_from_filename__LilyPackage(             \
                                    self->root_package,                            \
                                    macro->location.filename);                     \
                                                                                   \
                                emit__Diagnostic(                                  \
                                  NEW_VARIANT(                                     \
                                    Diagnostic,                                    \
                                    simple_lily_error,                             \
                                    file,                                          \
                                    &token->location,                              \
                                    NEW(                                           \
                                      LilyError,                                   \
                                      LILY_ERROR_KIND_MACRO_IDENTIFIER_NOT_FOUND), \
                                    NULL,                                          \
                                    NULL,                                          \
                                    format__String(                                \
                                      "unknown macro identifier named {S}",        \
                                      token->identifier_macro)),                   \
                                  &self->package->count_error);                    \
                                                                                   \
                                return;                                            \
                            }                                                      \
                                                                                   \
                        exit_loop: {                                               \
                        }                                                          \
                                                                                   \
                        break;                                                     \
                        default:                                                   \
                            break;                                                 \
                    }                                                              \
                                                                                   \
                    ++i;                                                           \
                }                                                                  \
            }                                                                      \
        }                                                                          \
    } else if (decl->macro_expand.params || macro->params) {                       \
        if (decl->macro_expand.params) {                                           \
            emit__Diagnostic(                                                      \
              NEW_VARIANT(                                                         \
                Diagnostic,                                                        \
                simple_lily_error,                                                 \
                &self->package->file,                                              \
                &decl->location,                                                   \
                NEW(LilyError,                                                     \
                    LILY_ERROR_KIND_MACRO_EXPAND_HAVE_TOO_MANY_PARAMS),            \
                NULL,                                                              \
                NULL,                                                              \
                NULL),                                                             \
              &self->package->count_error);                                        \
        } else {                                                                   \
            emit__Diagnostic(                                                      \
              NEW_VARIANT(                                                         \
                Diagnostic,                                                        \
                simple_lily_error,                                                 \
                &self->package->file,                                              \
                &decl->location,                                                   \
                NEW(LilyError, LILY_ERROR_KIND_MACRO_EXPAND_MISS_FEW_PARAMS),      \
                NULL,                                                              \
                NULL,                                                              \
                NULL),                                                             \
              &self->package->count_error);                                        \
        }                                                                          \
                                                                                   \
        return;                                                                    \
    }                                                                              \
                                                                                   \
    if (self->package->count_error > 0) {                                          \
        return;                                                                    \
    }

#define CLEAN_UP_CHECK_MACRO(id, dt)                       \
    FREE_BUFFER_ITEMS(id->buffer, id->len, dt);            \
    FREE(Vec, id);                                         \
    lily_free(macro_tokens_copy.buffer);                   \
    if (expand_tokens) {                                   \
        for (Usize i = 0; i < expand_tokens->len; ++i) {   \
            LilyToken *token = get__Vec(expand_tokens, i); \
            if (token) {                                   \
                FREE(LilyToken, token);                    \
            }                                              \
        }                                                  \
        FREE(Vec, expand_tokens);                          \
    }

void
apply_macro_expansion_in_record__LilyParser(LilyParser *self,
                                            LilyPreparserRecordBodyItem *item,
                                            Vec *body)
{
    CHECK_MACRO(item);

    // 3. Prepare and parse the content of the macro, then expand
    // it.
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        preparse_macro_expand.current = get__Vec(&macro_tokens_copy, 0);

        Vec *pre_record_body_items =
          preparse_record_body__LilyPreparser(&preparse_macro_expand);

        if (!pre_record_body_items) {
            return;
        }

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser = (LilyParser){ .decls = NULL,
                                          .package = package,
                                          .root_package = self->root_package,
                                          .current = NULL,
                                          .preparser_info = NULL,
                                          .position = 0 };

        for (Usize i = 0; i < pre_record_body_items->len; ++i) {
            LilyAstField *field = parse_record_field__LilyParser(
              &parser, get__Vec(pre_record_body_items, i));

            if (field) {
                push__Vec(body, field);
            }
        }

        CLEAN_UP_CHECK_MACRO(pre_record_body_items,
                             LilyPreparserRecordBodyItem);
    }
}

void
apply_macro_expansion_in_enum__LilyParser(LilyParser *self,
                                          LilyPreparserEnumBodyItem *item,
                                          Vec *body)
{
    CHECK_MACRO(item);

    /* 3. Prepare and parse the content of the macro, then expand
    it. */
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        preparse_macro_expand.current = get__Vec(&macro_tokens_copy, 0);

        Vec *pre_enum_body_items =
          preparse_enum_body__LilyPreparser(&preparse_macro_expand);

        if (!pre_enum_body_items) {
            return;
        }

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser = (LilyParser){ .decls = NULL,
                                          .package = package,
                                          .root_package = self->root_package,
                                          .current = NULL,
                                          .preparser_info = NULL,
                                          .position = 0 };

        for (Usize i = 0; i < pre_enum_body_items->len; ++i) {
            LilyAstVariant *variant = parse_enum_variant__LilyParser(
              &parser, get__Vec(pre_enum_body_items, i));

            if (variant) {
                push__Vec(body, variant);
            }
        }

        CLEAN_UP_CHECK_MACRO(pre_enum_body_items, LilyPreparserEnumBodyItem);
    }
}

void
apply_macro_expansion_in_class__LilyParser(LilyParser *self,
                                           LilyPreparserClassBodyItem *item,
                                           Vec *body)
{
    CHECK_MACRO(item);

    // 3. Prepare and parse the content of the macro, then expand
    // it.
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        preparse_macro_expand.current = get__Vec(&macro_tokens_copy, 0);

        Vec *pre_class_body_items =
          preparse_class_body__LilyPreparser(&preparse_macro_expand);

        if (!pre_class_body_items) {
            return;
        }

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser = (LilyParser){ .decls = NULL,
                                          .package = package,
                                          .root_package = self->root_package,
                                          .current = NULL,
                                          .preparser_info = NULL,
                                          .position = 0 };

        Vec *expand_body =
          parse_class_body__LilyParser(&parser, pre_class_body_items);

        append__Vec(body, expand_body);

        CLEAN_UP_CHECK_MACRO(pre_class_body_items, LilyPreparserClassBodyItem);
        FREE(Vec, expand_body);
    }
}

void
apply_macro_expansion_in_record_object__LilyParser(
  LilyParser *self,
  LilyPreparserRecordObjectBodyItem *item,
  Vec *body)
{
    CHECK_MACRO(item);

    // 3. Prepare and parse the content of the macro, then expand
    // it.
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        preparse_macro_expand.current = get__Vec(&macro_tokens_copy, 0);

        Vec *pre_record_object_body_items =
          preparse_record_object_body__LilyPreparser(&preparse_macro_expand);

        if (!pre_record_object_body_items) {
            return;
        }

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser = (LilyParser){ .decls = NULL,
                                          .package = package,
                                          .root_package = self->root_package,
                                          .current = NULL,
                                          .preparser_info = NULL,
                                          .position = 0 };

        Vec *expand_body = parse_record_object_body__LilyParser(
          &parser, pre_record_object_body_items);

        append__Vec(body, expand_body);

        CLEAN_UP_CHECK_MACRO(pre_record_object_body_items,
                             LilyPreparserRecordObjectBodyItem);
        FREE(Vec, expand_body);
    }
}

void
apply_macro_expansion_in_enum_object__LilyParser(
  LilyParser *self,
  LilyPreparserEnumObjectBodyItem *item,
  Vec *body)
{
    CHECK_MACRO(item);

    // 3. Prepare and parse the content of the macro, then expand
    // it.
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        preparse_macro_expand.current = get__Vec(&macro_tokens_copy, 0);

        Vec *pre_enum_object_body_items =
          preparse_enum_object_body__LilyPreparser(&preparse_macro_expand);

        if (!pre_enum_object_body_items) {
            return;
        }

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser = (LilyParser){ .decls = NULL,
                                          .package = package,
                                          .root_package = self->root_package,
                                          .current = NULL,
                                          .preparser_info = NULL,
                                          .position = 0 };

        Vec *expand_body = parse_enum_object_body__LilyParser(
          &parser, pre_enum_object_body_items);

        append__Vec(body, expand_body);

        CLEAN_UP_CHECK_MACRO(pre_enum_object_body_items,
                             LilyPreparserEnumObjectBodyItem);
        FREE(Vec, expand_body);
    }
}

void
apply_macro_expansion_in_trait__LilyParser(LilyParser *self,
                                           LilyPreparserTraitBodyItem *item,
                                           Vec *body)
{
    CHECK_MACRO(item);

    // 3. Prepare and parse the content of the macro, then expand
    // it.
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        preparse_macro_expand.current = get__Vec(&macro_tokens_copy, 0);

        Vec *pre_trait_body_items =
          preparse_trait_body__LilyPreparser(&preparse_macro_expand);

        if (!pre_trait_body_items) {
            return;
        }

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser = (LilyParser){ .decls = NULL,
                                          .package = package,
                                          .root_package = self->root_package,
                                          .current = NULL,
                                          .preparser_info = NULL,
                                          .position = 0 };

        Vec *expand_body =
          parse_trait_body__LilyParser(&parser, pre_trait_body_items);

        append__Vec(body, expand_body);

        CLEAN_UP_CHECK_MACRO(pre_trait_body_items, LilyPreparserTraitBodyItem);
        FREE(Vec, expand_body);
    }
}

bool
must_close_macro_expand__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_EOF;
}

void
apply_macro_expansion_in_fun__LilyParser(LilyParser *self,
                                         LilyPreparserFunBodyItem *item,
                                         Vec *body)
{
    CHECK_MACRO(item);

    // 3. Prepare and parse the content of the macro, then expand
    // it.
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        preparse_macro_expand.current = get__Vec(&macro_tokens_copy, 0);

        Vec *pre_fun_body_items = NEW(Vec);

        while (
          !must_close_macro_expand__LilyPreparser(&preparse_macro_expand)) {
            LilyPreparserFunBodyItem *pre_item = preparse_block__LilyPreparser(
              &preparse_macro_expand,
              &must_close_macro_expand__LilyPreparser,
              true);

            if (pre_item) {
                push__Vec(pre_fun_body_items, pre_item);
            }
        }

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser = (LilyParser){ .decls = NULL,
                                          .package = package,
                                          .root_package = self->root_package,
                                          .current = NULL,
                                          .preparser_info = NULL,
                                          .position = 0 };

        Vec *expand_body =
          parse_fun_body__LilyParser(&parser, pre_fun_body_items);

        append__Vec(body, expand_body);

        CLEAN_UP_CHECK_MACRO(pre_fun_body_items, LilyPreparserFunBodyItem);
        FREE(Vec, expand_body);
    }
}

void
apply_macro_expansion__LilyParser(LilyParser *self,
                                  LilyPreparserDecl *decl,
                                  Vec *decls)
{
    CHECK_MACRO(decl);

    // 3. Prepare, precompiler and parse the content of the macro, then expand
    // it.
    {
        const File *file = get_file_from_filename__LilyPackage(
          self->root_package, macro->location.filename);
        LilyPreparserInfo preparser_info = NEW(LilyPreparserInfo, NULL);
        LilyPreparser preparse_macro_expand =
          NEW(LilyPreparser, file, &macro_tokens_copy, NULL, false);

        run__LilyPreparser(&preparse_macro_expand, &preparser_info);

        LilyPrecompiler precompiler =
          NEW(LilyPrecompiler,
              &preparser_info,
              &self->package->file,
              self->package,
              self->package->precompiler.default_path);

        run__LilyPrecompiler(&precompiler, self->root_package, true);

        LilyPackage *package = search_package_from_filename__LilyPackage(
          self->root_package, file->name);
        LilyParser parser =
          NEW(LilyParser, package, self->root_package, &preparser_info);

        run__LilyParser(&parser, true);

        for (Usize i = 0; i < parser.decls->len; ++i) {
            push__Vec(decls, get__Vec(parser.decls, i));
        }

        // Clean up allocations

        FREE(LilyPreparserInfo, &preparser_info);
        FREE(Vec, parser.decls);
        lily_free(macro_tokens_copy.buffer);

        if (expand_tokens) {
            for (Usize i = 0; i < expand_tokens->len; ++i) {
                LilyToken *token = get__Vec(expand_tokens, i);

                if (token) {
                    FREE(LilyToken, token);
                }
            }

            FREE(Vec, expand_tokens);
        }
    }
}

LilyAstDecl *
parse_decl__LilyParser(LilyParser *self, LilyPreparserDecl *decl)
{
    switch (decl->kind) {
        case LILY_PREPARSER_DECL_KIND_CONSTANT: {
            switch (decl->constant.kind) {
                case LILY_PREPARSER_CONSTANT_KIND_SIMPLE:
                    return parse_constant_decl__LilyParser(
                      self, decl->constant.simple, &decl->location);
                case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE:
                    for (Usize i = 0; i < decl->constant.multiple->len; ++i) {
                        LilyAstDecl *constant = parse_constant_decl__LilyParser(
                          self,
                          get__Vec(decl->constant.multiple, i),
                          &decl->location);

                        if (constant) {
                            push__Vec(self->decls, constant);
                        }
                    }

                    return NULL;
            }
        }
        case LILY_PREPARSER_DECL_KIND_ERROR:
            return parse_error_decl__LilyParser(self, decl);
        case LILY_PREPARSER_DECL_KIND_FUN:
            return parse_fun_decl__LilyParser(self, decl);
        case LILY_PREPARSER_DECL_KIND_MODULE:
            return parse_module_decl__LilyParser(self, decl);
        case LILY_PREPARSER_DECL_KIND_OBJECT:
            return parse_object_decl__LilyParser(self, decl);
        case LILY_PREPARSER_DECL_KIND_TYPE:
            return parse_type_decl__LilyParser(self, decl);
        case LILY_PREPARSER_DECL_KIND_USE:
            return parse_use_decl__LilyParser(self, decl);
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyParser,
            LilyParser,
            LilyPackage *package,
            LilyPackage *root_package,
            const LilyPreparserInfo *preparser_info)
{
    return (LilyParser){ .decls = NEW(Vec),
                         .package = package,
                         .root_package = root_package,
                         .current = NULL,
                         .preparser_info = preparser_info
                                             ? preparser_info
                                             : &package->preparser_info,
                         .position = 0 };
}

TEST(LilyAstDataType *, parse_data_type, LilyParseBlock *self)
{
    return parse_data_type__LilyParseBlock(self);
}

TEST(LilyAstExpr *, parse_expr, LilyParseBlock *self)
{
    return parse_expr__LilyParseBlock(self);
}

void
parse_decls__LilyParser(LilyParser *self, Vec *decls, const Vec *pre_decls)
{
    for (Usize i = 0; i < pre_decls->len; ++i) {
        LilyPreparserDecl *pre_decl = get__Vec(pre_decls, i);

        switch (pre_decl->kind) {
            case LILY_PREPARSER_DECL_KIND_MACRO_EXPAND:
                apply_macro_expansion__LilyParser(self, pre_decl, decls);

                break;
            default: {
                LilyAstDecl *decl = parse_decl__LilyParser(self, pre_decl);

                if (decl) {
                    push__Vec(decls, decl);
                }
            }
        }
    }
}

void
run__LilyParser(LilyParser *self, bool parse_for_macro_expand)
{
    parse_decls__LilyParser(self, self->decls, self->preparser_info->decls);

    if (!parse_for_macro_expand) {
#ifdef DEBUG_PARSER
        printf("====Parser(%s)====\n", self->package->file.name);

        for (Usize i = 0; i < self->decls->len; ++i) {
            CALL_DEBUG(LilyAstDecl, get__Vec(self->decls, i));
        }
#endif

        switch (self->package->kind) {
            case LILY_PACKAGE_KIND_COMPILER:
                if (self->package->compiler.config->dump_parser) {
                    TODO("dump parser");
                }

                break;
            case LILY_PACKAGE_KIND_INTERPRETER:
                // TODO: maybe add dump parser
                if (false) {
                    TODO("interpreter: maybe dump parser");
                }

                break;
            case LILY_PACKAGE_KIND_JIT:
                // TODO: maybe add dump parser
                if (false) {
                    TODO("interpreter: maybe dump jit");
                }

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    if (self->package->count_error > 0) {
        exit(1);
    }
}

DESTRUCTOR(LilyParser, const LilyParser *self)
{
    FREE_BUFFER_ITEMS(self->decls->buffer, self->decls->len, LilyAstDecl);
    FREE(Vec, self->decls);
}
