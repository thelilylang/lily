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

#ifndef LILY_BASE_ATOI_H
#define LILY_BASE_ATOI_H

#include <base/optional.h>
#include <base/platform.h>
#include <base/types.h>

#pragma GCC diagnostic ignored "-Warray-bounds"

// MAX: 0b1111111, 0o177, 127, 0x7F
// MIN: -0b10000000, -0o200, -128, -0x80
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: support base 2, 8, 10, 16.
// NOTE: this check not support lead by 0 will do nothing (e.g. 000000033)
#define CHECK_INT8_BASE_2_OVERFLOW_FROM_STRING(x)                        \
    ((x[0] != '-' &&                                                     \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7])) ||  \
     (x[0] == '-' &&                                                     \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8]       \
         ? 1                                                             \
         : x[1] == '1' && x[2] == '0' && x[3] == '0' && x[4] == '0' &&   \
             x[5] == '0' && x[6] == '0' && x[7] == '0' && x[8] == '0' && \
             !x[9])))

#define CHECK_INT8_BASE_8_OVERFLOW_FROM_STRING(x)                    \
    ((x[0] != '-' && (!x[1] || !x[2] ? 1 : x[0] == '1' && !x[3])) || \
     (x[0] == '-' &&                                                 \
      (!x[2] || !x[3] || (x[1] < '2' && !x[4])                       \
         ? 1                                                         \
         : x[1] == '2' && x[2] == '0' && x[3] == '0' && !x[4])))

#define CHECK_INT8_BASE_10_OVERFLOW_FROM_STRING(x)            \
    ((x[0] != '-' && (!x[1] || !x[2] ? 1                      \
                      : x[0] == '1'  ? (x[1] < '2'   ? !x[3]  \
                                        : x[1] > '2' ? 0      \
                                        : x[2] < '7' ? !x[3]  \
                                        : x[2] > '7' ? 0      \
                                                     : !x[3]) \
                                     : 0)) ||                  \
     (x[0] == '-' && (!x[2] || !x[3] ? 1                      \
                      : x[1] == '1'  ? (x[2] < '2'   ? !x[4]  \
                                        : x[2] > '2' ? 0      \
                                        : x[3] < '8' ? !x[4]  \
                                        : x[3] > '8' ? 0      \
                                                     : !x[4]) \
                                     : 0)))

#define CHECK_INT8_BASE_16_OVERFLOW_FROM_STRING(x)                    \
    ((x[0] != '-' &&                                                  \
      (!x[1] || (x[0] < '7' && !x[2]) ? 1 : x[0] == '7' && !x[2])) || \
     (x[0] == '-' &&                                                  \
      (!x[2] || (x[1] < '8' && !x[3]) ? 1                             \
                                      : x[1] <= '8' && x[2] <= '0' && !x[3])))

#define CHECK_INT8_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_INT8_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_INT8_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_INT8_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_INT8_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

// MAX: 0b111111111111111, 0o77777, 32'767, 0x7FFF
// MIN: -0b1000000000000000, -0o100000, -32'768, -0x8000
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: support base 2, 8, 10, 16.
// NOTE: this check not support lead by 0 will do nothing (e.g. 000000033)
#define CHECK_INT16_BASE_2_OVERFLOW_FROM_STRING(x)                             \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
       !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15])) ||  \
     (x[0] == '-' &&                                                           \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] || \
           !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] || !x[16]  \
         ? 1                                                                   \
         : x[1] <= '1' && x[2] == '0' && x[3] == '0' && x[4] == '0' &&         \
             x[5] == '0' && x[6] == '0' && x[7] == '0' && x[8] == '0' &&       \
             x[9] == '0' && x[10] == '0' && x[11] == '0' && x[12] == '0' &&    \
             x[13] == '0' && x[14] == '0' && x[15] == '0' && x[16] == '0' &&   \
             !x[17])))

#define CHECK_INT16_BASE_8_OVERFLOW_FROM_STRING(x)                       \
    (((x[0] != '-' &&                                                    \
       (!x[1] || !x[2] || !x[3] || !x[4] || (x[0] <= '7' && !x[5])))) || \
     (x[0] == '-' &&                                                     \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6]                \
         ? 1                                                             \
         : x[1] <= '1' && x[2] == '0' && x[3] == '0' && x[4] == '0' &&   \
             x[5] == '0' && x[6] == '0' && !x[7])))

#define CHECK_INT16_BASE_10_OVERFLOW_FROM_STRING(x)                  \
    ((x[0] != '-' &&                                                 \
      (!x[1] || !x[2] || !x[3] || !x[4] || (x[0] < '3' && !x[5]) ? 1 \
       : x[0] == '3' ? (x[1] < '2'   ? !x[5]                         \
                        : x[1] > '2' ? 0                             \
                        : x[2] < '7' ? !x[5]                         \
                        : x[2] > '7' ? 0                             \
                        : x[3] < '6' ? !x[5]                         \
                        : x[3] > '6' ? 0                             \
                        : x[4] < '7' ? !x[5]                         \
                        : x[4] > '7' ? 0                             \
                                     : !x[5])                        \
                     : 0)) ||                                        \
     (x[0] == '-' &&                                                 \
      (!x[2] || !x[3] || !x[4] || !x[5] || (x[1] < '3' && !x[6]) ? 1 \
       : x[1] == '3' ? (x[2] < '2'   ? !x[6]                         \
                        : x[2] > '2' ? 0                             \
                        : x[3] < '7' ? !x[6]                         \
                        : x[3] > '7' ? 0                             \
                        : x[4] < '6' ? !x[6]                         \
                        : x[4] > '6' ? 0                             \
                        : x[5] < '8' ? !x[6]                         \
                        : x[5] > '8' ? 0                             \
                                     : !x[6])                        \
                     : 0)))

#define CHECK_INT16_BASE_16_OVERFLOW_FROM_STRING(x)                      \
    ((x[0] != '-' && (!x[1] || !x[2] || !x[3] || (x[0] < '7' && !x[4])   \
                        ? 1                                              \
                        : x[0] <= '7' && (x[1] <= 'F' || x[1] <= 'f') && \
                            (x[2] <= 'F' || x[2] <= 'f') &&              \
                            (x[3] <= 'F' || x[3] <= 'f') && !x[4])) ||   \
     (x[0] == '-' && (!x[2] || !x[3] || !x[4] || (x[1] < '8' && !x[5])   \
                        ? 1                                              \
                        : x[1] <= '8' && x[2] <= '0' && x[3] <= '0' &&   \
                            x[4] <= '0' && !x[5])))

#define CHECK_INT16_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_INT16_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_INT16_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_INT16_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_INT16_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

// MAX: 0b1111111111111111111111111111111, 0o17777777777, 2'147'483'647,
// 0x7FFFFFFF
// MIN: -0b10000000000000000000000000000000, -0o20000000000,
// –2'147'483'648, -0x80000000
// NOTE: return 0 if is overflow/underflow otherwise
// return 1. NOTE: support base 2, 8, 10, 16. NOTE: this check not support lead
// by 0 will do nothing (e.g. 000000033)
#define CHECK_INT32_BASE_2_OVERFLOW_FROM_STRING(x)                             \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
       !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||    \
       !x[16] || !x[17] || !x[18] || !x[19] || !x[20] || !x[21] || !x[22] ||   \
       !x[23] || !x[24] || !x[25] || !x[26] || !x[27] || !x[28] || !x[29] ||   \
       !x[30] || !x[31])) ||                                                   \
     (x[0] == '-' &&                                                           \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] || \
           !x[10] || !x[11] | !x[12] || !x[13] || !x[14] || !x[15] ||          \
           !x[16] || !x[17] || !x[18] || !x[19] || !x[20] || !x[21] ||         \
           !x[22] || !x[23] || !x[24] || !x[25] || !x[26] || !x[27] ||         \
           !x[28] || !x[29] || !x[30] || !x[31] || !x[32]                      \
         ? 1                                                                   \
         : x[1] <= '1' && x[2] == '0' && x[3] == '0' && x[4] == '0' &&         \
             x[5] == '0' && x[6] == '0' && x[7] == '0' && x[8] == '0' &&       \
             x[9] == '0' && x[10] == '0' && x[11] == '0' && x[12] == '0' &&    \
             x[13] == '0' && x[14] == '0' && x[15] == '0' && x[16] == '0' &&   \
             x[17] == '0' && x[18] == '0' && x[19] == '0' && x[20] == '0' &&   \
             x[21] == '0' && x[22] == '0' && x[23] == '0' && x[24] == '0' &&   \
             x[25] == '0' && x[26] == '0' && x[27] == '0' && x[28] == '0' &&   \
             x[29] == '0' && x[30] == '0' && x[31] == '0' && x[32] == '0' &&   \
             !x[33])))

#define CHECK_INT32_BASE_8_OVERFLOW_FROM_STRING(x)                             \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
       !x[9] || !x[10] || (x[0] == '1' && !x[11]))) ||                         \
     (x[0] == '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
           !x[9] || !x[10] || !x[11] || !x[12]                                 \
         ? 1                                                                   \
         : x[1] <= '2' && x[2] == '0' && x[3] == '0' && x[4] == '0' &&         \
             x[5] == '0' && x[6] == '0' && x[7] == '0' && x[8] == '0' &&       \
             x[9] == '0' && x[10] == '0' && x[11] == '0' && !x[12])))

#define CHECK_INT32_BASE_10_OVERFLOW_FROM_STRING(x)                          \
    ((x[0] != '-' && (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] ||  \
                          !x[7] || !x[8] || !x[9] || (x[0] < '2' && !x[10])  \
                        ? 1                                                  \
                      : x[0] == '2' ? (x[1] < '1'   ? !x[10]                 \
                                       : x[1] > '1' ? 0                      \
                                       : x[2] < '4' ? !x[10]                 \
                                       : x[2] > '4' ? 0                      \
                                       : x[3] < '7' ? !x[10]                 \
                                       : x[3] > '7' ? 0                      \
                                       : x[4] < '4' ? !x[10]                 \
                                       : x[4] > '4' ? 0                      \
                                       : x[5] < '8' ? !x[10]                 \
                                       : x[5] > '8' ? 0                      \
                                       : x[6] < '3' ? !x[10]                 \
                                       : x[6] > '3' ? 0                      \
                                       : x[7] < '6' ? !x[10]                 \
                                       : x[7] > '6' ? 0                      \
                                       : x[8] < '4' ? !x[10]                 \
                                       : x[8] > '4' ? 0                      \
                                       : x[9] < '7' ? !x[10]                 \
                                       : x[9] > '7' ? 0                      \
                                                    : !x[10])                \
                                    : 0)) ||                                 \
     (x[0] == '-' && (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] ||  \
                          !x[8] || !x[9] || !x[10] || (x[1] < '2' && !x[11]) \
                        ? 1                                                  \
                      : x[1] == '2' ? (x[2] < '1'    ? !x[11]                \
                                       : x[2] > '1'  ? 0                     \
                                       : x[3] < '4'  ? !x[11]                \
                                       : x[3] > '4'  ? 0                     \
                                       : x[4] < '7'  ? !x[11]                \
                                       : x[4] > '7'  ? 0                     \
                                       : x[5] < '4'  ? !x[11]                \
                                       : x[5] > '4'  ? 0                     \
                                       : x[6] < '8'  ? !x[11]                \
                                       : x[6] > '8'  ? 0                     \
                                       : x[7] < '3'  ? !x[11]                \
                                       : x[7] > '3'  ? 0                     \
                                       : x[8] < '6'  ? !x[11]                \
                                       : x[8] > '6'  ? 0                     \
                                       : x[9] < '4'  ? !x[11]                \
                                       : x[9] > '4'  ? 0                     \
                                       : x[10] < '7' ? !x[11]                \
                                       : x[10] > '7' ? 0                     \
                                                     : !x[11])               \
                                    : 0)))

#define CHECK_INT32_BASE_16_OVERFLOW_FROM_STRING(x)                            \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
           (x[0] < '7' && !x[8])                                               \
         ? 1                                                                   \
         : x[0] <= '7' && (x[1] <= 'F' || x[1] <= 'f') &&                      \
             (x[2] <= 'F' || x[2] <= 'f') && (x[3] <= 'F' || x[3] <= 'f') &&   \
             (x[4] <= 'F' || x[4] <= 'f') && (x[5] <= 'F' || x[5] <= 'f') &&   \
             (x[6] <= 'F' || x[6] <= 'f') && (x[7] <= 'F' || x[7] <= 'f') &&   \
             !x[8])) ||                                                        \
     (x[0] == '-' && (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] ||    \
                          !x[8] || !x[9] || (x[1] < '8' && !x[9])              \
                        ? 1                                                    \
                        : x[1] <= '8' && x[2] <= '0' && x[3] <= '0' &&         \
                            x[4] <= '0' && x[5] <= '0' && x[6] <= '0' &&       \
                            x[7] <= '0' && x[8] <= '0' && !x[9])))

#define CHECK_INT32_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_INT32_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_INT32_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_INT32_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_INT32_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

// MAX: 0b111111111111111111111111111111111111111111111111111111111111111,
// 0o777777777777777777777, 9'223'372'036'854'775'807, 0x7FFFFFFFFFFFFFFF
// MIN: -0b1000000000000000000000000000000000000000000000000000000000000000,
// -0o1000000000000000000000,-9'223'372'036'854'775'808, -0x8000000000000000
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: support base 2, 8, 10, 16.
// NOTE: this check not support lead by 0 will do nothing (e.g. 000000033)
#define CHECK_INT64_BASE_2_OVERFLOW_FROM_STRING(x)                             \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
       !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||    \
       !x[16] || !x[17] || !x[18] || !x[19] || !x[20] || !x[21] || !x[22] ||   \
       !x[23] || !x[24] || !x[25] || !x[26] || !x[27] || !x[28] || !x[29] ||   \
       !x[30] || !x[31] || !x[32] || !x[33] || !x[34] || !x[35] || !x[36] ||   \
       !x[37] || !x[38] || !x[39] || !x[40] || !x[41] || !x[42] || !x[43] ||   \
       !x[44] || !x[45] || !x[46] || !x[47] || !x[48] || !x[49] || !x[50] ||   \
       !x[51] || !x[52] || !x[53] || !x[54] || !x[55] || !x[56] || !x[57] ||   \
       !x[58] || !x[59] || !x[60] || !x[61] || !x[62] || !x[63])) ||           \
     (x[0] == '-' &&                                                           \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] || \
           !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||         \
           !x[16] || !x[17] || !x[18] || !x[19] || !x[20] || !x[21] ||         \
           !x[22] || !x[23] || !x[24] || !x[25] || !x[26] || !x[27] ||         \
           !x[28] || !x[29] || !x[30] || !x[31] || !x[32] || !x[33] ||         \
           !x[34] || !x[35] || !x[36] || !x[37] || !x[38] || !x[39] ||         \
           !x[40] || !x[41] || !x[42] || !x[43] || !x[44] || !x[45] ||         \
           !x[46] || !x[47] || !x[48] || !x[49] || !x[50] || !x[51] ||         \
           !x[52] || !x[53] || !x[54] || !x[55] || !x[56] || !x[57] ||         \
           !x[58] || !x[59] || !x[60] || !x[61] || !x[62] || !x[63] || !x[64]  \
         ? 1                                                                   \
         : x[1] <= '1' && x[2] <= '0' && x[3] <= '0' && x[4] <= '0' &&         \
             x[5] <= '0' && x[6] <= '0' && x[7] <= '0' && x[8] <= '0' &&       \
             x[9] <= '0' && x[10] <= '0' && x[11] <= '0' && x[12] <= '0' &&    \
             x[13] <= '0' && x[14] <= '0' && x[15] <= '0' && x[16] <= '0' &&   \
             x[17] <= '0' && x[18] <= '0' && x[19] <= '0' && x[20] <= '0' &&   \
             x[21] <= '0' && x[22] <= '0' && x[23] <= '0' && x[24] <= '0' &&   \
             x[25] <= '0' && x[26] <= '0' && x[27] <= '0' && x[28] <= '0' &&   \
             x[29] <= '0' && x[30] <= '0' && x[31] <= '0' && x[32] <= '0' &&   \
             x[33] <= '0' && x[34] <= '0' && x[35] <= '0' && x[36] <= '0' &&   \
             x[37] <= '0' && x[38] <= '0' && x[39] <= '0' && x[40] <= '0' &&   \
             x[41] <= '0' && x[42] <= '0' && x[43] <= '0' && x[44] <= '0' &&   \
             x[45] <= '0' && x[46] <= '0' && x[47] <= '0' && x[48] <= '0' &&   \
             x[49] <= '0' && x[50] <= '0' && x[51] <= '0' && x[52] <= '0' &&   \
             x[53] <= '0' && x[54] <= '0' && x[55] <= '0' && x[56] <= '0' &&   \
             x[57] <= '0' && x[58] <= '0' && x[59] <= '0' && x[60] <= '0' &&   \
             x[61] <= '0' && x[62] <= '0' && x[63] <= '0' && x[64] <= '0' &&   \
             !x[65])))

#define CHECK_INT64_BASE_8_OVERFLOW_FROM_STRING(x)                             \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
       !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||    \
       !x[16] || !x[17] || !x[18] || !x[19] || !x[20] ||                       \
       (x[0] == '1' && !x[21]))) ||                                            \
     (x[0] == '-' &&                                                           \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] || \
           !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||         \
           !x[16] || !x[17] || !x[18] || !x[19] || !x[20] || !x[21] ||         \
           !x[22] || (x[1] < '2' && !x[23])                                    \
         ? 1                                                                   \
         : x[1] == '2' && x[2] == '0' && x[3] == '0' && x[4] == '0' &&         \
             x[5] == '0' && x[6] == '0' && x[7] == '0' && x[8] == '0' &&       \
             x[9] == '0' && x[10] == '0' && x[11] == '0' && x[12] == '0' &&    \
             x[13] == '0' && x[14] == '0' && x[15] == '0' && x[16] == '0' &&   \
             x[17] == '0' && x[18] == '0' && x[19] == '0' && x[20] == '0' &&   \
             x[21] == '0' && x[22] == '0' && !x[23])))

#define CHECK_INT64_BASE_10_OVERFLOW_FROM_STRING(x)                            \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
           !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] ||          \
           !x[15] || !x[16] || !x[17] || !x[18] || (x[0] < '9' && !x[19])      \
         ? 1                                                                   \
       : x[0] == '9' ? (x[1] < '2'    ? !x[19]                                 \
                        : x[1] > '2'  ? 0                                      \
                        : x[2] < '2'  ? !x[19]                                 \
                        : x[2] > '2'  ? 0                                      \
                        : x[3] < '3'  ? !x[19]                                 \
                        : x[3] > '3'  ? 0                                      \
                        : x[4] < '3'  ? !x[19]                                 \
                        : x[4] > '3'  ? 0                                      \
                        : x[5] < '7'  ? !x[19]                                 \
                        : x[5] > '7'  ? 0                                      \
                        : x[6] < '2'  ? !x[19]                                 \
                        : x[6] > '2'  ? 0                                      \
                        : x[7] < '0'  ? !x[19]                                 \
                        : x[7] > '0'  ? 0                                      \
                        : x[8] < '3'  ? !x[19]                                 \
                        : x[8] > '3'  ? 0                                      \
                        : x[9] < '6'  ? !x[19]                                 \
                        : x[9] > '6'  ? 0                                      \
                        : x[10] < '8' ? !x[19]                                 \
                        : x[10] > '8' ? 0                                      \
                        : x[11] < '5' ? !x[19]                                 \
                        : x[11] > '5' ? 0                                      \
                        : x[12] < '4' ? !x[19]                                 \
                        : x[12] > '4' ? 0                                      \
                        : x[13] < '7' ? !x[19]                                 \
                        : x[13] > '7' ? 0                                      \
                        : x[14] < '7' ? !x[19]                                 \
                        : x[14] > '7' ? 0                                      \
                        : x[15] < '5' ? !x[19]                                 \
                        : x[15] > '5' ? 0                                      \
                        : x[16] < '8' ? !x[19]                                 \
                        : x[16] > '8' ? !x[19]                                 \
                        : x[17] > '0' ? 0                                      \
                        : x[18] < '7' ? !x[19]                                 \
                        : x[18] > '7' ? 0                                      \
                                      : !x[19])                                \
                     : 0)) ||                                                  \
     (x[0] == '-' &&                                                           \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] || \
           !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||         \
           !x[16] || !x[17] || !x[18] || !x[19] || (x[1] < '9' && !x[20])      \
         ? 1                                                                   \
       : x[1] == '9' ? (x[2] < '2'    ? !x[20]                                 \
                        : x[2] > '2'  ? 0                                      \
                        : x[3] < '2'  ? !x[20]                                 \
                        : x[3] > '2'  ? 0                                      \
                        : x[4] < '3'  ? !x[20]                                 \
                        : x[4] > '3'  ? 0                                      \
                        : x[5] < '3'  ? !x[20]                                 \
                        : x[5] > '3'  ? 0                                      \
                        : x[6] < '7'  ? !x[20]                                 \
                        : x[6] > '7'  ? 0                                      \
                        : x[7] < '2'  ? !x[20]                                 \
                        : x[7] > '2'  ? 0                                      \
                        : x[8] < '0'  ? !x[20]                                 \
                        : x[8] > '0'  ? 0                                      \
                        : x[9] < '3'  ? !x[20]                                 \
                        : x[9] > '3'  ? 0                                      \
                        : x[10] < '6' ? !x[20]                                 \
                        : x[10] > '6' ? 0                                      \
                        : x[11] < '8' ? !x[20]                                 \
                        : x[11] > '8' ? 0                                      \
                        : x[12] < '5' ? !x[20]                                 \
                        : x[12] > '5' ? 0                                      \
                        : x[13] < '4' ? !x[20]                                 \
                        : x[13] > '4' ? 0                                      \
                        : x[14] < '7' ? !x[20]                                 \
                        : x[14] > '7' ? 0                                      \
                        : x[15] < '7' ? !x[20]                                 \
                        : x[15] > '7' ? 0                                      \
                        : x[16] < '5' ? !x[20]                                 \
                        : x[16] > '5' ? 0                                      \
                        : x[17] < '8' ? !x[20]                                 \
                        : x[17] > '8' ? !x[20]                                 \
                        : x[18] > '0' ? 0                                      \
                        : x[19] < '7' ? !x[20]                                 \
                        : x[19] > '7' ? 0                                      \
                                      : !x[20])                                \
                     : 0)))

#define CHECK_INT64_BASE_16_OVERFLOW_FROM_STRING(x)                            \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
           !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] ||          \
           !x[15] || (x[0] < '7' && !x[13])                                    \
         ? 1                                                                   \
         : x[0] == '7' && (x[1] <= 'F' || x[1] <= 'f') &&                      \
             (x[2] <= 'F' || x[2] <= 'f') && (x[3] <= 'F' || x[3] <= 'f') &&   \
             (x[4] <= 'F' || x[4] <= 'f') && (x[5] <= 'F' || x[5] <= 'f') &&   \
             (x[6] <= 'F' || x[6] <= 'f') && (x[7] <= 'F' || x[7] <= 'f') &&   \
             (x[8] <= 'F' || x[8] <= 'f') && (x[9] <= 'F' || x[9] <= 'f') &&   \
             (x[10] <= 'F' || x[10] <= 'f') &&                                 \
             (x[11] <= 'F' || x[11] <= 'f') &&                                 \
             (x[12] <= 'F' || x[12] <= 'f') && !x[13])) ||                     \
     (x[0] == '-' &&                                                           \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] || \
           !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||         \
           !x[16] || !x[17] || (x[1] < '8' && !x[17])                          \
         ? 1                                                                   \
         : x[1] == '8' && x[2] == '0' && x[3] == '0' && x[4] == '0' &&         \
             x[5] == '0' && x[6] == '0' && x[7] == '0' && x[8] == '0' &&       \
             x[9] == '0' && x[10] == '0' && x[11] == '0' && x[12] == '0' &&    \
             x[13] == '0' && x[14] == '0' && x[15] == '0' && x[16] == '0' &&   \
             !x[17])))

#define CHECK_INT64_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_INT64_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_INT64_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_INT64_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_INT64_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

// MAX: 0b11111111, 0o377, 255, 0xFF
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: support base 2, 8, 10, 16.
// NOTE: this check not support lead by 0 will do nothing (e.g. 000000033)
#define CHECK_UINT8_BASE_2_OVERFLOW_FROM_STRING(x) \
    ((x[0] != '-') &&                              \
     (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8]))

#define CHECK_UINT8_BASE_8_OVERFLOW_FROM_STRING(x) \
    ((x[0] != '-') &&                              \
     (!x[1] || !x[2] || (x[0] < '3' && !x[3])      \
        ? 1                                        \
        : x[0] == '3' && x[1] <= '7' && x[2] <= '7' && !x[3]))

#define CHECK_UINT8_BASE_10_OVERFLOW_FROM_STRING(x)                \
    ((x[0] != '-') && (!x[1] || !x[2] || (x[0] < '2' && !x[3]) ? 1 \
                       : x[0] == '2' ? (x[1] < '5'   ? !x[3]       \
                                        : x[1] > '5' ? 0           \
                                        : x[2] < '5' ? !x[3]       \
                                        : x[2] > '5' ? 0           \
                                                     : !x[3])      \
                                     : 0))

#define CHECK_UINT8_BASE_16_OVERFLOW_FROM_STRING(x) \
    ((x[0] != '-') && (!x[1] || !x[2]))

#define CHECK_UINT8_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_UINT8_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_UINT8_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_UINT8_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_UINT8_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

// MAX: 0b1111111111111111, 0o177777, 65'535, 0xFFFF
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: support base 2, 8, 10, 16.
// NOTE: this check not support lead by 0 will do nothing (e.g. 000000033)
#define CHECK_UINT16_BASE_2_OVERFLOW_FROM_STRING(x)                          \
    ((x[0] != '-') && (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || \
                       !x[7] || !x[8] || !x[9] || !x[10] || !x[11] ||        \
                       !x[12] || !x[13] || !x[14] || !x[15] || !x[16]))

#define CHECK_UINT16_BASE_8_OVERFLOW_FROM_STRING(x) \
    ((x[0] != '-') &&                               \
     (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || (x[0] == '1' && !x[6])))

#define CHECK_UINT16_BASE_10_OVERFLOW_FROM_STRING(x)                \
    ((x[0] != '-') &&                                               \
     (!x[1] || !x[2] || !x[3] || !x[4] || (x[0] < '6' && !x[5]) ? 1 \
      : x[0] == '6' ? (x[1] < '5'   ? !x[5]                         \
                       : x[1] > '5' ? 0                             \
                       : x[2] < '5' ? !x[5]                         \
                       : x[2] > '5' ? 0                             \
                       : x[3] < '3' ? !x[5]                         \
                       : x[3] > '3' ? 0                             \
                       : x[4] < '5' ? !x[5]                         \
                       : x[4] > '5' ? 0                             \
                                    : !x[5])                        \
                    : 0))

#define CHECK_UINT16_BASE_16_OVERFLOW_FROM_STRING(x) \
    ((x[0] != '-') && (!x[1] || !x[2] || !x[3] || !x[4]))

#define CHECK_UINT16_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_UINT16_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_UINT16_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_UINT16_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_UINT16_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

// MAX: 0b11111111111111111111111111111111, 0o37777777777, 4'294'967'295,
// 0xFFFFFFFF
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: support base 2, 8, 10, 16.
// NOTE: this check not support lead by 0 will do nothing (e.g. 000000033)
#define CHECK_UINT32_BASE_2_OVERFLOW_FROM_STRING(x)                           \
    ((x[0] != '-') &&                                                         \
     (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
      !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||    \
      !x[16] || !x[17] || !x[18] || !x[19] || !x[20] || !x[21] || !x[22] ||   \
      !x[23] || !x[24] || !x[25] || !x[26] || !x[27] || !x[28] || !x[29] ||   \
      !x[30] || !x[31] || !x[32]))

#define CHECK_UINT32_BASE_8_OVERFLOW_FROM_STRING(x)                           \
    ((x[0] != '-') &&                                                         \
     (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
      !x[9] || !x[10] || (x[0] <= '3' && !x[11])))

#define CHECK_UINT32_BASE_10_OVERFLOW_FROM_STRING(x)                          \
    ((x[0] != '-') &&                                                         \
     (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
          !x[9] || !x[10] || (x[0] < '4' && !x[11])                           \
        ? 1                                                                   \
      : x[0] == '4' ? (x[1] < '2'    ? !x[11]                                 \
                       : x[1] > '2'  ? 0                                      \
                       : x[2] < '9'  ? !x[11]                                 \
                       : x[2] > '9'  ? 0                                      \
                       : x[3] < '4'  ? !x[11]                                 \
                       : x[3] > '4'  ? 0                                      \
                       : x[4] < '9'  ? !x[11]                                 \
                       : x[4] > '9'  ? 0                                      \
                       : x[5] < '6'  ? !x[11]                                 \
                       : x[6] > '6'  ? 0                                      \
                       : x[7] < '7'  ? !x[11]                                 \
                       : x[7] > '7'  ? 0                                      \
                       : x[8] < '2'  ? !x[11]                                 \
                       : x[8] > '2'  ? 0                                      \
                       : x[9] < '9'  ? !x[11]                                 \
                       : x[9] > '9'  ? 0                                      \
                       : x[10] < '5' ? !x[11]                                 \
                       : x[10] > '5' ? 0                                      \
                                     : !x[11])                                \
                    : 0))

#define CHECK_UINT32_BASE_16_OVERFLOW_FROM_STRING(x) \
    ((x[0] != '-') &&                                \
     (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8]))

#define CHECK_UINT32_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_UINT32_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_UINT32_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_UINT32_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_UINT32_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

// MAX: 0b1111111111111111111111111111111111111111111111111111111111111111,
// 0o1777777777777777777777, 18'446'744'073'709'551'615, 0xFFFFFFFFFFFFFFFF
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
// NOTE: support base 2, 8, 10, 16.
// NOTE: this check not support lead by 0 will do nothing
// (e.g. 000000033)
#define CHECK_UINT64_BASE_2_OVERFLOW_FROM_STRING(x)                           \
    ((x[0] != '-') &&                                                         \
     (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
      !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||    \
      !x[16] || !x[17] || !x[18] || !x[19] || !x[20] || !x[21] || !x[22] ||   \
      !x[23] || !x[24] || !x[25] || !x[26] || !x[27] || !x[28] || !x[29] ||   \
      !x[30] || !x[31] || !x[32] || !x[33] || !x[34] || !x[35] || !x[36] ||   \
      !x[37] || !x[38] || !x[39] || !x[40] || !x[41] || !x[42] || !x[43] ||   \
      !x[44] || !x[45] || !x[46] || !x[47] || !x[48] || !x[49] || !x[50] ||   \
      !x[51] || !x[52] || !x[53] || !x[54] || !x[55] || !x[56] || !x[57] ||   \
      !x[58] || !x[59] || !x[60] || !x[61] || !x[62] || !x[63] || !x[64]))

#define CHECK_UINT64_BASE_8_OVERFLOW_FROM_STRING(x)                            \
    ((x[0] != '-' && (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] ||    \
                      !x[7] || !x[8] || !x[9] || !x[10] || !x[11] || !x[12] || \
                      !x[13] || !x[14] || !x[15] || !x[16] || !x[17] ||        \
                      !x[18] || !x[19] || !x[20] || (x[0] == '1' && !x[21]))))

#define CHECK_UINT64_BASE_10_OVERFLOW_FROM_STRING(x)                           \
    ((x[0] != '-' &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
           !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] ||          \
           !x[15] || !x[16] || !x[17] || !x[18]                                \
         ? 1                                                                   \
       : x[0] == '1' ? (x[1] < '8'    ? !x[19]                                 \
                        : x[1] > '8'  ? 0                                      \
                        : x[2] < '4'  ? !x[19]                                 \
                        : x[2] > '4'  ? 0                                      \
                        : x[3] < '4'  ? !x[19]                                 \
                        : x[3] > '4'  ? 0                                      \
                        : x[4] < '6'  ? !x[19]                                 \
                        : x[4] > '6'  ? 0                                      \
                        : x[5] < '7'  ? !x[19]                                 \
                        : x[5] > '7'  ? 0                                      \
                        : x[6] < '4'  ? !x[19]                                 \
                        : x[6] > '4'  ? 0                                      \
                        : x[7] > '0'  ? 0                                      \
                        : x[8] < '7'  ? !x[19]                                 \
                        : x[8] > '7'  ? 0                                      \
                        : x[9] < '3'  ? !x[19]                                 \
                        : x[9] > '3'  ? 0                                      \
                        : x[10] < '7' ? !x[19]                                 \
                        : x[10] > '7' ? 0                                      \
                        : x[11] < '0' ? !x[19]                                 \
                        : x[11] > '0' ? 0                                      \
                        : x[12] < '9' ? !x[19]                                 \
                        : x[13] < '5' ? !x[19]                                 \
                        : x[13] > '5' ? 0                                      \
                        : x[14] < '5' ? !x[19]                                 \
                        : x[14] > '5' ? 0                                      \
                        : x[15] < '1' ? !x[19]                                 \
                        : x[15] > '1' ? 0                                      \
                        : x[16] < '6' ? !x[19]                                 \
                        : x[16] > '6' ? !x[19]                                 \
                        : x[17] > '1' ? 0                                      \
                        : x[17] < '1' ? !x[19]                                 \
                        : x[18] < '5' ? !x[19]                                 \
                        : x[18] > '5' ? 0                                      \
                                      : !x[19])                                \
                     : 0)))

#define CHECK_UINT64_BASE_16_OVERFLOW_FROM_STRING(x)                         \
    ((x[0] != '-') && (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || \
                       !x[7] || !x[8] || !x[9] || !x[10] || !x[11] ||        \
                       !x[12] || !x[13] || !x[14] || !x[15] || !x[16]))

#define CHECK_UINT64_OVERFLOW_FROM_STRING(x, base)               \
    (base == 2    ? CHECK_UINT64_BASE_2_OVERFLOW_FROM_STRING(x)  \
     : base == 8  ? CHECK_UINT64_BASE_8_OVERFLOW_FROM_STRING(x)  \
     : base == 10 ? CHECK_UINT64_BASE_10_OVERFLOW_FROM_STRING(x) \
     : base == 16 ? CHECK_UINT64_BASE_16_OVERFLOW_FROM_STRING(x) \
                  : 0)

#if defined(PLATFORM_64)
#define CHECK_ISIZE_OVERFLOW_FROM_STRING(x, base) \
    CHECK_INT64_OVERFLOW_FROM_STRING(x, base)
#define CHECK_USIZE_OVERFLOW_FROM_STRING(x, base) \
    CHECK_UINT64_OVERFLOW_FROM_STRING(x, base)
#else
#define CHECK_ISIZE_OVERFLOW_FROM_STRING(x, base) \
    CHECK_INT32_OVERFLOW_FROM_STRING(x, base)
#define CHECK_USIZE_OVERFLOW_FROM_STRING(x, base) \
    CHECK_UINT32_OVERFLOW_FROM_STRING(x, base)
#endif

/**
 *
 * @brief Convert a string to Int8.
 */
Int8
atoi__Int8(const char *s, int base);

/**
 *
 * @brief Convert a string to Int16.
 */
Int16
atoi__Int16(const char *s, int base);

/**
 *
 * @brief Convert a string to Int32.
 */
Int32
atoi__Int32(const char *s, int base);

/**
 *
 * @brief convert a string to Int64.
 */
Int64
atoi__Int64(const char *s, int base);

/**
 *
 * @brief Convert a string to Uint8.
 */
Uint8
atoi__Uint8(const char *s, int base);

/**
 *
 * @brief Convert a string to Uint16.
 */
Uint16
atoi__Uint16(const char *s, int base);

/**
 *
 * @brief convert a string to Uint32.
 */
Uint32
atoi__Uint32(const char *s, int base);

/**
 *
 * @brief convert a string to Uint64.
 */
Uint64
atoi__Uint64(const char *s, int base);

/**
 *
 * @brief convert a string to Isize.
 */
Isize
atoi__Isize(const char *s, int base);

/**
 *
 * @brief convert a string to Usize.
 */
Usize
atoi__Usize(const char *s, int base);

/**
 *
 * @brief Convert a string to Int8.
 * @return Optional<Int8*>*
 */
Optional *
atoi_safe__Int8(const char *s, int base);

/**
 *
 * @brief Convert a string to Int16.
 * @return Optional<Int16*>*
 */
Optional *
atoi_safe__Int16(const char *s, int base);

/**
 *
 * @brief Convert a string to Int32.
 * @return Optional<Int32*>*
 */
Optional *
atoi_safe__Int32(const char *s, int base);

/**
 *
 * @brief convert a string to Int64.
 * @return Optional<Int64*>*
 */
Optional *
atoi_safe__Int64(const char *s, int base);

/**
 *
 * @brief Convert a string to Uint8.
 * @return Optional<Uint8*>*
 */
Optional *
atoi_safe__Uint8(const char *s, int base);

/**
 *
 * @brief Convert a string to Uint16.
 * @return Optional<Uint16*>*
 */
Optional *
atoi_safe__Uint16(const char *s, int base);

/**
 *
 * @brief convert a string to Uint32.
 * @return Optional<Uint32*>*
 */
Optional *
atoi_safe__Uint32(const char *s, int base);

/**
 *
 * @brief convert a string to Uint64.
 * @return Optional<Uint64*>*
 */
Optional *
atoi_safe__Uint64(const char *s, int base);

/**
 *
 * @brief convert a string to Isize.
 * @return Optional<Isize*>*
 */
Optional *
atoi_safe__Isize(const char *s, int base);

/**
 *
 * @brief convert a string to Usize.
 */
Optional *
atoi_safe__Usize(const char *s, int base);

#endif // LILY_BASE_ATOI_H