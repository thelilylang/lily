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
#include <base/dir.h>
#include <base/macros.h>

#include <core/cc/ci/generator.h>

#include <stdio.h>
#include <stdlib.h>

static void
run_file__CIGenerator(const CIResultFile *file_result);

static void
write_str__CIGenerator(char *s);

static void
write_String__CIGenerator(String *s);

static inline void
inc_tab_count__CIGenerator();

static inline void
dec_tab_count__CIGenerator();

static inline void
write_tab__CIGenerator();

static void
generate_storage_class__CIGenerator(const int *storage_class_flag);

static void
serialize_data_type__CIGenerator(const CIDataType *data_type);

static inline void
serialize_generic_param__CIGenerator(const CIDataType *data_type);

/// @params generic_params const Vec<CIDataType*>* (&)
static void
serialize_generic_params__CIGenerator(const Vec *generic_params);

static void
generate_data_type__CIGenerator(const CIDataType *data_type);

static void
generate_enum_decl__CIGenerator(const CIDeclEnum *enum_);

static void
generate_enum_variant__CIGenerator(const CIDeclEnumVariant *enum_variant);

static void
generate_enum_variants__CIGenerator(const Vec *enum_variants);

static void
generate_function_params__CIGenerator(const Vec *params);

static void
generate_function_binary_expr__CIGenerator(const CIExprBinary *binary);

static void
generate_function_unary_expr__CIGenerator(const CIExprUnary *unary);

static void
generate_function_literal_expr__CIGenerator(const CIExprLiteral *literal);

static void
generate_function_expr__CIGenerator(const CIExpr *expr);

static void
generate_function_do_while_stmt__CIGenerator(const CIStmtDoWhile *do_while);

static void
generate_function_if_stmt_branch__CIGenerator(const CIStmtIfBranch *if_branch);

static void
generate_function_if_stmt__CIGenerator(const CIStmtIf *if_);

static void
generate_function_for_stmt__CIGenerator(const CIStmtFor *for_);

static void
generate_function_while_stmt__CIGenerator(const CIStmtWhile *while_);

static void
generate_function_switch_stmt__CIGenerator(const CIStmtSwitch *switch_);

static void
generate_function_stmt__CIGenerator(const CIStmt *stmt);

static void
generate_function_body_item__CIGenerator(const CIDeclFunctionItem *item);

static void
generate_function_body__CIGenerator(const Vec *body);

static void
generate_function_decl__CIGenerator(const CIDeclFunction *function);

static void
generate_struct_field__CIGenerator(const CIDeclStructField *field);

static void
generate_struct_fields__CIGenerator(const Vec *fields);

static void
generate_struct_decl__CIGenerator(const CIDeclStruct *struct_);

static void
generate_union_decl__CIGenerator(const CIDeclUnion *union_);

static void
generate_variable_decl__CIGenerator(const CIDeclVariable *variable);

static void
generate_decl__CIGenerator(const CIDecl *decl);

// NOTE: If the program is multi-threaded, you'll need to adapt these variables
// to multi-threading.
static String *current_result_content = NULL;
static Usize tab_count = 0;

void
run__CIGenerator(const CIResult *result)
{
    HashMapIter iter_headers = NEW(HashMapIter, result->headers);
    HashMapIter iter_sources = NEW(HashMapIter, result->sources);
    CIResultFile *current_header = NULL;
    CIResultFile *current_source = NULL;

    while ((current_header = next__HashMapIter(&iter_headers))) {
        run_file__CIGenerator(current_header);
    }

    while ((current_source = next__HashMapIter(&iter_sources))) {
        run_file__CIGenerator(current_source);
    }
}

void
write_str__CIGenerator(char *s)
{
    push_str__String(current_result_content, s);
}

void
write_String__CIGenerator(String *s)
{
    push_str__String(current_result_content, s->buffer);
    FREE(String, s);
}

void
inc_tab_count__CIGenerator()
{
    ++tab_count;
}

void
dec_tab_count__CIGenerator()
{
    --tab_count;
}

void
write_tab__CIGenerator()
{
    write_String__CIGenerator(repeat__String("\t", tab_count));
}

void
generate_storage_class__CIGenerator(const int *storage_class_flag)
{
    write_str__CIGenerator(to_string__CIStorageClass(*storage_class_flag));

    if (*storage_class_flag != CI_STORAGE_CLASS_NONE) {
        write_str__CIGenerator(" ");
    }
}

static void
serialize_data_type__CIGenerator(const CIDataType *data_type)
{
#define SERIALIZE_NAME(name, name_len) hash_sip(name, name_len, SIP_K0, SIP_K1)

#define SERIALIZE_TYPE_WITH_GENERIC_PARAMS(ty)                                \
    write_String__CIGenerator(                                                \
      format__String("{zu}", SERIALIZE_NAME(ty.name->buffer, ty.name->len))); \
                                                                              \
    if (ty.generic_params) {                                                  \
        serialize_generic_params__CIGenerator(ty.generic_params);             \
    }

    write_String__CIGenerator(format__String("{d}", data_type->kind));

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            serialize_data_type__CIGenerator(data_type->array.data_type);

            break;
        case CI_DATA_TYPE_KIND__ATOMIC:
            serialize_data_type__CIGenerator(data_type->_atomic);

            break;
        case CI_DATA_TYPE_KIND_ENUM:
            write_String__CIGenerator(format__String(
              "{zu}",
              SERIALIZE_NAME(data_type->enum_->buffer, data_type->enum_->len)));

            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            TODO("function");
        case CI_DATA_TYPE_KIND_PRE_CONST:
            serialize_data_type__CIGenerator(data_type->pre_const);

            break;
        case CI_DATA_TYPE_KIND_POST_CONST:
            serialize_data_type__CIGenerator(data_type->post_const);

            break;
        case CI_DATA_TYPE_KIND_PTR:
            serialize_data_type__CIGenerator(data_type->ptr);

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            SERIALIZE_TYPE_WITH_GENERIC_PARAMS(data_type->struct_);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            SERIALIZE_TYPE_WITH_GENERIC_PARAMS(data_type->union_);

            break;
        case CI_DATA_TYPE_KIND_BOOL:
        case CI_DATA_TYPE_KIND_CHAR:
        case CI_DATA_TYPE_KIND_DOUBLE:
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
        case CI_DATA_TYPE_KIND__DECIMAL32:
        case CI_DATA_TYPE_KIND__DECIMAL64:
        case CI_DATA_TYPE_KIND__DECIMAL128:
        case CI_DATA_TYPE_KIND_FLOAT:
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
        case CI_DATA_TYPE_KIND_INT:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
        case CI_DATA_TYPE_KIND_LONG_INT:
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_SHORT_INT:
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
        case CI_DATA_TYPE_KIND_VOID:
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

static inline void
serialize_generic_param__CIGenerator(const CIDataType *data_type)
{
    return serialize_data_type__CIGenerator(data_type);
}

static void
serialize_generic_params__CIGenerator(const Vec *generic_params)
{
    for (Usize i = 0; i < generic_params->len; ++i) {
        serialize_generic_param__CIGenerator(get__Vec(generic_params, i));
    }
}

void
generate_data_type__CIGenerator(const CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            generate_data_type__CIGenerator(data_type->array.data_type);

            switch (data_type->array.kind) {
                case CI_DATA_TYPE_ARRAY_KIND_NONE:
                    write_String__CIGenerator(format__String(
                      "{s}[]",
                      data_type->array.name ? data_type->array.name->buffer
                                            : ""));

                    break;
                case CI_DATA_TYPE_ARRAY_KIND_SIZED:
                    write_String__CIGenerator(format__String(
                      "{s}[{zu}]",
                      data_type->array.name ? data_type->array.name->buffer
                                            : "",
                      data_type->array.size));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        case CI_DATA_TYPE_KIND__ATOMIC:
            write_str__CIGenerator("_Atomic ");
            generate_data_type__CIGenerator(data_type->_atomic);

            break;
        case CI_DATA_TYPE_KIND_BOOL:
            write_str__CIGenerator("bool");

            break;
        case CI_DATA_TYPE_KIND_CHAR:
            write_str__CIGenerator("char");

            break;
        case CI_DATA_TYPE_KIND_DOUBLE:
            write_str__CIGenerator("double");

            break;
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
            write_str__CIGenerator("double _Complex");

            break;
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
            write_str__CIGenerator("double _Imaginary");

            break;
        case CI_DATA_TYPE_KIND__DECIMAL32:
            write_str__CIGenerator("_Decimal32");

            break;
        case CI_DATA_TYPE_KIND__DECIMAL64:
            write_str__CIGenerator("_Decimal64");

            break;
        case CI_DATA_TYPE_KIND__DECIMAL128:
            write_str__CIGenerator("_Decimal128");

            break;
        case CI_DATA_TYPE_KIND_ENUM:
            write_String__CIGenerator(
              format__String("enum {S}", data_type->enum_));

            break;
        case CI_DATA_TYPE_KIND_FLOAT:
            write_str__CIGenerator("float");

            break;
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
            write_str__CIGenerator("float _Complex");

            break;
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
            write_str__CIGenerator("float _Imaginary");

            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            TODO("function");
        case CI_DATA_TYPE_KIND_INT:
            write_str__CIGenerator("int");

            break;
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
            write_str__CIGenerator("long double");

            break;
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
            write_str__CIGenerator("long double _Complex");

            break;
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            write_str__CIGenerator("long double _Imaginary");

            break;
        case CI_DATA_TYPE_KIND_LONG_INT:
            write_str__CIGenerator("long int");

            break;
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
            write_str__CIGenerator("long long int");

            break;
        case CI_DATA_TYPE_KIND_PRE_CONST:
            write_str__CIGenerator("const ");
            generate_data_type__CIGenerator(data_type->pre_const);

            break;
        case CI_DATA_TYPE_KIND_POST_CONST:
            generate_data_type__CIGenerator(data_type->post_const);
            write_str__CIGenerator(" const");

            break;
        case CI_DATA_TYPE_KIND_PTR:
            generate_data_type__CIGenerator(data_type->ptr);
            write_str__CIGenerator("*");

            break;
        case CI_DATA_TYPE_KIND_SHORT_INT:
            write_str__CIGenerator("short int");

            break;
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            write_str__CIGenerator("signed char");

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            write_String__CIGenerator(
              format__String("struct {S}__", data_type->struct_.name));

            if (data_type->struct_.generic_params) {
                serialize_generic_params__CIGenerator(
                  data_type->struct_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
            write_str__CIGenerator("unsigned int");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
            write_str__CIGenerator("unsigned char");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
            write_str__CIGenerator("unsigned long int");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
            write_str__CIGenerator("unsigned long long int");

            break;
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            write_str__CIGenerator("unsigned short int");

            break;
        case CI_DATA_TYPE_KIND_UNION:
            write_String__CIGenerator(
              format__String("union {S}__", data_type->union_.name));

            if (data_type->union_.generic_params) {
                serialize_generic_params__CIGenerator(
                  data_type->union_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_VOID:
            write_str__CIGenerator("void");

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_enum_variant__CIGenerator(const CIDeclEnumVariant *enum_variant)
{
    switch (enum_variant->kind) {
        case CI_DECL_ENUM_VARIANT_KIND_CUSTOM:
            write_String__CIGenerator(format__String(
              "{S} = {zi}", enum_variant->name, enum_variant->value));

            break;
        case CI_DECL_ENUM_VARIANT_KIND_DEFAULT:
            write_str__CIGenerator(enum_variant->name->buffer);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_enum_variants__CIGenerator(const Vec *enum_variants)
{
    for (Usize i = 0; i < enum_variants->len; ++i) {
        generate_enum_variant__CIGenerator(get__Vec(enum_variants, i));
        write_str__CIGenerator(",\n");
    }
}

void
generate_enum_decl__CIGenerator(const CIDeclEnum *enum_)
{
    write_String__CIGenerator(format__String("enum {S} {{\n", enum_->name));
    generate_enum_variants__CIGenerator(enum_->variants);
    write_str__CIGenerator("}");
}

void
generate_function_params__CIGenerator(const Vec *params)
{
    write_str__CIGenerator("(");

    if (params) {
        for (Usize i = 0; i < params->len; ++i) {
            const CIDeclFunctionParam *param = get__Vec(params, i);

            generate_data_type__CIGenerator(param->data_type);
            write_String__CIGenerator(format__String(" {S}", param->name));

            if (i + 1 != params->len) {
                write_str__CIGenerator(", ");
            }
        }
    }

    write_str__CIGenerator(")");
}

void
generate_function_binary_expr__CIGenerator(const CIExprBinary *binary)
{
    generate_function_expr__CIGenerator(binary->left);

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

    write_String__CIGenerator(format__String(" {s} ", s_kind));
    generate_function_expr__CIGenerator(binary->right);
}

void
generate_function_unary_expr__CIGenerator(const CIExprUnary *unary)
{
    char *s_kind = NULL;

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            s_kind = "--";

            break;
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
            s_kind = "++";

            break;
        default:
            break;
    }

    if (s_kind) {
        generate_function_expr__CIGenerator(unary->expr);
        write_str__CIGenerator(s_kind);

        s_kind = NULL;
    }

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
            s_kind = "++";

            break;
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
            s_kind = "--";

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
        write_str__CIGenerator(s_kind);
        generate_function_expr__CIGenerator(unary->expr);
    }
}

void
generate_function_literal_expr__CIGenerator(const CIExprLiteral *literal)
{
    switch (literal->kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            if (literal->bool_) {
                write_str__CIGenerator("true");
            } else {
                write_str__CIGenerator("false");
            }

            break;
        case CI_EXPR_LITERAL_KIND_CHAR:
            write_String__CIGenerator(
              format__String("\'{c}\'", literal->char_));

            break;
        case CI_EXPR_LITERAL_KIND_FLOAT:
            write_String__CIGenerator(format__String("{f}", literal->float_));

            break;
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            write_String__CIGenerator(
              format__String("{zi}", literal->signed_int));

            break;
        case CI_EXPR_LITERAL_KIND_STRING:
            write_String__CIGenerator(
              format__String("\"{S}\"", literal->string));

            break;
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            write_String__CIGenerator(
              format__String("{zu}", literal->unsigned_int));

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_expr__CIGenerator(const CIExpr *expr)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            write_str__CIGenerator("alignof(");
            generate_function_expr__CIGenerator(expr->alignof_);
            write_str__CIGenerator(")");

            break;
        case CI_EXPR_KIND_BINARY:
            generate_function_binary_expr__CIGenerator(&expr->binary);

            break;
        case CI_EXPR_KIND_CAST:
            write_str__CIGenerator("(");
            generate_data_type__CIGenerator(expr->cast.data_type);
            write_str__CIGenerator(")");
            generate_function_expr__CIGenerator(expr->cast.expr);

            break;
        case CI_EXPR_KIND_DATA_TYPE:
            generate_data_type__CIGenerator(expr->data_type);

            break;
        case CI_EXPR_KIND_IDENTIFIER:
            write_str__CIGenerator(expr->identifier->buffer);

            break;
        case CI_EXPR_KIND_LITERAL:
            generate_function_literal_expr__CIGenerator(&expr->literal);

            break;
        case CI_EXPR_KIND_SIZEOF:
            write_str__CIGenerator("sizeof(");
            generate_function_expr__CIGenerator(expr->sizeof_);
            write_str__CIGenerator(")");

            break;
        case CI_EXPR_KIND_TERNARY:
            generate_function_expr__CIGenerator(expr->ternary.cond);
            write_str__CIGenerator(" ? ");
            generate_function_expr__CIGenerator(expr->ternary.if_);
            write_str__CIGenerator(" : ");
            generate_function_expr__CIGenerator(expr->ternary.else_);

            break;
        case CI_EXPR_KIND_UNARY:
            generate_function_unary_expr__CIGenerator(&expr->unary);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_do_while_stmt__CIGenerator(const CIStmtDoWhile *do_while)
{
    write_str__CIGenerator("do");
    generate_function_body__CIGenerator(do_while->body);
    write_str__CIGenerator("while (");
    generate_function_expr__CIGenerator(do_while->cond);
    write_str__CIGenerator(");\n");
}

void
generate_function_if_stmt_branch__CIGenerator(const CIStmtIfBranch *if_branch)
{
    write_str__CIGenerator("(");
    generate_function_expr__CIGenerator(if_branch->cond);
    write_str__CIGenerator(")");
    generate_function_body__CIGenerator(if_branch->body);
}

void
generate_function_if_stmt__CIGenerator(const CIStmtIf *if_)
{
    write_str__CIGenerator("if");
    generate_function_if_stmt_branch__CIGenerator(if_->if_);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            write_str__CIGenerator("else if");
            generate_function_if_stmt_branch__CIGenerator(
              get__Vec(if_->else_ifs, i));
        }
    }

    if (if_->else_) {
        write_str__CIGenerator("else");
        generate_function_body__CIGenerator(if_->else_);
    }
}

void
generate_function_for_stmt__CIGenerator(const CIStmtFor *for_)
{
    write_str__CIGenerator("for (");

    if (for_->init_clause) {
        generate_function_body_item__CIGenerator(for_->init_clause);
    }

    write_str__CIGenerator(";");

    if (for_->expr1) {
        generate_function_expr__CIGenerator(for_->expr1);
    }

    write_str__CIGenerator(";");

    if (for_->exprs2) {
        for (Usize i = 0; i < for_->exprs2->len; ++i) {
            generate_function_expr__CIGenerator(get__Vec(for_->exprs2, i));

            if (i + 1 != for_->exprs2->len) {
                write_str__CIGenerator(", ");
            }
        }
    }

    write_str__CIGenerator(")");
    generate_function_body__CIGenerator(for_->body);
}

void
generate_function_while_stmt__CIGenerator(const CIStmtWhile *while_)
{
    write_str__CIGenerator("while (");
    generate_function_expr__CIGenerator(while_->cond);
    write_str__CIGenerator(")");
    generate_function_body__CIGenerator(while_->body);
}

void
generate_function_switch_stmt__CIGenerator(const CIStmtSwitch *switch_)
{
    write_str__CIGenerator("switch (");
    generate_function_expr__CIGenerator(switch_->expr);
    write_str__CIGenerator(")");
    write_str__CIGenerator("{\n");
    inc_tab_count__CIGenerator();

    for (Usize i = 0; i < switch_->cases->len; ++i) {
        const CIStmtSwitchCase *case_ = get__Vec(switch_->cases, i);

        write_tab__CIGenerator();
        write_str__CIGenerator("case ");
        generate_function_expr__CIGenerator(case_->value);
        write_str__CIGenerator(":\n");
        generate_function_body__CIGenerator(case_->body);
    }

    write_tab__CIGenerator();
    write_str__CIGenerator("default:\n");
    generate_function_body__CIGenerator(switch_->default_case);

    dec_tab_count__CIGenerator();
    write_str__CIGenerator("}");
}

void
generate_function_stmt__CIGenerator(const CIStmt *stmt)
{
    switch (stmt->kind) {
        case CI_STMT_KIND_BREAK:
            write_str__CIGenerator("break;");

            break;
        case CI_STMT_KIND_CONTINUE:
            write_str__CIGenerator("continue;");

            break;
        case CI_STMT_KIND_DO_WHILE:
            generate_function_do_while_stmt__CIGenerator(&stmt->do_while);

            break;
        case CI_STMT_KIND_FOR:
            generate_function_for_stmt__CIGenerator(&stmt->for_);

            break;
        case CI_STMT_KIND_GOTO:
            write_String__CIGenerator(format__String("goto {S};", stmt->goto_));

            break;
        case CI_STMT_KIND_IF:
            generate_function_if_stmt__CIGenerator(&stmt->if_);

            break;
        case CI_STMT_KIND_RETURN:
            write_str__CIGenerator("return ");
            generate_function_expr__CIGenerator(stmt->return_);
            write_str__CIGenerator(";");

            break;
        case CI_STMT_KIND_SWITCH:
            generate_function_switch_stmt__CIGenerator(&stmt->switch_);

            break;
        case CI_STMT_KIND_WHILE:
            generate_function_while_stmt__CIGenerator(&stmt->while_);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_body_item__CIGenerator(const CIDeclFunctionItem *item)
{
    switch (item->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            generate_decl__CIGenerator(item->decl);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            generate_function_expr__CIGenerator(item->expr);
            write_str__CIGenerator(";\n");

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            generate_function_stmt__CIGenerator(&item->stmt);
            write_str__CIGenerator("\n");

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_function_body__CIGenerator(const Vec *body)
{
    write_str__CIGenerator(" {\n");
    inc_tab_count__CIGenerator();

    for (Usize i = 0; i < body->len; ++i) {
        write_tab__CIGenerator();
        generate_function_body_item__CIGenerator(get__Vec(body, i));
    }

    dec_tab_count__CIGenerator();
    write_str__CIGenerator("}\n");
}

void
generate_function_decl__CIGenerator(const CIDeclFunction *function)
{
    generate_data_type__CIGenerator(function->return_data_type);
    write_String__CIGenerator(format__String(" {S}", function->name));
    generate_function_params__CIGenerator(function->params);
    generate_function_body__CIGenerator(function->body);
}

void
generate_struct_field__CIGenerator(const CIDeclStructField *field)
{
    generate_data_type__CIGenerator(field->data_type);
    write_String__CIGenerator(format__String(" {S};\n", field->name));
}

void
generate_struct_fields__CIGenerator(const Vec *fields)
{
    for (Usize i = 0; i < fields->len; ++i) {
        generate_struct_field__CIGenerator(get__Vec(fields, i));
    }
}

void
generate_struct_decl__CIGenerator(const CIDeclStruct *struct_)
{
    write_String__CIGenerator(format__String("struct {S} {{\n", struct_->name));
    generate_struct_fields__CIGenerator(struct_->fields);
    write_str__CIGenerator("}");
}

void
generate_union_decl__CIGenerator(const CIDeclUnion *union_)
{
    write_String__CIGenerator(format__String("union {S} {{\n", union_->name));
    generate_struct_fields__CIGenerator(union_->fields);
    write_str__CIGenerator("}");
}

void
generate_variable_decl__CIGenerator(const CIDeclVariable *variable)
{
    generate_data_type__CIGenerator(variable->data_type);
    write_String__CIGenerator(format__String(" {S}", variable->name));

    if (variable->expr) {
        write_str__CIGenerator(" = ");
        generate_function_expr__CIGenerator(variable->expr);
    }

    write_str__CIGenerator(";\n\n");
}

void
generate_decl__CIGenerator(const CIDecl *decl)
{
    generate_storage_class__CIGenerator(&decl->storage_class_flag);

    switch (decl->kind) {
        case CI_DECL_KIND_ENUM:
            generate_enum_decl__CIGenerator(&decl->enum_);

            break;
        case CI_DECL_KIND_FUNCTION:
            return generate_function_decl__CIGenerator(&decl->function);
        case CI_DECL_KIND_STRUCT:
            generate_struct_decl__CIGenerator(&decl->struct_);

            break;
        case CI_DECL_KIND_UNION:
            generate_union_decl__CIGenerator(&decl->union_);

            break;
        case CI_DECL_KIND_VARIABLE:
            return generate_variable_decl__CIGenerator(&decl->variable);
        default:
            UNREACHABLE("unknown variant");
    }

    if (decl->storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
        ASSERT(decl->typedef_name);

        write_String__CIGenerator(
          format__String("{S};\n\n", get_typedef_name__CIDecl(decl)));
    } else {
        write_str__CIGenerator(";\n\n");
    }
}

void
run_file__CIGenerator(const CIResultFile *file_result)
{
    current_result_content = NEW(String);
    // TODO: Add a possibly to the user to "create its custom output director"
    const char *output_dir = "out.ci";
    String *dir_result = format__String(
      "{s}/{Sr}", output_dir, get_dir__File(file_result->file_input.name));
    String *path_result =
      format__String("{S}/{S}", dir_result, file_result->filename_result);
    OrderedHashMapIter iter_decls = NEW(OrderedHashMapIter, file_result->decls);
    CIDecl *decl = NULL;

    while ((decl = next__OrderedHashMapIter(&iter_decls))) {
        generate_decl__CIGenerator(decl);
    }

    create_recursive_dir__Dir(dir_result->buffer,
                              DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    write_file__File(path_result->buffer, current_result_content->buffer);

    FREE(String, current_result_content);
    FREE(String, dir_result);
    FREE(String, path_result);
}
