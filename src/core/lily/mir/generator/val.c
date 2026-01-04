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
#include <core/lily/mir/generator/literal.h>
#include <core/lily/mir/generator/val.h>

#include <stdio.h>
#include <string.h>

LilyMirInstructionVal *
generate_val__LilyMir(LilyMirModule *module,
                      LilyCheckedSignatureFun *fun_signature,
                      LilyMirScope *scope,
                      LilyCheckedExpr *expr,
                      bool in_return)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_ARRAY: {
            switch (expr->data_type->array.kind) {
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_DYNAMIC:
                    TODO("do [_]");
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED: {
                    Vec *array = NEW(Vec); // Vec<LilyMirInstructionVal*>*

                    for (Usize i = 0; i < expr->array.items->len; ++i) {
                        push__Vec(
                          array,
                          generate_val__LilyMir(module,
                                                fun_signature,
                                                scope,
                                                get__Vec(expr->array.items, i),
                                                in_return));
                    }

                    return NEW_VARIANT(
                      LilyMirInstructionVal,
                      array,
                      generate_dt__LilyMir(module, expr->data_type),
                      array);
                }
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS:
                    TODO("do [*]");
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNDETERMINED:
                    TODO("do [?]");
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN:
                    TODO("do unknown array");
                default:
                    UNREACHABLE("unknown variant");
            }
        }
        case LILY_CHECKED_EXPR_KIND_CALL:
            switch (expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_CONSTANT: {
                    if (scope) {
                        return LilyMirBuildLoad(
                          module,
                          NEW_VARIANT(
                            LilyMirInstructionVal,
                            const,
                            generate_dt__LilyMir(module, expr->data_type),
                            expr->call.global_name->buffer),
                          generate_dt__LilyMir(module, expr->data_type),
                          NEW_VARIANT(LilyMirInstructionFunLoadName,
                                      var,
                                      expr->call.global_name->buffer));
                    }

                    LilyMirInstruction *const_inst = get__OrderedHashMap(
                      module->insts, expr->call.global_name->buffer);

                    ASSERT(const_inst);

                    switch (const_inst->const_.val->dt->kind) {
                        case LILY_MIR_DT_KIND_I1:
                        case LILY_MIR_DT_KIND_I8:
                        case LILY_MIR_DT_KIND_I16:
                        case LILY_MIR_DT_KIND_I32:
                        case LILY_MIR_DT_KIND_I64:
                        case LILY_MIR_DT_KIND_ISIZE:
                        case LILY_MIR_DT_KIND_U8:
                        case LILY_MIR_DT_KIND_U16:
                        case LILY_MIR_DT_KIND_U32:
                        case LILY_MIR_DT_KIND_U64:
                        case LILY_MIR_DT_KIND_USIZE:
                        case LILY_MIR_DT_KIND_F32:
                        case LILY_MIR_DT_KIND_F64:
                        case LILY_MIR_DT_KIND_PTR:
                            // TODO: maybe clone instead of ref count in this
                            // case.
                            return ref__LilyMirInstructionVal(
                              const_inst->const_.val);
                        default:
                            UNREACHABLE(
                              "impossible to get this value at comptime or "
                              "is not yet implemented");
                    }
                }
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE: {
                    ASSERT(scope);

                    return LilyMirBuildLoad(
                      module,
                      NEW_VARIANT(
                        LilyMirInstructionVal,
                        var,
                        generate_dt__LilyMir(
                          module,
                          LilyMirGetCheckedDtFromExpr(module, scope, expr)),
                        expr->call.global_name->buffer),
                      generate_dt__LilyMir(module, expr->data_type),
                      NEW_VARIANT(LilyMirInstructionFunLoadName,
                                  var,
                                  expr->call.global_name->buffer));
                }
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
                    ASSERT(fun_signature);
                    ASSERT(expr->call.fun_param < fun_signature->types->len);

                    return NEW_VARIANT(
                      LilyMirInstructionVal,
                      param,
                      generate_dt__LilyMir(
                        module,
                        get__Vec(fun_signature->types, expr->call.fun_param)),
                      expr->call.fun_param);
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS:
                    TODO("record field access");
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE:
                    TODO("record field single");
                default:
                    UNREACHABLE("not expected to generate a val");
            }
        case LILY_CHECKED_EXPR_KIND_LITERAL:
            GENERATE_LITERAL(expr, EXPR, in_return);
        case LILY_CHECKED_EXPR_KIND_TUPLE: {
            Vec *tuple = NEW(Vec); // Vec<LilyMirInstructionVal*>*

            for (Usize i = 0; i < expr->tuple.items->len; ++i) {
                push__Vec(tuple,
                          generate_val__LilyMir(module,
                                                fun_signature,
                                                scope,
                                                get__Vec(expr->tuple.items, i),
                                                in_return));
            }

            return NEW_VARIANT(LilyMirInstructionVal,
                               tuple,
                               generate_dt__LilyMir(module, expr->data_type),
                               tuple);
        }
        case LILY_CHECKED_EXPR_KIND_LIST: {
            Vec *list = NEW(Vec); // Vec<LilyMirInstructionVal*>*

            for (Usize i = 0; i < expr->list.items->len; ++i) {
                push__Vec(list,
                          generate_val__LilyMir(module,
                                                fun_signature,
                                                scope,
                                                get__Vec(expr->list.items, i),
                                                in_return));
            }

            return NEW_VARIANT(LilyMirInstructionVal,
                               list,
                               generate_dt__LilyMir(module, expr->data_type),
                               list);
        }
        default:
            UNREACHABLE("cannot generate a val");
    }
}
