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

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND__ATOMIC).
static VARIANT_DESTRUCTOR(CIDataType, _atomic, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_BUILTIN).
static inline VARIANT_DESTRUCTOR(CIDataType, builtin, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_ENUM).
static inline VARIANT_DESTRUCTOR(CIDataType, enum, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_GENERIC).
static inline VARIANT_DESTRUCTOR(CIDataType, generic, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_PRE_CONST).
static VARIANT_DESTRUCTOR(CIDataType, pre_const, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_POST_CONST).
static VARIANT_DESTRUCTOR(CIDataType, post_const, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(CIDataType, ptr, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_STRUCT).
static VARIANT_DESTRUCTOR(CIDataType, struct, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_TYPEDEF).
static VARIANT_DESTRUCTOR(CIDataType, typedef, CIDataType *self);

/// @brief Free CIDataType type (CI_DATA_TYPE_KIND_UNION).
static VARIANT_DESTRUCTOR(CIDataType, union, CIDataType *self);

static const CISizeInfo *
get_size_info__CIDecl(const CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_ENUM).
static VARIANT_DESTRUCTOR(CIDecl, enum, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIDecl, function, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_FUNCTION_GEN).
static VARIANT_DESTRUCTOR(CIDecl, function_gen, CIDecl *self);

/// @brief Free CIDecl type (CI_DECL_KIND_LABEL).
static inline VARIANT_DESTRUCTOR(CIDecl, label, CIDecl *self);

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

/// @brief Free CIExpr type (CI_EXPR_KIND_ARRAY).
static VARIANT_DESTRUCTOR(CIExpr, array, CIExpr *self);

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
static inline VARIANT_DESTRUCTOR(CIExpr, identifier, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_LITERAL).
static VARIANT_DESTRUCTOR(CIExpr, literal, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_SIZEOF).
static VARIANT_DESTRUCTOR(CIExpr, sizeof, CIExpr *self);

/// @brief Free CIExpr type (CI_EXPR_KIND_STRUCT_CALL).
static VARIANT_DESTRUCTOR(CIExpr, struct_call, CIExpr *self);

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

#define CI_SERIALIZE_NAME(name)                                    \
    ASSERT(called_generic_params);                                 \
                                                                   \
    String *res = format__String("{S}__", name);                   \
                                                                   \
    serialize_vec__CIDataType(called_generic_params->params, res); \
                                                                   \
    return res;

CONSTRUCTOR(CIScopeID *, CIScopeID, Usize id)
{
    CIScopeID *self = lily_malloc(sizeof(CIScopeID));

    self->id = id;

    return self;
}

CONSTRUCTOR(CIEnumID *, CIEnumID, CIFileID file_id, Usize id)
{
    CIEnumID *self = lily_malloc(sizeof(CIEnumID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIStructID *, CIStructID, CIFileID file_id, Usize id)
{
    CIStructID *self = lily_malloc(sizeof(CIStructID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CITypedefID *, CITypedefID, CIFileID file_id, Usize id)
{
    CITypedefID *self = lily_malloc(sizeof(CITypedefID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIFunctionID *, CIFunctionID, CIFileID file_id, Usize id)
{
    CIFunctionID *self = lily_malloc(sizeof(CIFunctionID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CILabelID *,
            CILabelID,
            CIFileID file_id,
            CIScopeID scope_id,
            Usize id)
{
    CILabelID *self = lily_malloc(sizeof(CILabelID));

    self->file_id = file_id;
    self->scope_id = scope_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIUnionID *, CIUnionID, CIFileID file_id, Usize id)
{
    CIUnionID *self = lily_malloc(sizeof(CIUnionID));

    self->file_id = file_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIVariableID *,
            CIVariableID,
            CIFileID file_id,
            CIScopeID scope_id,
            Usize id)
{
    CIVariableID *self = lily_malloc(sizeof(CIVariableID));

    self->file_id = file_id;
    self->scope_id = scope_id;
    self->id = id;

    return self;
}

CONSTRUCTOR(CIScope *, CIScope, CIScopeID *parent, Usize id, bool is_block)
{
    CIScope *self = lily_malloc(sizeof(CIScope));

    self->parent = parent;
    self->scope_id = NEW(CIScopeID, id);
    self->is_block = is_block;
    self->enums = NEW(HashMap);
    self->functions = NEW(HashMap);
    self->labels = NEW(HashMap);
    self->structs = NEW(HashMap);
    self->typedefs = NEW(HashMap);
    self->unions = NEW(HashMap);
    self->variables = NEW(HashMap);

    return self;
}

DESTRUCTOR(CIScope, CIScope *self)
{
    FREE(CIScopeID, self->scope_id);

    FREE_HASHMAP_VALUES(self->enums, CIEnumID);
    FREE(HashMap, self->enums);

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

    lily_free(self);
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
                if (!strcmp(param->generic->buffer, name->buffer)) {
                    return i;
                }

                break;
            default:
                break;
        }
    }

    return -1;
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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeContext, int self)
{
#define CTXS_LEN 5
    int ctxs[CTXS_LEN] = { CI_DATA_TYPE_CONTEXT_NONE,
                           CI_DATA_TYPE_CONTEXT_HEAP,
                           CI_DATA_TYPE_CONTEXT_NON_NULL,
                           CI_DATA_TYPE_CONTEXT_STACK,
                           CI_DATA_TYPE_CONTEXT_TRACE };
    char *ctxs_s[CTXS_LEN] = {
        "CI_DATA_TYPE_CONTEXT_NONE",     "CI_DATA_TYPE_CONTEXT_HEAP",
        "CI_DATA_TYPE_CONTEXT_NON_NULL", "CI_DATA_TYPE_CONTEXT_STACK",
        "CI_DATA_TYPE_CONTEXT_TRACE",
    };

    String *res = NEW(String);

    for (Usize i = 0; i < CTXS_LEN; ++i) {
        if (self & ctxs[i]) {
            push_str__String(res, ctxs_s[i]);
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
        case CI_DATA_TYPE_KIND_ARRAY:
            return "CI_DATA_TYPE_KIND_ARRAY";
        case CI_DATA_TYPE_KIND__ATOMIC:
            return "CI_DATA_TYPE_KIND__ATOMIC";
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
        case CI_DATA_TYPE_KIND_PRE_CONST:
            return "CI_DATA_TYPE_KIND_PRE_CONST";
        case CI_DATA_TYPE_KIND_POST_CONST:
            return "CI_DATA_TYPE_KIND_POST_CONST";
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
                 self->name ? self->name->buffer : "NULL",
                 self->size)
             : format__String(
                 "CIDataTypeArray{{ kind = {s}, data_type = {Sr}, name = {s} }",
                 to_string__Debug__CIDataTypeArrayKind(self->kind),
                 to_string__Debug__CIDataType(self->data_type),
                 self->name ? self->name->buffer : "NULL");
}
#endif

DESTRUCTOR(CIDataTypeArray, const CIDataTypeArray *self)
{
    FREE(CIDataType, self->data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeFunction, const CIDataTypeFunction *self)
{
    String *res = format__String("CIDataTypeFunction{{ name = {s}, params =",
                                 self->name ? self->name->buffer : "NULL");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, CIDeclFunctionParam);
    } else {
        push_str__String(res, " NULL");
    }

    {
        String *s = format__String(
          ", return_data_type = {Sr}, function_data_type = {Sr} }",
          to_string__Debug__CIDataType(self->return_data_type),
          to_string__Debug__CIDataType(self->function_data_type));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIDataTypeFunction, const CIDataTypeFunction *self)
{
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
    String *res = format__String(
      "CIDataTypeStruct{{ name = {s}, generic_params = {Sr}, fields =",
      self->name ? self->name->buffer : "NULL",
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"));

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIDataTypeStruct, const CIDataTypeStruct *self)
{
    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }

    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeTypedef, const CIDataTypeTypedef *self)
{
    return format__String(
      "CIDataTypeTypedef{{ name = {S}, generic_params = {Sr} }",
      self->name,
      to_string__Debug__CIGenericParams(self->generic_params));
}
#endif

DESTRUCTOR(CIDataTypeTypedef, const CIDataTypeTypedef *self)
{
    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataTypeUnion, const CIDataTypeUnion *self)
{
    String *res = format__String(
      "CIDataTypeUnion{{ name = {s}, generic_params = {Sr}, fields =",
      self->name ? self->name->buffer : "NULL",
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"));

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIDataTypeUnion, const CIDataTypeUnion *self)
{
    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }

    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
    }
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, array, CIDataTypeArray array)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_ARRAY;
    self->ref_count = 0;
    self->array = array;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, _atomic, CIDataType *_atomic)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND__ATOMIC;
    self->ref_count = 0;
    self->_atomic = _atomic;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, builtin, Usize builtin)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_BUILTIN;
    self->ref_count = 0;
    self->builtin = builtin;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, enum, String *enum_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_ENUM;
    self->ref_count = 0;
    self->enum_ = enum_;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

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

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, generic, String *generic)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_GENERIC;
    self->ref_count = 0;
    self->generic = generic;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, pre_const, CIDataType *pre_const)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_PRE_CONST;
    self->ref_count = 0;
    self->pre_const = pre_const;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *,
                    CIDataType,
                    post_const,
                    CIDataType *post_const)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_POST_CONST;
    self->ref_count = 0;
    self->post_const = post_const;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, ptr, CIDataType *ptr)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_PTR;
    self->ref_count = 0;
    self->ptr = ptr;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, struct, CIDataTypeStruct struct_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_STRUCT;
    self->ref_count = 0;
    self->struct_ = struct_;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

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

    return self;
}

VARIANT_CONSTRUCTOR(CIDataType *, CIDataType, union, CIDataTypeUnion union_)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = CI_DATA_TYPE_KIND_UNION;
    self->ref_count = 0;
    self->union_ = union_;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

    return self;
}

CONSTRUCTOR(CIDataType *, CIDataType, enum CIDataTypeKind kind)
{
    CIDataType *self = lily_malloc(sizeof(CIDataType));

    self->kind = kind;
    self->ref_count = 0;
    self->ctx = CI_DATA_TYPE_CONTEXT_NONE;

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
        case CI_DATA_TYPE_KIND__ATOMIC:
            res = NEW_VARIANT(
              CIDataType, _atomic, clone__CIDataType(self->_atomic));

            break;
        case CI_DATA_TYPE_KIND_BUILTIN:
            res = NEW_VARIANT(CIDataType, builtin, self->builtin);

            break;
        case CI_DATA_TYPE_KIND_ENUM:
            res = NEW_VARIANT(CIDataType, enum, self->enum_);

            break;
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
        case CI_DATA_TYPE_KIND_PRE_CONST:
            res = NEW_VARIANT(
              CIDataType, pre_const, clone__CIDataType(self->pre_const));

            break;
        case CI_DATA_TYPE_KIND_POST_CONST:
            res = NEW_VARIANT(
              CIDataType, pre_const, clone__CIDataType(self->post_const));

            break;
        case CI_DATA_TYPE_KIND_PTR:
            res = NEW_VARIANT(
              CIDataType, ptr, self->ptr ? clone__CIDataType(self->ptr) : NULL);

            break;
        case CI_DATA_TYPE_KIND_STRUCT: {
            Vec *fields = NULL;

            if (self->struct_.fields) {
                fields = NEW(Vec);

                for (Usize i = 0; i < self->struct_.fields->len; ++i) {
                    push__Vec(fields,
                              clone__CIDeclStructField(
                                get__Vec(self->struct_.fields, i)));
                }
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
            Vec *fields = NULL;

            if (self->union_.fields) {
                fields = NEW(Vec);

                for (Usize i = 0; i < self->union_.fields->len; ++i) {
                    push__Vec(fields,
                              clone__CIDeclStructField(
                                get__Vec(self->union_.fields, i)));
                }
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

    return res;
}

void
serialize__CIDataType(const CIDataType *self, String *buffer)
{
#define SERIALIZE_NAME(name, name_len) hash_sip(name, name_len, SIP_K0, SIP_K1)

#define SERIALIZE_TYPE_WITH_GENERIC_PARAMS(ty)                            \
    {                                                                     \
        push_str__String(buffer, ty.name->buffer);                        \
                                                                          \
        if (ty.generic_params) {                                          \
            serialize_vec__CIDataType(ty.generic_params->params, buffer); \
        }                                                                 \
    }

#define SERIALIZE_FMT_PUSH_TO_BUFFER(fmt, ...) \
    {                                          \
        char *s = format(fmt, __VA_ARGS__);    \
        PUSH_STR_AND_FREE(buffer, s);          \
    }

    SERIALIZE_FMT_PUSH_TO_BUFFER("{d}", self->kind);

    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            serialize__CIDataType(self->array.data_type, buffer);

            break;
        case CI_DATA_TYPE_KIND__ATOMIC:
            serialize__CIDataType(self->_atomic, buffer);

            break;
        case CI_DATA_TYPE_KIND_BUILTIN:
            SERIALIZE_FMT_PUSH_TO_BUFFER("{zu}", self->builtin);

            break;
        case CI_DATA_TYPE_KIND_ENUM:
            push_str__String(buffer, self->enum_->buffer);

            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            serialize__CIDataType(self->function.return_data_type, buffer);
            serialize_vec__CIDataType(self->function.params, buffer);

            break;
        case CI_DATA_TYPE_KIND_GENERIC:
            SERIALIZE_FMT_PUSH_TO_BUFFER(
              "{zu}",
              SERIALIZE_NAME(self->generic->buffer, self->generic->len));

            break;
        case CI_DATA_TYPE_KIND_PRE_CONST:
            serialize__CIDataType(self->pre_const, buffer);

            break;
        case CI_DATA_TYPE_KIND_POST_CONST:
            serialize__CIDataType(self->post_const, buffer);

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
    if (self->kind != other->kind || self->ctx != other->ctx) {
        return false;
    }

    switch (self->kind) {
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
        case CI_DATA_TYPE_KIND__ATOMIC:
            return eq__CIDataType(self->_atomic, other->_atomic);
        case CI_DATA_TYPE_KIND_BUILTIN:
            return self->builtin == other->builtin;
        case CI_DATA_TYPE_KIND_ENUM:
            return !strcmp(self->enum_->buffer, other->enum_->buffer);
        case CI_DATA_TYPE_KIND_FUNCTION:
            TODO("eq__CIDataType: function data_type");
        case CI_DATA_TYPE_KIND_GENERIC:
            return !strcmp(self->generic->buffer, other->generic->buffer);
        case CI_DATA_TYPE_KIND_PRE_CONST:
            return eq__CIDataType(self->pre_const, other->pre_const);
        case CI_DATA_TYPE_KIND_POST_CONST:
            return eq__CIDataType(self->post_const, other->post_const);
        case CI_DATA_TYPE_KIND_PTR:
            return self->ptr && other->ptr
                     ? eq__CIDataType(self->ptr, other->ptr)
                     : false;
        case CI_DATA_TYPE_KIND_STRUCT:
            if ((!self->struct_.fields || !other->struct_.fields) ||
                self->struct_.fields->len != other->struct_.fields->len ||
                (bool)self->struct_.generic_params ==
                  (bool)other->struct_.generic_params) {
                return false;
            }

            return eq__CIDeclStructField(self->struct_.fields,
                                         other->struct_.fields);
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return !strcmp(self->typedef_.name->buffer,
                           other->typedef_.name->buffer) &&
                   eq__CIGenericParams(self->typedef_.generic_params,
                                       other->typedef_.generic_params);
        case CI_DATA_TYPE_KIND_UNION:
            TODO("eq__CIDataType: union data type");
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
        case CI_DATA_TYPE_KIND_PRE_CONST:
            return get_ptr__CIDataType(self->pre_const);
        case CI_DATA_TYPE_KIND_POST_CONST:
            return get_ptr__CIDataType(self->post_const);
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

const Vec *
get_fields__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
            return self->struct_.fields;
        case CI_DATA_TYPE_KIND_UNION:
            return self->union_.fields;
        default:
            UNREACHABLE("cannot get fields from data type");
    }
}

String *
get_name__CIDataType(const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ENUM:
            return self->enum_;
        case CI_DATA_TYPE_KIND_STRUCT:
            return self->struct_.name;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return self->typedef_.name;
        case CI_DATA_TYPE_KIND_UNION:
            return self->union_.name;
        default:
            UNREACHABLE("cannot get name from data type");
    }
}

String *
serialize_name__CIDataType(const CIDataType *self)
{
    const CIGenericParams *called_generic_params =
      get_generic_params__CIDataType(self);
    String *name = get_name__CIDataType(self);

    if (!called_generic_params || !name) {
        UNREACHABLE("called_generic_params is NULL or name is NULL");
    }

    CI_SERIALIZE_NAME(name);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDataType, const CIDataType *self)
{
    switch (self->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, array = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeArray(&self->array));
        case CI_DATA_TYPE_KIND__ATOMIC:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, _atomic = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataType(self->_atomic));
        case CI_DATA_TYPE_KIND_BUILTIN:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, builtin = {zu} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              self->builtin);
        case CI_DATA_TYPE_KIND_ENUM:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, enum = {S} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              self->enum_);
        case CI_DATA_TYPE_KIND_FUNCTION:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, function = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeFunction(&self->function));
        case CI_DATA_TYPE_KIND_GENERIC:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, generic = {S} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              self->generic);
        case CI_DATA_TYPE_KIND_PRE_CONST:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, pre_const = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataType(self->pre_const));
        case CI_DATA_TYPE_KIND_POST_CONST:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, post_const = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataType(self->post_const));
        case CI_DATA_TYPE_KIND_PTR:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, ptr = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              self->ptr ? to_string__Debug__CIDataType(self->ptr)
                        : from__String("NULL"));
        case CI_DATA_TYPE_KIND_STRUCT:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, struct_ = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeStruct(&self->struct_));
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, typedef_ = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeTypedef(&self->typedef_));
        case CI_DATA_TYPE_KIND_UNION:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr}, union_ = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx),
              to_string__Debug__CIDataTypeUnion(&self->union_));
        default:
            return format__String(
              "CIDataType{{ kind = {s}, ctx = {Sr} }",
              to_string__Debug__CIDataTypeKind(self->kind),
              to_string__Debug__CIDataTypeContext(self->ctx));
    }
}
#endif

VARIANT_DESTRUCTOR(CIDataType, array, CIDataType *self)
{
    FREE(CIDataTypeArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, _atomic, CIDataType *self)
{
    FREE(CIDataType, self->_atomic);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, builtin, CIDataType *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, enum, CIDataType *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, function, CIDataType *self)
{
    FREE(CIDataTypeFunction, &self->function);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, generic, CIDataType *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, pre_const, CIDataType *self)
{
    FREE(CIDataType, self->pre_const);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIDataType, post_const, CIDataType *self)
{
    FREE(CIDataType, self->post_const);
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
        case CI_DATA_TYPE_KIND__ATOMIC:
            FREE_VARIANT(CIDataType, _atomic, self);
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
        case CI_DATA_TYPE_KIND_PRE_CONST:
            FREE_VARIANT(CIDataType, pre_const, self);
            break;
        case CI_DATA_TYPE_KIND_POST_CONST:
            FREE_VARIANT(CIDataType, post_const, self);
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
              self->deprecated);
        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
            return format__String(
              "CIAttributeStandard{{ kind = {s}, nodiscard = {S} }",
              to_string__Debug__CIAttributeStandardKind(self->kind),
              self->nodiscard);
        default:
            return format__String(
              "CIAttributeStandard{{ kind = {s} }",
              to_string__Debug__CIAttributeStandardKind(self->kind));
    }
}
#endif

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

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIStorageClass, int storage_class_flag)
{
    switch (storage_class_flag) {
        case CI_STORAGE_CLASS_NONE:
            return "CI_STORAGE_CLASS_NONE";
        case CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_CONSTEXPR:
            return "CI_STORAGE_CLASS_CONSTEXPR";
        case CI_STORAGE_CLASS_EXTERN:
            return "CI_STORAGE_CLASS_EXTERN";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO | "
                   "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC | "
                   "CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC | "
                   "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO | CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC | "
                   "CI_STORAGE_CLASS_AUTO | CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_INLINE:
            return "CI_STORAGE_CLASS_INLINE";
        case CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_REGISTER | CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_REGISTER | CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_STATIC:
            return "CI_STORAGE_CLASS_STATIC";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONSTEXPR:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONSTEXPR";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_EXTERN:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_EXTERN";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_INLINE:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_INLINE";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_THREAD_LOCAL:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_THREAD_LOCAL";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_TYPEDEF:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_TYPEDEF";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO | "
                   "CI_STORAGE_CLASS_REGISTER";
        case CI_STORAGE_CLASS_THREAD_LOCAL:
            return "CI_STORAGE_CLASS_THREAD_LOCAL";
        case CI_STORAGE_CLASS_TYPEDEF:
            return "CI_STORAGE_CLASS_TYPEDEF";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

char *
to_string__CIStorageClass(int storage_class_flag)
{
    switch (storage_class_flag) {
        case CI_STORAGE_CLASS_NONE:
            return "";
        case CI_STORAGE_CLASS_AUTO:
            return "auto";
        case CI_STORAGE_CLASS_CONSTEXPR:
            return "constexpr";
        case CI_STORAGE_CLASS_EXTERN:
            return "extern";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO:
            return "extern auto";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "extern auto register";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO:
            return "extern static auto";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_REGISTER:
            return "extern static register";
        case CI_STORAGE_CLASS_EXTERN | CI_STORAGE_CLASS_STATIC |
          CI_STORAGE_CLASS_AUTO | CI_STORAGE_CLASS_REGISTER:
            return "extern static auto register";
        case CI_STORAGE_CLASS_INLINE:
            return "inline";
        case CI_STORAGE_CLASS_REGISTER:
            return "register";
        case CI_STORAGE_CLASS_REGISTER | CI_STORAGE_CLASS_AUTO:
            return "register auto";
        case CI_STORAGE_CLASS_STATIC:
            return "static";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO:
            return "static auto";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_CONSTEXPR:
            return "static constexpr";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_EXTERN:
            return "static extern";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_INLINE:
            return "static inline";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_REGISTER:
            return "static register";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_THREAD_LOCAL:
            return "static thread_local";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_TYPEDEF:
            return "static typedef";
        case CI_STORAGE_CLASS_STATIC | CI_STORAGE_CLASS_AUTO |
          CI_STORAGE_CLASS_REGISTER:
            return "static auto register";
        case CI_STORAGE_CLASS_THREAD_LOCAL:
            return "thread_local";
        case CI_STORAGE_CLASS_TYPEDEF:
            return "typedef";
        default:
            UNREACHABLE("unknown variant");
    }
}

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

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIDeclEnumVariantKind,
               enum CIDeclEnumVariantKind self)
{
    switch (self) {
        case CI_DECL_ENUM_VARIANT_KIND_CUSTOM:
            return "CI_DECL_ENUM_VARIANT_KIND_CUSTOM";
        case CI_DECL_ENUM_VARIANT_KIND_DEFAULT:
            return "CI_DECL_ENUM_VARIANT_KIND_DEFAULT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(CIDeclEnumVariant *,
                    CIDeclEnumVariant,
                    custom,
                    String *name,
                    Isize value)
{
    CIDeclEnumVariant *self = lily_malloc(sizeof(CIDeclEnumVariant));

    self->kind = CI_DECL_ENUM_VARIANT_KIND_CUSTOM;
    self->name = name;
    self->value = value;

    return self;
}

VARIANT_CONSTRUCTOR(CIDeclEnumVariant *,
                    CIDeclEnumVariant,
                    default,
                    String *name)
{
    CIDeclEnumVariant *self = lily_malloc(sizeof(CIDeclEnumVariant));

    self->kind = CI_DECL_ENUM_VARIANT_KIND_DEFAULT;
    self->name = name;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnumVariant, const CIDeclEnumVariant *self)
{
    switch (self->kind) {
        case CI_DECL_ENUM_VARIANT_KIND_CUSTOM:
            // TODO: change {d} format specifier.
            return format__String(
              "CIDeclEnumVariant{{ kind = {s}, name = {S}, value = {d} }",
              to_string__Debug__CIDeclEnumVariantKind(self->kind),
              self->name,
              self->value);
        case CI_DECL_ENUM_VARIANT_KIND_DEFAULT:
            return format__String(
              "CIDeclEnumVariant{{ kind = {s}, name = {S} }",
              to_string__Debug__CIDeclEnumVariantKind(self->kind),
              self->name);
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

bool
match_prototype__CIDeclEnum(const CIDeclEnum *self, const CIDeclEnum *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(self->name->buffer, other->name->buffer));
    ASSERT(!self->variants && !other->variants);

    return (self->data_type && other->data_type &&
            eq__CIDataType(self->data_type, other->data_type)) ||
           (!self->data_type && !other->data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclEnum, const CIDeclEnum *self)
{
    String *res =
      format__String("CIDeclEnum{{ name = {S}, variants =", self->name);

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

DESTRUCTOR(CIDeclEnum, const CIDeclEnum *self)
{
    free_as_prototype__CIDeclEnum(self);

    if (self->variants) {
        FREE_BUFFER_ITEMS(
          self->variants->buffer, self->variants->len, CIDeclEnumVariant);
        FREE(Vec, self->variants);
    }
}

CONSTRUCTOR(CIDeclFunctionParam *,
            CIDeclFunctionParam,
            String *name,
            CIDataType *data_type)
{
    CIDeclFunctionParam *self = lily_malloc(sizeof(CIDeclFunctionParam));

    self->name = name;
    self->data_type = data_type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclFunctionParam, const CIDeclFunctionParam *self)
{
    if (self->name) {
        return format__String(
          "CIDeclFunctionParam{{ name = {S}, data_type = {Sr} }",
          self->name,
          to_string__Debug__CIDataType(self->data_type));
    }

    return format__String(
      "CIDeclFunctionParam{{ name = NULL, data_type = {Sr} }",
      to_string__Debug__CIDataType(self->data_type));
}
#endif

DESTRUCTOR(CIDeclFunctionParam, CIDeclFunctionParam *self)
{
    FREE(CIDataType, self->data_type);
    lily_free(self);
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
    CI_SERIALIZE_NAME(self->name);
}

bool
match_prototype__CIDeclFunction(const CIDeclFunction *self,
                                const CIDeclFunction *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(self->name->buffer, other->name->buffer));
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
                     self->name,
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
    return is_generic_params_contains_generic__CIDecl(
      self->called_generic_params);
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
    return format__String("CIDeclLabel{{ name = {S} }", self->name);
}
#endif

CONSTRUCTOR(CIDeclStructField *,
            CIDeclStructField,
            String *name,
            CIDataType *data_type)
{
    CIDeclStructField *self = lily_malloc(sizeof(CIDeclStructField));

    self->name = name;
    self->data_type = data_type;

    return self;
}

CIDeclStructField *
clone__CIDeclStructField(CIDeclStructField *self)
{
    return NEW(
      CIDeclStructField, self->name, clone__CIDataType(self->data_type));
}

Vec *
clone_fields__CIDeclStructField(Vec *fields)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < fields->len; ++i) {
        push__Vec(res, clone__CIDeclStructField(get__Vec(fields, i)));
    }

    return res;
}

bool
eq__CIDeclStructField(const Vec *self_fields, const Vec *other_fields)
{
    if (self_fields->len != other_fields->len) {
        return false;
    }

    for (Usize i = 0; i < self_fields->len; ++i) {
        CIDeclStructField *self_field = get__Vec(self_fields, i);
        CIDeclStructField *other_field = get__Vec(other_fields, i);

        if (!eq__CIDataType(self_field->data_type, other_field->data_type)) {
            return false;
        }
    }

    return true;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStructField, const CIDeclStructField *self)
{
    return format__String("CIDeclStructField{{ name = {s}, data_type = {Sr} }",
                          self->name ? self->name->buffer : "NULL",
                          to_string__Debug__CIDataType(self->data_type));
}
#endif

DESTRUCTOR(CIDeclStructField, CIDeclStructField *self)
{
    FREE(CIDataType, self->data_type);
    lily_free(self);
}

bool
match_prototype__CIDeclStruct(const CIDeclStruct *self,
                              const CIDeclStruct *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(self->name->buffer, other->name->buffer));
    // NOTE: Expected prototype
    ASSERT(!self->fields && !other->fields);

    return eq_generic_params__CIDeclStruct(self, other);
}

String *
serialize_name__CIDeclStruct(const CIDeclStruct *self,
                             const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(self->name);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStruct, const CIDeclStruct *self)
{
    String *res = format__String(
      "CIDeclStruct{{ name = {S}, generic_params = {Sr}, fields =",
      self->name,
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"));

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", size_info = {Sr} }",
                         to_string__Debug__CISizeInfo(&self->size_info));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

void
free_as_prototype__CIDeclStruct(const CIDeclStruct *self)
{
    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }
}

DESTRUCTOR(CIDeclStruct, const CIDeclStruct *self)
{
    free_as_prototype__CIDeclStruct(self);

    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
    }
}

bool
has_generic__CIDeclStructGen(const CIDeclStructGen *self)
{
    return is_generic_params_contains_generic__CIDecl(
      self->called_generic_params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclStructGen, const CIDeclStructGen *self)
{
    String *res = format__String(
      "CIDeclStructGen{{ struct_ = {Sr}, name = {S}, called_generic_params = "
      "{Sr}, fields =",
      to_string__Debug__CIDeclStruct(self->struct_),
      self->name,
      to_string__Debug__CIGenericParams(self->called_generic_params));

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", size_info = {Sr} }",
                         to_string__Debug__CISizeInfo(&self->size_info));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIDeclStructGen, const CIDeclStructGen *self)
{
    FREE(String, self->name);
    FREE(CIGenericParams, self->called_generic_params);

    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
    }
}

bool
match_prototype__CIDeclTypedef(const CIDeclTypedef *self,
                               const CIDeclTypedef *other)
{
    ASSERT(self->name && other->name);
    ASSERT(!strcmp(self->name->buffer, other->name->buffer));
    ASSERT(self->data_type && other->data_type);

    return eq__CIDataType(self->data_type, other->data_type) &&
           eq_generic_params__CIDeclTypedef(self, other);
}

String *
serialize_name__CIDeclTypedef(const CIDeclTypedef *self,
                              const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(self->name);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclTypedef, const CIDeclTypedef *self)
{
    return format__String(
      "CIDeclTypedef{{ name = {S}, generic_params = {Sr}, data_type = {Sr}, "
      "size_info = {Sr} }",
      self->name,
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
    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }

    FREE(CIDataType, self->data_type);
}

bool
has_generic__CIDeclTypedefGen(const CIDeclTypedefGen *self)
{
    return is_generic_params_contains_generic__CIDecl(
      self->called_generic_params);
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
    ASSERT(!strcmp(self->name->buffer, other->name->buffer));
    // NOTE: Expected prototype
    ASSERT(!self->fields && !other->fields);

    return eq_generic_params__CIDeclUnion(self, other);
}

String *
serialize_name__CIDeclUnion(const CIDeclUnion *self,
                            const CIGenericParams *called_generic_params)
{
    CI_SERIALIZE_NAME(self->name);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclUnion, const CIDeclUnion *self)
{
    String *res = format__String(
      "CIDeclUnion{{ name = {S}, generic_params = {Sr}, fields =",
      self->name,
      self->generic_params
        ? to_string__Debug__CIGenericParams(self->generic_params)
        : from__String("NULL"));

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", size_info = {Sr} }",
                         to_string__Debug__CISizeInfo(&self->size_info));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

void
free_as_prototype__CIDeclUnion(const CIDeclUnion *self)
{
    if (self->generic_params) {
        FREE(CIGenericParams, self->generic_params);
    }
}

DESTRUCTOR(CIDeclUnion, const CIDeclUnion *self)
{
    free_as_prototype__CIDeclUnion(self);

    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
    }
}

bool
has_generic__CIDeclUnionGen(const CIDeclUnionGen *self)
{
    return is_generic_params_contains_generic__CIDecl(
      self->called_generic_params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDeclUnionGen, const CIDeclUnionGen *self)
{
    String *res = format__String(
      "CIDeclUnionGen{{ union_ = {Sr}, name = {S}, called_generic_params = "
      "{Sr}, fields =",
      to_string__Debug__CIDeclUnion(self->union_),
      self->name,
      to_string__Debug__CIGenericParams(self->called_generic_params));

    if (self->fields) {
        DEBUG_VEC_STRING(self->fields, res, CIDeclStructField);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", size_info = {Sr} }",
                         to_string__Debug__CISizeInfo(&self->size_info));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIDeclUnionGen, const CIDeclUnionGen *self)
{
    FREE(String, self->name);
    FREE(CIGenericParams, self->called_generic_params);

    if (self->fields) {
        FREE_BUFFER_ITEMS(
          self->fields->buffer, self->fields->len, CIDeclStructField);
        FREE(Vec, self->fields);
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
                              self->name,
                              to_string__Debug__CIExpr(self->expr),
                              self->is_local);
    }

    return format__String("CIDeclVariable{{ data_type = {Sr}, name = {S}, expr "
                          "= NULL, is_local = {b} }",
                          to_string__Debug__CIDataType(self->data_type),
                          self->name,
                          self->is_local);
}
#endif

DESTRUCTOR(CIDeclVariable, const CIDeclVariable *self)
{
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
                    CIDecl *function,
                    CIGenericParams *called_generic_params,
                    String *name,
                    CIDataType *return_data_type)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclFunction *f = &function->function;

    self->kind = CI_DECL_KIND_FUNCTION_GEN;
    // NOTE: We add the "static" storage class flag to all functions
    // generated by the transpiler (which basically have a params
    // generic), to avoid symbol duplication between entities (e.g.
    // lib, bin).
    self->storage_class_flag =
      function->storage_class_flag | CI_STORAGE_CLASS_STATIC;
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
                    CIDecl *struct_,
                    CIGenericParams *called_generic_params,
                    String *name,
                    Vec *fields,
                    CISizeInfo size_info)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclStruct *s = &struct_->struct_;

    self->kind = CI_DECL_KIND_STRUCT_GEN;
    self->storage_class_flag = struct_->storage_class_flag;
    self->is_prototype = false;
    self->ref_count = 0;
    self->struct_gen =
      NEW(CIDeclStructGen, s, name, called_generic_params, fields, size_info);

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
                    CIDecl *typedef_,
                    CIGenericParams *called_generic_params,
                    String *name,
                    CIDataType *data_type,
                    CISizeInfo size_info)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclTypedef *t = &typedef_->typedef_;

    self->kind = CI_DECL_KIND_TYPEDEF_GEN;
    self->storage_class_flag = CI_STORAGE_CLASS_NONE;
    self->is_prototype = true;
    self->ref_count = 0;
    self->typedef_gen = NEW(
      CIDeclTypedefGen, t, name, called_generic_params, data_type, size_info);

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
                    CIDecl *union_,
                    CIGenericParams *called_generic_params,
                    String *name,
                    Vec *fields,
                    CISizeInfo size_info)
{
    CIDecl *self = lily_malloc(sizeof(CIDecl));
    const CIDeclUnion *u = &union_->union_;

    self->kind = CI_DECL_KIND_UNION_GEN;
    self->storage_class_flag = union_->storage_class_flag;
    self->is_prototype = false;
    self->ref_count = 0;
    self->union_gen =
      NEW(CIDeclUnionGen, u, name, called_generic_params, fields, size_info);

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

bool
is_generic_params_contains_generic__CIDecl(
  const CIGenericParams *generic_params)
{
    if (generic_params) {
        for (Usize i = 0; i < generic_params->params->len; ++i) {
            if (CAST(CIDataType *, get__Vec(generic_params->params, i))->kind ==
                CI_DATA_TYPE_KIND_GENERIC) {
                return true;
            }
        }
    }

    return false;
}

const Vec *
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
            return self->enum_.name;
        case CI_DECL_KIND_FUNCTION:
            return self->function.name;
        case CI_DECL_KIND_FUNCTION_GEN:
            return self->function_gen.name;
        case CI_DECL_KIND_LABEL:
            return self->label.name;
        case CI_DECL_KIND_STRUCT:
            return self->struct_.name;
        case CI_DECL_KIND_STRUCT_GEN:
            return self->struct_gen.name;
        case CI_DECL_KIND_TYPEDEF:
            return self->typedef_.name;
        case CI_DECL_KIND_TYPEDEF_GEN:
            return self->typedef_gen.name;
        case CI_DECL_KIND_UNION:
            return self->union_.name;
        case CI_DECL_KIND_UNION_GEN:
            return self->union_gen.name;
        case CI_DECL_KIND_VARIABLE:
            return self->variable.name;
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
            if (self->struct_.generic_params) {
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
            if (self->union_.generic_params) {
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

Usize
get_alignment__CIDecl(const CIDecl *self)
{
    return get_size_info__CIDecl(self)->alignment;
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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIDecl, const CIDecl *self)
{
    switch (self->kind) {
        case CI_DECL_KIND_ENUM:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, enum_ = {Sr} }",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclEnum(&self->enum_));
        case CI_DECL_KIND_FUNCTION:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, function = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclFunction(&self->function));
        case CI_DECL_KIND_FUNCTION_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, function_gen = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclFunctionGen(&self->function_gen));
        case CI_DECL_KIND_LABEL:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, label = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclLabel(&self->label));
        case CI_DECL_KIND_STRUCT:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, struct_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclStruct(&self->struct_));
        case CI_DECL_KIND_STRUCT_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, struct_gen = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclStructGen(&self->struct_gen));
        case CI_DECL_KIND_TYPEDEF:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, typedef_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclTypedef(&self->typedef_));
        case CI_DECL_KIND_TYPEDEF_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, typedef_gen = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclTypedefGen(&self->typedef_gen));
        case CI_DECL_KIND_VARIABLE:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, variable = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclVariable(&self->variable));
        case CI_DECL_KIND_UNION:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
              "{b}, union_ = {Sr} "
              "}",
              to_string__Debug__CIDeclKind(self->kind),
              to_string__Debug__CIStorageClass(self->storage_class_flag),
              self->is_prototype,
              to_string__Debug__CIDeclUnion(&self->union_));
        case CI_DECL_KIND_UNION_GEN:
            return format__String(
              "CIDecl{{ kind = {s}, storage_class_flag = {s}, is_prototype = "
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
IMPL_FOR_DEBUG(to_string, CIExprArray, const CIExprArray *self)
{
    String *res = from__String("CIExprArray{ array =");

    DEBUG_VEC_STRING(self->array, res, CIExpr);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIExprArray, const CIExprArray *self)
{
    FREE_BUFFER_ITEMS(self->array->buffer, self->array->len, CIExpr);
    FREE(Vec, self->array);
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
              self->string.value);
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
    if (self->string.must_free) {
        FREE(String, self->string.value);
    }
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
    CI_SERIALIZE_NAME(self->identifier);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprFunctionCall, const CIExprFunctionCall *self)
{
    String *res = format__String(
      "CIExprFunctionCall{{ identifier = {S}, params =", self->identifier);

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

CONSTRUCTOR(CIExprStructFieldCall *,
            CIExprStructFieldCall,
            Vec *path,
            CIExpr *value)
{
    CIExprStructFieldCall *self = lily_malloc(sizeof(CIExprStructFieldCall));

    self->path = path;
    self->value = value;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CIExprStructFieldCall,
               const CIExprStructFieldCall *self)
{
    String *res = format__String("CIExprStructFieldCall{{ path = {{ ");

    for (Usize i = 0; i < self->path->len; ++i) {
        push_str__String(res, CAST(String *, get__Vec(self->path, i))->buffer);

        if (i + 1 != self->path->len) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, " }");

    {
        String *s = format__String(", value = {Sr} }",
                                   to_string__Debug__CIExpr(self->value));

        APPEND_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(CIExprStructFieldCall, CIExprStructFieldCall *self)
{
    FREE(Vec, self->path);
    FREE(CIExpr, self->value);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CIExprStructCall, const CIExprStructCall *self)
{
    String *res = format__String("CIExprStructCall{{ fields =");

    DEBUG_VEC_STRING(self->fields, res, CIExprStructFieldCall);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(CIExprStructCall, const CIExprStructCall *self)
{
    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, CIExprStructFieldCall);
    FREE(Vec, self->fields);
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
        case CI_EXPR_KIND_LITERAL:
            return "CI_EXPR_KIND_LITERAL";
        case CI_EXPR_KIND_NULLPTR:
            return "CI_EXPR_KIND_NULLPTR";
        case CI_EXPR_KIND_SIZEOF:
            return "CI_EXPR_KIND_SIZEOF";
        case CI_EXPR_KIND_STRUCT_CALL:
            return "CI_EXPR_KIND_STRUCT_CALL";
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

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, array, CIExprArray array)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_ARRAY;
    self->ref_count = 0;
    self->array = array;

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

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, identifier, String *identifier)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_IDENTIFIER;
    self->ref_count = 0;
    self->identifier = identifier;

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

VARIANT_CONSTRUCTOR(CIExpr *, CIExpr, struct_call, CIExprStructCall struct_call)
{
    CIExpr *self = lily_malloc(sizeof(CIExpr));

    self->kind = CI_EXPR_KIND_STRUCT_CALL;
    self->ref_count = 0;
    self->struct_call = struct_call;

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

CIDataType *
get_data_type__CIExpr(const CIExpr *self)
{
    switch (self->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            TODO("alignof: implement size_t");
        case CI_EXPR_KIND_ARRAY:
            TODO("array");
        case CI_EXPR_KIND_ARRAY_ACCESS:
            TODO("array access");
        case CI_EXPR_KIND_BINARY:
            switch (self->binary.kind) {
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
                case CI_EXPR_BINARY_KIND_ADD:
                case CI_EXPR_BINARY_KIND_SUB:
                case CI_EXPR_BINARY_KIND_MUL:
                case CI_EXPR_BINARY_KIND_DIV:
                case CI_EXPR_BINARY_KIND_MOD:
                case CI_EXPR_BINARY_KIND_BIT_AND:
                case CI_EXPR_BINARY_KIND_BIT_OR:
                case CI_EXPR_BINARY_KIND_BIT_XOR:
                case CI_EXPR_BINARY_KIND_BIT_LSHIFT:
                case CI_EXPR_BINARY_KIND_BIT_RSHIFT: {
                    CIDataType *left_data_type =
                      get_data_type__CIExpr(self->binary.left);
                    CIDataType *right_data_type =
                      get_data_type__CIExpr(self->binary.right);

                    if (!eq__CIDataType(left_data_type, right_data_type)) {
                        FREE(CIDataType, left_data_type);
                        FREE(CIDataType, right_data_type);

                        return NULL;
                    }

                    FREE(CIDataType, right_data_type);

                    return left_data_type;
                }
                case CI_EXPR_BINARY_KIND_AND:
                case CI_EXPR_BINARY_KIND_OR:
                case CI_EXPR_BINARY_KIND_EQ:
                case CI_EXPR_BINARY_KIND_NE:
                case CI_EXPR_BINARY_KIND_LESS:
                case CI_EXPR_BINARY_KIND_GREATER:
                case CI_EXPR_BINARY_KIND_LESS_EQ:
                case CI_EXPR_BINARY_KIND_GREATER_EQ: {
                    // TODO: Check the left and right expr's data type
                    return NEW(CIDataType, CI_DATA_TYPE_KIND_BOOL);
                }
                case CI_EXPR_BINARY_KIND_DOT:
                case CI_EXPR_BINARY_KIND_ARROW:
                    TODO("get data type of right expression of . and ->");
                default:
                    UNREACHABLE("unknown variant");
            }
        case CI_EXPR_KIND_CAST:
            return clone__CIDataType(self->cast.data_type);
        case CI_EXPR_KIND_FUNCTION_CALL:
            TODO("get data from function");
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN: {
            CIBuiltin *builtin = get_ref__CIBuiltin();
            const CIBuiltinFunction *function = get_builtin_function__CIBuiltin(
              builtin, self->function_call_builtin.id);

            ASSERT(function);

            return clone__CIDataType(function->return_data_type);
        }
        case CI_EXPR_KIND_GROUPING:
            return get_data_type__CIExpr(self->grouping);
        case CI_EXPR_KIND_IDENTIFIER:
            TODO("search identifier");
        case CI_EXPR_KIND_LITERAL:
            switch (self->literal.kind) {
                case CI_EXPR_LITERAL_KIND_BOOL:
                    return NEW(CIDataType, CI_DATA_TYPE_KIND_BOOL);
                case CI_EXPR_LITERAL_KIND_CHAR:
                    return NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR);
                case CI_EXPR_LITERAL_KIND_FLOAT:
                    return NEW(CIDataType, CI_DATA_TYPE_KIND_DOUBLE);
                case CI_EXPR_LITERAL_KIND_SIGNED_INT:
                    return NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
                case CI_EXPR_LITERAL_KIND_STRING:
                    return NEW_VARIANT(
                      CIDataType, ptr, NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR));
                case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
                    return NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_INT);
                default:
                    UNREACHABLE("unknown variant");
            }
        case CI_EXPR_KIND_NULLPTR:
            // void*
            return NEW_VARIANT(
              CIDataType, ptr, NEW(CIDataType, CI_DATA_TYPE_KIND_VOID));
        case CI_EXPR_KIND_SIZEOF:
            TODO("sizeof: implement size_t");
        case CI_EXPR_KIND_STRUCT_CALL:
            TODO("struct call");
        case CI_EXPR_KIND_TERNARY: {
            CIDataType *if_ = get_data_type__CIExpr(self->ternary.if_);
            CIDataType *else_ = get_data_type__CIExpr(self->ternary.else_);

            if (!eq__CIDataType(if_, else_)) {
                FREE(CIDataType, if_);
                FREE(CIDataType, else_);

                return NULL;
            }

            FREE(CIDataType, else_);

            return if_;
        }
        case CI_EXPR_KIND_UNARY:
            switch (self->unary.kind) {
                case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
                case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
                case CI_EXPR_UNARY_KIND_POST_INCREMENT:
                case CI_EXPR_UNARY_KIND_POST_DECREMENT:
                case CI_EXPR_UNARY_KIND_POSITIVE:
                case CI_EXPR_UNARY_KIND_NEGATIVE:
                case CI_EXPR_UNARY_KIND_BIT_NOT:
                case CI_EXPR_UNARY_KIND_NOT: {
                    CIDataType *right = get_data_type__CIExpr(self->unary.expr);

                    if (right) {
                        if (is_integer__CIDataType(right)) {
                            return right;
                        }

                        FREE(CIDataType, right);
                    }

                    return NULL;
                }
                case CI_EXPR_UNARY_KIND_DEREFERENCE: {
                    CIDataType *right = get_data_type__CIExpr(self->unary.expr);

                    if (right) {
                        return get_ptr__CIDataType(right);
                    }

                    return NULL;
                }
                case CI_EXPR_UNARY_KIND_REF: {
                    CIDataType *right = get_data_type__CIExpr(self->unary.expr);

                    if (right) {
                        switch (right->kind) {
                            case CI_DATA_TYPE_KIND_PRE_CONST:
                                return NEW_VARIANT(
                                  CIDataType,
                                  pre_const,
                                  NEW_VARIANT(CIDataType, ptr, right));
                            default:
                                return NEW_VARIANT(CIDataType, ptr, right);
                        }
                    }

                    return NULL;
                }
            }
        default:
            UNREACHABLE("unknown variant");
    }
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
        case CI_EXPR_KIND_ARRAY:
            return format__String("CIExpr{{ kind = {s}, array = {Sr} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  to_string__Debug__CIExprArray(&self->array));
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
            return format__String("CIExpr{{ kind = {s}, identifier = {S} }",
                                  to_string__Debug__CIExprKind(self->kind),
                                  self->identifier);
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
        case CI_EXPR_KIND_STRUCT_CALL:
            return format__String(
              "CIExpr{{ kind = {s}, struct_call = {Sr} }",
              to_string__Debug__CIExprKind(self->kind),
              to_string__Debug__CIExprStructCall(&self->struct_call));
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

VARIANT_DESTRUCTOR(CIExpr, array, CIExpr *self)
{
    FREE(CIExprArray, &self->array);
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

VARIANT_DESTRUCTOR(CIExpr, struct_call, CIExpr *self)
{
    FREE(CIExprStructCall, &self->struct_call);
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
        case CI_EXPR_KIND_ARRAY:
            FREE_VARIANT(CIExpr, array, self);
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
        case CI_EXPR_KIND_LITERAL:
            FREE_VARIANT(CIExpr, literal, self);
            break;
        case CI_EXPR_KIND_NULLPTR:
            lily_free(self);
            break;
        case CI_EXPR_KIND_SIZEOF:
            FREE_VARIANT(CIExpr, sizeof, self);
            break;
        case CI_EXPR_KIND_STRUCT_CALL:
            FREE_VARIANT(CIExpr, struct_call, self);
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
    String *res = format__String(
      "CIStmtFor{{ body = {Sr}, init_clause = {Sr}, expr1 = {Sr}, exprs2 =",
      to_string__Debug__CIDeclFunctionBody(self->body),
      self->init_clause
        ? to_string__Debug__CIDeclFunctionItem(self->init_clause)
        : from__String("NULL"),
      self->expr1 ? to_string__Debug__CIExpr(self->expr1)
                  : from__String("NULL"));

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

    if (self->init_clause) {
        FREE(CIDeclFunctionItem, self->init_clause);
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
                                  self->goto_);
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
        case CI_STMT_KIND_GOTO:
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
