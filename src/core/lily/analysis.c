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

#include <base/new.h>
#include <base/print.h>

#include <core/lily/analysis.h>
#include <core/lily/lily.h>
#include <core/lily/package.h>

#include <core/shared/diagnostic.h>

static inline void
push_constant__LilyAnalysis(LilyAnalysis *self,
                            LilyAstDecl *constant,
                            LilyCheckedDeclModule *module);

static inline void
push_error__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *error,
                         LilyCheckedDeclModule *module);

static inline void
push_fun__LilyAnalysis(LilyAnalysis *self,
                       LilyAstDecl *fun,
                       LilyCheckedDeclModule *module);

static void
push_all_delcs__LilyAnalysis(LilyAnalysis *self,
                             const Vec *decls,
                             LilyCheckedDeclModule *module);

static void
run_step1__LilyAnalysis(LilyAnalysis *self);

void
push_constant__LilyAnalysis(LilyAnalysis *self,
                            LilyAstDecl *constant,
                            LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(LilyCheckedDecl,
                          constant,
                          &constant->location,
                          constant,
                          NEW(LilyCheckedDeclConstant,
                              constant->constant.name,
                              NULL,
                              NULL,
                              constant->constant.visibility)));
}

void
push_error__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *error,
                         LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(LilyCheckedDecl,
                          error,
                          &error->location,
                          error,
                          NEW(LilyCheckedDeclError,
                              error->error.name,
                              NULL,
                              NULL,
                              error->error.visibility)));
}

void
push_fun__LilyAnalysis(LilyAnalysis *self,
                       LilyAstDecl *fun,
                       LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(LilyCheckedDecl,
                          fun,
                          &fun->location,
                          fun,
                          NEW(LilyCheckedDeclFun,
                              fun->fun.name,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              fun->fun.visibility,
                              fun->fun.is_async,
                              fun->fun.is_operator)));
}

void
push_all_delcs__LilyAnalysis(LilyAnalysis *self,
                             const Vec *decls,
                             LilyCheckedDeclModule *module)
{
    for (Usize i = 0; i < decls->len; ++i) {
        LilyAstDecl *decl = get__Vec(decls, i);

        switch (decl->kind) {
            case LILY_AST_DECL_KIND_CONSTANT: {
                push_constant__LilyAnalysis(self, decl, module);

                LilyCheckedScopeContainerConstant *sc_constant =
                  NEW(LilyCheckedScopeContainerConstant,
                      decl->constant.name,
                      NEW(LilyCheckedAccessConstant, module->access, i));

                int status =
                  add_constant__LilyCheckedScope(module->scope, sc_constant);

                if (status) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &decl->location,
                        NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_CONSTANT),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);

                    FREE(LilyCheckedScopeContainerConstant, sc_constant);
                }

                break;
            }
            case LILY_AST_DECL_KIND_ERROR: {
                push_error__LilyAnalysis(self, decl, module);

                LilyCheckedScopeContainerError *sc_error =
                  NEW(LilyCheckedScopeContainerError,
                      decl->error.name,
                      NEW(LilyCheckedAccessError, module->access, i));

                int status =
                  add_error__LilyCheckedScope(module->scope, sc_error);

                if (status) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &decl->location,
                        NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_ERROR),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);

                    FREE(LilyCheckedScopeContainerError, sc_error);
                }

                break;
            }
            case LILY_AST_DECL_KIND_FUN: {
                LilyCheckedScopeContainerFun *overload_fun =
                  search_fun__LilyCheckedScope(module->scope, decl->fun.name);

                push_fun__LilyAnalysis(self, decl, module);

                if (overload_fun) {
                    push__Vec(overload_fun->accesses,
                              NEW(LilyCheckedAccessFun, module->access, i));
                } else {
                    add_fun__LilyCheckedScope(
                      module->scope,
                      NEW(LilyCheckedScopeContainerFun,
                          decl->fun.name,
                          init__Vec(
                            1, NEW(LilyCheckedAccessFun, module->access, i))));
                }

                break;
            }
            default:
                TODO("analysis step 1");
        }
    }
}

void
run_step1__LilyAnalysis(LilyAnalysis *self)
{
    push_all_delcs__LilyAnalysis(self, self->parser->decls, &self->module);
}

void
run__LilyAnalysis(LilyAnalysis *self)
{
    run_step1__LilyAnalysis(self);

    if (self->package->count_error > 0) {
        exit(1);
    }

#ifdef DEBUG_ANALYSIS
    printf("====Analysis(%s)====\n", self->package->file.name);

    PRINTLN("{Sr}", to_string__Debug__LilyCheckedScope(self->module.scope));
#endif
}

DESTRUCTOR(LilyAnalysis, const LilyAnalysis *self)
{
    // FREE(LilyCheckedDeclModule, &self->module);
}
