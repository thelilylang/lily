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

static inline void
set_result_ref__CIGenerator(const CIResult *result);

static inline void
unset_result_ref__CIGenerator();

/// @return CIDataType*? (&)
static CIDataType *
substitute_generic__CIGenerator(const String *generic_name);

/// @return CIDataType*?
static CIDataType *
substitute_data_type__CIGenerator(CIDataType *data_type);

static void
run_file__CIGenerator(const CIResultFile *file_result);

static void
write__CIGenerator(char c);

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

/// @param unresolved_generic_params CIGenericParams* (&)
/// @param name const String* (&)
/// @return String*
static String *
substitute_and_serialize_generic_params__CIGenerator(
  CIGenericParams *unresolved_generic_params,
  const String *name);

static void
generate_storage_class__CIGenerator(const int *storage_class_flag);

static void
generate_attribute_standard__CIGenerator(
  const CIAttributeStandard *attribute_standard);

static void
generate_attribute__CIGenerator(const CIAttribute *attribute);

static void
generate_attributes__CIGenerator(const Vec *attributes);

static void
generate_attributes_by_decl__CIGenerator(const CIDecl *decl);

static void
generate_data_type_qualifier__CIGenerator(const int *data_type_qualifier);

static void
generate_data_type__CIGenerator(CIDataType *data_type);

static void
generate_enum_decl__CIGenerator(const CIDeclEnum *enum_);

static void
generate_enum_variant__CIGenerator(const CIDeclEnumVariant *enum_variant);

static void
generate_enum_variants__CIGenerator(const Vec *enum_variants);

static void
generate_function_params__CIGenerator(const Vec *params);

static void
generate_function_array_expr__CIGenerator(const CIExprArray *array);

static void
generate_function_binary_expr__CIGenerator(const CIExprBinary *binary);

static void
generate_function_unary_expr__CIGenerator(const CIExprUnary *unary);

static void
generate_function_call_params_expr__CIGenerator(Vec *params);

static void
generate_function_call_expr__CIGenerator(
  const CIExprFunctionCall *function_call);

static void
generate_function_call_builtin_expr__CIGenerator(
  const CIExprFunctionCallBuiltin *function_call_builtin);

static void
generate_function_literal_character_expr__CIGenerator(const char c);

static void
generate_function_literal_string_expr__CIGenerator(const String *string);

static void
generate_function_literal_expr__CIGenerator(const CIExprLiteral *literal);

static void
generate_struct_call_expr__CIGenerator(const CIExprStructCall *struct_call);

static void
generate_function_expr__CIGenerator(const CIExpr *expr);

static inline void
generate_function_block_stmt__CIGenerator(const CIStmtBlock *block);

static void
generate_case_stmt__CIGenerator(const CIStmtSwitchCase *case_);

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
generate_function_body__CIGenerator(const CIDeclFunctionBody *body);

static void
generate_function_decl__CIGenerator(const CIDeclFunction *function);

static void
generate_function_gen_decl__CIGenerator(const CIDeclFunctionGen *function_gen);

static void
generate_label_decl__CIGenerator(const CIDeclLabel *label);

static void
generate_struct_field__CIGenerator(const CIDeclStructField *field);

static void
generate_struct_fields__CIGenerator(const Vec *fields);

static void
generate_struct_decl__CIGenerator(const CIDeclStruct *struct_);

static void
generate_struct_gen_decl__CIGenerator(const CIDeclStructGen *struct_gen);

static void
generate_typedef_decl__CIGenerator(const CIDeclTypedef *typedef_);

static void
generate_typedef_gen_decl__CIGenerator(const CIDeclTypedefGen *typedef_gen);

static void
generate_union_decl__CIGenerator(const CIDeclUnion *union_);

static void
generate_union_gen_decl__CIGenerator(const CIDeclUnionGen *union_gen);

static void
generate_variable_decl__CIGenerator(const CIDeclVariable *variable);

static void
generate_decl__CIGenerator(const CIDecl *decl);

static void
generate_global_decls__CIGenerator(const CIResultFile *file_result);

static void
generate_enum_prototype__CIGenerator(const CIDeclEnum *enum_);

static void
generate_struct_prototype__CIGenerator(const CIDeclStruct *struct_);

static void
generate_struct_gen_prototype__CIGenerator(const CIDeclStructGen *struct_gen);

static void
generate_union_prototype__CIGenerator(const CIDeclUnion *union_);

static void
generate_union_gen_prototype__CIGenerator(const CIDeclUnionGen *union_gen);

static void
generate_function_prototype__CIGenerator(const CIDeclFunction *function);

static void
generate_function_gen_prototype__CIGenerator(
  const CIDeclFunctionGen *function_gen);

static const CIResult *result_ref = NULL; // CIResult* (&)

// NOTE: If the program is multi-threaded, you'll need to adapt these variables
// to multi-threading.
static String *current_result_content = NULL;
static Usize tab_count = 0;

static CIGenericParams *current_generic_params = NULL;
static CIGenericParams *current_called_generic_params = NULL;

static enum CIStandard current_standard = CI_STANDARD_NONE;

#define SET_CURRENT_GENERIC_PARAMS(cgp, ccgp) \
    current_generic_params = cgp;             \
    current_called_generic_params = ccgp;

#define RESET_CURRENT_GENERIC_PARAMS() \
    current_generic_params = NULL;     \
    current_called_generic_params = NULL;

// This variable can be used to disable the writing of ';' on file contents, for
// example when generating an expression or variable declaration.
static bool write_semicolon = true;

#define SET_WRITE_SEMICOLON() write_semicolon = true;
#define UNSET_WRITE_SEMICOLON() write_semicolon = false;

void
set_result_ref__CIGenerator(const CIResult *result)
{
    result_ref = result;
}

void
unset_result_ref__CIGenerator()
{
    result_ref = NULL;
}

CIDataType *
substitute_generic__CIGenerator(const String *generic_name)
{
    return substitute_generic__CIParser(
      generic_name, current_generic_params, current_called_generic_params);
}

CIDataType *
substitute_data_type__CIGenerator(CIDataType *data_type)
{
    return substitute_data_type__CIParser(
      data_type, current_generic_params, current_called_generic_params);
}

void
run__CIGenerator(const CIResult *result)
{
    OrderedHashMapIter iter_libs = NEW(OrderedHashMapIter, result->libs);
    OrderedHashMapIter iter_bins = NEW(OrderedHashMapIter, result->bins);
    CIResultLib *current_lib = NULL;
    CIResultBin *current_bin = NULL;

    set_result_ref__CIGenerator(result);

    while ((current_lib = next__OrderedHashMapIter(&iter_libs))) {
        run_file__CIGenerator(current_lib->file);
    }

    while ((current_bin = next__OrderedHashMapIter(&iter_bins))) {
        run_file__CIGenerator(current_bin->file);
    }

    unset_result_ref__CIGenerator();
}

void
write__CIGenerator(char c)
{
    push__String(current_result_content, c);
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

String *
substitute_and_serialize_generic_params__CIGenerator(
  CIGenericParams *unresolved_generic_params,
  const String *name)
{
    ASSERT(unresolved_generic_params);
    ASSERT(name);

    if (has_generic__CIGenericParams(unresolved_generic_params)) {
        ASSERT(current_generic_params);
        ASSERT(current_called_generic_params);

        CIGenericParams *resolved_generic_params =
          substitute_generic_params__CIParser(unresolved_generic_params,
                                              current_generic_params,
                                              current_called_generic_params);

        ASSERT(resolved_generic_params);

        String *serialized_name =
          serialize_name__CIGenericParams(resolved_generic_params, name);

        FREE(CIGenericParams, resolved_generic_params);

        return serialized_name;
    }

    return serialize_name__CIGenericParams(unresolved_generic_params, name);
}

void
generate_storage_class__CIGenerator(const int *storage_class_flag)
{
    write_String__CIGenerator(
      to_string__CIStorageClass(*storage_class_flag, current_standard));

    if (*storage_class_flag != CI_STORAGE_CLASS_NONE) {
        write__CIGenerator(' ');
    }
}

void
generate_attribute_standard__CIGenerator(
  const CIAttributeStandard *attribute_standard)
{
    write_str__CIGenerator("[[");

    switch (attribute_standard->kind) {
        case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
            write_String__CIGenerator(format__String(
              "deprecated({S})",
              GET_PTR_RC(String, attribute_standard->deprecated)));

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH:
            write_str__CIGenerator("fallthrough");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED:
            write_str__CIGenerator("maybe_unused");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
            write_String__CIGenerator(format__String(
              "nodiscard({S})",
              GET_PTR_RC(String, attribute_standard->nodiscard)));

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_NORETURN:
            write_str__CIGenerator("noreturn");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED:
            write_str__CIGenerator("unsequenced");

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE:
            write_str__CIGenerator("reproducible");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    write_str__CIGenerator("]]");
}

void
generate_attribute__CIGenerator(const CIAttribute *attribute)
{
    switch (attribute->kind) {
        case CI_ATTRIBUTE_KIND_CLANG:
            TODO("generate Clang attribute");
        case CI_ATTRIBUTE_KIND_GNU:
            TODO("generate GNU attribute");
        case CI_ATTRIBUTE_KIND_STANDARD:
            generate_attribute_standard__CIGenerator(&attribute->standard);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
generate_attributes__CIGenerator(const Vec *attributes)
{
    if (attributes) {
        for (Usize i = 0; i < attributes->len; ++i) {
            generate_attribute__CIGenerator(get__Vec(attributes, i));
            write_str__CIGenerator(" ");
        }
    }
}

void
generate_attributes_by_decl__CIGenerator(const CIDecl *decl)
{
    switch (decl->kind) {
        case CI_DECL_KIND_FUNCTION:
            generate_attributes__CIGenerator(decl->function.attributes);

            break;
        default:
            break;
    }
}

void
generate_data_type_qualifier__CIGenerator(const int *data_type_qualifier)
{
    write_String__CIGenerator(
      to_string__CIDataTypeQualifier(*data_type_qualifier));
}

void
generate_data_type__CIGenerator(CIDataType *data_type)
{
    CIDataType *subs_data_type = NULL;

    if (current_generic_params && current_called_generic_params) {
        subs_data_type = substitute_data_type__CIGenerator(data_type);
    } else {
        subs_data_type = ref__CIDataType(data_type);
    }

    switch (subs_data_type->kind) {
        case CI_DATA_TYPE_KIND_ANY:
            UNREACHABLE("cannot generate this data type");
        case CI_DATA_TYPE_KIND_ARRAY:
            generate_data_type__CIGenerator(subs_data_type->array.data_type);

            switch (subs_data_type->array.kind) {
                case CI_DATA_TYPE_ARRAY_KIND_NONE:
                    write_String__CIGenerator(format__String(
                      " {s}[]",
                      subs_data_type->array.name
                        ? GET_PTR_RC(String, subs_data_type->array.name)->buffer
                        : ""));

                    break;
                case CI_DATA_TYPE_ARRAY_KIND_SIZED:
                    write_String__CIGenerator(format__String(
                      " {s}[{zu}]",
                      subs_data_type->array.name
                        ? GET_PTR_RC(String, subs_data_type->array.name)->buffer
                        : "",
                      subs_data_type->array.size));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        case CI_DATA_TYPE_KIND_BOOL:
            if (current_standard < CI_STANDARD_23) {
                write_str__CIGenerator("_Bool");
            } else {
                write_str__CIGenerator("bool");
            }

            break;
        case CI_DATA_TYPE_KIND_BUILTIN: {
            const CIBuiltinType *builtin_type = get_builtin_type__CIBuiltin(
              result_ref->builtin, subs_data_type->builtin);

            ASSERT(builtin_type);

            write_str__CIGenerator((char *)builtin_type->name->buffer);

            break;
        }
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
            write_String__CIGenerator(format__String(
              "enum {s}",
              subs_data_type->enum_
                ? GET_PTR_RC(String, subs_data_type->enum_)->buffer
                : ""));

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
        case CI_DATA_TYPE_KIND_FUNCTION: {
            generate_data_type__CIGenerator(
              subs_data_type->function.return_data_type);

            if (subs_data_type->function.function_data_type) {
                write_str__CIGenerator("(");

                generate_data_type__CIGenerator(
                  subs_data_type->function.function_data_type);

                if (subs_data_type->function.name) {
                    write_str__CIGenerator(
                      GET_PTR_RC(String, subs_data_type->function.name)
                        ->buffer);
                }

                write_str__CIGenerator(")");
            } else {
                write__CIGenerator(' ');

                if (subs_data_type->function.name) {
                    write_str__CIGenerator(
                      GET_PTR_RC(String, subs_data_type->function.name)
                        ->buffer);
                }
            }

            if (subs_data_type->function.params) {
                generate_function_params__CIGenerator(
                  subs_data_type->function.params);
            } else {
                write_str__CIGenerator("()");
            }

            break;
        }
        case CI_DATA_TYPE_KIND_GENERIC: {
            String *buffer = NEW(String);

            serialize__CIDataType(subs_data_type, buffer);
            write_String__CIGenerator(buffer);

            break;
        }
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
        case CI_DATA_TYPE_KIND_PTR:
            if (subs_data_type->ptr) {
                generate_data_type__CIGenerator(subs_data_type->ptr);
            }

            write_str__CIGenerator("*");

            break;
        case CI_DATA_TYPE_KIND_SHORT_INT:
            write_str__CIGenerator("short int");

            break;
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            write_str__CIGenerator("signed char");

            break;
        case CI_DATA_TYPE_KIND_STRUCT: {
#define GENERATE_STRUCT_OR_UNION_DT(dt_name, s)                              \
    write_str__CIGenerator(s);                                               \
                                                                             \
    if (subs_data_type->dt_name.generic_params) {                            \
        /* NOTE: Normally you can't declare a anonymous struct/union with    \
        generic parameters. This possibility is rejected in the              \
        parser. */                                                           \
        ASSERT(subs_data_type->dt_name.name);                                \
                                                                             \
        String *serialized_name =                                            \
          substitute_and_serialize_generic_params__CIGenerator(              \
            subs_data_type->dt_name.generic_params,                          \
            GET_PTR_RC(String, subs_data_type->dt_name.name));               \
                                                                             \
        write__CIGenerator(' ');                                             \
        write_String__CIGenerator(serialized_name);                          \
    } else if (subs_data_type->dt_name.name) {                               \
        write__CIGenerator(' ');                                             \
        write_str__CIGenerator(                                              \
          GET_PTR_RC(String, subs_data_type->dt_name.name)->buffer);         \
    }                                                                        \
                                                                             \
    if (subs_data_type->dt_name.fields) {                                    \
        write_str__CIGenerator(" {\n");                                      \
        generate_struct_fields__CIGenerator(subs_data_type->dt_name.fields); \
        write_tab__CIGenerator();                                            \
        write_str__CIGenerator("}");                                         \
    }

            GENERATE_STRUCT_OR_UNION_DT(struct_, "struct");

            break;
        }
        case CI_DATA_TYPE_KIND_TYPEDEF:
            if (subs_data_type->typedef_.generic_params) {
                String *serialized_name =
                  substitute_and_serialize_generic_params__CIGenerator(
                    subs_data_type->typedef_.generic_params,
                    GET_PTR_RC(String, subs_data_type->typedef_.name));

                write_String__CIGenerator(serialized_name);
            } else {
                write_str__CIGenerator(
                  GET_PTR_RC(String, subs_data_type->typedef_.name)->buffer);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPE_INFO:
            UNREACHABLE("cannot generate this data type");
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
        case CI_DATA_TYPE_KIND_UNION: {
            GENERATE_STRUCT_OR_UNION_DT(union_, "union");

            break;
        }
        case CI_DATA_TYPE_KIND_VOID:
            write_str__CIGenerator("void");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    if (data_type->qualifier != CI_DATA_TYPE_QUALIFIER_NONE) {
        write__CIGenerator(' ');
        generate_data_type_qualifier__CIGenerator(&data_type->qualifier);
    }

    FREE(CIDataType, subs_data_type);
}

void
generate_enum_variant__CIGenerator(const CIDeclEnumVariant *enum_variant)
{
    write_String__CIGenerator(
      format__String("{S} = {zi},\n",
                     GET_PTR_RC(String, enum_variant->name),
                     enum_variant->value));
}

void
generate_enum_variants__CIGenerator(const Vec *enum_variants)
{
    inc_tab_count__CIGenerator();

    for (Usize i = 0; i < enum_variants->len; ++i) {
        write_tab__CIGenerator();
        generate_enum_variant__CIGenerator(get__Vec(enum_variants, i));
    }

    dec_tab_count__CIGenerator();
}

void
generate_enum_decl__CIGenerator(const CIDeclEnum *enum_)
{
    write_String__CIGenerator(
      format__String("enum {S}", GET_PTR_RC(String, enum_->name)));

    if (enum_->data_type) {
        write_str__CIGenerator(" : ");
        generate_data_type__CIGenerator(enum_->data_type);
    }

    write_str__CIGenerator(" {\n");

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

            switch (param->kind) {
                case CI_DECL_FUNCTION_PARAM_KIND_NORMAL:
                    ASSERT(param->data_type);

                    generate_data_type__CIGenerator(param->data_type);

                    if (param->name &&
                        !has_name__CIDataType(param->data_type)) {
                        write_String__CIGenerator(format__String(
                          " {S}", GET_PTR_RC(String, param->name)));
                    }

                    break;
                case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
                    write_str__CIGenerator("...");

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            if (i + 1 != params->len) {
                write_str__CIGenerator(", ");
            }
        }
    }

    write_str__CIGenerator(")");
}

void
generate_function_array_expr__CIGenerator(const CIExprArray *array)
{
    write_str__CIGenerator("{");

    for (Usize i = 0; i < array->array->len; ++i) {
        generate_function_expr__CIGenerator(get__Vec(array->array, i));
        write_str__CIGenerator(",");
    }

    write_str__CIGenerator("}");
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

    switch (binary->kind) {
        // Glue the operator to the expression on the left and right.
        case CI_EXPR_BINARY_KIND_ARROW:
        case CI_EXPR_BINARY_KIND_DOT:
            write_str__CIGenerator(s_kind);

            break;
        default:
            write_String__CIGenerator(format__String(" {s} ", s_kind));
    }

    generate_function_expr__CIGenerator(binary->right);
}

void
generate_function_unary_expr__CIGenerator(const CIExprUnary *unary)
{
    char *s_kind = NULL;

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
            s_kind = "++";

            break;
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
            s_kind = "--";

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
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            s_kind = "--";

            break;
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
            s_kind = "++";

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
generate_function_literal_character_expr__CIGenerator(const char c)
{
    switch (c) {
        case '\"':
            write_str__CIGenerator("\\\"");
            break;
        case '\'':
            write_str__CIGenerator("\\'");
            break;
        case '\b':
            write_str__CIGenerator("\\b");
            break;
        case '\f':
            write_str__CIGenerator("\\f");
            break;
        case '\n':
            write_str__CIGenerator("\\n");
            break;
        case '\t':
            write_str__CIGenerator("\\t");
            break;
        case '\r':
            write_str__CIGenerator("\\r");
            break;
        case '\v':
            write_str__CIGenerator("\\v");
            break;
        case '\\':
            write_str__CIGenerator("\\");
            break;
        case '\0':
            write_str__CIGenerator("\\0");
            break;
        default:
            write__CIGenerator(c);
    }
}

void
generate_function_literal_string_expr__CIGenerator(const String *string)
{
    StringIter iter = NEW(StringIter, (String *)string);
    char current;

    while ((current = next__StringIter(&iter))) {
        generate_function_literal_character_expr__CIGenerator(current);
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
            write__CIGenerator('\'');
            generate_function_literal_character_expr__CIGenerator(
              literal->char_);
            write__CIGenerator('\'');

            break;
        case CI_EXPR_LITERAL_KIND_FLOAT:
            write_String__CIGenerator(format__String("{f}", literal->float_));

            break;
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            write_String__CIGenerator(
              format__String("{zi}", literal->signed_int));

            break;
        case CI_EXPR_LITERAL_KIND_STRING:
            write__CIGenerator('\"');
            generate_function_literal_string_expr__CIGenerator(
              GET_PTR_RC(String, literal->string));
            write__CIGenerator('\"');

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
generate_struct_call_expr__CIGenerator(const CIExprStructCall *struct_call)
{
    write_str__CIGenerator("{");

    for (Usize i = 0; i < struct_call->fields->len; ++i) {
        CIExprStructFieldCall *field = get__Vec(struct_call->fields, i);

        for (Usize j = 0; j < field->path->len; ++j) {
            write_String__CIGenerator(format__String(
              ".{S}",
              GET_PTR_RC(String, CAST(Rc *, get__Vec(field->path, j)))));
        }

        write_str__CIGenerator(" = ");
        generate_function_expr__CIGenerator(field->value);

        if (i + 1 != struct_call->fields->len) {
            write_str__CIGenerator(", ");
        }
    }

    write_str__CIGenerator("}");
}

void
generate_function_call_params_expr__CIGenerator(Vec *params)
{
    write_str__CIGenerator("(");

    for (Usize i = 0; i < params->len; ++i) {
        generate_function_expr__CIGenerator(get__Vec(params, i));

        if (i + 1 != params->len) {
            write_str__CIGenerator(", ");
        }
    }

    write_str__CIGenerator(")");
}

void
generate_function_call_expr__CIGenerator(
  const CIExprFunctionCall *function_call)
{
    if (function_call->generic_params) {
        write_String__CIGenerator(
          substitute_and_serialize_generic_params__CIGenerator(
            function_call->generic_params,
            GET_PTR_RC(String, function_call->identifier)));
    } else {
        write_str__CIGenerator(
          GET_PTR_RC(String, function_call->identifier)->buffer);
    }

    generate_function_call_params_expr__CIGenerator(function_call->params);
}

void
generate_function_call_builtin_expr__CIGenerator(
  const CIExprFunctionCallBuiltin *function_call_builtin)
{
    const CIBuiltinFunction *builtin_function = get_builtin_function__CIBuiltin(
      result_ref->builtin, function_call_builtin->id);

    ASSERT(builtin_function);

    write_str__CIGenerator((char *)builtin_function->name->buffer);
    generate_function_call_params_expr__CIGenerator(
      function_call_builtin->params);
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
        case CI_EXPR_KIND_ARRAY:
            generate_function_array_expr__CIGenerator(&expr->array);

            break;
        case CI_EXPR_KIND_ARRAY_ACCESS:
            generate_function_expr__CIGenerator(expr->array_access.array);
            write_str__CIGenerator("[");
            generate_function_expr__CIGenerator(expr->array_access.access);
            write_str__CIGenerator("]");

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
        case CI_EXPR_KIND_FUNCTION_CALL:
            generate_function_call_expr__CIGenerator(&expr->function_call);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            generate_function_call_builtin_expr__CIGenerator(
              &expr->function_call_builtin);

            break;
        case CI_EXPR_KIND_GROUPING:
            write_str__CIGenerator("(");
            generate_function_expr__CIGenerator(expr->grouping);
            write_str__CIGenerator(")");

            break;
        case CI_EXPR_KIND_IDENTIFIER:
            write_str__CIGenerator(
              GET_PTR_RC(String, expr->identifier)->buffer);

            break;
        case CI_EXPR_KIND_LITERAL:
            generate_function_literal_expr__CIGenerator(&expr->literal);

            break;
        case CI_EXPR_KIND_NULLPTR:
            write_str__CIGenerator("nullptr");

            break;
        case CI_EXPR_KIND_SIZEOF:
            write_str__CIGenerator("sizeof(");
            generate_function_expr__CIGenerator(expr->sizeof_);
            write_str__CIGenerator(")");

            break;
        case CI_EXPR_KIND_STRUCT_CALL:
            generate_struct_call_expr__CIGenerator(&expr->struct_call);

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
generate_function_block_stmt__CIGenerator(const CIStmtBlock *block)
{
    generate_function_body__CIGenerator(block->body);
}

void
generate_case_stmt__CIGenerator(const CIStmtSwitchCase *case_)
{
    write_str__CIGenerator("case ");
    generate_function_expr__CIGenerator(case_->value);
    write_str__CIGenerator(":");
}

void
generate_function_do_while_stmt__CIGenerator(const CIStmtDoWhile *do_while)
{
    write_str__CIGenerator("do ");
    generate_function_body__CIGenerator(do_while->body);
    write_str__CIGenerator("while (");
    generate_function_expr__CIGenerator(do_while->cond);
    write_str__CIGenerator(");\n");
}

void
generate_function_if_stmt_branch__CIGenerator(const CIStmtIfBranch *if_branch)
{
    write_str__CIGenerator(" (");
    generate_function_expr__CIGenerator(if_branch->cond);
    write_str__CIGenerator(") ");
    generate_function_body__CIGenerator(if_branch->body);
}

void
generate_function_if_stmt__CIGenerator(const CIStmtIf *if_)
{
    write_str__CIGenerator("if");
    generate_function_if_stmt_branch__CIGenerator(if_->if_);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            write_tab__CIGenerator();
            write_str__CIGenerator("else if");
            generate_function_if_stmt_branch__CIGenerator(
              get__Vec(if_->else_ifs, i));
        }
    }

    if (if_->else_) {
        write_tab__CIGenerator();
        write_str__CIGenerator("else ");
        generate_function_body__CIGenerator(if_->else_);
    }
}

void
generate_function_for_stmt__CIGenerator(const CIStmtFor *for_)
{
    write_str__CIGenerator("for (");

    if (for_->init_clauses) {
        UNSET_WRITE_SEMICOLON();

        for (Usize i = 0; i < for_->init_clauses->len; ++i) {
            generate_function_body_item__CIGenerator(
              get__Vec(for_->init_clauses, i));

            if (i + 1 != for_->init_clauses->len) {
                write__CIGenerator(',');
            }
        }

        SET_WRITE_SEMICOLON();
    }

    write__CIGenerator(';');

    if (for_->expr1) {
        generate_function_expr__CIGenerator(for_->expr1);
    }

    write__CIGenerator(';');

    if (for_->exprs2) {
        for (Usize i = 0; i < for_->exprs2->len; ++i) {
            generate_function_expr__CIGenerator(get__Vec(for_->exprs2, i));

            if (i + 1 != for_->exprs2->len) {
                write_str__CIGenerator(", ");
            }
        }
    }

    write_str__CIGenerator(") ");
    generate_function_body__CIGenerator(for_->body);
}

void
generate_function_while_stmt__CIGenerator(const CIStmtWhile *while_)
{
    write_str__CIGenerator("while (");
    generate_function_expr__CIGenerator(while_->cond);
    write_str__CIGenerator(") ");
    generate_function_body__CIGenerator(while_->body);
}

void
generate_function_switch_stmt__CIGenerator(const CIStmtSwitch *switch_)
{
    write_str__CIGenerator("switch (");
    generate_function_expr__CIGenerator(switch_->expr);
    write_str__CIGenerator(") ");
    generate_function_body__CIGenerator(switch_->body);
}

void
generate_function_stmt__CIGenerator(const CIStmt *stmt)
{
    switch (stmt->kind) {
        case CI_STMT_KIND_BLOCK:
            generate_function_block_stmt__CIGenerator(&stmt->block);

            break;
        case CI_STMT_KIND_BREAK:
            write_str__CIGenerator("break;");

            break;
        case CI_STMT_KIND_CASE:
            generate_case_stmt__CIGenerator(&stmt->case_);

            break;
        case CI_STMT_KIND_CONTINUE:
            write_str__CIGenerator("continue;");

            break;
        case CI_STMT_KIND_DEFAULT:
            write_str__CIGenerator("default:");

            break;
        case CI_STMT_KIND_DO_WHILE:
            generate_function_do_while_stmt__CIGenerator(&stmt->do_while);

            break;
        case CI_STMT_KIND_FOR:
            generate_function_for_stmt__CIGenerator(&stmt->for_);

            break;
        case CI_STMT_KIND_GOTO:
            write_String__CIGenerator(
              format__String("goto {S};", GET_PTR_RC(String, stmt->goto_)));

            break;
        case CI_STMT_KIND_IF:
            generate_function_if_stmt__CIGenerator(&stmt->if_);

            break;
        case CI_STMT_KIND_RETURN:
            write_str__CIGenerator("return ");

            if (stmt->return_) {
                generate_function_expr__CIGenerator(stmt->return_);
            }

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

            if (write_semicolon) {
                write_str__CIGenerator(";\n");
            }

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
generate_function_body__CIGenerator(const CIDeclFunctionBody *body)
{
    write_str__CIGenerator("{\n");
    inc_tab_count__CIGenerator();

    for (Usize i = 0; i < body->content->len; ++i) {
        write_tab__CIGenerator();
        generate_function_body_item__CIGenerator(get__Vec(body->content, i));
    }

    dec_tab_count__CIGenerator();
    write_tab__CIGenerator();
    write_str__CIGenerator("}\n");
}

void
generate_function_decl__CIGenerator(const CIDeclFunction *function)
{
    generate_function_prototype__CIGenerator(function);
    write_str__CIGenerator(" ");
    generate_function_body__CIGenerator(function->body);
}

void
generate_function_gen_decl__CIGenerator(const CIDeclFunctionGen *function_gen)
{
    generate_function_gen_prototype__CIGenerator(function_gen);
    SET_CURRENT_GENERIC_PARAMS(function_gen->function->generic_params,
                               function_gen->called_generic_params);
    write_str__CIGenerator(" ");
    generate_function_body__CIGenerator(function_gen->function->body);
    RESET_CURRENT_GENERIC_PARAMS();
}

void
generate_label_decl__CIGenerator(const CIDeclLabel *label)
{
    write_String__CIGenerator(
      format__String("{S}:\n", GET_PTR_RC(String, label->name)));
}

void
generate_struct_field__CIGenerator(const CIDeclStructField *field)
{
    generate_data_type__CIGenerator(field->data_type);

    if (field->name && !has_name__CIDataType(field->data_type)) {
        write_String__CIGenerator(
          format__String(" {S}", GET_PTR_RC(String, field->name)));
    }

    if (field->bit != 0) {
        write_String__CIGenerator(format__String(" : {zu}", field->bit));
    }

    write_str__CIGenerator(";\n");
}

void
generate_struct_fields__CIGenerator(const Vec *fields)
{
    inc_tab_count__CIGenerator();

    for (Usize i = 0; i < fields->len; ++i) {
        write_tab__CIGenerator();
        generate_struct_field__CIGenerator(get__Vec(fields, i));
    }

    dec_tab_count__CIGenerator();
}

void
generate_struct_decl__CIGenerator(const CIDeclStruct *struct_)
{
    write_String__CIGenerator(
      format__String("struct {S} {{\n", GET_PTR_RC(String, struct_->name)));
    generate_struct_fields__CIGenerator(struct_->fields);
    write_str__CIGenerator("}");
}

void
generate_struct_gen_decl__CIGenerator(const CIDeclStructGen *struct_gen)
{
    SET_CURRENT_GENERIC_PARAMS(struct_gen->struct_->generic_params,
                               struct_gen->called_generic_params);
    write_String__CIGenerator(
      format__String("struct {S} {{\n", struct_gen->name));
    generate_struct_fields__CIGenerator(struct_gen->fields);
    write_str__CIGenerator("}");
    RESET_CURRENT_GENERIC_PARAMS();
}

void
generate_typedef_decl__CIGenerator(const CIDeclTypedef *typedef_)
{
    write_str__CIGenerator("typedef ");
    generate_data_type__CIGenerator(typedef_->data_type);

    if (!has_name__CIDataType(typedef_->data_type)) {
        write_String__CIGenerator(
          format__String(" {S}", GET_PTR_RC(String, typedef_->name)));
    }
}

void
generate_typedef_gen_decl__CIGenerator(const CIDeclTypedefGen *typedef_gen)
{
    SET_CURRENT_GENERIC_PARAMS(typedef_gen->typedef_->generic_params,
                               typedef_gen->called_generic_params);
    write_str__CIGenerator("typedef ");
    generate_data_type__CIGenerator(typedef_gen->typedef_->data_type);

    if (!has_name__CIDataType(typedef_gen->data_type)) {
        write_String__CIGenerator(format__String(" {S}", typedef_gen->name));
    }

    RESET_CURRENT_GENERIC_PARAMS();
}

void
generate_union_decl__CIGenerator(const CIDeclUnion *union_)
{
    write_String__CIGenerator(
      format__String("union {S} {{\n", GET_PTR_RC(String, union_->name)));
    generate_struct_fields__CIGenerator(union_->fields);
    write_str__CIGenerator("}");
}

void
generate_union_gen_decl__CIGenerator(const CIDeclUnionGen *union_gen)
{
    SET_CURRENT_GENERIC_PARAMS(union_gen->union_->generic_params,
                               union_gen->called_generic_params);
    write_String__CIGenerator(
      format__String("union {S} {{\n", union_gen->name));
    generate_struct_fields__CIGenerator(union_gen->fields);
    write_str__CIGenerator("}");
    RESET_CURRENT_GENERIC_PARAMS();
}

void
generate_variable_decl__CIGenerator(const CIDeclVariable *variable)
{
    generate_data_type__CIGenerator(variable->data_type);

    if (!has_name__CIDataType(variable->data_type)) {
        write_String__CIGenerator(
          format__String(" {S}", GET_PTR_RC(String, variable->name)));
    }

    if (variable->expr) {
        write_str__CIGenerator(" = ");
        generate_function_expr__CIGenerator(variable->expr);
    }

    if (write_semicolon) {
        write_str__CIGenerator(";\n");
    }
}

void
generate_decl__CIGenerator(const CIDecl *decl)
{
    if (!has_generic__CIDecl(decl)) {
        generate_attributes_by_decl__CIGenerator(decl);
        generate_storage_class__CIGenerator(&decl->storage_class_flag);

        switch (decl->kind) {
            case CI_DECL_KIND_ENUM:
                if (decl->is_prototype) {
                    generate_enum_prototype__CIGenerator(&decl->enum_);

                    break;
                }

                generate_enum_decl__CIGenerator(&decl->enum_);

                break;
            case CI_DECL_KIND_ENUM_VARIANT:
                // NOTE: We don't want to generate enum variant here.
                return;
            case CI_DECL_KIND_FUNCTION:
                if (decl->is_prototype) {
                    generate_function_prototype__CIGenerator(&decl->function);

                    break;
                }

                return generate_function_decl__CIGenerator(&decl->function);
            case CI_DECL_KIND_FUNCTION_GEN:
                if (decl->is_prototype) {
                    generate_function_gen_prototype__CIGenerator(
                      &decl->function_gen);

                    break;
                }

                return generate_function_gen_decl__CIGenerator(
                  &decl->function_gen);
            case CI_DECL_KIND_LABEL:
                return generate_label_decl__CIGenerator(&decl->label);
            case CI_DECL_KIND_STRUCT:
                if (decl->is_prototype) {
                    generate_struct_prototype__CIGenerator(&decl->struct_);

                    break;
                }

                generate_struct_decl__CIGenerator(&decl->struct_);

                break;
            case CI_DECL_KIND_STRUCT_GEN:
                if (decl->is_prototype) {
                    generate_struct_gen_prototype__CIGenerator(
                      &decl->struct_gen);

                    break;
                }

                generate_struct_gen_decl__CIGenerator(&decl->struct_gen);

                break;
            case CI_DECL_KIND_TYPEDEF:
                generate_typedef_decl__CIGenerator(&decl->typedef_);

                break;
            case CI_DECL_KIND_TYPEDEF_GEN:
                generate_typedef_gen_decl__CIGenerator(&decl->typedef_gen);

                break;
            case CI_DECL_KIND_UNION:
                if (decl->is_prototype) {
                    generate_union_prototype__CIGenerator(&decl->union_);

                    break;
                }

                generate_union_decl__CIGenerator(&decl->union_);

                break;
            case CI_DECL_KIND_UNION_GEN:
                if (decl->is_prototype) {
                    generate_union_gen_prototype__CIGenerator(&decl->union_gen);

                    break;
                }

                generate_union_gen_decl__CIGenerator(&decl->union_gen);

                break;
            case CI_DECL_KIND_VARIABLE:
                return generate_variable_decl__CIGenerator(&decl->variable);
            default:
                UNREACHABLE("unknown variant");
        }

        write_str__CIGenerator(";\n");
    }
}

void
generate_global_decls__CIGenerator(const CIResultFile *file_result)
{
    CIDecl *decl = NULL;
    VecIter iter_decl = NEW(VecIter, file_result->entity.decls);

    while ((decl = next__VecIter(&iter_decl))) {
        switch (decl->kind) {
            case CI_DECL_KIND_VARIABLE:
                // The expression `!decl->variable.is_local` is used to prevent
                // local variables from being generated in the global scope.
                if (!decl->variable.is_local) {
                    generate_decl__CIGenerator(decl);
                }

                break;
            default:
                generate_decl__CIGenerator(decl);
        }
    }
}

void
generate_enum_prototype__CIGenerator(const CIDeclEnum *enum_)
{
    write_String__CIGenerator(
      format__String("enum {S}", GET_PTR_RC(String, enum_->name)));

    if (enum_->data_type) {
        write_str__CIGenerator(" : ");
        generate_data_type__CIGenerator(enum_->data_type);
    }
}

void
generate_struct_prototype__CIGenerator(const CIDeclStruct *struct_)
{
    write_String__CIGenerator(
      format__String("struct {S}", GET_PTR_RC(String, struct_->name)));
}

void
generate_struct_gen_prototype__CIGenerator(const CIDeclStructGen *struct_gen)
{
    write_String__CIGenerator(format__String("struct {S}", struct_gen->name));
}

void
generate_union_prototype__CIGenerator(const CIDeclUnion *union_)
{
    write_String__CIGenerator(
      format__String("union {S}", GET_PTR_RC(String, union_->name)));
}

void
generate_union_gen_prototype__CIGenerator(const CIDeclUnionGen *union_gen)
{
    write_String__CIGenerator(format__String("union {S}", union_gen->name));
}

void
generate_function_prototype__CIGenerator(const CIDeclFunction *function)
{
    generate_data_type__CIGenerator(function->return_data_type);
    write_String__CIGenerator(
      format__String(" {S}", GET_PTR_RC(String, function->name)));
    generate_function_params__CIGenerator(function->params);
}

void
generate_function_gen_prototype__CIGenerator(
  const CIDeclFunctionGen *function_gen)
{
    SET_CURRENT_GENERIC_PARAMS(function_gen->function->generic_params,
                               function_gen->called_generic_params);
    generate_data_type__CIGenerator(function_gen->function->return_data_type);
    write_String__CIGenerator(format__String(" {S}", function_gen->name));
    generate_function_params__CIGenerator(function_gen->function->params);
    RESET_CURRENT_GENERIC_PARAMS();
}

void
run_file__CIGenerator(const CIResultFile *file_result)
{
    current_result_content = NEW(String);

    // Set the standard specified in the project configuration.
    if (current_standard == CI_STANDARD_NONE) {
        current_standard = file_result->scanner.config->standard;
    }

    // TODO: Add a possibly to the user to "create its custom output director"
    const char *output_dir = "out.ci";
    String *dir_result = format__String(
      "{s}/{Sr}", output_dir, get_dir__File(file_result->file_input.name));
    String *path_result = format__String(
      "{S}/{S}", dir_result, file_result->entity.filename_result);

    generate_global_decls__CIGenerator(file_result);
    create_recursive_dir__Dir(dir_result->buffer,
                              DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    write_file__File(path_result->buffer, current_result_content->buffer);

    FREE(String, current_result_content);
    FREE(String, dir_result);
    FREE(String, path_result);
}
