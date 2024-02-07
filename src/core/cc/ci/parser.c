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

#include <core/cc/ci/parser.h>
#include <core/cc/ci/result.h>

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
static Vec *
parse_generic_params__CIParser(CIParser *self);

/// @brief Parser function call.
static CIExpr *
parse_function_call__CIParser(CIParser *self, String *identifier);

/// @brief Parse literal expression.
/// @brief @return CIExpr*?
static CIExpr *
parse_literal_expr__CIParser(CIParser *self);

/// @brief Parse primary expression.
/// @return CIExpr*?
static CIExpr *
parse_primary_expr__CIParser(CIParser *self);

static CIExpr *
parse_binary_expr__CIParser(CIParser *self, CIExpr *expr);

/// @brief Parse expression.
/// @return CIExpr*?
static CIExpr *
parse_expr__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_for_stmt__CIParser(CIParser *self);

static CIStmtIfBranch *
parse_if_branch__CIParser(CIParser *self, bool in_loop);

static CIDeclFunctionItem *
parse_if_stmt__CIParser(CIParser *self, bool in_loop);

static CIDeclFunctionItem *
parse_while_stmt__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_block_stmt__CIParser(CIParser *self);

/// @brief Parse statement.
/// @return CIDeclFunctionItem*?
static CIDeclFunctionItem *
parse_stmt__CIParser(CIParser *self, bool in_loop);

/// @brief Parse function body.
/// @return Vec<CIDeclFunctionItem*>*
static Vec *
parse_function_body__CIParser(CIParser *self, bool in_loop);

static CIDeclFunctionItem *
parse_function_body_item__CIParser(CIParser *self, bool in_loop);

/// @brief Parse function declaration.
static CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         String *name);

/// @brief Parse struct (union) fields.
static Vec *
parse_struct_fields__CIParser(CIParser *self);

/// @brief Parse struct declaration.
static CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       String *name,
                       Vec *generic_params);

/// @brief Parse union declaration.
static CIDeclUnion
parse_union__CIParser(CIParser *self, String *name, Vec *generic_params);

/// @brief Parse variable declaration.
static CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         String *name,
                         bool is_prototype);

/// @brief Parse declaration.
static CIDecl *
parse_decl__CIParser(CIParser *self, bool in_function_body);

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

CONSTRUCTOR(struct CIParserContext, CIParserContext)
{
    return (struct CIParserContext){ .current_decl = NULL,
                                     .current_expr = NULL,
                                     .current_stmt = NULL };
}

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

    ctx = NEW(CIParserContext);

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
parse_data_type__CIParser(CIParser *self)
{
    CIDataType *res = NULL;

    next_token__CIParser(self);

    switch (self->tokens_iters.previous_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            res = NEW_VARIANT(CIDataType,
                              typedef,
                              self->tokens_iters.previous_token->identifier);

            break;
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
                case CI_TOKEN_KIND_IDENTIFIER:
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
            String *name = NULL;               // String* (&)
            Vec *generic_params = NULL;        // Vec<CIDataType*>*?
            Vec *generic_params_cloned = NULL; // Vec<CIDataType*>*?

            // struct <name><generic_params, ...> ...;
            // struct <name><generic_params, ...> { ... } ...;
            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                name = self->tokens_iters.previous_token->identifier;
            } else {
                name = generate_name_error__CIParser();
            }

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LSHIFT:
                    generic_params = parse_generic_params__CIParser(self);
                    generic_params_cloned = NEW(Vec);

                    for (Usize i = 0; i < generic_params->len; ++i) {
                        push__Vec(
                          generic_params_cloned,
                          clone__CIDataType(get__Vec(generic_params, i)));
                    }

                    break;
                default:
                    break;
            }

            switch (previous_token_kind) {
                case CI_TOKEN_KIND_KEYWORD_STRUCT:
                    res = NEW_VARIANT(
                      CIDataType,
                      struct,
                      NEW(CIDataTypeStruct, name, generic_params_cloned));

                    break;
                case CI_TOKEN_KIND_KEYWORD_UNION:
                    res = NEW_VARIANT(
                      CIDataType,
                      union,
                      NEW(CIDataTypeUnion, name, generic_params_cloned));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    if (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
                        goto parse_struct;
                    }

                    break;
                case CI_TOKEN_KIND_LBRACE:
                case CI_TOKEN_KIND_SEMICOLON: {
                parse_struct : {
                    CIDecl *struct_decl = parse_struct__CIParser(
                      self, storage_class_flag, name, generic_params);

                    if (add_struct__CIResultFile(self->file, struct_decl)) {
                        FREE(CIDecl, struct_decl);

                        FAILED("struct name is already defined");
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

Vec *
parse_generic_params__CIParser(CIParser *self)
{
    next_token__CIParser(self); // skip `<`

    Vec *generic_params = NEW(Vec); // Vec<CIDataType*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RSHIFT &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);

        if (data_type) {
            push__Vec(generic_params, data_type);
        }

        if (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RSHIFT) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    if (generic_params->len == 0) {
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

    return generic_params;
}

CIExpr *
parse_function_call__CIParser(CIParser *self, String *identifier)
{
    next_token__CIParser(self); // skip `(`

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_RPAREN:
            next_token__CIParser(self);

            break;
        case CI_TOKEN_KIND_EOF:
            FAILED("unexpected EOF");
        default:
            UNREACHABLE("expected `)` or `EOF`");
    }

    TODO("parse function call");
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
                    int_s =
                      self->tokens_iters.previous_token->literal_constant_int;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
                    int_s =
                      self->tokens_iters.previous_token->literal_constant_octal;
                    base = 8;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
                    int_s =
                      self->tokens_iters.previous_token->literal_constant_hex;
                    base = 16;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
                    int_s =
                      self->tokens_iters.previous_token->literal_constant_bin;
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
              NEW_VARIANT(CIExprLiteral,
                          float,
                          atof__Float64(self->tokens_iters.previous_token
                                          ->literal_constant_float->buffer)));
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

            TODO("parse grouping expression");
        }
        case CI_TOKEN_KIND_LBRACE:
            TODO("parse struct call");
        case CI_TOKEN_KIND_KEYWORD_SIZEOF: {
            CIExpr *sizeof_expr = parse_expr__CIParser(self);

            if (sizeof_expr) {
                return NEW_VARIANT(CIExpr, sizeof, sizeof_expr);
            }

            return NULL;
        }
        case CI_TOKEN_KIND_IDENTIFIER: {
            String *identifier = self->tokens_iters.previous_token->identifier;
            Vec *generic_params = NULL; // Vec<CIDataType*>*?

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LSHIFT:
                    generic_params = parse_generic_params__CIParser(self);

                    break;
                default:
                    break;
            }

            switch (self->tokens_iters.current_token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                    return parse_function_call__CIParser(self, identifier);
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
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_EQ_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_PLUS_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_MINUS_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_STAR_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_SLASH_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_PERCENTAGE_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_AMPERSAND_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_BAR_EQ ||
      self->tokens_iters.current_token->kind == CI_TOKEN_KIND_BAR_EQ ||
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
        default:
            return expr;
    }
}

CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self)
{
    Vec *body = NULL;

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_body__CIParser(self, true);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true);

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
parse_for_stmt__CIParser(CIParser *self)
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
            body = parse_function_body__CIParser(self, true);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true);

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
parse_if_branch__CIParser(CIParser *self, bool in_loop)
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
            body = parse_function_body__CIParser(self, in_loop);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, in_loop);

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
parse_if_stmt__CIParser(CIParser *self, bool in_loop)
{
    CIStmtIfBranch *if_ = parse_if_branch__CIParser(self, in_loop);
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

        CIStmtIfBranch *else_if = parse_if_branch__CIParser(self, in_loop);

        if (else_if) {
            push__Vec(else_ifs, else_if);
        }
    }

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            if (expect__CIParser(self, CI_TOKEN_KIND_LBRACE, false)) {
                else_ = parse_function_body__CIParser(self, in_loop);
            } else {
                CIDeclFunctionItem *item =
                  parse_function_body_item__CIParser(self, in_loop);

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
parse_while_stmt__CIParser(CIParser *self)
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
            body = parse_function_body__CIParser(self, true);

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
parse_block_stmt__CIParser(CIParser *self)
{
    Vec *body = parse_function_body__CIParser(self, false);

    return NEW_VARIANT(CIDeclFunctionItem,
                       stmt,
                       NEW_VARIANT(CIStmt, block, NEW(CIStmtBlock, body)));
}

CIDeclFunctionItem *
parse_stmt__CIParser(CIParser *self, bool in_loop)
{
    next_token__CIParser(self);

    switch (self->tokens_iters.previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_BREAK:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            if (in_loop) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, break));
            } else {
                FAILED("break is not expected outside of a loop");
            }
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            if (in_loop) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, continue));
            } else {
                FAILED("continue is not expected outside of a loop");
            }
        case CI_TOKEN_KIND_KEYWORD_DO:
            return parse_do_while_stmt__CIParser(self);
        case CI_TOKEN_KIND_KEYWORD_FOR:
            return parse_for_stmt__CIParser(self);
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
        case CI_TOKEN_KIND_KEYWORD_IF:
            return parse_if_stmt__CIParser(self, in_loop);
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
            TODO("switch statement");
        case CI_TOKEN_KIND_KEYWORD_WHILE:
            return parse_while_stmt__CIParser(self);
        case CI_TOKEN_KIND_LBRACE:
            return parse_block_stmt__CIParser(self);
        default:
            UNREACHABLE("not expected token");
    }
}

CIDeclFunctionItem *
parse_function_body_item__CIParser(CIParser *self, bool in_loop)
{
    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_BREAK:
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
        case CI_TOKEN_KIND_KEYWORD_DO:
        case CI_TOKEN_KIND_KEYWORD_FOR:
        case CI_TOKEN_KIND_KEYWORD_GOTO:
        case CI_TOKEN_KIND_KEYWORD_IF:
        case CI_TOKEN_KIND_KEYWORD_RETURN:
        case CI_TOKEN_KIND_KEYWORD_SWITCH:
        case CI_TOKEN_KIND_KEYWORD_WHILE:
        case CI_TOKEN_KIND_LBRACE:
            return parse_stmt__CIParser(self, in_loop);
        case CI_TOKEN_KIND_SEMICOLON:
            return NULL;
        default: {
            if (is_data_type__CIParser(self)) {
                CIDecl *decl = parse_decl__CIParser(self, true);

                if (decl) {
                    return NEW_VARIANT(CIDeclFunctionItem, decl, decl);
                } else if (!data_type_as_expression) {
                    return NULL;
                }
            }

            return NEW_VARIANT(
              CIDeclFunctionItem, expr, parse_expr__CIParser(self));
        }
    }
}

Vec *
parse_function_body__CIParser(CIParser *self, bool in_loop)
{
    Vec *body = NEW(Vec); // Vec<CIDelcFunctionItem*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDeclFunctionItem *item =
          parse_function_body_item__CIParser(self, in_loop);

        if (item) {
            push__Vec(body, item);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return body;
}

CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         String *name)
{
    Vec *params =
      parse_function_params__CIParser(self); // Vec<CIDeclFunctionParam*>*?

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);

            return NEW_VARIANT(
              CIDecl,
              function,
              storage_class_flag,
              true,
              NEW(CIDeclFunction, name, return_data_type, params, NULL));
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);

            return NEW_VARIANT(CIDecl,
                               function,
                               storage_class_flag,
                               false,
                               NEW(CIDeclFunction,
                                   name,
                                   return_data_type,
                                   params,
                                   parse_function_body__CIParser(self, false)));
        default:
            FAILED("expected `;` or `{`");
    }
}

Vec *
parse_struct_fields__CIParser(CIParser *self)
{
    Vec *fields = NEW(Vec); // Vec<CIDeclStructField*>*

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);
        String *name = NULL;

        if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
            name = self->tokens_iters.previous_token->identifier;
        } else {
            name = generate_name_error__CIParser();
        }

        // TODO: parse bits set

        push__Vec(fields, NEW(CIDeclStructField, name, data_type));
        expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return fields;
}

CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       String *name,
                       Vec *generic_params)
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
                       struct,
                       storage_class_flag,
                       false,
                       typedef_name,
                       NEW(CIDeclStruct, name, generic_params, fields));
}

CIDeclUnion
parse_union__CIParser(CIParser *self, String *name, Vec *generic_params)
{
    Vec *fields =
      parse_struct_fields__CIParser(self); // Vec<CIDeclStructField*>*

    return NEW(CIDeclUnion, name, fields, generic_params);
}

CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         String *name,
                         bool is_prototype)
{
    next_token__CIParser(self); // skip `=` or `;`

    if (is_prototype) {
        return NEW_VARIANT(CIDecl,
                           variable,
                           storage_class_flag,
                           true,
                           NEW(CIDeclVariable, data_type, name, NULL));
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
                       true,
                       NEW(CIDeclVariable, data_type, name, expr));
}

CIDecl *
parse_decl__CIParser(CIParser *self, bool in_function_body)
{
    storage_class_flag = CI_STORAGE_CLASS_NONE;

    parse_storage_class_specifiers__CIParser(self, &storage_class_flag);

    CIDataType *data_type = parse_data_type__CIParser(self);

    switch (self->tokens_iters.current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            String *name = self->tokens_iters.current_token->identifier;
            Vec *generic_params = NULL;

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
                        FAILED("no generic params are expected in variable "
                               "declaration");
                    }

                    return parse_variable__CIParser(
                      self, storage_class_flag, data_type, name, false);
                case CI_TOKEN_KIND_SEMICOLON:
                    return parse_variable__CIParser(
                      self, storage_class_flag, data_type, name, true);
                case CI_TOKEN_KIND_LPAREN:
                    return parse_function__CIParser(
                      self, storage_class_flag, data_type, name);
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
    // Initialize the first token.
    next_token__CIParser(self);

    while (self->tokens_iters.current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDecl *decl = parse_decl__CIParser(self, false);

        if (decl) {
            switch (decl->kind) {
                case CI_DECL_KIND_FUNCTION:
                    if (add_function__CIResultFile(self->file, decl)) {
                        FREE(CIDecl, decl);
                        FAILED("function name is already defined");
                    }

                    break;
                case CI_DECL_KIND_VARIABLE:
                    if (add_variable__CIResultFile(self->file, decl)) {
                        FREE(CIDecl, decl);
                        FAILED("variable name is already defined");
                    }

                    break;
                default:
                    UNREACHABLE("this situation is impossible");
            }
        }
    }

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
    FREE_STACK_ITEMS(self->macros, CIParserMacro);
    FREE(Stack, self->macros);
}
