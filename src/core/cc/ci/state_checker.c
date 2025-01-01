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

#include <core/cc/ci/result.h>
#include <core/cc/ci/state_checker.h>

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
init_scope__CIStateChecker(CIStateChecker *self);

static inline void
deinit_scope__CIStateChecker(CIStateChecker *self);

static void
check_function_body_item__CIStateChecker(CIStateChecker *self,
                                         const CIDeclFunctionItem *item);

static void
check_function_body__CIStateChecker(CIStateChecker *self,
                                    const CIDeclFunctionBody *body);

static void
check_function_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl);

static void
check_global_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl);

static void
check_global_decls__CIStateChecker(CIStateChecker *self);

static void
run_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file);

/// @param other_args void* (CIStateChecker* (&))
static void
handler__CIStateChecker(const CIResultFile *file, void *other_args);

DESTRUCTOR(CIStateCheckerValueStruct, const CIStateCheckerValueStruct *self)
{
    FREE_HASHMAP_VALUES(self->values, CIStateCheckerValue);
    FREE(HashMap, self->values);
}

VARIANT_CONSTRUCTOR(CIStateCheckerValue *,
                    CIStateCheckerValue,
                    struct,
                    CIStateCheckerValueStruct struct_)
{
    CIStateCheckerValue *self = lily_malloc(sizeof(CIStateCheckerValue));

    self->kind = CI_STATE_CHECKER_VALUE_KIND_STRUCT;
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
    self->variable = variable;

    return self;
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
}

void
unset_file__CIStateChecker(CIStateChecker *self, const CIResultFile *file)
{
    self->file = NULL;
}

void
init_scope__CIStateChecker(CIStateChecker *self)
{
    self->scope = NEW(CIStateCheckerScope, NULL);
}

void
deinit_scope__CIStateChecker(CIStateChecker *self)
{
    ASSERT(self->scope);

    FREE(CIStateCheckerScope, self->scope);
    self->scope = NULL;
}

void
check_function_body_item__CIStateChecker(CIStateChecker *self,
                                         const CIDeclFunctionItem *item)
{
}

void
check_function_body__CIStateChecker(CIStateChecker *self,
                                    const CIDeclFunctionBody *body)
{
    for (Usize i = 0; i < body->content->len; ++i) {
        check_function_body_item__CIStateChecker(self,
                                                 get__Vec(body->content, i));
    }
}

void
check_function_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl)
{
    init_scope__CIStateChecker(self);
    check_function_body__CIStateChecker(self, decl->function.body);
    deinit_scope__CIStateChecker(self);
}

void
check_global_decl__CIStateChecker(CIStateChecker *self, const CIDecl *decl)
{
    if (is_prototype__CIDecl((CIDecl *)decl)) {
        return;
    }

    switch (decl->kind) {
        case CI_DECL_KIND_FUNCTION:
            check_function_decl__CIStateChecker(self, decl);

            break;
        case CI_DECL_KIND_FUNCTION_GEN:
            break;
        default:
            break;
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
handler__CIStateChecker(const CIResultFile *file, void *other_args)
{
    CIStateChecker *self = (CIStateChecker *)other_args;

    run_file__CIStateChecker(self, file);
}

void
run__CIStateChecker(CIStateChecker *self)
{
    pass_through_result__CIResult(self->result, &handler__CIStateChecker, self);
}
