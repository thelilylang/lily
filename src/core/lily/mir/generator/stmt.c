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

#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/stmt.h>

LilyMirInstruction *
generate_stmt__LilyMir(LilyMirModule *module,
                       LilyCheckedSignatureFun *fun_signature,
                       LilyMirScope *scope,
                       LilyMirBlockLimit *block_limit,
                       LilyMirInstruction *exit_block,
                       LilyMirInstruction *next_block,
                       LilyCheckedStmt *stmt)
{
    switch (stmt->kind) {
        case LILY_CHECKED_STMT_KIND_ASM:
            TODO("generate asm stmt");
        case LILY_CHECKED_STMT_KIND_AWAIT:
            TODO("generate await stmt");
        case LILY_CHECKED_STMT_KIND_BLOCK:
            LilyMirBuildBlockStmt(
              module, fun_signature, scope, block_limit, &stmt->block);

            return NULL;
        case LILY_CHECKED_STMT_KIND_BREAK:
            LilyMirBuildBreak(module, exit_block);

            return NULL;
        case LILY_CHECKED_STMT_KIND_DROP:
            TODO("generate drop stmt");
        case LILY_CHECKED_STMT_KIND_FOR:
            TODO("generate for stmt");
        case LILY_CHECKED_STMT_KIND_IF:
            LilyMirBuildIf(
              module, fun_signature, scope, block_limit, &stmt->if_);

            return NULL;
        case LILY_CHECKED_STMT_KIND_MATCH:
            UNREACHABLE("match statement is not expected for MIR generator");
        case LILY_CHECKED_STMT_KIND_NEXT:
            LilyMirBuildNext(module, next_block);

            return NULL;
        case LILY_CHECKED_STMT_KIND_RAISE:
            TODO("generate raise stmt");
        case LILY_CHECKED_STMT_KIND_RETURN:
            if (stmt->return_.expr) {
                return NEW_VARIANT(LilyMirInstruction,
                                   ret,
                                   generate_expr__LilyMir(module,
                                                          fun_signature,
                                                          scope,
                                                          stmt->return_.expr,
                                                          NULL,
                                                          true));
            } else {
                return NEW_VARIANT(
                  LilyMirInstruction,
                  ret,
                  NEW_VARIANT(LilyMirInstruction,
                              val,
                              NEW(LilyMirInstructionVal,
                                  LILY_MIR_INSTRUCTION_VAL_KIND_UNIT,
                                  NEW(LilyMirDt, LILY_MIR_DT_KIND_UNIT))));
            }
        case LILY_CHECKED_STMT_KIND_SWITCH:
            LilyMirBuildSwitch(
              module, fun_signature, scope, block_limit, &stmt->switch_);

            return NULL;
        case LILY_CHECKED_STMT_KIND_TRY:
            TODO("generate try stmt");
        case LILY_CHECKED_STMT_KIND_UNSAFE:
            LilyMirBuildUnsafe(
              module, fun_signature, scope, block_limit, &stmt->unsafe);

            return NULL;
        case LILY_CHECKED_STMT_KIND_VARIABLE: {
            // TODO: add ref count for LilyMirDt

            LilyCheckedDataType *var_data_type =
              LilyMirGetCheckedDtFromExpr(module, scope, stmt->variable.expr);

            LilyMirAddVar(scope, stmt->variable.name, var_data_type);
            LilyMirBuildVarAlloc(module,
                                 stmt->variable.name->buffer,
                                 generate_dt__LilyMir(module, var_data_type));

            LilyMirInstructionVal *var_val =
              NEW_VARIANT(LilyMirInstructionVal,
                          var,
                          generate_dt__LilyMir(module, var_data_type),
                          stmt->variable.name->buffer);
            LilyMirInstruction *inst =
              generate_expr__LilyMir(module,
                                     fun_signature,
                                     scope,
                                     stmt->variable.expr,
                                     var_val,
                                     false);

            FREE(LilyMirInstructionVal, var_val);

            LilyMirBuildVar(module,
                            stmt->variable.name->buffer,
                            generate_dt__LilyMir(module, var_data_type),
                            inst,
                            false);

            return NULL;
        }
        case LILY_CHECKED_STMT_KIND_WHILE:
            LilyMirBuildWhile(
              module, fun_signature, scope, block_limit, &stmt->while_);

            return NULL;
        default:
            UNREACHABLE("unknown variant");
    }
}
