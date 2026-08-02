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
#include <base/dir.h>
#include <base/format.h>

#include <core/cc/ci/diagnostic/emit.h>
#include <core/cc/ci/include.h>
#include <core/cc/ci/resolver.h>

// Emit a located error on the token the resolver is on and count it, without
// stopping the resolve: the caller carries on so that a single run reports more
// than one error. The stage boundary in `run_file__CIResult` stops the
// pipeline.
#define FAILED__CIResolver(self, error_kind)                          \
    EMIT_ERROR__CI(get_file_from_location__CIResultFile(              \
                     (self)->file, &(self)->current_token->location), \
                   &(self)->current_token->location,                  \
                   NEW(CIError, error_kind),                          \
                   (self)->count_error)
#include <core/cc/ci/resolver/expr.h>
#include <core/cc/ci/result.h>
#include <core/shared/diagnostic.h>

// Files written with `#pragma once`, which are included the first time they
// are written and passed over afterwards.
static Vec *included_once = NULL; // Vec<String*>*?

/// @brief Whether the file at the given path is written to be included once
/// and has been already.
static bool
is_included_once__CIResolver(const char *path);

/// @brief Record that the file at the given path is written to be included
/// once.
static void
add_included_once__CIResolver(const char *path);

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
/// @brief Split what a macro parameter stands for on the commas written
/// at the
/// top of it, each part becoming a parameter of the call of its own.
///
/// A macro written in the body of another is given the tokens the parameters
/// of the latter stand for, and those tokens are read where the body is read
/// again, so a comma among them separates what the call is given as much as
/// one written by hand does.
///
/// e.g. #define VIA(...) TAKE1(__VA_ARGS__, 0)
///      VIA(1, 2) gives TAKE1 three parameters: 1, 2 and 0.
///
/// The tokens a macro written on its own stands for are given as they are, as
/// what a call is given is split before it is expanded.
///
/// e.g. #define PAIR 1, 2
///      TAKE1(PAIR, 0) gives TAKE1 two parameters: PAIR and 0.
/// @return The number of parameters the call gained.
static Usize
add_split_param__CIResolver(CIResolverMacroCallParams *params,
                            CIResolverMacroCallParam *param)
{
    CIResolvedTokens *content = param->resolved_content;
    Usize content_count = count__CIResolvedTokens(content);
    Usize depth = 0;
    bool has_comma = false;

    for (Usize i = 0; i < content_count; ++i) {
        const CIToken *token = get__CIResolvedTokens(content, i);

        switch (token->kind) {
            case CI_TOKEN_KIND_LPAREN:
            case CI_TOKEN_KIND_LHOOK:
            case CI_TOKEN_KIND_LBRACE:
                ++depth;

                break;
            case CI_TOKEN_KIND_RPAREN:
            case CI_TOKEN_KIND_RHOOK:
            case CI_TOKEN_KIND_RBRACE:
                if (depth > 0) {
                    --depth;
                }

                break;
            case CI_TOKEN_KIND_COMMA:
                if (depth == 0) {
                    has_comma = true;
                }

                break;
            default:
                break;
        }
    }

    if (!has_comma) {
        add__CIResolverMacroCallParams(params, param);

        return 1;
    }

    CIResolvedTokens *current = NEW(CIResolvedTokens);
    Usize added = 0;

    depth = 0;

    for (Usize i = 0; i < content_count; ++i) {
        CIToken *token = get__CIResolvedTokens(content, i);

        switch (token->kind) {
            case CI_TOKEN_KIND_LPAREN:
            case CI_TOKEN_KIND_LHOOK:
            case CI_TOKEN_KIND_LBRACE:
                ++depth;

                break;
            case CI_TOKEN_KIND_RPAREN:
            case CI_TOKEN_KIND_RHOOK:
            case CI_TOKEN_KIND_RBRACE:
                if (depth > 0) {
                    --depth;
                }

                break;
            case CI_TOKEN_KIND_COMMA:
                if (depth == 0) {
                    add__CIResolverMacroCallParams(
                      params, NEW(CIResolverMacroCallParam, current));

                    current = NEW(CIResolvedTokens);
                    ++added;

                    continue;
                }

                break;
            default:
                break;
        }

        add__CIResolvedTokens(current, ref__CIToken(token));
    }

    add__CIResolverMacroCallParams(params,
                                   NEW(CIResolverMacroCallParam, current));

    FREE(CIResolverMacroCallParam, param);

    return added + 1;
}

CIResolverMacroCall *
parse_macro_call_params__CIResolver(CIResolver *self,
                                    CIToken **current_token,
                                    Isize macro_param_variadic,
                                    Usize macro_params_length,
                                    const CIResultDefine *define);

static void
resolve_macro_call__CIResolver(CIResolver *self,
                               CIToken *identifier_token,
                               const CIResultDefine *define);

/// @brief Whether the expansion of the macro named `name` is the one currently
/// being resolved, or one this resolution is nested in.
/// @param name const String* (&)
static bool
is_being_expanded__CIResolver(const CIResolver *self, const String *name);

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

/// @brief Resolve `__VA_OPT__`, which stands for what is written between the
/// parentheses that follow it where something is written for the variadic part
/// of the macro it is written in, and for nothing where nothing is.
static void
resolve_macro_param_va_opt__CIResolver(CIResolver *self,
                                       CIToken *macro_param_va_opt_token);

/// @brief Resolve `_Pragma`, which says what a pragma written on a line of its
/// own says, written where an expression is written.
static void
resolve_pragma_operator__CIResolver(CIResolver *self,
                                    CIToken *pragma_operator_token);

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
resolve_preprocessor_embed_param__CIResolver(CIResolver *self, CITokens *param);

/// @brief Get the number of bytes the `limit` parameter of an `#embed` is
/// written to hold.
static Usize
resolve_preprocessor_embed_limit__CIResolver(CIResolver *self, CITokens *limit);

static bool
load_embed__CIResolver(CIResolver *self,
                       CIToken *preprocessor_embed_token,
                       const String *current_dir,
                       const String *embed_path);

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

static bool
is_included_once__CIResolver(const char *path)
{
    if (!included_once) {
        return false;
    }

    for (Usize i = 0; i < included_once->len; ++i) {
        if (!strcmp(CAST(String *, get__Vec(included_once, i))->buffer, path)) {
            return true;
        }
    }

    return false;
}

void
add_included_once__CIResolver(const char *path)
{
    if (!included_once) {
        included_once = NEW(Vec);
    }

    if (!is_included_once__CIResolver(path)) {
        push__Vec(included_once, from__String((char *)path));
    }
}

void
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

/// @brief Add the string a standard predefined macro stands for, as the string
/// literal it is replaced by.
/// @param value String* (the token takes it over)
static void
add_resolved_string_token__CIResolver(CIResolver *self,
                                      const CIToken *token,
                                      String *value);

/// @brief Replace `__DATE__` by the date it stands for.
static void
resolve_standard_predefined_macro_date__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_date_token);

/// @brief Replace `__FILE__` by the name of the file it is written in.
static void
resolve_standard_predefined_macro_file__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_file_token);

/// @brief Replace `__LINE__` by the line it is written on.
static void
resolve_standard_predefined_macro_line__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_line_token);

/// @brief Replace `__TIME__` by the time it stands for.
static void
resolve_standard_predefined_macro_time__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_time_token);

static CIToken *
perform_stringification__CIResolver(CIResolver *self,
                                    CIResolvedTokens *resolved_tokens);

static void
resolve_stringification__CIResolver(CIResolver *self);

/// @return String*
static String *
get_merged_id_lhs_content__CIResolver(CIResolver *self, CIToken *lhs);

/// @return String*
static String *
get_merged_id_rhs_content__CIResolver(CIResolver *self, CIToken *rhs);

/// @param merged_id_lhs_content String* (&)
/// @param merged_id_rhs_content String* (&)
static CIToken *
perform_merged_id__CIResolver(CIResolver *self,
                              CIToken *merged_id_lhs,
                              String *merged_id_lhs_content,
                              String *merged_id_rhs_content,
                              Location new_token_location);

static void
resolve_merged_id_left_or_right__CIResolver(CIResolver *self);

static inline void
resolve_merged_id_left__CIResolver(CIResolver *self);

static void
resolve_merged_id_right__CIResolver(CIResolver *self);

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
                                                                            \
        /* Whatever is resolved from here is still part of the expansions   \
           already in progress, so a macro among them stays unexpanded. */  \
        _resolver.expansion = self->expansion;                              \
                                                                            \
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
    self->has_variadic_param = false;
    self->ref_count = 0;

    return self;
}

VARIANT_CONSTRUCTOR(CIResolverMacroCall *, CIResolverMacroCall, is_empty)
{
    CIResolverMacroCall *self = lily_malloc(sizeof(CIResolverMacroCall));

    // self->params = <undefined>
    self->is_empty = true;
    self->has_variadic_param = false;
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
                FAILED__CIResolver(self,
                                   CI_ERROR_KIND_UNEXPECTED_CLOSING_DELIMITER);

                break;
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
#define EXPECT(ct, k)                                             \
    if (ct->kind != k) {                                          \
        FAILED__CIResolver(self, CI_ERROR_KIND_UNEXPECTED_TOKEN); \
                                                                  \
        return NULL;                                              \
    }
#define CURRENT(ct) ct

    NEXT((*current_token)); // skip `(`

    CIResolverMacroCall *macro_call = NEW(CIResolverMacroCall);
    Usize macro_param_count = 0;

    while (CURRENT((*current_token))->kind != CI_TOKEN_KIND_RPAREN &&
           CURRENT((*current_token))->kind != CI_TOKEN_KIND_EOF) {
        bool is_variadic = macro_param_variadic != -1 &&
                           macro_param_count >= macro_param_variadic;
        // Whether what is given is written as a parameter of the macro the
        // call is written in and nothing else, which is what is read again
        // where the body is.
        bool is_lone_macro_param =
          (CURRENT((*current_token))->kind == CI_TOKEN_KIND_MACRO_PARAM ||
           CURRENT((*current_token))->kind ==
             CI_TOKEN_KIND_MACRO_PARAM_VARIADIC) &&
          (PEEK((*current_token)) &&
           (PEEK((*current_token))->kind == CI_TOKEN_KIND_COMMA ||
            PEEK((*current_token))->kind == CI_TOKEN_KIND_RPAREN));
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

        // What is given is written as a parameter of the macro the call is
        // written in, or as anything else. The tokens a parameter stands for
        // are read where the body is read again, so a comma among them
        // separates what the call is given.
        if (is_lone_macro_param) {
            macro_param_count +=
              add_split_param__CIResolver(&macro_call->params, param);
        } else {
            add__CIResolverMacroCallParams(&macro_call->params, param);

            ++macro_param_count;
        }
    }

    EXPECT((*current_token), CI_TOKEN_KIND_RPAREN);

    // A macro written with a variadic parameter is called with nothing given
    // for it as often as with something, where `__VA_ARGS__` stands for
    // nothing, so what is written for it is not counted among the parameters a
    // call is required to give.
    bool is_variadic_macro = macro_param_variadic != -1;

    // As many are written for the variadic part as there are, and none at
    // all, so only what is written for the parameters named before it is
    // counted.
    if (is_variadic_macro ? macro_param_count < macro_params_length - 1
                          : macro_param_count != macro_params_length) {
        FAILED__CIResolver(self, CI_ERROR_KIND_MACRO_PARAMS_COUNT_MISMATCH);

        FREE(CIResolverMacroCall, macro_call);

        return NULL;
    }

    macro_call->has_variadic_param =
      is_variadic_macro && macro_param_count > (Usize)macro_param_variadic;

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
                if (!define->define->params) {
                    goto empty_macro_call;
                }

                NEXT(next_token); // skip maco identifier
                macro_call =
                  parse_macro_call_params__CIResolver(self,
                                                      &next_token,
                                                      macro_param_variadic,
                                                      macro_params_length,
                                                      define);

                // The call is reported on where it cannot be read, so there is
                // nothing left to expand it with.
                if (!macro_call) {
                    return;
                }

                break;
            default:
            empty_macro_call:
                if (macro_params_length != 0) {
                    FAILED__CIResolver(
                      self, CI_ERROR_KIND_MACRO_PARAMS_COUNT_MISMATCH);

                    return;
                }

                macro_call = NEW_VARIANT(CIResolverMacroCall, is_empty);
        }

        const CIResolverExpansion expansion = {
            .name = GET_PTR_RC(String, identifier_token->identifier),
            .parent = self->expansion
        };

        RESOLVE_TOKENS(
          &define->define->tokens,
          NULL,
          {
              set_macro_call__CIResolver(&_resolver, macro_call);
              _resolver.expansion = &expansion;
          },
          {
              merge__CIResolvedTokens(self->resolved_tokens,
                                      _resolver.resolved_tokens);
          });

        set_current_token__CIResolver(self, next_token);

        return;
    }

    FAILED__CIResolver(self, CI_ERROR_KIND_EXPECTED_AT_LEAST_EOF);

    return;

#undef NEXT
#undef PEEK
#undef EXPECT
#undef CURRENT
}

bool
is_being_expanded__CIResolver(const CIResolver *self, const String *name)
{
    for (const CIResolverExpansion *current = self->expansion; current;
         current = current->parent) {
        if (!strcmp(current->name->buffer, name->buffer)) {
            return true;
        }
    }

    return false;
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
    const String *identifier = GET_PTR_RC(String, identifier_token->identifier);

    // A macro met again while its own expansion is being resolved is left as a
    // plain identifier rather than expanded once more, as the standard
    // requires. Expanding it would not terminate.
    if ((define = get_define__CIResultFile(self->file, (String *)identifier)) &&
        !is_being_expanded__CIResolver(self, identifier)) {
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

    // `__VA_ARGS__` stands for what is written for the variadic part of the
    // macro, and for nothing where nothing is written for it. Left to what
    // follows, it would stand for what is written for the last parameter
    // named before it.
    if (!self->macro_call->has_variadic_param) {
        return;
    }

    CIResolverMacroCallParam *param =
      get_macro_param_variadic__CIResolverMacroCallParams(
        &self->macro_call->params);

    merge__CIResolvedTokens(self->resolved_tokens, param->resolved_content);
}

void
resolve_macro_param_va_opt__CIResolver(CIResolver *self,
                                       CIToken *macro_param_va_opt_token)
{
    ASSERT(self->macro_call);

    bool has_variadic = self->macro_call->has_variadic_param;
    Usize depth = 0;

    next_token__CIResolver(self); // skip `__VA_OPT__`

    if (self->current_token->kind != CI_TOKEN_KIND_LPAREN) {
        FAILED__CIResolver(self, CI_ERROR_KIND_EXPECTED_TOKEN);

        return;
    }

    next_token__CIResolver(self); // skip `(`

    while (self->current_token->kind != CI_TOKEN_KIND_EOF &&
           self->current_token->kind != CI_TOKEN_KIND_EOT) {
        if (self->current_token->kind == CI_TOKEN_KIND_LPAREN) {
            ++depth;
        } else if (self->current_token->kind == CI_TOKEN_KIND_RPAREN) {
            if (depth == 0) {
                break;
            }

            --depth;
        }

        // What it holds is read where something is written for the variadic
        // part, and passed over where nothing is.
        if (has_variadic) {
            resolve_token__CIResolver(self);
        }

        next_token__CIResolver(self);
    }
}

void
resolve_pragma_operator__CIResolver(CIResolver *self,
                                    CIToken *pragma_operator_token)
{
    next_token__CIResolver(self); // skip `_Pragma`

    if (self->current_token->kind != CI_TOKEN_KIND_LPAREN) {
        FAILED__CIResolver(self, CI_ERROR_KIND_EXPECTED_TOKEN);

        return;
    }

    next_token__CIResolver(self); // skip `(`

    if (self->current_token->kind != CI_TOKEN_KIND_LITERAL_CONSTANT_STRING) {
        FAILED__CIResolver(self, CI_ERROR_KIND_EXPECTED_STRING_LITERAL);

        return;
    }

    // What is given says what a pragma written on a line of its own says, so
    // it is read the same way.
    if (!strcmp(GET_PTR_RC(String, self->current_token->literal_constant_string)
                  ->buffer,
                "once")) {
        add_included_once__CIResolver(self->current_token->location.filename);
    }

    next_token__CIResolver(self); // skip what is given

    if (self->current_token->kind != CI_TOKEN_KIND_RPAREN) {
        FAILED__CIResolver(self, CI_ERROR_KIND_EXPECTED_TOKEN);
    }
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
            FAILED__CIResolver(self,
                               CI_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION);
        }

        const CIResolverExpr resolver_expr =
          NEW(CIResolverExpr, NULL, NULL, self->file, self->count_error, true);

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
        CIResolverExpr resolver_expr =
          NEW(CIResolverExpr, NULL, NULL, self->file, self->count_error, true);
        bool cond_res = is_true__CIResolverExpr(&resolver_expr, cond_expr);

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
resolve_preprocessor_embed_param__CIResolver(CIResolver *self, CITokens *param)
{
    // A parameter that is not written holds nothing, so there is nothing to
    // resolve from it.
    if (is_empty__CITokens(param)) {
        return;
    }

    RESOLVE_TOKENS(param, self->resolved_tokens, {}, {});
}

Usize
resolve_preprocessor_embed_limit__CIResolver(CIResolver *self, CITokens *limit)
{
    CIExpr *limit_expr = resolve_preprocessor_if_cond__CIResolver(self, limit);
    Usize res = 0;

    if (!limit_expr) {
        return res;
    }

    switch (limit_expr->kind) {
        case CI_EXPR_KIND_LITERAL:
            switch (limit_expr->literal.kind) {
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:
                    // The number of bytes to embed is a count, so what is
                    // written as a negative value is not one of them.
                    if (limit_expr->literal.signed_int < 0) {
                        goto expected_positive_integer_value;
                    }

                    res = (Usize)limit_expr->literal.signed_int;

                    break;
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
                    res = limit_expr->literal.unsigned_int;

                    break;
                default:
                    goto expected_positive_integer_value;
            }

            break;
        default:
        expected_positive_integer_value:
            FAILED__CIResolver(self,
                               CI_ERROR_KIND_EXPECTED_POSITIVE_INTEGER_VALUE);
    }

    FREE(CIExpr, limit_expr);

    return res;
}

bool
load_embed__CIResolver(CIResolver *self,
                       CIToken *preprocessor_embed_token,
                       const String *current_dir,
                       const String *embed_path)
{
    char *full_include_path = format("{S}/{S}", current_dir, embed_path);
    bool load_res = false;

    if (exists__File(full_include_path)) {
        CITokenPreprocessorEmbed *embed =
          &preprocessor_embed_token->preprocessor_embed;
        // What is embedded is the sequence of bytes the file holds, whose
        // length is measured rather than read from the content itself: a byte
        // set to zero is one of them as much as any other is.
        Usize content_len = get_size__File(full_include_path);
        char *content = read_file__File(full_include_path); // char*

        // The file is read no further than the number of bytes `limit` is
        // written to hold.
        if (!is_empty__CITokens(&embed->limit)) {
            const Usize limit =
              resolve_preprocessor_embed_limit__CIResolver(self, &embed->limit);

            if (limit < content_len) {
                content_len = limit;
            }
        }

        // What `if_empty` is written to hold is what an empty resource is
        // embedded as, so nothing else is when there is no byte to embed.
        if (!content_len) {
            resolve_preprocessor_embed_param__CIResolver(self,
                                                         &embed->if_empty);

            lily_free(content);
            lily_free(full_include_path);

            return true;
        }

        // What `prefix` and `suffix` are written to hold surrounds the bytes,
        // so they are only written when there are bytes to surround.
        resolve_preprocessor_embed_param__CIResolver(self, &embed->prefix);

        for (Usize i = 0; i < content_len; ++i) {
            add_resolved_token__CIResolver(
              self,
              NEW_VARIANT(
                CIToken,
                literal_constant_int,
                clone__Location(&preprocessor_embed_token->location),
                NEW(CITokenLiteralConstantInt,
                    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
                    // The value of a byte is the one it holds
                    // unsigned, as what is embedded is read as a
                    // sequence of them rather than of characters.
                    format__String("{zu}", (Usize)(Uint8)content[i]))));

            if (i + 1 < content_len) {
                add_resolved_token__CIResolver(
                  self,
                  NEW(CIToken,
                      CI_TOKEN_KIND_COMMA,
                      clone__Location(&preprocessor_embed_token->location)));
            }
        }

        resolve_preprocessor_embed_param__CIResolver(self, &embed->suffix);

        lily_free(content);

        load_res = true;
    }

    lily_free(full_include_path);

    return load_res;
}

void
resolve_preprocessor_embed__CIResolver(CIResolver *self,
                                       CIToken *preprocessor_embed_token)
{
    char *current_dir = get_cwd__Dir();

    if (!current_dir) {
        UNREACHABLE("cannot call get_cwd__Dir function");
    }

#define EMBED_DIRS_LEN 2
    String *embed_dirs[EMBED_DIRS_LEN] = {
        // Current directory of the file.
        get_dir__File(preprocessor_embed_token->location.filename),
        format__String("{sa}", current_dir)
    };

    for (Usize i = 0; i < EMBED_DIRS_LEN; ++i) {
        const String *current_dir = embed_dirs[i];

        if (load_embed__CIResolver(
              self,
              preprocessor_embed_token,
              current_dir,
              preprocessor_embed_token->preprocessor_embed.value)) {
            goto exit;
        }
    }

    FAILED__CIResolver(self, CI_ERROR_KIND_FAILED_TO_OPEN_EMBED_PATH);

    goto exit;

exit:
    FREE(String, embed_dirs[0]);
    FREE(String, embed_dirs[1]);

#undef EMBED_DIRS_LEN
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
        // A file written with `#pragma once` is included the first time it is
        // written and passed over afterwards.
        if (is_included_once__CIResolver(full_include_path)) {
            lily_free(full_include_path);

            return true;
        }

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

    FAILED__CIResolver(self, CI_ERROR_KIND_INCLUDE_FILE_IS_NOT_FOUND);

    return;
}

void
resolve_preprocessor_line__CIResolver(CIResolver *self,
                                      CIToken *preprocessor_line_token)
{
    // TODO: Renumber what follows the directive, and rename the file it is
    // reported as being written in, so that `__LINE__` and `__FILE__` give
    // back what the directive says. The directive is read and left alone until
    // then, rather than stopping the whole of what it is written in.
}

void
resolve_preprocessor_pragma__CIResolver(CIResolver *self,
                                        CIToken *preprocessor_pragma_token)
{
    const String *content = preprocessor_pragma_token->preprocessor_pragma;

    // `once` says the file it is written in is included the first time it is
    // written and passed over afterwards.
    if (!strcmp(content->buffer, "once")) {
        add_included_once__CIResolver(
          preprocessor_pragma_token->location.filename);

        return;
    }

    // A pragma nothing is known about is written to be passed over, so it is
    // left alone rather than reported on.
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
add_resolved_string_token__CIResolver(CIResolver *self,
                                      const CIToken *token,
                                      String *value)
{
    // NOTE: The token takes the reference over, so it is not released here.
    add_resolved_token__CIResolver(
      self,
      NEW_VARIANT(CIToken,
                  literal_constant_string,
                  clone__Location(&token->location),
                  NEW(Rc, value)));
}

void
resolve_standard_predefined_macro_date__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_date_token)
{
    add_resolved_string_token__CIResolver(
      self,
      standard_predefined_macro_date_token,
      clone__String(standard_predefined_macro_date_token
                      ->standard_predefined_macro___date__));
}

void
resolve_standard_predefined_macro_file__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_file_token)
{
    add_resolved_string_token__CIResolver(
      self,
      standard_predefined_macro_file_token,
      from__String(self->file->file_input.name));
}

void
resolve_standard_predefined_macro_line__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_line_token)
{
    add_resolved_token__CIResolver(
      self,
      NEW_VARIANT(
        CIToken,
        literal_constant_int,
        clone__Location(&standard_predefined_macro_line_token->location),
        NEW(CITokenLiteralConstantInt,
            CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
            format__String(
              "{zu}",
              standard_predefined_macro_line_token->location.start_line))));
}

void
resolve_standard_predefined_macro_time__CIResolver(
  CIResolver *self,
  CIToken *standard_predefined_macro_time_token)
{
    add_resolved_string_token__CIResolver(
      self,
      standard_predefined_macro_time_token,
      clone__String(standard_predefined_macro_time_token
                      ->standard_predefined_macro___time__));
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

    // NOTE: The scanner already emits this error, but as an error does not stop
    // the scan, a `#` not followed by a macro param can still reach the
    // resolver.
    if (!macro_param_token ||
        (macro_param_token->kind != CI_TOKEN_KIND_MACRO_PARAM &&
         macro_param_token->kind != CI_TOKEN_KIND_MACRO_PARAM_VARIADIC)) {
        FAILED__CIResolver(self,
                           CI_ERROR_KIND_EXPECTED_MACRO_PARAM_AFTER_STRINGIFY);
        next_token__CIResolver(self); // Skip `#`

        return;
    }

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
            return clone__String(GET_PTR_RC(String, lhs->identifier));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return clone__String(lhs->literal_constant_bin.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return clone__String(lhs->literal_constant_float.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return clone__String(lhs->literal_constant_int.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return clone__String(lhs->literal_constant_hex.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return clone__String(lhs->literal_constant_octal.value);
        default:
            if (is_keyword__CITokenKind(lhs->kind)) {
                return to_string__CIToken(lhs);
            }

            FAILED__CIResolver(self, CI_ERROR_KIND_UNEXPECTED_LHS_OF_MERGE);

            return NULL;
    }
}

String *
get_merged_id_rhs_content__CIResolver(CIResolver *self, CIToken *rhs)
{
    // TODO: Need to manage keyword.
    switch (rhs->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return clone__String(GET_PTR_RC(String, rhs->identifier));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return clone__String(rhs->literal_constant_int.value);
        default:
            if (is_keyword__CITokenKind(rhs->kind)) {
                return to_string__CIToken(rhs);
            }

            FAILED__CIResolver(self, CI_ERROR_KIND_UNEXPECTED_RHS_OF_MERGE);

            return NULL;
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
        identifier_case:
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
            if (is_keyword__CITokenKind(merged_id_lhs->kind)) {
                goto identifier_case;
            }

            UNREACHABLE("unknown variant");
    }

#undef NEW_INTEGER_TOKEN
}

void
resolve_merged_id_left_or_right__CIResolver(CIResolver *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_MACRO_PARAM:
            return resolve_macro_param__CIResolver(self, self->current_token);
        case CI_TOKEN_KIND_MACRO_PARAM_VARIADIC:
            return resolve_macro_param_variadic__CIResolver(
              self, self->current_token);
        default:
            return add_resolved_token__CIResolver(
              self, ref__CIToken(self->current_token));
    }
}

void
resolve_merged_id_left__CIResolver(CIResolver *self)
{
    return resolve_merged_id_left_or_right__CIResolver(self);
}

void
resolve_merged_id_right__CIResolver(CIResolver *self)
{
    if (self->current_token->next &&
        self->current_token->next->kind == CI_TOKEN_KIND_HASHTAG_HASHTAG) {
        return resolve_merged_id__CIResolver(self);
    }

    resolve_merged_id_left_or_right__CIResolver(self);
}

void
resolve_merged_id__CIResolver(CIResolver *self)
{
    resolve_merged_id_left__CIResolver(self);
    next_token__CIResolver(self);

    if (count__CIResolvedTokens(self->resolved_tokens) > 0) {
        Usize last_token_index =
          count__CIResolvedTokens(self->resolved_tokens) - 1;

        next_token__CIResolver(self);
        resolve_merged_id_right__CIResolver(self);

        CIToken *merged_id_lhs =
          get__CIResolvedTokens(self->resolved_tokens, last_token_index);

        if (last_token_index + 1 >=
            count__CIResolvedTokens(self->resolved_tokens)) {
            FAILED__CIResolver(self, CI_ERROR_KIND_EXPECTED_RHS_OF_MERGE);

            return;
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

        {
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
        FREE(String, lhs_content);
        FREE(String, rhs_content);

        return;
    }

    FAILED__CIResolver(self, CI_ERROR_KIND_MERGE_AT_START);

    return;
}

void
resolve_token__CIResolver(CIResolver *self)
{
    if (self->current_token->next &&
        self->current_token->next->kind == CI_TOKEN_KIND_HASHTAG_HASHTAG) {
        return resolve_merged_id__CIResolver(self);
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return resolve_identifier__CIResolver(self, self->current_token);
        case CI_TOKEN_KIND_MACRO_DEFINED:
            return resolve_macro_defined__CIResolver(self, self->current_token);
        case CI_TOKEN_KIND_MACRO_PARAM:
            return resolve_macro_param__CIResolver(self, self->current_token);
        case CI_TOKEN_KIND_MACRO_PARAM_VA_OPT:
            return resolve_macro_param_va_opt__CIResolver(self,
                                                          self->current_token);
        case CI_TOKEN_KIND_KEYWORD__PRAGMA:
            return resolve_pragma_operator__CIResolver(self,
                                                       self->current_token);
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
        // NOTE: The standard predefined macros are replaced here, so the parser
        // reads the literals they stand for, as it does for any other macro.
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__:
            return resolve_standard_predefined_macro_date__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__:
            return resolve_standard_predefined_macro_file__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__:
            return resolve_standard_predefined_macro_line__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__:
            return resolve_standard_predefined_macro_time__CIResolver(
              self, self->current_token);
        case CI_TOKEN_KIND_HASHTAG:
            return resolve_stringification__CIResolver(self);
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

    // NOTE: The error count is deliberately not checked here: `run__CIResolver`
    // is also used for nested resolves (a `#if` condition, a macro call, ...),
    // and stopping in one of those would hide the errors of the conditionals
    // that follow. The check belongs to the stage boundary, in
    // `run_file__CIResult`.
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
