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

#ifndef LILY_BASE_ATOI_H
#define LILY_BASE_ATOI_H

#include <base/platform.h>
#include <base/types.h>

// MAX: 127
// MIN: -128
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_INT8_OVERFLOW_FROM_STRING(x)                                   \
    (!x[1] || !x[2] ? 1                                                      \
                    : x[0] <= '1' && x[1] <= '2' && x[2] <= '7' && !x[3]) || \
      (x[2] == '\0'                                                          \
         ? 1                                                                 \
         : x[0] == '-' && x[1] <= '1' && x[2] <= '2' && x[3] <= '8' && !x[4])

// MAX: 32'767
// MIN: -32'768
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_INT16_OVERFLOW_FROM_STRING(x)                            \
    (!x[1] || !x[2] || x[3] == '\0' || x[4] == '\0'                    \
       ? 1                                                             \
       : x[0] <= '3' && x[1] <= '2' && x[2] <= '7' && x[3] <= '6' &&   \
           x[4] <= '7' && !x[5]) ||                                    \
      (!x[2] || !x[3] || !x[4] || !x[5]                                \
         ? 1                                                           \
         : x[0] == '-' && x[1] <= '3' && x[2] <= '2' && x[3] <= '7' && \
             x[4] <= '6' && x[5] <= '8' && !x[6])

// MAX: 2'147'483'647
// MIN: –2'147'483'648
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_INT32_OVERFLOW_FROM_STRING(x)                                    \
    (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] ||   \
         !x[9]                                                                 \
       ? 1                                                                     \
       : x[0] <= '2' && x[1] <= '1' && x[2] <= '4' && x[3] <= '7' &&           \
           x[4] <= '4' && x[5] <= '8' && x[6] <= '3' && x[7] <= '6' &&         \
           x[8] <= '4' && x[9] <= '7' && !x[10]) ||                            \
      (!x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] || \
           !x[10]                                                              \
         ? 1                                                                   \
         : x[0] == '-' && x[1] <= '2' && x[2] <= '1' && x[3] <= '4' &&         \
             x[4] <= '7' && x[5] <= '4' && x[6] <= '8' && x[7] <= '3' &&       \
             x[8] <= '6' && x[9] <= '4' && x[10] <= '7' && !x[11])

// MAX: 9'223'372'036'854'775'807
// MIN: -9'223'372'036'854'775'808
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_INT64_OVERFLOW_FROM_STRING(x)                                   \
    (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] ||  \
         !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] || \
         !x[16] || !x[17] || !x[18]                                           \
       ? 1                                                                    \
       : x[0] <= '9' && x[1] <= '2' && x[2] <= '2' && x[3] <= '3' &&          \
           x[4] <= '3' && x[5] <= '7' && x[6] <= '2' && x[7] <= '0' &&        \
           x[8] <= '3' && x[9] <= '6' && x[10] <= '8' && x[11] <= '5' &&      \
           x[12] <= '4' && x[13] <= '7' && x[14] <= '7' && x[15] <= '5' &&    \
           x[16] <= '8' && x[17] <= '0' && x[18] <= '7' && !x[19]) ||         \
      (!x[2] !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || !x[9] ||   \
           !x[10] || !x[11] || !x[12] || !x[13] || !x[14] || !x[15] ||        \
           !x[16] || !x[17] || !x[18] || !x[19]                               \
         ? 1                                                                  \
         : x[0] == '-' && x[1] <= '9' && x[2] <= '2' && x[3] <= '2' &&        \
             x[4] <= '3' && x[5] <= '3' && x[6] <= '7' && x[7] <= '2' &&      \
             x[8] <= '0' && x[9] <= '3' && x[10] <= '6' && x[11] <= '8' &&    \
             x[12] <= '5' && x[13] <= '4' && x[14] <= '7' && x[15] <= '7' &&  \
             x[16] <= '5' && x[17] <= '8' && x[18] <= '0' && x[19] <= '8' &&  \
             !x[20])

// MAX: 256
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_UINT8_OVERFLOW_FROM_STRING(x) \
    (x[0] != '-') &&                        \
      (!x[1] || !x[2] ? 1                   \
                      : x[0] <= '2' && x[1] <= '5' && x[2] <= '5' && !x[3])

// MAX: 65'535
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_UINT16_OVERFLOW_FROM_STRING(x)                           \
    (x[0] != '-') && (!x[1] || !x[2] || !x[3] || !x[4]                 \
                        ? 1                                            \
                        : x[0] <= '6' && x[1] <= '5' && x[2] <= '5' && \
                            x[3] <= '3' && x[4] <= '5' && !x[5])

// MAX: 4'294'967'295
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_UINT32_OVERFLOW_FROM_STRING(x)                                   \
    (x[0] != '-') &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
           !x[9]                                                               \
         ? 1                                                                   \
         : x[0] <= '4' && x[1] <= '2' && x[2] <= '9' && x[3] <= '4' &&         \
             x[4] <= '9' && x[5] <= '6' && x[6] <= '7' && x[7] <= '2' &&       \
             x[8] <= '9' && x[9] <= '5')

// MAX: 18'446'744'073'709'551'615
// MIN: 0
// NOTE: return 0 if is overflow/underflow otherwise return 1.
#define CHECK_UINT64_OVERFLOW_FROM_STRING(x)                                   \
    (x[0] != '-') &&                                                           \
      (!x[1] || !x[2] || !x[3] || !x[4] || !x[5] || !x[6] || !x[7] || !x[8] || \
           !x[9] || !x[10] || !x[11] || !x[12] || !x[13] || !x[14] ||          \
           !x[15] || !x[16] || !x[17] || !x[18] || !x[19]                      \
         ? 1                                                                   \
         : x[0] <= '1' && x[1] <= '8' && x[2] <= '4' && x[3] <= '4' &&         \
             x[4] <= '4' && x[5] <= '6' && x[6] <= '7' && x[7] <= '4' &&       \
             x[8] <= '4' && x[9] <= '0' && x[10] <= '7' && x[11] <= '3' &&     \
             x[12] <= '7' && x[13] <= '0' && x[14] <= '9' && x[15] <= '5' &&   \
             x[16] <= '5' && x[17] <= '1' && x[18] <= '6' && x[19] <= '1' &&   \
             x[20] <= '5' && !x[21])

#if defined(PLATFORM_64)
#define CHECK_ISIZE_OVERFLOW_FROM_STRING(x) CHECK_INT64_OVERFLOW_FROM_STRING(x)
#define CHECK_USIZE_OVERFLOW_FROM_STRING(x) CHECK_UINT64_OVERFLOW_FROM_STRING(x)
#else
#define CHECK_ISIZE_OVERFLOW_FROM_STRING(x) CHECK_INT32_OVERFLOW_FROM_STRING(x)
#define CHECK_USIZE_OVERFLOW_FROM_STRING(x) CHECK_UINT32_OVERFLOW_FROM_STRING(x)
#endif

/**
 *
 * @brief Convert to string an Int8.
 */
Int8
atoi__Int8(const char *s);

/**
 *
 * @brief Convert to string an Int16.
 */
Int16
atoi__Int16(const char *s);

/**
 *
 * @brief Convert to string an Int32.
 */
Int32
atoi__Int32(const char *s);

/**
 *
 * @brief convert to string an int64.
 */
Int64
atoi__Int64(const char *s);

/**
 *
 * @brief Convert to string an Uint8.
 */
Uint8
atoi__Uint8(const char *s);

/**
 *
 * @brief Convert to string an Uint16.
 */
Uint16
atoi__Uint16(const char *s);

/**
 *
 * @brief convert to string an Uint32.
 */
Uint32
atoi__Uint32(const char *s);

/**
 *
 * @brief convert to string an Uint64.
 */
Uint64
atoi__Uint64(const char *s);

/**
 *
 * @brief convert to string an Isize.
 */
Isize
atoi__Isize(const char *s);

/**
 *
 * @brief convert to string a Usize.
 */
Usize
atoi__Usize(const char *s);

#endif // LILY_BASE_ATOI_H
