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

#include <core/cc/ci/parser.h>
#include <core/cc/ci/result.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static String *
generate_name_error__CIParser();

/// @brief Advance to one token.
static inline void
next_token__CIParser(CIParser *self);

/// @brief Peek token at position + n.
static inline CIToken *
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

/// @brief Parse data type.
static CIDataType *
parse_data_type__CIParser(CIParser *self);

/// @brief Parse enum declaration.
static CIDeclEnum
parse_enum__CIParser(CIParser *self, String *name);

/// @brief Parse function params.
/// @return Vec<CIDeclFunctionParam*>*?
static Vec *
parse_function_params__CIParser(CIParser *self);

/// @brief Parse function body.
/// @return Vec<CIDeclFunctionItem*>*
static Vec *
parse_function_body__CIParser(CIParser *self);

/// @brief Parse function declaration.
static CIDecl *
parse_function__CIParser(CIParser *self, int storage_class_flag, String *name);

/// @brief Parse struct declaration.
static CIDeclStruct
parse_struct__CIParser(CIParser *self);

/// @brief Parse union declaration.
static CIDeclUnion
parse_union__CIParser(CIParser *self);

/// @brief Parse variable declaration.
static CIDecl *
parse_variable__CIParser(CIParser *self);

/// @brief Parse declaration.
static CIDecl *
parse_decl__CIParser(CIParser *self);

/// @brief Look for macro.
static bool
look_for_macro__CIParser(CIParser *self);

/// @brief Parse storage class specifier.
/// @return true if the token is a storage class specifier, false otherwise.
static bool
parse_storage_class_specifier__CIParser(CIParser *self,
                                        int *storage_class_flag);

/// @brief Parse many storage class specifiers.
static void
parse_storage_class_specifiers__CIParser(CIParser *self,
                                         int *storage_class_flag);

#define LOOK_FOR_MACRO_AND_DO_ACTION(action)          \
    switch (self->tokens_iters.current_token->kind) { \
        case CI_TOKEN_KIND_IDENTIFIER:                \
            look_for_macro__CIParser(self);           \
        default:                                      \
            action;                                   \
    }

// NOTE: If the program is multi-threaded, you'll need to adapt these variables
// to multi-threading.
static Vec *names_error = NULL; // Vec<String*>*
static Usize name_error_count = 0;

CONSTRUCTOR(CIParserMacro *, CIParserMacro, Vec *params)
{
    CIParserMacro *self = lily_malloc(sizeof(CIParserMacro));

    self->params = params;

    return self;
}

CONSTRUCTOR(CIParser, CIParser, CIResultFile *file, const CIScanner *scanner)
{
    CITokensIters tokens_iters = NEW(CITokensIters);
    CITokensIter *iter = NEW(CITokensIter, scanner->tokens);

    add_iter__CITokensIters(&tokens_iters, iter);

    if (!names_error) {
        names_error = NEW(Vec);
    }

    return (CIParser){ .file = file,
                       .scanner = scanner,
                       .count_error = scanner->base.count_error,
                       .tokens_iters = tokens_iters,
                       .macros = NEW(Stack, CI_PARSER_MACROS_MAX_SIZE) };
}

String *
generate_name_error__CIParser()
{
    ASSERT(names_error);

    String *name_error = format__String("%__error__{zu}", name_error_count++);

    push__Vec(names_error, name_error);

    return name_error;
}

void
next_token__CIParser(CIParser *self)
{
    next_token__CITokensIters(&self->tokens_iters);
}

CIToken *
peek_token__CIParser(CIParser *self, Usize n)
{
    return peek_token__CITokensIters(
      &self->tokens_iters, self->file, self->macros, n);
}

bool
expect__CIParser(CIParser *self, enum CITokenKind kind, bool emit_error)
{
    if (self->tokens_iters.current_token->kind == kind) {
        next_token__CIParser(self);

        return true;
    }

    if (emit_error) {
        FAILED("expected ...");
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
    LOOK_FOR_MACRO_AND_DO_ACTION(
      switch (self->tokens_iters.current_token->kind) {
          case CI_TOKEN_KIND_IDENTIFIER:
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
      });
}

CIDataType *
parse_data_type__CIParser(CIParser *self)
{
    CIDataType *res = NULL;

    next_token__CIParser(self);

    switch (self->tokens_iters.previous_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            TODO("type");
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
                case CI_TOKEN_KIND_LBRACE: {
                    next_token__CIParser(self);

                    CIDecl *enum_decl =
                      NEW_VARIANT(CIDecl,
                                  enum,
                                  CI_STORAGE_CLASS_NONE,
                                  false,
                                  parse_enum__CIParser(self, name));

                    if (add_decl__CIResultFile(self->file, enum_decl)) {
                        FREE(CIDecl, enum_decl);

                        FAILED("duplicate enum declaration");
                    }

                    break;
                }
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
            TODO("struct");
        case CI_TOKEN_KIND_KEYWORD_UNION:
            TODO("union");
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

CIDeclEnum
parse_enum__CIParser(CIParser *self, String *name)
{
    next_token__CIParser(self); // skip `{`

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

    if (variants->len == 0) {
        FAILED("expected one or many variants");
    }

    return NEW(CIDeclEnum, name, variants);
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

    next_token__CIParser(self);

    return params;
}

Vec *
parse_function_body__CIParser(CIParser *self)
{
    TODO("parse function body");
}

CIDecl *
parse_function__CIParser(CIParser *self, int storage_class_flag, String *name)
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
                               NEW(CIDeclFunction, name, params, NULL));
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);

            return NEW_VARIANT(CIDecl,
                               function,
                               storage_class_flag,
                               false,
                               NEW(CIDeclFunction,
                                   name,
                                   params,
                                   parse_function_body__CIParser(self)));
        default:
            FAILED("expected `;` or `{`");
    }
}

CIDeclStruct
parse_struct__CIParser(CIParser *self)
{
    TODO("struct");
}

CIDeclUnion
parse_union__CIParser(CIParser *self)
{
    TODO("union");
}

CIDecl *
parse_variable__CIParser(CIParser *self)
{
    TODO("variable");
}

CIDecl *
parse_decl__CIParser(CIParser *self)
{
    int storage_class_flag = CI_STORAGE_CLASS_NONE;
    enum CIDeclKind kind = 0;

    parse_storage_class_specifiers__CIParser(self, &storage_class_flag);

    // struct <name>;
    // struct <name> { ... }
    // enum <name>;
    // enum <name> { ... }
    // union <name>;
    // union <name> { ... }

    CIDataType *data_type = parse_data_type__CIParser(self);

    LOOK_FOR_MACRO_AND_DO_ACTION(
      switch (self->tokens_iters.current_token->kind) {
          case CI_TOKEN_KIND_KEYWORD_STRUCT:
              kind = CI_DECL_KIND_STRUCT | CI_DECL_KIND_FUNCTION |
                     CI_DECL_KIND_VARIABLE;
              break;
          case CI_TOKEN_KIND_KEYWORD_ENUM:
              kind = CI_DECL_KIND_ENUM | CI_DECL_KIND_FUNCTION |
                     CI_DECL_KIND_VARIABLE;
              break;
          case CI_TOKEN_KIND_KEYWORD_UNION:
              kind = CI_DECL_KIND_UNION | CI_DECL_KIND_FUNCTION |
                     CI_DECL_KIND_VARIABLE;
              break;
          default:
              if (is_data_type__CIParser(self)) {
                  // The parser doesn't know whether it's a function or a
                  // variable.
                  kind = CI_DECL_KIND_FUNCTION | CI_DECL_KIND_VARIABLE;

                  data_type = parse_data_type__CIParser(self);

                  break;
              }

              FAILED("expected struct, enum, union, data type, or identifier");
      });

    if (storage_class_flag & CI_STORAGE_CLASS_INLINE) {
        kind = CI_DECL_KIND_FUNCTION;
    }

    expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
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
    LOOK_FOR_MACRO_AND_DO_ACTION({
        int old_storage_class_flag = *storage_class_flag;

        while (
          parse_storage_class_specifier__CIParser(self, storage_class_flag)) {
            next_token__CIParser(self);

            if (old_storage_class_flag == *storage_class_flag) {
                FAILED("warning: duplicate storage class specifier");
            }

            old_storage_class_flag = *storage_class_flag;
        }
    });
}

void
run__CIParser(CIParser *self)
{
}

DESTRUCTOR(CIParser, const CIParser *self)
{
    if (names_error) {
        FREE_BUFFER_ITEMS(names_error->buffer, names_error->len, String);
        FREE(Vec, names_error);

        names_error = NULL;
    }

    FREE(CITokensIters, &self->tokens_iters);
    FREE_STACK_ITEMS(self->macros, CIParserMacro);
    FREE(Stack, self->macros);
}
