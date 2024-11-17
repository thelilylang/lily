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
#include <base/format.h>

#include <core/cc/ci/include.h>
#include <core/cc/ci/resolver.h>
#include <core/cc/ci/resolver/expr.h>
#include <core/cc/ci/result.h>
#include <core/shared/diagnostic.h>

/// @brief Init the CIResolver type.
static void
init__CIResolver(CIResolver *self, CIResolvedTokens *resolved_tokens);

/// @brief Add resolved token to `resolved_tokens` field.
/// @param resolved_token CIToken*
static inline void
add_resolved_token__CIResolver(CIResolver *self, CIToken *resolved_token);

static inline void
set_current_token__CIResolver(CIResolver *self, CIToken *token);

static inline void
set_macro_call__CIResolver(CIResolver *self, CIResolverMacroCall *macro_call);

static inline void
set_look_for_keyword__CIResolver(CIResolver *self, bool look_for_keyword);

static void
parse_macro_call_target__CIResolver(CIResolver *self,
                                    enum CITokenKind target,
                                    bool macro_param_is_used);

/// @param current_token - Current token of parent CIResolver.
static void
parse_tokens_in_macro_call_param__CIResolver(CIResolver *self,
                                             CIToken **current_token,
                                             bool is_variadic,
                                             bool is_used);

static CIResolverMacroCallParam *
parse_macro_call_param__CIResolver(CIResolver *self,
                                   CIToken **current_token,
                                   bool is_variadic,
                                   bool is_used);

/// @param macro_param_variadic Index of the first variadic macro param. If the
/// macro has no variadic parameter, the value `-1` is assigned.
/// @param define const CIResultDefine* (&)
/// @see `get_variadic_param_index__CITokenPreprocessorDefine` function in
/// `include/core/cc/ci/token.h`
static CIResolverMacroCall *
parse_macro_call_params__CIResolver(CIResolver *self,
                                    CIToken **current_token,
                                    Isize macro_param_variadic,
                                    Usize macro_params_length,
                                    const CIResultDefine *define);

static void
resolve_macro_call__CIResolver(CIResolver *self,
                               CIToken *identifier_token,
                               const CIResultDefine *define);

static void
resolve_identifier__CIResolver(CIResolver *self, CIToken *identifier_token);

static inline bool
is_macro_defined__CIResolver(CIResolver *self, String *macro_name);

static void
resolve_macro_defined__CIResolver(CIResolver *self,
                                  CIToken *macro_defined_token);

static void
resolve_macro_param__CIResolver(CIResolver *self, CIToken *macro_param_token);

static void
resolve_macro_param_variadic__CIResolver(CIResolver *self,
                                         CIToken *macro_param_variadic_token);

static void
resolve_preprocessor_define__CIResolver(CIResolver *self,
                                        CIToken *preprocessor_define_token);

static void
next_conditional_preprocessor__CIResolver(CIResolver *self);

static CIExpr *
resolve_preprocessor_if_cond__CIResolver(CIResolver *self, CITokens *cond);

static void
resolve_next_conditional_preprocessor__CIResolver(CIResolver *self,
                                                  CITokens *content,
                                                  bool is_if_branch);

/// @param cond CITokens* (&)
/// @param content CITokens* (&) Can be the content of if or elif branch
static void
resolve_preprocessor_if__CIResolver(CIResolver *self,
                                    CITokens *cond,
                                    bool is_if_branch,
                                    CITokens *content);

/// @param identifier const String* (&)
/// @param content CITokens* (&) Can be the content of if or elif branch
static void
resolve_preprocessor_ifdef__CIResolver(CIResolver *self,
                                       String *identifier,
                                       bool is_if_branch,
                                       CITokens *content,
                                       bool reverse);

static void
resolve_preprocessor_else__CIResolver(CIResolver *self, CITokens *content);

static void
resolve_preprocessor_conditional__CIResolver(CIResolver *self,
                                             CIToken *preprocessor_cond_token);

static void
resolve_preprocessor_embed__CIResolver(CIResolver *self,
                                       CIToken *preprocessor_embed_token);

static void
resolve_preprocessor_error__CIResolver(CIResolver *self,
                                       CIToken *preprocessor_error_token);

static bool
load_include__CIResolver(CIResolver *self,
                         CIToken *preprocessor_include_token,
                         const String *current_dir,
                         const String *include_path);

static void
resolve_preprocessor_include__CIResolver(CIResolver *self,
                                         CIToken *preprocessor_include_token);

static void
resolve_preprocessor_line__CIResolver(CIResolver *self,
                                      CIToken *preprocessor_line_token);

static void
resolve_preprocessor_pragma__CIResolver(CIResolver *self,
                                        CIToken *preprocessor_pragma_token);

static void
resolve_preprocessor_undef__CIResolver(CIResolver *self,
                                       CIToken *preprocessor_undef_token);

static void
resolve_preprocessor_warning__CIResolver(CIResolver *self,
                                         CIToken *preprocessor_warning_token);

static CIToken *
perform_stringification__CIResolver(CIResolver *self,
                                    CIResolvedTokens *resolved_tokens);

static void
resolve_stringification__CIResolver(CIResolver *self);

/// @return String* (&)
static String *
get_merged_id_lhs_content__CIResolver(CIResolver *self, CIToken *lhs);

/// @return String* (&)
static String *
get_merged_id_rhs_content__CIResolver(CIResolver *self, CIToken *rhs);

static CIToken *
perform_merged_id__CIResolver(CIResolver *self,
                              CIToken *merged_id_lhs,
                              String *merged_id_lhs_content,
                              String *merged_id_rhs_content,
                              Location new_token_location);

static void
resolve_merged_id__CIResolver(CIResolver *self);

/// @brief Resolve token such as preprocessor, <id>##<id2>, ...
static void
resolve_token__CIResolver(CIResolver *self);

/// @brief Advance to the next token.
static void
next_token__CIResolver(CIResolver *self);

#define RESOLVE_TOKENS(tokens, resolved_tokens, initializer, action)        \
    {                                                                       \
        CIResolver _resolver = NEW(CIResolver,                              \
                                   self->file,                              \
                                   (tokens),                                \
                                   self->count_error,                       \
                                   self->count_warning);                    \
        initializer;                                                        \
                                                                            \
        run__CIResolver(                                                    \
          &_resolver,                                                       \
          resolved_tokens ? ref__CIResolvedTokens(resolved_tokens) : NULL); \
                                                                            \
        action;                                                             \
                                                                            \
        FREE(CIResolver, &_resolver);                                       \
    }

#define HAS_REACH_END(r) ((r)->current_token == (r)->tokens->last)

CONSTRUCTOR(CIResolvedTokens *, CIResolvedTokens)
{
    CIResolvedTokens *self = lily_malloc(sizeof(CIResolvedTokens));

    self->content = NEW(Vec);
    self->ref_count = 0;

    return self;
}

void
merge__CIResolvedTokens(const CIResolvedTokens *self,
                        const CIResolvedTokens *other)
{
    for (Usize i = 0; i < count__CIResolvedTokens(other); ++i) {
        add__CIResolvedTokens(self,
                              ref__CIToken(get__CIResolvedTokens(other, i)));
    }
}

void
insert_after_many__CIResolvedTokens(const CIResolvedTokens *self,
                                    const CIResolvedTokens *other,
                                    Usize index)
{
    for (Usize i = 0; i < count__CIResolvedTokens(other); ++i) {
        insert_after__CIResolvedTokens(
          self, ref__CIToken(get__CIResolvedTokens(other, i)), index + i);
    }
}

void
pop_if_eof__CIResolvedTokens(const CIResolvedTokens *self)
{
    if (count__CIResolvedTokens(self) > 0) {
        CIToken *last_token = last__CIResolvedTokens(self);

        if (last_token->kind == CI_TOKEN_KIND_EOF) {
            FREE(CIToken, pop__Vec(self->content));
        }
    }
}

DESTRUCTOR(CIResolvedTokens, CIResolvedTokens *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    FREE_BUFFER_ITEMS(self->content->buffer, self->content->len, CIToken);
    FREE(Vec, self->content);
    lily_free(self);
}

CONSTRUCTOR(CIResolverMacroCallParam *,
            CIResolverMacroCallParam,
            CIResolvedTokens *resolved_content)
{
    CIResolverMacroCallParam *self =
      lily_malloc(sizeof(CIResolverMacroCallParam));

    self->resolved_content = resolved_content;
    self->next = NULL;

    return self;
}

DESTRUCTOR(CIResolverMacroCallParam, CIResolverMacroCallParam *self)
{
    FREE(CIResolvedTokens, self->resolved_content);
    lily_free(self);
}

void
add__CIResolverMacroCallParams(CIResolverMacroCallParams *self,
                               CIResolverMacroCallParam *param)
{
    if (!self->first) {
        self->first = param;
        self->last = param;

        return;
    }

    self->last->next = param;
    self->last = self->last->next;
    ++self->len;
}

CIResolverMacroCallParam *
get__CIResolverMacroCallParams(const CIResolverMacroCallParams *self,
                               Usize index)
{
    CIResolverMacroCallParam *param = self->first;

    for (Usize i = 0; i < index; ++i) {
        param = param->next;
    }

    ASSERT(param);

    return param;
}

DESTRUCTOR(CIResolverMacroCallParams, const CIResolverMacroCallParams *self)
{
    CIResolverMacroCallParam *current = self->first;

    while (current) {
        CIResolverMacroCallParam *prev = current;

        current = current->next;

        FREE(CIResolverMacroCallParam, prev);
    }
}

CONSTRUCTOR(CIResolverMacroCall *, CIResolverMacroCall)
{
    CIResolverMacroCall *self = lily_malloc(sizeof(CIResolverMacroCall));

    self->params = NEW(CIResolverMacroCallParams);
    self->is_empty = false;
    self->ref_count = 0;

    return self;
}

VARIANT_CONSTRUCTOR(CIResolverMacroCall *, CIResolverMacroCall, is_empty)
{
    CIResolverMacroCall *self = lily_malloc(sizeof(CIResolverMacroCall));

    // self->params = <undefined>
    self->is_empty = true;
    self->ref_count = 0;

    return self;
}

DESTRUCTOR(CIResolverMacroCall, CIResolverMacroCall *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    if (!self->is_empty) {
        FREE(CIResolverMacroCallParams, &self->params);
    }

    lily_free(self);
}

void
init__CIResolver(CIResolver *self, CIResolvedTokens *resolved_tokens)
{
    ASSERT(self->tokens->first);

    self->current_token = self->tokens->first;
    self->resolved_tokens = resolved_tokens;
}

void
add_resolved_token__CIResolver(CIResolver *self, CIToken *resolved_token)
{
    add__CIResolvedTokens(self->resolved_tokens, resolved_token);
}

void
set_current_token__CIResolver(CIResolver *self, CIToken *token)
{
    ASSERT(token);

    self->current_token = token;
}

void
set_macro_call__CIResolver(CIResolver *self, CIResolverMacroCall *macro_call)
{
    ASSERT(macro_call);

    self->macro_call = macro_call;
}

void
set_look_for_keyword__CIResolver(CIResolver *self, bool look_for_keyword)
{
    self->look_for_keyword = look_for_keyword;
}

void
parse_macro_call_target__CIResolver(CIResolver *self,
                                    enum CITokenKind target,
                                    bool macro_param_is_used)
{
    next_token__CIResolver(self);

loop:
    resolve_token__CIResolver(self);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LPAREN:
            parse_macro_call_target__CIResolver(
              self, CI_TOKEN_KIND_LPAREN, macro_param_is_used);

            goto loop;
        case CI_TOKEN_KIND_LBRACE:
            parse_macro_call_target__CIResolver(
              self, CI_TOKEN_KIND_LBRACE, macro_param_is_used);

            goto loop;
        case CI_TOKEN_KIND_LHOOK:
            parse_macro_call_target__CIResolver(
              self, CI_TOKEN_KIND_LHOOK, macro_param_is_used);

            goto loop;
        case CI_TOKEN_KIND_RPAREN:
        case CI_TOKEN_KIND_RBRACE:
        case CI_TOKEN_KIND_RHOOK: {
            static enum CITokenKind
              match_target_with_current[CI_TOKEN_KIND_MAX] = {
                  [CI_TOKEN_KIND_LPAREN] = CI_TOKEN_KIND_RPAREN,
                  [CI_TOKEN_KIND_LBRACE] = CI_TOKEN_KIND_RBRACE,
                  [CI_TOKEN_KIND_LHOOK] = CI_TOKEN_KIND_RHOOK
              };

            if (match_target_with_current[target] ==
                self->current_token->kind) {
                next_token__CIResolver(self);

                return;
            } else {
                FAILED("`)`, `}` or `]` is not expected");
            }

            break;
        }
        default:
            break;
    }

    next_token__CIResolver(self);

    goto loop;
}

void
parse_tokens_in_macro_call_param__CIResolver(CIResolver *self,
                                             CIToken **current_token,
                                             bool is_variadic,
                                             bool is_used)
{
    // In cases where the macro parameter is not variadic, we want the parser to
    // stop before the comma or the closing parenthesis. On the other hand, in
    // cases where the macro parameter is variadic, we want the parser to stop
    // only before the closing parenthesis.
    while ((self->current_token->kind != CI_TOKEN_KIND_COMMA || is_variadic) &&
           self->current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        resolve_token__CIResolver(self);

        if (self->current_token->kind == CI_TOKEN_KIND_LPAREN ||
            self->current_token->kind == CI_TOKEN_KIND_LBRACE ||
            self->current_token->kind == CI_TOKEN_KIND_LHOOK) {
            parse_macro_call_target__CIResolver(
              self, self->current_token->kind, is_used);
        } else {
            next_token__CIResolver(self);
        }
    }

    *current_token = self->current_token;
}

CIResolverMacroCallParam *
parse_macro_call_param__CIResolver(CIResolver *self,
                                   CIToken **current_token,
                                   bool is_variadic,
                                   bool is_used)
{
    CIResolver macro_call_param_resolver = NEW(CIResolver,
                                               self->file,
                                               self->tokens,
                                               self->count_error,
                                               self->count_warning);

    init__CIResolver(&macro_call_param_resolver, NEW(CIResolvedTokens));

    if (self->macro_call) {
        set_macro_call__CIResolver(&macro_call_param_resolver,
                                   ref__CIResolverMacroCall(self->macro_call));
    }

    set_current_token__CIResolver(&macro_call_param_resolver, *current_token);
    parse_tokens_in_macro_call_param__CIResolver(
      &macro_call_param_resolver, current_token, is_variadic, is_used);

    CIResolverMacroCallParam *param =
      NEW(CIResolverMacroCallParam,
          ref__CIResolvedTokens(macro_call_param_resolver.resolved_tokens));

    FREE(CIResolver, &macro_call_param_resolver);

    return param;
}

CIResolverMacroCall *
parse_macro_call_params__CIResolver(CIResolver *self,
                                    CIToken **current_token,
                                    Isize macro_param_variadic,
                                    Usize macro_params_length,
                                    const CIResultDefine *define)
{
#define NEXT(ct) ct = ct->kind != CI_TOKEN_KIND_EOF ? ct->next : ct
#define PEEK(ct) ct->next
#define EXPECT(ct, k)               \
    if (ct->kind != k) {            \
        FAILED("unexpected token"); \
    }
#define CURRENT(ct) ct

    NEXT((*current_token)); // skip `(`

    CIResolverMacroCall *macro_call = NEW(CIResolverMacroCall);
    Usize macro_param_count = 0;

    while (CURRENT((*current_token))->kind != CI_TOKEN_KIND_RPAREN &&
           CURRENT((*current_token))->kind != CI_TOKEN_KIND_EOF) {
        bool is_variadic = macro_param_variadic != -1 &&
                           macro_param_count >= macro_param_variadic;
        CIResolverMacroCallParam *param = parse_macro_call_param__CIResolver(
          self,
          current_token,
          is_variadic,
          is_variadic
            ? CAST(CITokenPreprocessorDefineParam *,
                   last__Vec(define->define->params))
                ->is_used
            : CAST(CITokenPreprocessorDefineParam *,
                   get__Vec(define->define->params, macro_param_count))
                ->is_used);

        if (CURRENT((*current_token))->kind != CI_TOKEN_KIND_RPAREN) {
            EXPECT((*current_token), CI_TOKEN_KIND_COMMA);
            NEXT((*current_token));
        }

        add__CIResolverMacroCallParams(&macro_call->params, param);

        ++macro_param_count;
    }

    EXPECT((*current_token), CI_TOKEN_KIND_RPAREN);

    if (macro_param_count != macro_params_length) {
        FAILED("The count of the number of macro parameters does not "
               "correspond to its declaration.");
    }

    return macro_call;
}

void
resolve_macro_call__CIResolver(CIResolver *self,
                               CIToken *identifier_token,
                               const CIResultDefine *define)
{
    ASSERT(define);

    CIToken *next_token = identifier_token;
    Isize macro_param_variadic =
      get_variadic_param_index__CITokenPreprocessorDefine(define->define);
    Usize macro_params_length =
      define->define->params ? define->define->params->len : 0;
    CIToken *peeked = PEEK(next_token);

    if (peeked) {
        CIResolverMacroCall *macro_call;

        switch (peeked->kind) {
            case CI_TOKEN_KIND_LPAREN:
                NEXT(next_token); // skip maco identifier
                macro_call =
                  parse_macro_call_params__CIResolver(self,
                                                      &next_token,
                                                      macro_param_variadic,
                                                      macro_params_length,
                                                      define);

                break;
            default:
                if (macro_params_length != 0) {
                    FAILED("The count of the number of macro parameters "
                           "does not "
                           "correspond to its declaration.");
                }

                macro_call = NEW_VARIANT(CIResolverMacroCall, is_empty);
        }

        RESOLVE_TOKENS(
          &define->define->tokens,
          NULL,
          { set_macro_call__CIResolver(&_resolver, macro_call); },
          {
              merge__CIResolvedTokens(self->resolved_tokens,
                                      _resolver.resolved_tokens);
          });

        set_current_token__CIResolver(self, next_token);

        return;
    }

    FAILED("expected to have at least EOF");

#undef NEXT
#undef PEEK
#undef EXPECT
#undef CURRENT
}

void
resolve_identifier__CIResolver(CIResolver *self, CIToken *identifier_token)
{
    if (self->look_for_keyword) {
        enum CITokenKind new_token_kind = get_keyword__CIScanner(
          GET_PTR_RC(String, identifier_token->identifier));

        if (new_token_kind != CI_TOKEN_KIND_IDENTIFIER) {
            return add_resolved_token__CIResolver(
              self, NEW(CIToken, new_token_kind, identifier_token->location));
        }
    }

    const CIResultDefine *define = NULL;

    if ((define = get_define__CIResultFile(
           self->file, GET_PTR_RC(String, identifier_token->identifier)))) {
        return resolve_macro_call__CIResolver(self, identifier_token, define);
    }

    add_resolved_token__CIResolver(self, ref__CIToken(identifier_token));
}

bool
is_macro_defined__CIResolver(CIResolver *self, String *macro_name)
{
    return get_define__CIResultFile(self->file, macro_name) ||
           is_standard_predefined_macro__CIScanner(macro_name);
}

void
resolve_macro_defined__CIResolver(CIResolver *self,
                                  CIToken *macro_defined_token)
{
    bool is_def =
      is_macro_defined__CIResolver(self, macro_defined_token->macro_defined);

    add_resolved_token__CIResolver(
      self,
      NEW_VARIANT(CIToken,
                  literal_constant_int,
                  clone__Location(&macro_defined_token->location),
                  NEW(CITokenLiteralConstantInt,
                      CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
                      from__String(is_def ? "1" : "0"))));
}

void
resolve_macro_param__CIResolver(CIResolver *self, CIToken *macro_param_token)
{
    ASSERT(self->macro_call);

    CIResolverMacroCallParam *param = get__CIResolverMacroCallParams(
      &self->macro_call->params, macro_param_token->macro_param.id);

    merge__CIResolvedTokens(self->resolved_tokens, param->resolved_content);
}

void
resolve_macro_param_variadic__CIResolver(CIResolver *self,
                                         CIToken *macro_param_variadic_token)
{
    ASSERT(self->macro_call);

    CIResolverMacroCallParam *param =
      get_macro_param_variadic__CIResolverMacroCallParams(
        &self->macro_call->params);

    merge__CIResolvedTokens(self->resolved_tokens, param->resolved_content);
}

void
resolve_preprocessor_define__CIResolver(CIResolver *self,
                                        CIToken *preprocessor_define_token)
{
    CIResultDefine *define =
      NEW(CIResultDefine,
          &preprocessor_define_token->preprocessor_define,
          NEW(CIFileID, self->file->entity.id, self->file->kind));

    if (add_define__CIResultFile(self->file, define)) {
        // TODO: check if the both macros are the same or raise an error

        FREE(CIResultDefine, define);
    }
}

void
next_conditional_preprocessor__CIResolver(CIResolver *self)
{
    ASSERT(self->current_token);

    // Move onto the next conditional preprocessor.
    while (self->current_token->kind != CI_TOKEN_KIND_EOT) {
        if (is_conditional_preprocessor__CITokenKind(
              self->current_token->kind) &&
            self->current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_IF &&
            self->current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_IFDEF &&
            self->current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_IFNDEF) {
            return;
        }

        next_token__CIResolver(self);
    }
}

CIExpr *
resolve_preprocessor_if_cond__CIResolver(CIResolver *self, CITokens *cond)
{
    CIExpr *resolved_cond_expr = NULL;

    RESOLVE_TOKENS(cond, NULL, {}, {
        CIParser parser_cond = NEW(CIParser, self->file);

        init__CIParser(&parser_cond, _resolver.resolved_tokens);

        CIExpr *cond_expr = parse_expr__CIParser(&parser_cond);

        if (!has_reach_end__CIParser(&parser_cond)) {
            FAILED("expected only one expression");
        }

        const CIResolverExpr resolver_expr =
          NEW(CIResolverExpr, NULL, NULL, true);

        resolved_cond_expr = run__CIResolverExpr(&resolver_expr, cond_expr);

        FREE(CIExpr, cond_expr);
        FREE(CIParser, &parser_cond);
    });

    return resolved_cond_expr;
}

void
resolve_next_conditional_preprocessor__CIResolver(CIResolver *self,
                                                  CITokens *content,
                                                  bool is_if_branch)
{
    CIResolver preprocessor_if_content_resolver; // CIResolver | <undefined>
    CIResolver *preprocessor_if_content_resolver_ref = self; // CIResolver* (&)

    if (is_if_branch) {
        preprocessor_if_content_resolver = NEW(CIResolver,
                                               self->file,
                                               content,
                                               self->count_error,
                                               self->count_warning);
        init__CIResolver(&preprocessor_if_content_resolver,
                         ref__CIResolvedTokens(self->resolved_tokens));
        preprocessor_if_content_resolver_ref =
          &preprocessor_if_content_resolver;
    } else {
        next_token__CIResolver(self);
    }

    next_conditional_preprocessor__CIResolver(
      preprocessor_if_content_resolver_ref);

    if (!HAS_REACH_END(preprocessor_if_content_resolver_ref)) {
        resolve_preprocessor_conditional__CIResolver(
          preprocessor_if_content_resolver_ref,
          preprocessor_if_content_resolver_ref->current_token);
    }

    if (is_if_branch) {
        FREE(CIResolver, &preprocessor_if_content_resolver);
    }
}

void
resolve_preprocessor_if__CIResolver(CIResolver *self,
                                    CITokens *cond,
                                    bool is_if_branch,
                                    CITokens *content)
{
    {
        CIExpr *cond_expr =
          resolve_preprocessor_if_cond__CIResolver(self, cond);
        bool cond_res = is_true__CIResolverExpr(cond_expr);

        FREE(CIExpr, cond_expr);

        if (cond_res) {
            RESOLVE_TOKENS(content, self->resolved_tokens, {}, {});

            return;
        }
    }

    resolve_next_conditional_preprocessor__CIResolver(
      self, content, is_if_branch);
}

void
resolve_preprocessor_ifdef__CIResolver(CIResolver *self,
                                       String *identifier,
                                       bool is_if_branch,
                                       CITokens *content,
                                       bool reverse)
{
    {
        bool is_def = is_macro_defined__CIResolver(self, identifier);

        if ((is_def && !reverse) || (!is_def && reverse)) {
            RESOLVE_TOKENS(content, self->resolved_tokens, {}, {});

            return;
        }
    }

    resolve_next_conditional_preprocessor__CIResolver(
      self, content, is_if_branch);
}

void
resolve_preprocessor_else__CIResolver(CIResolver *self, CITokens *content)
{
    RESOLVE_TOKENS(content, self->resolved_tokens, {}, {});
}

void
resolve_preprocessor_conditional__CIResolver(CIResolver *self,
                                             CIToken *preprocessor_cond_token)
{
    switch (preprocessor_cond_token->kind) {
        case CI_TOKEN_KIND_PREPROCESSOR_IF: {
            CITokens *content_if =
              &preprocessor_cond_token->preprocessor_if.content;

            return resolve_preprocessor_if__CIResolver(
              self,
              &preprocessor_cond_token->preprocessor_if.cond,
              true,
              content_if);
        }
        case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
            return resolve_preprocessor_if__CIResolver(
              self,
              &preprocessor_cond_token->preprocessor_elif.cond,
              false,
              &preprocessor_cond_token->preprocessor_elif.content);
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
            return resolve_preprocessor_ifdef__CIResolver(
              self,
              preprocessor_cond_token->preprocessor_elifdef.identifier,
              false,
              &preprocessor_cond_token->preprocessor_elifdef.content,
              false);
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
            return resolve_preprocessor_ifdef__CIResolver(
              self,
              preprocessor_cond_token->preprocessor_elifndef.identifier,
              false,
              &preprocessor_cond_token->preprocessor_elifndef.content,
              true);
        case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
            return resolve_preprocessor_else__CIResolver(
              self, &preprocessor_cond_token->preprocessor_else.content);
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF: {
            CITokens *content_if =
              &preprocessor_cond_token->preprocessor_ifdef.content;

            return resolve_preprocessor_ifdef__CIResolver(
              self,
              preprocessor_cond_token->preprocessor_ifdef.identifier,
              true,
              content_if,
              false);
        }
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF: {
            CITokens *content_if =
              &preprocessor_cond_token->preprocessor_ifndef.content;

            return resolve_preprocessor_ifdef__CIResolver(
              self,
              preprocessor_cond_token->preprocessor_ifndef.identifier,
              true,
              content_if,
              true);
        }
        default:
            return;
    }
}

void
resolve_preprocessor_embed__CIResolver(CIResolver *self,
                                       CIToken *preprocessor_embed_token)
{
    TODO("preprocessor embed");
}

void
resolve_preprocessor_error__CIResolver(CIResolver *self,
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

bool
load_include__CIResolver(CIResolver *self,
                         CIToken *preprocessor_include_token,
                         const String *current_dir,
                         const String *include_path)
{
    // current_dir + '/' + include_path
    char *full_include_path = format("{S}/{S}", current_dir, include_path);

    if (exists__File(full_include_path)) {
        CIResultFile *header =
          add_and_run_header__CIResult(self->file->entity.result,
                                       self->file,
                                       full_include_path,
                                       self->file->entity.result->config);

        RESOLVE_TOKENS(&header->scanner.tokens, self->resolved_tokens, {}, {});

        pop_if_eof__CIResolvedTokens(self->resolved_tokens);

        lily_free(full_include_path);

        return true;
    }

    lily_free(full_include_path);

    return false;
}

void
resolve_preprocessor_include__CIResolver(CIResolver *self,
                                         CIToken *preprocessor_include_token)
{
    const Vec *include_dirs = get_include_dirs__CIInclude();

    for (Usize i = 0; i < include_dirs->len; ++i) {
        const String *current_dir = get__Vec(include_dirs, i);

        if (load_include__CIResolver(
              self,
              preprocessor_include_token,
              current_dir,
              preprocessor_include_token->preprocessor_include.value)) {
            return;
        }
    }

    String *current_dir =
      get_dir__File(preprocessor_include_token->location.filename);
    bool load_include_res = load_include__CIResolver(
      self,
      preprocessor_include_token,
      current_dir,
      preprocessor_include_token->preprocessor_include.value);

    FREE(String, current_dir);

    if (load_include_res) {
        return;
    }

    FAILED("the include file is not found");
}

void
resolve_preprocessor_line__CIResolver(CIResolver *self,
                                      CIToken *preprocessor_line_token)
{
    TODO("preprocessor line");
}

void
resolve_preprocessor_pragma__CIResolver(CIResolver *self,
                                        CIToken *preprocessor_pragma_token)
{
    TODO("preprocessor pragma");
}

void
resolve_preprocessor_undef__CIResolver(CIResolver *self,
                                       CIToken *preprocessor_undef_token)
{
    undef_define__CIResultFile(self->file,
                               preprocessor_undef_token->preprocessor_undef);
}

void
resolve_preprocessor_warning__CIResolver(CIResolver *self,
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

void
resolve_standard_predefined_macro_date__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_date_token)
{
    TODO("__DATE__");
}

void
resolve_standard_predefined_macro_file__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_file_token)
{
    TODO("__FILE__");
}

void
resolve_standard_predefined_macro_line__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_line_token)
{
    TODO("__LINE__");
}

void
resolve_standard_predefined_macro_stdc__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_stdc_token)
{
    TODO("__STDC__");
}

void
resolve_standard_predefined_macro_stdc_version__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_stdc_version_token)
{
    TODO("__STDC_VERSION__");
}

void
resolve_standard_predefined_macro_stdc_hosted__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_stdc_hosted_token)
{
    TODO("__STDC_HOSTED__");
}

void
resolve_standard_predefined_macro_time__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_time_token)
{
    TODO("__TIME__");
}

CIToken *
perform_stringification__CIResolver(CIResolver *self,
                                    CIResolvedTokens *resolved_tokens)
{
    Usize resolved_tokens_index = 0;
    Usize resolved_tokens_count = count__CIResolvedTokens(resolved_tokens);
    CIToken *current_token =
      get__CIResolvedTokens(resolved_tokens, resolved_tokens_index);
    Location location = clone__Location(&current_token->location);
    String *res = NEW(String);

    while (current_token) {
        String *s = NULL;

        switch (current_token->kind) {
            case CI_TOKEN_KIND_IDENTIFIER:
                s =
                  clone__String(GET_PTR_RC(String, current_token->identifier));

                break;
            case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
                s =
                  format__String("{S}{s}",
                                 current_token->literal_constant_int.value,
                                 to_string__CITokenLiteralConstantIntSuffix(
                                   current_token->literal_constant_int.suffix));

                break;
            case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
                s = format__String(
                  "{S}{s}",
                  current_token->literal_constant_float.value,
                  to_string__CITokenLiteralConstantFloatSuffix(
                    current_token->literal_constant_float.suffix));

                break;
            case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
                s = format__String(
                  "0o{S}{s}",
                  current_token->literal_constant_octal.value,
                  to_string__CITokenLiteralConstantIntSuffix(
                    current_token->literal_constant_octal.suffix));

                break;
            case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
                s =
                  format__String("0x{S}{s}",
                                 current_token->literal_constant_hex.value,
                                 to_string__CITokenLiteralConstantIntSuffix(
                                   current_token->literal_constant_hex.suffix));

                break;
            case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
                s =
                  format__String("0b{S}{s}",
                                 current_token->literal_constant_hex.value,
                                 to_string__CITokenLiteralConstantIntSuffix(
                                   current_token->literal_constant_hex.suffix));

                break;
            case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
                s = format__String("{c}",
                                   current_token->literal_constant_character);

                break;
            case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
                s = format__String(
                  "\"{S}\"",
                  GET_PTR_RC(String, current_token->literal_constant_string));

                break;
            case CI_TOKEN_KIND_EOT:
                break;
            default: {
                s = to_string__CIToken(current_token);
            }
        }

        if (s) {
            APPEND_AND_FREE(res, s);
            push__String(res, ' ');
        }

        if (++resolved_tokens_index < resolved_tokens_count) {
            current_token =
              get__CIResolvedTokens(resolved_tokens, resolved_tokens_index);
        } else {
            break;
        }
    }

    pop__String(res); // Remove the extra space
    end__Location(&location,
                  current_token->location.end_line,
                  current_token->location.end_column,
                  current_token->location.end_position);

    return NEW_VARIANT(
      CIToken, literal_constant_string, location, NEW(Rc, res));
}

void
resolve_stringification__CIResolver(CIResolver *self)
{
    CIToken *macro_param_token = self->current_token->next;

    ASSERT(macro_param_token &&
           (macro_param_token->kind == CI_TOKEN_KIND_MACRO_PARAM ||
            macro_param_token->kind == CI_TOKEN_KIND_MACRO_PARAM_VARIADIC));

    CIResolverMacroCallParam *param =
      macro_param_token->kind == CI_TOKEN_KIND_MACRO_PARAM
        ? get__CIResolverMacroCallParams(&self->macro_call->params,
                                         macro_param_token->macro_param.id)
        : get_macro_param_variadic__CIResolverMacroCallParams(
            &self->macro_call->params);

    CIToken *stringification_res =
      perform_stringification__CIResolver(self, param->resolved_content);

    add_resolved_token__CIResolver(self, stringification_res);
    next_token__CIResolver(self); // Skip `#`
}

String *
get_merged_id_lhs_content__CIResolver(CIResolver *self, CIToken *lhs)
{
    // TODO: Need to manage keyword.
    switch (lhs->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return GET_PTR_RC(String, lhs->identifier);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return lhs->literal_constant_bin.value;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return lhs->literal_constant_float.value;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return lhs->literal_constant_int.value;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return lhs->literal_constant_hex.value;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return lhs->literal_constant_octal.value;
        default:
            FAILED("not expected to be lhs of `##`");
    }
}

String *
get_merged_id_rhs_content__CIResolver(CIResolver *self, CIToken *rhs)
{
    // TODO: Need to manage keyword.
    switch (rhs->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return GET_PTR_RC(String, rhs->identifier);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return rhs->literal_constant_int.value;
        default:
            FAILED("not expected to be rhs of `##`");
    }
}

CIToken *
perform_merged_id__CIResolver(CIResolver *self,
                              CIToken *merged_id_lhs,
                              String *merged_id_lhs_content,
                              String *merged_id_rhs_content,
                              Location new_token_location)
{
    // TODO: Add a check after the merged of token according the kind of LHS
    // token.
    String *merged_id =
      format__String("{S}{S}", merged_id_lhs_content, merged_id_rhs_content);

    switch (merged_id_lhs->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return NEW_VARIANT(
              CIToken, identifier, new_token_location, NEW(Rc, merged_id));
#define NEW_INTEGER_TOKEN(k)                                   \
    NEW_VARIANT(CIToken,                                       \
                literal_constant_##k,                          \
                new_token_location,                            \
                NEW(CITokenLiteralConstantInt,                 \
                    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE, \
                    merged_id));

        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return NEW_INTEGER_TOKEN(bin);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return NEW_INTEGER_TOKEN(int);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return NEW_INTEGER_TOKEN(hex);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return NEW_INTEGER_TOKEN(octal);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return NEW_VARIANT(CIToken,
                               literal_constant_float,
                               new_token_location,
                               NEW(CITokenLiteralConstantFloat,
                                   CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_NONE,
                                   merged_id));
        default:
            UNREACHABLE("unknown variant");
    }

#undef NEW_INTEGER_TOKEN
}

void
resolve_merged_id__CIResolver(CIResolver *self)
{
    if (count__CIResolvedTokens(self->resolved_tokens) > 0) {
        Usize last_token_index =
          count__CIResolvedTokens(self->resolved_tokens) - 1;

        ++self->count_merged_id;

        next_token__CIResolver(self);
        resolve_token__CIResolver(self);

        CIToken *merged_id_lhs =
          get__CIResolvedTokens(self->resolved_tokens, last_token_index);

        if (last_token_index + 1 >=
            count__CIResolvedTokens(self->resolved_tokens)) {
            FAILED("expected right parameter to `##`");
        }

        CIToken *merged_id_rhs =
          get__CIResolvedTokens(self->resolved_tokens, last_token_index + 1);
        String *lhs_content =
          get_merged_id_lhs_content__CIResolver(self, merged_id_lhs);
        String *rhs_content =
          get_merged_id_rhs_content__CIResolver(self, merged_id_rhs);
        Location new_token_location = clone__Location(&merged_id_lhs->location);

        end__Location(&new_token_location,
                      merged_id_rhs->location.end_line,
                      merged_id_rhs->location.end_column,
                      merged_id_rhs->location.end_position);

        CIToken *new_token = perform_merged_id__CIResolver(
          self, merged_id_lhs, lhs_content, rhs_content, new_token_location);

        replace__CIResolvedTokens(
          self->resolved_tokens, last_token_index, new_token);
        FREE(CIToken, merged_id_lhs);
        FREE(CIToken,
             remove__CIResolvedTokens(self->resolved_tokens,
                                      last_token_index + 1));

        if (--self->count_merged_id == 0) {
            CITokens identifier_resolver_tokens = NEW(CITokens);
            Location eof_location = clone__Location(&new_token_location);

            start__Location(&eof_location,
                            eof_location.end_line,
                            eof_location.end_column,
                            new_token_location.end_position);

            CIToken *eof_token = NEW(CIToken, CI_TOKEN_KIND_EOF, eof_location);

            add__CITokens(&identifier_resolver_tokens, new_token);
            add__CITokens(&identifier_resolver_tokens, eof_token);

            RESOLVE_TOKENS(
              &identifier_resolver_tokens,
              NULL,
              { set_look_for_keyword__CIResolver(&_resolver, true); },
              {
                  pop_if_eof__CIResolvedTokens(_resolver.resolved_tokens);
                  insert_after_many__CIResolvedTokens(self->resolved_tokens,
                                                      _resolver.resolved_tokens,
                                                      last_token_index);
              });

            FREE(CIToken, eof_token);

            new_token->next = NULL;
        }

        FREE(CIToken,
             remove__CIResolvedTokens(self->resolved_tokens, last_token_index));

        return;
    }

    FAILED("## is not expected at start");
}

void
resolve_token__CIResolver(CIResolver *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return resolve_identifier__CIResolver(self, self->current_token);
        case CI_TOKEN_KIND_MACRO_DEFINED:
            return resolve_macro_defined__CIResolver(self, self->current_token);
        case CI_TOKEN_KIND_MACRO_PARAM:
            return resolve_macro_param__CIResolver(self, self->current_token);
        case CI_TOKEN_KIND_MACRO_PARAM_VARIADIC:
            return resolve_macro_param_variadic__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
            return resolve_preprocessor_define__CIResolver(self,
                                                           self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_IF:
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
            return resolve_preprocessor_conditional__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
            return resolve_preprocessor_embed__CIResolver(self,
                                                          self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
            return resolve_preprocessor_error__CIResolver(self,
                                                          self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
            return resolve_preprocessor_include__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_LINE:
            return resolve_preprocessor_line__CIResolver(self,
                                                         self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_PRAGMA:
            return resolve_preprocessor_pragma__CIResolver(self,
                                                           self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
            return resolve_preprocessor_undef__CIResolver(self,
                                                          self->current_token);
        case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
            return resolve_preprocessor_warning__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_HASHTAG:
            return resolve_stringification__CIResolver(self);
        case CI_TOKEN_KIND_HASHTAG_HASHTAG:
            return resolve_merged_id__CIResolver(self);
        case CI_TOKEN_KIND_EOT:
        case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
            // NOTE: We don't add this kind of token to `resolved_tokens` field.
            return;
        default:
            add_resolved_token__CIResolver(self,
                                           ref__CIToken(self->current_token));
    }
}

void
next_token__CIResolver(CIResolver *self)
{
    if (self->current_token && self->current_token->next) {
        self->current_token = self->current_token->next;
    }
}

void
run__CIResolver(CIResolver *self, CIResolvedTokens *resolved_tokens)
{
    init__CIResolver(self,
                     resolved_tokens ? resolved_tokens : NEW(CIResolvedTokens));

    ASSERT(self->resolved_tokens);
    ASSERT(self->tokens->last);

    while (!HAS_REACH_END(self)) {
        resolve_token__CIResolver(self);
        next_token__CIResolver(self);
    }

    if (self->current_token == self->tokens->last) {
        resolve_token__CIResolver(self);
    }

    if (*self->count_error > 0) {
        exit(1);
    }
}

DESTRUCTOR(CIResolver, const CIResolver *self)
{
    if (self->resolved_tokens) {
        FREE(CIResolvedTokens, self->resolved_tokens);
    }

    if (self->macro_call) {
        FREE(CIResolverMacroCall, self->macro_call);
    }
}
