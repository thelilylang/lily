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

#include <base/print.h>

#include <core/lily/lily.h>
#include <core/lily/mir/generator.h>
#include <core/lily/mir/generator/constant.h>
#include <core/lily/mir/generator/enum.h>
#include <core/lily/mir/generator/fun.h>
#include <core/lily/mir/generator/record.h>
#include <core/lily/package/package.h>

#define RESOLVE_TYPE_DEPS(decl_kind)                                      \
    for (Usize i = 0; i < type->decl_kind.deps->len; ++i) {               \
        LilyCheckedDecl *type_dep = get__Vec(type->decl_kind.deps, i);    \
        /* Get a signature with only user defined data type, because the  \
        generic signatures are not used in the MIR. */                    \
        LilyCheckedSignatureType *signature =                             \
          get_user_defined_signature__LilyCheckedSignatureType(           \
            get_signatures__LilyCheckedDecl(type_dep));                   \
                                                                          \
        if (signature) {                                                  \
            if (LilyMirKeyIsUnique(module,                                \
                                   signature->ser_global_name->buffer)) { \
                generate_type__LilyMir(module, type_dep);                 \
            }                                                             \
        }                                                                 \
    }

void
generate_type__LilyMir(LilyMirModule *module, LilyCheckedDecl *type)
{
    switch (type->kind) {
        case LILY_CHECKED_DECL_KIND_ERROR:
            break;
        case LILY_CHECKED_DECL_KIND_OBJECT:
            RESOLVE_TYPE_DEPS(object);

            switch (type->object.kind) {
                case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
                    break;
                case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                    break;
                case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                    break;
                case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        case LILY_CHECKED_DECL_KIND_TYPE:
            RESOLVE_TYPE_DEPS(type);

            switch (type->type.kind) {
                case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                    break;
                case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                    generate_enum__LilyMir(module, type);

                    break;
                case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                    generate_record__LilyMir(module, type);

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        default:
            UNREACHABLE("this variant is not expected in this context.");
    }
}

void
run__LilyMir(LilyPackage *self)
{
    // Generate all types.
    for (Usize i = 0; i < self->analysis.module.decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(self->analysis.module.decls, i);

        switch (decl->kind) {
            case LILY_CHECKED_DECL_KIND_TYPE:
            case LILY_CHECKED_DECL_KIND_OBJECT:
            case LILY_CHECKED_DECL_KIND_ERROR:
                generate_type__LilyMir(&self->mir_module, decl);

                break;
            default:
                continue;
        }
    }

    // Generate all constants.
    for (Usize i = 0; i < self->analysis.module.decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(self->analysis.module.decls, i);

        switch (decl->kind) {
            case LILY_CHECKED_DECL_KIND_CONSTANT:
                generate_constant__LilyMir(&self->mir_module, decl);

                break;
            default:
                continue;
        }

        LilyMirPopCurrent(&self->mir_module);
    }

    // Generate all methods or functions.
    for (Usize i = 0; i < self->analysis.module.decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(self->analysis.module.decls, i);

        switch (decl->kind) {
            case LILY_CHECKED_DECL_KIND_METHOD:
                break;
            case LILY_CHECKED_DECL_KIND_FUN:
                generate_fun__LilyMir(&self->mir_module, decl);

                break;
            default:
                continue;
        }
    }

#ifdef DEBUG_MIR
    printf("====MIR Generator(%s)====\n", self->global_name->buffer);

    PRINT_STRING_ORD_HASH_MAP(self->mir_module.insts,
                              to_string__Debug__LilyMirInstruction);
#endif
}
