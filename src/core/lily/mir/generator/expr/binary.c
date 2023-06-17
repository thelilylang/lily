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
#include <core/lily/mir/generator/expr/assignable.h>
#include <core/lily/mir/generator/expr/binary.h>

#define GENERATE_ASSIGN_BINARY(inst_name, value)                               \
    {                                                                          \
        LilyMirAddInst(                                                        \
          module,                                                              \
          LilyMirBuildReg(module,                                              \
                          NEW_VARIANT(LilyMirInstruction, inst_name, value))); \
                                                                               \
        lily_free(left_inst);                                                  \
        lily_free(right_inst);                                                 \
                                                                               \
        return LilyMirBuildStore(                                              \
          generate_assignable_expr__LilyMir(                                   \
            module, fun_signature, scope, expr->binary.left),                  \
          LilyMirBuildRegVal(module,                                           \
                             generate_dt__LilyMir(right_data_type),            \
                             from__String(LilyMirGetLastRegName(module))));    \
    }

LilyMirInstruction *
generate_binary_expr__LilyMir(LilyMirModule *module,
                              LilyCheckedSignatureFun *fun_signature,
                              LilyMirScope *scope,
                              LilyCheckedExpr *expr)
{
    ASSERT(expr->kind == LILY_CHECKED_EXPR_KIND_BINARY);

    LilyCheckedDataType *left_data_type =
      LilyMirGetCheckedDtFromExpr(module, scope, expr->binary.left);
    LilyCheckedDataType *right_data_type =
      LilyMirGetCheckedDtFromExpr(module, scope, expr->binary.right);

    LilyMirInstruction *left_inst =
      generate_expr__LilyMir(module, fun_signature, scope, expr->binary.left);
    LilyMirInstruction *right_inst =
      generate_expr__LilyMir(module, fun_signature, scope, expr->binary.right);

    ASSERT(left_inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL &&
           right_inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

    bool operator_is_builtin = false;

    switch (expr->binary.kind) {
        case LILY_CHECKED_EXPR_BINARY_KIND_AND:
        case LILY_CHECKED_EXPR_BINARY_KIND_OR:
            if (left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BOOL &&
                right_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BOOL) {
                operator_is_builtin = true;
            }

            break;
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_XOR:
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND:
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_L_SHIFT:
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR:
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_R_SHIFT:
        case LILY_CHECKED_EXPR_BINARY_KIND_XOR:
            if ((is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                 left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BOOL ||
                 left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) &&
                left_data_type->kind == right_data_type->kind) {
                operator_is_builtin = true;
            }

            break;
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL:
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB:
        case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
        case LILY_CHECKED_EXPR_BINARY_KIND_DIV:
        case LILY_CHECKED_EXPR_BINARY_KIND_EXP:
        case LILY_CHECKED_EXPR_BINARY_KIND_MOD:
        case LILY_CHECKED_EXPR_BINARY_KIND_MUL:
        case LILY_CHECKED_EXPR_BINARY_KIND_SUB:
            if ((is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                 is_float_data_type__LilyCheckedDataType(left_data_type) ||
                 left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) &&
                left_data_type->kind == right_data_type->kind) {
                operator_is_builtin = true;
            }

            break;
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN:
            operator_is_builtin = true;

            break;
        case LILY_CHECKED_EXPR_BINARY_KIND_EQ:
        case LILY_CHECKED_EXPR_BINARY_KIND_NOT_EQ:
        case LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ:
        case LILY_CHECKED_EXPR_BINARY_KIND_GREATER:
        case LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ:
        case LILY_CHECKED_EXPR_BINARY_KIND_LESS:
            if ((is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                 is_float_data_type__LilyCheckedDataType(left_data_type) ||
                 left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE ||
                 left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BOOL) &&
                left_data_type->kind == right_data_type->kind) {
                operator_is_builtin = true;
            }

            break;
        case LILY_CHECKED_EXPR_BINARY_KIND_LIST_HEAD:
        case LILY_CHECKED_EXPR_BINARY_KIND_LIST_TAIL:
            TODO("generate -> and <-");
        case LILY_CHECKED_EXPR_BINARY_KIND_RANGE:
            TODO("generate ..");
        default:
            UNREACHABLE("unknown variant");
    }

    if (operator_is_builtin) {
        LilyMirInstruction *op_inst = NULL;

        switch (expr->binary.kind) {
            case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          iadd,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fadd,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_AND:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                      and,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    GENERATE_ASSIGN_BINARY(iadd,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                } else {
                    GENERATE_ASSIGN_BINARY(fadd,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                }
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
                GENERATE_ASSIGN_BINARY(bitand,
                                       NEW(LilyMirInstructionDestSrc,
                                           left_inst->val,
                                           right_inst->val));
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
                GENERATE_ASSIGN_BINARY(shl,
                                       NEW(LilyMirInstructionDestSrc,
                                           left_inst->val,
                                           right_inst->val));
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
                GENERATE_ASSIGN_BINARY(bitor,
                                       NEW(LilyMirInstructionDestSrc,
                                           left_inst->val,
                                           right_inst->val));
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
                GENERATE_ASSIGN_BINARY(shr,
                                       NEW(LilyMirInstructionDestSrc,
                                           left_inst->val,
                                           right_inst->val));
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    GENERATE_ASSIGN_BINARY(idiv,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                } else {
                    GENERATE_ASSIGN_BINARY(fdiv,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                }
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP:
                GENERATE_ASSIGN_BINARY(exp,
                                       NEW(LilyMirInstructionDestSrc,
                                           left_inst->val,
                                           right_inst->val));
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    GENERATE_ASSIGN_BINARY(irem,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                } else {
                    GENERATE_ASSIGN_BINARY(frem,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                }
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    GENERATE_ASSIGN_BINARY(imul,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                } else {
                    GENERATE_ASSIGN_BINARY(fmul,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                }
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    GENERATE_ASSIGN_BINARY(isub,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                } else {
                    GENERATE_ASSIGN_BINARY(fsub,
                                           NEW(LilyMirInstructionDestSrc,
                                               left_inst->val,
                                               right_inst->val));
                }
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_XOR:
                GENERATE_ASSIGN_BINARY(xor,
                                       NEW(LilyMirInstructionDestSrc,
                                           left_inst->val,
                                           right_inst->val));
            case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN:
                return LilyMirBuildStore(left_inst->val, right_inst->val);
            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                        bitand,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                      bitor
                                      ,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_DIV:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          idiv,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fdiv,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_EQ:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          icmp_eq,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fcmp_eq,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_EXP:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                      exp,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          icmp_ge,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fcmp_ge,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_GREATER:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          icmp_gt,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fcmp_gt,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_L_SHIFT:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                      shl,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          icmp_le,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fcmp_le,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_LESS:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          icmp_lt,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fcmp_lt,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_MOD:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          irem,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          frem,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_MUL:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          imul,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fmul,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_NOT_EQ:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          icmp_ne,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fcmp_ne,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_OR:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                      or
                                      ,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_R_SHIFT:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                      shr,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_RANGE:
                TODO("generate .. in MIR");
            case LILY_CHECKED_EXPR_BINARY_KIND_SUB:
                if (is_integer_data_type__LilyCheckedDataType(left_data_type) ||
                    left_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BYTE) {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          isub,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                } else {
                    op_inst = NEW_VARIANT(LilyMirInstruction,
                                          fsub,
                                          NEW(LilyMirInstructionDestSrc,
                                              left_inst->val,
                                              right_inst->val));
                }

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_XOR:
                op_inst = NEW_VARIANT(LilyMirInstruction,
                                      xor,
                                      NEW(LilyMirInstructionDestSrc,
                                          left_inst->val,
                                          right_inst->val));

                break;
            case LILY_CHECKED_EXPR_BINARY_KIND_LIST_HEAD:
                TODO("generate -> in MIR");
            case LILY_CHECKED_EXPR_BINARY_KIND_LIST_TAIL:
                TODO("generate <- in MIR");
            default:
                UNREACHABLE("unknown variant");
        }

        lily_free(left_inst);
        lily_free(right_inst);

        LilyMirAddInst(module, LilyMirBuildReg(module, op_inst));

        LilyCheckedDataType *return_data_type = NULL;

        switch (expr->data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
                Vec *cond = init__Vec(2, left_data_type, right_data_type);

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
                      generate_dt__LilyMir(return_data_type),
                      from__String(LilyMirGetLastRegName(module))));
    } else {
        TODO("generate for user defined binary operator");
    }
}
