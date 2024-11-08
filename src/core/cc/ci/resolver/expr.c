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

#include <core/cc/ci/builtin.h>
#include <core/cc/ci/resolver/expr.h>

#include <stdio.h>

struct DataTypeInfoAlignment
{
    Usize alignment;
    bool is_incomplete;
};

struct DataTypeInfoSize
{
    Usize size;
    bool is_incomplete;
};

static Usize
calculate_enum_alignment__CIResolverExpr(const CIResolverExpr *self,
                                         CIDecl *enum_decl);

/// @param fields const Vec<CIDeclStructField*>* (&)
static Usize
calculate_struct_or_union_alignment__CIResolverExpr(const CIResolverExpr *self,
                                                    const Vec *fields);

static struct DataTypeInfoAlignment
resolve_struct_or_union_alignment__CIResolverExpr(
  const CIResolverExpr *self,
  const CIDataType *data_type,
  CIDecl *(*search)(const CIParser *self,
                    const String *name,
                    CIGenericParams *called_generic_params));

static struct DataTypeInfoAlignment
resolve_struct_alignment__CIResolverExpr(const CIResolverExpr *self,
                                         const CIDataType *struct_data_type);

static struct DataTypeInfoAlignment
resolve_union_alignment__CIResolverExpr(const CIResolverExpr *self,
                                        const CIDataType *union_data_type);

static struct DataTypeInfoAlignment
resolve_enum_alignment__CIResolverExpr(const CIResolverExpr *self,
                                       const String *enum_name);

static Usize
resolve_data_type_alignment__CIResolverExpr(const CIResolverExpr *self,
                                            const CIDataType *data_type);

/// @brief Resolve alignof(...)
static CIExpr *
resolve_alignof_expr__CIResolverExpr(const CIResolverExpr *self,
                                     const CIExpr *expr);

/// @brief lhs + rhs
static CIExpr *
resolve_add_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs - rhs
static CIExpr *
resolve_sub_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs * rhs
static CIExpr *
resolve_mul_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs / rhs
static CIExpr *
resolve_div_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs % rhs
static CIExpr *
resolve_mod_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs & rhs
static CIExpr *
resolve_bit_and_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs | rhs
static CIExpr *
resolve_bit_or_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs ^ rhs
static CIExpr *
resolve_bit_xor_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs << rhs
static CIExpr *
resolve_bit_lshift_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs >> rhs
static CIExpr *
resolve_bit_rshift_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs && rhs
static CIExpr *
resolve_logical_and_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs || rhs
static CIExpr *
resolve_logical_or_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs == rhs
static CIExpr *
resolve_eq_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs != rhs
static CIExpr *
resolve_ne_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs < rhs
static CIExpr *
resolve_lt_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs <= rhs
static CIExpr *
resolve_le_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs > rhs
static CIExpr *
resolve_gt_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

/// @brief lhs >= rhs
static CIExpr *
resolve_ge_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs);

static Usize
calculate_enum_size__CIResolverExpr(const CIResolverExpr *self,
                                    CIDecl *enum_decl);

/// @param enum_name const String* (&)
static struct DataTypeInfoSize
resolve_enum_size__CIResolverExpr(const CIResolverExpr *self,
                                  const String *enum_name);

/// @param fields const Vec<CIDeclStructField*>* (&)
static Usize
calculate_struct_size__CIResolverExpr(const CIResolverExpr *self,
                                      const Vec *fields);

/// @param fields const Vec<CIDeclStructField*>* (&)
static Usize
calculate_union_size__CIResolverExpr(const CIResolverExpr *self,
                                     const Vec *fields);

/// @param struct_data_type const CIDataType* (&)
static struct DataTypeInfoSize
resolve_struct_size__CIResolverExpr(const CIResolverExpr *self,
                                    const CIDataType *struct_data_type);

/// @param union_data_type const CIDataType* (&)
static struct DataTypeInfoSize
resolve_union_size__CIResolverExpr(const CIResolverExpr *self,
                                   const CIDataType *union_data_type);

/// @param data_type const CIDataType* (&)
/// @return If the function returns 0, this means that the size could not be
/// resolved.
static Usize
resolve_data_type_size__CIResolverExpr(const CIResolverExpr *self,
                                       const CIDataType *data_type);

/// @brief Resolve sizeof(...)
static CIExpr *
resolve_sizeof_expr__CIResolverExpr(const CIResolverExpr *self,
                                    const CIExpr *expr);

/// @brief Resolve: ++rhs
static CIExpr *
resolve_pre_increment_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: --rhs
static CIExpr *
resolve_pre_decrement_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: rhs++
static CIExpr *
resolve_post_increment_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: rhs--
static CIExpr *
resolve_post_decrement_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: +rhs
static CIExpr *
resolve_positive_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: -rhs
static CIExpr *
resolve_negative_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: ~rhs
static CIExpr *
resolve_bit_not_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: !rhs
static CIExpr *
resolve_not_expr__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: *rhs
static CIExpr *
resolve_dereference__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve: &rhs
static CIExpr *
resolve_ref__CIResolverExpr(CIExpr *rhs);

/// @brief Resolve unary operator.
static CIExpr *
resolve_unary_expr__CIResolverExpr(const CIResolverExpr *self, CIExpr *expr);

bool
is_true__CIResolverExpr(CIExpr *expr)
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
                    FAILED("not expected to have a string as result at "
                           "preprocessor-time");
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
                    return expr->literal.unsigned_int;
                default:
                    UNREACHABLE("unknown variant");
            }
        default:
            return false;
    }
}

// +, -, *, /
#define RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(op, binary_kind)                  \
    ASSERT(lhs &&rhs);                                                         \
                                                                               \
    if (lhs->kind == CI_EXPR_KIND_LITERAL &&                                   \
        rhs->kind == CI_EXPR_KIND_LITERAL) {                                   \
        switch (lhs->literal.kind) {                                           \
            case CI_EXPR_LITERAL_KIND_BOOL:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.bool_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.bool_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            float,                                             \
                            lhs->literal.bool_ op rhs->literal.float_));       \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.bool_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.bool_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_CHAR:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.char_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.char_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            float,                                             \
                            lhs->literal.char_ op rhs->literal.float_));       \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.char_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.char_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_FLOAT:                                   \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            float,                                             \
                            lhs->literal.float_ op rhs->literal.bool_));       \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            float,                                             \
                            lhs->literal.float_ op rhs->literal.char_));       \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            float,                                             \
                            lhs->literal.float_ op rhs->literal.float_));      \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            float,                                             \
                            lhs->literal.float_ op rhs->literal.signed_int));  \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      float,                                   \
                                      lhs->literal.float_ op                   \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_SIGNED_INT:                              \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.signed_int op rhs->literal.bool_));   \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.signed_int op rhs->literal.char_));   \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            float,                                             \
                            lhs->literal.signed_int op rhs->literal.float_));  \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      signed_int,                              \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      signed_int,                              \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_STRING:                                  \
                FAILED("this operation is unsure at compile-time");            \
            case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                            \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.unsigned_int op rhs->literal.bool_)); \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.unsigned_int op rhs->literal.char_)); \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      float,                                   \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.float_));                 \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      unsigned_int,                            \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      unsigned_int,                            \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
        }                                                                      \
    } else {                                                                   \
        FAILED("This expression is not resolvable at preprocessor-time");      \
    }

Usize
calculate_enum_alignment__CIResolverExpr(const CIResolverExpr *self,
                                         CIDecl *enum_decl)
{
    return enum_decl->enum_.data_type
             ? resolve_data_type_alignment__CIResolverExpr(
                 self, enum_decl->enum_.data_type)
             : alignof(int);
}

static Usize
calculate_struct_or_union_alignment__CIResolverExpr(const CIResolverExpr *self,
                                                    const Vec *fields)
{
    Usize max_alignment = 0;

    for (Usize i = 0; i < fields->len; ++i) {
        const CIDeclStructField *field = get__Vec(fields, i);
        Usize field_alignment =
          resolve_data_type_alignment__CIResolverExpr(self, field->data_type);

        if (field_alignment > max_alignment) {
            max_alignment = field_alignment;
        }
    }

    return max_alignment;
}

struct DataTypeInfoAlignment
resolve_struct_or_union_alignment__CIResolverExpr(
  const CIResolverExpr *self,
  const CIDataType *data_type,
  CIDecl *(*search)(const CIParser *self,
                    const String *name,
                    CIGenericParams *called_generic_params))
{
    Vec *fields = data_type->struct_.fields;
    CIDecl *decl = !fields ? search(self->parser,
                                    GET_PTR_RC(String, data_type->struct_.name),
                                    data_type->struct_.generic_params)
                           : NULL;
    Usize alignment = 0;

    if (decl) {
        alignment = get_alignment__CIDecl(decl);
        fields = alignment == 0 ? (Vec *)get_fields__CIDecl(decl) : NULL;
    }

    if (fields) {
        alignment =
          calculate_struct_or_union_alignment__CIResolverExpr(self, fields);

        fill_alignment__CIDecl(decl, alignment);
    }

    return (struct DataTypeInfoAlignment){ .alignment = alignment,
                                           .is_incomplete = decl || fields };
}

struct DataTypeInfoAlignment
resolve_struct_alignment__CIResolverExpr(const CIResolverExpr *self,
                                         const CIDataType *struct_data_type)
{
    return resolve_struct_or_union_alignment__CIResolverExpr(
      self, struct_data_type, &search_struct__CIParser);
}

struct DataTypeInfoAlignment
resolve_union_alignment__CIResolverExpr(const CIResolverExpr *self,
                                        const CIDataType *union_data_type)
{
    return resolve_struct_or_union_alignment__CIResolverExpr(
      self, union_data_type, &search_union__CIParser);
}

struct DataTypeInfoAlignment
resolve_enum_alignment__CIResolverExpr(const CIResolverExpr *self,
                                       const String *enum_name)
{
    CIDecl *decl = search_enum__CIParser(self->parser, enum_name);
    Usize enum_alignment = 0;

    if (decl) {
        enum_alignment = get_alignment__CIDecl(decl);

        if (enum_alignment == 0) {
            enum_alignment =
              calculate_enum_alignment__CIResolverExpr(self, decl);

            fill_alignment__CIDecl(decl, enum_alignment);
        }
    }

    return (struct DataTypeInfoAlignment){ .alignment = enum_alignment,
                                           .is_incomplete = decl };
}

Usize
resolve_data_type_alignment__CIResolverExpr(const CIResolverExpr *self,
                                            const CIDataType *data_type)
{
    // NOTE: This part of the code is clearly not adapted to have a transpiler
    // option for any architecture or OS.
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ANY:
            UNREACHABLE("cannot resolve the size of any data type");
        case CI_DATA_TYPE_KIND_ARRAY:
            ASSERT(data_type->array.kind == CI_DATA_TYPE_ARRAY_KIND_SIZED);

            return resolve_data_type_alignment__CIResolverExpr(
                     self, data_type->array.data_type) *
                   data_type->array.size;
        case CI_DATA_TYPE_KIND_BOOL:
            return alignof(bool);
        case CI_DATA_TYPE_KIND_BUILTIN: {
            const CIBuiltin *builtin = get_ref__CIBuiltin();
            const CIBuiltinType *builtin_type =
              get_builtin_type__CIBuiltin(builtin, data_type->builtin);

            ASSERT(builtin_type);

            return builtin_type->alignment;
        }
        case CI_DATA_TYPE_KIND_CHAR:
            return alignof(char);
        case CI_DATA_TYPE_KIND_DOUBLE:
            return alignof(double);
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
            return alignof(double _Complex);
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
            TODO("get double imaginary alignment");
        case CI_DATA_TYPE_KIND__DECIMAL32:
            return alignof(int32_t);
        case CI_DATA_TYPE_KIND__DECIMAL64:
            return alignof(int64_t);
        case CI_DATA_TYPE_KIND__DECIMAL128:
            return alignof(__int128);
        case CI_DATA_TYPE_KIND_ENUM: {
            ASSERT(data_type->enum_);

            struct DataTypeInfoAlignment enum_alignment_info =
              resolve_enum_alignment__CIResolverExpr(
                self, GET_PTR_RC(String, data_type->enum_));

            if (enum_alignment_info.is_incomplete) {
                FAILED("enum type is incomplete");
            }

            return enum_alignment_info.alignment;
        }
        case CI_DATA_TYPE_KIND_FLOAT:
            return alignof(float);
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
            return alignof(float _Complex);
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
            TODO("get float imaginary alignment");
        case CI_DATA_TYPE_KIND_FUNCTION:
            return alignof(int(int));
        case CI_DATA_TYPE_KIND_GENERIC:
            return 0; // At this point, we have no idea about the alignment of
                      // this
        case CI_DATA_TYPE_KIND_INT:
            return alignof(int);
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
            return alignof(long double);
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
            return alignof(long double _Complex);
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            TODO("get long double imaginary alignment");
        case CI_DATA_TYPE_KIND_LONG_INT:
            return alignof(long int);
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
            return alignof(long long int);
        case CI_DATA_TYPE_KIND_PTR:
            return alignof(void *);
        case CI_DATA_TYPE_KIND_SHORT_INT:
            return alignof(short int);
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            return alignof(signed char);
        case CI_DATA_TYPE_KIND_STRUCT: {
            struct DataTypeInfoAlignment struct_alignment_info =
              resolve_struct_alignment__CIResolverExpr(self, data_type);

            if (struct_alignment_info.is_incomplete) {
                FAILED("struct type is incomplete");
            }

            return struct_alignment_info.alignment;
        }
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDecl *decl = search_typedef__CIParser(
              self->parser,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params);

            if (decl) {
                return resolve_data_type_alignment__CIResolverExpr(
                  self, get_typedef_data_type__CIDecl(decl));
            }

            FAILED("type of typedef is incomplete");
        }
        case CI_DATA_TYPE_KIND_TYPE_INFO:
            FAILED("cannot resolve the size of type info data type");
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
            return alignof(unsigned int);
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
            return alignof(unsigned char);
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
            return alignof(unsigned long int);
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
            return alignof(unsigned long long int);
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            return alignof(unsigned short int);
        case CI_DATA_TYPE_KIND_UNION: {
            struct DataTypeInfoAlignment union_alignment_info =
              resolve_union_alignment__CIResolverExpr(self, data_type);

            if (union_alignment_info.is_incomplete) {
                FAILED("union type is incomplete");
            }

            return union_alignment_info.alignment;
        }
        case CI_DATA_TYPE_KIND_VOID:
            return alignof(void);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIExpr *
resolve_alignof_expr__CIResolverExpr(const CIResolverExpr *self,
                                     const CIExpr *expr)
{
    if (self->is_at_preprocessor_time) {
        FAILED("alignof is not expected in preprocessor condition");
    }

    ASSERT(self->scope);
    ASSERT(expr->kind == CI_EXPR_KIND_ALIGNOF);

    CIDataType *expr_data_type = infer_expr_data_type__CIParser(
      self->parser, expr->alignof_, self->scope->scope_id, NULL, NULL);

    CIExpr *res =
      NEW_VARIANT(CIExpr,
                  literal,
                  NEW_VARIANT(CIExprLiteral,
                              unsigned_int,
                              resolve_data_type_alignment__CIResolverExpr(
                                self, expr->alignof_->data_type)));

    FREE(CIDataType, expr_data_type);

    return res;
}

CIExpr *
resolve_add_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(+, CI_EXPR_BINARY_KIND_ADD);
}

CIExpr *
resolve_sub_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(-, CI_EXPR_BINARY_KIND_SUB);
}

CIExpr *
resolve_mul_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(*, CI_EXPR_BINARY_KIND_MUL);
}

CIExpr *
resolve_div_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_ARITHMETIC_EXPR(/, CI_EXPR_BINARY_KIND_DIV);
}

// &, |, ^, <<, >>
// Also %
#define RESOLVE_BASIC_BINARY_BIT_EXPR(op, binary_kind)                         \
    ASSERT(lhs &&rhs);                                                         \
                                                                               \
    if (lhs->kind == CI_EXPR_KIND_LITERAL &&                                   \
        rhs->kind == CI_EXPR_KIND_LITERAL) {                                   \
        switch (lhs->literal.kind) {                                           \
            case CI_EXPR_LITERAL_KIND_BOOL:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.bool_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.bool_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.bool_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.bool_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_CHAR:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.char_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.char_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.char_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.char_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_FLOAT:                                   \
                FAILED("cannot perform this operation with no-castable "       \
                       "to int types");                                        \
            case CI_EXPR_LITERAL_KIND_SIGNED_INT:                              \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.signed_int op rhs->literal.bool_));   \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            signed_int,                                        \
                            lhs->literal.signed_int op rhs->literal.char_));   \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      signed_int,                              \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      unsigned_int,                            \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_STRING:                                  \
                FAILED("this operation is unsure at compile-time");            \
            case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                            \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.unsigned_int op rhs->literal.bool_)); \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            unsigned_int,                                      \
                            lhs->literal.unsigned_int op rhs->literal.char_)); \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      unsigned_int,                            \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      unsigned_int,                            \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
        }                                                                      \
    } else {                                                                   \
        FAILED("This expression is not resolvable at preprocessor-time");      \
    }

CIExpr *
resolve_mod_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(%, CI_EXPR_BINARY_KIND_MOD);
}

CIExpr *
resolve_bit_and_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(&, CI_EXPR_BINARY_KIND_BIT_AND);
}

CIExpr *
resolve_bit_or_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(|, CI_EXPR_BINARY_KIND_BIT_OR);
}

CIExpr *
resolve_bit_xor_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(^, CI_EXPR_BINARY_KIND_BIT_XOR);
}

CIExpr *
resolve_bit_lshift_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(<<, CI_EXPR_BINARY_KIND_BIT_LSHIFT);
}

CIExpr *
resolve_bit_rshift_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_BIT_EXPR(>>, CI_EXPR_BINARY_KIND_BIT_RSHIFT);
}

// &&, ||
#define RESOLVE_BASIC_BINARY_LOGICAL_EXPR(op, binary_kind)                     \
    ASSERT(lhs &&rhs);                                                         \
                                                                               \
    if (lhs->kind == CI_EXPR_KIND_LITERAL &&                                   \
        rhs->kind == CI_EXPR_KIND_LITERAL) {                                   \
        switch (lhs->literal.kind) {                                           \
            case CI_EXPR_LITERAL_KIND_BOOL:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_CHAR:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_FLOAT:                                   \
                FAILED("cannot perform this operation with no-castable "       \
                       "to int types");                                        \
            case CI_EXPR_LITERAL_KIND_SIGNED_INT:                              \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.signed_int op rhs->literal.bool_));   \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.signed_int op rhs->literal.char_));   \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_STRING:                                  \
                FAILED("this operation is unsure at compile-time");            \
            case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                            \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.unsigned_int op rhs->literal.bool_)); \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.unsigned_int op rhs->literal.char_)); \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        FAILED("cannot perform this operation with "           \
                               "no-castable to int types");                    \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
        }                                                                      \
    } else {                                                                   \
        FAILED("This expression is not resolvable at preprocessor-time");      \
    }

CIExpr *
resolve_logical_and_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_LOGICAL_EXPR(&&, CI_EXPR_BINARY_KIND_AND);
}

CIExpr *
resolve_logical_or_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_LOGICAL_EXPR(||, CI_EXPR_BINARY_KIND_OR);
}

#define RESOLVE_BASIC_BINARY_COMPARISON_EXPR(op, binary_kind)                  \
    ASSERT(lhs &&rhs);                                                         \
                                                                               \
    if (lhs->kind == CI_EXPR_KIND_LITERAL &&                                   \
        rhs->kind == CI_EXPR_KIND_LITERAL) {                                   \
        switch (lhs->literal.kind) {                                           \
            case CI_EXPR_LITERAL_KIND_BOOL:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.float_));       \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.bool_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_CHAR:                                    \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.bool_));        \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.char_));        \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.float_));       \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.signed_int));   \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.char_ op rhs->literal.unsigned_int)); \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_FLOAT:                                   \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.float_ op rhs->literal.bool_));       \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.float_ op rhs->literal.char_));       \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.float_ op rhs->literal.float_));      \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.float_ op rhs->literal.signed_int));  \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.float_ op                   \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_SIGNED_INT:                              \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.signed_int op rhs->literal.bool_));   \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.signed_int op rhs->literal.char_));   \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.signed_int op rhs->literal.float_));  \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.signed_int op               \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
            case CI_EXPR_LITERAL_KIND_STRING:                                  \
                FAILED("this operation is unsure at compile-time");            \
            case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                            \
                switch (rhs->literal.kind) {                                   \
                    case CI_EXPR_LITERAL_KIND_BOOL:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.unsigned_int op rhs->literal.bool_)); \
                    case CI_EXPR_LITERAL_KIND_CHAR:                            \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(                                         \
                            CIExprLiteral,                                     \
                            bool,                                              \
                            lhs->literal.unsigned_int op rhs->literal.char_)); \
                    case CI_EXPR_LITERAL_KIND_FLOAT:                           \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.float_));                 \
                    case CI_EXPR_LITERAL_KIND_STRING:                          \
                        FAILED("this operation is unsure at compile-time");    \
                    case CI_EXPR_LITERAL_KIND_SIGNED_INT:                      \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.signed_int));             \
                    case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                    \
                        return NEW_VARIANT(                                    \
                          CIExpr,                                              \
                          literal,                                             \
                          NEW_VARIANT(CIExprLiteral,                           \
                                      bool,                                    \
                                      lhs->literal.unsigned_int op             \
                                        rhs->literal.unsigned_int));           \
                    default:                                                   \
                        UNREACHABLE("unknown variant");                        \
                }                                                              \
        }                                                                      \
    } else {                                                                   \
        FAILED("This expression is not resolvable at preprocessor-time");      \
    }

CIExpr *
resolve_eq_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(==, CI_EXPR_BINARY_KIND_EQ);
}

CIExpr *
resolve_ne_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(!=, CI_EXPR_BINARY_KIND_NE);
}

CIExpr *
resolve_lt_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(<, CI_EXPR_BINARY_KIND_LESS);
}

CIExpr *
resolve_le_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(<=, CI_EXPR_BINARY_KIND_LESS_EQ);
}

CIExpr *
resolve_gt_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(>, CI_EXPR_BINARY_KIND_GREATER);
}

CIExpr *
resolve_ge_expr__CIResolverExpr(CIExpr *lhs, CIExpr *rhs)
{
    RESOLVE_BASIC_BINARY_COMPARISON_EXPR(>=, CI_EXPR_BINARY_KIND_GREATER_EQ);
}

Usize
calculate_enum_size__CIResolverExpr(const CIResolverExpr *self,
                                    CIDecl *enum_decl)
{
    return enum_decl->enum_.data_type ? resolve_data_type_size__CIResolverExpr(
                                          self, enum_decl->enum_.data_type)
                                      : sizeof(int);
}

struct DataTypeInfoSize
resolve_enum_size__CIResolverExpr(const CIResolverExpr *self,
                                  const String *enum_name)
{
    CIDecl *decl = search_enum__CIParser(self->parser, enum_name);
    Usize enum_size = 0;

    if (decl) {
        enum_size = get_size__CIDecl(decl);

        if (enum_size == 0) {
            enum_size = calculate_enum_size__CIResolverExpr(self, decl);

            fill_size__CIDecl(decl, enum_size);
        }
    }

    return (struct DataTypeInfoSize){ .size = enum_size,
                                      .is_incomplete = decl };
}

Usize
calculate_struct_size__CIResolverExpr(const CIResolverExpr *self,
                                      const Vec *fields)
{
    ASSERT(!has_generic__CIDeclStructField(fields));

    Usize total_size = 0;
    Usize max_alignment = 0;

    for (Usize i = 0; i < fields->len; ++i) {
        const CIDeclStructField *field = get__Vec(fields, i);
        Usize field_alignment =
          resolve_data_type_alignment__CIResolverExpr(self, field->data_type);

        // NOTE: We skip this case, as it means that the alignment could not be
        // resolved.
        if (field_alignment == 0) {
            continue;
        }

        if (max_alignment < field_alignment) {
            max_alignment = field_alignment;
        }

        if (total_size % field_alignment != 0) {
            total_size += field_alignment - (total_size % field_alignment);
        }

        total_size +=
          resolve_data_type_size__CIResolverExpr(self, field->data_type);
    }

    if (total_size % max_alignment != 0) {
        total_size += max_alignment - (total_size % max_alignment);
    }

    return total_size;
}

Usize
calculate_union_size__CIResolverExpr(const CIResolverExpr *self,
                                     const Vec *fields)
{
    Usize max_size = 0;

    for (Usize i = 0; i < fields->len; ++i) {
        const CIDeclStructField *field = get__Vec(fields, i);
        Usize field_size =
          resolve_data_type_size__CIResolverExpr(self, field->data_type);

        if (field_size > max_size) {
            max_size = field_size;
        }
    }

    return max_size;
}

struct DataTypeInfoSize
resolve_struct_size__CIResolverExpr(const CIResolverExpr *self,
                                    const CIDataType *struct_data_type)
{
    Vec *struct_fields = struct_data_type->struct_.fields;
    CIDecl *decl = !struct_fields
                     ? search_struct__CIParser(
                         self->parser,
                         GET_PTR_RC(String, struct_data_type->struct_.name),
                         struct_data_type->struct_.generic_params)
                     : NULL;
    Usize struct_size = 0;

    if (decl) {
        struct_size = get_size__CIDecl(decl);
        struct_fields =
          struct_size == 0 ? (Vec *)get_fields__CIDecl(decl) : NULL;
    }

    if (struct_fields) {
        struct_size =
          calculate_struct_size__CIResolverExpr(self, struct_fields);

        fill_size__CIDecl(decl, struct_size);
    }

    return (struct DataTypeInfoSize){ .size = struct_size,
                                      .is_incomplete = decl || struct_fields };
}

struct DataTypeInfoSize
resolve_union_size__CIResolverExpr(const CIResolverExpr *self,
                                   const CIDataType *union_data_type)
{
    Vec *union_fields = union_data_type->union_.fields;
    CIDecl *decl = !union_fields
                     ? search_union__CIParser(
                         self->parser,
                         GET_PTR_RC(String, union_data_type->union_.name),
                         union_data_type->union_.generic_params)
                     : NULL;
    Usize union_size = 0;

    if (decl) {
        union_size = get_size__CIDecl(decl);
        union_fields = union_size == 0 ? (Vec *)get_fields__CIDecl(decl) : NULL;
    }

    if (union_fields) {
        union_size = calculate_union_size__CIResolverExpr(self, union_fields);

        fill_size__CIDecl(decl, union_size);
    }

    return (struct DataTypeInfoSize){ .size = union_size,
                                      .is_incomplete = decl || union_fields };
}

Usize
resolve_data_type_size__CIResolverExpr(const CIResolverExpr *self,
                                       const CIDataType *data_type)
{
    // NOTE: This part of the code is clearly not adapted to have a transpiler
    // option for any architecture or OS.
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ANY:
            FAILED("cannot resolve size of any data type");
        case CI_DATA_TYPE_KIND_ARRAY:
            ASSERT(data_type->array.kind == CI_DATA_TYPE_ARRAY_KIND_SIZED);

            return resolve_data_type_size__CIResolverExpr(
                     self, data_type->array.data_type) *
                   data_type->array.size;
        case CI_DATA_TYPE_KIND_BOOL:
            return sizeof(bool);
        case CI_DATA_TYPE_KIND_BUILTIN: {
            const CIBuiltin *builtin = get_ref__CIBuiltin();
            const CIBuiltinType *builtin_type =
              get_builtin_type__CIBuiltin(builtin, data_type->builtin);

            ASSERT(builtin_type);

            return builtin_type->size;
        }
        case CI_DATA_TYPE_KIND_CHAR:
            return sizeof(char);
        case CI_DATA_TYPE_KIND_DOUBLE:
            return sizeof(double);
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
            return sizeof(double _Complex);
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
            TODO("get double imaginary size");
        case CI_DATA_TYPE_KIND__DECIMAL32:
            return sizeof(int32_t);
        case CI_DATA_TYPE_KIND__DECIMAL64:
            return sizeof(int64_t);
        case CI_DATA_TYPE_KIND__DECIMAL128:
            return sizeof(__int128);
        case CI_DATA_TYPE_KIND_ENUM: {
            ASSERT(data_type->enum_);

            struct DataTypeInfoSize enum_size_info =
              resolve_enum_size__CIResolverExpr(
                self, GET_PTR_RC(String, data_type->enum_));

            if (enum_size_info.is_incomplete) {
                FAILED("enum type is incomplete");
            }

            return enum_size_info.size;
        }
        case CI_DATA_TYPE_KIND_FLOAT:
            return sizeof(float);
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
            return sizeof(float _Complex);
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
            TODO("get float imaginary size");
        case CI_DATA_TYPE_KIND_FUNCTION:
            // NOTE: sizeof(int(int)) == sizeof(int(int, int, int)) ==
            // sizeof(int(int, int, ...)) A function passed by value, will
            // always have the same size.
            return sizeof(int(int));
        case CI_DATA_TYPE_KIND_GENERIC:
            return 0; // At this point, we have no idea about the size of this
                      // type.
        case CI_DATA_TYPE_KIND_INT:
            return sizeof(int);
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
            return sizeof(long double);
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
            return sizeof(long double _Complex);
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            TODO("get long double imaginary size");
        case CI_DATA_TYPE_KIND_LONG_INT:
            return sizeof(long int);
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
            return sizeof(long long int);
        case CI_DATA_TYPE_KIND_PTR:
            return sizeof(void *);
        case CI_DATA_TYPE_KIND_SHORT_INT:
            return sizeof(short int);
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            return sizeof(signed char);
        case CI_DATA_TYPE_KIND_STRUCT: {
            struct DataTypeInfoSize struct_size_info =
              resolve_struct_size__CIResolverExpr(self, data_type);

            if (struct_size_info.is_incomplete) {
                FAILED("struct type is incomplete");
            }

            return struct_size_info.size;
        }
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            const CIDecl *decl = search_typedef__CIParser(
              self->parser,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params);

            if (decl) {
                return resolve_data_type_size__CIResolverExpr(
                  self, get_typedef_data_type__CIDecl(decl));
            }

            FAILED("typedef type is incomplete");
        }
        case CI_DATA_TYPE_KIND_TYPE_INFO:
            FAILED("cannot resolve size of type info data type");
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
            return sizeof(unsigned int);
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
            return sizeof(unsigned char);
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
            return sizeof(unsigned long int);
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
            return sizeof(unsigned long long int);
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            return sizeof(unsigned short int);
        case CI_DATA_TYPE_KIND_UNION: {
            struct DataTypeInfoSize union_size_info =
              resolve_union_size__CIResolverExpr(self, data_type);

            if (union_size_info.is_incomplete) {
                FAILED("union type is incomplete");
            }

            return union_size_info.size;
        }
        case CI_DATA_TYPE_KIND_VOID:
            return sizeof(void);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIExpr *
resolve_sizeof_expr__CIResolverExpr(const CIResolverExpr *self,
                                    const CIExpr *expr)
{
    if (self->is_at_preprocessor_time) {
        FAILED("sizeof is not expected in preprcoessor condition");
    }

    ASSERT(expr->kind == CI_EXPR_KIND_SIZEOF);

    switch (expr->kind) {
        case CI_EXPR_KIND_DATA_TYPE:
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(CIExprLiteral,
                          unsigned_int,
                          resolve_data_type_size__CIResolverExpr(
                            self, expr->sizeof_->data_type)));
        default: {
            ASSERT(self->scope);

            // NOTE: This function is used only to resolve expression in
            // preprocessor, so in this situation it's impossible to have
            // generic cases.
            CIDataType *expr_data_type = infer_expr_data_type__CIParser(
              self->parser, expr, self->scope->scope_id, NULL, NULL);
            CIExpr *res =
              NEW_VARIANT(CIExpr,
                          literal,
                          NEW_VARIANT(CIExprLiteral,
                                      unsigned_int,
                                      resolve_data_type_size__CIResolverExpr(
                                        self, expr_data_type)));

            FREE(CIDataType, expr_data_type);

            return res;
        }
    }
}

#define RESOLVE_BASIC_ARITHMETIC_UNARY_EXPR(op, unary_kind)                    \
    ASSERT(rhs);                                                               \
                                                                               \
    if (rhs->kind == CI_EXPR_KIND_LITERAL) {                                   \
        switch (rhs->literal.kind) {                                           \
            case CI_EXPR_LITERAL_KIND_BOOL:                                    \
                return NEW_VARIANT(CIExpr,                                     \
                                   literal,                                    \
                                   NEW_VARIANT(CIExprLiteral,                  \
                                               signed_int,                     \
                                               op(Isize) rhs->literal.bool_)); \
            case CI_EXPR_LITERAL_KIND_CHAR:                                    \
                return NEW_VARIANT(                                            \
                  CIExpr,                                                      \
                  literal,                                                     \
                  NEW_VARIANT(CIExprLiteral, char, op rhs->literal.char_));    \
            case CI_EXPR_LITERAL_KIND_FLOAT:                                   \
                if (unary_kind == CI_EXPR_UNARY_KIND_BIT_NOT) {                \
                    FAILED("~ operation is not expected for float");           \
                }                                                              \
                                                                               \
                return NEW_VARIANT(                                            \
                  CIExpr,                                                      \
                  literal,                                                     \
                  NEW_VARIANT(                                                 \
                    CIExprLiteral, float, op(Isize) rhs->literal.float_));     \
            case CI_EXPR_LITERAL_KIND_SIGNED_INT:                              \
                return NEW_VARIANT(CIExpr,                                     \
                                   literal,                                    \
                                   NEW_VARIANT(CIExprLiteral,                  \
                                               signed_int,                     \
                                               op rhs->literal.signed_int));   \
            case CI_EXPR_LITERAL_KIND_STRING:                                  \
                FAILED("this kind of operation is not expected for string");   \
            case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:                            \
                return NEW_VARIANT(CIExpr,                                     \
                                   literal,                                    \
                                   NEW_VARIANT(CIExprLiteral,                  \
                                               unsigned_int,                   \
                                               op rhs->literal.unsigned_int)); \
            default:                                                           \
                UNREACHABLE("unknown variant");                                \
        }                                                                      \
    } else {                                                                   \
        FAILED("This expression is not resolvable at preprocessor-time");      \
    }

CIExpr *
resolve_pre_increment_expr__CIResolverExpr(CIExpr *rhs)
{
    FAILED("++rhs cannot be resolved at preprocessor-time");
}

CIExpr *
resolve_pre_decrement_expr__CIResolverExpr(CIExpr *rhs)
{
    FAILED("--rhs cannot be resolved at preprocessor-time");
}

CIExpr *
resolve_post_increment_expr__CIResolverExpr(CIExpr *rhs)
{
    FAILED("rhs++ cannot be resolved at preprocessor-time");
}

CIExpr *
resolve_post_decrement_expr__CIResolverExpr(CIExpr *rhs)
{
    FAILED("rhs-- cannot be resolved at preprocessor-time");
}

CIExpr *
resolve_positive_expr__CIResolverExpr(CIExpr *rhs)
{
    RESOLVE_BASIC_ARITHMETIC_UNARY_EXPR(+, CI_EXPR_UNARY_KIND_POSITIVE);
}

CIExpr *
resolve_negative_expr__CIResolverExpr(CIExpr *rhs)
{
    RESOLVE_BASIC_ARITHMETIC_UNARY_EXPR(-, CI_EXPR_UNARY_KIND_NEGATIVE);
}

CIExpr *
resolve_bit_not_expr__CIResolverExpr(CIExpr *rhs)
{
    RESOLVE_BASIC_ARITHMETIC_UNARY_EXPR(~, CI_EXPR_UNARY_KIND_BIT_NOT);
}

CIExpr *
resolve_not_expr__CIResolverExpr(CIExpr *rhs)
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
            FAILED("This expression is not resolvable at preprocessor-time");
    }
}

CIExpr *
resolve_dereference__CIResolverExpr(CIExpr *rhs)
{
    FAILED("This expression is not resolvable at preprocessor-time");
}

CIExpr *
resolve_ref__CIResolverExpr(CIExpr *rhs)
{
    FAILED("This expression is not resolvable at preprocessor-time");
}

CIExpr *
resolve_unary_expr__CIResolverExpr(const CIResolverExpr *self, CIExpr *expr)
{
    CIExpr *rhs = run__CIResolverExpr(self, expr->unary.expr);
    CIExpr *res = NULL;

    switch (expr->unary.kind) {
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
            res = resolve_pre_increment_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            res = resolve_pre_decrement_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
            res = resolve_post_increment_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
            res = resolve_post_decrement_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_POSITIVE:
            res = resolve_positive_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_NEGATIVE:
            res = resolve_negative_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_BIT_NOT:
            res = resolve_bit_not_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_NOT:
            res = resolve_not_expr__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_DEREFERENCE:
            res = resolve_dereference__CIResolverExpr(rhs);

            break;
        case CI_EXPR_UNARY_KIND_REF:
            res = resolve_ref__CIResolverExpr(rhs);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    FREE(CIExpr, rhs);

    return res;
}

CIExpr *
run__CIResolverExpr(const CIResolverExpr *self, CIExpr *expr)
{
    ASSERT(expr);

    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            return resolve_alignof_expr__CIResolverExpr(self, expr);
        case CI_EXPR_KIND_ARRAY_ACCESS:
            TODO("array access");
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
                    return ref__CIExpr(expr);
                default:
                    break;
            }

            CIExpr *lhs = run__CIResolverExpr(self, expr->binary.left);

            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_AND:
                    if (!is_true__CIResolverExpr(lhs)) {
                        return lhs;
                    }

                    break;
                case CI_EXPR_BINARY_KIND_OR:
                    if (is_true__CIResolverExpr(lhs)) {
                        return lhs;
                    }

                    break;
                default:
                    break;
            }

            CIExpr *rhs = run__CIResolverExpr(self, expr->binary.right);
            CIExpr *res = NULL;

            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_ADD:
                    res = resolve_add_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_SUB:
                    res = resolve_sub_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_MUL:
                    res = resolve_mul_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_DIV:
                    res = resolve_div_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_MOD:
                    res = resolve_mod_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_AND:
                    res = resolve_bit_and_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_OR:
                    res = resolve_bit_or_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_XOR:
                    res = resolve_bit_xor_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
                    res = resolve_bit_lshift_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_BIT_RSHIFT:
                    res = resolve_bit_rshift_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_AND:
                    res = resolve_logical_and_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_OR:
                    res = resolve_logical_or_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_EQ:
                    res = resolve_eq_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_NE:
                    res = resolve_ne_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_LESS:
                    res = resolve_lt_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_LESS_EQ:
                    res = resolve_le_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_GREATER:
                    res = resolve_gt_expr__CIResolverExpr(lhs, rhs);

                    break;
                case CI_EXPR_BINARY_KIND_GREATER_EQ:
                    res = resolve_ge_expr__CIResolverExpr(lhs, rhs);

                    break;
                default:
                    UNREACHABLE("unknown binary kind");
            }

            FREE(CIExpr, lhs);
            FREE(CIExpr, rhs);

            return res;
        }
        case CI_EXPR_KIND_CAST:
            TODO("resolve cast");
        case CI_EXPR_KIND_DATA_TYPE:
            return ref__CIExpr(expr);
        case CI_EXPR_KIND_GROUPING:
            return run__CIResolverExpr(self, expr->grouping);
        case CI_EXPR_KIND_IDENTIFIER:
            if (self->is_at_preprocessor_time) {
                // NOTE: Return 0 by default if the preprocessor encounters an
                // identifier, since this means that the macro named with this
                // identifier has not been defined.
                return NEW_VARIANT(
                  CIExpr, literal, NEW_VARIANT(CIExprLiteral, signed_int, 0));
            }

            TODO("resolve identifier");
        case CI_EXPR_KIND_LITERAL:
            return ref__CIExpr(expr);
        case CI_EXPR_KIND_NULLPTR:
            return ref__CIExpr(expr);
        case CI_EXPR_KIND_SIZEOF:
            return resolve_sizeof_expr__CIResolverExpr(self, expr);
        case CI_EXPR_KIND_STRUCT_CALL:
            TODO("resolve struct");
        case CI_EXPR_KIND_TERNARY: {
            CIExpr *resolved_cond =
              run__CIResolverExpr(self, expr->ternary.cond);

            if (is_true__CIResolverExpr(resolved_cond)) {
                FREE(CIExpr, resolved_cond);

                return run__CIResolverExpr(self, expr->ternary.if_);
            }

            FREE(CIExpr, resolved_cond);

            return run__CIResolverExpr(self, expr->ternary.else_);
        }
        case CI_EXPR_KIND_UNARY:
            return resolve_unary_expr__CIResolverExpr(self, expr);
        default:
            UNREACHABLE("unknown variant");
    }
}
