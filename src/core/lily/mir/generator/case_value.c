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

#include <core/lily/mir/generator/case_value.h>

Vec *
generate_case_value__LilyMir(LilyMirModule *module,
                             LilyCheckedSignatureFun *fun_signature,
                             LilyMirScope *scope,
                             LilyCheckedStmtSwitchCaseValue *case_value)
{
    switch (case_value->kind) {
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_BOOL:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         int,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_I1),
                                         case_value->bool_));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT8:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         int,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_I8),
                                         case_value->int8));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT16:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         int,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_I16),
                                         case_value->int16));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT32:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         int,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_I32),
                                         case_value->int32));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT64:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         int,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_I64),
                                         case_value->int64));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ISIZE:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         int,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_ISIZE),
                                         case_value->isize));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT32:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         float,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_F32),
                                         case_value->float32));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT64:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         float,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_F64),
                                         case_value->float64));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT8:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         uint,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_U8),
                                         case_value->uint8));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT16:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         uint,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_U16),
                                         case_value->uint16));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT32:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         uint,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_U32),
                                         case_value->uint32));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT64:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         uint,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_U64),
                                         case_value->uint64));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_USIZE:
            return init__Vec(1,
                             NEW_VARIANT(LilyMirInstructionVal,
                                         uint,
                                         NEW(LilyMirDt, LILY_MIR_DT_KIND_USIZE),
                                         case_value->usize));
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE:
            return init__Vec(1, NULL);
        case LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UNION: {
            Vec *case_values = NEW(Vec);

            for (Usize i = 0; i < case_value->union_->len; ++i) {
                push__Vec(case_values,
                          generate_case_value__LilyMir(
                            module,
                            fun_signature,
                            scope,
                            get__Vec(case_value->union_, i)));
            }

            return case_values;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}
