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

#ifndef LILY_CORE_LILY_MIR_GENERATOR_LITERAL_H
#define LILY_CORE_LILY_MIR_GENERATOR_LITERAL_H

#define GENERATE_LITERAL(v, name_u, in_return)                             \
    switch (v->literal.kind) {                                             \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_BOOL:                    \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               int,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.bool_);                          \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_BYTE:                    \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               int,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.byte);                           \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_BYTES:                   \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               bytes,                                      \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.bytes);                          \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_CHAR:                    \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.char_);                          \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_CSTR:                    \
            return NEW_VARIANT(                                            \
              LilyMirInstructionVal,                                       \
              cstr,                                                        \
              NEW_VARIANT(                                                 \
                LilyMirDt,                                                 \
                cstr,                                                      \
                NEW_VARIANT(LilyMirDtLen, def, strlen(v->literal.cstr))),  \
              v->literal.cstr);                                            \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_FLOAT32:                 \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               float,                                      \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.float32);                        \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_FLOAT64:                 \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               float,                                      \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.float64);                        \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_INT32: {                 \
            LilyMirDt *dt = generate_dt__LilyMir(module, v->data_type);    \
                                                                           \
            if (is_signed__LilyMirDt(dt)) {                                \
                return NEW_VARIANT(                                        \
                  LilyMirInstructionVal, int, dt, v->literal.int32);       \
            }                                                              \
                                                                           \
            return NEW_VARIANT(                                            \
              LilyMirInstructionVal, uint, dt, v->literal.int32);          \
        }                                                                  \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_INT64: {                 \
            LilyMirDt *dt = generate_dt__LilyMir(module, v->data_type);    \
                                                                           \
            if (is_signed__LilyMirDt(dt)) {                                \
                return NEW_VARIANT(                                        \
                  LilyMirInstructionVal, int, dt, v->literal.int64);       \
            }                                                              \
                                                                           \
            return NEW_VARIANT(                                            \
              LilyMirInstructionVal, uint, dt, v->literal.int64);          \
        }                                                                  \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_NIL:                     \
            return NEW(LilyMirInstructionVal,                              \
                       LILY_MIR_INSTRUCTION_VAL_KIND_NIL,                  \
                       generate_dt__LilyMir(module, v->data_type));        \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_NONE:                    \
            return NEW(LilyMirInstructionVal,                              \
                       LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF,                \
                       generate_dt__LilyMir(module, v->data_type));        \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_STR:                     \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               str,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.str);                            \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_FLOAT32:          \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               float,                                      \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_float32);                 \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_FLOAT64:          \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               float,                                      \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_float64);                 \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_INT8:             \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               int,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_int8);                    \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_INT16:            \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               int,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_int16);                   \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_INT32:            \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               int,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_int32);                   \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_INT64:            \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               int,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_int64);                   \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_ISIZE:            \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               int,                                        \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_isize);                   \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_UINT8:            \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_uint8);                   \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_UINT16:           \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_uint16);                  \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_UINT32:           \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_uint32);                  \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_UINT64:           \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_uint64);                  \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_SUFFIX_USIZE:            \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.suffix_usize);                   \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_UINT32:                  \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.uint32);                         \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_UINT64:                  \
            return NEW_VARIANT(LilyMirInstructionVal,                      \
                               uint,                                       \
                               generate_dt__LilyMir(module, v->data_type), \
                               v->literal.uint64);                         \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_UNDEF:                   \
            return NEW(LilyMirInstructionVal,                              \
                       LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF,                \
                       generate_dt__LilyMir(module, v->data_type));        \
        case LILY_CHECKED_##name_u##_LITERAL_KIND_UNIT:                    \
            if (in_return) {                                               \
                return NEW(LilyMirInstructionVal,                          \
                           LILY_MIR_INSTRUCTION_VAL_KIND_UNIT,             \
                           generate_dt__LilyMir(module, v->data_type));    \
            } else {                                                       \
                return NULL;                                               \
            }                                                              \
        default:                                                           \
            UNREACHABLE("unknown variant");                                \
    }

#endif // LILY_CORE_LILY_MIR_GENERATOR_LITERAL_H
