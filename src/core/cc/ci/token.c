/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#include <base/print.h>
#endif

#include <core/cc/ci/token.h>

#include <stdio.h>
#include <stdlib.h>

// Free CIToken type (CI_TOKEN_KIND_COMMENT_DOC).
static VARIANT_DESTRUCTOR(CIToken, comment_doc, CIToken *self);

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

CONSTRUCTOR(CIToken *, CIToken, enum CITokenKind kind, Location location)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = kind;
    self->location = location;

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
    self->comment_doc = comment_doc;

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
    self->identifier = identifier;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_int,
                    Location location,
                    String *literal_constant_int)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_INT;
    self->location = location;
    self->literal_constant_int = literal_constant_int;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_float,
                    Location location,
                    String *literal_constant_float)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT;
    self->location = location;
    self->literal_constant_float = literal_constant_float;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_octal,
                    Location location,
                    String *literal_constant_octal)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL;
    self->location = location;
    self->literal_constant_octal = literal_constant_octal;

    return self;
}

VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_hex,
                    Location location,
                    String *literal_constant_hex)
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
                    String *literal_constant_bin)
{
    CIToken *self = lily_malloc(sizeof(CIToken));

    self->kind = CI_TOKEN_KIND_LITERAL_CONSTANT_BIN;
    self->location = location;
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
    self->literal_constant_string = literal_constant_string;

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
        case CI_TOKEN_KIND_EQ:
            return from__String("=");
        case CI_TOKEN_KIND_EQ_EQ:
            return from__String("==");
        case CI_TOKEN_KIND_HASHTAG:
            return from__String("#");
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
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            return from__String("else");
        case CI_TOKEN_KIND_KEYWORD_ENUM:
            return from__String("enum");
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
            return from__String("extern");
        case CI_TOKEN_KIND_KEYWORD_FALSE:
            return from__String("false");
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            return from__String("float");
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
        case CI_TOKEN_KIND_KEYWORD_LONG_INT:
            return from__String("long int");
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
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT:
            return from__String("unsigned long int");
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT:
            return from__String("unsigned long long int");
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
                                  self->literal_constant_int);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return format__String("LITERAL_CONSTANT(FLOAT({S}))",
                                  self->literal_constant_float);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return format__String("LITERAL_CONSTANT(OCTAL({S}))",
                                  self->literal_constant_octal);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return format__String("LITERAL_CONSTANT(HEX({S}))",
                                  self->literal_constant_hex);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return format__String("LITERAL_CONSTANT(BIN({S}))",
                                  self->literal_constant_bin);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return format__String("LITERAL_CONSTANT(CHARACTER({c}))",
                                  self->literal_constant_character);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return format__String("LITERAL_CONSTANT(STRING({S}))",
                                  self->literal_constant_string);
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
        case CI_TOKEN_KIND_STAR:
            return from__String("*");
        case CI_TOKEN_KIND_STAR_EQ:
            return from__String("*=");
        case CI_TOKEN_KIND_WAVE:
            return from__String("~");
        case CI_TOKEN_KIND_WAVE_EQ:
            return from__String("~=");
        case CI_TOKEN_KIND_MAX:
            UNREACHABLE("this token is not used");
        default:
            UNREACHABLE("unknown variant");
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
        case CI_TOKEN_KIND_EQ:
            return "CI_TOKEN_KIND_EQ";
        case CI_TOKEN_KIND_EQ_EQ:
            return "CI_TOKEN_KIND_EQ_EQ";
        case CI_TOKEN_KIND_HASHTAG:
            return "CI_TOKEN_KIND_HASHTAG";
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
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            return "CI_TOKEN_KIND_KEYWORD_ELSE";
        case CI_TOKEN_KIND_KEYWORD_ENUM:
            return "CI_TOKEN_KIND_KEYWORD_ENUM";
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
            return "CI_TOKEN_KIND_KEYWORD_EXTERN";
        case CI_TOKEN_KIND_KEYWORD_FALSE:
            return "CI_TOKEN_KIND_KEYWORD_FALSE";
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            return "CI_TOKEN_KIND_KEYWORD_FLOAT";
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
        case CI_TOKEN_KIND_KEYWORD_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_LONG_INT";
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
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT";
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT:
            return "CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT";
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
        case CI_TOKEN_KIND_STAR:
            return "CI_TOKEN_KIND_STAR";
        case CI_TOKEN_KIND_STAR_EQ:
            return "CI_TOKEN_KIND_STAR_EQ";
        case CI_TOKEN_KIND_WAVE:
            return "CI_TOKEN_KIND_WAVE";
        case CI_TOKEN_KIND_WAVE_EQ:
            return "CI_TOKEN_KIND_WAVE_EQ";
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
        case CI_TOKEN_KIND_COMMENT_DOC:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, comment_doc = {S} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->comment_doc);
        case CI_TOKEN_KIND_IDENTIFIER:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, identifier = {S} }",
              CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->identifier);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_int = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_int);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_float = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_float);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_octal = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_octal);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_hex = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_hex);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_bin = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_bin);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_character = {c} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_character);
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_constant_string = {S} }",
                          CALL_DEBUG_IMPL(to_string, CITokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_constant_character);
        default:
            return format(
              "LilyToken{{ kind = {s}, location = {sa} }",
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

VARIANT_DESTRUCTOR(CIToken, comment_doc, CIToken *self)
{
    FREE(String, self->comment_doc);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, identifier, CIToken *self)
{
    FREE(String, self->identifier);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_int, CIToken *self)
{
    FREE(String, self->literal_constant_int);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_float, CIToken *self)
{
    FREE(String, self->literal_constant_float);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_octal, CIToken *self)
{
    FREE(String, self->literal_constant_octal);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_hex, CIToken *self)
{
    FREE(String, self->literal_constant_hex);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIToken, literal_constant_bin, CIToken *self)
{
    FREE(String, self->literal_constant_bin);
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

DESTRUCTOR(CIToken, CIToken *self)
{
    switch (self->kind) {
        case CI_TOKEN_KIND_COMMENT_DOC:
            FREE_VARIANT(CIToken, comment_doc, self);
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
        default:
            lily_free(self);
    }
}
