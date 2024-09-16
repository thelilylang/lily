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
#include <base/macros.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#include <base/print.h>
#endif

#include <core/cc/ci/parser.h>
#include <core/cc/ci/token.h>

#include <stdio.h>
#include <stdlib.h>

// Free CITokenEot type (CI_TOKEN_EOT_CONTEXT_MACRO_CALL).
static inline VARIANT_DESTRUCTOR(CITokenEot,
                                 macro_call,
                                 const CITokenEot *self);

// Free CITokenEot type (CI_TOKEN_EOT_CONTEXT_MACRO_PARAM).
static inline VARIANT_DESTRUCTOR(CITokenEot,
                                 macro_param,
                                 const CITokenEot *self);

// Free CIToken type (CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED).
static VARIANT_DESTRUCTOR(CIToken, attribute_deprecated, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_ATTRIBUTE_NODISCARD).
static VARIANT_DESTRUCTOR(CIToken, attribute_nodiscard, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_COMMENT_DOC).
static VARIANT_DESTRUCTOR(CIToken, comment_doc, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_EOT).
static VARIANT_DESTRUCTOR(CIToken, eot, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_IDENTIFIER).
static VARIANT_DESTRUCTOR(CIToken, identifier, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_INT).
static VARIANT_DESTRUCTOR(CIToken, literal_constant_int, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT).
static VARIANT_DESTRUCTOR(CIToken, literal_constant_float, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL).
static VARIANT_DESTRUCTOR(CIToken, literal_constant_octal, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_HEX).
static VARIANT_DESTRUCTOR(CIToken, literal_constant_hex, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_BIN).
static VARIANT_DESTRUCTOR(CIToken, literal_constant_bin, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER).
static inline VARIANT_DESTRUCTOR(CIToken,
                                 literal_constant_character,
                                 CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_STRING).
static VARIANT_DESTRUCTOR(CIToken, literal_constant_string, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_MACRO_DEFINED).
static VARIANT_DESTRUCTOR(CIToken, macro_defined, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_MACRO_PARAM).
static VARIANT_DESTRUCTOR(CIToken, macro_param, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_MACRO_PARAM_VARIADIC).
static VARIANT_DESTRUCTOR(CIToken, macro_param_variadic, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_DEFINE).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_define, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELIF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_elif, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_elifdef, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_elifndef, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELSE).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_else, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_EMBED).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_embed, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_IF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_if, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_IFDEF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_ifdef, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_IFNDEF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_ifndef, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_elifdef, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ERROR).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_error, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_INCLUDE).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_include, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_LINE).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_line, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_UNDEF).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_undef, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_PREPROCESSOR_WARNING).
static VARIANT_DESTRUCTOR(CIToken, preprocessor_warning, CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__).
static VARIANT_DESTRUCTOR(CIToken,
                          standard_predefined_macro___date__,
                          CIToken *self);

// Free CIToken type (CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__).
static VARIANT_DESTRUCTOR(CIToken,
                          standard_predefined_macro___time__,
                          CIToken *self);

void
add__CITokens(CITokens *self, struct CIToken *token)
{
    if (self->last) {
        switch (self->last->kind) {
            case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
                self->last->preprocessor_elif.content.last->next = token;

                break;
            case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
                self->last->preprocessor_elifdef.content.last->next = token;

                break;
            case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
                self->last->preprocessor_elifndef.content.last->next = token;

                break;
            case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
                self->last->preprocessor_else.content.last->next = token;

                break;
            case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
                self->last->preprocessor_embed.content.last->next = token;

                break;
            case CI_TOKEN_KIND_PREPROCESSOR_IF:
                self->last->preprocessor_if.content.last->next = token;

                break;
            case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
                self->last->preprocessor_ifdef.content.last->next = token;

                break;
            case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
                self->last->preprocessor_ifndef.content.last->next = token;

                break;
            default:
                break;
        }

        self->last->next = token;
        self->last = self->last->next;
    } else {
        self->first = token;
        self->last = token;
    }
}

bool
insert_after_match__CITokens(CITokens *self,
                             struct CIToken *match,
                             struct CIToken *token)
{
    ASSERT(!token->next);

    CIToken *current = self->first;

    while (current && current != match) {
        current = current->next;
    }

    if (current) {
        CIToken *old_next = current->next;

        current->next = token;
        current->next->next = old_next;

        if (current == self->last) {
            self->last = current->next;
        }

        return true;
    }

    return false;
}

CIToken *
remove_when_match__CITokens(CITokens *self, struct CIToken *match)
{
    CIToken *prev = NULL;
    CIToken *current = self->first;

    while (current && current != match) {
        prev = current;
        current = current->next;
    }

    if (current) {
        if (prev) {
            prev->next = current->next;
        } else {
            self->first = current->next;
        }

        if (current == self->last) {
            self->last = prev;
        }

        return current;
    }

    return NULL;
}

#define X_TOKENS(tokens, res, f, is_debug)                         \
    {                                                              \
        push_str__String(res, " { ");                              \
                                                                   \
        CIToken *current_token = tokens.first;                     \
                                                                   \
        while (current_token) {                                    \
            void *s = f(current_token);                            \
                                                                   \
            if (is_debug) {                                        \
                PUSH_STR_AND_FREE(res, (char *)s);                 \
            } else {                                               \
                APPEND_AND_FREE(res, (String *)s);                 \
            }                                                      \
                                                                   \
            push_str__String(res, ",\n");                          \
                                                                   \
            if (current_token->kind == CI_TOKEN_KIND_EOF ||        \
                (current_token->kind == CI_TOKEN_KIND_EOT &&       \
                 is_eot_break__CITokenEot(&current_token->eot))) { \
                break;                                             \
            }                                                      \
                                                                   \
            current_token = current_token->next;                   \
        }                                                          \
                                                                   \
        push_str__String(res, " }");                               \
    }

#define DEBUG_TOKENS(tokens, res) \
    X_TOKENS(tokens, res, to_string__Debug__CIToken, true);
#define TO_STRING_TOKENS(tokens, res) \
    X_TOKENS(tokens, res, to_string__CIToken, false);

String *
to_string__CITokens(const CITokens *self)
{
    String *res = NEW(String);

    TO_STRING_TOKENS((*self), res);

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CITokens, const CITokens *self)
{
    String *res = NEW(String);

    DEBUG_TOKENS((*self), res);

    return res;
}
#endif

#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, CITokens, const CITokens *self)
{
    PRINTLN("{Sr}", CALL_DEBUG_IMPL(to_string, CITokens, self));
}
#endif

DESTRUCTOR(CITokens, const CITokens *self)
{
    if (!self->first || !self->last) {
        abort();
    }

    ASSERT(self->first);
    ASSERT(self->last);

    CIToken *current = self->first;

    while (current && (current->kind != CI_TOKEN_KIND_EOF &&
                       (current->kind != CI_TOKEN_KIND_EOT ||
                        !is_eot_break__CITokenEot(&current->eot)))) {
        CIToken *prev = current;

        current = current->next;

        FREE(CIToken, prev);
    }

    if (current) {
        FREE(CIToken, current);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CITokenEotContext, enum CITokenEotContext self)
{
    switch (self) {
        case CI_TOKEN_EOT_CONTEXT_INCLUDE:
            return "CI_TOKEN_EOT_CONTEXT_INCLUDE";
        case CI_TOKEN_EOT_CONTEXT_MACRO_PARAM:
            return "CI_TOKEN_EOT_CONTEXT_MACRO_PARAM";
        case CI_TOKEN_EOT_CONTEXT_MACRO_CALL:
            return "CI_TOKEN_EOT_CONTEXT_MACRO_CALL";
        case CI_TOKEN_EOT_CONTEXT_MERGED_ID:
            return "CI_TOKEN_EOT_CONTEXT_MERGED_ID";
        case CI_TOKEN_EOT_CONTEXT_STRINGIFICATION:
            return "CI_TOKEN_EOT_CONTEXT_STRINGIFICATION";
        case CI_TOKEN_EOT_CONTEXT_OTHER:
            return "CI_TOKEN_EOT_CONTEXT_OTHER";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CITokenEot, const CITokenEot *self)
{
    // NOTE: Other struct fields will always be NULL after the scanner.
    return format__String(
      "CITokenEot{{ ctx = {s}, ... }",
      CALL_DEBUG_IMPL(to_string, CITokenEotContext, self->ctx));
}
#endif

VARIANT_DESTRUCTOR(CITokenEot, macro_call, const CITokenEot *self)
{
    FREE(Stack, self->macro_call);
}

VARIANT_DESTRUCTOR(CITokenEot, macro_param, const CITokenEot *self)
{
    FREE(Stack, self->macro_param);
}

DESTRUCTOR(CITokenEot, const CITokenEot *self)
{
    switch (self->ctx) {
        case CI_TOKEN_EOT_CONTEXT_MACRO_CALL:
            FREE_VARIANT(CITokenEot, macro_call, self);

            break;
        case CI_TOKEN_EOT_CONTEXT_MACRO_PARAM:
            FREE_VARIANT(CITokenEot, macro_param, self);

            break;
        default:
            break;
    }
}

char *
to_string__CITokenLiteralConstantIntSuffix(
  enum CITokenLiteralConstantIntSuffix self)
{
    switch (self) {
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_L:
            return "L";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LL:
            return "LL";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LU:
            return "LU";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LLU:
            return "LLU";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_U:
            return "U";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE:
            return "";
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantIntSuffix,
               enum CITokenLiteralConstantIntSuffix self)
{
    switch (self) {
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_L:
            return "CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_L";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LL:
            return "CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LL";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LU:
            return "CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LU";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LLU:
            return "CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LLU";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_U:
            return "CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_U";
        case CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE:
            return "CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantInt,
               const CITokenLiteralConstantInt *self)
{
    return format__String(
      "CITokenLiteralConstantInt{{ suffix = {s}, value = {S} }",
      to_string__Debug__CITokenLiteralConstantIntSuffix(self->suffix),
      self->value);
}
#endif

char *
to_string__CITokenLiteralConstantFloatSuffix(
  enum CITokenLiteralConstantFloatSuffix self)
{
    switch (self) {
        case CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_F:
            return "F";
        case CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_L:
            return "L";
        case CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_NONE:
            return "";
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantFloatSuffix,
               enum CITokenLiteralConstantFloatSuffix self)
{
    switch (self) {
        case CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_F:
            return "CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_F";
        case CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_L:
            return "CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_L";
        case CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_NONE:
            return "CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_NONE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantFloat,
               const CITokenLiteralConstantFloat *self)
{
    return format__String(
      "CITokenLiteralConstantFloat{{ suffix = {s}, literal_constant_float = "
      "{S} }",
      to_string__Debug__CITokenLiteralConstantFloatSuffix(self->suffix),
      self->value);
}
#endif

VARIANT_CONSTRUCTOR(CITokenPreprocessorDefineParam *,
                    CITokenPreprocessorDefineParam,
                    variadic)
{
    CITokenPreprocessorDefineParam *self =
      lily_malloc(sizeof(CITokenPreprocessorDefineParam));

    self->name = NULL;
    self->is_variadic = true;

    return self;
}

VARIANT_CONSTRUCTOR(CITokenPreprocessorDefineParam *,
                    CITokenPreprocessorDefineParam,
                    normal,
                    String *name)
{
    CITokenPreprocessorDefineParam *self =
      lily_malloc(sizeof(CITokenPreprocessorDefineParam));

    self->name = name;
    self->is_variadic = false;

    return self;
}

String *
to_string__CITokenPreprocessorDefineParam(
  const CITokenPreprocessorDefineParam *self)
{
    if (self->is_variadic) {
        return from__String("...");
    } else {
        ASSERT(self->name);

        return clone__String(self->name);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorDefineParam,
               const CITokenPreprocessorDefineParam *self)
{
    return format__String(
      "CITokenPreprocessorDefineParam{{ name = {s}, is_variadic = {b} }",
      self->name ? self->name->buffer : "NULL",
      self->is_variadic);
}
#endif

DESTRUCTOR(CITokenPreprocessorDefineParam, CITokenPreprocessorDefineParam *self)
{
    if (self->name) {
        FREE(String, self->name);
    }

    lily_free(self);
}

String *
to_string__CITokenPreprocessorDefine(const CITokenPreprocessorDefine *self)
{
    String *res = format__String("#define {S}", self->name);

    if (self->params) {
        push_str__String(res, "(");

        for (Usize i = 0; i < self->params->len; ++i) {
            CITokenPreprocessorDefineParam *param = get__Vec(self->params, i);

            if (param->is_variadic) {
                push_str__String(res, "...");
            } else {
                ASSERT(param->name);

                append__String(res, param->name);
            }

            if (i + 1 != self->params->len) {
                push_str__String(res, ", ");
            }
        }

        push_str__String(res, ")\\\n");
    } else {
        push_str__String(res, " ");
    }

    {
        Usize current_line = 0;
        CIToken *current_token = self->tokens.first;

        ASSERT(current_token);

        while (current_token && current_token->kind != CI_TOKEN_KIND_EOT) {
            if (current_line == 0) {
                current_line = current_token->location.start_line;
            } else if (current_line != current_token->location.start_line) {
                current_line = current_token->location.start_line;

                push_str__String(res, "\\\n");
            }

            String *token_string = to_string__CIToken(current_token);

            APPEND_AND_FREE(res, token_string);

            current_token = current_token->next;
        }
    }

    return res;
}

Isize
get_variadic_param_index__CITokenPreprocessorDefine(
  const CITokenPreprocessorDefine *self)
{
    if (!self->is_variadic) {
        return -1;
    }

    for (Usize i = 0; i < self->params->len; ++i) {
        CITokenPreprocessorDefineParam *param = get__Vec(self->params, i);

        if (param->is_variadic) {
            return i;
        }
    }

    UNREACHABLE("a variadic param must be found");
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorDefine,
               const CITokenPreprocessorDefine *self)
{
    String *res = format__String(
      "CITokenPreprocessorDefine{{ name = {S}, params =", self->name);

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, CITokenPreprocessorDefineParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", tokens =");

    DEBUG_TOKENS(self->tokens, res);

    {
        char *s = format(", is_variadic = {b} }", self->is_variadic);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorDefine, const CITokenPreprocessorDefine *self)
{
    FREE(String, self->name);

    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer,
                          self->params->len,
                          CITokenPreprocessorDefineParam);
        FREE(Vec, self->params);
    }

    FREE(CITokens, &self->tokens);
}

String *
to_string__CITokenPreprocessorEmbed(const CITokenPreprocessorEmbed *self)
{
    return format__String("#embed \"{S}\"", self->value);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorEmbed,
               const CITokenPreprocessorEmbed *self)
{
    String *res = format__String(
      "CITokenPreprocessorEmbed{{ value = {S}, content =", self->value);

    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorEmbed, const CITokenPreprocessorEmbed *self)
{
    FREE(String, self->value);
    FREE(CITokens, &self->content);
}

String *
to_string__CITokenPreprocessorLine(const CITokenPreprocessorLine *self)
{
    if (self->filename) {
        return format__String("#line {zu}", self->line);
    }

    return format__String("#line {zu} \"{S}\"", self->line, self->filename);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorLine,
               const CITokenPreprocessorLine *self)
{
    if (self->filename) {
        return format__String(
          "CITokenPreprocessorLine{{ line = {zu}, filename = {S} }",
          self->line,
          self->filename);
    }

    return format__String(
      "CITokenPreprocessorLine{{ line = {zu}, filename = NULL }", self->line);
}
#endif

DESTRUCTOR(CITokenPreprocessorLine, const CITokenPreprocessorLine *self)
{
    if (self->filename) {
        FREE(String, self->filename);
    }
}

String *
to_string__CITokenPreprocessorIf(const CITokenPreprocessorIf *self)
{
#define TO_STRING_CITOKEN_PREPROCESSOR_IF(pks) \
    String *res = from__String(pks);           \
                                               \
    TO_STRING_TOKENS(self->cond, res);         \
                                               \
    push_str__String(res, "\n");               \
                                               \
    TO_STRING_TOKENS(self->content, res);

    TO_STRING_CITOKEN_PREPROCESSOR_IF("#if ");

    push_str__String(res, "#endif\n");

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorIf,
               const CITokenPreprocessorIf *self)
{
    String *res = format__String("CITokenPreprocessorIf{{ cond =");

    DEBUG_TOKENS(self->cond, res);
    push_str__String(res, ", content =");
    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorIf, const CITokenPreprocessorIf *self)
{
    FREE(CITokens, &self->cond);
    FREE(CITokens, &self->content);
}

String *
to_string__CITokenPreprocessorElif(const CITokenPreprocessorElif *self)
{
    TO_STRING_CITOKEN_PREPROCESSOR_IF("#elif ");

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElif,
               const CITokenPreprocessorElif *self)
{
    String *res = format__String("CITokenPreprocessorElif{{ cond =");

    DEBUG_TOKENS(self->cond, res);
    push_str__String(res, ", content =");
    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

String *
to_string__CITokenPreprocessorIfdef(const CITokenPreprocessorIfdef *self)
{
    String *res = format__String("#ifdef {S}\n", self->identifier);

    TO_STRING_TOKENS(self->content, res);

    push_str__String(res, "#endif\n");

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorIfdef,
               const CITokenPreprocessorIfdef *self)
{
    String *res =
      format__String("CITokenPreprocessorIfdef{{ identifier = {S}, content =",
                     self->identifier);

    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorIfdef, const CITokenPreprocessorIfdef *self)
{
    FREE(String, self->identifier);
    FREE(CITokens, &self->content);
}

String *
to_string__CITokenPreprocessorIfndef(const CITokenPreprocessorIfndef *self)
{
    String *res = format__String("#ifndef {S}\n", self->identifier);

    TO_STRING_TOKENS(self->content, res);

    push_str__String(res, "#endif\n");

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorIfndef,
               const CITokenPreprocessorIfndef *self)
{
    String *res =
      format__String("CITokenPreprocessorIfndef{{ identifier = {S}, content =",
                     self->identifier);

    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorIfndef, const CITokenPreprocessorIfndef *self)
{
    FREE(CITokenPreprocessorIfdef, self);
}

String *
to_string__CITokenPreprocessorElifdef(const CITokenPreprocessorElifdef *self)
{
    String *res = format__String("#elifdef {S}\n", self->identifier);

    TO_STRING_TOKENS(self->content, res);

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElifdef,
               const CITokenPreprocessorElifdef *self)
{
    String *res =
      format__String("CITokenPreprocessorElifdef{{ identifier = {S}, content =",
                     self->identifier);

    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorElifdef, const CITokenPreprocessorElifdef *self)
{
    FREE(CITokenPreprocessorIfdef, self);
}

String *
to_string__CITokenPreprocessorElifndef(const CITokenPreprocessorElifndef *self)
{
    String *res = format__String("#elifndef {S}\n", self->identifier);

    TO_STRING_TOKENS(self->content, res);

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElifndef,
               const CITokenPreprocessorElifndef *self)
{
    String *res = format__String(
      "CITokenPreprocessorElifndef{{ identifier = {S}, content =",
      self->identifier);

    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorElifndef, const CITokenPreprocessorElifndef *self)
{
    FREE(CITokenPreprocessorIfdef, self);
}

String *
to_string__CITokenPreprocessorElse(const CITokenPreprocessorElse *self)
{
    String *res = from__String("#else\n");

    TO_STRING_TOKENS(self->content, res);

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElse,
               const CITokenPreprocessorElse *self)
{
    String *res = from__String("CITokenPreprocessorElse{{ content =");

    DEBUG_TOKENS(self->content, res);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CITokenPreprocessorElse, const CITokenPreprocessorElse *self)
{
    FREE(CITokens, &self->content);
}

String *
to_string__CITokenPreprocessorInclude(const CITokenPreprocessorInclude *self)
{
    return format__String("#include \"{S}\"", self->value);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorInclude,
               const CITokenPreprocessorInclude *self)
{
    return format__String("CITokenPreprocessorInclude{{ value = {S} }",
                          self->value);
}
#endif

CONSTRUCTOR(CITokenMacroCallId *, CITokenMacroCallId, Usize id)
{
    CITokenMacroCallId *self = lily_malloc(sizeof(CITokenMacroCallId));

    self->id = id;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CITokenMacroParam, const CITokenMacroParam *self)
{
    return format__String("CITokenMacroParam{{ id = {zu} }", self->id);
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenMacroParamVariadic,
               const CITokenMacroParamVariadic *self)
{
    return format__String("CITokenMacroParamVariadic{{ }");
}
#endif

CONSTRUCTOR(CIToken *, CIToken, enum CITokenKind kind, Location location)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = kind;
    self->location = location;
    self->next = NULL;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    comment_doc,
                    Location location,
                    String *comment_doc)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_COMMENT_DOC;
    self->location = location;
    self->next = NULL;
    self->comment_doc = comment_doc;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *, CIToken, eot, Location location, CITokenEot eot)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_EOT;
    self->location = location;
    self->next = NULL;
    self->eot = eot;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    attribute_deprecated,
                    Location location,
                    String *attribute_deprecated)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED;
    self->location = location;
    self->next = NULL;
    self->attribute_deprecated = attribute_deprecated;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    attribute_nodiscard,
                    Location location,
                    String *attribute_nodiscard)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_ATTRIBUTE_NODISCARD;
    self->location = location;
    self->next = NULL;
    self->attribute_nodiscard = attribute_nodiscard;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    identifier,
                    Location location,
                    String *identifier)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_IDENTIFIER;
    self->location = location;
    self->next = NULL;
    self->identifier = identifier;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_int,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_int)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_INT;
    self->location = location;
    self->next = NULL;
    self->literal_constant_int = literal_constant_int;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_float,
                    Location location,
                    CITokenLiteralConstantFloat literal_constant_float)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT;
    self->location = location;
    self->next = NULL;
    self->literal_constant_float = literal_constant_float;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_octal,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_octal)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL;
    self->location = location;
    self->next = NULL;
    self->literal_constant_octal = literal_constant_octal;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_hex,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_hex)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_HEX;
    self->location = location;
    self->literal_constant_hex = literal_constant_hex;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_bin,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_bin)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_BIN;
    self->location = location;
    self->next = NULL;
    self->literal_constant_bin = literal_constant_bin;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_character,
                    Location location,
                    char literal_constant_character)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER;
    self->location = location;
    self->next = NULL;
    self->literal_constant_character = literal_constant_character;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_string,
                    Location location,
                    String *literal_constant_string)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_STRING;
    self->location = location;
    self->next = NULL;
    self->literal_constant_string = literal_constant_string;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    macro_param,
                    Location location,
                    CITokenMacroParam macro_param)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_MACRO_PARAM;
    self->location = location;
    self->next = NULL;
    self->macro_param = macro_param;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    macro_param_variadic,
                    Location location,
                    CITokenMacroParamVariadic macro_param_variadic)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_MACRO_PARAM_VARIADIC;
    self->location = location;
    self->next = NULL;
    self->macro_param_variadic = macro_param_variadic;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    macro_defined,
                    Location location,
                    String *macro_defined)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_MACRO_DEFINED;
    self->location = location;
    self->next = NULL;
    self->macro_defined = macro_defined;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_define,
                    Location location,
                    CITokenPreprocessorDefine preprocessor_define)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_DEFINE;
    self->location = location;
    self->next = NULL;
    self->preprocessor_define = preprocessor_define;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_elif,
                    Location location,
                    CITokenPreprocessorElif preprocessor_elif)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_ELIF;
    self->location = location;
    self->next = NULL;
    self->preprocessor_elif = preprocessor_elif;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_elifdef,
                    Location location,
                    CITokenPreprocessorElifdef preprocessor_elifdef)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF;
    self->location = location;
    self->next = NULL;
    self->preprocessor_elifdef = preprocessor_elifdef;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_elifndef,
                    Location location,
                    CITokenPreprocessorElifndef preprocessor_elifndef)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF;
    self->location = location;
    self->next = NULL;
    self->preprocessor_elifndef = preprocessor_elifndef;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_else,
                    Location location,
                    CITokenPreprocessorElse preprocessor_else)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_ELSE;
    self->location = location;
    self->next = NULL;
    self->preprocessor_else = preprocessor_else;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_embed,
                    Location location,
                    CITokenPreprocessorEmbed preprocessor_embed)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_EMBED;
    self->location = location;
    self->next = NULL;
    self->preprocessor_embed = preprocessor_embed;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_error,
                    Location location,
                    String *preprocessor_error)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_ERROR;
    self->location = location;
    self->next = NULL;
    self->preprocessor_error = preprocessor_error;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_if,
                    Location location,
                    CITokenPreprocessorIf preprocessor_if)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_IF;
    self->location = location;
    self->next = NULL;
    self->preprocessor_if = preprocessor_if;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_ifdef,
                    Location location,
                    CITokenPreprocessorIfdef preprocessor_ifdef)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_IFDEF;
    self->location = location;
    self->next = NULL;
    self->preprocessor_ifdef = preprocessor_ifdef;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_ifndef,
                    Location location,
                    CITokenPreprocessorIfdef preprocessor_ifndef)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_IFDEF;
    self->location = location;
    self->next = NULL;
    self->preprocessor_ifndef = preprocessor_ifndef;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_include,
                    Location location,
                    CITokenPreprocessorInclude preprocessor_include)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_INCLUDE;
    self->location = location;
    self->next = NULL;
    self->preprocessor_include = preprocessor_include;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_line,
                    Location location,
                    CITokenPreprocessorLine preprocessor_line)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_LINE;
    self->location = location;
    self->next = NULL;
    self->preprocessor_line = preprocessor_line;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_undef,
                    Location location,
                    String *preprocessor_undef)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_UNDEF;
    self->location = location;
    self->next = NULL;
    self->preprocessor_undef = preprocessor_undef;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_warning,
                    Location location,
                    String *preprocessor_warning)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_PREPROCESSOR_WARNING;
    self->location = location;
    self->next = NULL;
    self->preprocessor_warning = preprocessor_warning;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    builtin_macro_has_feature,
                    Location location,
                    enum CIExtensionsHasFeature has_feature)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE;
    self->location = location;
    self->next = NULL;
    self->has_feature = has_feature;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    standard_predefined_macro___date__,
                    Location location,
                    String *standard_predefined_macro___date__)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__;
    self->location = location;
    self->next = NULL;
    self->standard_predefined_macro___date__ =
      standard_predefined_macro___date__;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    standard_predefined_macro___time__,
                    Location location,
                    String *standard_predefined_macro___time__)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__;
    self->location = location;
    self->next = NULL;
    self->standard_predefined_macro___time__ =
      standard_predefined_macro___time__;

    return self;
}

String *
to_string__CIToken(CIToken *self)
{
    switch (self->kind) {
        case CI_TOKEN_KIND_AMPERSAND:
            return from__String("&");
        case CI_TOKEN_KIND_AMPERSAND_AMPERSAND:
            return from__String("&&");
        case CI_TOKEN_KIND_AMPERSAND_EQ:
            return from__String("&=");
        case CI_TOKEN_KIND_ARROW:
            return from__String("->");
        case CI_TOKEN_KIND_AT:
            return from__String("@");
        case CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED:
            if (self->attribute_deprecated) {
                return format__String("[[deprecated(\"{S}\")]]",
                                      self->attribute_deprecated);
            }

            return from__String("[[deprecated]]");
        case CI_TOKEN_KIND_ATTRIBUTE_FALLTHROUGH:
            return from__String("[[fallthrough]]");
        case CI_TOKEN_KIND_ATTRIBUTE_MAYBE_UNUSED:
            return from__String("[[maybe_unused]]");
        case CI_TOKEN_KIND_ATTRIBUTE_NODISCARD:
            if (self->attribute_nodiscard) {
                return format__String("[[nodiscard(\"{S}\")]]",
                                      self->attribute_nodiscard);
            }

            return from__String("[[nodiscard]]");
        case CI_TOKEN_KIND_ATTRIBUTE_NORETURN:
            return from__String("[[noreturn]]");
        case CI_TOKEN_KIND_ATTRIBUTE__NORETURN:
            return from__String("[[_Noreturn]]");
        case CI_TOKEN_KIND_ATTRIBUTE_UNSEQUENCED:
            return from__String("[[unsequenced]]");
        case CI_TOKEN_KIND_ATTRIBUTE_REPRODUCIBLE:
            return from__String("[[reproducible]]");
        case CI_TOKEN_KIND_BANG:
            return from__String("!");
        case CI_TOKEN_KIND_BANG_EQ:
            return from__String("!=");
        case CI_TOKEN_KIND_BAR:
            return from__String("|");
        case CI_TOKEN_KIND_BAR_BAR:
            return from__String("||");
        case CI_TOKEN_KIND_BAR_EQ:
            return from__String("|=");
        case CI_TOKEN_KIND_COLON:
            return from__String(":");
        case CI_TOKEN_KIND_COMMA:
            return from__String(",");
        case CI_TOKEN_KIND_COMMENT_LINE:
            return from__String("COMMENT(LINE)");
        case CI_TOKEN_KIND_COMMENT_BLOCK:
            return from__String("COMMENT(BLOCK)");
        case CI_TOKEN_KIND_COMMENT_DOC:
            return format__String("COMMENT(DOC({S}))", self->comment_doc);
        case CI_TOKEN_KIND_DOT:
            return from__String(".");
        case CI_TOKEN_KIND_DOT_DOT_DOT:
            return from__String("...");
        case CI_TOKEN_KIND_EOF:
            return from__String("EOF");
        case CI_TOKEN_KIND_EOT:
            return from__String("EOT");
        case CI_TOKEN_KIND_EQ:
            return from__String("=");
        case CI_TOKEN_KIND_EQ_EQ:
            return from__String("==");
        case CI_TOKEN_KIND_HASHTAG:
            return from__String("#");
        case CI_TOKEN_KIND_HASHTAG_HASHTAG:
            return from__String("##");
        case CI_TOKEN_KIND_HAT:
            return from__String("^");
        case CI_TOKEN_KIND_HAT_EQ:
            return from__String("^=");
        case CI_TOKEN_KIND_IDENTIFIER:
            return format__String("IDENTIFIER({S})", self->identifier);
        case CI_TOKEN_KIND_INTERROGATION:
            return from__String("?");
        case CI_TOKEN_KIND_KEYWORD_ALIGNOF:
            return from__String("alignof");
        case CI_TOKEN_KIND_KEYWORD_ALIGNAS:
            return from__String("alignas");
        case CI_TOKEN_KIND_KEYWORD_ASM:
            return from__String("asm");
        case CI_TOKEN_KIND_KEYWORD_AUTO:
            return from__String("auto");
        case CI_TOKEN_KIND_KEYWORD_BOOL:
            return from__String("bool");
        case CI_TOKEN_KIND_KEYWORD_BREAK:
            return from__String("break");
        case CI_TOKEN_KIND_KEYWORD_CASE:
            return from__String("case");
        case CI_TOKEN_KIND_KEYWORD_CHAR:
            return from__String("char");
        case CI_TOKEN_KIND_KEYWORD_CONST:
            return from__String("const");
        case CI_TOKEN_KIND_KEYWORD_CONSTEXPR:
            return from__String("constexpr");
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
            return from__String("continue");
        case CI_TOKEN_KIND_KEYWORD_DEFAULT:
            return from__String("default");
        case CI_TOKEN_KIND_KEYWORD_DO:
            return from__String("do");
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
            return from__String("double");
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX:
            return from__String("double _Complex");
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY:
            return from__String("double _Imaginary");
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            return from__String("else");
        case CI_TOKEN_KIND_KEYWORD_ELSE_IF:
            return from__String("else if");
        case CI_TOKEN_KIND_KEYWORD_ENUM:
            return from__String("enum");
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
            return from__String("extern");
        case CI_TOKEN_KIND_KEYWORD_FALSE:
            return from__String("false");
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            return from__String("float");
        case CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX:
            return from__String("float _Complex");
        case CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY:
            return from__String("float _Imaginary");
        case CI_TOKEN_KIND_KEYWORD_FOR:
            return from__String("for");
        case CI_TOKEN_KIND_KEYWORD_GOTO:
            return from__String("goto");
        case CI_TOKEN_KIND_KEYWORD_IF:
            return from__String("if");
        case CI_TOKEN_KIND_KEYWORD_INLINE:
            return from__String("inline");
        case CI_TOKEN_KIND_KEYWORD_INT:
            return from__String("int");
        case CI_TOKEN_KIND_KEYWORD_LONG:
            return from__String("long");
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE:
            return from__String("long double");
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX:
            return from__String("long double _Complex");
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY:
            return from__String("long double _Imaginary");
        case CI_TOKEN_KIND_KEYWORD_LONG_INT:
            return from__String("long int");
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG:
            return from__String("long long");
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT:
            return from__String("long long int");
        case CI_TOKEN_KIND_KEYWORD_NULLPTR:
            return from__String("nullptr");
        case CI_TOKEN_KIND_KEYWORD_REGISTER:
            return from__String("register");
        case CI_TOKEN_KIND_KEYWORD_RESTRICT:
            return from__String("restrict");
        case CI_TOKEN_KIND_KEYWORD_RETURN:
            return from__String("return");
        case CI_TOKEN_KIND_KEYWORD_SHORT:
            return from__String("short");
        case CI_TOKEN_KIND_KEYWORD_SHORT_INT:
            return from__String("short int");
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
            return from__String("signed");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR:
            return from__String("signed char");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_INT:
            return from__String("signed int");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG:
            return from__String("signed long");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT:
            return from__String("signed long int");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG:
            return from__String("signed long long");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT:
            return from__String("signed long long int");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT:
            return from__String("signed short");
        case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT:
            return from__String("signed short int");
        case CI_TOKEN_KIND_KEYWORD_SIZEOF:
            return from__String("sizeof");
        case CI_TOKEN_KIND_KEYWORD_STATIC:
            return from__String("static");
        case CI_TOKEN_KIND_KEYWORD_STATIC_ASSERT:
            return from__String("static_assert");
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
            return from__String("struct");
        case CI_TOKEN_KIND_KEYWORD_SWITCH:
            return from__String("switch");
        case CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL:
            return from__String("thread_local");
        case CI_TOKEN_KIND_KEYWORD_TRUE:
            return from__String("true");
        case CI_TOKEN_KIND_KEYWORD_TYPEDEF:
            return from__String("typedef");
        case CI_TOKEN_KIND_KEYWORD_TYPEOF:
            return from__String("typeof");
        case CI_TOKEN_KIND_KEYWORD_TYPEOF_UNQUAL:
            return from__String("typeof_unqual");
        case CI_TOKEN_KIND_KEYWORD_UNION:
            return from__String("union");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
            return from__String("unsigned");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR:
            return from__String("unsigned char");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT:
            return from__String("unsigned int");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG:
            return from__String("unsigned long");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT:
            return from__String("unsigned long int");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG:
            return from__String("unsigned long long");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT:
            return from__String("unsigned long long int");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT:
            return from__String("unsigned short");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT:
            return from__String("unsigned short int");
        case CI_TOKEN_KIND_KEYWORD_VOID:
            return from__String("void");
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
            return from__String("volatile");
        case CI_TOKEN_KIND_KEYWORD_WHILE:
            return from__String("while");
        case CI_TOKEN_KIND_KEYWORD__ALIGNAS:
            return from__String("_Alignas");
        case CI_TOKEN_KIND_KEYWORD__ALIGNOF:
            return from__String("_Alignof");
        case CI_TOKEN_KIND_KEYWORD__ATOMIC:
            return from__String("_Atomic");
        case CI_TOKEN_KIND_KEYWORD__BITINT:
            return from__String("_BitInt");
        case CI_TOKEN_KIND_KEYWORD__BOOL:
            return from__String("_Bool");
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
            return from__String("_Complex");
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
            return from__String("_Decimal128");
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
            return from__String("_Decimal32");
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
            return from__String("_Decimal64");
        case CI_TOKEN_KIND_KEYWORD__GENERIC:
            return from__String("_Generic");
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return from__String("_Imaginary");
        case CI_TOKEN_KIND_KEYWORD__NORETURN:
            return from__String("_Noreturn");
        case CI_TOKEN_KIND_KEYWORD__STATIC_ASSERT:
            return from__String("_Static_assert");
        case CI_TOKEN_KIND_KEYWORD__THREAD_LOCAL:
            return from__String("_Thread_local");
        case CI_TOKEN_KIND_LBRACE:
            return from__String("{");
        case CI_TOKEN_KIND_LHOOK:
            return from__String("[");
        case CI_TOKEN_KIND_LPAREN:
            return from__String("(");
        case CI_TOKEN_KIND_LSHIFT:
            return from__String("<");
        case CI_TOKEN_KIND_LSHIFT_EQ:
            return from__String("<=");
        case CI_TOKEN_KIND_LSHIFT_LSHIFT:
            return from__String("<<");
        case CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ:
            return from__String("<<=");
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return format__String("LITERAL_CONSTANT(INT({S}))",
                                  self->literal_constant_int.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return format__String("LITERAL_CONSTANT(FLOAT({S}))",
                                  self->literal_constant_float.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return format__String("LITERAL_CONSTANT(OCTAL({S}))",
                                  self->literal_constant_octal.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return format__String("LITERAL_CONSTANT(HEX({S}))",
                                  self->literal_constant_hex.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return format__String("LITERAL_CONSTANT(BIN({S}))",
                                  self->literal_constant_bin.value);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return format__String("LITERAL_CONSTANT(CHARACTER({c}))",
                                  self->literal_constant_character);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return format__String("LITERAL_CONSTANT(STRING({S}))",
                                  self->literal_constant_string);
        case CI_TOKEN_KIND_MACRO_DEFINED:
            return format__String("MACRO_DEFINED({S})", self->macro_defined);
        case CI_TOKEN_KIND_MACRO_PARAM:
            return format__String("MACRO_PARAM({zu})", self->macro_param.id);
        case CI_TOKEN_KIND_MACRO_PARAM_VARIADIC:
            return format__String("__VA_ARGS__");
        case CI_TOKEN_KIND_MINUS:
            return from__String("-");
        case CI_TOKEN_KIND_MINUS_EQ:
            return from__String("-=");
        case CI_TOKEN_KIND_MINUS_MINUS:
            return from__String("--");
        case CI_TOKEN_KIND_PERCENTAGE:
            return from__String("%");
        case CI_TOKEN_KIND_PERCENTAGE_EQ:
            return from__String("%=");
        case CI_TOKEN_KIND_PLUS:
            return from__String("+");
        case CI_TOKEN_KIND_PLUS_EQ:
            return from__String("+=");
        case CI_TOKEN_KIND_PLUS_PLUS:
            return from__String("++");
        case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
            return format__String(
              "{Sr}",
              to_string__CITokenPreprocessorDefine(&self->preprocessor_define));
        case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
            return to_string__CITokenPreprocessorElif(&self->preprocessor_elif);
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
            return to_string__CITokenPreprocessorElifdef(
              &self->preprocessor_elifdef);
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
            return to_string__CITokenPreprocessorElifndef(
              &self->preprocessor_elifndef);
        case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
            return to_string__CITokenPreprocessorElse(&self->preprocessor_else);
        case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
            return format__String(
              "{Sr}",
              to_string__CITokenPreprocessorEmbed(&self->preprocessor_embed));
        case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
            return format__String("#error {S}", self->preprocessor_error);
        case CI_TOKEN_KIND_PREPROCESSOR_IF:
            return to_string__CITokenPreprocessorIf(&self->preprocessor_if);
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
            return to_string__CITokenPreprocessorIfdef(
              &self->preprocessor_ifdef);
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
            return to_string__CITokenPreprocessorIfndef(
              &self->preprocessor_ifndef);
        case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
            return to_string__CITokenPreprocessorInclude(
              &self->preprocessor_include);
        case CI_TOKEN_KIND_PREPROCESSOR_LINE:
            return to_string__CITokenPreprocessorLine(&self->preprocessor_line);
        case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
            return format__String("#undef {S}", self->preprocessor_undef);
        case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
            return format__String("#warning {S}", self->preprocessor_warning);
        case CI_TOKEN_KIND_RBRACE:
            return from__String("}");
        case CI_TOKEN_KIND_RHOOK:
            return from__String("]");
        case CI_TOKEN_KIND_RPAREN:
            return from__String(")");
        case CI_TOKEN_KIND_RSHIFT:
            return from__String(">");
        case CI_TOKEN_KIND_RSHIFT_EQ:
            return from__String(">=");
        case CI_TOKEN_KIND_RSHIFT_RSHIFT:
            return from__String(">>");
        case CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ:
            return from__String(">>=");
        case CI_TOKEN_KIND_SEMICOLON:
            return from__String(";");
        case CI_TOKEN_KIND_SLASH:
            return from__String("/");
        case CI_TOKEN_KIND_SLASH_EQ:
            return from__String("/=");
        case CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE:
            return format__String("__has_feature({d})", self->has_feature);
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__:
            return format__String("__DATE__ {S}",
                                  self->standard_predefined_macro___date__);
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__:
            return from__String("__FILE__");
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__:
            return from__String("__LINE__");
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC__:
            return from__String("__STDC__");
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_VERSION__:
            return from__String("__STDC_VERSION__");
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_HOSTED__:
            return from__String("__STDC_HOSTED__");
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__:
            return format__String("__TIME__ {S}",
                                  self->standard_predefined_macro___time__);
        case CI_TOKEN_KIND_STAR:
            return from__String("*");
        case CI_TOKEN_KIND_STAR_EQ:
            return from__String("*=");
        case CI_TOKEN_KIND_WAVE:
            return from__String("~");
        case CI_TOKEN_KIND_WAVE_EQ:
            return from__String("~=");
        case CI_TOKEN_KIND_BUILTIN_MACRO:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO:
        case CI_TOKEN_KIND_MAX:
            UNREACHABLE("this token is not used");
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
is_conditional_preprocessor__CITokenKind(enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
        case CI_TOKEN_KIND_PREPROCESSOR_IF:
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
            return true;
        default:
            return false;
    }
}

bool
is_preprocessor__CITokenKind(enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
        case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
        case CI_TOKEN_KIND_PREPROCESSOR_ENDIF:
        case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
        case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
        case CI_TOKEN_KIND_PREPROCESSOR_LINE:
        case CI_TOKEN_KIND_PREPROCESSOR_PRAGMA:
        case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
        case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
            return true;
        default:
            return is_conditional_preprocessor__CITokenKind(kind);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CITokenKind, enum CITokenKind self)
{
    switch (self) {
        case CI_TOKEN_KIND_AMPERSAND:
            return "CI_TOKEN_KIND_AMPERSAND";
        case CI_TOKEN_KIND_AMPERSAND_AMPERSAND:
            return "CI_TOKEN_KIND_AMPERSAND_AMPERSAND";
        case CI_TOKEN_KIND_AMPERSAND_EQ:
            return "CI_TOKEN_KIND_AMPERSAND_EQ";
        case CI_TOKEN_KIND_ARROW:
            return "CI_TOKEN_KIND_ARROW";
        case CI_TOKEN_KIND_AT:
            return "CI_TOKEN_KIND_AT";
        case CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED:
            return "CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED";
        case CI_TOKEN_KIND_ATTRIBUTE_FALLTHROUGH:
            return "CI_TOKEN_KIND_ATTRIBUTE_FALLTHROUGH";
        case CI_TOKEN_KIND_ATTRIBUTE_MAYBE_UNUSED:
            return "CI_TOKEN_KIND_ATTRIBUTE_MAYBE_UNUSED";
        case CI_TOKEN_KIND_ATTRIBUTE_NODISCARD:
            return "CI_TOKEN_KIND_ATTRIBUTE_NODISCARD";
        case CI_TOKEN_KIND_ATTRIBUTE_NORETURN:
            return "CI_TOKEN_KIND_ATTRIBUTE_NORETURN";
        case CI_TOKEN_KIND_ATTRIBUTE__NORETURN:
            return "CI_TOKEN_KIND_ATTRIBUTE__NORETURN";
        case CI_TOKEN_KIND_ATTRIBUTE_UNSEQUENCED:
            return "CI_TOKEN_KIND_ATTRIBUTE_UNSEQUENCED";
        case CI_TOKEN_KIND_ATTRIBUTE_REPRODUCIBLE:
            return "CI_TOKEN_KIND_ATTRIBUTE_REPRODUCIBLE";
        case CI_TOKEN_KIND_BANG:
            return "CI_TOKEN_KIND_BANG";
        case CI_TOKEN_KIND_BANG_EQ:
            return "CI_TOKEN_KIND_BANG_EQ";
        case CI_TOKEN_KIND_BAR:
            return "CI_TOKEN_KIND_BAR";
        case CI_TOKEN_KIND_BAR_BAR:
            return "CI_TOKEN_KIND_BAR_BAR";
        case CI_TOKEN_KIND_BAR_EQ:
            return "CI_TOKEN_KIND_BAR_EQ";
        case CI_TOKEN_KIND_COLON:
            return "CI_TOKEN_KIND_COLON";
        case CI_TOKEN_KIND_COMMA:
            return "CI_TOKEN_KIND_COMMA";
        case CI_TOKEN_KIND_COMMENT_LINE:
            return "CI_TOKEN_KIND_COMMENT_LINE";
        case CI_TOKEN_KIND_COMMENT_BLOCK:
            return "CI_TOKEN_KIND_COMMENT_BLOCK";
        case CI_TOKEN_KIND_COMMENT_DOC:
            return "CI_TOKEN_KIND_COMMENT_DOC";
        case CI_TOKEN_KIND_DOT:
            return "CI_TOKEN_KIND_DOT";
        case CI_TOKEN_KIND_DOT_DOT_DOT:
            return "CI_TOKEN_KIND_DOT_DOT_DOT";
        case CI_TOKEN_KIND_EOF:
            return "CI_TOKEN_KIND_EOF";
        case CI_TOKEN_KIND_EOT:
            return "CI_TOKEN_KIND_EOT";
        case CI_TOKEN_KIND_EQ:
            return "CI_TOKEN_KIND_EQ";
        case CI_TOKEN_KIND_EQ_EQ:
            return "CI_TOKEN_KIND_EQ_EQ";
        case CI_TOKEN_KIND_HASHTAG:
            return "CI_TOKEN_KIND_HASHTAG";
        case CI_TOKEN_KIND_HASHTAG_HASHTAG:
            return "CI_TOKEN_KIND_HASHTAG_HASHTAG";
        case CI_TOKEN_KIND_HAT:
            return "CI_TOKEN_KIND_HAT";
        case CI_TOKEN_KIND_HAT_EQ:
            return "CI_TOKEN_KIND_HAT_EQ";
        case CI_TOKEN_KIND_IDENTIFIER:
            return "CI_TOKEN_KIND_IDENTIFIER";
        case CI_TOKEN_KIND_INTERROGATION:
            return "CI_TOKEN_KIND_INTERROGATION";
        case CI_TOKEN_KIND_KEYWORD_ALIGNAS:
            return "CI_TOKEN_KIND_KEYWORD_ALIGNAS";
        case CI_TOKEN_KIND_KEYWORD_ALIGNOF:
            return "CI_TOKEN_KIND_KEYWORD_ALIGNOF";
        case CI_TOKEN_KIND_KEYWORD_ASM:
            return "CI_TOKEN_KIND_KEYWORD_ASM";
        case CI_TOKEN_KIND_KEYWORD_AUTO:
            return "CI_TOKEN_KIND_KEYWORD_AUTO";
        case CI_TOKEN_KIND_KEYWORD_BOOL:
            return "CI_TOKEN_KIND_KEYWORD_BOOL";
        case CI_TOKEN_KIND_KEYWORD_BREAK:
            return "CI_TOKEN_KIND_KEYWORD_BREAK";
        case CI_TOKEN_KIND_KEYWORD_CASE:
            return "CI_TOKEN_KIND_KEYWORD_CASE";
        case CI_TOKEN_KIND_KEYWORD_CHAR:
            return "CI_TOKEN_KIND_KEYWORD_CHAR";
        case CI_TOKEN_KIND_KEYWORD_CONST:
            return "CI_TOKEN_KIND_KEYWORD_CONST";
        case CI_TOKEN_KIND_KEYWORD_CONSTEXPR:
            return "CI_TOKEN_KIND_KEYWORD_CONSTEXPR";
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
            return "CI_TOKEN_KIND_KEYWORD_CONTINUE";
        case CI_TOKEN_KIND_KEYWORD_DEFAULT:
            return "CI_TOKEN_KIND_KEYWORD_DEFAULT";
        case CI_TOKEN_KIND_KEYWORD_DO:
            return "CI_TOKEN_KIND_KEYWORD_DO";
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
            return "CI_TOKEN_KIND_KEYWORD_DOUBLE";
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX:
            return "CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX";
        case CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY:
            return "CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY";
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            return "CI_TOKEN_KIND_KEYWORD_ELSE";
        case CI_TOKEN_KIND_KEYWORD_ELSE_IF:
            return "CI_TOKEN_KIND_KEYWORD_ELSE_IF";
        case CI_TOKEN_KIND_KEYWORD_ENUM:
            return "CI_TOKEN_KIND_KEYWORD_ENUM";
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
            return "CI_TOKEN_KIND_KEYWORD_EXTERN";
        case CI_TOKEN_KIND_KEYWORD_FALSE:
            return "CI_TOKEN_KIND_KEYWORD_FALSE";
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            return "CI_TOKEN_KIND_KEYWORD_FLOAT";
        case CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX:
            return "CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX";
        case CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY:
            return "CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY";
        case CI_TOKEN_KIND_KEYWORD_FOR:
            return "CI_TOKEN_KIND_KEYWORD_FOR";
        case CI_TOKEN_KIND_KEYWORD_GOTO:
            return "CI_TOKEN_KIND_KEYWORD_GOTO";
        case CI_TOKEN_KIND_KEYWORD_IF:
            return "CI_TOKEN_KIND_KEYWORD_IF";
        case CI_TOKEN_KIND_KEYWORD_INLINE:
            return "CI_TOKEN_KIND_KEYWORD_INLINE";
        case CI_TOKEN_KIND_KEYWORD_INT:
            return "CI_TOKEN_KIND_KEYWORD_INT";
        case CI_TOKEN_KIND_KEYWORD_LONG:
            return "CI_TOKEN_KIND_KEYWORD_LONG";
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE:
            return "CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE";
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX:
            return "CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX";
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY:
            return "CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY";
        case CI_TOKEN_KIND_KEYWORD_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_LONG_INT";
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG:
            return "CI_TOKEN_KIND_KEYWORD_LONG_LONG";
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT";
        case CI_TOKEN_KIND_KEYWORD_NULLPTR:
            return "CI_TOKEN_KIND_KEYWORD_NULLPTR";
        case CI_TOKEN_KIND_KEYWORD_REGISTER:
            return "CI_TOKEN_KIND_KEYWORD_REGISTER";
        case CI_TOKEN_KIND_KEYWORD_RESTRICT:
            return "CI_TOKEN_KIND_KEYWORD_RESTRICT";
        case CI_TOKEN_KIND_KEYWORD_RETURN:
            return "CI_TOKEN_KIND_KEYWORD_RETURN";
        case CI_TOKEN_KIND_KEYWORD_SHORT:
            return "CI_TOKEN_KIND_KEYWORD_SHORT";
        case CI_TOKEN_KIND_KEYWORD_SHORT_INT:
            return "CI_TOKEN_KIND_KEYWORD_SHORT_INT";
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_INT:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_INT";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_LONG";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT";
        case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT:
            return "CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT";
        case CI_TOKEN_KIND_KEYWORD_SIZEOF:
            return "CI_TOKEN_KIND_KEYWORD_SIZEOF";
        case CI_TOKEN_KIND_KEYWORD_STATIC:
            return "CI_TOKEN_KIND_KEYWORD_STATIC";
        case CI_TOKEN_KIND_KEYWORD_STATIC_ASSERT:
            return "CI_TOKEN_KIND_KEYWORD_STATIC_ASSERT";
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
            return "CI_TOKEN_KIND_KEYWORD_STRUCT";
        case CI_TOKEN_KIND_KEYWORD_SWITCH:
            return "CI_TOKEN_KIND_KEYWORD_SWITCH";
        case CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL:
            return "CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL";
        case CI_TOKEN_KIND_KEYWORD_TRUE:
            return "CI_TOKEN_KIND_KEYWORD_TRUE";
        case CI_TOKEN_KIND_KEYWORD_TYPEDEF:
            return "CI_TOKEN_KIND_KEYWORD_TYPEDEF";
        case CI_TOKEN_KIND_KEYWORD_TYPEOF:
            return "CI_TOKEN_KIND_KEYWORD_TYPEOF";
        case CI_TOKEN_KIND_KEYWORD_TYPEOF_UNQUAL:
            return "CI_TOKEN_KIND_KEYWORD_TYPEOF_UNQUAL";
        case CI_TOKEN_KIND_KEYWORD_UNION:
            return "CI_TOKEN_KIND_KEYWORD_UNION";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT";
        case CI_TOKEN_KIND_KEYWORD_VOID:
            return "CI_TOKEN_KIND_KEYWORD_VOID";
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
            return "CI_TOKEN_KIND_KEYWORD_VOLATILE";
        case CI_TOKEN_KIND_KEYWORD_WHILE:
            return "CI_TOKEN_KIND_KEYWORD_WHILE";
        case CI_TOKEN_KIND_KEYWORD__ALIGNAS:
            return "CI_TOKEN_KIND_KEYWORD__ALIGNAS";
        case CI_TOKEN_KIND_KEYWORD__ALIGNOF:
            return "CI_TOKEN_KIND_KEYWORD__ALIGNOF";
        case CI_TOKEN_KIND_KEYWORD__ATOMIC:
            return "CI_TOKEN_KIND_KEYWORD__ATOMIC";
        case CI_TOKEN_KIND_KEYWORD__BITINT:
            return "CI_TOKEN_KIND_KEYWORD__BITINT";
        case CI_TOKEN_KIND_KEYWORD__BOOL:
            return "CI_TOKEN_KIND_KEYWORD__BOOL";
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
            return "CI_TOKEN_KIND_KEYWORD__COMPLEX";
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
            return "CI_TOKEN_KIND_KEYWORD__DECIMAL128";
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
            return "CI_TOKEN_KIND_KEYWORD__DECIMAL32";
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
            return "CI_TOKEN_KIND_KEYWORD__DECIMAL64";
        case CI_TOKEN_KIND_KEYWORD__GENERIC:
            return "CI_TOKEN_KIND_KEYWORD__GENERIC";
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return "CI_TOKEN_KIND_KEYWORD__IMAGINARY";
        case CI_TOKEN_KIND_KEYWORD__NORETURN:
            return "CI_TOKEN_KIND_KEYWORD__NORETURN";
        case CI_TOKEN_KIND_KEYWORD__STATIC_ASSERT:
            return "CI_TOKEN_KIND_KEYWORD__STATIC_ASSERT";
        case CI_TOKEN_KIND_KEYWORD__THREAD_LOCAL:
            return "CI_TOKEN_KIND_KEYWORD__THREAD_LOCAL";
        case CI_TOKEN_KIND_LBRACE:
            return "CI_TOKEN_KIND_LBRACE";
        case CI_TOKEN_KIND_LHOOK:
            return "CI_TOKEN_KIND_LHOOK";
        case CI_TOKEN_KIND_LPAREN:
            return "CI_TOKEN_KIND_LPAREN";
        case CI_TOKEN_KIND_LSHIFT:
            return "CI_TOKEN_KIND_LSHIFT";
        case CI_TOKEN_KIND_LSHIFT_EQ:
            return "CI_TOKEN_KIND_LSHIFT_EQ";
        case CI_TOKEN_KIND_LSHIFT_LSHIFT:
            return "CI_TOKEN_KIND_LSHIFT_LSHIFT";
        case CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ:
            return "CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ";
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return "CI_TOKEN_KIND_LITERAL_CONSTANT_INT";
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return "CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT";
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return "CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL";
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return "CI_TOKEN_KIND_LITERAL_CONSTANT_HEX";
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return "CI_TOKEN_KIND_LITERAL_CONSTANT_BIN";
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return "CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER";
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return "CI_TOKEN_KIND_LITERAL_CONSTANT_STRING";
        case CI_TOKEN_KIND_MACRO_DEFINED:
            return "CI_TOKEN_KIND_MACRO_DEFINED";
        case CI_TOKEN_KIND_MACRO_PARAM:
            return "CI_TOKEN_KIND_MACRO_PARAM";
        case CI_TOKEN_KIND_MACRO_PARAM_VARIADIC:
            return "CI_TOKEN_KIND_MACRO_PARAM_VARIADIC";
        case CI_TOKEN_KIND_MINUS:
            return "CI_TOKEN_KIND_MINUS";
        case CI_TOKEN_KIND_MINUS_EQ:
            return "CI_TOKEN_KIND_MINUS_EQ";
        case CI_TOKEN_KIND_MINUS_MINUS:
            return "CI_TOKEN_KIND_MINUS_MINUS";
        case CI_TOKEN_KIND_PERCENTAGE:
            return "CI_TOKEN_KIND_PERCENTAGE";
        case CI_TOKEN_KIND_PERCENTAGE_EQ:
            return "CI_TOKEN_KIND_PERCENTAGE_EQ";
        case CI_TOKEN_KIND_PLUS:
            return "CI_TOKEN_KIND_PLUS";
        case CI_TOKEN_KIND_PLUS_EQ:
            return "CI_TOKEN_KIND_PLUS_EQ";
        case CI_TOKEN_KIND_PLUS_PLUS:
            return "CI_TOKEN_KIND_PLUS_PLUS";
        case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
            return "CI_TOKEN_KIND_PREPROCESSOR_DEFINE";
        case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
            return "CI_TOKEN_KIND_PREPROCESSOR_ELIF";
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
            return "CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF";
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
            return "CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF";
        case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
            return "CI_TOKEN_KIND_PREPROCESSOR_ELSE";
        case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
            return "CI_TOKEN_KIND_PREPROCESSOR_EMBED";
        case CI_TOKEN_KIND_PREPROCESSOR_ENDIF:
            return "CI_TOKEN_KIND_PREPROCESSOR_ENDIF";
        case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
            return "CI_TOKEN_KIND_PREPROCESSOR_ERROR";
        case CI_TOKEN_KIND_PREPROCESSOR_IF:
            return "CI_TOKEN_KIND_PREPROCESSOR_IF";
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
            return "CI_TOKEN_KIND_PREPROCESSOR_IFDEF";
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
            return "CI_TOKEN_KIND_PREPROCESSOR_IFNDEF";
        case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
            return "CI_TOKEN_KIND_PREPROCESSOR_INCLUDE";
        case CI_TOKEN_KIND_PREPROCESSOR_LINE:
            return "CI_TOKEN_KIND_PREPROCESSOR_LINE";
        case CI_TOKEN_KIND_PREPROCESSOR_PRAGMA:
            return "CI_TOKEN_KIND_PREPROCESSOR_PRAGMA";
        case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
            return "CI_TOKEN_KIND_PREPROCESSOR_UNDEF";
        case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
            return "CI_TOKEN_KIND_PREPROCESSOR_WARNING";
        case CI_TOKEN_KIND_RBRACE:
            return "CI_TOKEN_KIND_RBRACE";
        case CI_TOKEN_KIND_RHOOK:
            return "CI_TOKEN_KIND_RHOOK";
        case CI_TOKEN_KIND_RPAREN:
            return "CI_TOKEN_KIND_RPAREN";
        case CI_TOKEN_KIND_RSHIFT:
            return "CI_TOKEN_KIND_RSHIFT";
        case CI_TOKEN_KIND_RSHIFT_EQ:
            return "CI_TOKEN_KIND_RSHIFT_EQ";
        case CI_TOKEN_KIND_RSHIFT_RSHIFT:
            return "CI_TOKEN_KIND_RSHIFT_RSHIFT";
        case CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ:
            return "CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ";
        case CI_TOKEN_KIND_SEMICOLON:
            return "CI_TOKEN_KIND_SEMICOLON";
        case CI_TOKEN_KIND_SLASH:
            return "CI_TOKEN_KIND_SLASH";
        case CI_TOKEN_KIND_SLASH_EQ:
            return "CI_TOKEN_KIND_SLASH_EQ";
        case CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE:
            return "CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE";
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__:
            return "CI_TOKEN_KIND_PREDEFINED_MACRO___DATE__";
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__:
            return "CI_TOKEN_KIND_PREDEFINED_MACRO___FILE__";
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__:
            return "CI_TOKEN_KIND_PREDEFINED_MACRO___LINE__";
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC__:
            return "CI_TOKEN_KIND_PREDEFINED_MACRO___STDC__";
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_VERSION__:
            return "CI_TOKEN_KIND_PREDEFINED_MACRO___STDC_VERSION__";
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_HOSTED__:
            return "CI_TOKEN_KIND_PREDEFINED_MACRO___STDC_HOSTED__";
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__:
            return "CI_TOKEN_KIND_PREDEFINED_MACRO___TIME__";
        case CI_TOKEN_KIND_STAR:
            return "CI_TOKEN_KIND_STAR";
        case CI_TOKEN_KIND_STAR_EQ:
            return "CI_TOKEN_KIND_STAR_EQ";
        case CI_TOKEN_KIND_WAVE:
            return "CI_TOKEN_KIND_WAVE";
        case CI_TOKEN_KIND_WAVE_EQ:
            return "CI_TOKEN_KIND_WAVE_EQ";
        case CI_TOKEN_KIND_BUILTIN_MACRO:
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO:
        case CI_TOKEN_KIND_MAX:
            UNREACHABLE("this token is not used");
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIToken, const CIToken *self)
{
    switch (self->kind) {
        case CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "attribute_deprecated = {s} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->attribute_deprecated
                            ? self->attribute_deprecated->buffer
                            : "NULL");
        case CI_TOKEN_KIND_ATTRIBUTE_NODISCARD:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "attribute_nodiscard = {s} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->attribute_nodiscard
                            ? self->attribute_nodiscard->buffer
                            : "NULL");
        case CI_TOKEN_KIND_COMMENT_DOC:
            return format(
              "CIToken{{ kind = {s}, location = {sa}, comment_doc = {S} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->comment_doc);
        case CI_TOKEN_KIND_EOT:
            return format("CIToken{{ kind = {s}, location = {sa}, eot = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string, CITokenEot, &self->eot));
        case CI_TOKEN_KIND_IDENTIFIER:
            return format(
              "CIToken{{ kind = {s}, location = {sa}, identifier = {S} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->identifier);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_int = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenLiteralConstantInt,
                                          &self->literal_constant_int));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_float = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenLiteralConstantFloat,
                                          &self->literal_constant_float));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_octal = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenLiteralConstantInt,
                                          &self->literal_constant_octal));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_hex = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenLiteralConstantInt,
                                          &self->literal_constant_hex));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_bin = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenLiteralConstantInt,
                                          &self->literal_constant_bin));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_character = {c} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_character);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_string = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_string);
        case CI_TOKEN_KIND_MACRO_DEFINED:
            return format(
              "CIToken{{ kind = {s}, location = {sa}, macro_defined = {S} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->macro_defined);
        case CI_TOKEN_KIND_MACRO_PARAM:
            return format(
              "CIToken{{ kind = {s}, location = {sa}, macro_param = {Sr} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              CALL_DEBUG_IMPL(
                to_string, CITokenMacroParam, &self->macro_param));
        case CI_TOKEN_KIND_MACRO_PARAM_VARIADIC:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "macro_param_variadic = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenMacroParamVariadic,
                                          &self->macro_param_variadic));
        case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_define = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorDefine,
                                          &self->preprocessor_define));
        case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_error = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->preprocessor_error);
        case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_elif = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorElif,
                                          &self->preprocessor_elif));
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_elifdef = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorElifdef,
                                          &self->preprocessor_elifdef));
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_elifndef = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorElifndef,
                                          &self->preprocessor_elifndef));
        case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_else = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorElse,
                                          &self->preprocessor_else));
        case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_embed = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorEmbed,
                                          &self->preprocessor_embed));
        case CI_TOKEN_KIND_PREPROCESSOR_IF:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_if = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorIf,
                                          &self->preprocessor_if));
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_ifdef = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorIfdef,
                                          &self->preprocessor_ifdef));
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_ifndef = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorIfndef,
                                          &self->preprocessor_ifndef));
        case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_include = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorInclude,
                                          &self->preprocessor_include));
        case CI_TOKEN_KIND_PREPROCESSOR_LINE:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_line = {Sr} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          CALL_DEBUG_IMPL(to_string,
                                          CITokenPreprocessorLine,
                                          &self->preprocessor_line));
        case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_undef = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->preprocessor_undef);
        case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "preprocessor_warning = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->preprocessor_warning);
        case CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE:
            return format(
              "CIToken{{ kind = {s}, location = {sa}, has_feature = {s} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              CALL_DEBUG_IMPL(
                to_string, CIExtensionsHasFeature, self->has_feature));
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "standard_predefined_macro___date__ = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->standard_predefined_macro___date__);
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__:
            return format("CIToken{{ kind = {s}, location = {sa}, "
                          "standard_predefined_macro___time__ = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->standard_predefined_macro___time__);
        default:
            return format(
              "CIToken{{ kind = {s}, location = {sa} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location));
    }
}
#endif

#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, CIToken, const CIToken *self)
{
    PRINTLN("{sa}", CALL_DEBUG_IMPL(to_string, CIToken, self));
}
#endif

VARIANT_DESTRUCTOR(CIToken, attribute_deprecated, CIToken *self)
{
    if (self->attribute_deprecated) {
        FREE(String, self->attribute_deprecated);
    }

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, attribute_nodiscard, CIToken *self)
{
    if (self->attribute_nodiscard) {
        FREE(String, self->attribute_nodiscard);
    }

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, comment_doc, CIToken *self)
{
    FREE(String, self->comment_doc);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, eot, CIToken *self)
{
    FREE(CITokenEot, &self->eot);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, identifier, CIToken *self)
{
    FREE(String, self->identifier);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_int, CIToken *self)
{
    FREE(CITokenLiteralConstantInt, &self->literal_constant_int);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_float, CIToken *self)
{
    FREE(CITokenLiteralConstantFloat, &self->literal_constant_float);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_octal, CIToken *self)
{
    FREE(CITokenLiteralConstantInt, &self->literal_constant_octal);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_hex, CIToken *self)
{
    FREE(CITokenLiteralConstantInt, &self->literal_constant_hex);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_bin, CIToken *self)
{
    FREE(CITokenLiteralConstantInt, &self->literal_constant_bin);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_character, CIToken *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_string, CIToken *self)
{
    FREE(String, self->literal_constant_string);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, macro_defined, CIToken *self)
{
    FREE(String, self->macro_defined);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, macro_param, CIToken *self)
{
    FREE(CITokenMacroParam, &self->macro_param);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, macro_param_variadic, CIToken *self)
{
    FREE(CITokenMacroParamVariadic, &self->macro_param_variadic);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_define, CIToken *self)
{
    FREE(CITokenPreprocessorDefine, &self->preprocessor_define);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_elif, CIToken *self)
{
    FREE(CITokenPreprocessorElif, &self->preprocessor_elif);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_elifdef, CIToken *self)
{
    FREE(CITokenPreprocessorElifdef, &self->preprocessor_elifdef);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_elifndef, CIToken *self)
{
    FREE(CITokenPreprocessorElifndef, &self->preprocessor_elifndef);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_else, CIToken *self)
{
    FREE(CITokenPreprocessorElse, &self->preprocessor_else);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_embed, CIToken *self)
{
    FREE(CITokenPreprocessorEmbed, &self->preprocessor_embed);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_if, CIToken *self)
{
    FREE(CITokenPreprocessorIf, &self->preprocessor_if);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_ifdef, CIToken *self)
{
    FREE(CITokenPreprocessorIfdef, &self->preprocessor_ifdef);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_ifndef, CIToken *self)
{
    FREE(CITokenPreprocessorIfndef, &self->preprocessor_ifndef);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_error, CIToken *self)
{
    FREE(String, self->preprocessor_error);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_include, CIToken *self)
{
    FREE(CITokenPreprocessorInclude, &self->preprocessor_include);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_line, CIToken *self)
{
    FREE(CITokenPreprocessorLine, &self->preprocessor_line);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_undef, CIToken *self)
{
    FREE(String, self->preprocessor_undef);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, preprocessor_warning, CIToken *self)
{
    FREE(String, self->preprocessor_warning);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, standard_predefined_macro___date__, CIToken *self)
{
    FREE(String, self->standard_predefined_macro___date__);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, standard_predefined_macro___time__, CIToken *self)
{
    FREE(String, self->standard_predefined_macro___time__);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, has_feature, CIToken *self)
{
    lily_free(self);
}

DESTRUCTOR(CIToken, CIToken *self)
{
    switch (self->kind) {
        case CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED:
            FREE_VARIANT(CIToken, attribute_deprecated, self);
            break;
        case CI_TOKEN_KIND_ATTRIBUTE_NODISCARD:
            FREE_VARIANT(CIToken, attribute_nodiscard, self);
            break;
        case CI_TOKEN_KIND_COMMENT_DOC:
            FREE_VARIANT(CIToken, comment_doc, self);
            break;
        case CI_TOKEN_KIND_EOT:
            FREE_VARIANT(CIToken, eot, self);
            break;
        case CI_TOKEN_KIND_IDENTIFIER:
            FREE_VARIANT(CIToken, identifier, self);
            break;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            FREE_VARIANT(CIToken, literal_constant_int, self);
            break;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            FREE_VARIANT(CIToken, literal_constant_float, self);
            break;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            FREE_VARIANT(CIToken, literal_constant_octal, self);
            break;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            FREE_VARIANT(CIToken, literal_constant_hex, self);
            break;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            FREE_VARIANT(CIToken, literal_constant_bin, self);
            break;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            FREE_VARIANT(CIToken, literal_constant_character, self);
            break;
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            FREE_VARIANT(CIToken, literal_constant_string, self);
            break;
        case CI_TOKEN_KIND_MACRO_DEFINED:
            FREE_VARIANT(CIToken, macro_defined, self);
            break;
        case CI_TOKEN_KIND_MACRO_PARAM:
            FREE_VARIANT(CIToken, macro_param, self);
            break;
        case CI_TOKEN_KIND_MACRO_PARAM_VARIADIC:
            FREE_VARIANT(CIToken, macro_param_variadic, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
            FREE_VARIANT(CIToken, preprocessor_define, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
            FREE_VARIANT(CIToken, preprocessor_elif, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
            FREE_VARIANT(CIToken, preprocessor_elifdef, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
            FREE_VARIANT(CIToken, preprocessor_elifndef, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
            FREE_VARIANT(CIToken, preprocessor_else, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
            FREE_VARIANT(CIToken, preprocessor_embed, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_IF:
            FREE_VARIANT(CIToken, preprocessor_if, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
            FREE_VARIANT(CIToken, preprocessor_ifdef, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
            FREE_VARIANT(CIToken, preprocessor_ifndef, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
            FREE_VARIANT(CIToken, preprocessor_error, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
            FREE_VARIANT(CIToken, preprocessor_include, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_LINE:
            FREE_VARIANT(CIToken, preprocessor_line, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
            FREE_VARIANT(CIToken, preprocessor_undef, self);
            break;
        case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
            FREE_VARIANT(CIToken, preprocessor_warning, self);
            break;
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__:
            FREE_VARIANT(CIToken, standard_predefined_macro___date__, self);
            break;
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__:
            FREE_VARIANT(CIToken, standard_predefined_macro___time__, self);
            break;
        default:
            lily_free(self);
    }
}
