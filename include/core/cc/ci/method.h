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

#ifndef LILY_CORE_CC_CI_METHOD_H
#define LILY_CORE_CC_CI_METHOD_H

#include <core/cc/ci/ast.h>
#include <core/cc/ci/result.h>

// What a name written as a receiver was found to name.
//
// A tag and a typedef are written in namespaces of their own, so one name
// reaches both of them: `struct Foo` and `typedef ... Foo` are two
// declarations, and only one of them is what a method is looked up on.
enum CIMethodReceiverKind
{
    // The name names nothing a method is looked up on.
    CI_METHOD_RECEIVER_KIND_NONE,
    // The name names one declaration, or a typedef and the very tag it is
    // written over - `typedef struct Foo { ... } Foo;` is one receiver rather
    // than two, as both name the same type.
    CI_METHOD_RECEIVER_KIND_FOUND,
    // The name names a tag and a typedef which are not the same type, so
    // there is no one thing the method would be looked up on.
    CI_METHOD_RECEIVER_KIND_AMBIGUOUS
};

typedef struct CIMethodReceiver
{
    enum CIMethodReceiverKind kind;
    // What the name names, which is the typedef where a name reaches both a
    // typedef and the tag it is written over.
    const CIDecl *decl; // const CIDecl* (&)?
    // The tag the typedef was found next to, kept to report an ambiguity on
    // both of the declarations that make it one.
    const CIDecl *other; // const CIDecl* (&)?
} CIMethodReceiver;

/**
 *
 * @brief Whether a function is callable as a method at all, which is what a
 * method convention is written in the configuration to say.
 */
bool
is_enabled__CIMethod(const CIResultFile *file);

/**
 *
 * @brief Whether a type written by this name is given methods, which every
 * type is where no type convention is written.
 * @param type_name const String* (&)
 */
bool
is_eligible_type_name__CIMethod(const CIResultFile *file,
                                const String *type_name);

/**
 *
 * @brief Read what a name written as a receiver names.
 *
 * The typedef is what is kept where a name reaches both a typedef and a tag,
 * as a typedef is what the receiver of a method is written as.
 *
 * @param type_name const String* (&)
 */
CIMethodReceiver
search_receiver__CIMethod(const CIResultFile *file, const String *type_name);

/**
 *
 * @brief Search the function a method is declared as on a receiver.
 *
 * The name is built forward, out of the receiver and the method name, rather
 * than read backwards out of the name of a function: `$type_$name` splits
 * `str__len` two ways, and building it leaves nothing to guess.
 *
 * @param type_name const String* (&)
 * @param method_name const String* (&)
 * @return CIDecl*? (&)
 */
CIDecl *
search_method__CIMethod(const CIResultFile *file,
                        const String *type_name,
                        const String *method_name);

// What was made of an expression written as a call on a receiver.
enum CIMethodRewriteStatus
{
    // The expression is not a method call, and is left as it is written. What
    // C reads it as is what it stays.
    CI_METHOD_REWRITE_STATUS_NONE,
    // The expression was rewritten as the call the method stands for.
    CI_METHOD_REWRITE_STATUS_DONE,
    // The name of the receiver names both a tag and a typedef which are not
    // the same type.
    CI_METHOD_REWRITE_STATUS_AMBIGUOUS_RECEIVER,
    // The receiver is given methods, but none by this name.
    CI_METHOD_REWRITE_STATUS_UNKNOWN_METHOD,
    // The receiver holds a member of this name which can be called, so C
    // reads the call as one made through the member. A method of the same
    // name is written as well, and there is no one thing that is called.
    CI_METHOD_REWRITE_STATUS_SHADOWED_BY_FIELD,
    // The method is written on a receiver the one it is called on cannot be
    // read as, whether it is taken the address of or read through.
    CI_METHOD_REWRITE_STATUS_RECEIVER_DOES_NOT_FIT
};

/**
 *
 * @brief Rewrite an expression written as `<recv>.<m>(...)` or
 * `<recv>-><m>(...)` as the call the method stands for, which is
 * `<T>_<m>(<recv>, ...)` under a `$type_$name` convention.
 *
 * The expression is rewritten where it stands rather than replaced, so what
 * holds it has nothing to substitute: what the stages after this one read is
 * an ordinary call, and what is generated is the C anyone would have written.
 *
 * Where C already reads the expression as something - a member of the
 * receiver which can be called - that is what it is left as, and the method
 * is only reported on where both are written.
 *
 * @param expr CIExpr* (&) The expression, rewritten where it is a method
 * call.
 * @param scope_id const CIScopeID* (&)
 * @param called_generic_params const CIGenericParams*? (&)
 * @param decl_generic_params const CIGenericParams*? (&)
 */
enum CIMethodRewriteStatus
rewrite_call__CIMethod(const CIResultFile *file,
                       CIExpr *expr,
                       const CIScopeID *scope_id,
                       const CIGenericParams *called_generic_params,
                       const CIGenericParams *decl_generic_params);

#endif // LILY_CORE_CC_CI_METHOD_H
