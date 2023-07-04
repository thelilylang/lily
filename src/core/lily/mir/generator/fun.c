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

#include <core/lily/analysis/checked/signature.h>
#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/fun.h>
#include <core/lily/mir/generator/stmt.h>
#include <core/lily/mir/generator/val.h>
#include <core/lily/mir/linkage.h>
#include <core/lily/mir/mir.h>

#include <stdio.h>

void
generate_fun__LilyMir(LilyMirModule *module, LilyCheckedDecl *fun)
{
    ASSERT(fun->fun.signatures->len > 0);

    for (Usize i = 0; i < fun->fun.fun_deps->len; ++i) {
        LilyCheckedDecl *fun_dep = get__Vec(fun->fun.fun_deps, i);
        // Get a signature with only user defined data type, because the
        // compiler defined or generic signatures are not used in the MIR.
        LilyCheckedSignatureFun *signature =
          get_user_defined_signature__LilyCheckedSignatureFun(
            fun_dep->fun.signatures);

        if (signature) {
            if (LilyMirKeyIsUnique(module,
                                   signature->ser_global_name->buffer)) {
                generate_fun__LilyMir(module, get__Vec(fun->fun.fun_deps, i));
            }
        }
    }

    for (Usize i = 0; i < fun->fun.signatures->len; ++i) {
        LilyCheckedSignatureFun *signature = get__Vec(fun->fun.signatures, i);

        if (!has_only_known_dt__LilyCheckedSignatureFun(signature)) {
            continue;
        } else if (!LilyMirKeyIsUnique(
                     module,
                     fun->fun.is_main ? "main"
                                      : signature->ser_global_name->buffer)) {
            continue;
        }

        ASSERT(signature->types->len != 0);

        LilyMirInstruction *inst = NEW_VARIANT(
          LilyMirInstruction,
          fun,
          NEW(LilyMirInstructionFun,
              get_linkage_from_visibility__LilyMirLinkage(fun->fun.visibility),
              fun->fun.is_main ? "main" : signature->ser_global_name->buffer,
              signature->global_name->buffer,
              NEW(Vec),
              signature->generic_params,
              generate_dt__LilyMir(module, last__Vec(signature->types))));

        LilyMirAddInst(module, inst);

        for (Usize i = 0; i < signature->types->len - 1; ++i) {
            LilyCheckedDataType *type = get__Vec(signature->types, i);

            LilyMirAddParam(&inst->fun.scope, type);

            push__Vec(inst->fun.args,
                      NEW_VARIANT(LilyMirInstruction,
                                  arg,
                                  NEW(LilyMirInstructionArg,
                                      generate_dt__LilyMir(module, type),
                                      i)));
        }

        for (Usize i = 0; i < fun->fun.body->len; ++i) {
            LilyCheckedBodyFunItem *item = get__Vec(fun->fun.body, i);

            switch (item->kind) {
                case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:
                    LilyMirAddInst(
                      module,
                      generate_expr__LilyMir(
                        module, signature, &inst->fun.scope, item->expr));

                    break;
                case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:
                    LilyMirAddInst(
                      module,
                      generate_stmt__LilyMir(
                        module, signature, &inst->fun.scope, &item->stmt));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }
        }

        LilyMirPopCurrent(module);
    }
}
