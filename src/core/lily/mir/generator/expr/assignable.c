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

#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr/assignable.h>

LilyMirInstruction *
generate_assignable_expr__LilyMir(LilyMirModule *module,
                                  LilyCheckedSignatureFun *fun_signature,
                                  LilyMirScope *scope,
                                  LilyCheckedExpr *expr)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_CALL:
            switch (expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
                    return NEW_VARIANT(
                      LilyMirInstruction,
                      val,
                      NEW_VARIANT(LilyMirInstructionVal,
                                  var,
                                  generate_dt__LilyMir(module, expr->data_type),
                                  expr->call.global_name->buffer));
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
                    return NEW_VARIANT(
                      LilyMirInstruction,
                      val,
                      NEW_VARIANT(LilyMirInstructionVal,
                                  param,
                                  NEW_VARIANT(LilyMirDt,
                                              ptr,
                                              generate_dt__LilyMir(
                                                module, expr->data_type)),
                                  expr->call.fun_param));
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS:
                    TODO("record field access");
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE:
                    TODO("record field single");
                default:
                    UNREACHABLE("can't assignable");
            }

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
