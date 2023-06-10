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

#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/fun.h>
#include <core/lily/mir/generator/val.h>
#include <core/lily/mir/linkage.h>

#include <stdio.h>

void
generate_fun__LilyMir(LilyMirModule *module, LilyCheckedDecl *fun)
{
    Vec *args = NEW(Vec);

    // TODO: add support of signature variants
    if (fun->fun.params) {
        for (Usize i = 0; i < fun->fun.params->len; ++i) {
            LilyCheckedDeclFunParam *param = get__Vec(fun->fun.params, i);

            push__Vec(args,
                      NEW_VARIANT(LilyMirInstruction,
                                  arg,
                                  NEW(LilyMirInstructionArg,
                                      generate_dt__LilyMir(param->data_type),
                                      param->name->buffer)));
        }
    }

    LilyMirInstruction *inst =
      NEW_VARIANT(LilyMirInstruction,
                  fun,
                  NEW(LilyMirInstructionFun,
                      get_linkage_from_visibility(fun->fun.visibility),
                      fun->fun.global_name->buffer,
                      args));

    LilyMirAddInst(module, inst);

    for (Usize i = 0; i < fun->fun.body->len; ++i) {
        LilyCheckedBodyFunItem *item = get__Vec(fun->fun.body, i);

        switch (item->kind) {
            case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:
                LilyMirAddInst(module,
                               generate_expr__LilyMir(module, item->expr));

                break;
            case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:
                TODO("generate stmt");
        }
    }
}
