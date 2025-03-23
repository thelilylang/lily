/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#include <core/cc/ci/resolver/expr.h>
#include <core/cc/ci/result.h>
#include <core/cc/ci/state_checker.h>

enum CIStateCheckerExpr
{
    CI_STATE_CHECKER_EXPR_READ,
    CI_STATE_CHECKER_EXPR_WRITE,
};

static CIStateCheckerValue *
build_function_ptr_from_data_type__CIStateCheckerValue(CIDataType *data_type);

/// @return HashMap<CIStateCheckerValue*>*
static HashMap *
build_struct_or_union_fields_from_data_type__CIStateCheckerValue(
  CIDeclStructFields *fields);

static CIStateCheckerValue *
build_struct_from_data_type__CIStateCheckerValue(CIDataType *data_type);

static CIStateCheckerValue *
build_union_from_data_type__CIStateCheckerValue(CIDataType *data_type);

/// @param name const char*? (&)
/// @return CIStateCheckerValue*?
static CIStateCheckerValue *
get_next_child_value_access__CIStateCheckerValue(CIStateCheckerValue *self,
                                                 const char *name);

static bool
value_need_to_be_destroyed__CIStateCheckerValue(CIStateCheckerValue *value);

/// @param values HashMap<CIStateCheckerValue*>* (&)
static bool
values_need_to_be_destroyed__CIStateCheckerValue(HashMap *values);

/// @brief Check if the struct is dropable
static bool
check_if_struct_is_dropable__CIStateCheckerValue(CIStateCheckerValue *self);

/// @brief Check if the union is dropable
static bool
check_if_union_is_dropable__CIStateCheckerValue(CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type (CI_STATE_CHECKER_VALUE_KIND_ARRAY).
static VARIANT_DESTRUCTOR(CIStateCheckerValue,
                          array,
                          CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type (CI_STATE_CHECKER_VALUE_KIND_FUNCTION).
static VARIANT_DESTRUCTOR(CIStateCheckerValue,
                          function,
                          CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type
/// (CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR).
static VARIANT_DESTRUCTOR(CIStateCheckerValue,
                          function_ptr,
                          CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type (CI_STATE_CHECKER_VALUE_KIND_PRIMARY).
static inline VARIANT_DESTRUCTOR(CIStateCheckerValue,
                                 primary,
                                 CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type (CI_STATE_CHECKER_VALUE_KIND_PTR).
static inline VARIANT_DESTRUCTOR(CIStateCheckerValue,
                                 ptr,
                                 CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type (CI_STATE_CHECKER_VALUE_KIND_STRUCT).
static VARIANT_DESTRUCTOR(CIStateCheckerValue,
                          struct,
                          CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type (CI_STATE_CHECKER_VALUE_KIND_UNION).
static VARIANT_DESTRUCTOR(CIStateCheckerValue,
                          union,
                          CIStateCheckerValue *self);

/// @brief Free CIStateCheckerValue type (CI_STATE_CHECKER_VALUE_KIND_VARIABLE).
static inline VARIANT_DESTRUCTOR(CIStateCheckerValue,
                                 variable,
                                 CIStateCheckerValue *self);

static inline void
set_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file);

static inline void
unset_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file);

static inline void
init_current_scope__CIStateChecker(CIStateChecker *self);

static inline void
deinit_current_scope__CIStateChecker(CIStateChecker *self);

static void
check_if_value_is_accesible__CIStateChecker(CIStateChecker *self,
                                            CIStateCheckerValue *value);

static CIStateCheckerValue *
check_function_expr_array_access__CIStateChecker(
  CIStateChecker *self,
  const CIExprArrayAccess *array_access);

static CIStateCheckerValue *
check_function_expr_binary__CIStateChecker(CIStateChecker *self,
                                           const CIExprBinary *binary);

static CIStateCheckerValue *
check_function_expr_cast__CIStateChecker(
  CIStateChecker *self,
  const CIExprCast *cast,
  enum CIStateCheckerExpr state_checker_expr);

/// @param params Vec<CIExpr*>* (&)
static void
check_function_expr_function_call_params__CIStateChecker(CIStateChecker *self,
                                                         const Vec *params);

static CIStateCheckerValue *
check_function_expr_function_call__CIStateChecker(
  CIStateChecker *self,
  const CIExprFunctionCall *function_call);

static CIStateCheckerValue *
check_function_expr_function_call_builtin__CIStateChecker(
  CIStateChecker *self,
  const CIExprFunctionCallBuiltin *function_call_builtin);

static void
check_function_expr_identifier_read_state__CIStateChecker(
  CIStateChecker *self,
  CIStateCheckerValue *identifier_value);

static void
check_function_expr_identifier_write_state__CIStateChecker(
  CIStateChecker *self,
  CIStateCheckerValue *identifier_value);

static CIStateCheckerValue *
check_function_expr_identifier__CIStateChecker(
  CIStateChecker *self,
  const CIExprIdentifier *identifier,
  enum CIStateCheckerExpr state_checker_expr);

static CIStateCheckerValue *
check_function_expr_initializer__CIStateChecker(
  CIStateChecker *self,
  const CIExprInitializer *initializer);

static CIStateCheckerValue *
check_function_expr_literal__CIStateChecker(CIStateChecker *self,
                                            const CIExprLiteral *literal);

static CIStateCheckerValue *
check_function_expr_ternary__CIStateChecker(CIStateChecker *self,
                                            const CIExprTernary *ternary);

static CIStateCheckerValue *
check_function_expr_unary__CIStateChecker(CIStateChecker *self,
                                          const CIExprUnary *unary);

/// @return CIStateCheckerValue*?
static CIStateCheckerValue *
check_function_expr__CIStateChecker(CIStateChecker *self,
                                    const CIExpr *expr,
                                    enum CIStateCheckerExpr state_checker_expr);

static void
check_function_stmt_block__CIStateChecker(CIStateChecker *self,
                                          const CIStmtBlock *block);

static void
check_function_stmt_do_while__CIStateChecker(CIStateChecker *self,
                                             const CIStmtDoWhile *do_while);

static void
check_function_stmt_for__CIStateChecker(CIStateChecker *self,
                                        const CIStmtFor *for_);

static void
check_function_stmt_if_branch__CIStateChecker(CIStateChecker *self,
                                              const CIStmtIfBranch *if_branch);

static void
check_function_stmt_if__CIStateChecker(CIStateChecker *self,
                                       const CIStmtIf *if_);

/// @param return_stmt_expr const CIExpr*? (&)
static void
check_function_stmt_return__CIStateChecker(CIStateChecker *self,
                                           const CIExpr *return_stmt_expr);

static void
check_function_stmt_switch__CIStateChecker(CIStateChecker *self,
                                           const CIStmtSwitch *switch_);

static void
check_function_stmt_while__CIStateChecker(CIStateChecker *self,
                                          const CIStmtWhile *while_);

static void
check_function_stmt__CIStateChecker(CIStateChecker *self, const CIStmt *stmt);

static void
check_function_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl);

static void
check_function_body_item__CIStateChecker(CIStateChecker *self,
                                         const CIDeclFunctionItem *item);

static void
check_function_body__CIStateChecker(CIStateChecker *self,
                                    const CIDeclFunctionBody *body);

static void
check_if_values_are_destroyed__CIStateChecker(CIStateChecker *self);

static void
check_non_generic_function__CIStateChecker(CIStateChecker *self,
                                           const CIDecl *decl);

static void
check_generic_function__CIStateChecker(CIStateChecker *self,
                                       const CIDecl *decl);

/// @brief Check if the given state, is compatible with the provided expression.
static void
check_state_according_expr__CIStateChecker(CIStateChecker *self,
                                           CIStateCheckerValue *value,
                                           CIExpr *expr);

static void
check_state_and_expr__CIStateChecker(CIStateChecker *self,
                                     CIStateCheckerValue *value,
                                     CIExpr *expr);

static void
check_variable__CIStateChecker(CIStateChecker *self, const CIDecl *decl);

static void
check_global_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl);

static void
check_global_decls__CIStateChecker(CIStateChecker *self);

static void
run_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file);

/// @param other_args void* (CIStateChecker* (&))
static void
handler__CIStateChecker([[maybe_unused]] void *entity,
                        const CIResultFile *file,
                        void *other_args);

DESTRUCTOR(CIStateCheckerValueFunction, const CIStateCheckerValueFunction *self)
{
    FREE_RC(String, self->name);
    FREE_HASHMAP_VALUES(self->final_params_state, CIStateCheckerValue);
    FREE(HashMap, self->final_params_state);
    FREE(CIStateCheckerValue, self->return_value);
}

DESTRUCTOR(CIStateCheckerValueFunctionPtr,
           const CIStateCheckerValueFunctionPtr *self)
{
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, CIStateCheckerValue);
    FREE(Vec, self->params);
    FREE(CIStateCheckerValue, self->return_value);
}

DESTRUCTOR(CIStateCheckerValuePtr, const CIStateCheckerValuePtr *self)
{
    FREE(CIStateCheckerValue, self->value);
}

DESTRUCTOR(CIStateCheckerValueStruct, const CIStateCheckerValueStruct *self)
{
    if (self->name) {
        FREE_RC(String, self->name);
    }

    FREE_HASHMAP_VALUES(self->values, CIStateCheckerValue);
    FREE(HashMap, self->values);
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    array,
                    struct CIStateCheckerValuePtr array)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_ARRAY;
    self->ref_count = 0;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    function,
                    CIStateCheckerValueFunction function)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_FUNCTION;
    self->ref_count = 0;
    self->function = function;

    return self;
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    function_ptr,
                    CIStateCheckerValueFunctionPtr function_ptr)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR;
    self->function_ptr = function_ptr;

    return self;
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    primary,
                    CIStateCheckerState primary)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_PRIMARY;
    self->ref_count = 0;
    self->primary = primary;

    return self;
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    ptr,
                    CIStateCheckerValuePtr ptr)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_PTR;
    self->ptr = ptr;

    return self;
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    struct,
                    CIStateCheckerValueStruct struct_)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_STRUCT;
    self->ref_count = 0;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    union,
                    CIStateCheckerValueStruct union_)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_UNION;
    self->ref_count = 0;
    self->union_ = union_;

    return self;
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    variable,
                    CIStateCheckerValueVariable variable)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_VARIABLE;
    self->ref_count = 0;
    self->variable = variable;

    return self;
}

const String *
get_name__CIStateCheckerValue(CIStateCheckerValue *self)
{
    switch (self->kind) {
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION:
            return GET_PTR_RC(String, self->function.name);
        case CI_STATE_CHECKER_VALUE_KIND_ARRAY:
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR:
        case CI_STATE_CHECKER_VALUE_KIND_PTR:
        case CI_STATE_CHECKER_VALUE_KIND_STRUCT:
        case CI_STATE_CHECKER_VALUE_KIND_UNION:
            return NULL;
        case CI_STATE_CHECKER_VALUE_KIND_VARIABLE:
            return GET_PTR_RC(String, self->variable.name);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIStateCheckerValue *
build_function_ptr_from_data_type__CIStateCheckerValue(CIDataType *data_type)
{
    ASSERT(data_type->kind == CI_DATA_TYPE_KIND_FUNCTION);

    TODO("function ptr");
}

HashMap *
build_struct_or_union_fields_from_data_type__CIStateCheckerValue(
  CIDeclStructFields *fields)
{
    HashMap *values = NEW(HashMap); // HashMap<CIStateCheckerValue*>*
    CIDeclStructField *current_field = fields->first;

    while (current_field) {
        switch (current_field->kind) {
            case CI_DECL_STRUCT_FIELD_KIND_MEMBER: {
                CIStateCheckerValue *value =
                  build_from_data_type__CIStateCheckerValue(
                    current_field->member.data_type);
                Rc *name = current_field->name; // Rc<String*>* (&)

                insert__HashMap(
                  values,
                  GET_PTR_RC(String, name)->buffer,
                  NEW_VARIANT(CIStateCheckerValue,
                              variable,
                              NEW(CIStateCheckerValueVariable, name, value)));

                break;
            }
            case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_STRUCT:
            case CI_DECL_STRUCT_FIELD_KIND_ANONYMOUS_UNION:
                TODO("anonymous struct/union");
            case CI_DECL_STRUCT_FIELD_KIND_NAMED_STRUCT:
            case CI_DECL_STRUCT_FIELD_KIND_NAMED_UNION:
                TODO("named struct/union");
            default:
                UNREACHABLE("unknown variant");
        }

        current_field = current_field->next;
    }

    return values;
}

CIStateCheckerValue *
build_struct_from_data_type__CIStateCheckerValue(CIDataType *data_type)
{
    ASSERT(data_type->kind == CI_DATA_TYPE_KIND_STRUCT);

    TODO("struct");
}

CIStateCheckerValue *
build_union_from_data_type__CIStateCheckerValue(CIDataType *data_type)
{
    ASSERT(data_type->kind == CI_DATA_TYPE_KIND_UNION);

    TODO("union");
}

CIStateCheckerValue *
build_from_data_type__CIStateCheckerValue(CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return NEW_VARIANT(
              CIStateCheckerValue,
              array,
              NEW(CIStateCheckerValuePtr,
                  NEW(CIStateCheckerState,
                      data_type->ctx | CI_DATA_TYPE_CONTEXT_UNDEFINED),
                  build_from_data_type__CIStateCheckerValue(
                    data_type->array.data_type)));
        case CI_DATA_TYPE_KIND_FUNCTION:
            return build_function_ptr_from_data_type__CIStateCheckerValue(
              data_type);
        case CI_DATA_TYPE_KIND_PTR:
            return NEW_VARIANT(
              CIStateCheckerValue,
              ptr,
              NEW(CIStateCheckerValuePtr,
                  NEW(CIStateCheckerState,
                      data_type->ctx | CI_DATA_TYPE_CONTEXT_UNDEFINED),
                  build_from_data_type__CIStateCheckerValue(
                    data_type->ptr.data_type)));
        case CI_DATA_TYPE_KIND_STRUCT:
            return build_struct_from_data_type__CIStateCheckerValue(data_type);
        case CI_DATA_TYPE_KIND_UNION:
            return build_union_from_data_type__CIStateCheckerValue(data_type);
        default:
            return NEW_VARIANT(
              CIStateCheckerValue,
              primary,
              NEW(CIStateCheckerState,
                  data_type->ctx | CI_DATA_TYPE_CONTEXT_UNDEFINED));
    }
}

CIStateCheckerValue *
get_next_child_value_access__CIStateCheckerValue(CIStateCheckerValue *self,
                                                 const char *name)
{
    switch (self->kind) {
        case CI_STATE_CHECKER_VALUE_KIND_ARRAY:
            return ref__CIStateCheckerValue(self->array.value);
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION:
            return ref__CIStateCheckerValue(self->function.return_value);
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR:
            return ref__CIStateCheckerValue(self->function_ptr.return_value);
        case CI_STATE_CHECKER_VALUE_KIND_PRIMARY:
            return NULL;
        case CI_STATE_CHECKER_VALUE_KIND_PTR:
            return ref__CIStateCheckerValue(self->ptr.value);
        case CI_STATE_CHECKER_VALUE_KIND_STRUCT: {
            ASSERT(name);

            CIStateCheckerValue *value =
              get__HashMap(self->struct_.values, (char *)name);

            return value ? ref__CIStateCheckerValue(value) : NULL;
        }
        case CI_STATE_CHECKER_VALUE_KIND_UNION: {
            ASSERT(name);

            CIStateCheckerValue *value =
              get__HashMap(self->union_.values, (char *)name);

            return value ? ref__CIStateCheckerValue(value) : NULL;
        }
        case CI_STATE_CHECKER_VALUE_KIND_VARIABLE:
            return ref__CIStateCheckerValue(self->variable.value);
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
value_need_to_be_destroyed__CIStateCheckerValue(CIStateCheckerValue *value)
{
    switch (value->kind) {
        case CI_STATE_CHECKER_VALUE_KIND_PTR:
            return value->ptr.state.flags & CI_DATA_TYPE_CONTEXT_HEAP &&
                   !(value->ptr.state.flags & CI_DATA_TYPE_CONTEXT_FREED);
        case CI_STATE_CHECKER_VALUE_KIND_STRUCT:
            return check_if_struct_is_dropable__CIStateCheckerValue(value);
        case CI_STATE_CHECKER_VALUE_KIND_UNION:
            return check_if_union_is_dropable__CIStateCheckerValue(value);
        case CI_STATE_CHECKER_VALUE_KIND_VARIABLE:
            return value_need_to_be_destroyed__CIStateCheckerValue(
              value->variable.value);
        default:
            return false;
    }
}

bool
values_need_to_be_destroyed__CIStateCheckerValue(HashMap *values)
{
    HashMapIter values_iter = NEW(HashMapIter, values);
    CIStateCheckerValue *value = NULL; // CIStateCheckerValue*? (&)

    while ((value = next__HashMapIter(&values_iter))) {
        if (value_need_to_be_destroyed__CIStateCheckerValue(value)) {
            return true;
        }
    }

    return false;
}

bool
check_if_struct_is_dropable__CIStateCheckerValue(CIStateCheckerValue *self)
{
    if (self->struct_.is_dropable == -1) {
        self->struct_.is_dropable =
          values_need_to_be_destroyed__CIStateCheckerValue(self->struct_.values)
            ? 1
            : 0;
    }

    return self->struct_.is_dropable;
}

bool
check_if_union_is_dropable__CIStateCheckerValue(CIStateCheckerValue *self)
{
    if (self->union_.is_dropable == -1) {
        self->union_.is_dropable =
          values_need_to_be_destroyed__CIStateCheckerValue(self->union_.values)
            ? 1
            : 0;
    }

    return self->union_.is_dropable;
}

CIStateCheckerState *
get_return_state__CIStateCheckerValue(CIStateCheckerValue *self)
{
    switch (self->kind) {
        case CI_STATE_CHECKER_VALUE_KIND_ARRAY:
            return &self->array.state;
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION:
            return get_return_state__CIStateCheckerValue(
              self->function.return_value);
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR:
            return &self->function_ptr.state;
        case CI_STATE_CHECKER_VALUE_KIND_PRIMARY:
            return &self->primary;
        case CI_STATE_CHECKER_VALUE_KIND_PTR:
            return &self->ptr.state;
        case CI_STATE_CHECKER_VALUE_KIND_STRUCT:
            return &self->struct_.state;
        case CI_STATE_CHECKER_VALUE_KIND_UNION:
            return &self->union_.state;
        case CI_STATE_CHECKER_VALUE_KIND_VARIABLE:
            return get_return_state__CIStateCheckerValue(self->variable.value);
        default:
            UNREACHABLE("unknown variant");
    }
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, array, CIStateCheckerValue *self)
{
    FREE(CIStateCheckerValuePtr, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, function, CIStateCheckerValue *self)
{
    FREE(CIStateCheckerValueFunction, &self->function);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, function_ptr, CIStateCheckerValue *self)
{
    FREE(CIStateCheckerValueFunctionPtr, &self->function_ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, primary, CIStateCheckerValue *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, ptr, CIStateCheckerValue *self)
{
    FREE(CIStateCheckerValuePtr, &self->ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, struct, CIStateCheckerValue *self)
{
    FREE(CIStateCheckerValueStruct, &self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, union, CIStateCheckerValue *self)
{
    FREE(CIStateCheckerValueStruct, &self->union_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CIStateCheckerValue, variable, CIStateCheckerValue *self)
{
    lily_free(self);
}

DESTRUCTOR(CIStateCheckerValue, CIStateCheckerValue *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case CI_STATE_CHECKER_VALUE_KIND_ARRAY:
            FREE_VARIANT(CIStateCheckerValue, array, self);

            break;
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION:
            FREE_VARIANT(CIStateCheckerValue, function, self);

            break;
        case CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR:
            FREE_VARIANT(CIStateCheckerValue, function_ptr, self);

            break;
        case CI_STATE_CHECKER_VALUE_KIND_PRIMARY:
            FREE_VARIANT(CIStateCheckerValue, primary, self);

            break;
        case CI_STATE_CHECKER_VALUE_KIND_PTR:
            FREE_VARIANT(CIStateCheckerValue, ptr, self);

            break;
        case CI_STATE_CHECKER_VALUE_KIND_STRUCT:
            FREE_VARIANT(CIStateCheckerValue, struct, self);

            break;
        case CI_STATE_CHECKER_VALUE_KIND_UNION:
            FREE_VARIANT(CIStateCheckerValue, union, self);

            break;
        case CI_STATE_CHECKER_VALUE_KIND_VARIABLE:
            FREE_VARIANT(CIStateCheckerValue, variable, self);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(CIStateCheckerScope *,
            CIStateCheckerScope,
            CIStateCheckerScope *parent)
{
    CIStateCheckerScope *self = lily_malloc(sizeof(CIStateCheckerScope));

    self->parent = parent;
    self->values = NEW(HashMap);

    return self;
}

void
add__CIStateCheckerScope(CIStateCheckerScope *self, CIStateCheckerValue *value)
{
    const String *name = get_name__CIStateCheckerValue(value);

    ASSERT(name);

    insert__HashMap(self->values, name->buffer, value);
}

CIStateCheckerValue *
get__CIStateCheckerScope(CIStateCheckerScope *self, Rc *name)
{
    char *name_buffer = GET_PTR_RC(String, name)->buffer;
    CIStateCheckerScope *current = self;
    CIStateCheckerValue *value = NULL;

    while (current && !value) {
        value = get__HashMap(current->values, name_buffer);
        current = current->parent;
    }

    // NOTE: Normally, if the scope doesn't find the requested identifier, this
    // probably means that there's a bug in the scope, or that a bug was present
    // during the steps preceding the state checker.
    ASSERT(value);

    return value;
}

DESTRUCTOR(CIStateCheckerScope, CIStateCheckerScope *self)
{
    FREE_HASHMAP_VALUES(self->values, CIStateCheckerValue);
    FREE(HashMap, self->values);
    lily_free(self);
}

void
set_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file)
{
    self->file = file;
    self->current_scope = NEW(CIStateCheckerScope, NULL);
}

void
unset_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file)
{
    self->file = NULL;
}

void
init_current_scope__CIStateChecker(CIStateChecker *self)
{
    self->current_scope = NEW(CIStateCheckerScope, self->current_scope);
}

void
deinit_current_scope__CIStateChecker(CIStateChecker *self)
{
    ASSERT(self->current_scope);

    CIStateCheckerScope *tmp_current_scope = self->current_scope;

    self->current_scope = self->current_scope->parent;

    FREE(CIStateCheckerScope, tmp_current_scope);
}

void
check_if_value_is_accesible__CIStateChecker(CIStateChecker *self,
                                            CIStateCheckerValue *value)
{
    CIStateCheckerState *state = get_return_state__CIStateCheckerValue(value);

    if (state->flags & CI_DATA_TYPE_CONTEXT_UNDEFINED) {
        FAILED("not expected to access to a undefined value");
    }

    switch (value->kind) {
        case CI_STATE_CHECKER_VALUE_KIND_PTR:
            if (!(state->flags & CI_DATA_TYPE_CONTEXT_NON_NULL)) {
                FAILED("not expected to access to a nullable pointer");
            }

            if (state->flags & CI_DATA_TYPE_CONTEXT_FREED) {
                FAILED("not expected to access to a freed pointer");
            }

            break;
        case CI_STATE_CHECKER_VALUE_KIND_STRUCT:
        case CI_STATE_CHECKER_VALUE_KIND_UNION:
            if (state->flags & CI_DATA_TYPE_CONTEXT_DROPPED) {
                FAILED("not expected to access to a dropped struct or union");
            }

            break;
        default:
            break;
    }
}

CIStateCheckerValue *
check_function_expr_array_access__CIStateChecker(
  CIStateChecker *self,
  const CIExprArrayAccess *array_access)
{
    CIStateCheckerValue *array_state_value =
      check_function_expr__CIStateChecker(
        self, array_access->array, CI_STATE_CHECKER_EXPR_READ);
    CIStateCheckerValue *array_access_state_value =
      check_function_expr__CIStateChecker(
        self, array_access->access, CI_STATE_CHECKER_EXPR_READ);
    CIStateCheckerState *array_access_state =
      get_return_state__CIStateCheckerValue(array_access_state_value);

    if (!(array_access_state->flags & CI_DATA_TYPE_CONTEXT_INDEX)) {
        FAILED("expected to have index context flag on return value");
    }

    check_if_value_is_accesible__CIStateChecker(self, array_state_value);

    CIStateCheckerValue *res =
      get_next_child_value_access__CIStateCheckerValue(array_state_value, NULL);

    FREE(CIStateCheckerValue, array_state_value);
    FREE(CIStateCheckerValue, array_access_state_value);

    return res;
}

CIStateCheckerValue *
check_function_expr_binary__CIStateChecker(CIStateChecker *self,
                                           const CIExprBinary *binary)
{
    if (is_assign__CIExprBinaryKind(binary->kind)) {
        CIStateCheckerValue *left_res = check_function_expr__CIStateChecker(
          self, binary->left, CI_STATE_CHECKER_EXPR_WRITE);
        CIStateCheckerState *left_res_state =
          get_return_state__CIStateCheckerValue(left_res);

        CIStateCheckerValue *right_res = check_function_expr__CIStateChecker(
          self, binary->right, CI_STATE_CHECKER_EXPR_READ);

        // After an assignment, the left value is no longer undefined.
        left_res_state->flags &= ~CI_DATA_TYPE_CONTEXT_UNDEFINED;

        FREE(CIStateCheckerValue, right_res);

        return left_res;
    }

    CIStateCheckerValue *left_res = check_function_expr__CIStateChecker(
      self, binary->left, CI_STATE_CHECKER_EXPR_READ);
    CIStateCheckerValue *right_res = check_function_expr__CIStateChecker(
      self, binary->right, CI_STATE_CHECKER_EXPR_READ);
    CIStateCheckerValue *res = NULL;

    if (left_res->kind == CI_STATE_CHECKER_VALUE_KIND_PTR ||
        left_res->kind == CI_STATE_CHECKER_VALUE_KIND_ARRAY ||
        left_res->kind == CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR) {
        res = left_res;
    } else if (right_res->kind == CI_STATE_CHECKER_VALUE_KIND_PTR ||
               right_res->kind == CI_STATE_CHECKER_VALUE_KIND_ARRAY ||
               right_res->kind == CI_STATE_CHECKER_VALUE_KIND_FUNCTION_PTR) {
        res = right_res;
    } else {
        res = left_res;
    }

    if (res != left_res) {
        FREE(CIStateCheckerValue, left_res);
    } else if (res != right_res) {
        FREE(CIStateCheckerValue, right_res);
    }

    return res;
}

CIStateCheckerValue *
check_function_expr_cast__CIStateChecker(
  CIStateChecker *self,
  const CIExprCast *cast,
  enum CIStateCheckerExpr state_checker_expr)
{
    // TODO: Check if the conversion is fine.
    return check_function_expr__CIStateChecker(
      self, cast->expr, state_checker_expr);
}

void
check_function_expr_function_call_params__CIStateChecker(CIStateChecker *self,
                                                         const Vec *params)
{
    for (Usize i = 0; i < params->len; ++i) {
        CIExpr *param = get__Vec(params, i);

        FREE(CIStateCheckerValue,
             check_function_expr__CIStateChecker(
               self, param, CI_STATE_CHECKER_EXPR_READ));
    }
}

CIStateCheckerValue *
check_function_expr_function_call__CIStateChecker(
  CIStateChecker *self,
  const CIExprFunctionCall *function_call)
{
    check_function_expr_function_call_params__CIStateChecker(
      self, function_call->params);

    CIStateCheckerValue *function_value =
      get__CIStateCheckerScope(self->current_scope, function_call->identifier);

    return get_next_child_value_access__CIStateCheckerValue(function_value,
                                                            NULL);
}

CIStateCheckerValue *
check_function_expr_function_call_builtin__CIStateChecker(
  CIStateChecker *self,
  const CIExprFunctionCallBuiltin *function_call_builtin)
{
    check_function_expr_function_call_params__CIStateChecker(
      self, function_call_builtin->params);

    CIBuiltin *builtin = get_ref__CIBuiltin();
    const CIBuiltinFunction *builtin_function =
      get_builtin_function__CIBuiltin(builtin, function_call_builtin->id);
    CIStateCheckerValue *value = build_from_data_type__CIStateCheckerValue(
      builtin_function->return_data_type);

    return value;
}

void
check_function_expr_identifier_read_state__CIStateChecker(
  CIStateChecker *self,
  CIStateCheckerValue *identifier_value)
{
    const CIStateCheckerState *state =
      get_return_state__CIStateCheckerValue(identifier_value);

    ASSERT(state);

    if (state->flags & CI_DATA_TYPE_CONTEXT_FREED) {
        FAILED("you cannot read an identifier that has already been freed");
    }

    if (state->flags & CI_DATA_TYPE_CONTEXT_DROPPED) {
        FAILED("you cannot read an identifier that has already been dropped");
    }

    if (state->flags & CI_DATA_TYPE_CONTEXT_UNDEFINED) {
        FAILED("you cannot read an identifier with undefined value");
    }
}

void
check_function_expr_identifier_write_state__CIStateChecker(
  CIStateChecker *self,
  CIStateCheckerValue *identifier_value)
{
    const CIStateCheckerState *state =
      get_return_state__CIStateCheckerValue(identifier_value);

    ASSERT(state);

    if (state->flags & CI_DATA_TYPE_CONTEXT_HEAP) {
        FAILED("direct memory leak detected");
    }
}

CIStateCheckerValue *
check_function_expr_identifier__CIStateChecker(
  CIStateChecker *self,
  const CIExprIdentifier *identifier,
  enum CIStateCheckerExpr state_checker_expr)
{
    CIStateCheckerValue *identifier_value =
      get__CIStateCheckerScope(self->current_scope, identifier->value);

    switch (state_checker_expr) {
        case CI_STATE_CHECKER_EXPR_READ:
            check_function_expr_identifier_read_state__CIStateChecker(
              self, identifier_value);

            break;
        case CI_STATE_CHECKER_EXPR_WRITE:
            check_function_expr_identifier_write_state__CIStateChecker(
              self, identifier_value);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return identifier_value;
}

CIStateCheckerValue *
check_function_expr_initializer__CIStateChecker(
  CIStateChecker *self,
  const CIExprInitializer *initializer)
{
    for (Usize i = 0; i < initializer->items->len; ++i) {
        CIExprInitializerItem *item = get__Vec(initializer->items, i);

        check_function_expr__CIStateChecker(
          self, item->value, CI_STATE_CHECKER_EXPR_READ);
    }
}

CIStateCheckerValue *
check_function_expr_literal__CIStateChecker(CIStateChecker *self,
                                            const CIExprLiteral *literal)
{
    int flags = CI_DATA_TYPE_CONTEXT_NONE;

    switch (literal->kind) {
        case CI_EXPR_LITERAL_KIND_STRING:
            flags = CI_DATA_TYPE_CONTEXT_STACK;

            break;
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            flags = CI_DATA_TYPE_CONTEXT_INDEX;

            break;
        default:
            break;
    }

    return NEW_VARIANT(
      CIStateCheckerValue, primary, NEW(CIStateCheckerState, flags));
}

CIStateCheckerValue *
check_function_expr_ternary__CIStateChecker(CIStateChecker *self,
                                            const CIExprTernary *ternary)
{
    FREE(CIStateCheckerValue,
         check_function_expr__CIStateChecker(
           self, ternary->cond, CI_STATE_CHECKER_EXPR_READ));

    CIStateCheckerValue *if_res = check_function_expr__CIStateChecker(
      self, ternary->if_, CI_STATE_CHECKER_EXPR_READ);
    CIStateCheckerValue *else_res = check_function_expr__CIStateChecker(
      self, ternary->else_, CI_STATE_CHECKER_EXPR_READ);

    return if_res;
}

CIStateCheckerValue *
check_function_expr_unary__CIStateChecker(CIStateChecker *self,
                                          const CIExprUnary *unary)
{
    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_DEREFERENCE: {
            CIStateCheckerValue *value = check_function_expr__CIStateChecker(
              self, unary->expr, CI_STATE_CHECKER_EXPR_READ);
            CIStateCheckerValue *res =
              get_next_child_value_access__CIStateCheckerValue(value, NULL);

            FREE(CIStateCheckerValue, value);

            return res;
        }
        default:
            return check_function_expr__CIStateChecker(
              self, unary->expr, CI_STATE_CHECKER_EXPR_READ);
    }
}

CIStateCheckerValue *
check_function_expr__CIStateChecker(CIStateChecker *self,
                                    const CIExpr *expr,
                                    enum CIStateCheckerExpr state_checker_expr)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            return NEW_VARIANT(
              CIStateCheckerValue,
              primary,
              NEW(CIStateCheckerState, CI_DATA_TYPE_CONTEXT_INDEX));
        case CI_EXPR_KIND_DATA_TYPE:
            return NEW_VARIANT(
              CIStateCheckerValue,
              primary,
              NEW(CIStateCheckerState, CI_DATA_TYPE_CONTEXT_NONE));
        case CI_EXPR_KIND_LITERAL:
            return check_function_expr_literal__CIStateChecker(self,
                                                               &expr->literal);
        case CI_EXPR_KIND_NULLPTR:
            return NEW_VARIANT(
              CIStateCheckerValue,
              primary,
              NEW(CIStateCheckerState, CI_DATA_TYPE_CONTEXT_NONE));
        case CI_EXPR_KIND_SIZEOF:
            return NEW_VARIANT(
              CIStateCheckerValue,
              primary,
              NEW(CIStateCheckerState, CI_DATA_TYPE_CONTEXT_INDEX));
            // NOTE: We don't need to perform any check in that case.
        case CI_EXPR_KIND_ARRAY_ACCESS:
            return check_function_expr_array_access__CIStateChecker(
              self, &expr->array_access);
        case CI_EXPR_KIND_BINARY:
            return check_function_expr_binary__CIStateChecker(self,
                                                              &expr->binary);
        case CI_EXPR_KIND_CAST:
            return check_function_expr_cast__CIStateChecker(
              self, &expr->cast, state_checker_expr);
        case CI_EXPR_KIND_FUNCTION_CALL:
            return check_function_expr_function_call__CIStateChecker(
              self, &expr->function_call);
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            return check_function_expr_function_call_builtin__CIStateChecker(
              self, &expr->function_call_builtin);
        case CI_EXPR_KIND_GROUPING:
            return check_function_expr__CIStateChecker(
              self, expr->grouping, state_checker_expr);
        case CI_EXPR_KIND_IDENTIFIER:
            return check_function_expr_identifier__CIStateChecker(
              self, &expr->identifier, state_checker_expr);
        case CI_EXPR_KIND_INITIALIZER:
            return check_function_expr_initializer__CIStateChecker(
              self, &expr->initializer);
        case CI_EXPR_KIND_TERNARY:
            return check_function_expr_ternary__CIStateChecker(self,
                                                               &expr->ternary);
        case CI_EXPR_KIND_UNARY:
            return check_function_expr_unary__CIStateChecker(self,
                                                             &expr->unary);
        default:
            UNREACHABLE("unknown variant");
    }
}

void
check_function_stmt_block__CIStateChecker(CIStateChecker *self,
                                          const CIStmtBlock *block)
{
    check_function_body__CIStateChecker(self, block->body);
}

void
check_function_stmt_do_while__CIStateChecker(CIStateChecker *self,
                                             const CIStmtDoWhile *do_while)
{
    check_function_expr__CIStateChecker(
      self, do_while->cond, CI_STATE_CHECKER_EXPR_READ);
    check_function_body__CIStateChecker(self, do_while->body);
}

void
check_function_stmt_for__CIStateChecker(CIStateChecker *self,
                                        const CIStmtFor *for_)
{
    if (for_->init_clauses) {
    }

    if (for_->expr1) {
    }

    if (for_->exprs2) {
    }

    check_function_body__CIStateChecker(self, for_->body);
}

void
check_function_stmt_if_branch__CIStateChecker(CIStateChecker *self,
                                              const CIStmtIfBranch *if_branch)
{
    check_function_expr__CIStateChecker(
      self, if_branch->cond, CI_STATE_CHECKER_EXPR_READ);
    check_function_body__CIStateChecker(self, if_branch->body);
}

void
check_function_stmt_if__CIStateChecker(CIStateChecker *self,
                                       const CIStmtIf *if_)
{
    check_function_stmt_if_branch__CIStateChecker(self, if_->if_);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            check_function_stmt_if_branch__CIStateChecker(
              self, get__Vec(if_->else_ifs, i));
        }
    }

    if (if_->else_) {
        check_function_body__CIStateChecker(self, if_->else_);
    }
}

void
check_function_stmt_return__CIStateChecker(CIStateChecker *self,
                                           const CIExpr *return_stmt_expr)
{
    if (return_stmt_expr) {
        check_function_expr__CIStateChecker(
          self, return_stmt_expr, CI_STATE_CHECKER_EXPR_READ);
    }
}

void
check_function_stmt_switch__CIStateChecker(CIStateChecker *self,
                                           const CIStmtSwitch *switch_)
{
    check_function_expr__CIStateChecker(
      self, switch_->expr, CI_STATE_CHECKER_EXPR_READ);
    check_function_body__CIStateChecker(self, switch_->body);
}

void
check_function_stmt_while__CIStateChecker(CIStateChecker *self,
                                          const CIStmtWhile *while_)
{
    check_function_expr__CIStateChecker(
      self, while_->cond, CI_STATE_CHECKER_EXPR_READ);
    check_function_body__CIStateChecker(self, while_->body);
}

void
check_function_stmt__CIStateChecker(CIStateChecker *self, const CIStmt *stmt)
{
    switch (stmt->kind) {
        case CI_STMT_KIND_BLOCK:
            return check_function_stmt_block__CIStateChecker(self,
                                                             &stmt->block);
        case CI_STMT_KIND_BREAK:
        case CI_STMT_KIND_CASE:
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_DEFAULT:
        case CI_STMT_KIND_GOTO:
            break;
        case CI_STMT_KIND_DO_WHILE:
            return check_function_stmt_do_while__CIStateChecker(
              self, &stmt->do_while);
        case CI_STMT_KIND_FOR:
            return check_function_stmt_for__CIStateChecker(self, &stmt->for_);
        case CI_STMT_KIND_IF:
            return check_function_stmt_if__CIStateChecker(self, &stmt->if_);
        case CI_STMT_KIND_RETURN:
            return check_function_stmt_return__CIStateChecker(self,
                                                              stmt->return_);
        case CI_STMT_KIND_SWITCH:
            return check_function_stmt_switch__CIStateChecker(self,
                                                              &stmt->switch_);
        case CI_STMT_KIND_WHILE:
            return check_function_stmt_while__CIStateChecker(self,
                                                             &stmt->while_);
        default:
            UNREACHABLE("unknown variant");
    }
}

void
check_function_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl)
{
    switch (decl->kind) {
        case CI_DECL_KIND_FUNCTION:
            check_non_generic_function__CIStateChecker(self, decl);

            break;
        case CI_DECL_KIND_VARIABLE:
            check_variable__CIStateChecker(self, decl);

            break;
        default:
            UNREACHABLE("variant is not expected");
    }
}

void
check_function_body_item__CIStateChecker(CIStateChecker *self,
                                         const CIDeclFunctionItem *item)
{
    switch (item->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            check_function_decl__CIStateChecker(self, item->decl);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            check_function_expr__CIStateChecker(
              self, item->expr, CI_STATE_CHECKER_EXPR_READ);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            check_function_stmt__CIStateChecker(self, &item->stmt);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
check_function_body__CIStateChecker(CIStateChecker *self,
                                    const CIDeclFunctionBody *body)
{
    // TODO: Add scope
    for (Usize i = 0; i < body->content->len; ++i) {
        check_function_body_item__CIStateChecker(self,
                                                 get__Vec(body->content, i));
    }
}

void
check_non_generic_function__CIStateChecker(CIStateChecker *self,
                                           const CIDecl *decl)
{
    if (is_prototype__CIDecl((CIDecl *)decl)) {
        CIStateCheckerValue *function_value = NEW_VARIANT(
          CIStateCheckerValue,
          function,
          NEW(CIStateCheckerValueFunction, decl->function.name, NULL, NULL));

        return add__CIStateCheckerScope(self->current_scope, function_value);
    }

    init_current_scope__CIStateChecker(self);
    check_function_body__CIStateChecker(self, decl->function.body);
    deinit_current_scope__CIStateChecker(self);
}

void
check_generic_function__CIStateChecker(CIStateChecker *self, const CIDecl *decl)
{
    if (is_prototype__CIDecl((CIDecl *)decl)) {
        // TODO: manage prototype function gen
        return;
    }
}

void
check_if_values_are_destroyed__CIStateChecker(CIStateChecker *self)
{
    HashMapIter values_iter = NEW(HashMapIter, self->current_scope->values);
    CIStateCheckerValue *value = NULL; // CIStateCheckerValue*? (&)

    while ((value = next__HashMapIter(&values_iter))) {
        if (value->kind == CI_STATE_CHECKER_VALUE_KIND_VARIABLE) {
            value = value->variable.value;

            switch (value->kind) {
                case CI_STATE_CHECKER_VALUE_KIND_PTR:
                    if (value->ptr.state.flags & CI_DATA_TYPE_CONTEXT_HEAP &&
                        !(value->ptr.state.flags &
                          CI_DATA_TYPE_CONTEXT_FREED)) {
                        FAILED("direct memory leak detected");
                    }

                    break;
                case CI_STATE_CHECKER_VALUE_KIND_STRUCT:
                    if (!(value->struct_.state.flags &
                          CI_DATA_TYPE_CONTEXT_DROPPED) &&
                        check_if_struct_is_dropable__CIStateCheckerValue(
                          value)) {
                        FAILED("direct memory leak detected");
                    }

                    break;
                case CI_STATE_CHECKER_VALUE_KIND_UNION:
                    if (!(value->union_.state.flags &
                          CI_DATA_TYPE_CONTEXT_DROPPED) &&
                        check_if_union_is_dropable__CIStateCheckerValue(
                          value)) {
                        FAILED("potential direct memory leak detected");
                    }

                    break;
                default:
                    break;
            }
        }
    }
}

void
check_state_according_expr__CIStateChecker(CIStateChecker *self,
                                           CIStateCheckerValue *value,
                                           CIExpr *expr)
{
    CIStateCheckerState *state = get_return_state__CIStateCheckerValue(value);

    if (state->flags & CI_DATA_TYPE_CONTEXT_NON_NULL &&
        is_null__CIResolverExpr(expr)) {
        FAILED("expected non null expression");
    }
}

void
check_state_and_expr__CIStateChecker(CIStateChecker *self,
                                     CIStateCheckerValue *value,
                                     CIExpr *expr)
{
    check_state_according_expr__CIStateChecker(self, value, expr);
    check_function_expr__CIStateChecker(self, expr, CI_STATE_CHECKER_EXPR_READ);
}

void
check_variable__CIStateChecker(CIStateChecker *self, const CIDecl *decl)
{
    CIStateCheckerValue *state_value =
      build_from_data_type__CIStateCheckerValue(decl->variable.data_type);

    if (decl->variable.expr) {
        check_state_and_expr__CIStateChecker(
          self, state_value, decl->variable.expr);
    }

    add__CIStateCheckerScope(
      self->current_scope,
      NEW_VARIANT(
        CIStateCheckerValue,
        variable,
        NEW(CIStateCheckerValueVariable, decl->variable.name, state_value)));
}

void
check_global_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl)
{
    if (is_local__CIDecl(decl)) {
        return;
    }

    switch (decl->kind) {
        case CI_DECL_KIND_ENUM:
            // TODO: enum
            break;
        case CI_DECL_KIND_ENUM_VARIANT:
            // TODO: enum variant
            break;
        case CI_DECL_KIND_FUNCTION:
            check_non_generic_function__CIStateChecker(self, decl);

            break;
        case CI_DECL_KIND_FUNCTION_GEN:
            check_generic_function__CIStateChecker(self, decl);

            break;
        case CI_DECL_KIND_LABEL:
            UNREACHABLE("cannot get label declaration in this context")
        case CI_DECL_KIND_STRUCT:
            // TODO: struct
            break;
        case CI_DECL_KIND_STRUCT_GEN:
            // TODO: struct gen
            break;
        case CI_DECL_KIND_TYPEDEF:
            // TODO: typedef
            break;
        case CI_DECL_KIND_TYPEDEF_GEN:
            // TODO: typedef gen
            break;
        case CI_DECL_KIND_UNION:
            // TODO: union
            break;
        case CI_DECL_KIND_UNION_GEN:
            // TODO: union gen
            break;
        case CI_DECL_KIND_VARIABLE:
            check_variable__CIStateChecker(self, decl);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

static void
check_global_decls__CIStateChecker(CIStateChecker *self)
{
    CIDecl *decl = NULL;
    VecIter iter_decl = NEW(VecIter, self->file->entity.decls);

    while ((decl = next__VecIter(&iter_decl))) {
        check_global_decl__CIStateChecker(self, decl);
    }
}

void
run_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file)
{
    set_file__CIStateChecker(self, file);
    check_global_decls__CIStateChecker(self);
    unset_file__CIStateChecker(self, file);
}

void
handler__CIStateChecker([[maybe_unused]] void *entity,
                        const CIResultFile *file,
                        void *other_args)
{
    CIStateChecker *self = (CIStateChecker *)other_args;

    run_file__CIStateChecker(self, file);
}

void
run__CIStateChecker(CIStateChecker *self)
{
    if (!self->result->config->no_state_check) {
        pass_through_result__CIResult(
          self->result, &handler__CIStateChecker, self);
    }
}
