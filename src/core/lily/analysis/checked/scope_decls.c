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

#include <core/lily/analysis/checked/decl.h>
#include <core/lily/analysis/checked/scope_decls.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeDeclsKind,
               enum LilyCheckedScopeDeclsKind self)
{
    switch (self) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            return "LILY_CHECKED_SCOPE_DECLS_KIND_DECL";
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            return "LILY_CHECKED_SCOPE_DECLS_KIND_MODULE";
        case LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE:
            return "LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE";
        case LILY_CHECKED_SCOPE_DECLS_KIND_STMT:
            return "LILY_CHECKED_SCOPE_DECLS_KIND_STMT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

void
add_unlock_data_type__LilyCheckedScopeDecls(const LilyCheckedScopeDecls *self,
                                            LilyCheckedDataType *unlock)
{
    // TODO: add unlock data type to lambda and method
    switch (self->kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                    push__Vec(self->decl->fun.unlock_data_type, unlock);
                    break;
                case LILY_CHECKED_DECL_KIND_METHOD:
                    TODO("add unlock data type to method");
                default:
                    UNREACHABLE("cannot add unlock data type");
            }

            break;
        default:
            UNREACHABLE("cannot add unlock data type");
    }
}

void
set_has_return__LilyCheckedScopeDecls(const LilyCheckedScopeDecls *self)
{
    // TODO: add support for method and lambda function
    switch (self->kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                    self->decl->fun.has_return = true;

                    break;
                case LILY_CHECKED_DECL_KIND_METHOD:
                    TODO("method");
                default:
                    UNREACHABLE("not expected in this context");
            }

            break;
        default:
            UNREACHABLE("not expected in this context");
    }
}

LilyCheckedDataType *
get_return_data_type__LilyCheckedScopeDecls(const LilyCheckedScopeDecls *self)
{
    // TODO: add support for method and lambda function
    switch (self->kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                    return self->decl->fun.return_data_type;
                case LILY_CHECKED_DECL_KIND_METHOD:
                    TODO("method");
                default:
                    UNREACHABLE("not expected in this context");
            }
        default:
            UNREACHABLE("not expected in this context");
    }
}

Vec *
get_used_compiler_generic__LilyCheckedScopeDecls(
  const LilyCheckedScopeDecls *self)
{
    // TODO: add support for method and lambda function
    switch (self->kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                    return self->decl->fun.used_compiler_generic;
                case LILY_CHECKED_DECL_KIND_METHOD:
                    TODO("method");
                default:
                    UNREACHABLE("not expected in this context");
            }
        default:
            UNREACHABLE("not expected in this context");
    }
}
