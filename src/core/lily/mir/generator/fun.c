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

#include <core/lily/checked/signature.h>
#include <core/lily/mir.h>
#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/fun.h>
#include <core/lily/mir/generator/stmt.h>
#include <core/lily/mir/generator/val.h>
#include <core/lily/mir/linkage.h>

#include <stdio.h>

void
generate_fun__LilyMir(LilyMirModule *module, LilyCheckedDecl *fun)
{
    if (fun->fun.signatures->len == 0) {
        FAILED("warning: function is unused");
    }

    for (Usize i = 0; i < fun->fun.signatures->len; ++i) {
        LilyCheckedSignatureFun *signature = get__Vec(fun->fun.signatures, i);

        if (contains_compiler_defined_dt__LilyCheckedSignatureFun(signature)) {
            continue;
        }

        ASSERT(signature->types->len != 0);

        LilyMirInstruction *inst = NEW_VARIANT(
          LilyMirInstruction,
          fun,
          NEW(LilyMirInstructionFun,
              get_linkage_from_visibility(fun->fun.visibility),
              fun->fun.is_main ? "main" : signature->ser_global_name->buffer,
              NEW(Vec),
              generate_dt__LilyMir(last__Vec(signature->types))));

        if (signature->types->len > 0) {
            for (Usize i = 0; i < signature->types->len - 1; ++i) {
                LilyCheckedDataType *type = get__Vec(signature->types, i);

                LilyMirAddParam(&inst->fun.scope, type);

                push__Vec(
                  inst->fun.args,
                  NEW_VARIANT(
                    LilyMirInstruction,
                    arg,
                    NEW(LilyMirInstructionArg, generate_dt__LilyMir(type), i)));
            }
        }

        LilyMirAddInst(module, inst);

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

        if (i != fun->fun.signatures->len - 1) {
            LilyMirPopCurrent(module);
        }
    }
}
