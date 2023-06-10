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

#include <base/assert.h>

#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/val.h>

#include <stdio.h>
#include <string.h>

LilyMirInstruction *
generate_expr__LilyMir(LilyMirModule *module, LilyCheckedExpr *expr)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_BINARY: {
            LilyMirInstruction *left_inst =
              generate_expr__LilyMir(module, expr->binary.left);
            LilyMirInstruction *right_inst =
              generate_expr__LilyMir(module, expr->binary.right);

            ASSERT(left_inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL &&
                   right_inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

            LilyMirInstruction *op_inst = NULL;

            switch (expr->binary.kind) {
                case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
                    if (is_integer_data_type__LilyCheckedDataType(
                          expr->data_type) ||
                        expr->data_type->kind ==
                          LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                        op_inst = NEW_VARIANT(LilyMirInstruction,
                                              iadd,
                                              NEW(LilyMirInstructionSrcDest,
                                                  left_inst->val,
                                                  right_inst->val));
                    } else {
                        op_inst = NEW_VARIANT(LilyMirInstruction,
                                              fadd,
                                              NEW(LilyMirInstructionSrcDest,
                                                  left_inst->val,
                                                  right_inst->val));
                    }

                    break;
                case LILY_CHECKED_EXPR_BINARY_KIND_AND:
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          and,
                                          NEW(LilyMirInstructionSrcDest,
                                              left_inst->val,
                                              right_inst->val));

                    break;
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD:
                    if (is_integer_data_type__LilyCheckedDataType(
                          expr->binary.left->data_type) ||
                        expr->binary.left->data_type->kind ==
                          LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                        LilyMirAddInst(
                          module,
                          LilyMirBuildReg(
                            module,
                            NEW_VARIANT(LilyMirInstruction,
                                        iadd,
                                        NEW(LilyMirInstructionSrcDest,
                                            left_inst->val,
                                            right_inst->val))));

                        return LilyMirBuildStore(
                          left_inst->val,
                          LilyMirBuildRegVal(
                            module,
                            generate_dt__LilyMir(expr->binary.right->data_type),
                            from__String(LilyMirGetLastRegName(module))));
                    } else {
                        LilyMirAddInst(
                          module,
                          LilyMirBuildReg(
                            module,
                            NEW_VARIANT(LilyMirInstruction,
                                        fadd,
                                        NEW(LilyMirInstructionSrcDest,
                                            left_inst->val,
                                            right_inst->val))));

                        return LilyMirBuildStore(
                          left_inst->val,
                          LilyMirBuildRegVal(
                            module,
                            generate_dt__LilyMir(expr->binary.right->data_type),
                            from__String(LilyMirGetLastRegName(module))));
                    }
                default:
                    UNREACHABLE("unknown variant");
            }

            lily_free(left_inst);
            lily_free(right_inst);

            LilyMirAddInst(module, LilyMirBuildReg(module, op_inst));

            return NEW_VARIANT(
              LilyMirInstruction,
              val,
              NEW_VARIANT(LilyMirInstructionVal,
                          reg,
                          generate_dt__LilyMir(expr->data_type),
                          from__String(LilyMirGetLastRegName(module))));
        }
        case LILY_CHECKED_EXPR_KIND_CALL:
            break;
        case LILY_CHECKED_EXPR_KIND_CAST:
            break;
        case LILY_CHECKED_EXPR_KIND_GROUPING:
            return generate_expr__LilyMir(module, expr->grouping);
        case LILY_CHECKED_EXPR_KIND_LAMBDA:
            break;
        case LILY_CHECKED_EXPR_KIND_UNARY:
            break;
        default:
            return NEW_VARIANT(
              LilyMirInstruction, val, generate_val__LilyMir(module, expr));
    }
}
