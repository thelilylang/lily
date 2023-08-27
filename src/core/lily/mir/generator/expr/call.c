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

#include <core/lily/analysis/checked/decl.h>
#include <core/lily/analysis/checked/decl/fun.h>
#include <core/lily/analysis/checked/limits.h>
#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/expr/call.h>

LilyMirInstruction *
generate_call_expr__LilyMir(LilyMirModule *module,
                            LilyCheckedSignatureFun *fun_signature,
                            LilyMirScope *scope,
                            LilyCheckedExpr *expr,
                            LilyMirInstructionVal *ptr_val)
{
    switch (expr->call.kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_ATTRIBUTE:
            TODO("attribute");
        case LILY_CHECKED_EXPR_CALL_KIND_CLASS:
            TODO("class");
        case LILY_CHECKED_EXPR_CALL_KIND_CONSTANT:
            TODO("constant");
        case LILY_CHECKED_EXPR_CALL_KIND_CSTR_LEN:
            TODO("cstr_len");
        case LILY_CHECKED_EXPR_CALL_KIND_ERROR:
            TODO("error");
        case LILY_CHECKED_EXPR_CALL_KIND_FUN: {
            Vec *params = NEW(Vec);
            LilyMirDt *types[MAX_FUN_PARAMS + 1] = { 0 };
            const Usize types_len =
              expr->call.fun.params ? expr->call.fun.params->len + 1 : 1;

            if (expr->call.fun.params) {
                for (Usize i = 0; i < expr->call.fun.params->len; ++i) {
                    LilyCheckedExprCallFunParam *param =
                      get__Vec(expr->call.fun.params, i);

                    LilyMirInstruction *inst =
                      generate_expr__LilyMir(module,
                                             fun_signature,
                                             scope,
                                             param->value,
                                             ptr_val,
                                             false);

                    ASSERT(inst);
                    ASSERT(inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

                    push__Vec(params, inst->val);
                    types[i] = inst->val->dt;

                    lily_free(inst);
                }
            }

            LilyCheckedDataType *return_data_type = NULL;

            switch (expr->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
                    Vec *cond = NEW(Vec);

                    for (Usize i = 0; i < expr->call.fun.params->len; ++i) {
                        push__Vec(
                          cond,
                          LilyMirGetCheckedDtFromExpr(
                            module, scope, get__Vec(expr->call.fun.params, i)));
                    }

                    return_data_type =
                      get_return_data_type_of_conditional_compiler_choice(
                        expr->data_type, cond);

                    FREE(Vec, cond);

                    ASSERT(return_data_type);

                    break;
                }
                default:
                    return_data_type = expr->data_type;
            }

            LilyMirDt *mir_return_data_type =
              generate_dt__LilyMir(module, return_data_type);
            types[types_len - 1] = mir_return_data_type;

            return LilyMirBuildCall(
              module,
              mir_return_data_type,
              LilyMirGetFunNameFromTypes(
                module,
                expr->call.fun.fun->fun.global_name->buffer,
                types,
                types_len),
              params);
        }
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS:
            TODO("fun sys");
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_BUILTIN:
            TODO("fun builtin");
        case LILY_CHECKED_EXPR_CALL_KIND_METHOD:
            TODO("method");
        case LILY_CHECKED_EXPR_CALL_KIND_MODULE:
            UNREACHABLE("module call not expected in the MIR");
        case LILY_CHECKED_EXPR_CALL_KIND_RECORD: {
            LilyMirDt *dt = generate_dt__LilyMir(module, expr->data_type);
            Vec *struct_ = NEW(Vec); // Vec<LilyMirInstructionVal*>*

            if (expr->call.record.params) {
                for (Usize i = 0; i < expr->call.record.params->len; ++i) {
                    LilyMirInstruction *val = generate_expr__LilyMir(
                      module,
                      fun_signature,
                      scope,
                      CAST(LilyCheckedExprCallRecordParam *,
                           get__Vec(expr->call.record.params, i))
                        ->value,
                      ptr_val,
                      false);

                    ASSERT(val);
                    ASSERT(val->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

                    push__Vec(struct_, val->val);
                    lily_free(val);
                }
            }

            return NEW_VARIANT(
              LilyMirInstruction, val, LilyMirBuildStruct(module, dt, struct_));
        }
        case LILY_CHECKED_EXPR_CALL_KIND_STR_LEN:
            TODO("str len");
        case LILY_CHECKED_EXPR_CALL_KIND_VARIANT:
            TODO("variant");
        case LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN:
            UNREACHABLE("the analysis has a bug!!");
        default:
            UNREACHABLE("not expected to generate in this function");
    }
}
