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

#include <stdio.h>
#include <stdlib.h>

LilyMirDt *
generate_dt__LilyMir(LilyMirModule *module, LilyCheckedDataType *data_type)
{
    switch (data_type->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_ANY);
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            switch (data_type->array.kind) {
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_DYNAMIC:
                    return NEW_VARIANT(
                      LilyMirDt,
                      ptr,
                      generate_dt__LilyMir(module, data_type->array.data_type));
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS:
                    return NEW_VARIANT(
                      LilyMirDt,
                      ptr,
                      generate_dt__LilyMir(module, data_type->array.data_type));
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED:
                    return NEW_VARIANT(
                      LilyMirDt,
                      array,
                      NEW(
                        LilyMirDtArray,
                        NEW_VARIANT(LilyMirDtLen, def, data_type->array.sized),
                        generate_dt__LilyMir(module,
                                             data_type->array.data_type)));
                case LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNDETERMINED:
                    return NEW(LilyMirDt, LILY_MIR_DT_KIND_C_VA_ARG);
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I1);
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U8);
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
            return NEW_VARIANT(
              LilyMirDt, ptr, NEW(LilyMirDt, LILY_MIR_DT_KIND_U8));
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U32);
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I16);
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U16);
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I32);
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U32);
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I32);
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U32);
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I64);
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U64);
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_F32);
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_F64);
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
            return NEW_VARIANT(
              LilyMirDt, ptr, NEW(LilyMirDt, LILY_MIR_DT_KIND_U8));
        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_UNIT);
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            switch (data_type->custom.kind) {
                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC: {
                    LilyMirCurrent *top = LilyMirGetCurrentOnTop(module);

                    ASSERT(top);

                    switch (top->kind) {
                        case LILY_MIR_CURRENT_KIND_CONST:
                            UNREACHABLE(
                              "const is not expected in this context");
                        case LILY_MIR_CURRENT_KIND_FUN:
                            return generate_dt__LilyMir(
                              module,
                              get__HashMap(top->inst->fun.generic_params,
                                           data_type->custom.name->buffer));
                        case LILY_MIR_CURRENT_KIND_STRUCT:
                            return generate_dt__LilyMir(
                              module,
                              get__OrderedHashMap(
                                top->inst->struct_.generic_params,
                                data_type->custom.name->buffer));
                        default:
                            UNREACHABLE("unknown variant");
                    }
                }
                default:
                    return NEW_VARIANT(LilyMirDt,
                                       struct_name,
                                       data_type->custom.global_name->buffer);
            }
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT: {
            if (data_type->result->errs) {
                if (data_type->result->errs->len > 1) {
                    Vec *struct_ =
                      init__Vec(1, NEW(LilyMirDt, LILY_MIR_DT_KIND_U8));

                    for (Usize i = 0; i < data_type->result->errs->len; ++i) {
                        push__Vec(
                          struct_,
                          generate_dt__LilyMir(
                            module, get__Vec(data_type->result->errs, i)));
                    }

                    return NEW_VARIANT(
                      LilyMirDt,
                      result,
                      NEW(LilyMirDtResult,
                          generate_dt__LilyMir(module, data_type->result->ok),
                          NEW_VARIANT(LilyMirDt, struct, struct_)));
                }

                return NEW_VARIANT(
                  LilyMirDt,
                  result,
                  NEW(LilyMirDtResult,
                      generate_dt__LilyMir(module, data_type->result->ok),
                      generate_dt__LilyMir(
                        module, last__Vec(data_type->result->errs))));
            }

            return NEW_VARIANT(
              LilyMirDt,
              result,
              NEW(LilyMirDtResult,
                  generate_dt__LilyMir(module, data_type->result->ok),
                  NEW(LilyMirDt, LILY_MIR_DT_KIND_ANY)));
        }
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_F32);
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_F64);
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I16);
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I32);
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I64);
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_I8);
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_ISIZE);
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            TODO("lambda...");
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            return NEW_VARIANT(
              LilyMirDt, list, generate_dt__LilyMir(module, data_type->list));
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return generate_dt__LilyMir(module, data_type->mut);
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_UNIT);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            return generate_dt__LilyMir(module, data_type->optional);
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return NEW_VARIANT(
              LilyMirDt, ptr, generate_dt__LilyMir(module, data_type->ptr));
        case LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT:
            return NEW_VARIANT(
              LilyMirDt, ptr, generate_dt__LilyMir(module, data_type->ptr_mut));
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return NEW_VARIANT(
              LilyMirDt, ref, generate_dt__LilyMir(module, data_type->ref));
        case LILY_CHECKED_DATA_TYPE_KIND_REF_MUT:
            return NEW_VARIANT(
              LilyMirDt, ref, generate_dt__LilyMir(module, data_type->ref_mut));
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            return NEW_VARIANT(
              LilyMirDt, str, from_check_len__LilyMirDtLen(&data_type->str));
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return NEW_VARIANT(
              LilyMirDt, trace, generate_dt__LilyMir(module, data_type->trace));
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT:
            return NEW_VARIANT(
              LilyMirDt,
              trace,
              generate_dt__LilyMir(module, data_type->trace_mut));
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            Vec *tuple = NEW(Vec);

            for (Usize i = 0; i < data_type->tuple->len; ++i) {
                push__Vec(
                  tuple,
                  generate_dt__LilyMir(module, get__Vec(data_type->tuple, i)));
            }

            return NEW_VARIANT(LilyMirDt, tuple, tuple);
        }
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U16);
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U32);
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U64);
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_U8);
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_UNIT);
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return NEW(LilyMirDt, LILY_MIR_DT_KIND_USIZE);
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            UNREACHABLE("unknown data type is not expected");
        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE:
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE:
        case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
            UNREACHABLE("compiler defined data type is not expected. The "
                        "analysis has a bug!!");
        default:
            UNREACHABLE("unknown variant");
    }
}
