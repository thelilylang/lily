/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#include <core/cc/ci/infer.h>
#include <core/cc/ci/method.h>
#include <core/cc/ci/resolver/data_type.h>

#include <string.h>

/// @brief Read the tag a name names, whichever of the three it is written as.
/// @param type_name const String* (&)
/// @return CIDecl*? (&)
static CIDecl *
search_tag__CIMethod(const CIResultFile *file, const String *type_name);

/// @brief Whether a typedef is written over the very tag it is named after -
/// `typedef struct Foo { ... } Foo;`, which is one type written twice rather
/// than two types sharing a name.
static bool
is_written_over__CIMethod(const CIDecl *typedef_, const CIDecl *tag);

CIDecl *
search_tag__CIMethod(const CIResultFile *file, const String *type_name)
{
    CIDecl *struct_ = search_struct__CIResultFile(file, type_name);

    if (struct_) {
        return struct_;
    }

    CIDecl *union_ = search_union__CIResultFile(file, type_name);

    if (union_) {
        return union_;
    }

    return search_enum__CIResultFile(file, type_name);
}

bool
is_written_over__CIMethod(const CIDecl *typedef_, const CIDecl *tag)
{
    const CIDataType *data_type = get_typedef_data_type__CIDecl(typedef_);

    if (!data_type) {
        return false;
    }

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ENUM:
            if (!(tag->kind & CI_DECL_KIND_ENUM)) {
                return false;
            }

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            if (!(tag->kind & CI_DECL_KIND_STRUCT)) {
                return false;
            }

            break;
        case CI_DATA_TYPE_KIND_UNION:
            if (!(tag->kind & CI_DECL_KIND_UNION)) {
                return false;
            }

            break;
        default:
            return false;
    }

    const String *data_type_name =
      get_name__CIDataType((CIDataType *)data_type);
    const String *tag_name = get_name__CIDecl(tag);

    // A tag written with no name is written nowhere but where the typedef is,
    // so the typedef is the only name the type is given.
    if (!data_type_name || !tag_name) {
        return true;
    }

    return !strcmp(data_type_name->buffer, tag_name->buffer);
}

bool
is_enabled__CIMethod(const CIResultFile *file)
{
    return file->config->method_convention;
}

bool
is_eligible_type_name__CIMethod(const CIResultFile *file,
                                const String *type_name)
{
    if (!is_enabled__CIMethod(file)) {
        return false;
    }

    // Where no type convention is written there is nothing to be eligible
    // for, so every type is given methods.
    if (!file->config->type_convention) {
        return true;
    }

    return match__CIProjectConfigTypeConvention(file->config->type_convention,
                                                type_name);
}

CIMethodReceiver
search_receiver__CIMethod(const CIResultFile *file, const String *type_name)
{
    if (!is_eligible_type_name__CIMethod(file, type_name)) {
        return (CIMethodReceiver){ .kind = CI_METHOD_RECEIVER_KIND_NONE,
                                   .decl = NULL,
                                   .other = NULL };
    }

    CIDecl *typedef_ = search_typedef__CIResultFile(file, type_name);
    CIDecl *tag = search_tag__CIMethod(file, type_name);

    if (typedef_ && tag && !is_written_over__CIMethod(typedef_, tag)) {
        return (CIMethodReceiver){ .kind = CI_METHOD_RECEIVER_KIND_AMBIGUOUS,
                                   .decl = typedef_,
                                   .other = tag };
    }

    // A method is written on what a receiver is declared as, which is the
    // typedef wherever one is written - the tag is only ever reached by a
    // type declared as `struct Foo` rather than as `Foo`.
    const CIDecl *decl = typedef_ ? typedef_ : tag;

    return (CIMethodReceiver){ .kind = decl ? CI_METHOD_RECEIVER_KIND_FOUND
                                            : CI_METHOD_RECEIVER_KIND_NONE,
                               .decl = decl,
                               .other = NULL };
}

CIDecl *
search_method__CIMethod(const CIResultFile *file,
                        const String *type_name,
                        const String *method_name)
{
    if (!is_eligible_type_name__CIMethod(file, type_name)) {
        return NULL;
    }

    String *name = build_name__CIProjectConfigMethodConvention(
      file->config->method_convention, type_name, method_name);
    CIDecl *res = search_function__CIResultFile(file, name);

    FREE(String, name);

    return res;
}

/// @brief Read the name a receiver is written as, which is the name the type
/// holds where it is one that is named at all - an `int` is written as no
/// name, so nothing is ever declared as a method on it.
/// @return const String* (&)?
static const String *
get_receiver_name__CIMethod(const CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ENUM:
        case CI_DATA_TYPE_KIND_STRUCT:
        case CI_DATA_TYPE_KIND_TYPEDEF:
        case CI_DATA_TYPE_KIND_UNION:
            return get_name__CIDataType((CIDataType *)data_type);
        default:
            return NULL;
    }
}

/// @brief Whether the receiver holds a member of this name which C reads a
/// call as being made through - a member is called where what it holds is a
/// function, and nowhere else.
/// @param method_name const String* (&)
static bool
has_callable_field__CIMethod(const CIResultFile *file,
                             CIDataType *data_type,
                             Rc *method_name,
                             const CIGenericParams *called_generic_params,
                             const CIGenericParams *decl_generic_params)
{
    // Only a structure or a union holds members, so what a receiver written
    // as anything else holds is the method and nothing else.
    //
    // NOTE: What the kind is is read here rather than asked of
    // `is_struct_or_union_data_type__CIResolverDataType`, which reports on
    // what is written as neither rather than answering that it is neither.
    CIDataType *resolved = run__CIResolverDataType(
      file, data_type, called_generic_params, decl_generic_params);
    bool is_struct_or_union = resolved->kind == CI_DATA_TYPE_KIND_STRUCT ||
                              resolved->kind == CI_DATA_TYPE_KIND_UNION;

    FREE(CIDataType, resolved);

    if (!is_struct_or_union) {
        return false;
    }

    const CIDeclStructFields *fields =
      get_fields_from_data_type__CIResolverDataType(
        file, data_type, called_generic_params, decl_generic_params);

    if (!fields) {
        return false;
    }

    CIDeclStructField *field =
      get_field_from_name__CIDeclStructFields(fields, method_name);

    if (!field) {
        return false;
    }

    CIDataType *field_data_type = build_data_type__CIDeclStructField(field);

    if (!field_data_type) {
        return false;
    }

    // What is called is what holds a function, whether it is written as one
    // or as the address of one. Anything else is not something C reads a call
    // on, so the name is left for the method to take.
    CIDataType *resolved_field = run__CIResolverDataType(
      file, field_data_type, called_generic_params, decl_generic_params);
    bool res =
      resolved_field->kind == CI_DATA_TYPE_KIND_FUNCTION ||
      (resolved_field->kind == CI_DATA_TYPE_KIND_PTR &&
       resolved_field->ptr.data_type &&
       resolved_field->ptr.data_type->kind == CI_DATA_TYPE_KIND_FUNCTION);

    FREE(CIDataType, resolved_field);
    FREE(CIDataType, field_data_type);

    return res;
}

/// @brief Read the receiver as the first param of the method is written.
///
/// A method written on a pointer is called on what is written as a value as
/// much as on a pointer, so the address is taken of what is written as a
/// value and what is written as a pointer is read through.
///
/// Only whether the receiver is written as a pointer is decided here.
/// Whether the types agree is what the typecheck reads of the call this is
/// rewritten as, and reading it here would say nothing anyway: a method
/// written on generics holds params standing for nothing until the call it is
/// instantiated for is written, which is what this rewrite is what makes.
///
/// @param receiver CIExpr* (&) What the method is called on.
/// @return CIExpr*? The receiver as the method is written to take it.
static CIExpr *
adjust_receiver__CIMethod(const CIResultFile *file,
                          CIExpr *receiver,
                          const CIDataType *receiver_data_type,
                          const CIDecl *method,
                          const CIGenericParams *called_generic_params,
                          const CIGenericParams *decl_generic_params)
{
    const CIDeclFunctionParams *params = get_function_params__CIDecl(method);

    // A method is written on what it is called on, so one written with no
    // param is written on nothing.
    if (!params || params->content->len == 0) {
        return NULL;
    }

    const CIDeclFunctionParam *first = get__Vec(params->content, 0);

    // `(void)` is written to say that the function takes nothing, so a method
    // written that way is written on nothing either.
    if (first->data_type->kind == CI_DATA_TYPE_KIND_VOID) {
        return NULL;
    }

    bool expects_ptr = first->data_type->kind == CI_DATA_TYPE_KIND_PTR;
    bool gives_ptr = receiver_data_type->kind == CI_DATA_TYPE_KIND_PTR;
    CIExpr *res = NULL;

    if (expects_ptr == gives_ptr) {
        res = ref__CIExpr(receiver);
    } else if (expects_ptr) {
        // The method is written on the address of what it is called on.
        res = NEW_VARIANT(
          CIExpr,
          unary,
          clone__Location(&receiver->location),
          NEW(CIExprUnary, CI_EXPR_UNARY_KIND_REF, ref__CIExpr(receiver)));
    } else {
        // The method is written on what the receiver points to.
        res = NEW_VARIANT(CIExpr,
                          unary,
                          clone__Location(&receiver->location),
                          NEW(CIExprUnary,
                              CI_EXPR_UNARY_KIND_DEREFERENCE,
                              ref__CIExpr(receiver)));
    }

    return res;
}

enum CIMethodRewriteStatus
rewrite_call__CIMethod(const CIResultFile *file,
                       CIExpr *expr,
                       const CIScopeID *scope_id,
                       const CIGenericParams *called_generic_params,
                       const CIGenericParams *decl_generic_params)
{
    if (!is_enabled__CIMethod(file)) {
        return CI_METHOD_REWRITE_STATUS_NONE;
    }

    // A call written on a receiver is parsed as the access it is written as,
    // with the call made on the name that follows the `.` or the `->`: what
    // is looked for is `<recv> . <m>(...)` rather than `(<recv> . <m>)(...)`.
    if (expr->kind != CI_EXPR_KIND_BINARY ||
        (expr->binary.kind != CI_EXPR_BINARY_KIND_DOT &&
         expr->binary.kind != CI_EXPR_BINARY_KIND_ARROW)) {
        return CI_METHOD_REWRITE_STATUS_NONE;
    }

    // What follows the `.` is nested to the right rather than to the left, so
    // `a().b().c()` is written as `a() . (b() . c())`: what is called on the
    // receiver is the leftmost call of what follows, and the rest of the
    // chain is called on what that one returns.
    CIExpr *right = expr->binary.right;
    CIExpr *method_call = NULL; // CIExpr* (&) the call the method is named by
    CIExpr *rest = NULL;        // CIExpr*? (&) what the chain holds after it

    if (right->kind == CI_EXPR_KIND_FUNCTION_CALL) {
        method_call = right;
    } else if (right->kind == CI_EXPR_KIND_BINARY &&
               (right->binary.kind == CI_EXPR_BINARY_KIND_DOT ||
                right->binary.kind == CI_EXPR_BINARY_KIND_ARROW) &&
               right->binary.left->kind == CI_EXPR_KIND_FUNCTION_CALL) {
        method_call = right->binary.left;
        rest = right;
    } else {
        return CI_METHOD_REWRITE_STATUS_NONE;
    }

    const CIExprFunctionCall *call = &method_call->function_call;
    const CIExprIdentifier *method_identifier =
      get_callee_identifier__CIExprFunctionCall(call);

    if (!method_identifier) {
        return CI_METHOD_REWRITE_STATUS_NONE;
    }

    // A method is called on what a method call returns as much as on a
    // variable, and `a().b().c()` is written as the call on `c` made on the
    // one on `b`. What the receiver is can only be read of the call it
    // already stands for, so it is rewritten before anything is read of it.
    enum CIMethodRewriteStatus receiver_status =
      rewrite_call__CIMethod(file,
                             expr->binary.left,
                             scope_id,
                             called_generic_params,
                             decl_generic_params);

    switch (receiver_status) {
        case CI_METHOD_REWRITE_STATUS_NONE:
        case CI_METHOD_REWRITE_STATUS_DONE:
            break;
        // What the receiver was reported on is what the call is reported on:
        // there is nothing to call the method on.
        default:
            return receiver_status;
    }

    CIExpr *receiver = expr->binary.left;
    const String *method_name = GET_PTR_RC(String, method_identifier->value);
    // The receiver is read as it is declared rather than as what it aliases:
    // a method is written on the name a typedef gives a type, and resolving
    // the type away is resolving that name away with it.
    CIDataType *receiver_data_type = infer_expr_data_type__CIInfer(
      file, receiver, scope_id, called_generic_params, decl_generic_params);

    if (!receiver_data_type) {
        return CI_METHOD_REWRITE_STATUS_NONE;
    }

    // What a `->` is written on is a pointer, and the method is written on
    // what it points to.
    const CIDataType *receiver_type_source =
      expr->binary.kind == CI_EXPR_BINARY_KIND_ARROW &&
          receiver_data_type->kind == CI_DATA_TYPE_KIND_PTR
        ? receiver_data_type->ptr.data_type
        : receiver_data_type;
    const String *type_name =
      receiver_type_source ? get_receiver_name__CIMethod(receiver_type_source)
                           : NULL;

    if (!type_name) {
        FREE(CIDataType, receiver_data_type);

        return CI_METHOD_REWRITE_STATUS_NONE;
    }

    CIMethodReceiver found = search_receiver__CIMethod(file, type_name);

    switch (found.kind) {
        case CI_METHOD_RECEIVER_KIND_NONE:
            FREE(CIDataType, receiver_data_type);

            return CI_METHOD_REWRITE_STATUS_NONE;
        case CI_METHOD_RECEIVER_KIND_AMBIGUOUS:
            FREE(CIDataType, receiver_data_type);

            return CI_METHOD_REWRITE_STATUS_AMBIGUOUS_RECEIVER;
        default:
            break;
    }

    CIDecl *method =
      search_method__CIMethod(file, type_name, (String *)method_name);
    // What C already reads the call as is what it stays: a member holding a
    // function is called through, whether a method of that name is written or
    // not.
    bool is_field =
      has_callable_field__CIMethod(file,
                                   (CIDataType *)receiver_type_source,
                                   method_identifier->value,
                                   called_generic_params,
                                   decl_generic_params);

    if (is_field) {
        FREE(CIDataType, receiver_data_type);

        return method ? CI_METHOD_REWRITE_STATUS_SHADOWED_BY_FIELD
                      : CI_METHOD_REWRITE_STATUS_NONE;
    }

    if (!method) {
        FREE(CIDataType, receiver_data_type);

        return CI_METHOD_REWRITE_STATUS_UNKNOWN_METHOD;
    }

    CIExpr *adjusted_receiver = adjust_receiver__CIMethod(file,
                                                          receiver,
                                                          receiver_data_type,
                                                          method,
                                                          called_generic_params,
                                                          decl_generic_params);

    FREE(CIDataType, receiver_data_type);

    if (!adjusted_receiver) {
        return CI_METHOD_REWRITE_STATUS_RECEIVER_DOES_NOT_FIT;
    }

    // What the method is called with is what is written between the
    // parentheses, with the receiver written first.
    Vec *params = init__Vec(1, adjusted_receiver); // Vec<CIExpr*>*

    for (Usize i = 0; i < call->params->len; ++i) {
        push__Vec(params, ref__CIExpr(get__Vec(call->params, i)));
    }

    String *name = build_name__CIProjectConfigMethodConvention(
      file->config->method_convention, type_name, method_name);
    CIScope *scope = get_scope_from_id__CIResultFile(file, scope_id);
    Rc *name_rc = NEW(Rc, name);
    // A method written on a generic type is instantiated for what its
    // receiver is written with: `Vec_len.[@T]` called on a `Vec.[int]` is the
    // instance for `[int]`. What the visitor instantiates is what a call
    // writes its generics on, so they are written here rather than left to be
    // read of the arguments.
    const CIGenericParams *receiver_generic_params =
      get_generic_params__CIDataType(receiver_type_source);
    CIExpr *callee =
      NEW_VARIANT(CIExpr,
                  identifier,
                  clone__Location(&method_call->location),
                  NEW(CIExprIdentifier,
                      name_rc,
                      search_identifier__CIExprIdentifierID(name, file, scope),
                      receiver_generic_params
                        ? clone__CIGenericParams(receiver_generic_params)
                        : NULL));

    FREE_RC(String, name_rc);

    // What the chain holds after this call is what is called on what this one
    // returns, so it is kept before the access holding it is released.
    enum CIExprBinaryKind rest_kind = rest ? rest->binary.kind : 0;
    CIExpr *rest_right = rest ? ref__CIExpr(rest->binary.right) : NULL;
    CIExpr *new_call = rest
                         ? NEW_VARIANT(CIExpr,
                                       function_call,
                                       clone__Location(&method_call->location),
                                       NEW(CIExprFunctionCall, callee, params))
                         : NULL;

    // The access the call was written as is what held the receiver and the
    // params, so it is released only once what is kept out of it has been
    // taken a reference of.
    FREE(CIExprBinary, &expr->binary);

    if (!rest) {
        expr->kind = CI_EXPR_KIND_FUNCTION_CALL;
        expr->function_call = NEW(CIExprFunctionCall, callee, params);

        return CI_METHOD_REWRITE_STATUS_DONE;
    }

    // What is left of the chain is called on what this call returns, and is
    // rewritten the same way in its turn.
    expr->binary = NEW(CIExprBinary, rest_kind, new_call, rest_right);

    enum CIMethodRewriteStatus rest_status = rewrite_call__CIMethod(
      file, expr, scope_id, called_generic_params, decl_generic_params);

    return rest_status == CI_METHOD_REWRITE_STATUS_NONE
             ? CI_METHOD_REWRITE_STATUS_DONE
             : rest_status;
}
