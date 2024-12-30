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
#include <base/hash/sip.h>
#include <base/new.h>

#include <core/cc/ci/ast.h>
#include <core/cc/ci/builtin.h>
#include <core/cc/ci/resolver/data_type.h>
#include <core/cc/ci/result.h>
#include <core/cc/ci/token.h>

#include <stdio.h>
#include <stdlib.h>

#define EXPR_PRECEDENCE_LEVEL_1 MAX_CI_EXPR_PRECEDENCE
#define EXPR_PRECEDENCE_LEVEL_2 95
#define EXPR_PRECEDENCE_LEVEL_3 90
#define EXPR_PRECEDENCE_LEVEL_4 85
#define EXPR_PRECEDENCE_LEVEL_5 80
#define EXPR_PRECEDENCE_LEVEL_6 75
#define EXPR_PRECEDENCE_LEVEL_7 70
#define EXPR_PRECEDENCE_LEVEL_8 65
#define EXPR_PRECEDENCE_LEVEL_9 60
#define EXPR_PRECEDENCE_LEVEL_10 55
#define EXPR_PRECEDENCE_LEVEL_11 50
#define EXPR_PRECEDENCE_LEVEL_12 45
#define EXPR_PRECEDENCE_LEVEL_13 40
#define EXPR_PRECEDENCE_LEVEL_14 35

static CIDataType *
build_struct_or_union_data_type__CIDeclStructField(
  const CIDeclStructField *self);

static inline bool
has_heap__CIDataTypeContext(int self);

static inline bool
has_non_null__CIDataTypeContext(int self);

static inline bool
has_stack__CIDataTypeContext(int self);

static inline bool
has_trace__CIDataTypeContext(int self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_ARRAY).
static VARIANT_DESTRUCTOR(CIDataType, array, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_BUILTIN).
static inline VARIANT_DESTRUCTOR(CIDataType, builtin, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_ENUM).
static VARIANT_DESTRUCTOR(CIDataType, enum, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_GENERIC).
static VARIANT_DESTRUCTOR(CIDataType, generic, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(CIDataType, ptr, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_STRUCT).
static VARIANT_DESTRUCTOR(CIDataType, struct, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_TYPEDEF).
static VARIANT_DESTRUCTOR(CIDataType, typedef, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_UNION).
static VARIANT_DESTRUCTOR(CIDataType, union, CIDataType *self);

/// @brief Free CIAttributeStandard type
/// (CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED).
static VARIANT_DESTRUCTOR(CIAttributeStandard,
                          deprecated,
                          const CIAttributeStandard *self);

/// @brief Free CIAttributeStandard type (CI_ATTRIBUTE_STANDARD_KIND_NODISCARD).
static VARIANT_DESTRUCTOR(CIAttributeStandard,
                          nodiscard,
                          const CIAttributeStandard *self);

/// @brief Free CIAttribute type (CI_ATTRIBUTE_KIND_STANDARD).
static VARIANT_DESTRUCTOR(CIAttribute, standard, CIAttribute *self);

/// @brief Free CIDeclFUnctionParam type (CI_DECL_FUNCTION_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(CIDeclFunctionParam,
                          normal,
                          CIDeclFunctionParam *self);

/// @brief Free CIDeclFUnctionParam type (CI_DECL_FUNCTION_PARAM_KIND_VARIADIC).
static inline VARIANT_DESTRUCTOR(CIDeclFunctionParam,
                                 variadic,
                                 CIDeclFunctionParam *self);

/// @brief Free CIDeclStructField type
/// (CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT).
static VARIANT_DESTRUCTOR(CIDeclStructField,
                          anonymous_struct,
                          CIDeclStructField *self);

/// @brief Free CIDeclStructField type
/// (CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION).
static VARIANT_DESTRUCTOR(CIDeclStructField,
                          anonymous_union,
                          CIDeclStructField *self);

/// @brief Free CIDeclStructField type (CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT).
static VARIANT_DESTRUCTOR(CIDeclStructField,
                          named_struct,
                          CIDeclStructField *self);

/// @brief Free CIDeclStructField type (CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION).
static VARIANT_DESTRUCTOR(CIDeclStructField,
                          named_union,
                          CIDeclStructField *self);

/// @brief Free CIDeclStructField type (CI_DECL_STRUCT_FIELD_KIND_MEMBER).
static VARIANT_DESTRUCTOR(CIDeclStructField, member, CIDeclStructField *self);

static const CISizeInfo *
get_size_info__CIDecl(const CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_ENUM).
static VARIANT_DESTRUCTOR(CIDecl, enum, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_ENUM_VARIANT).
static VARIANT_DESTRUCTOR(CIDecl, enum_variant, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIDecl, function, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_FUNCTION_GEN).
static VARIANT_DESTRUCTOR(CIDecl, function_gen, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_LABEL).
static VARIANT_DESTRUCTOR(CIDecl, label, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_STRUCT).
static VARIANT_DESTRUCTOR(CIDecl, struct, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_STRUCT_GEN).
static VARIANT_DESTRUCTOR(CIDecl, struct_gen, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_TYPEDEF).
static VARIANT_DESTRUCTOR(CIDecl, typedef, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_TYPEDEF_GEN).
static VARIANT_DESTRUCTOR(CIDecl, typedef_gen, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_UNION).
static VARIANT_DESTRUCTOR(CIDecl, union, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_UNION_GEN).
static VARIANT_DESTRUCTOR(CIDecl, union_gen, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_VARIABLE).
static VARIANT_DESTRUCTOR(CIDecl, variable, CIDecl *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_ALIGNOF).
static VARIANT_DESTRUCTOR(CIExpr, alignof, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_ARRAY_ACCESS).
static VARIANT_DESTRUCTOR(CIExpr, array_access, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_BINARY).
static VARIANT_DESTRUCTOR(CIExpr, binary, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_CAST).
static VARIANT_DESTRUCTOR(CIExpr, cast, CIExpr *self);

/// @brief Free CIExprLiteral (CI_EXPR_LITERAL_KIND_STRING).
static VARIANT_DESTRUCTOR(CIExprLiteral, string, const CIExprLiteral *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_DATA_TYPE).
static VARIANT_DESTRUCTOR(CIExpr, data_type, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_FUNCTION_CALL).
static VARIANT_DESTRUCTOR(CIExpr, function_call, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_FUNCTION_CALL_BUILTIN).
static VARIANT_DESTRUCTOR(CIExpr, function_call_builtin, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_GROUPING).
static VARIANT_DESTRUCTOR(CIExpr, grouping, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_IDENTIFIER).
static VARIANT_DESTRUCTOR(CIExpr, identifier, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_INITIALIZER).
static VARIANT_DESTRUCTOR(CIExpr, initializer, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_LITERAL).
static VARIANT_DESTRUCTOR(CIExpr, literal, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_SIZEOF).
static VARIANT_DESTRUCTOR(CIExpr, sizeof, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_TERNARY).
static VARIANT_DESTRUCTOR(CIExpr, ternary, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_UNARY).
static VARIANT_DESTRUCTOR(CIExpr, unary, CIExpr *self);

/// @brief Free CIStmt type (CI_STMT_KIND_BLOCK).
static inline VARIANT_DESTRUCTOR(CIStmt, block, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_CASE).
static inline VARIANT_DESTRUCTOR(CIStmt, case, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_DO_WHILE).
static inline VARIANT_DESTRUCTOR(CIStmt, do_while, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_FOR).
static inline VARIANT_DESTRUCTOR(CIStmt, for, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_GOTO).
static VARIANT_DESTRUCTOR(CIStmt, goto, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_IF).
static inline VARIANT_DESTRUCTOR(CIStmt, if, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_RETURN).
static VARIANT_DESTRUCTOR(CIStmt, return, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_SWITCH).
static inline VARIANT_DESTRUCTOR(CIStmt, switch, const CIStmt *self);

/// @brief Free CIStmt type (CI_STMT_KIND_WHILE).
static inline VARIANT_DESTRUCTOR(CIStmt, while, const CIStmt *self);

/// @brief Free CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_DECL).
static VARIANT_DESTRUCTOR(CIDeclFunctionItem, decl, CIDeclFunctionItem *self);

/// @brief Free CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_EXPR).
static VARIANT_DESTRUCTOR(CIDeclFunctionItem, expr, CIDeclFunctionItem *self);

/// @brief Free CIDeclFunctionItem type (CI_DECL_FUNCTION_ITEM_KIND_STMT).
static VARIANT_DESTRUCTOR(CIDeclFunctionItem, stmt, CIDeclFunctionItem *self);

static const enum CIDataTypeContext data_type_ctx_ids[] = {
    CI_DATA_TYPE_CONTEXT_NONE,
    CI_DATA_TYPE_CONTEXT_HEAP,
    CI_DATA_TYPE_CONTEXT_NON_NULL,
    CI_DATA_TYPE_CONTEXT_STACK,
    CI_DATA_TYPE_CONTEXT_TRACE
};
static const Usize data_type_ctx_ids_count =
  sizeof(data_type_ctx_ids) / sizeof(*data_type_ctx_ids);

static const enum CIStorageClass storage_class_ids[] = {
    CI_STORAGE_CLASS_NONE,      CI_STORAGE_CLASS_AUTO,
    CI_STORAGE_CLASS_CONSTEXPR, CI_STORAGE_CLASS_EXTERN,
    CI_STORAGE_CLASS_INLINE,    CI_STORAGE_CLASS_REGISTER,
    CI_STORAGE_CLASS_STATIC,    CI_STORAGE_CLASS_THREAD_LOCAL,
    CI_STORAGE_CLASS_TYPEDEF,
};
static const Usize storage_class_ids_count =
  sizeof(storage_class_ids) / sizeof(*storage_class_ids);

static const enum CIDataTypeQualifier data_type_qualifier_ids[] = {
    CI_DATA_TYPE_QUALIFIER_NONE,     CI_DATA_TYPE_QUALIFIER_CONST,
    CI_DATA_TYPE_QUALIFIER_VOLATILE, CI_DATA_TYPE_QUALIFIER_RESTRICT,
    CI_DATA_TYPE_QUALIFIER__ATOMIC,  CI_DATA_TYPE_QUALIFIER__NORETURN
};
static const Usize data_type_qualifier_ids_count =
  sizeof(data_type_qualifier_ids) / sizeof(*data_type_qualifier_ids);

#define CI_SERIALIZE_NAME(name)                                           \
    ASSERT(called_generic_params);                                        \
                                                                          \
    String *res = format__String("{S}__", name);                          \
                                                                          \
    put_serialization_into_buffer__CIGenericParams(called_generic_params, \
                                                   res);                  \
                                                                          \
    return res;

CONSTRUCTOR(CIScopeID *, CIScopeID, Usize id)
{
    CIScopeID *self = lily_malloc(sizeof(CIScopeID));

    self->id = id;

    return self;
}

CONSTRUCTOR(CIEnumID *, CIEnumID, CIFileID file_id, Usize id, Usize decl_id)
{
    CIEnumID *self = lily_malloc(sizeof(CIEnumID));

    self->file_id = file_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CIEnumVariantID *,
            CIEnumVariantID,
            CIFileID file_id,
            Usize id,
            Usize decl_id)
{
    CIEnumVariantID *self = lily_malloc(sizeof(CIEnumVariantID));

    self->file_id = file_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CIStructID *, CIStructID, CIFileID file_id, Usize id, Usize decl_id)
{
    CIStructID *self = lily_malloc(sizeof(CIStructID));

    self->file_id = file_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CITypedefID *,
            CITypedefID,
            CIFileID file_id,
            Usize id,
            Usize decl_id)
{
    CITypedefID *self = lily_malloc(sizeof(CITypedefID));

    self->file_id = file_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CIFunctionID *,
            CIFunctionID,
            CIFileID file_id,
            Usize id,
            Usize decl_id)
{
    CIFunctionID *self = lily_malloc(sizeof(CIFunctionID));

    self->file_id = file_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CILabelID *,
            CILabelID,
            CIFileID file_id,
            CIScopeID scope_id,
            Usize id,
            Usize decl_id)
{
    CILabelID *self = lily_malloc(sizeof(CILabelID));

    self->file_id = file_id;
    self->scope_id = scope_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CIUnionID *, CIUnionID, CIFileID file_id, Usize id, Usize decl_id)
{
    CIUnionID *self = lily_malloc(sizeof(CIUnionID));

    self->file_id = file_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CIVariableID *,
            CIVariableID,
            CIFileID file_id,
            CIScopeID scope_id,
            Usize id,
            Usize decl_id)
{
    CIVariableID *self = lily_malloc(sizeof(CIVariableID));

    self->file_id = file_id;
    self->scope_id = scope_id;
    self->id = id;
    self->decl_id = decl_id;

    return self;
}

CONSTRUCTOR(CIStateCheckerItem *,
            CIStateCheckerItem,
            Uint64 state,
            CIVariableID *variable_id)
{
    CIStateCheckerItem *self = lily_malloc(sizeof(CIStateCheckerItem));

    self->state = state;
    self->variable_id = variable_id;

    return self;
}

DESTRUCTOR(CIStateCheckerItem, CIStateCheckerItem *self)
{
    lily_free(self);
}

CONSTRUCTOR(CIScope *, CIScope, CIScopeID *parent, Usize id, bool is_block)
{
    CIScope *self = lily_malloc(sizeof(CIScope));

    self->parent = parent;
    self->scope_id = NEW(CIScopeID, id);
    self->is_block = is_block;
    self->enums = NEW(HashMap);
    self->enum_variants = NEW(HashMap);
    self->functions = NEW(HashMap);
    self->labels = NEW(HashMap);
    self->structs = NEW(HashMap);
    self->typedefs = NEW(HashMap);
    self->unions = NEW(HashMap);
    self->variables = NEW(HashMap);
    self->states = NEW(HashMap);

    return self;
}

Usize
get_decl_id_from_decl__CIScope(const CIScope *self,
                               const CIResultFile *file,
                               const CIDecl *decl)
{
    const String *name = get_name__CIDecl(decl);

    for (; self; self = self->parent
                          ? get_scope_from_id__CIResultFile(file, self->parent)
                          : NULL) {
        switch (decl->kind) {
            case CI_DECL_KIND_ENUM: {
                const CIEnumID *enum_id = search_enum__CIScope(self, name);

                if (enum_id) {
                    return enum_id->decl_id;
                }

                continue;
            }
            case CI_DECL_KIND_ENUM_VARIANT: {
                const CIEnumVariantID *enum_variant_id =
                  search_enum_variant__CIScope(self, name);

                if (enum_variant_id) {
                    return enum_variant_id->decl_id;
                }

                continue;
            }
            case CI_DECL_KIND_FUNCTION:
            case CI_DECL_KIND_FUNCTION_GEN: {
                const CIFunctionID *function_id =
                  search_function__CIScope(self, name);

                if (function_id) {
                    return function_id->decl_id;
                }

                continue;
            }
            case CI_DECL_KIND_LABEL: {
                const CILabelID *label_id = search_label__CIScope(self, name);

                if (label_id) {
                    return label_id->decl_id;
                }

                continue;
            }
            case CI_DECL_KIND_STRUCT:
            case CI_DECL_KIND_STRUCT_GEN: {
                const CIStructID *struct_id =
                  search_struct__CIScope(self, name);

                if (struct_id) {
                    return struct_id->decl_id;
                }

                continue;
            }
            case CI_DECL_KIND_TYPEDEF:
            case CI_DECL_KIND_TYPEDEF_GEN: {
                const CITypedefID *typedef_id =
                  search_typedef__CIScope(self, name);

                if (typedef_id) {
                    return typedef_id->decl_id;
                }

                continue;
            }
            case CI_DECL_KIND_UNION:
            case CI_DECL_KIND_UNION_GEN: {
                const CIUnionID *union_id = search_union__CIScope(self, name);

                if (union_id) {
                    return union_id->decl_id;
                }

                continue;
            }
            case CI_DECL_KIND_VARIABLE: {
                const CIVariableID *variable_id =
                  search_variable__CIScope(self, name);

                if (variable_id) {
                    return variable_id->decl_id;
                }

                continue;
            }
            default:
                UNREACHABLE("unknown variant");
        }
    }

    UNREACHABLE("cannot found this declaration in any scopes");
}

DESTRUCTOR(CIScope, CIScope *self)
{
    FREE(CIScopeID, self->scope_id);

    FREE_HASHMAP_VALUES(self->enums, CIEnumID);
    FREE(HashMap, self->enums);

    FREE_HASHMAP_VALUES(self->enum_variants, CIEnumVariantID);
    FREE(HashMap, self->enum_variants);

    FREE_HASHMAP_VALUES(self->functions, CIFunctionID);
    FREE(HashMap, self->functions);

    FREE_HASHMAP_VALUES(self->labels, CILabelID);
    FREE(HashMap, self->labels);

    FREE_HASHMAP_VALUES(self->structs, CIStructID);
    FREE(HashMap, self->structs);

    FREE_HASHMAP_VALUES(self->typedefs, CITypedefID);
    FREE(HashMap, self->typedefs);

    FREE_HASHMAP_VALUES(self->unions, CIUnionID);
    FREE(HashMap, self->unions);

    FREE_HASHMAP_VALUES(self->variables, CIVariableID);
    FREE(HashMap, self->variables);

    FREE_HASHMAP_VALUES(self->states, CIStateCheckerItem);
    FREE(HashMap, self->states);

    lily_free(self);
}

void
fill_size__CISizeInfo(CISizeInfo *self, Usize size)
{
    ASSERT(self);

    self->size = size;
}

void
fill_alignment__CISizeInfo(CISizeInfo *self, Usize alignment)
{
    ASSERT(self);

    self->alignment = alignment;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CISizeInfo, const CISizeInfo *self)
{
    return format__String("CISizeInfo{{ size = {zu}, alignment = {zu} }",
                          self->size,
                          self->alignment);
}
#endif

CONSTRUCTOR(CIGenericParams *, CIGenericParams, Vec *params)
{
    CIGenericParams *self = lily_malloc(sizeof(CIGenericParams));

    self->ref_count = 0;
    self->params = params;

    return self;
}

bool
eq__CIGenericParams(const CIGenericParams *self, const CIGenericParams *other)
{
    if (self->params->len != other->params->len) {
        return false;
    }

    for (Usize i = 0; i < self->params->len; ++i) {
        if (!eq__CIDataType(get__Vec(self->params, i),
                            get__Vec(other->params, i))) {
            return false;
        }
    }

    return true;
}

CIGenericParams *
clone__CIGenericParams(const CIGenericParams *self)
{
    Vec *params = NEW(Vec);

    for (Usize i = 0; i < self->params->len; ++i) {
        push__Vec(params, clone__CIDataType(get__Vec(self->params, i)));
    }

    return NEW(CIGenericParams, params);
}

Isize
find_generic__CIGenericParams(const CIGenericParams *self, String *name)
{
    for (Usize i = 0; i < self->params->len; ++i) {
        CIDataType *param = get__Vec(self->params, i);

        switch (param->kind) {
            case CI_DATA_TYPE_KIND_GENERIC:
                if (!strcmp(GET_PTR_RC(String, param->generic)->buffer,
                            name->buffer)) {
                    return i;
                }

                break;
            default:
                break;
        }
    }

    return -1;
}

bool
has_generic__CIGenericParams(const CIGenericParams *self)
{
    if (self) {
        for (Usize i = 0; i < self->params->len; ++i) {
            if (CAST(CIDataType *, get__Vec(self->params, i))->kind ==
                CI_DATA_TYPE_KIND_GENERIC) {
                return true;
            }
        }
    }

    return false;
}

void
put_serialization_into_buffer__CIGenericParams(const CIGenericParams *self,
                                               String *buffer)
{
    serialize_vec__CIDataType(self->params, buffer);
}

String *
serialize_name__CIGenericParams(const CIGenericParams *self, const String *name)
{
    const CIGenericParams *called_generic_params = self;

    CI_SERIALIZE_NAME(name);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIGenericParams, const CIGenericParams *self)
{
    String *res = from__String("CIGenericParams{ params =");

    DEBUG_VEC_STRING(self->params, res, CIDataType);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIGenericParams, CIGenericParams *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, CIDataType);
    FREE(Vec, self->params);
    lily_free(self);
}

CONSTRUCTOR(CIDeclStructFields *, CIDeclStructFields)
{
    CIDeclStructFields *self = lily_malloc(sizeof(CIDeclStructFields));

    self->first = NULL;
    self->last = NULL;
    self->members = NEW(OrderedHashMap);
    self->ref_count = 0;

    return self;
}

bool
add__CIDeclStructFields(CIDeclStructFields *self,
                        struct CIDeclStructField *field,
                        struct CIDeclStructField *prev)
{
    if (prev) {
        prev->next = field;

        ASSERT(field->prev == prev);
    }

    bool is_duplicated = false;

    if (!field->name) {
        goto add_to_linked_list;
    }

    CIDeclStructField *current_parent = field->parent;

    while (true) {
        if (current_parent) {
            switch (current_parent->kind) {
                case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
                case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
                    current_parent = current_parent->parent;

                    continue;
                case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
                case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
                    switch (current_parent->kind) {
                        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
                            is_duplicated = insert__OrderedHashMap(
                              current_parent->named_struct,
                              GET_PTR_RC(String, field->name)->buffer,
                              field);

                            break;
                        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
                            is_duplicated = insert__OrderedHashMap(
                              current_parent->named_union,
                              GET_PTR_RC(String, field->name)->buffer,
                              field);

                            break;
                        default:
                            UNREACHABLE("unknown variant");
                    }

                    break;
                case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
                    UNREACHABLE("impossible to have member as parent");
                default:
                    UNREACHABLE("unknown variant");
            }
        } else {
            is_duplicated = insert__OrderedHashMap(
              self->members, GET_PTR_RC(String, field->name)->buffer, field);
        }

        break;
    }

add_to_linked_list:
    if (!is_duplicated) {
        if (self->first) {
            self->last->next = field;
            self->last = self->last->next;
        } else {
            self->first = field;
            self->last = self->first;
        }

        return true;
    }

    return false;
}

CIDeclStructFields *
clone__CIDeclStructFields(CIDeclStructFields *self)
{
    CIDeclStructFields *cloned_self = NEW(CIDeclStructFields);
    CIDeclStructField *current = self->first;
    CIDeclStructField *cloned_prev = NULL;
    CIDeclStructField *cloned_parent = NULL;

    while (current) {
        CIDeclStructField *cloned_current = clone__CIDeclStructField(current);

        cloned_current->prev = cloned_prev;
        cloned_current->parent = cloned_parent;

        ASSERT(
          add__CIDeclStructFields(cloned_self, cloned_current, cloned_prev));

        switch (cloned_current->kind) {
            case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
            case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
            case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
            case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
                cloned_parent = cloned_current;

                break;
            case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
                break;
            default:
                UNREACHABLE("unknown variant");
        }

        cloned_prev = cloned_current;
        current = current->next;
    }

    return cloned_self;
}

struct CIDeclStructField *
get_field_from_path__CIDeclStructFields(
  const CIDeclStructFields *self,
  const Vec *path,
  const CIResultFile *file,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDeclStructField *current_field = NULL;

    for (Usize i = 0; i < path->len; ++i) {
        Rc *path_part = get__Vec(path, i); // Rc<String*>* (&)
        const OrderedHashMap *members =
          current_field
            ? get_members__CIDeclStructField(current_field)
            : self
                ->members; // const OrderedHashMap<CIDeclStructField* (&)>*? (&)

        if (!members && i > 0) {
            CIDataType *current_field_dt =
              build_data_type__CIDeclStructField(current_field);

            const CIDeclStructFields *current_field_fields =
              get_fields_from_data_type__CIResolverDataType(
                file,
                current_field_dt,
                called_generic_params,
                decl_generic_params);

            FREE(CIDataType, current_field_dt);

            if (current_field_fields) {
                members = current_field_fields->members;
            } else {
                return NULL;
            }
        }

        if (members) {
            current_field = get__OrderedHashMap(
              (OrderedHashMap *)members, GET_PTR_RC(String, path_part)->buffer);

            if (!current_field) {
                return NULL;
            }
        } else {
            return NULL;
        }
    }

    return current_field;
}

bool
has_generic__CIDeclStructFields(const CIDeclStructFields *self)
{
    for (CIDeclStructField *current = self->first; current;
         current = current->next) {
        switch (current->kind) {
            case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
                switch (current->member.data_type->kind) {
                    case CI_DATA_TYPE_KIND_GENERIC:
                        return true;
                    default: {
                        const CIGenericParams *generic_params =
                          get_generic_params__CIDataType(
                            current->member.data_type);

                        if (generic_params &&
                            has_generic__CIGenericParams(generic_params)) {
                            return true;
                        }
                    }
                }

                break;
            default:
                break;
        }
    }

    return false;
}

bool
eq__CIDeclStructFields(const CIDeclStructFields *self,
                       const CIDeclStructFields *other)
{
    CIDeclStructField *current_self = self->first;
    CIDeclStructField *current_other = other->first;

    for (; current_self && current_other; current_self = current_self->next,
                                          current_other = current_other->next) {
        if (!eq__CIDeclStructField(current_self, current_other)) {
            return false;
        }
    }

    return !current_self && !current_other;
}

struct CIDeclStructField *
get_first_named_member__CIDeclStructFields(const CIDeclStructFields *self)
{
    for (CIDeclStructField *current = self->first; current;
         current = current->next) {
        switch (current->kind) {
            case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
            case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
                return current;
            default:
                continue;
        }
    }

    return NULL;
}

struct CIDeclStructField *
get_first_member__CIDeclStructFields(const CIDeclStructFields *self)
{
    CIDeclStructField *current = self->first;

    while (current) {
        switch (current->kind) {
            case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
                return current;
            default:
                break;
        }

        current = current->next;
    }

    return NULL;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStructFields, const CIDeclStructFields *self)
{
    return format__String(
      "CIDeclStructFields{{ first = {Sr}, last = {Sr} }",
      self->first ? to_string__Debug__CIDeclStructField(self->first)
                  : from__String("NULL"),
      self->last ? to_string__Debug__CIDeclStructField(self->last)
                 : from__String("NULL"));
}
#endif

DESTRUCTOR(CIDeclStructFields, CIDeclStructFields *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    CIDeclStructField *current = self->first;

    while (current) {
        CIDeclStructField *prev = current;

        current = current->next;

        FREE(CIDeclStructField, prev);
    }

    FREE(OrderedHashMap, self->members);

    lily_free(self);
}

bool
eq__CIDeclStructFieldMember(const CIDeclStructFieldMember *self,
                            const CIDeclStructFieldMember *other)
{
    return eq__CIDataType(self->data_type, other->data_type) &&
           self->bit == other->bit;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CIDeclStructFieldMember,
               const CIDeclStructFieldMember *self)
{
    return format__String(
      "CIDeclStructFieldMember{{ data_type = {Sr}, bit = {d} }",
      to_string__Debug__CIDataType(self->data_type),
      self->bit);
}
#endif

DESTRUCTOR(CIDeclStructFieldMember, const CIDeclStructFieldMember *self)
{
    FREE(CIDataType, self->data_type);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclStructFieldKind,
               enum CIDeclStructFieldKind self)
{
    switch (self) {
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
            return "CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT";
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
            return "CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION";
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
            return "CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT";
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
            return "CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION";
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            return "CI_DECL_STRUCT_FIELD_KIND_MEMBER";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(CIDeclStructField *,
                    CIDeclStructField,
                    anonymous_struct,
                    struct CIDeclStructField *parent,
                    struct CIDeclStructField *prev)
{
    CIDeclStructField *self = lily_malloc(sizeof(CIDeclStructField));

    self->name = NULL;
    self->kind = CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT;
    self->ref_count = 0;
    self->parent = parent;
    self->next = NULL;
    self->prev = prev;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclStructField *,
                    CIDeclStructField,
                    anonymous_union,
                    struct CIDeclStructField *parent,
                    struct CIDeclStructField *prev)
{
    CIDeclStructField *self = lily_malloc(sizeof(CIDeclStructField));

    self->name = NULL;
    self->kind = CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION;
    self->ref_count = 0;
    self->parent = parent;
    self->next = NULL;
    self->prev = prev;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclStructField *,
                    CIDeclStructField,
                    named_struct,
                    Rc *name,
                    struct CIDeclStructField *parent,
                    struct CIDeclStructField *prev)
{
    CIDeclStructField *self = lily_malloc(sizeof(CIDeclStructField));

    self->name = ref__Rc(name);
    self->kind = CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT;
    self->ref_count = 0;
    self->parent = parent;
    self->next = NULL;
    self->prev = prev;
    self->named_struct = NEW(OrderedHashMap);

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclStructField *,
                    CIDeclStructField,
                    named_union,
                    Rc *name,
                    struct CIDeclStructField *parent,
                    struct CIDeclStructField *prev)
{
    CIDeclStructField *self = lily_malloc(sizeof(CIDeclStructField));

    self->name = ref__Rc(name);
    self->kind = CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION;
    self->ref_count = 0;
    self->parent = parent;
    self->next = NULL;
    self->prev = prev;
    self->named_union = NEW(OrderedHashMap);

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclStructField *,
                    CIDeclStructField,
                    member,
                    Rc *name,
                    struct CIDeclStructField *parent,
                    struct CIDeclStructField *prev,
                    CIDeclStructFieldMember member)
{
    CIDeclStructField *self = lily_malloc(sizeof(CIDeclStructField));

    self->name = ref__Rc(name);
    self->kind = CI_DECL_STRUCT_FIELD_KIND_MEMBER;
    self->ref_count = 0;
    self->parent = parent;
    self->next = NULL;
    self->prev = prev;
    self->member = member;

    return self;
}

const OrderedHashMap *
get_members__CIDeclStructField(const CIDeclStructField *self)
{
    switch (self->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
            return self->named_struct;
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
            return self->named_union;
        default:
            return NULL;
    }
}

CIDeclStructField *
clone__CIDeclStructField(CIDeclStructField *self)
{
    switch (self->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
            return NEW_VARIANT(
              CIDeclStructField, anonymous_struct, self->parent, self->prev);
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
            return NEW_VARIANT(
              CIDeclStructField, anonymous_union, self->parent, self->prev);
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
            return NEW_VARIANT(CIDeclStructField,
                               named_struct,
                               self->name,
                               self->parent,
                               self->prev);
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
            return NEW_VARIANT(CIDeclStructField,
                               named_union,
                               self->name,
                               self->parent,
                               self->prev);
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            return NEW_VARIANT(CIDeclStructField,
                               member,
                               self->name,
                               self->parent,
                               self->prev,
                               NEW(CIDeclStructFieldMember,
                                   clone__CIDataType(self->member.data_type),
                                   self->member.bit));
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
eq__CIDeclStructField(const CIDeclStructField *self,
                      const CIDeclStructField *other)
{
    if (self->kind != other->kind) {
        return false;
    }

    if (!((self->name && other->name &&
           !strcmp(GET_PTR_RC(String, self->name)->buffer,
                   GET_PTR_RC(String, other->name)->buffer)) ||
          (!self->name && !other->name))) {
        return false;
    }

    switch (self->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
            return true;
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            return eq__CIDeclStructFieldMember(&self->member, &other->member);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIDataType *
build_struct_or_union_data_type__CIDeclStructField(
  const CIDeclStructField *self)
{
    ASSERT(self->kind != CI_DECL_STRUCT_FIELD_KIND_MEMBER);

    const CIDeclStructField *parent_master = self;
    CIDeclStructField *current_field = self->next;
    CIDeclStructField *prev_cloned_field = NULL;
    CIDeclStructField *current_parent = NULL;
    CIDeclStructField *current_cloned_parent = NULL;
    CIDeclStructFields *fields = NEW(CIDeclStructFields);

    while (current_field && has_parent_by_addr__CIDeclStructField(
                              current_field, parent_master)) {
        // Downgrade parent if needed
        while (
          current_parent && current_field->parent != current_parent &&
          current_field->parent !=
            parent_master /* Equivalent to: `&& current_cloned_parent` */) {
            current_parent = current_parent->parent;
            current_cloned_parent = current_cloned_parent->parent;
        }

        CIDeclStructField *cloned_field =
          clone__CIDeclStructField(current_field);

        cloned_field->parent = current_cloned_parent;
        cloned_field->prev = prev_cloned_field;

        ASSERT(
          add__CIDeclStructFields(fields, cloned_field, prev_cloned_field));

        if (cloned_field->kind != CI_DECL_STRUCT_FIELD_KIND_MEMBER) {
            current_parent = current_field;
            current_cloned_parent = cloned_field;
        }

        prev_cloned_field = cloned_field;
        current_field = current_field->next;
    }

    return parent_master->kind == CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT ||
               parent_master->kind == CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT
             ? NEW_VARIANT(
                 CIDataType, struct, NEW(CIDataTypeStruct, NULL, NULL, fields))
             : NEW_VARIANT(
                 CIDataType, union, NEW(CIDataTypeUnion, NULL, NULL, fields));
}

CIDataType *
build_data_type__CIDeclStructField(const CIDeclStructField *self)
{
    switch (self->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
            return build_struct_or_union_data_type__CIDeclStructField(self);
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            return ref__CIDataType(self->member.data_type);
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
has_parent_by_addr__CIDeclStructField(const CIDeclStructField *self,
                                      const CIDeclStructField *parent)
{
    CIDeclStructField *current_parent = self->parent;

    while (current_parent) {
        if (current_parent == parent) {
            return true;
        }

        current_parent = current_parent->parent;
    }

    return false;
}

CIDeclStructField *
get_next_field_with_no_parent__CIDeclStructField(const CIDeclStructField *self)
{
    ASSERT(self);

    CIDeclStructField *current = self->next;

    while (current && current->parent) {
        current = current->next;
    }

    return current;
}

Usize
get_bit__CIDeclStructField(const CIDeclStructField *self)
{
    switch (self->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            return self->member.bit;
        default:
            return 0;
    }
}

CIDeclStructField *
get_next_member__CIDeclStructField(const CIDeclStructField *self)
{
    ASSERT(self);

    CIDeclStructField *current = self->next;

    while (current) {
        switch (current->kind) {
            case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
                return current;
            default:
                break;
        }

        current = current->next;
    }

    return NULL;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStructField, const CIDeclStructField *self)
{
    switch (self->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
            return format__String(
              "CIDeclStructField{{ name = {s}, kind = {s} }",
              self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL",
              to_string__Debug__CIDeclStructFieldKind(self->kind));
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            return format__String(
              "CIDeclStructField{{ name = {s}, kind = {s}, member = {Sr} }",
              GET_PTR_RC(String, self->name)->buffer,
              to_string__Debug__CIDeclStructFieldKind(self->kind),
              to_string__Debug__CIDeclStructFieldMember(&self->member));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIDeclStructField, anonymous_struct, CIDeclStructField *self)
{
    ASSERT(!self->name);

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclStructField, anonymous_union, CIDeclStructField *self)
{
    ASSERT(!self->name);

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclStructField, named_struct, CIDeclStructField *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    FREE(OrderedHashMap, self->named_struct);

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclStructField, named_union, CIDeclStructField *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    FREE(OrderedHashMap, self->named_union);

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclStructField, member, CIDeclStructField *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    FREE(CIDeclStructFieldMember, &self->member);

    lily_free(self);
}

DESTRUCTOR(CIDeclStructField, CIDeclStructField *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
            FREE_VARIANT(CIDeclStructField, anonymous_struct, self);

            break;
        case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
            FREE_VARIANT(CIDeclStructField, anonymous_union, self);

            break;
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
            FREE_VARIANT(CIDeclStructField, named_struct, self);

            break;
        case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
            FREE_VARIANT(CIDeclStructField, named_union, self);

            break;
        case CI_DECL_STRUCT_FIELD_KIND_MEMBER:
            FREE_VARIANT(CIDeclStructField, member, self);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
has_heap__CIDataTypeContext(int self)
{
    return self & CI_DATA_TYPE_CONTEXT_HEAP;
}

bool
has_non_null__CIDataTypeContext(int self)
{
    return self & CI_DATA_TYPE_CONTEXT_NON_NULL;
}

bool
has_stack__CIDataTypeContext(int self)
{
    return self & CI_DATA_TYPE_CONTEXT_STACK;
}

bool
has_trace__CIDataTypeContext(int self)
{
    return self & CI_DATA_TYPE_CONTEXT_TRACE;
}

bool
is_compatible__CIDataTypeContext(int self, int other)
{
#define HAS_HEAP2() \
    (has_heap__CIDataTypeContext(self) && has_heap__CIDataTypeContext(other))
#define HAS_NON_NULL2()                       \
    (has_non_null__CIDataTypeContext(self) && \
     has_non_null__CIDataTypeContext(other))
#define HAS_STACK2() \
    (has_stack__CIDataTypeContext(self) && has_stack__CIDataTypeContext(other))
#define HAS_TRACE2() \
    (has_trace__CIDataTypeContext(self) && has_trace__CIDataTypeContext(other))

    // Incompatible (vice-versa):
    //
    // !heap v. !stack
    // !non_null v. <nothing>
    // !trace v. <nothing>
    // !non_null v. <nothing>
    // !heap v. <nothing>
    // !stack v. <nothing>

    if (HAS_HEAP2() && HAS_STACK2()) {
        return false;
    } else if (HAS_NON_NULL2() && !HAS_NON_NULL2()) {
        return false;
    } else if (!HAS_TRACE2()) {
        return false;
    }

    return true;

#undef HAS_HEAP2
#undef HAS_NON_NULL2
#undef HAS_STACK2
#undef HAS_TRACE2
}

CIDataTypeContextIds
get_ids__CIDataTypeContext()
{
    return (struct CIDataTypeContextIds){ .buffer = data_type_ctx_ids,
                                          .len = data_type_ctx_ids_count };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeContext, int self)
{
    static char *data_type_ctxs[] = {
        "CI_DATA_TYPE_CONTEXT_NONE",     "CI_DATA_TYPE_CONTEXT_HEAP",
        "CI_DATA_TYPE_CONTEXT_NON_NULL", "CI_DATA_TYPE_CONTEXT_STACK",
        "CI_DATA_TYPE_CONTEXT_TRACE",
    };

    static_assert(data_type_ctx_ids_count ==
                    sizeof(data_type_ctxs) / sizeof(*data_type_ctxs),
                  "number of items don't match");

    String *res = NEW(String);

    for (Usize i = 0; i < data_type_ctx_ids_count; ++i) {
        if (self & data_type_ctx_ids[i]) {
            push_str__String(res, data_type_ctxs[i]);
            push__String(res, ' ');
        }
    }

#undef CTXS_LEN

    return res;
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDataTypeKind, enum CIDataTypeKind self)
{
    switch (self) {
        case CI_DATA_TYPE_KIND_ANY:
            return "CI_DATA_TYPE_KIND_ANY";
        case CI_DATA_TYPE_KIND_ARRAY:
            return "CI_DATA_TYPE_KIND_ARRAY";
        case CI_DATA_TYPE_KIND_BOOL:
            return "CI_DATA_TYPE_KIND_BOOL";
        case CI_DATA_TYPE_KIND_BUILTIN:
            return "CI_DATA_TYPE_KIND_BUILTIN";
        case CI_DATA_TYPE_KIND_CHAR:
            return "CI_DATA_TYPE_KIND_CHAR";
        case CI_DATA_TYPE_KIND_DOUBLE:
            return "CI_DATA_TYPE_KIND_DOUBLE";
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
            return "CI_DATA_TYPE_KIND_DOUBLE__COMPLEX";
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
            return "CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY";
        case CI_DATA_TYPE_KIND__DECIMAL32:
            return "CI_DATA_TYPE_KIND__DECIMAL32";
        case CI_DATA_TYPE_KIND__DECIMAL64:
            return "CI_DATA_TYPE_KIND__DECIMAL64";
        case CI_DATA_TYPE_KIND__DECIMAL128:
            return "CI_DATA_TYPE_KIND__DECIMAL128";
        case CI_DATA_TYPE_KIND_ENUM:
            return "CI_DATA_TYPE_KIND_ENUM";
        case CI_DATA_TYPE_KIND_FLOAT:
            return "CI_DATA_TYPE_KIND_FLOAT";
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
            return "CI_DATA_TYPE_KIND_FLOAT__COMPLEX";
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
            return "CI_DATA_TYPE_KIND_FLOAT__IMAGINARY";
        case CI_DATA_TYPE_KIND_FUNCTION:
            return "CI_DATA_TYPE_KIND_FUNCTION";
        case CI_DATA_TYPE_KIND_GENERIC:
            return "CI_DATA_TYPE_KIND_GENERIC";
        case CI_DATA_TYPE_KIND_INT:
            return "CI_DATA_TYPE_KIND_INT";
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
            return "CI_DATA_TYPE_KIND_LONG_DOUBLE";
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
            return "CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX";
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            return "CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY";
        case CI_DATA_TYPE_KIND_LONG_INT:
            return "CI_DATA_TYPE_KIND_LONG_INT";
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
            return "CI_DATA_TYPE_KIND_LONG_LONG_INT";
        case CI_DATA_TYPE_KIND_PTR:
            return "CI_DATA_TYPE_KIND_PTR";
        case CI_DATA_TYPE_KIND_SHORT_INT:
            return "CI_DATA_TYPE_KIND_SHORT_INT";
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
            return "CI_DATA_TYPE_KIND_SIGNED_CHAR";
        case CI_DATA_TYPE_KIND_STRUCT:
            return "CI_DATA_TYPE_KIND_STRUCT";
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return "CI_DATA_TYPE_KIND_TYPEDEF";
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_INT";
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
            return "CI_DATA_TYPE_KIND_UNSIGNED_CHAR";
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT";
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT";
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            return "CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT";
        case CI_DATA_TYPE_KIND_UNION:
            return "CI_DATA_TYPE_KIND_UNION";
        case CI_DATA_TYPE_KIND_VOID:
            return "CI_DATA_TYPE_KIND_VOID";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDataTypeArrayKind, enum CIDataTypeArrayKind self)
{
    switch (self) {
        case CI_DATA_TYPE_ARRAY_KIND_SIZED:
            return "CI_DATA_TYPE_ARRAY_KIND_SIZED";
        case CI_DATA_TYPE_ARRAY_KIND_NONE:
            return "CI_DATA_TYPE_ARRAY_KIND_NONE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeArray, const CIDataTypeArray *self)
{
    return self->kind == CI_DATA_TYPE_ARRAY_KIND_SIZED
             ? format__String(
                 "CIDataTypeArray{{ kind = {s}, data_type = {Sr}, name = {s}, "
                 "size = {zu} }",
                 to_string__Debug__CIDataTypeArrayKind(self->kind),
                 to_string__Debug__CIDataType(self->data_type),
                 self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL",
                 self->size)
             : format__String(
                 "CIDataTypeArray{{ kind = {s}, data_type = {Sr}, name = {s} }",
                 to_string__Debug__CIDataTypeArrayKind(self->kind),
                 to_string__Debug__CIDataType(self->data_type),
                 self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL");
}
#endif

DESTRUCTOR(CIDataTypeArray, const CIDataTypeArray *self)
{
    FREE(CIDataType, self->data_type);

    if (self->name) {
        FREE_RC(String, self->name);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeEnum, const CIDataTypeEnum *self)
{
    String *res = format__String(
      "CIDataTypeEnum{{ name = {s}, variants =",
      self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL");

    if (self->variants) {
        DEBUG_VEC_STRING(self->variants, res, CIDeclEnumVariant);
    } else {
        push_str__String(res, " NULL");
    }

    {
        String *s = format__String(
          ", data_type = {Sr} }",
          self->data_type ? to_string__Debug__CIDataType(self->data_type)
                          : from__String("NULL"));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIDataTypeEnum, const CIDataTypeEnum *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    if (self->variants) {
        FREE_BUFFER_ITEMS(
          self->variants->buffer, self->variants->len, CIDeclEnumVariant);
        FREE(Vec, self->variants);
    }

    if (self->data_type) {
        FREE(CIDataType, self->data_type);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeFunction, const CIDataTypeFunction *self)
{
    String *res = format__String(
      "CIDataTypeFunction{{ name = {s}, params =",
      self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, CIDeclFunctionParam);
    } else {
        push_str__String(res, " NULL");
    }

    {
        String *s = format__String(
          ", return_data_type = {Sr}, function_data_type = {Sr} }",
          to_string__Debug__CIDataType(self->return_data_type),
          self->function_data_type
            ? to_string__Debug__CIDataType(self->function_data_type)
            : from__String("NULL"));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIDataTypeFunction, const CIDataTypeFunction *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, CIDeclFunctionParam);
        FREE(Vec, self->params);
    }

    FREE(CIDataType, self->return_data_type);

    if (self->function_data_type) {
        FREE(CIDataType, self->function_data_type);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeStruct, const CIDataTypeStruct *self)
{
    return format__String(
      "CIDataTypeStruct{{ name = {s}, generic_params = {Sr}, fields = {Sr} }",
      self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL",
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"),
      self->fields ? to_string__Debug__CIDeclStructFields(self->fields)
                   : from__String("NULL"));
}
#endif

DESTRUCTOR(CIDataTypeStruct, const CIDataTypeStruct *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }

    if (self->fields) {
        FREE(CIDeclStructFields, self->fields);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeTypedef, const CIDataTypeTypedef *self)
{
    return format__String(
      "CIDataTypeTypedef{{ name = {S}, generic_params = {Sr} }",
      GET_PTR_RC(String, self->name),
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"));
}
#endif

DESTRUCTOR(CIDataTypeTypedef, const CIDataTypeTypedef *self)
{
    FREE_RC(String, self->name);

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeUnion, const CIDataTypeUnion *self)
{
    return format__String(
      "CIDataTypeUnion{{ name = {s}, generic_params = {Sr}, fields = {Sr} }",
      self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL",
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"),
      self->fields ? to_string__Debug__CIDeclStructFields(self->fields)
                   : from__String("NULL"));
}
#endif

DESTRUCTOR(CIDataTypeUnion, const CIDataTypeUnion *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }

    if (self->fields) {
        FREE(CIDeclStructFields, self->fields);
    }
}

String *
to_string__CIDataTypeQualifier(int data_type_qualifier_flag)
{
    static char *data_type_qualifiers[] = {
        "", "const", "volatile", "restrict", "_Atomic", "_Noreturn"
    };
    static_assert(data_type_qualifier_ids_count ==
                    sizeof(data_type_qualifiers) /
                      sizeof(*data_type_qualifiers),
                  "number of items don't match");

    String *res = NEW(String);

    for (Usize i = 0;
         i < data_type_qualifier_ids_count && data_type_qualifier_flag > 0;
         ++i) {
        if (data_type_qualifier_flag & data_type_qualifier_ids[i]) {
            push_str__String(res, data_type_qualifiers[i]);

            data_type_qualifier_flag &= ~data_type_qualifier_ids[i];

            if (data_type_qualifier_flag != CI_DATA_TYPE_QUALIFIER_NONE) {
                push__String(res, ' ');
            }
        }
    }

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeQualifier, int data_type_qualifier_flag)
{
    static char *data_type_qualifiers[] = {
        "CI_STORAGE_CLASS_NONE",           "CI_DATA_TYPE_QUALIFIER_CONST",
        "CI_DATA_TYPE_QUALIFIER_VOLATILE", "CI_DATA_TYPE_QUALIFIER_RESTRICT",
        "CI_DATA_TYPE_QUALIFIER__ATOMIC",  "CI_DATA_TYPE_QUALIFIER__NORETURN"
    };
    static_assert(data_type_qualifier_ids_count ==
                    sizeof(data_type_qualifiers) /
                      sizeof(*data_type_qualifiers),
                  "number of items don't match");

    String *res = data_type_qualifier_flag == 0
                    ? from__String(data_type_qualifiers[CI_STORAGE_CLASS_NONE])
                    : NEW(String);

    for (Usize i = 0;
         i < data_type_qualifier_ids_count && data_type_qualifier_flag > 0;
         ++i) {
        if (data_type_qualifier_flag & data_type_qualifier_ids[i]) {
            push_str__String(res, data_type_qualifiers[i]);

            data_type_qualifier_flag &= ~data_type_qualifier_ids[i];

            if (data_type_qualifier_flag != 0) {
                push_str__String(res, " | ");
            }
        }
    }

    return res;
}
#endif

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, array, CIDataTypeArray array)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_ARRAY;
    self->ref_count = 0;
    self->array = array;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, builtin, Usize builtin)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_BUILTIN;
    self->ref_count = 0;
    self->builtin = builtin;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, enum, CIDataTypeEnum enum_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_ENUM;
    self->ref_count = 0;
    self->enum_ = enum_;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *,
                    CIDataType,
                    function,
                    CIDataTypeFunction function)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_FUNCTION;
    self->ref_count = 0;
    self->function = function;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, generic, Rc *generic)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_GENERIC;
    self->ref_count = 0;
    self->generic = ref__Rc(generic);
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, ptr, CIDataType *ptr)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_PTR;
    self->ref_count = 0;
    self->ptr = ptr;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, struct, CIDataTypeStruct struct_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_STRUCT;
    self->ref_count = 0;
    self->struct_ = struct_;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *,
                    CIDataType,
                    typedef,
                    CIDataTypeTypedef typedef_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_TYPEDEF;
    self->ref_count = 0;
    self->typedef_ = typedef_;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, union, CIDataTypeUnion union_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_UNION;
    self->ref_count = 0;
    self->union_ = union_;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

CONSTRUCTOR(CIDataType *, CIDataType, enum CIDataTypeKind kind)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = kind;
    self->ref_count = 0;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;
    self->qualifier = CI_DATA_TYPE_QUALIFIER_NONE;

    return self;
}

CIDataType *
clone__CIDataType(const CIDataType *self)
{
    CIDataType *res = NULL;

    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            switch (self->array.kind) {
                case CI_DATA_TYPE_ARRAY_KIND_NONE:
                    res = NEW_VARIANT(
                      CIDataType,
                      array,
                      NEW_VARIANT(CIDataTypeArray,
                                  none,
                                  clone__CIDataType(self->array.data_type),
                                  self->array.name));

                    break;
                case CI_DATA_TYPE_ARRAY_KIND_SIZED:
                    res = NEW_VARIANT(
                      CIDataType,
                      array,
                      NEW_VARIANT(CIDataTypeArray,
                                  sized,
                                  clone__CIDataType(self->array.data_type),
                                  self->array.name,
                                  self->array.size));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        case CI_DATA_TYPE_KIND_BUILTIN:
            res = NEW_VARIANT(CIDataType, builtin, self->builtin);

            break;
        case CI_DATA_TYPE_KIND_ENUM: {
            Vec *variants = NULL;

            if (self->enum_.variants) {
                variants = NEW(Vec);

                for (Usize i = 0; i < self->enum_.variants->len; ++i) {
                    push__Vec(variants,
                              clone__CIDeclEnumVariant(
                                get__Vec(self->enum_.variants, i)));
                }
            }

            res = NEW_VARIANT(CIDataType,
                              enum,
                              NEW(CIDataTypeEnum,
                                  self->enum_.name,
                                  variants,
                                  self->enum_.data_type
                                    ? clone__CIDataType(self->enum_.data_type)
                                    : NULL));

            break;
        }
        case CI_DATA_TYPE_KIND_FUNCTION: {
            Vec *params = NEW(Vec); // Vec<CIDataType*>*

            for (Usize i = 0; i < self->function.params->len; ++i) {
                push__Vec(
                  params,
                  clone__CIDataType(get__Vec(self->function.params, i)));
            }

            res = NEW_VARIANT(
              CIDataType,
              function,
              NEW(CIDataTypeFunction,
                  self->function.name,
                  params,
                  clone__CIDataType(self->function.return_data_type),
                  clone__CIDataType(self->function.function_data_type)));

            break;
        }
        case CI_DATA_TYPE_KIND_GENERIC:
            res = NEW_VARIANT(CIDataType, generic, self->generic);

            break;
        case CI_DATA_TYPE_KIND_PTR:
            res = NEW_VARIANT(
              CIDataType, ptr, self->ptr ? clone__CIDataType(self->ptr) : NULL);

            break;
        case CI_DATA_TYPE_KIND_STRUCT: {
            CIDeclStructFields *fields = NULL;

            if (self->struct_.fields) {
                fields = clone__CIDeclStructFields(self->struct_.fields);
            }

            res = NEW_VARIANT(
              CIDataType,
              struct,
              NEW(CIDataTypeStruct,
                  self->struct_.name,
                  self->struct_.generic_params
                    ? clone__CIGenericParams(self->struct_.generic_params)
                    : NULL,
                  fields));

            break;
        }
        case CI_DATA_TYPE_KIND_TYPEDEF:
            res = NEW_VARIANT(
              CIDataType,
              typedef,
              NEW(CIDataTypeTypedef,
                  self->typedef_.name,
                  self->typedef_.generic_params
                    ? clone__CIGenericParams(self->typedef_.generic_params)
                    : NULL));

            break;
        case CI_DATA_TYPE_KIND_UNION: {
            CIDeclStructFields *fields = NULL;

            if (self->union_.fields) {
                fields = clone__CIDeclStructFields(self->union_.fields);
            }

            res = NEW_VARIANT(
              CIDataType,
              union,
              NEW(CIDataTypeUnion,
                  self->union_.name,
                  self->union_.generic_params
                    ? clone__CIGenericParams(self->union_.generic_params)
                    : NULL,
                  fields));

            break;
        }
        default:
            res = NEW(CIDataType, self->kind);
    }

    set_context__CIDataType(res, self->ctx);
    set_qualifier__CIDataType(res, self->qualifier);

    return res;
}

// FIXME: With the technique currently used for type serialization, there is a
// risk of name collision. A simple way of reducing this risk would be to
// generate a random hash each time the program is run, and use it as a prefix
// for serialization.
void
serialize__CIDataType(const CIDataType *self, String *buffer)
{
#define SERIALIZE_NAME(name, name_len) hash_sip(name, name_len, SIP_K0, SIP_K1)

#define SERIALIZE_TYPE_WITH_GENERIC_PARAMS(ty)                                \
    {                                                                         \
        push_str__String(buffer, GET_PTR_RC(String, ty.name)->buffer);        \
                                                                              \
        if (ty.generic_params) {                                              \
            put_serialization_into_buffer__CIGenericParams(ty.generic_params, \
                                                           buffer);           \
        }                                                                     \
    }

#define SERIALIZE_FMT_PUSH_TO_BUFFER(fmt, ...) \
    {                                          \
        char *s = format(fmt, __VA_ARGS__);    \
        PUSH_STR_AND_FREE(buffer, s);          \
    }

    SERIALIZE_FMT_PUSH_TO_BUFFER("{d}", self->kind);

    if (self->qualifier != CI_DATA_TYPE_QUALIFIER_NONE) {
        SERIALIZE_FMT_PUSH_TO_BUFFER("{d}", self->qualifier);
    }

    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            serialize__CIDataType(self->array.data_type, buffer);

            break;
        case CI_DATA_TYPE_KIND_BUILTIN:
            SERIALIZE_FMT_PUSH_TO_BUFFER("{zu}", self->builtin);

            break;
        case CI_DATA_TYPE_KIND_ENUM:
            push_str__String(buffer,
                             GET_PTR_RC(String, self->enum_.name)->buffer);

            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            serialize__CIDataType(self->function.return_data_type, buffer);
            serialize_vec__CIDataType(self->function.params, buffer);

            break;
        case CI_DATA_TYPE_KIND_GENERIC:
            SERIALIZE_FMT_PUSH_TO_BUFFER(
              "{zu}",
              SERIALIZE_NAME(GET_PTR_RC(String, self->generic)->buffer,
                             GET_PTR_RC(String, self->generic)->len));

            break;
        case CI_DATA_TYPE_KIND_PTR:
            if (self->ptr) {
                serialize__CIDataType(self->ptr, buffer);
            }

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            SERIALIZE_TYPE_WITH_GENERIC_PARAMS(self->struct_);

            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            SERIALIZE_TYPE_WITH_GENERIC_PARAMS(self->typedef_);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            SERIALIZE_TYPE_WITH_GENERIC_PARAMS(self->union_);

            break;
        case CI_DATA_TYPE_KIND_ANY:
        case CI_DATA_TYPE_KIND_TYPE_INFO:
            UNREACHABLE("these types couldn't been called in this context");
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

#undef SERIALIZE_NAME
#undef SERIALIZE_TYPE_WITH_GENERIC_PARAMS
#undef SERIALIZE_FMT_PUSH_TO_BUFFER
}

void
serialize_vec__CIDataType(const Vec *data_types, String *buffer)
{
    for (Usize i = 0; i < data_types->len; ++i) {
        serialize__CIDataType(get__Vec(data_types, i), buffer);
    }
}

bool
eq__CIDataType(const CIDataType *self, const CIDataType *other)
{
    if (self->kind == CI_DATA_TYPE_KIND_ANY ||
        other->kind == CI_DATA_TYPE_KIND_ANY) {
        return true;
    } else if (self->kind != other->kind || self->ctx != other->ctx ||
               self->qualifier != other->qualifier) {
        return false;
    }

    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ANY:
            UNREACHABLE("unexpected variant at this point");
        case CI_DATA_TYPE_KIND_ARRAY:
            if (self->array.kind != other->array.kind ||
                !eq__CIDataType(self->array.data_type,
                                other->array.data_type)) {
                return false;
            }

            switch (self->array.kind) {
                case CI_DATA_TYPE_ARRAY_KIND_SIZED:
                    return self->array.size == other->array.size;
                case CI_DATA_TYPE_ARRAY_KIND_NONE:
                    return false;
                default:
                    UNREACHABLE("unknown variant");
            }
        case CI_DATA_TYPE_KIND_BUILTIN:
            return self->builtin == other->builtin;
        case CI_DATA_TYPE_KIND_ENUM: {
            if ((!self->enum_.variants || !other->enum_.variants) ||
                self->enum_.variants->len != other->enum_.variants->len) {
                return !self->enum_.variants && !other->enum_.variants &&
                           self->enum_.name && other->enum_.name
                         ? !strcmp(
                             GET_PTR_RC(String, self->enum_.name)->buffer,
                             GET_PTR_RC(String, other->enum_.name)->buffer)
                         : false;
            }

            return eq__CIDeclEnumVariant(self->enum_.variants,
                                         other->enum_.variants);
        }
        case CI_DATA_TYPE_KIND_FUNCTION:
            if (!(self->function.function_data_type &&
                  other->function.function_data_type) ||
                !eq__CIDataType(self->function.function_data_type,
                                other->function.function_data_type) ||
                !eq__CIDataType(self->function.return_data_type,
                                other->function.return_data_type)) {
                return false;
            } else if (self->function.params && other->function.params
                         ? self->function.params->len !=
                             other->function.params->len
                         : true) {
                return !self->function.params && !other->function.params;
            }

            for (Usize i = 0; i < self->function.params->len; ++i) {
                CIDeclFunctionParam *self_param =
                  get__Vec(self->function.params, i);
                CIDeclFunctionParam *other_param =
                  get__Vec(other->function.params, i);

                if (!eq__CIDataType(self_param->data_type,
                                    other_param->data_type)) {
                    return false;
                }
            }

            return true;
        case CI_DATA_TYPE_KIND_GENERIC:
            return !strcmp(GET_PTR_RC(String, self->generic)->buffer,
                           GET_PTR_RC(String, other->generic)->buffer);
        case CI_DATA_TYPE_KIND_PTR:
            if (!self->ptr && !other->ptr) {
                return true;
            }

            return self->ptr && other->ptr
                     ? eq__CIDataType(self->ptr, other->ptr)
                     : false;
        case CI_DATA_TYPE_KIND_STRUCT:
        case CI_DATA_TYPE_KIND_UNION: {
            const CIDeclStructFields *self_fields =
              get_fields__CIDataType(self);
            const CIDeclStructFields *other_fields =
              get_fields__CIDataType(other);

            if (!self_fields && !other_fields) {
                const String *self_name = get_name__CIDataType(self);
                const String *other_name = get_name__CIDataType(other);

                return self_name && other_name
                         ? !strcmp(self_name->buffer, other_name->buffer)
                         : false;
            }

            return self_fields && other_fields &&
                   eq__CIDeclStructFields(self_fields, other_fields);
        }
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return !strcmp(GET_PTR_RC(String, self->typedef_.name)->buffer,
                           GET_PTR_RC(String, other->typedef_.name)->buffer) &&
                   eq__CIGenericParams(self->typedef_.generic_params,
                                       other->typedef_.generic_params);
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
        case CI_DATA_TYPE_KIND_TYPE_INFO:
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
        case CI_DATA_TYPE_KIND_VOID:
            return true;
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
is_integer__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_BOOL:
        case CI_DATA_TYPE_KIND_CHAR:
        case CI_DATA_TYPE_KIND_INT:
        case CI_DATA_TYPE_KIND_LONG_INT:
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_SHORT_INT:
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            return true;
        default:
            return false;
    }
}

bool
is_float__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND__DECIMAL128:
        case CI_DATA_TYPE_KIND__DECIMAL32:
        case CI_DATA_TYPE_KIND__DECIMAL64:
        case CI_DATA_TYPE_KIND_DOUBLE:
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
        case CI_DATA_TYPE_KIND_FLOAT:
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            return true;
        default:
            return false;
    }
}

CIDataType *
get_ptr__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_PTR:
            return self->ptr;
        default:
            return NULL;
    }
}

const CIGenericParams *
get_generic_params__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
            return self->struct_.generic_params;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return self->typedef_.generic_params;
        case CI_DATA_TYPE_KIND_UNION:
            return self->union_.generic_params;
        default:
            return NULL;
    }
}

bool
has_name__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            if (self->array.name) {
                return true;
            }

            return has_name__CIDataType(self->array.data_type);
        case CI_DATA_TYPE_KIND_FUNCTION:
            return self->function.name;
        default:
            return false;
    }
}

const CIDeclStructFields *
get_fields__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
            return self->struct_.fields;
        case CI_DATA_TYPE_KIND_UNION:
            return self->union_.fields;
        default:
            return NULL;
    }
}

String *
get_name__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ENUM:
            return self->enum_.name ? GET_PTR_RC(String, self->enum_.name)
                                    : NULL;
        case CI_DATA_TYPE_KIND_STRUCT:
            return self->struct_.name ? GET_PTR_RC(String, self->struct_.name)
                                      : NULL;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return self->typedef_.name ? GET_PTR_RC(String, self->typedef_.name)
                                       : NULL;
        case CI_DATA_TYPE_KIND_UNION:
            return self->union_.name ? GET_PTR_RC(String, self->union_.name)
                                     : NULL;
        default:
            UNREACHABLE("cannot get name from data type");
    }
}

String *
serialize_name__CIDataType(const CIDataType *self,
                           const CIGenericParams *called_generic_params)
{
    String *name = get_name__CIDataType(self);

    if (!called_generic_params || !name) {
        UNREACHABLE("called_generic_params is NULL or name is NULL");
    }

    CI_SERIALIZE_NAME(name);
}

CIDataType *
wrap_ptr__CIDataType(CIDataType *self, int ptr_ctx)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_FUNCTION:
            self->function.function_data_type =
              NEW_VARIANT(CIDataType, ptr, self->function.function_data_type);

            set_context__CIDataType(self->function.function_data_type, ptr_ctx);

            return self;
        default: {
            CIDataType *res = NEW_VARIANT(CIDataType, ptr, self);

            set_context__CIDataType(res, ptr_ctx);

            return res;
        }
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataType, const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, array = "
              "{Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              to_string__Debug__CIDataTypeArray(&self->array));
        case CI_DATA_TYPE_KIND_BUILTIN:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, builtin "
              "= {zu} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              self->builtin);
        case CI_DATA_TYPE_KIND_ENUM:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, enum = "
              "{Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              to_string__Debug__CIDataTypeEnum(&self->enum_));
        case CI_DATA_TYPE_KIND_FUNCTION:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, function "
              "= {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              to_string__Debug__CIDataTypeFunction(&self->function));
        case CI_DATA_TYPE_KIND_GENERIC:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, generic "
              "= {S} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              GET_PTR_RC(String, self->generic));
        case CI_DATA_TYPE_KIND_PTR:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, ptr = "
              "{Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              self->ptr ? to_string__Debug__CIDataType(self->ptr)
                        : from__String("NULL"));
        case CI_DATA_TYPE_KIND_STRUCT:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, struct_ "
              "= {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              to_string__Debug__CIDataTypeStruct(&self->struct_));
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, typedef_ "
              "= {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              to_string__Debug__CIDataTypeTypedef(&self->typedef_));
        case CI_DATA_TYPE_KIND_UNION:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr}, union_ = "
              "{Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier),
              to_string__Debug__CIDataTypeUnion(&self->union_));
        default:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, qualifier = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeQualifier(self->qualifier));
    }
}
#endif

VARIANT_DESTRUCTOR(CIDataType, array, CIDataType *self)
{
    FREE(CIDataTypeArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, builtin, CIDataType *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, enum, CIDataType *self)
{
    FREE(CIDataTypeEnum, &self->enum_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self)
{
    FREE(CIDataTypeFunction, &self->function);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, generic, CIDataType *self)
{
    if (self->generic) {
        FREE_RC(String, self->generic);
    }

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, ptr, CIDataType *self)
{
    if (self->ptr) {
        FREE(CIDataType, self->ptr);
    }

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, struct, CIDataType *self)
{
    FREE(CIDataTypeStruct, &self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, typedef, CIDataType *self)
{
    FREE(CIDataTypeTypedef, &self->typedef_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, union, CIDataType *self)
{
    FREE(CIDataTypeUnion, &self->union_);
    lily_free(self);
}

DESTRUCTOR(CIDataType, CIDataType *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            FREE_VARIANT(CIDataType, array, self);
            break;
        case CI_DATA_TYPE_KIND_BUILTIN:
            FREE_VARIANT(CIDataType, builtin, self);
            break;
        case CI_DATA_TYPE_KIND_ENUM:
            FREE_VARIANT(CIDataType, enum, self);
            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            FREE_VARIANT(CIDataType, function, self);
            break;
        case CI_DATA_TYPE_KIND_GENERIC:
            FREE_VARIANT(CIDataType, generic, self);
            break;
        case CI_DATA_TYPE_KIND_PTR:
            FREE_VARIANT(CIDataType, ptr, self);
            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            FREE_VARIANT(CIDataType, struct, self);
            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            FREE_VARIANT(CIDataType, typedef, self);
            break;
        case CI_DATA_TYPE_KIND_UNION:
            FREE_VARIANT(CIDataType, union, self);
            break;
        default:
            lily_free(self);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIAttributeKind, const enum CIAttributeKind self)
{
    switch (self) {
        case CI_ATTRIBUTE_KIND_CLANG:
            return "CI_ATTRIBUTE_KIND_CLANG";
        case CI_ATTRIBUTE_KIND_GNU:
            return "CI_ATTRIBUTE_KIND_GNU";
        case CI_ATTRIBUTE_KIND_STANDARD:
            return "CI_ATTRIBUTE_KIND_STANDARD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIAttributeStandardKind,
               const enum CIAttributeStandardKind self)
{
    switch (self) {
        case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
            return "CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED";
        case CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH:
            return "CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH";
        case CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED:
            return "CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED";
        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
            return "CI_ATTRIBUTE_STANDARD_KIND_NODISCARD";
        case CI_ATTRIBUTE_STANDARD_KIND_NORETURN:
            return "CI_ATTRIBUTE_STANDARD_KIND_NORETURN";
        case CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED:
            return "CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED";
        case CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE:
            return "CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIAttributeStandard, const CIAttributeStandard *self)
{
    switch (self->kind) {
        case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
            return format__String(
              "CIAttributeStandard{{ kind = {s}, deprecated = {S} }",
              to_string__Debug__CIAttributeStandardKind(self->kind),
              GET_PTR_RC(String, self->deprecated));
        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
            return format__String(
              "CIAttributeStandard{{ kind = {s}, nodiscard = {S} }",
              to_string__Debug__CIAttributeStandardKind(self->kind),
              GET_PTR_RC(String, self->nodiscard));
        default:
            return format__String(
              "CIAttributeStandard{{ kind = {s} }",
              to_string__Debug__CIAttributeStandardKind(self->kind));
    }
}
#endif

VARIANT_DESTRUCTOR(CIAttributeStandard,
                   deprecated,
                   const CIAttributeStandard *self)
{
    FREE_RC(String, self->deprecated);
}

VARIANT_DESTRUCTOR(CIAttributeStandard,
                   nodiscard,
                   const CIAttributeStandard *self)
{
    FREE_RC(String, self->nodiscard);
}

DESTRUCTOR(CIAttributeStandard, const CIAttributeStandard *self)
{
    switch (self->kind) {
        case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
            FREE_VARIANT(CIAttributeStandard, deprecated, self);

            break;
        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
            FREE_VARIANT(CIAttributeStandard, nodiscard, self);

            break;
        default:
            break;
    }
}

VARIANT_CONSTRUCTOR(CIAttribute *,
                    CIAttribute,
                    standard,
                    CIAttributeStandard standard)
{
    CIAttribute *self = lily_malloc(sizeof(CIAttribute));

    self->kind = CI_ATTRIBUTE_KIND_STANDARD;
    self->standard = standard;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIAttribute, const CIAttribute *self)
{
    switch (self->kind) {
        case CI_ATTRIBUTE_KIND_STANDARD:
            return format__String(
              "CIAttribute{{ kind = {s}, standard = {Sr} }",
              to_string__Debug__CIAttributeKind(self->kind),
              to_string__Debug__CIAttributeStandard(&self->standard));
        default:
            TODO("not yet implemented");
    }
}
#endif

VARIANT_DESTRUCTOR(CIAttribute, standard, CIAttribute *self)
{
    FREE(CIAttributeStandard, &self->standard);
    lily_free(self);
}

DESTRUCTOR(CIAttribute, CIAttribute *self)
{
    switch (self->kind) {
        case CI_ATTRIBUTE_KIND_STANDARD:
            FREE_VARIANT(CIAttribute, standard, self);

            break;
        default:
            break;
    }
}

String *
to_string__CIStorageClass(int storage_class_flag, enum CIStandard standard)
{
    char *storage_classes[] = {
        "",
        "auto",
        "contexpr",
        "extern",
        "inline",
        "register",
        "static",
        standard >= CI_STANDARD_23 ? "thread_local" : "_Thread_local",
        "typedef",
    };

    String *res = NEW(String);

    for (Usize i = 0; i < storage_class_ids_count && storage_class_flag > 0;
         ++i) {
        if (storage_class_flag & storage_class_ids[i]) {
            push_str__String(res, storage_classes[i]);

            storage_class_flag &= ~storage_class_ids[i];

            if (storage_class_flag != CI_STORAGE_CLASS_NONE) {
                push__String(res, ' ');
            }
        }
    }

    return res;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStorageClass, int storage_class_flag)
{
    static char *storage_classes[] = {
        "CI_STORAGE_CLASS_NONE",      "CI_STORAGE_CLASS_AUTO",
        "CI_STORAGE_CLASS_CONSTEXPR", "CI_STORAGE_CLASS_EXTERN",
        "CI_STORAGE_CLASS_INLINE",    "CI_STORAGE_CLASS_REGISTER",
        "CI_STORAGE_CLASS_STATIC",    "CI_STORAGE_CLASS_THREAD_LOCAL",
        "CI_STORAGE_CLASS_TYPEDEF",
    };
    static_assert(storage_class_ids_count ==
                    sizeof(storage_classes) / sizeof(*storage_classes),
                  "number of items don't match");

    String *res = storage_class_flag == 0
                    ? from__String(storage_classes[CI_STORAGE_CLASS_NONE])
                    : NEW(String);

    for (Usize i = 0; i < storage_class_ids_count && storage_class_flag > 0;
         ++i) {
        if (storage_class_flag & storage_class_ids[i]) {
            push_str__String(res, storage_classes[i]);

            storage_class_flag &= ~storage_class_ids[i];

            if (storage_class_flag != 0) {
                push_str__String(res, " | ");
            }
        }
    }

    return res;
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIDeclKind, enum CIDeclKind self)
{
    switch (self) {
        case CI_DECL_KIND_ENUM:
            return "CI_DECL_KIND_ENUM";
        case CI_DECL_KIND_FUNCTION:
            return "CI_DECL_KIND_FUNCTION";
        case CI_DECL_KIND_STRUCT:
            return "CI_DECL_KIND_STRUCT";
        case CI_DECL_KIND_TYPEDEF:
            return "CI_DECL_KIND_TYPEDEF";
        case CI_DECL_KIND_UNION:
            return "CI_DECL_KIND_UNION";
        case CI_DECL_KIND_VARIABLE:
            return "CI_DECL_KIND_VARIABLE";
        case CI_DECL_KIND_FUNCTION_GEN:
            return "CI_DECL_KIND_FUNCTION_GEN";
        case CI_DECL_KIND_STRUCT_GEN:
            return "CI_DECL_KIND_STRUCT_GEN";
        case CI_DECL_KIND_TYPEDEF_GEN:
            return "CI_DECL_KIND_TYPEDEF_GEN";
        case CI_DECL_KIND_UNION_GEN:
            return "CI_DECL_KIND_UNION_GEN";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(CIDeclEnumVariant *, CIDeclEnumVariant, Rc *name, Isize value)
{
    CIDeclEnumVariant *self = lily_malloc(sizeof(CIDeclEnumVariant));

    self->name = ref__Rc(name);
    self->value = value;
    self->ref_count = 0;

    return self;
}

CIDeclEnumVariant *
clone__CIDeclEnumVariant(CIDeclEnumVariant *self)
{
    return NEW(CIDeclEnumVariant, self->name, self->value);
}

Vec *
clone_variants__CIDeclEnumVariant(Vec *variants)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < variants->len; ++i) {
        push__Vec(res, clone__CIDeclEnumVariant(get__Vec(variants, i)));
    }

    return res;
}

bool
eq__CIDeclEnumVariant(const Vec *self_variants, const Vec *other_variants)
{
    if (self_variants->len != other_variants->len) {
        return false;
    }

    for (Usize i = 0; i < self_variants->len; ++i) {
        CIDeclEnumVariant *self_variant = get__Vec(self_variants, i);
        CIDeclEnumVariant *other_variant = get__Vec(other_variants, i);

        if (strcmp(GET_PTR_RC(String, self_variant->name)->buffer,
                   GET_PTR_RC(String, other_variant->name)->buffer) ||
            self_variant->value != other_variant->value) {
            return false;
        }
    }

    return true;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnumVariant, const CIDeclEnumVariant *self)
{
    return format__String("CIDeclEnumVariant{{ name = {S}, value = {zi} }",
                          GET_PTR_RC(String, self->name),
                          self->value);
}
#endif

DESTRUCTOR(CIDeclEnumVariant, CIDeclEnumVariant *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    FREE_RC(String, self->name);
    lily_free(self);
}

bool
match_prototype__CIDeclEnum(const CIDeclEnum *self, const CIDeclEnum *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(GET_PTR_RC(String, self->name)->buffer,
                   GET_PTR_RC(String, other->name)->buffer));
    ASSERT(!self->variants && !other->variants);

    return (self->data_type && other->data_type &&
            eq__CIDataType(self->data_type, other->data_type)) ||
           (!self->data_type && !other->data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnum, const CIDeclEnum *self)
{
    String *res = format__String("CIDeclEnum{{ name = {S}, variants =",
                                 GET_PTR_RC(String, self->name));

    if (self->variants) {
        DEBUG_VEC_STRING(self->variants, res, CIDeclEnumVariant);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", data_type = {Sr} }",
                         to_string__Debug__CIDataType(self->data_type));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

void
free_as_prototype__CIDeclEnum(const CIDeclEnum *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    if (self->data_type) {
        FREE(CIDataType, self->data_type);
    }
}

DESTRUCTOR(CIDeclEnum, const CIDeclEnum *self)
{
    free_as_prototype__CIDeclEnum(self);

    if (self->variants) {
        FREE_BUFFER_ITEMS(
          self->variants->buffer, self->variants->len, CIDeclEnumVariant);
        FREE(Vec, self->variants);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclFunctionParamKind,
               enum CIDeclFunctionParamKind self)
{
    switch (self) {
        case CI_DECL_FUNCTION_PARAM_KIND_NORMAL:
            return "CI_DECL_FUNCTION_PARAM_KIND_NORMAL";
        case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
            return "CI_DECL_FUNCTION_PARAM_KIND_VARIADIC";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(CIDeclFunctionParam *,
                    CIDeclFunctionParam,
                    normal,
                    Rc *name,
                    CIDataType *data_type)
{
    CIDeclFunctionParam *self = lily_malloc(sizeof(CIDeclFunctionParam));

    self->kind = CI_DECL_FUNCTION_PARAM_KIND_NORMAL;
    self->name = name ? ref__Rc(name) : NULL;
    self->data_type = data_type;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclFunctionParam *, CIDeclFunctionParam, variadic)
{
    CIDeclFunctionParam *self = lily_malloc(sizeof(CIDeclFunctionParam));

    self->kind = CI_DECL_FUNCTION_PARAM_KIND_VARIADIC;
    self->name = NULL;
    self->data_type = NULL;

    return self;
}

CIDeclFunctionParam *
clone__CIDeclFunctionParam(const CIDeclFunctionParam *self)
{
    switch (self->kind) {
        case CI_DECL_FUNCTION_PARAM_KIND_NORMAL:
            return NEW_VARIANT(
              CIDeclFunctionParam,
              normal,
              self->name,
              self->data_type ? clone__CIDataType(self->data_type) : NULL);
        case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
            return NEW_VARIANT(CIDeclFunctionParam, variadic);
        default:
            UNREACHABLE("unknown variant");
    }
}

Vec *
clone_params__CIDeclFunctionParam(const Vec *params)
{
    Vec *cloned_params = NEW(Vec);

    for (Usize i = 0; i < params->len; ++i) {
        push__Vec(cloned_params,
                  clone__CIDeclFunctionParam(get__Vec(params, i)));
    }

    return cloned_params;
}

bool
is_variadic__CIDeclFunctionParam(const Vec *params)
{
    for (Usize i = 0; i < params->len; ++i) {
        CIDeclFunctionParam *param = get__Vec(params, i);

        if (param->kind == CI_DECL_FUNCTION_PARAM_KIND_VARIADIC) {
            return true;
        }
    }

    return false;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionParam, const CIDeclFunctionParam *self)
{
    switch (self->kind) {
        case CI_DECL_FUNCTION_PARAM_KIND_NORMAL:
            return format__String(
              "CIDeclFunctionParam{{ kind = {s}, name = {s}, data_type = {Sr}",
              to_string__Debug__CIDeclFunctionParamKind(self->kind),
              self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL",
              to_string__Debug__CIDataType(self->data_type));
        case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
            return format__String(
              "CIDeclFunctionParam{{ kind = {s}, name = NULL, data_type = NULL "
              "}",
              to_string__Debug__CIDeclFunctionParamKind(self->kind));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIDeclFunctionParam, normal, CIDeclFunctionParam *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    FREE(CIDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclFunctionParam, variadic, CIDeclFunctionParam *self)
{
    lily_free(self);
}

DESTRUCTOR(CIDeclFunctionParam, CIDeclFunctionParam *self)
{
    switch (self->kind) {
        case CI_DECL_FUNCTION_PARAM_KIND_NORMAL:
            FREE_VARIANT(CIDeclFunctionParam, normal, self);

            break;
        case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
            FREE_VARIANT(CIDeclFunctionParam, variadic, self);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(CIDeclFunctionBody *, CIDeclFunctionBody, CIScopeID *scope_id)
{
    CIDeclFunctionBody *self = lily_malloc(sizeof(CIDeclFunctionBody));

    self->scope_id = scope_id;
    self->content = NEW(Vec);

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionBody, const CIDeclFunctionBody *self)
{
    String *res = format__String(
      "CIDeclFunctionBody{{ scope_id = ScopeID {{ id = {zu} }, content =",
      self->scope_id->id);

    DEBUG_VEC_STRING(self->content, res, CIDeclFunctionItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIDeclFunctionBody, CIDeclFunctionBody *self)
{
    FREE_BUFFER_ITEMS(
      self->content->buffer, self->content->len, CIDeclFunctionItem);
    FREE(Vec, self->content);
    lily_free(self);
}

String *
serialize_name__CIDeclFunction(const CIDeclFunction *self,
                               const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(GET_PTR_RC(String, self->name));
}

bool
match_prototype__CIDeclFunction(const CIDeclFunction *self,
                                const CIDeclFunction *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(GET_PTR_RC(String, self->name)->buffer,
                   GET_PTR_RC(String, other->name)->buffer));
    // NOTE: Expected prototype
    ASSERT(!self->body && !other->body);

    return eq__CIDataType(self->return_data_type, other->return_data_type) &&
           eq_generic_params__CIDeclFunction(self, other);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunction, const CIDeclFunction *self)
{
    String *res =
      format__String("CIDeclFunction{{ name = {S}, return_data_type = {Sr}, "
                     "generic_params = {Sr}, params =",
                     GET_PTR_RC(String, self->name),
                     to_string__Debug__CIDataType(self->return_data_type),
                     self->generic_params
                       ? to_string__Debug__CIGenericParams(self->generic_params)
                       : from__String("NULL"));

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, CIDeclFunctionParam);
    } else {
        push_str__String(res, " NULL");
    }

    {
        String *s = format__String(
          ", body = {Sr}",
          self->body ? to_string__Debug__CIDeclFunctionBody(self->body)
                     : from__String("NULL"));

        APPEND_AND_FREE(res, s);
    }

    push_str__String(res, ", attributes =");

    if (self->attributes) {
        DEBUG_VEC_STRING(self->attributes, res, CIAttribute);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

void
free_as_prototype__CIDeclFunction(const CIDeclFunction *self)
{
    FREE_RC(String, self->name);
    FREE(CIDataType, self->return_data_type);

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, CIDeclFunctionParam);
        FREE(Vec, self->params);
    }

    if (self->attributes) {
        FREE_BUFFER_ITEMS(
          self->attributes->buffer, self->attributes->len, CIAttribute);
        FREE(Vec, self->attributes);
    }
}

DESTRUCTOR(CIDeclFunction, const CIDeclFunction *self)
{
    free_as_prototype__CIDeclFunction(self);

    if (self->body) {
        FREE(CIDeclFunctionBody, self->body);
    }
}

bool
has_generic__CIDeclFunctionGen(const CIDeclFunctionGen *self)
{
    return has_generic__CIGenericParams(self->called_generic_params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionGen, const CIDeclFunctionGen *self)
{
    return format__String(
      "CIDeclFunctionGen{{ function = {Sr}, name = {S}, "
      "called_generic_params = {Sr}, return_data_type = {Sr} }",
      to_string__Debug__CIDeclFunction(self->function),
      self->name,
      to_string__Debug__CIGenericParams(self->called_generic_params),
      to_string__Debug__CIDataType(self->return_data_type));
}
#endif

DESTRUCTOR(CIDeclFunctionGen, const CIDeclFunctionGen *self)
{
    FREE(String, self->name);
    FREE(CIGenericParams, self->called_generic_params);
    FREE(CIDataType, self->return_data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclLabel, const CIDeclLabel *self)
{
    return format__String("CIDeclLabel{{ name = {S} }",
                          GET_PTR_RC(String, self->name));
}
#endif

DESTRUCTOR(CIDeclLabel, const CIDeclLabel *self)
{
    FREE_RC(String, self->name);
}

bool
match_prototype__CIDeclStruct(const CIDeclStruct *self,
                              const CIDeclStruct *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(GET_PTR_RC(String, self->name)->buffer,
                   GET_PTR_RC(String, other->name)->buffer));
    // NOTE: Expected prototype
    ASSERT(!self->fields && !other->fields);

    return eq_generic_params__CIDeclStruct(self, other);
}

String *
serialize_name__CIDeclStruct(const CIDeclStruct *self,
                             const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(GET_PTR_RC(String, self->name));
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStruct, const CIDeclStruct *self)
{
    return format__String(
      "CIDeclStruct{{ name = {s}, generic_params = {Sr}, fields = {Sr}, "
      "size_info = {Sr} }",
      self->name ? GET_PTR_RC(String, self->name)->buffer : "NULL",
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"),
      self->fields ? to_string__Debug__CIDeclStructFields(self->fields)
                   : from__String("NULL"),
      to_string__Debug__CISizeInfo(&self->size_info));
}
#endif

void
free_as_prototype__CIDeclStruct(const CIDeclStruct *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }
}

DESTRUCTOR(CIDeclStruct, const CIDeclStruct *self)
{
    free_as_prototype__CIDeclStruct(self);

    if (self->fields) {
        FREE(CIDeclStructFields, self->fields);
    }
}

bool
has_generic__CIDeclStructGen(const CIDeclStructGen *self)
{
    return has_generic__CIGenericParams(self->called_generic_params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStructGen, const CIDeclStructGen *self)
{
    return format__String(
      "CIDeclStructGen{{ struct_ = {Sr}, name = {S}, called_generic_params = "
      "{Sr}, fields = {Sr}, size_info = {Sr} }",
      to_string__Debug__CIDeclStruct(self->struct_),
      self->name,
      to_string__Debug__CIGenericParams(self->called_generic_params),
      self->fields ? to_string__Debug__CIDeclStructFields(self->fields)
                   : from__String("NULL"),
      to_string__Debug__CISizeInfo(&self->size_info));
}
#endif

DESTRUCTOR(CIDeclStructGen, const CIDeclStructGen *self)
{
    FREE(String, self->name);
    FREE(CIGenericParams, self->called_generic_params);

    if (self->fields) {
        FREE(CIDeclStructFields, self->fields);
    }
}

bool
match_prototype__CIDeclTypedef(const CIDeclTypedef *self,
                               const CIDeclTypedef *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(GET_PTR_RC(String, self->name)->buffer,
                   GET_PTR_RC(String, other->name)->buffer));
    ASSERT(self->data_type && other->data_type);

    return eq__CIDataType(self->data_type, other->data_type) &&
           eq_generic_params__CIDeclTypedef(self, other);
}

String *
serialize_name__CIDeclTypedef(const CIDeclTypedef *self,
                              const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(GET_PTR_RC(String, self->name));
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclTypedef, const CIDeclTypedef *self)
{
    return format__String(
      "CIDeclTypedef{{ name = {S}, generic_params = {Sr}, data_type = {Sr}, "
      "size_info = {Sr} }",
      GET_PTR_RC(String, self->name),
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"),
      to_string__Debug__CIDataType(self->data_type),
      to_string__Debug__CISizeInfo(&self->size_info));
}
#endif

void
free_as_prototype__CIDeclTypedef(const CIDeclTypedef *self)
{
    FREE_RC(String, self->name);

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }

    FREE(CIDataType, self->data_type);
}

bool
has_generic__CIDeclTypedefGen(const CIDeclTypedefGen *self)
{
    return has_generic__CIGenericParams(self->called_generic_params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclTypedefGen, const CIDeclTypedefGen *self)
{
    return format__String(
      "CIDeclTypedefGen{{ typedef_ = {Sr}, name = {S}, called_generic_params = "
      "{Sr}, data_type = {Sr}, size_info = {Sr} }",
      to_string__Debug__CIDeclTypedef(self->typedef_),
      self->name,
      to_string__Debug__CIGenericParams(self->called_generic_params),
      to_string__Debug__CIDataType(self->data_type),
      to_string__Debug__CISizeInfo(&self->size_info));
}
#endif

DESTRUCTOR(CIDeclTypedefGen, const CIDeclTypedefGen *self)
{
    FREE(String, self->name);
    FREE(CIGenericParams, self->called_generic_params);
    FREE(CIDataType, self->data_type);
}

bool
match_prototype__CIDeclUnion(const CIDeclUnion *self, const CIDeclUnion *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(GET_PTR_RC(String, self->name)->buffer,
                   GET_PTR_RC(String, other->name)->buffer));
    // NOTE: Expected prototype
    ASSERT(!self->fields && !other->fields);

    return eq_generic_params__CIDeclUnion(self, other);
}

String *
serialize_name__CIDeclUnion(const CIDeclUnion *self,
                            const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(GET_PTR_RC(String, self->name));
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclUnion, const CIDeclUnion *self)
{
    return format__String(
      "CIDeclUnion{{ name = {S}, generic_params = {Sr}, fields = {Sr}, "
      "size_info = {Sr} }",
      GET_PTR_RC(String, self->name),
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"),
      self->fields ? to_string__Debug__CIDeclStructFields(self->fields)
                   : from__String("NULL"),
      to_string__Debug__CISizeInfo(&self->size_info));
}
#endif

void
free_as_prototype__CIDeclUnion(const CIDeclUnion *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }
}

DESTRUCTOR(CIDeclUnion, const CIDeclUnion *self)
{
    free_as_prototype__CIDeclUnion(self);

    if (self->fields) {
        FREE(CIDeclStructFields, self->fields);
    }
}

bool
has_generic__CIDeclUnionGen(const CIDeclUnionGen *self)
{
    return has_generic__CIGenericParams(self->called_generic_params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclUnionGen, const CIDeclUnionGen *self)
{
    return format__String(
      "CIDeclUnionGen{{ union_ = {Sr}, name = {S}, called_generic_params = "
      "{Sr}, fields = {Sr}, size_info = {Sr} }",
      to_string__Debug__CIDeclUnion(self->union_),
      self->name,
      to_string__Debug__CIGenericParams(self->called_generic_params),
      self->fields ? to_string__Debug__CIDeclStructFields(self->fields)
                   : from__String("NULL"),
      to_string__Debug__CISizeInfo(&self->size_info));
}
#endif

DESTRUCTOR(CIDeclUnionGen, const CIDeclUnionGen *self)
{
    FREE(String, self->name);
    FREE(CIGenericParams, self->called_generic_params);

    if (self->fields) {
        FREE(CIDeclStructFields, self->fields);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclVariable, const CIDeclVariable *self)
{
    if (self->expr) {
        return format__String("CIDeclVariable{{ data_type = {Sr}, name = {S}, "
                              "expr = {Sr}, is_local = {b} }",
                              to_string__Debug__CIDataType(self->data_type),
                              GET_PTR_RC(String, self->name),
                              to_string__Debug__CIExpr(self->expr),
                              self->is_local);
    }

    return format__String("CIDeclVariable{{ data_type = {Sr}, name = {S}, expr "
                          "= NULL, is_local = {b} }",
                          to_string__Debug__CIDataType(self->data_type),
                          GET_PTR_RC(String, self->name),
                          self->is_local);
}
#endif

DESTRUCTOR(CIDeclVariable, const CIDeclVariable *self)
{
    FREE_RC(String, self->name);
    FREE(CIDataType, self->data_type);

    if (self->expr) {
        FREE(CIExpr, self->expr);
    }
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    enum,
                    int storage_class_flag,
                    bool is_prototype,
                    CIDeclEnum enum_)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_ENUM;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->ref_count = 0;
    self->enum_ = enum_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    enum_variant,
                    int storage_class_flag,
                    CIDeclEnumVariant *enum_variant)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_ENUM_VARIANT;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = false;
    self->ref_count = 0;
    self->enum_variant = enum_variant;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    function,
                    int storage_class_flag,
                    bool is_prototype,
                    CIDeclFunction function)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_FUNCTION;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->ref_count = 0;
    self->function = function;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    function_gen,
                    CIDecl *function_decl,
                    CIGenericParams *called_generic_params,
                    String *name,
                    CIDataType *return_data_type)
{
    ASSERT(function_decl->kind == CI_DECL_KIND_FUNCTION);

    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclFunction *f = &function_decl->function;

    self->kind = CI_DECL_KIND_FUNCTION_GEN;
    // NOTE: We add the "static" storage class flag to all functions
    // generated by the transpiler (which basically have a params
    // generic), to avoid symbol duplication between entities (e.g.
    // lib, bin).
    self->storage_class_flag =
      function_decl->storage_class_flag | CI_STORAGE_CLASS_STATIC;
    self->is_prototype = false;
    self->ref_count = 0;
    self->function_gen =
      NEW(CIDeclFunctionGen, f, name, called_generic_params, return_data_type);

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *, CIDecl, label, CIDeclLabel label)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_LABEL;
    self->storage_class_flag = CI_STORAGE_CLASS_NONE;
    self->is_prototype = false;
    self->ref_count = 0;
    self->label = label;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    struct,
                    int storage_class_flag,
                    bool is_prototype,
                    CIDeclStruct struct_)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_STRUCT;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->ref_count = 0;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    struct_gen,
                    CIDecl *struct_decl,
                    CIGenericParams *called_generic_params,
                    String *name,
                    CIDeclStructFields *fields)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclStruct *s = &struct_decl->struct_;

    self->kind = CI_DECL_KIND_STRUCT_GEN;
    self->storage_class_flag = struct_decl->storage_class_flag;
    self->is_prototype = false;
    self->ref_count = 0;
    self->struct_gen =
      NEW(CIDeclStructGen, s, name, called_generic_params, fields);

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *, CIDecl, typedef, CIDeclTypedef typedef_)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_TYPEDEF;
    self->storage_class_flag = CI_STORAGE_CLASS_NONE;
    self->is_prototype = true;
    self->ref_count = 0;
    self->typedef_ = typedef_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    typedef_gen,
                    CIDecl *typedef_decl,
                    CIGenericParams *called_generic_params,
                    String *name,
                    CIDataType *data_type)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclTypedef *t = &typedef_decl->typedef_;

    self->kind = CI_DECL_KIND_TYPEDEF_GEN;
    self->storage_class_flag = CI_STORAGE_CLASS_NONE;
    self->is_prototype = true;
    self->ref_count = 0;
    self->typedef_gen =
      NEW(CIDeclTypedefGen, t, name, called_generic_params, data_type);

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    union,
                    int storage_class_flag,
                    bool is_prototype,
                    CIDeclUnion union_)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_UNION;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->ref_count = 0;
    self->union_ = union_;

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    union_gen,
                    CIDecl *union_decl,
                    CIGenericParams *called_generic_params,
                    String *name,
                    CIDeclStructFields *fields)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclUnion *u = &union_decl->union_;

    self->kind = CI_DECL_KIND_UNION_GEN;
    self->storage_class_flag = union_decl->storage_class_flag;
    self->is_prototype = false;
    self->ref_count = 0;
    self->union_gen =
      NEW(CIDeclUnionGen, u, name, called_generic_params, fields);

    return self;
}

VARIANT_CONSTRUCTOR(CIDecl *,
                    CIDecl,
                    variable,
                    int storage_class_flag,
                    bool is_prototype,
                    CIDeclVariable variable)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));

    self->kind = CI_DECL_KIND_VARIABLE;
    self->storage_class_flag = storage_class_flag;
    self->is_prototype = is_prototype;
    self->ref_count = 0;
    self->variable = variable;

    return self;
}

const CIDeclStructFields *
get_fields__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_STRUCT:
            return self->struct_.fields;
        case CI_DECL_KIND_STRUCT_GEN:
            return self->struct_gen.fields;
        case CI_DECL_KIND_UNION:
            return self->union_.fields;
        case CI_DECL_KIND_UNION_GEN:
            return self->union_gen.fields;
        default:
            UNREACHABLE("cannot get fields from decl");
    }
}

CIGenericParams *
get_generic_params__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_FUNCTION:
            return self->function.generic_params;
        case CI_DECL_KIND_STRUCT:
            return self->struct_.generic_params;
        case CI_DECL_KIND_UNION:
            return self->union_.generic_params;
        default:
            UNREACHABLE("cannot get `generic_params`");
    }
}

String *
get_name__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            return self->enum_.name ? GET_PTR_RC(String, self->enum_.name)
                                    : NULL;
        case CI_DECL_KIND_ENUM_VARIANT:
            return GET_PTR_RC(String, self->enum_variant->name);
        case CI_DECL_KIND_FUNCTION:
            return GET_PTR_RC(String, self->function.name);
        case CI_DECL_KIND_FUNCTION_GEN:
            return self->function_gen.name;
        case CI_DECL_KIND_LABEL:
            return GET_PTR_RC(String, self->label.name);
        case CI_DECL_KIND_STRUCT:
            return self->struct_.name ? GET_PTR_RC(String, self->struct_.name)
                                      : NULL;
        case CI_DECL_KIND_STRUCT_GEN:
            return self->struct_gen.name;
        case CI_DECL_KIND_TYPEDEF:
            return GET_PTR_RC(String, self->typedef_.name);
        case CI_DECL_KIND_TYPEDEF_GEN:
            return self->typedef_gen.name;
        case CI_DECL_KIND_UNION:
            return self->union_.name ? GET_PTR_RC(String, self->union_.name)
                                     : NULL;
        case CI_DECL_KIND_UNION_GEN:
            return self->union_gen.name;
        case CI_DECL_KIND_VARIABLE:
            return GET_PTR_RC(String, self->variable.name);
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
has_generic__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_FUNCTION:
            return self->function.generic_params;
        case CI_DECL_KIND_FUNCTION_GEN:
            return has_generic__CIDeclFunctionGen(&self->function_gen);
        case CI_DECL_KIND_ENUM:
        case CI_DECL_KIND_ENUM_VARIANT:
        case CI_DECL_KIND_LABEL:
        case CI_DECL_KIND_VARIABLE:
            return false;
        case CI_DECL_KIND_STRUCT:
            return self->struct_.generic_params;
        case CI_DECL_KIND_STRUCT_GEN:
            return has_generic__CIDeclStructGen(&self->struct_gen);
        case CI_DECL_KIND_TYPEDEF:
            return self->typedef_.generic_params;
        case CI_DECL_KIND_TYPEDEF_GEN:
            return has_generic__CIDeclTypedefGen(&self->typedef_gen);
        case CI_DECL_KIND_UNION:
            return self->union_.generic_params;
        case CI_DECL_KIND_UNION_GEN:
            return has_generic__CIDeclUnionGen(&self->union_gen);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIDataType *
get_expected_data_type__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            TODO("add a data type for type of variant (int, short int, ...) "
                 "(all integers data type)");
        case CI_DECL_KIND_FUNCTION:
            return self->function.return_data_type;
        case CI_DECL_KIND_FUNCTION_GEN:
            TODO("get data type from function gen");
        case CI_DECL_KIND_LABEL:
        case CI_DECL_KIND_STRUCT:
        case CI_DECL_KIND_STRUCT_GEN:
        case CI_DECL_KIND_TYPEDEF:
        case CI_DECL_KIND_TYPEDEF_GEN:
        case CI_DECL_KIND_UNION:
        case CI_DECL_KIND_UNION_GEN:
            return NULL;
        case CI_DECL_KIND_VARIABLE:
            return self->variable.data_type;
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
match_prototype__CIDecl(const CIDecl *self, const CIDecl *other)
{
    ASSERT(self->kind == other->kind);

    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            return match_prototype__CIDeclEnum(&self->enum_, &other->enum_);
        case CI_DECL_KIND_FUNCTION:
            return match_prototype__CIDeclFunction(&self->function,
                                                   &other->function);
        case CI_DECL_KIND_STRUCT:
            return match_prototype__CIDeclStruct(&self->struct_,
                                                 &other->struct_);
        case CI_DECL_KIND_TYPEDEF:
            return match_prototype__CIDeclTypedef(&self->typedef_,
                                                  &other->typedef_);
        case CI_DECL_KIND_UNION:
            return match_prototype__CIDeclUnion(&self->union_, &other->union_);
        default:
            UNREACHABLE("match_prototype__* is not implemented for this type "
                        "of declaration or is impossible to implement");
    }
}

const CISizeInfo *
get_size_info__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            return &self->enum_.size_info;
        case CI_DECL_KIND_STRUCT:
            if (self->struct_.generic_params &&
                has_generic__CIGenericParams(self->struct_.generic_params)) {
                UNREACHABLE("cannot get the size of genreic struct");
            }

            return &self->struct_.size_info;
        case CI_DECL_KIND_STRUCT_GEN:
            return &self->struct_gen.size_info;
        case CI_DECL_KIND_TYPEDEF:
            return &self->typedef_.size_info;
        case CI_DECL_KIND_TYPEDEF_GEN:
            return &self->typedef_gen.size_info;
        case CI_DECL_KIND_UNION:
            if (self->union_.generic_params &&
                has_generic__CIGenericParams(self->union_.generic_params)) {
                UNREACHABLE("cannot get the size of generic union");
            }

            return &self->union_gen.size_info;
        case CI_DECL_KIND_UNION_GEN:
            return &self->union_gen.size_info;
        default:
            UNREACHABLE(
              "not expected to get size with this kind of declaration");
    }
}

Usize
get_size__CIDecl(const CIDecl *self)
{
    return get_size_info__CIDecl(self)->size;
}

void
fill_size__CIDecl(const CIDecl *self, Usize size)
{
    CISizeInfo *size_info = (CISizeInfo *)get_size_info__CIDecl(self);

    fill_size__CISizeInfo(size_info, size);
}

Usize
get_alignment__CIDecl(const CIDecl *self)
{
    return get_size_info__CIDecl(self)->alignment;
}

void
fill_alignment__CIDecl(const CIDecl *self, Usize alignment)
{
    CISizeInfo *size_info = (CISizeInfo *)get_size_info__CIDecl(self);

    fill_alignment__CISizeInfo(size_info, alignment);
}

const CIDataType *
get_typedef_data_type__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_TYPEDEF:
            return self->typedef_.data_type;
        case CI_DECL_KIND_TYPEDEF_GEN:
            return self->typedef_gen.data_type;
        default:
            UNREACHABLE(
              "impossible to get data type of typedef with this variant");
    }
}

const CIDataType *
get_return_data_type__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_FUNCTION:
            return self->function.return_data_type;
        case CI_DECL_KIND_FUNCTION_GEN:
            return self->function_gen.return_data_type;
        default:
            UNREACHABLE("impossible to get return data type of function with "
                        "this variant");
    }
}

bool
is_local__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_LABEL:
            return true;
        case CI_DECL_KIND_VARIABLE:
            return self->variable.is_local;
        default:
            return false;
    }
}

const Vec *
get_function_params__CIDecl(const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_FUNCTION:
            return self->function.params;
        case CI_DECL_KIND_FUNCTION_GEN:
            return self->function_gen.function->params;
        default:
            UNREACHABLE("cannot get function params");
    }
}

void
set_function_body__CIDecl(CIDecl *self, CIDeclFunctionBody *body)
{
    ASSERT(self->kind == CI_DECL_KIND_FUNCTION);

    self->is_prototype = false;
    self->function.body = body;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDecl, const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, enum_ = {Sr} }",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclEnum(&self->enum_));
        case CI_DECL_KIND_FUNCTION:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, function = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclFunction(&self->function));
        case CI_DECL_KIND_FUNCTION_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, function_gen = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclFunctionGen(&self->function_gen));
        case CI_DECL_KIND_LABEL:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, label = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclLabel(&self->label));
        case CI_DECL_KIND_STRUCT:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, struct_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclStruct(&self->struct_));
        case CI_DECL_KIND_STRUCT_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, struct_gen = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclStructGen(&self->struct_gen));
        case CI_DECL_KIND_TYPEDEF:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, typedef_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclTypedef(&self->typedef_));
        case CI_DECL_KIND_TYPEDEF_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, typedef_gen = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclTypedefGen(&self->typedef_gen));
        case CI_DECL_KIND_VARIABLE:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, variable = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclVariable(&self->variable));
        case CI_DECL_KIND_UNION:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, union_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclUnion(&self->union_));
        case CI_DECL_KIND_UNION_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {Sr}, is_prototype = "
              "{b}, union_gen = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclUnionGen(&self->union_gen));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

void
free_as_prototype__CIDecl(CIDecl *self)
{
    ASSERT(self->is_prototype);

    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            ASSERT(!self->enum_.variants);

            free_as_prototype__CIDeclEnum(&self->enum_);

            break;
        case CI_DECL_KIND_FUNCTION:
            ASSERT(!self->function.body);

            free_as_prototype__CIDeclFunction(&self->function);

            break;
        case CI_DECL_KIND_STRUCT:
            ASSERT(!self->struct_.fields);

            free_as_prototype__CIDeclStruct(&self->struct_);

            break;
        case CI_DECL_KIND_TYPEDEF:
            free_as_prototype__CIDeclTypedef(&self->typedef_);

            break;
        case CI_DECL_KIND_UNION:
            ASSERT(!self->union_.fields);

            free_as_prototype__CIDeclUnion(&self->union_);

            break;
        default:
            UNREACHABLE("This case of declaration prototype free has not yet "
                        "been implemented for this type of declaration, or it "
                        "is simply unnecessary");
    }

    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, enum, CIDecl *self)
{
    FREE(CIDeclEnum, &self->enum_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, enum_variant, CIDecl *self)
{
    FREE(CIDeclEnumVariant, self->enum_variant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, function, CIDecl *self)
{
    FREE(CIDeclFunction, &self->function);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, function_gen, CIDecl *self)
{
    FREE(CIDeclFunctionGen, &self->function_gen);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, label, CIDecl *self)
{
    FREE(CIDeclLabel, &self->label);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, struct, CIDecl *self)
{
    FREE(CIDeclStruct, &self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, struct_gen, CIDecl *self)
{
    FREE(CIDeclStructGen, &self->struct_gen);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, typedef, CIDecl *self)
{
    FREE(CIDeclTypedef, &self->typedef_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, typedef_gen, CIDecl *self)
{
    FREE(CIDeclTypedefGen, &self->typedef_gen);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, union, CIDecl *self)
{
    FREE(CIDeclUnion, &self->union_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, union_gen, CIDecl *self)
{
    FREE(CIDeclUnionGen, &self->union_gen);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDecl, variable, CIDecl *self)
{
    FREE(CIDeclVariable, &self->variable);
    lily_free(self);
}

DESTRUCTOR(CIDecl, CIDecl *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            FREE_VARIANT(CIDecl, enum, self);
            break;
        case CI_DECL_KIND_ENUM_VARIANT:
            FREE_VARIANT(CIDecl, enum_variant, self);
            break;
        case CI_DECL_KIND_FUNCTION:
            FREE_VARIANT(CIDecl, function, self);
            break;
        case CI_DECL_KIND_FUNCTION_GEN:
            FREE_VARIANT(CIDecl, function_gen, self);
            break;
        case CI_DECL_KIND_LABEL:
            FREE_VARIANT(CIDecl, label, self);
            break;
        case CI_DECL_KIND_STRUCT:
            FREE_VARIANT(CIDecl, struct, self);
            break;
        case CI_DECL_KIND_STRUCT_GEN:
            FREE_VARIANT(CIDecl, struct_gen, self);
            break;
        case CI_DECL_KIND_TYPEDEF:
            FREE_VARIANT(CIDecl, typedef, self);
            break;
        case CI_DECL_KIND_TYPEDEF_GEN:
            FREE_VARIANT(CIDecl, typedef_gen, self);
            break;
        case CI_DECL_KIND_UNION:
            FREE_VARIANT(CIDecl, union, self);
            break;
        case CI_DECL_KIND_UNION_GEN:
            FREE_VARIANT(CIDecl, union_gen, self);
            break;
        case CI_DECL_KIND_VARIABLE:
            FREE_VARIANT(CIDecl, variable, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprArrayAccess, const CIExprArrayAccess *self)
{
    return format__String("CIExprArrayAccess{{ array = {Sr}, access = {Sr} }",
                          to_string__Debug__CIExpr(self->array),
                          to_string__Debug__CIExpr(self->access));
}
#endif

DESTRUCTOR(CIExprArrayAccess, const CIExprArrayAccess *self)
{
    FREE(CIExpr, self->array);
    FREE(CIExpr, self->access);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprBinaryKind, enum CIExprBinaryKind self)
{
    switch (self) {
        case CI_EXPR_BINARY_KIND_ASSIGN:
            return "CI_EXPR_BINARY_KIND_ASSIGN";
        case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
            return "CI_EXPR_BINARY_KIND_ASSIGN_ADD";
        case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
            return "CI_EXPR_BINARY_KIND_ASSIGN_SUB";
        case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
            return "CI_EXPR_BINARY_KIND_ASSIGN_MUL";
        case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
            return "CI_EXPR_BINARY_KIND_ASSIGN_DIV";
        case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
            return "CI_EXPR_BINARY_KIND_ASSIGN_MOD";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR";
        case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
            return "CI_EXPR_BINARY_KIND_ASSIGN_XOR";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT";
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT:
            return "CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT";
        case CI_EXPR_BINARY_KIND_ADD:
            return "CI_EXPR_BINARY_KIND_ADD";
        case CI_EXPR_BINARY_KIND_SUB:
            return "CI_EXPR_BINARY_KIND_SUB";
        case CI_EXPR_BINARY_KIND_MUL:
            return "CI_EXPR_BINARY_KIND_MUL";
        case CI_EXPR_BINARY_KIND_DIV:
            return "CI_EXPR_BINARY_KIND_DIV";
        case CI_EXPR_BINARY_KIND_MOD:
            return "CI_EXPR_BINARY_KIND_MOD";
        case CI_EXPR_BINARY_KIND_BIT_AND:
            return "CI_EXPR_BINARY_KIND_BIT_AND";
        case CI_EXPR_BINARY_KIND_BIT_OR:
            return "CI_EXPR_BINARY_KIND_BIT_OR";
        case CI_EXPR_BINARY_KIND_BIT_XOR:
            return "CI_EXPR_BINARY_KIND_BIT_XOR";
        case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
            return "CI_EXPR_BINARY_KIND_BIT_LSHIFT";
        case CI_EXPR_BINARY_KIND_BIT_RSHIFT:
            return "CI_EXPR_BINARY_KIND_BIT_RSHIFT";
        case CI_EXPR_BINARY_KIND_AND:
            return "CI_EXPR_BINARY_KIND_AND";
        case CI_EXPR_BINARY_KIND_OR:
            return "CI_EXPR_BINARY_KIND_OR";
        case CI_EXPR_BINARY_KIND_EQ:
            return "CI_EXPR_BINARY_KIND_EQ";
        case CI_EXPR_BINARY_KIND_NE:
            return "CI_EXPR_BINARY_KIND_NE";
        case CI_EXPR_BINARY_KIND_LESS:
            return "CI_EXPR_BINARY_KIND_LESS";
        case CI_EXPR_BINARY_KIND_GREATER:
            return "CI_EXPR_BINARY_KIND_GREATER";
        case CI_EXPR_BINARY_KIND_LESS_EQ:
            return "CI_EXPR_BINARY_KIND_LESS_EQ";
        case CI_EXPR_BINARY_KIND_GREATER_EQ:
            return "CI_EXPR_BINARY_KIND_GREATER_EQ";
        case CI_EXPR_BINARY_KIND_DOT:
            return "CI_EXPR_BINARY_KIND_DOT";
        case CI_EXPR_BINARY_KIND_ARROW:
            return "CI_EXPR_BINARY_KIND_ARROW";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

Uint8
to_precedence__CIExprBinaryKind(enum CIExprBinaryKind kind)
{
    switch (kind) {
        case CI_EXPR_BINARY_KIND_DOT:
        case CI_EXPR_BINARY_KIND_ARROW:
            return 100;
        case CI_EXPR_BINARY_KIND_MUL:
        case CI_EXPR_BINARY_KIND_DIV:
        case CI_EXPR_BINARY_KIND_MOD:
            return 90;
        case CI_EXPR_BINARY_KIND_ADD:
        case CI_EXPR_BINARY_KIND_SUB:
            return 85;
        case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
        case CI_EXPR_BINARY_KIND_BIT_RSHIFT:
            return 80;
        case CI_EXPR_BINARY_KIND_LESS:
        case CI_EXPR_BINARY_KIND_LESS_EQ:
        case CI_EXPR_BINARY_KIND_GREATER:
        case CI_EXPR_BINARY_KIND_GREATER_EQ:
            return 75;
        case CI_EXPR_BINARY_KIND_EQ:
        case CI_EXPR_BINARY_KIND_NE:
            return 70;
        case CI_EXPR_BINARY_KIND_BIT_AND:
            return 65;
        case CI_EXPR_BINARY_KIND_BIT_XOR:
            return 60;
        case CI_EXPR_BINARY_KIND_BIT_OR:
            return 55;
        case CI_EXPR_BINARY_KIND_AND:
            return 50;
        case CI_EXPR_BINARY_KIND_OR:
            return 45;
        default:
            return 40;
    }
}

enum CIExprBinaryKind
from_token__CIExprBinaryKind(const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN;
        case CI_TOKEN_KIND_PLUS_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_ADD;
        case CI_TOKEN_KIND_MINUS_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_SUB;
        case CI_TOKEN_KIND_STAR_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_MUL;
        case CI_TOKEN_KIND_SLASH_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_DIV;
        case CI_TOKEN_KIND_PERCENTAGE_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_MOD;
        case CI_TOKEN_KIND_AMPERSAND_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND;
        case CI_TOKEN_KIND_BAR_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR;
        case CI_TOKEN_KIND_HAT_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_XOR;
        case CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT;
        case CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ:
            return CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT;
        case CI_TOKEN_KIND_PLUS:
            return CI_EXPR_BINARY_KIND_ADD;
        case CI_TOKEN_KIND_MINUS:
            return CI_EXPR_BINARY_KIND_SUB;
        case CI_TOKEN_KIND_STAR:
            return CI_EXPR_BINARY_KIND_MUL;
        case CI_TOKEN_KIND_SLASH:
            return CI_EXPR_BINARY_KIND_DIV;
        case CI_TOKEN_KIND_PERCENTAGE:
            return CI_EXPR_BINARY_KIND_MOD;
        case CI_TOKEN_KIND_AMPERSAND:
            return CI_EXPR_BINARY_KIND_BIT_AND;
        case CI_TOKEN_KIND_BAR:
            return CI_EXPR_BINARY_KIND_BIT_OR;
        case CI_TOKEN_KIND_HAT:
            return CI_EXPR_BINARY_KIND_BIT_XOR;
        case CI_TOKEN_KIND_LSHIFT_LSHIFT:
            return CI_EXPR_BINARY_KIND_BIT_LSHIFT;
        case CI_TOKEN_KIND_RSHIFT_RSHIFT:
            return CI_EXPR_BINARY_KIND_BIT_RSHIFT;
        case CI_TOKEN_KIND_AMPERSAND_AMPERSAND:
            return CI_EXPR_BINARY_KIND_AND;
        case CI_TOKEN_KIND_BAR_BAR:
            return CI_EXPR_BINARY_KIND_OR;
        case CI_TOKEN_KIND_EQ_EQ:
            return CI_EXPR_BINARY_KIND_EQ;
        case CI_TOKEN_KIND_BANG_EQ:
            return CI_EXPR_BINARY_KIND_NE;
        case CI_TOKEN_KIND_LSHIFT:
            return CI_EXPR_BINARY_KIND_LESS;
        case CI_TOKEN_KIND_RSHIFT:
            return CI_EXPR_BINARY_KIND_GREATER;
        case CI_TOKEN_KIND_LSHIFT_EQ:
            return CI_EXPR_BINARY_KIND_LESS_EQ;
        case CI_TOKEN_KIND_RSHIFT_EQ:
            return CI_EXPR_BINARY_KIND_GREATER_EQ;
        case CI_TOKEN_KIND_DOT:
            return CI_EXPR_BINARY_KIND_DOT;
        case CI_TOKEN_KIND_ARROW:
            return CI_EXPR_BINARY_KIND_ARROW;
        default:
            UNREACHABLE("impossible to convert this token in CIExprBinaryKind");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprBinary, const CIExprBinary *self)
{
    return format__String(
      "CIExprBinary{{ kind = {s}, left = {Sr}, right = {Sr} }",
      to_string__Debug__CIExprBinaryKind(self->kind),
      to_string__Debug__CIExpr(self->left),
      to_string__Debug__CIExpr(self->right));
}
#endif

DESTRUCTOR(CIExprBinary, const CIExprBinary *self)
{
    FREE(CIExpr, self->left);
    FREE(CIExpr, self->right);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprLiteralKind, enum CIExprLiteralKind self)
{
    switch (self) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            return "CI_EXPR_LITERAL_KIND_BOOL";
        case CI_EXPR_LITERAL_KIND_CHAR:
            return "CI_EXPR_LITERAL_KIND_CHAR";
        case CI_EXPR_LITERAL_KIND_FLOAT:
            return "CI_EXPR_LITERAL_KIND_FLOAT";
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            return "CI_EXPR_LITERAL_KIND_SIGNED_INT";
        case CI_EXPR_LITERAL_KIND_STRING:
            return "CI_EXPR_LITERAL_KIND_STRING";
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            return "CI_EXPR_LITERAL_KIND_UNSIGNED_INT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprLiteral, const CIExprLiteral *self)
{
    switch (self->kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            return format__String(
              "CIExprLiteral{{ kind = {s}, bool_ = {b} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->bool_);
        case CI_EXPR_LITERAL_KIND_CHAR:
            return format__String(
              "CIExprLiteral{{ kind = {s}, char_ = {c} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->char_);
        case CI_EXPR_LITERAL_KIND_FLOAT:
            return format__String(
              "CIExprLiteral{{ kind = {s}, float_ = {f} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->float_);
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            // TODO: Update {d} format specifier.
            return format__String(
              "CIExprLiteral{{ kind = {s}, signed_int = {d} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->signed_int);
        case CI_EXPR_LITERAL_KIND_STRING:
            return format__String(
              "CIExprLiteral{{ kind = {s}, string = {S} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              GET_PTR_RC(String, self->string));
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            return format__String(
              "CIExprLiteral{{ kind = {s}, unsigned_int = {zu} }",
              to_string__Debug__CIExprLiteralKind(self->kind),
              self->unsigned_int);
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIExprLiteral, string, const CIExprLiteral *self)
{
    FREE_RC(String, self->string);
}

DESTRUCTOR(CIExprLiteral, const CIExprLiteral *self)
{
    switch (self->kind) {
        case CI_EXPR_LITERAL_KIND_STRING:
            FREE_VARIANT(CIExprLiteral, string, self);

            break;
        default:
            break;
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprUnaryKind, enum CIExprUnaryKind self)
{
    switch (self) {
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
            return "CI_EXPR_UNARY_KIND_PRE_INCREMENT";
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            return "CI_EXPR_UNARY_KIND_PRE_DECREMENT";
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
            return "CI_EXPR_UNARY_KIND_POST_INCREMENT";
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
            return "CI_EXPR_UNARY_KIND_POST_DECREMENT";
        case CI_EXPR_UNARY_KIND_POSITIVE:
            return "CI_EXPR_UNARY_KIND_POSITIVE";
        case CI_EXPR_UNARY_KIND_NEGATIVE:
            return "CI_EXPR_UNARY_KIND_NEGATIVE";
        case CI_EXPR_UNARY_KIND_BIT_NOT:
            return "CI_EXPR_UNARY_KIND_BIT_NOT";
        case CI_EXPR_UNARY_KIND_NOT:
            return "CI_EXPR_UNARY_KIND_NOT";
        case CI_EXPR_UNARY_KIND_DEREFERENCE:
            return "CI_EXPR_UNARY_KIND_DEREFERENCE";
        case CI_EXPR_UNARY_KIND_REF:
            return "CI_EXPR_UNARY_KIND_REF";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

Uint8
to_precedence__CIExprUnaryKind(enum CIExprUnaryKind kind)
{
    switch (kind) {
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
            return EXPR_PRECEDENCE_LEVEL_1;
        default:
            return EXPR_PRECEDENCE_LEVEL_2;
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprUnary, const CIExprUnary *self)
{
    return format__String("CIExprUnary{{ kind = {s}, expr = {Sr} }",
                          to_string__Debug__CIExprUnaryKind(self->kind),
                          to_string__Debug__CIExpr(self->expr));
}
#endif

DESTRUCTOR(CIExprUnary, const CIExprUnary *self)
{
    FREE(CIExpr, self->expr);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprTernary, const CIExprTernary *self)
{
    return format__String(
      "CIExprTernary{{ cond = {Sr}, if_ = {Sr}, else_ = {Sr} }",
      to_string__Debug__CIExpr(self->cond),
      to_string__Debug__CIExpr(self->if_),
      to_string__Debug__CIExpr(self->else_));
}
#endif

DESTRUCTOR(CIExprTernary, const CIExprTernary *self)
{
    FREE(CIExpr, self->cond);
    FREE(CIExpr, self->if_);
    FREE(CIExpr, self->else_);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprCast, const CIExprCast *self)
{
    return format__String("CIExprCast{{ data_type = {Sr}, expr = {Sr} }",
                          to_string__Debug__CIDataType(self->data_type),
                          to_string__Debug__CIExpr(self->expr));
}
#endif

DESTRUCTOR(CIExprCast, const CIExprCast *self)
{
    FREE(CIDataType, self->data_type);
    FREE(CIExpr, self->expr);
}

String *
serialize_name__CIExprFunctionCall(const CIExprFunctionCall *self,
                                   const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(GET_PTR_RC(String, self->identifier));
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprFunctionCall, const CIExprFunctionCall *self)
{
    String *res =
      format__String("CIExprFunctionCall{{ identifier = {S}, params =",
                     GET_PTR_RC(String, self->identifier));

    DEBUG_VEC_STRING(self->params, res, CIExpr);

    {
        char *s =
          format(", generic_params = {Sr} }",
                 self->generic_params
                   ? to_string__Debug__CIGenericParams(self->generic_params)
                   : from__String("NULL"));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIExprFunctionCall, const CIExprFunctionCall *self)
{
    FREE_RC(String, self->identifier);
    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, CIExpr);
    FREE(Vec, self->params);

    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CIExprFunctionCallBuiltin,
               const CIExprFunctionCallBuiltin *self)
{
    String *res = format__String(
      "CIExprFunctionCallBuiltin{{ id = {zu}, params = ", self->id);

    DEBUG_VEC_STRING(self->params, res, CIExpr);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIExprFunctionCallBuiltin, const CIExprFunctionCallBuiltin *self)
{
    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, CIExpr);
    FREE(Vec, self->params);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIExprIdentifierIDKind,
               enum CIExprIdentifierIDKind self)
{
    switch (self) {
        case CI_EXPR_IDENTIFIER_ID_KIND_NONE:
            return "CI_EXPR_IDENTIFIER_ID_KIND_NONE";
        case CI_EXPR_IDENTIFIER_ID_KIND_ENUM_VARIANT:
            return "CI_EXPR_IDENTIFIER_ID_KIND_ENUM_VARIANT";
        case CI_EXPR_IDENTIFIER_ID_KIND_FUNCTION:
            return "CI_EXPR_IDENTIFIER_ID_KIND_FUNCTION";
        case CI_EXPR_IDENTIFIER_ID_KIND_LABEL:
            return "CI_EXPR_IDENTIFIER_ID_KIND_LABEL";
        case CI_EXPR_IDENTIFIER_ID_KIND_VARIABLE:
            return "CI_EXPR_IDENTIFIER_ID_KIND_VARIABLE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CIExprIdentifierID
search_identifier__CIExprIdentifierID(const String *name,
                                      const CIResultFile *file,
                                      CIScope *scope)
{
    static const void *(*search_functions[])(
      const CIScope *,
      const String *) = { (const void *(*)(const CIScope *, const String *)) &
                            search_enum_variant__CIScope,
                          (const void *(*)(const CIScope *, const String *)) &
                            search_function__CIScope,
                          (const void *(*)(const CIScope *, const String *)) &
                            search_label__CIScope,
                          (const void *(*)(const CIScope *, const String *)) &
                            search_variable__CIScope };
    static CIExprIdentifierID (*constructor_functions[])(const void *) = {
        (CIExprIdentifierID(*)(const void *)) &
          VARIANT_CONSTRUCTOR_NAME(CIExprIdentifierID, enum_variant),
        (CIExprIdentifierID(*)(const void *)) &
          VARIANT_CONSTRUCTOR_NAME(CIExprIdentifierID, function),
        (CIExprIdentifierID(*)(const void *)) &
          VARIANT_CONSTRUCTOR_NAME(CIExprIdentifierID, label),
        (CIExprIdentifierID(*)(const void *)) &
          VARIANT_CONSTRUCTOR_NAME(CIExprIdentifierID, variable),
    };

    for (; scope; scope = scope->parent ? get_scope_from_id__CIResultFile(
                                            file, scope->parent)
                                        : NULL) {
        for (Usize i = 0; i < LEN(search_functions, *search_functions); ++i) {
            const void *res_id = search_functions[i](scope, name);

            if (res_id) {
                return constructor_functions[i](res_id);
            }
        }
    }

    return NEW_VARIANT(CIExprIdentifierID, none);
}

CIDecl *
get_decl__CIExprIdentifierID(const CIExprIdentifierID *self,
                             const CIResultFile *file)
{
    switch (self->kind) {
        case CI_EXPR_IDENTIFIER_ID_KIND_ENUM_VARIANT: {
            CIDecl *enum_variant_decl =
              get_enum_variant_from_id__CIResultFile(file, self->enum_variant);

            ASSERT(enum_variant_decl);

            return enum_variant_decl;
        }
        case CI_EXPR_IDENTIFIER_ID_KIND_FUNCTION: {
            CIDecl *function_decl =
              get_function_from_id__CIResultFile(file, self->function);

            ASSERT(function_decl);

            return function_decl;
        }
        case CI_EXPR_IDENTIFIER_ID_KIND_LABEL: {
            CIDecl *label_decl =
              get_label_from_id__CIResultFile(file, self->label);

            ASSERT(label_decl);

            return label_decl;
        }
        case CI_EXPR_IDENTIFIER_ID_KIND_VARIABLE: {
            CIDecl *variable_decl =
              get_variable_from_id__CIResultFile(file, self->variable);

            ASSERT(variable_decl);

            return variable_decl;
        }
        case CI_EXPR_IDENTIFIER_ID_KIND_NONE:
            return NULL;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprIdentifierID, const CIExprIdentifierID *self)
{
    return format__String("CIExprIdentifierID{{ kind = {s}, ... }",
                          to_string__Debug__CIExprIdentifierIDKind(self->kind));
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprIdentifier, const CIExprIdentifier *self)
{
    return format__String("CIExprIdentifier{{ value = {S}, id = {Sr} }",
                          GET_PTR_RC(String, self->value),
                          to_string__Debug__CIExprIdentifierID(&self->id));
}
#endif

CONSTRUCTOR(CIExprInitializerItem *,
            CIExprInitializerItem,
            Vec *path,
            CIExpr *value)
{
    CIExprInitializerItem *self = lily_malloc(sizeof(CIExprInitializerItem));

    self->path = path;
    self->value = value;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CIExprInitializerItem,
               const CIExprInitializerItem *self)
{
    String *res = format__String("CIExprInitializerItem{{ path =");

    if (self->path) {
        push_str__String(res, "{ ");

        for (Usize i = 0; i < self->path->len; ++i) {
            push_str__String(
              res,
              GET_PTR_RC(String, CAST(Rc *, get__Vec(self->path, i)))->buffer);

            if (i + 1 != self->path->len) {
                push_str__String(res, ", ");
            }
        }

        push_str__String(res, " }");
    } else {
        push_str__String(res, " NULL");
    }

    {
        String *s = format__String(", value = {Sr} }",
                                   to_string__Debug__CIExpr(self->value));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIExprInitializerItem, CIExprInitializerItem *self)
{
    if (self->path) {
        FREE_BUFFER_RC_ITEMS(self->path->buffer, self->path->len, String);
        FREE(Vec, self->path);
    }

    FREE(CIExpr, self->value);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprInitializer, const CIExprInitializer *self)
{
    String *res = format__String("CIExprStructCall{{ items =");

    DEBUG_VEC_STRING(self->items, res, CIExprInitializerItem);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIExprInitializer, const CIExprInitializer *self)
{
    FREE_BUFFER_ITEMS(
      self->items->buffer, self->items->len, CIExprInitializerItem);
    FREE(Vec, self->items);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIExprKind, enum CIExprKind self)
{
    switch (self) {
        case CI_EXPR_KIND_ALIGNOF:
            return "CI_EXPR_KIND_ALIGNOF";
        case CI_EXPR_KIND_ARRAY_ACCESS:
            return "CI_EXPR_KIND_ARRAY_ACCESS";
        case CI_EXPR_KIND_BINARY:
            return "CI_EXPR_KIND_BINARY";
        case CI_EXPR_KIND_CAST:
            return "CI_EXPR_KIND_CAST";
        case CI_EXPR_KIND_DATA_TYPE:
            return "CI_EXPR_KIND_DATA_TYPE";
        case CI_EXPR_KIND_FUNCTION_CALL:
            return "CI_EXPR_KIND_FUNCTION_CALL";
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            return "CI_EXPR_KIND_FUNCTION_CALL_BUILTIN";
        case CI_EXPR_KIND_GROUPING:
            return "CI_EXPR_KIND_GROUPING";
        case CI_EXPR_KIND_IDENTIFIER:
            return "CI_EXPR_KIND_IDENTIFIER";
        case CI_EXPR_KIND_INITIALIZER:
            return "CI_EXPR_KIND_INITIALIZER";
        case CI_EXPR_KIND_LITERAL:
            return "CI_EXPR_KIND_LITERAL";
        case CI_EXPR_KIND_NULLPTR:
            return "CI_EXPR_KIND_NULLPTR";
        case CI_EXPR_KIND_SIZEOF:
            return "CI_EXPR_KIND_SIZEOF";
        case CI_EXPR_KIND_TERNARY:
            return "CI_EXPR_KIND_TERNARY";
        case CI_EXPR_KIND_UNARY:
            return "CI_EXPR_KIND_UNARY";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(CIExpr *, CIExpr, enum CIExprKind kind)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = kind;
    self->ref_count = 0;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, alignof, CIExpr *alignof_)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_ALIGNOF;
    self->ref_count = 0;
    self->alignof_ = alignof_;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *,
                    CIExpr,
                    array_access,
                    CIExprArrayAccess array_access)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_ARRAY_ACCESS;
    self->ref_count = 0;
    self->array_access = array_access;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, binary, CIExprBinary binary)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_BINARY;
    self->ref_count = 0;
    self->binary = binary;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, cast, CIExprCast cast)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_CAST;
    self->ref_count = 0;
    self->cast = cast;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, data_type, CIDataType *data_type)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_DATA_TYPE;
    self->ref_count = 0;
    self->data_type = data_type;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *,
                    CIExpr,
                    function_call,
                    CIExprFunctionCall function_call)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_FUNCTION_CALL;
    self->ref_count = 0;
    self->function_call = function_call;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *,
                    CIExpr,
                    function_call_builtin,
                    CIExprFunctionCallBuiltin function_call_builtin)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_FUNCTION_CALL_BUILTIN;
    self->ref_count = 0;
    self->function_call_builtin = function_call_builtin;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, grouping, CIExpr *grouping)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_GROUPING;
    self->ref_count = 0;
    self->grouping = grouping;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, identifier, CIExprIdentifier identifier)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_IDENTIFIER;
    self->ref_count = 0;
    self->identifier = identifier;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *,
                    CIExpr,
                    initializer,
                    CIExprInitializer initializer)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_INITIALIZER;
    self->ref_count = 0;
    self->initializer = initializer;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, literal, CIExprLiteral literal)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_LITERAL;
    self->ref_count = 0;
    self->literal = literal;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, sizeof, CIExpr *sizeof_)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_SIZEOF;
    self->ref_count = 0;
    self->sizeof_ = sizeof_;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, ternary, CIExprTernary ternary)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_TERNARY;
    self->ref_count = 0;
    self->ternary = ternary;

    return self;
}

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, unary, CIExprUnary unary)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_UNARY;
    self->ref_count = 0;
    self->unary = unary;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExpr, const CIExpr *self)
{
    switch (self->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            return format__String("CIExpr{{ kind = {s}, alignof_ = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExpr(self->alignof_));
        case CI_EXPR_KIND_ARRAY_ACCESS:
            return format__String(
              "CIExpr{{ kind = {s}, array_access = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprArrayAccess(&self->array_access));
        case CI_EXPR_KIND_BINARY:
            return format__String(
              "CIExpr{{ kind = {s}, binary = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprBinary(&self->binary));
        case CI_EXPR_KIND_CAST:
            return format__String("CIExpr{{ kind = {s}, cast = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExprCast(&self->cast));
        case CI_EXPR_KIND_DATA_TYPE:
            return format__String(
              "CIExpr{{ kind = {s}, data_type = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIDataType(self->data_type));
        case CI_EXPR_KIND_FUNCTION_CALL:
            return format__String(
              "CIExpr{{ kind = {s}, function_call = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprFunctionCall(&self->function_call));
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            return format__String(
              "CIExpr{{ kind = {s}, function_call_builtin = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprFunctionCallBuiltin(
                &self->function_call_builtin));
        case CI_EXPR_KIND_GROUPING:
            return format__String("CIExpr{{ kind = {s}, grouping = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExpr(self->grouping));
        case CI_EXPR_KIND_IDENTIFIER:
            return format__String(
              "CIExpr{{ kind = {s}, identifier = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprIdentifier(&self->identifier));
        case CI_EXPR_KIND_INITIALIZER:
            return format__String(
              "CIExpr{{ kind = {s}, initializer = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprInitializer(&self->initializer));
        case CI_EXPR_KIND_LITERAL:
            return format__String(
              "CIExpr{{ kind = {s}, literal = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprLiteral(&self->literal));
        case CI_EXPR_KIND_NULLPTR:
            return format__String("CIExpr{{ kind = {s} }",
                                  to_string__Debug__CIExprKind(self->kind));
        case CI_EXPR_KIND_SIZEOF:
            return format__String("CIExpr{{ kind = {s}, sizeof_ = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExpr(self->sizeof_));
        case CI_EXPR_KIND_TERNARY:
            return format__String(
              "CIExpr{{ kind = {s}, ternary = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprTernary(&self->ternary));
        case CI_EXPR_KIND_UNARY:
            return format__String("CIExpr{{ kind = {s}, unary = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExprUnary(&self->unary));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

Uint8
to_precedence__CIExpr(const CIExpr *self)
{
    switch (self->kind) {
        case CI_EXPR_KIND_BINARY:
            return to_precedence__CIExprBinaryKind(self->binary.kind);
        case CI_EXPR_KIND_UNARY:
            return to_precedence__CIExprUnaryKind(self->unary.kind);
        case CI_EXPR_KIND_CAST:
        case CI_EXPR_KIND_SIZEOF:
        case CI_EXPR_KIND_ALIGNOF:
            return EXPR_PRECEDENCE_LEVEL_2;
        case CI_EXPR_KIND_ARRAY_ACCESS:
        case CI_EXPR_KIND_FUNCTION_CALL:
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            return EXPR_PRECEDENCE_LEVEL_1;
        case CI_EXPR_KIND_TERNARY:
            return EXPR_PRECEDENCE_LEVEL_13;
        default:
            return MAX_CI_EXPR_PRECEDENCE + 1;
    }
}

VARIANT_DESTRUCTOR(CIExpr, alignof, CIExpr *self)
{
    FREE(CIExpr, self->alignof_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, array_access, CIExpr *self)
{
    FREE(CIExprArrayAccess, &self->array_access);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, binary, CIExpr *self)
{
    FREE(CIExprBinary, &self->binary);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, cast, CIExpr *self)
{
    FREE(CIExprCast, &self->cast);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, data_type, CIExpr *self)
{
    FREE(CIDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, function_call, CIExpr *self)
{
    FREE(CIExprFunctionCall, &self->function_call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, function_call_builtin, CIExpr *self)
{
    FREE(CIExprFunctionCallBuiltin, &self->function_call_builtin);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, grouping, CIExpr *self)
{
    FREE(CIExpr, self->grouping);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, identifier, CIExpr *self)
{
    FREE(CIExprIdentifier, &self->identifier);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, initializer, CIExpr *self)
{
    FREE(CIExprInitializer, &self->initializer);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, literal, CIExpr *self)
{
    FREE(CIExprLiteral, &self->literal);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, sizeof, CIExpr *self)
{
    FREE(CIExpr, self->sizeof_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, ternary, CIExpr *self)
{
    FREE(CIExprTernary, &self->ternary);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIExpr, unary, CIExpr *self)
{
    FREE(CIExprUnary, &self->unary);
    lily_free(self);
}

DESTRUCTOR(CIExpr, CIExpr *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            FREE_VARIANT(CIExpr, alignof, self);
            break;
        case CI_EXPR_KIND_ARRAY_ACCESS:
            FREE_VARIANT(CIExpr, array_access, self);
            break;
        case CI_EXPR_KIND_BINARY:
            FREE_VARIANT(CIExpr, binary, self);
            break;
        case CI_EXPR_KIND_CAST:
            FREE_VARIANT(CIExpr, cast, self);
            break;
        case CI_EXPR_KIND_DATA_TYPE:
            FREE_VARIANT(CIExpr, data_type, self);
            break;
        case CI_EXPR_KIND_FUNCTION_CALL:
            FREE_VARIANT(CIExpr, function_call, self);
            break;
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            FREE_VARIANT(CIExpr, function_call_builtin, self);
            break;
        case CI_EXPR_KIND_GROUPING:
            FREE_VARIANT(CIExpr, grouping, self);
            break;
        case CI_EXPR_KIND_IDENTIFIER:
            FREE_VARIANT(CIExpr, identifier, self);
            break;
        case CI_EXPR_KIND_INITIALIZER:
            FREE_VARIANT(CIExpr, initializer, self);
            break;
        case CI_EXPR_KIND_LITERAL:
            FREE_VARIANT(CIExpr, literal, self);
            break;
        case CI_EXPR_KIND_NULLPTR:
            lily_free(self);
            break;
        case CI_EXPR_KIND_SIZEOF:
            FREE_VARIANT(CIExpr, sizeof, self);
            break;
        case CI_EXPR_KIND_TERNARY:
            FREE_VARIANT(CIExpr, ternary, self);
            break;
        case CI_EXPR_KIND_UNARY:
            FREE_VARIANT(CIExpr, unary, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIStmtKind, enum CIStmtKind self)
{
    switch (self) {
        case CI_STMT_KIND_BLOCK:
            return "CI_STMT_KIND_BLOCK";
        case CI_STMT_KIND_BREAK:
            return "CI_STMT_KIND_BREAK";
        case CI_STMT_KIND_CASE:
            return "CI_STMT_KIND_CASE";
        case CI_STMT_KIND_CONTINUE:
            return "CI_STMT_KIND_CONTINUE";
        case CI_STMT_KIND_DEFAULT:
            return "CI_STMT_KIND_DEFAULT";
        case CI_STMT_KIND_DO_WHILE:
            return "CI_STMT_KIND_DO_WHILE";
        case CI_STMT_KIND_FOR:
            return "CI_STMT_KIND_FOR";
        case CI_STMT_KIND_GOTO:
            return "CI_STMT_KIND_GOTO";
        case CI_STMT_KIND_IF:
            return "CI_STMT_KIND_IF";
        case CI_STMT_KIND_RETURN:
            return "CI_STMT_KIND_RETURN";
        case CI_STMT_KIND_SWITCH:
            return "CI_STMT_KIND_SWITCH";
        case CI_STMT_KIND_WHILE:
            return "CI_STMT_KIND_WHILE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtBlock, const CIStmtBlock *self)
{
    return format__String("CIStmtBlock{{ body = {Sr} }",
                          to_string__Debug__CIDeclFunctionBody(self->body));
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtDoWhile, const CIStmtDoWhile *self)
{
    return format__String("CIStmtDoWhile{{ body = {Sr}, cond = {Sr}",
                          to_string__Debug__CIDeclFunctionBody(self->body),
                          to_string__Debug__CIExpr(self->cond));
}
#endif

DESTRUCTOR(CIStmtDoWhile, const CIStmtDoWhile *self)
{
    FREE(CIDeclFunctionBody, self->body);
    FREE(CIExpr, self->cond);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtFor, const CIStmtFor *self)
{
    String *res =
      format__String("CIStmtFor{{ body = {Sr}, init_clause =",
                     to_string__Debug__CIDeclFunctionBody(self->body));

    if (self->init_clauses) {
        DEBUG_VEC_STRING(self->exprs2, res, CIDeclFunctionItem);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", expr1 =");

    if (self->expr1) {
        String *s = to_string__Debug__CIExpr(self->expr1);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", exprs2 =");

    if (self->exprs2) {
        DEBUG_VEC_STRING(self->exprs2, res, CIExpr);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIStmtFor, const CIStmtFor *self)
{
    FREE(CIDeclFunctionBody, self->body);

    if (self->init_clauses) {
        FREE_BUFFER_ITEMS(self->init_clauses->buffer,
                          self->init_clauses->len,
                          CIDeclFunctionItem);
        FREE(Vec, self->init_clauses);
    }

    if (self->expr1) {
        FREE(CIExpr, self->expr1);
    }

    if (self->exprs2) {
        FREE_BUFFER_ITEMS(self->exprs2->buffer, self->exprs2->len, CIExpr);
        FREE(Vec, self->exprs2);
    }
}

CONSTRUCTOR(CIStmtIfBranch *,
            CIStmtIfBranch,
            CIExpr *cond,
            CIDeclFunctionBody *body)
{
    CIStmtIfBranch *self = lily_malloc(sizeof(CIStmtIfBranch));

    self->cond = cond;
    self->body = body;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtIfBranch, const CIStmtIfBranch *self)
{
    return format__String("CIStmtIfBranch{{ body = {Sr}, cond = {Sr}",
                          to_string__Debug__CIDeclFunctionBody(self->body),
                          to_string__Debug__CIExpr(self->cond));
}
#endif

DESTRUCTOR(CIStmtIfBranch, CIStmtIfBranch *self)
{
    FREE(CIDeclFunctionBody, self->body);
    FREE(CIExpr, self->cond);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtIf, const CIStmtIf *self)
{
    String *res = format__String("CIStmtIf{{ if = {Sr}",
                                 to_string__Debug__CIStmtIfBranch(self->if_));

    push_str__String(res, ", else_ifs =");

    if (self->else_ifs) {
        DEBUG_VEC_STRING(self->else_ifs, res, CIStmtIfBranch);
    } else {
        push_str__String(res, " NULL");
    }

    {
        String *s = format__String(
          ", else_ = {Sr} }",
          self->else_ ? to_string__Debug__CIDeclFunctionBody(self->else_)
                      : from__String("NULL"));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIStmtIf, const CIStmtIf *self)
{
    FREE(CIStmtIfBranch, self->if_);

    if (self->else_ifs) {
        FREE_BUFFER_ITEMS(
          self->else_ifs->buffer, self->else_ifs->len, CIStmtIfBranch);
        FREE(Vec, self->else_ifs);
    }

    if (self->else_) {
        FREE(CIDeclFunctionBody, self->else_);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtSwitchCase, const CIStmtSwitchCase *self)
{
    return format__String("CIStmtSwitchCase{{ value = {Sr} }",
                          to_string__Debug__CIExpr(self->value));
}
#endif

DESTRUCTOR(CIStmtSwitchCase, const CIStmtSwitchCase *self)
{
    FREE(CIExpr, self->value);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtSwitch, const CIStmtSwitch *self)
{
    return format__String("CIStmtSwitch{{ expr = {Sr}, body = {Sr} }",
                          to_string__Debug__CIExpr(self->expr),
                          to_string__Debug__CIDeclFunctionBody(self->body));
}
#endif

DESTRUCTOR(CIStmtSwitch, const CIStmtSwitch *self)
{
    FREE(CIExpr, self->expr);
    FREE(CIDeclFunctionBody, self->body);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmtWhile, const CIStmtWhile *self)
{
    return format__String("CIStmtWhile{{ body = {Sr}, cond = {Sr} }",
                          to_string__Debug__CIDeclFunctionBody(self->body),
                          to_string__Debug__CIExpr(self->cond));
}
#endif

DESTRUCTOR(CIStmtWhile, const CIStmtWhile *self)
{
    FREE(CIDeclFunctionBody, self->body);
    FREE(CIExpr, self->cond);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIStmt, const CIStmt *self)
{
    switch (self->kind) {
        case CI_STMT_KIND_BLOCK:
            return format__String("CIStmt{{ kind = {s}, block = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIStmtBlock(&self->block));
        case CI_STMT_KIND_BREAK:
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_DEFAULT:
            return format__String("CIStmt{ kind = {s} }",
                                  to_string__Debug__CIStmtKind(self->kind));
        case CI_STMT_KIND_CASE:
            return format__String(
              "CIStmt{{ kind = {s}, case_ = {Sr} }",
              to_string__Debug__CIStmtKind(self->kind),
              to_string__Debug__CIStmtSwitchCase(&self->case_));
        case CI_STMT_KIND_DO_WHILE:
            return format__String(
              "CIStmt{{ kind = {s}, do_while = {Sr} }",
              to_string__Debug__CIStmtKind(self->kind),
              to_string__Debug__CIStmtDoWhile(&self->do_while));
        case CI_STMT_KIND_FOR:
            return format__String("CIStmt{{ kind = {s}, for_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIStmtFor(&self->for_));
        case CI_STMT_KIND_GOTO:
            return format__String("CIStmt{{ kind = {s}, goto_ = {S} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  GET_PTR_RC(String, self->goto_));
        case CI_STMT_KIND_IF:
            return format__String("CIStmt{{ kind = {s}, if_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIStmtIf(&self->if_));
        case CI_STMT_KIND_RETURN:
            return format__String("CIStmt{{ kind = {s}, return_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  self->return_
                                    ? to_string__Debug__CIExpr(self->return_)
                                    : from__String("NULL"));
        case CI_STMT_KIND_SWITCH:
            return format__String(
              "CIStmt{{ kind = {s}, switch_ = {Sr} }",
              to_string__Debug__CIStmtKind(self->kind),
              to_string__Debug__CIStmtSwitch(&self->switch_));
        case CI_STMT_KIND_WHILE:
            return format__String("CIStmt{{ kind = {s}, while_ = {Sr} }",
                                  to_string__Debug__CIStmtKind(self->kind),
                                  to_string__Debug__CIStmtWhile(&self->while_));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIStmt, block, const CIStmt *self)
{
    FREE(CIStmtBlock, &self->block);
}

VARIANT_DESTRUCTOR(CIStmt, case, const CIStmt *self)
{
    FREE(CIStmtSwitchCase, &self->case_);
}

VARIANT_DESTRUCTOR(CIStmt, do_while, const CIStmt *self)
{
    FREE(CIStmtDoWhile, &self->do_while);
}

VARIANT_DESTRUCTOR(CIStmt, for, const CIStmt *self)
{
    FREE(CIStmtFor, &self->for_);
}

VARIANT_DESTRUCTOR(CIStmt, goto, const CIStmt *self)
{
    FREE_RC(String, self->goto_);
}

VARIANT_DESTRUCTOR(CIStmt, if, const CIStmt *self)
{
    FREE(CIStmtIf, &self->if_);
}

VARIANT_DESTRUCTOR(CIStmt, return, const CIStmt *self)
{
    if (self->return_) {
        FREE(CIExpr, self->return_);
    }
}

VARIANT_DESTRUCTOR(CIStmt, switch, const CIStmt *self)
{
    FREE(CIStmtSwitch, &self->switch_);
}

VARIANT_DESTRUCTOR(CIStmt, while, const CIStmt *self)
{
    FREE(CIStmtWhile, &self->while_);
}

DESTRUCTOR(CIStmt, const CIStmt *self)
{
    switch (self->kind) {
        case CI_STMT_KIND_BLOCK:
            FREE_VARIANT(CIStmt, block, self);
            break;
        case CI_STMT_KIND_BREAK:
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_DEFAULT:
            break;
        case CI_STMT_KIND_CASE:
            FREE_VARIANT(CIStmt, case, self);
            break;
        case CI_STMT_KIND_DO_WHILE:
            FREE_VARIANT(CIStmt, do_while, self);
            break;
        case CI_STMT_KIND_FOR:
            FREE_VARIANT(CIStmt, for, self);
            break;
        case CI_STMT_KIND_GOTO:
            FREE_VARIANT(CIStmt, goto, self);
            break;
        case CI_STMT_KIND_IF:
            FREE_VARIANT(CIStmt, if, self);
            break;
        case CI_STMT_KIND_RETURN:
            FREE_VARIANT(CIStmt, return, self);
            break;
        case CI_STMT_KIND_SWITCH:
            FREE_VARIANT(CIStmt, switch, self);
            break;
        case CI_STMT_KIND_WHILE:
            FREE_VARIANT(CIStmt, while, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclFunctionItemKind,
               enum CIDeclFunctionItemKind self)
{
    switch (self) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            return "CI_DECL_FUNCTION_ITEM_KIND_DECL";
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            return "CI_DECL_FUNCTION_ITEM_KIND_EXPR";
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            return "CI_DECL_FUNCTION_ITEM_KIND_STMT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(CIDeclFunctionItem *,
                    CIDeclFunctionItem,
                    decl,
                    CIDecl *decl)
{
    CIDeclFunctionItem *self = lily_malloc(sizeof(CIDeclFunctionItem));

    self->kind = CI_DECL_FUNCTION_ITEM_KIND_DECL;
    self->decl = decl;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclFunctionItem *,
                    CIDeclFunctionItem,
                    expr,
                    CIExpr *expr)
{
    CIDeclFunctionItem *self = lily_malloc(sizeof(CIDeclFunctionItem));

    self->kind = CI_DECL_FUNCTION_ITEM_KIND_EXPR;
    self->expr = expr;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclFunctionItem *, CIDeclFunctionItem, stmt, CIStmt stmt)
{
    CIDeclFunctionItem *self = lily_malloc(sizeof(CIDeclFunctionItem));

    self->kind = CI_DECL_FUNCTION_ITEM_KIND_STMT;
    self->stmt = stmt;

    return self;
}

bool
is_for_init_clause__CIDeclFunctionItem(const CIDeclFunctionItem *self)
{
    ASSERT(self);

    switch (self->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            if (self->expr->kind == CI_EXPR_KIND_BINARY) {
                switch (self->expr->binary.kind) {
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
                        return true;
                    default:
                        return false;
                }
            }

            return false;
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            return self->decl->kind == CI_DECL_KIND_VARIABLE;
        default:
            return false;
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionItem, const CIDeclFunctionItem *self)
{
    switch (self->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            return format__String(
              "CIDeclFunctionItem{{ kind = {s}, decl = {Sr} }",
              to_string__Debug__CIDeclFunctionItemKind(self->kind),
              to_string__Debug__CIDecl(self->decl));
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            return format__String(
              "CIDeclFunctionItem{{ kind = {s}, expr = {Sr} }",
              to_string__Debug__CIDeclFunctionItemKind(self->kind),
              to_string__Debug__CIExpr(self->expr));
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            return format__String(
              "CIDeclFunctionItem{{ kind = {s}, stmt = {Sr} }",
              to_string__Debug__CIDeclFunctionItemKind(self->kind),
              to_string__Debug__CIStmt(&self->stmt));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(CIDeclFunctionItem, decl, CIDeclFunctionItem *self)
{
    FREE(CIDecl, self->decl);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclFunctionItem, expr, CIDeclFunctionItem *self)
{
    FREE(CIExpr, self->expr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDeclFunctionItem, stmt, CIDeclFunctionItem *self)
{
    FREE(CIStmt, &self->stmt);
    lily_free(self);
}

DESTRUCTOR(CIDeclFunctionItem, CIDeclFunctionItem *self)
{
    switch (self->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            FREE_VARIANT(CIDeclFunctionItem, decl, self);
            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            FREE_VARIANT(CIDeclFunctionItem, expr, self);
            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            FREE_VARIANT(CIDeclFunctionItem, stmt, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
