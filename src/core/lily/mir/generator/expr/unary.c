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
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/expr/unary.h>

LilyMirInstruction *
generate_unary_expr__LilyMir(LilyMirModule *module,
                             LilyCheckedSignatureFun *fun_signature,
                             LilyMirScope *scope,
                             LilyCheckedExpr *expr,
                             LilyMirInstructionVal *ptr_val,
                             bool in_return)
{
    ASSERT(expr->kind == LILY_CHECKED_EXPR_KIND_UNARY);

    LilyCheckedDataType *right_data_type =
      LilyMirGetCheckedDtFromExpr(module, scope, expr->unary.right);

    LilyMirInstruction *right_inst = generate_expr__LilyMir(
      module, fun_signature, scope, expr->unary.right, ptr_val, in_return);

    ASSERT(right_inst);
    ASSERT(right_inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

    bool operator_is_builtin = false;

    switch (expr->unary.kind) {
        case LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE:
            if (right_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_PTR ||
                right_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_REF) {
                operator_is_builtin = true;
            }

            break;
        case LILY_CHECKED_EXPR_UNARY_KIND_NEG:
            if (is_signed_integer_data_type__LilyCheckedDataType(
                  right_data_type) ||
                is_float_data_type__LilyCheckedDataType(right_data_type)) {
                operator_is_builtin = true;
            }

            break;
        case LILY_CHECKED_EXPR_UNARY_KIND_NOT:
            if (right_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BOOL) {
                operator_is_builtin = true;
            }

            break;
        case LILY_CHECKED_EXPR_UNARY_KIND_REF:
        case LILY_CHECKED_EXPR_UNARY_KIND_REF_MUT:
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE:
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE_MUT:
            operator_is_builtin = true;

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    if (operator_is_builtin) {
        LilyMirInstruction *op_inst = NULL;

        switch (expr->unary.kind) {
            case LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE:
                op_inst =
                  NEW_VARIANT(LilyMirInstruction,
                              getptr,
                              NEW(LilyMirInstructionSrc, right_inst->val));

                break;
            case LILY_CHECKED_EXPR_UNARY_KIND_NEG:
                if (is_float_data_type__LilyCheckedDataType(right_data_type)) {
                    op_inst =
                      NEW_VARIANT(LilyMirInstruction,
                                  fneg,
                                  NEW(LilyMirInstructionSrc, right_inst->val));
                } else {
                    op_inst =
                      NEW_VARIANT(LilyMirInstruction,
                                  ineg,
                                  NEW(LilyMirInstructionSrc, right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_UNARY_KIND_NOT:
                op_inst =
                  NEW_VARIANT(LilyMirInstruction,
                              not,
                              NEW(LilyMirInstructionSrc, right_inst->val));

                break;
            case LILY_CHECKED_EXPR_UNARY_KIND_REF:
            case LILY_CHECKED_EXPR_UNARY_KIND_REF_MUT:
            case LILY_CHECKED_EXPR_UNARY_KIND_TRACE:
            case LILY_CHECKED_EXPR_UNARY_KIND_TRACE_MUT:
                TODO("generate MIR unary ref, ref mut, trace, trace mut");
            default:
                UNREACHABLE("unknown variant");
        }

        partial_free__LilyMirInstruction(right_inst);

        LilyMirAddInst(module, LilyMirBuildReg(module, op_inst));

        LilyCheckedDataType *return_data_type = NULL;

        switch (expr->data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
                Vec *cond = init__Vec(1, right_data_type);

                return_data_type =
                  get_return_data_type_of_conditional_compiler_choice(
                    expr->data_type, cond);

                ASSERT(return_data_type);

                FREE(Vec, cond);

                break;
            }
            default:
                return_data_type = expr->data_type;
        }

        return NEW_VARIANT(
          LilyMirInstruction,
          val,
          NEW_VARIANT(LilyMirInstructionVal,
                      reg,
                      generate_dt__LilyMir(module, return_data_type),
                      LilyMirGetLastRegName(module)));
    } else {
        TODO("generate for user defined unary operator");
    }
}
