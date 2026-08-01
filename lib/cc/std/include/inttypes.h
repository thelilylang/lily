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

/*
 * ISO C99 Standard: 7.8 Format conversion of integer types <inttypes.h>
 */

#ifndef _CC_STD_INTTYPES_H
#define _CC_STD_INTTYPES_H

#include <stddef.h>
#include <stdint.h>
#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.8.1: what a type of <stdint.h> is written and read with, which is only the
   length modifier the type is held by. The compiler says which one that is for
   each type it gives, so what it says is what is used, and the fallback is the
   modifier the type usually is on a platform where it is not said. */

/* The modifier a 64 bit integer is written with is the one long has, where a
   long is that wide, and the one long long has otherwise. */
#if defined(__SIZEOF_LONG__) && __SIZEOF_LONG__ == 8
#define __CC_STD_PRI64_MODIFIER "l"
#else
#define __CC_STD_PRI64_MODIFIER "ll"
#endif /* defined(__SIZEOF_LONG__) && __SIZEOF_LONG__ == 8 */

/* 7.8.1: the fprintf macros. */
#ifdef __INT8_FMTd__
#define PRId8 __INT8_FMTd__
#else
#define PRId8 "hhd"
#endif /* __INT8_FMTd__ */
#ifdef __INT_LEAST8_FMTd__
#define PRIdLEAST8 __INT_LEAST8_FMTd__
#else
#define PRIdLEAST8 "hhd"
#endif /* __INT_LEAST8_FMTd__ */
#ifdef __INT_FAST8_FMTd__
#define PRIdFAST8 __INT_FAST8_FMTd__
#else
#define PRIdFAST8 "hhd"
#endif /* __INT_FAST8_FMTd__ */
#ifdef __INT16_FMTd__
#define PRId16 __INT16_FMTd__
#else
#define PRId16 "hd"
#endif /* __INT16_FMTd__ */
#ifdef __INT_LEAST16_FMTd__
#define PRIdLEAST16 __INT_LEAST16_FMTd__
#else
#define PRIdLEAST16 "hd"
#endif /* __INT_LEAST16_FMTd__ */
#ifdef __INT_FAST16_FMTd__
#define PRIdFAST16 __INT_FAST16_FMTd__
#else
#define PRIdFAST16 "hd"
#endif /* __INT_FAST16_FMTd__ */
#ifdef __INT32_FMTd__
#define PRId32 __INT32_FMTd__
#else
#define PRId32 "d"
#endif /* __INT32_FMTd__ */
#ifdef __INT_LEAST32_FMTd__
#define PRIdLEAST32 __INT_LEAST32_FMTd__
#else
#define PRIdLEAST32 "d"
#endif /* __INT_LEAST32_FMTd__ */
#ifdef __INT_FAST32_FMTd__
#define PRIdFAST32 __INT_FAST32_FMTd__
#else
#define PRIdFAST32 "d"
#endif /* __INT_FAST32_FMTd__ */
#ifdef __INT64_FMTd__
#define PRId64 __INT64_FMTd__
#else
#define PRId64 __CC_STD_PRI64_MODIFIER "d"
#endif /* __INT64_FMTd__ */
#ifdef __INT_LEAST64_FMTd__
#define PRIdLEAST64 __INT_LEAST64_FMTd__
#else
#define PRIdLEAST64 __CC_STD_PRI64_MODIFIER "d"
#endif /* __INT_LEAST64_FMTd__ */
#ifdef __INT_FAST64_FMTd__
#define PRIdFAST64 __INT_FAST64_FMTd__
#else
#define PRIdFAST64 __CC_STD_PRI64_MODIFIER "d"
#endif /* __INT_FAST64_FMTd__ */
#ifdef __INTMAX_FMTd__
#define PRIdMAX __INTMAX_FMTd__
#else
#define PRIdMAX __CC_STD_PRI64_MODIFIER "d"
#endif /* __INTMAX_FMTd__ */
#ifdef __INTPTR_FMTd__
#define PRIdPTR __INTPTR_FMTd__
#else
#define PRIdPTR __CC_STD_PRI64_MODIFIER "d"
#endif /* __INTPTR_FMTd__ */

#ifdef __INT8_FMTi__
#define PRIi8 __INT8_FMTi__
#else
#define PRIi8 "hhi"
#endif /* __INT8_FMTi__ */
#ifdef __INT_LEAST8_FMTi__
#define PRIiLEAST8 __INT_LEAST8_FMTi__
#else
#define PRIiLEAST8 "hhi"
#endif /* __INT_LEAST8_FMTi__ */
#ifdef __INT_FAST8_FMTi__
#define PRIiFAST8 __INT_FAST8_FMTi__
#else
#define PRIiFAST8 "hhi"
#endif /* __INT_FAST8_FMTi__ */
#ifdef __INT16_FMTi__
#define PRIi16 __INT16_FMTi__
#else
#define PRIi16 "hi"
#endif /* __INT16_FMTi__ */
#ifdef __INT_LEAST16_FMTi__
#define PRIiLEAST16 __INT_LEAST16_FMTi__
#else
#define PRIiLEAST16 "hi"
#endif /* __INT_LEAST16_FMTi__ */
#ifdef __INT_FAST16_FMTi__
#define PRIiFAST16 __INT_FAST16_FMTi__
#else
#define PRIiFAST16 "hi"
#endif /* __INT_FAST16_FMTi__ */
#ifdef __INT32_FMTi__
#define PRIi32 __INT32_FMTi__
#else
#define PRIi32 "i"
#endif /* __INT32_FMTi__ */
#ifdef __INT_LEAST32_FMTi__
#define PRIiLEAST32 __INT_LEAST32_FMTi__
#else
#define PRIiLEAST32 "i"
#endif /* __INT_LEAST32_FMTi__ */
#ifdef __INT_FAST32_FMTi__
#define PRIiFAST32 __INT_FAST32_FMTi__
#else
#define PRIiFAST32 "i"
#endif /* __INT_FAST32_FMTi__ */
#ifdef __INT64_FMTi__
#define PRIi64 __INT64_FMTi__
#else
#define PRIi64 __CC_STD_PRI64_MODIFIER "i"
#endif /* __INT64_FMTi__ */
#ifdef __INT_LEAST64_FMTi__
#define PRIiLEAST64 __INT_LEAST64_FMTi__
#else
#define PRIiLEAST64 __CC_STD_PRI64_MODIFIER "i"
#endif /* __INT_LEAST64_FMTi__ */
#ifdef __INT_FAST64_FMTi__
#define PRIiFAST64 __INT_FAST64_FMTi__
#else
#define PRIiFAST64 __CC_STD_PRI64_MODIFIER "i"
#endif /* __INT_FAST64_FMTi__ */
#ifdef __INTMAX_FMTi__
#define PRIiMAX __INTMAX_FMTi__
#else
#define PRIiMAX __CC_STD_PRI64_MODIFIER "i"
#endif /* __INTMAX_FMTi__ */
#ifdef __INTPTR_FMTi__
#define PRIiPTR __INTPTR_FMTi__
#else
#define PRIiPTR __CC_STD_PRI64_MODIFIER "i"
#endif /* __INTPTR_FMTi__ */

#ifdef __UINT8_FMTo__
#define PRIo8 __UINT8_FMTo__
#else
#define PRIo8 "hho"
#endif /* __UINT8_FMTo__ */
#ifdef __UINT_LEAST8_FMTo__
#define PRIoLEAST8 __UINT_LEAST8_FMTo__
#else
#define PRIoLEAST8 "hho"
#endif /* __UINT_LEAST8_FMTo__ */
#ifdef __UINT_FAST8_FMTo__
#define PRIoFAST8 __UINT_FAST8_FMTo__
#else
#define PRIoFAST8 "hho"
#endif /* __UINT_FAST8_FMTo__ */
#ifdef __UINT16_FMTo__
#define PRIo16 __UINT16_FMTo__
#else
#define PRIo16 "ho"
#endif /* __UINT16_FMTo__ */
#ifdef __UINT_LEAST16_FMTo__
#define PRIoLEAST16 __UINT_LEAST16_FMTo__
#else
#define PRIoLEAST16 "ho"
#endif /* __UINT_LEAST16_FMTo__ */
#ifdef __UINT_FAST16_FMTo__
#define PRIoFAST16 __UINT_FAST16_FMTo__
#else
#define PRIoFAST16 "ho"
#endif /* __UINT_FAST16_FMTo__ */
#ifdef __UINT32_FMTo__
#define PRIo32 __UINT32_FMTo__
#else
#define PRIo32 "o"
#endif /* __UINT32_FMTo__ */
#ifdef __UINT_LEAST32_FMTo__
#define PRIoLEAST32 __UINT_LEAST32_FMTo__
#else
#define PRIoLEAST32 "o"
#endif /* __UINT_LEAST32_FMTo__ */
#ifdef __UINT_FAST32_FMTo__
#define PRIoFAST32 __UINT_FAST32_FMTo__
#else
#define PRIoFAST32 "o"
#endif /* __UINT_FAST32_FMTo__ */
#ifdef __UINT64_FMTo__
#define PRIo64 __UINT64_FMTo__
#else
#define PRIo64 __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINT64_FMTo__ */
#ifdef __UINT_LEAST64_FMTo__
#define PRIoLEAST64 __UINT_LEAST64_FMTo__
#else
#define PRIoLEAST64 __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINT_LEAST64_FMTo__ */
#ifdef __UINT_FAST64_FMTo__
#define PRIoFAST64 __UINT_FAST64_FMTo__
#else
#define PRIoFAST64 __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINT_FAST64_FMTo__ */
#ifdef __UINTMAX_FMTo__
#define PRIoMAX __UINTMAX_FMTo__
#else
#define PRIoMAX __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINTMAX_FMTo__ */
#ifdef __UINTPTR_FMTo__
#define PRIoPTR __UINTPTR_FMTo__
#else
#define PRIoPTR __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINTPTR_FMTo__ */

#ifdef __UINT8_FMTu__
#define PRIu8 __UINT8_FMTu__
#else
#define PRIu8 "hhu"
#endif /* __UINT8_FMTu__ */
#ifdef __UINT_LEAST8_FMTu__
#define PRIuLEAST8 __UINT_LEAST8_FMTu__
#else
#define PRIuLEAST8 "hhu"
#endif /* __UINT_LEAST8_FMTu__ */
#ifdef __UINT_FAST8_FMTu__
#define PRIuFAST8 __UINT_FAST8_FMTu__
#else
#define PRIuFAST8 "hhu"
#endif /* __UINT_FAST8_FMTu__ */
#ifdef __UINT16_FMTu__
#define PRIu16 __UINT16_FMTu__
#else
#define PRIu16 "hu"
#endif /* __UINT16_FMTu__ */
#ifdef __UINT_LEAST16_FMTu__
#define PRIuLEAST16 __UINT_LEAST16_FMTu__
#else
#define PRIuLEAST16 "hu"
#endif /* __UINT_LEAST16_FMTu__ */
#ifdef __UINT_FAST16_FMTu__
#define PRIuFAST16 __UINT_FAST16_FMTu__
#else
#define PRIuFAST16 "hu"
#endif /* __UINT_FAST16_FMTu__ */
#ifdef __UINT32_FMTu__
#define PRIu32 __UINT32_FMTu__
#else
#define PRIu32 "u"
#endif /* __UINT32_FMTu__ */
#ifdef __UINT_LEAST32_FMTu__
#define PRIuLEAST32 __UINT_LEAST32_FMTu__
#else
#define PRIuLEAST32 "u"
#endif /* __UINT_LEAST32_FMTu__ */
#ifdef __UINT_FAST32_FMTu__
#define PRIuFAST32 __UINT_FAST32_FMTu__
#else
#define PRIuFAST32 "u"
#endif /* __UINT_FAST32_FMTu__ */
#ifdef __UINT64_FMTu__
#define PRIu64 __UINT64_FMTu__
#else
#define PRIu64 __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINT64_FMTu__ */
#ifdef __UINT_LEAST64_FMTu__
#define PRIuLEAST64 __UINT_LEAST64_FMTu__
#else
#define PRIuLEAST64 __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINT_LEAST64_FMTu__ */
#ifdef __UINT_FAST64_FMTu__
#define PRIuFAST64 __UINT_FAST64_FMTu__
#else
#define PRIuFAST64 __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINT_FAST64_FMTu__ */
#ifdef __UINTMAX_FMTu__
#define PRIuMAX __UINTMAX_FMTu__
#else
#define PRIuMAX __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINTMAX_FMTu__ */
#ifdef __UINTPTR_FMTu__
#define PRIuPTR __UINTPTR_FMTu__
#else
#define PRIuPTR __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINTPTR_FMTu__ */

#ifdef __UINT8_FMTx__
#define PRIx8 __UINT8_FMTx__
#else
#define PRIx8 "hhx"
#endif /* __UINT8_FMTx__ */
#ifdef __UINT_LEAST8_FMTx__
#define PRIxLEAST8 __UINT_LEAST8_FMTx__
#else
#define PRIxLEAST8 "hhx"
#endif /* __UINT_LEAST8_FMTx__ */
#ifdef __UINT_FAST8_FMTx__
#define PRIxFAST8 __UINT_FAST8_FMTx__
#else
#define PRIxFAST8 "hhx"
#endif /* __UINT_FAST8_FMTx__ */
#ifdef __UINT16_FMTx__
#define PRIx16 __UINT16_FMTx__
#else
#define PRIx16 "hx"
#endif /* __UINT16_FMTx__ */
#ifdef __UINT_LEAST16_FMTx__
#define PRIxLEAST16 __UINT_LEAST16_FMTx__
#else
#define PRIxLEAST16 "hx"
#endif /* __UINT_LEAST16_FMTx__ */
#ifdef __UINT_FAST16_FMTx__
#define PRIxFAST16 __UINT_FAST16_FMTx__
#else
#define PRIxFAST16 "hx"
#endif /* __UINT_FAST16_FMTx__ */
#ifdef __UINT32_FMTx__
#define PRIx32 __UINT32_FMTx__
#else
#define PRIx32 "x"
#endif /* __UINT32_FMTx__ */
#ifdef __UINT_LEAST32_FMTx__
#define PRIxLEAST32 __UINT_LEAST32_FMTx__
#else
#define PRIxLEAST32 "x"
#endif /* __UINT_LEAST32_FMTx__ */
#ifdef __UINT_FAST32_FMTx__
#define PRIxFAST32 __UINT_FAST32_FMTx__
#else
#define PRIxFAST32 "x"
#endif /* __UINT_FAST32_FMTx__ */
#ifdef __UINT64_FMTx__
#define PRIx64 __UINT64_FMTx__
#else
#define PRIx64 __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINT64_FMTx__ */
#ifdef __UINT_LEAST64_FMTx__
#define PRIxLEAST64 __UINT_LEAST64_FMTx__
#else
#define PRIxLEAST64 __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINT_LEAST64_FMTx__ */
#ifdef __UINT_FAST64_FMTx__
#define PRIxFAST64 __UINT_FAST64_FMTx__
#else
#define PRIxFAST64 __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINT_FAST64_FMTx__ */
#ifdef __UINTMAX_FMTx__
#define PRIxMAX __UINTMAX_FMTx__
#else
#define PRIxMAX __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINTMAX_FMTx__ */
#ifdef __UINTPTR_FMTx__
#define PRIxPTR __UINTPTR_FMTx__
#else
#define PRIxPTR __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINTPTR_FMTx__ */

#ifdef __UINT8_FMTX__
#define PRIX8 __UINT8_FMTX__
#else
#define PRIX8 "hhX"
#endif /* __UINT8_FMTX__ */
#ifdef __UINT_LEAST8_FMTX__
#define PRIXLEAST8 __UINT_LEAST8_FMTX__
#else
#define PRIXLEAST8 "hhX"
#endif /* __UINT_LEAST8_FMTX__ */
#ifdef __UINT_FAST8_FMTX__
#define PRIXFAST8 __UINT_FAST8_FMTX__
#else
#define PRIXFAST8 "hhX"
#endif /* __UINT_FAST8_FMTX__ */
#ifdef __UINT16_FMTX__
#define PRIX16 __UINT16_FMTX__
#else
#define PRIX16 "hX"
#endif /* __UINT16_FMTX__ */
#ifdef __UINT_LEAST16_FMTX__
#define PRIXLEAST16 __UINT_LEAST16_FMTX__
#else
#define PRIXLEAST16 "hX"
#endif /* __UINT_LEAST16_FMTX__ */
#ifdef __UINT_FAST16_FMTX__
#define PRIXFAST16 __UINT_FAST16_FMTX__
#else
#define PRIXFAST16 "hX"
#endif /* __UINT_FAST16_FMTX__ */
#ifdef __UINT32_FMTX__
#define PRIX32 __UINT32_FMTX__
#else
#define PRIX32 "X"
#endif /* __UINT32_FMTX__ */
#ifdef __UINT_LEAST32_FMTX__
#define PRIXLEAST32 __UINT_LEAST32_FMTX__
#else
#define PRIXLEAST32 "X"
#endif /* __UINT_LEAST32_FMTX__ */
#ifdef __UINT_FAST32_FMTX__
#define PRIXFAST32 __UINT_FAST32_FMTX__
#else
#define PRIXFAST32 "X"
#endif /* __UINT_FAST32_FMTX__ */
#ifdef __UINT64_FMTX__
#define PRIX64 __UINT64_FMTX__
#else
#define PRIX64 __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINT64_FMTX__ */
#ifdef __UINT_LEAST64_FMTX__
#define PRIXLEAST64 __UINT_LEAST64_FMTX__
#else
#define PRIXLEAST64 __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINT_LEAST64_FMTX__ */
#ifdef __UINT_FAST64_FMTX__
#define PRIXFAST64 __UINT_FAST64_FMTX__
#else
#define PRIXFAST64 __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINT_FAST64_FMTX__ */
#ifdef __UINTMAX_FMTX__
#define PRIXMAX __UINTMAX_FMTX__
#else
#define PRIXMAX __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINTMAX_FMTX__ */
#ifdef __UINTPTR_FMTX__
#define PRIXPTR __UINTPTR_FMTX__
#else
#define PRIXPTR __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINTPTR_FMTX__ */

/* 7.8.1: the fscanf macros. */
#ifdef __INT8_FMTd__
#define SCNd8 __INT8_FMTd__
#else
#define SCNd8 "hhd"
#endif /* __INT8_FMTd__ */
#ifdef __INT_LEAST8_FMTd__
#define SCNdLEAST8 __INT_LEAST8_FMTd__
#else
#define SCNdLEAST8 "hhd"
#endif /* __INT_LEAST8_FMTd__ */
#ifdef __INT_FAST8_FMTd__
#define SCNdFAST8 __INT_FAST8_FMTd__
#else
#define SCNdFAST8 "hhd"
#endif /* __INT_FAST8_FMTd__ */
#ifdef __INT16_FMTd__
#define SCNd16 __INT16_FMTd__
#else
#define SCNd16 "hd"
#endif /* __INT16_FMTd__ */
#ifdef __INT_LEAST16_FMTd__
#define SCNdLEAST16 __INT_LEAST16_FMTd__
#else
#define SCNdLEAST16 "hd"
#endif /* __INT_LEAST16_FMTd__ */
#ifdef __INT_FAST16_FMTd__
#define SCNdFAST16 __INT_FAST16_FMTd__
#else
#define SCNdFAST16 "hd"
#endif /* __INT_FAST16_FMTd__ */
#ifdef __INT32_FMTd__
#define SCNd32 __INT32_FMTd__
#else
#define SCNd32 "d"
#endif /* __INT32_FMTd__ */
#ifdef __INT_LEAST32_FMTd__
#define SCNdLEAST32 __INT_LEAST32_FMTd__
#else
#define SCNdLEAST32 "d"
#endif /* __INT_LEAST32_FMTd__ */
#ifdef __INT_FAST32_FMTd__
#define SCNdFAST32 __INT_FAST32_FMTd__
#else
#define SCNdFAST32 "d"
#endif /* __INT_FAST32_FMTd__ */
#ifdef __INT64_FMTd__
#define SCNd64 __INT64_FMTd__
#else
#define SCNd64 __CC_STD_PRI64_MODIFIER "d"
#endif /* __INT64_FMTd__ */
#ifdef __INT_LEAST64_FMTd__
#define SCNdLEAST64 __INT_LEAST64_FMTd__
#else
#define SCNdLEAST64 __CC_STD_PRI64_MODIFIER "d"
#endif /* __INT_LEAST64_FMTd__ */
#ifdef __INT_FAST64_FMTd__
#define SCNdFAST64 __INT_FAST64_FMTd__
#else
#define SCNdFAST64 __CC_STD_PRI64_MODIFIER "d"
#endif /* __INT_FAST64_FMTd__ */
#ifdef __INTMAX_FMTd__
#define SCNdMAX __INTMAX_FMTd__
#else
#define SCNdMAX __CC_STD_PRI64_MODIFIER "d"
#endif /* __INTMAX_FMTd__ */
#ifdef __INTPTR_FMTd__
#define SCNdPTR __INTPTR_FMTd__
#else
#define SCNdPTR __CC_STD_PRI64_MODIFIER "d"
#endif /* __INTPTR_FMTd__ */

#ifdef __INT8_FMTi__
#define SCNi8 __INT8_FMTi__
#else
#define SCNi8 "hhi"
#endif /* __INT8_FMTi__ */
#ifdef __INT_LEAST8_FMTi__
#define SCNiLEAST8 __INT_LEAST8_FMTi__
#else
#define SCNiLEAST8 "hhi"
#endif /* __INT_LEAST8_FMTi__ */
#ifdef __INT_FAST8_FMTi__
#define SCNiFAST8 __INT_FAST8_FMTi__
#else
#define SCNiFAST8 "hhi"
#endif /* __INT_FAST8_FMTi__ */
#ifdef __INT16_FMTi__
#define SCNi16 __INT16_FMTi__
#else
#define SCNi16 "hi"
#endif /* __INT16_FMTi__ */
#ifdef __INT_LEAST16_FMTi__
#define SCNiLEAST16 __INT_LEAST16_FMTi__
#else
#define SCNiLEAST16 "hi"
#endif /* __INT_LEAST16_FMTi__ */
#ifdef __INT_FAST16_FMTi__
#define SCNiFAST16 __INT_FAST16_FMTi__
#else
#define SCNiFAST16 "hi"
#endif /* __INT_FAST16_FMTi__ */
#ifdef __INT32_FMTi__
#define SCNi32 __INT32_FMTi__
#else
#define SCNi32 "i"
#endif /* __INT32_FMTi__ */
#ifdef __INT_LEAST32_FMTi__
#define SCNiLEAST32 __INT_LEAST32_FMTi__
#else
#define SCNiLEAST32 "i"
#endif /* __INT_LEAST32_FMTi__ */
#ifdef __INT_FAST32_FMTi__
#define SCNiFAST32 __INT_FAST32_FMTi__
#else
#define SCNiFAST32 "i"
#endif /* __INT_FAST32_FMTi__ */
#ifdef __INT64_FMTi__
#define SCNi64 __INT64_FMTi__
#else
#define SCNi64 __CC_STD_PRI64_MODIFIER "i"
#endif /* __INT64_FMTi__ */
#ifdef __INT_LEAST64_FMTi__
#define SCNiLEAST64 __INT_LEAST64_FMTi__
#else
#define SCNiLEAST64 __CC_STD_PRI64_MODIFIER "i"
#endif /* __INT_LEAST64_FMTi__ */
#ifdef __INT_FAST64_FMTi__
#define SCNiFAST64 __INT_FAST64_FMTi__
#else
#define SCNiFAST64 __CC_STD_PRI64_MODIFIER "i"
#endif /* __INT_FAST64_FMTi__ */
#ifdef __INTMAX_FMTi__
#define SCNiMAX __INTMAX_FMTi__
#else
#define SCNiMAX __CC_STD_PRI64_MODIFIER "i"
#endif /* __INTMAX_FMTi__ */
#ifdef __INTPTR_FMTi__
#define SCNiPTR __INTPTR_FMTi__
#else
#define SCNiPTR __CC_STD_PRI64_MODIFIER "i"
#endif /* __INTPTR_FMTi__ */

#ifdef __UINT8_FMTo__
#define SCNo8 __UINT8_FMTo__
#else
#define SCNo8 "hho"
#endif /* __UINT8_FMTo__ */
#ifdef __UINT_LEAST8_FMTo__
#define SCNoLEAST8 __UINT_LEAST8_FMTo__
#else
#define SCNoLEAST8 "hho"
#endif /* __UINT_LEAST8_FMTo__ */
#ifdef __UINT_FAST8_FMTo__
#define SCNoFAST8 __UINT_FAST8_FMTo__
#else
#define SCNoFAST8 "hho"
#endif /* __UINT_FAST8_FMTo__ */
#ifdef __UINT16_FMTo__
#define SCNo16 __UINT16_FMTo__
#else
#define SCNo16 "ho"
#endif /* __UINT16_FMTo__ */
#ifdef __UINT_LEAST16_FMTo__
#define SCNoLEAST16 __UINT_LEAST16_FMTo__
#else
#define SCNoLEAST16 "ho"
#endif /* __UINT_LEAST16_FMTo__ */
#ifdef __UINT_FAST16_FMTo__
#define SCNoFAST16 __UINT_FAST16_FMTo__
#else
#define SCNoFAST16 "ho"
#endif /* __UINT_FAST16_FMTo__ */
#ifdef __UINT32_FMTo__
#define SCNo32 __UINT32_FMTo__
#else
#define SCNo32 "o"
#endif /* __UINT32_FMTo__ */
#ifdef __UINT_LEAST32_FMTo__
#define SCNoLEAST32 __UINT_LEAST32_FMTo__
#else
#define SCNoLEAST32 "o"
#endif /* __UINT_LEAST32_FMTo__ */
#ifdef __UINT_FAST32_FMTo__
#define SCNoFAST32 __UINT_FAST32_FMTo__
#else
#define SCNoFAST32 "o"
#endif /* __UINT_FAST32_FMTo__ */
#ifdef __UINT64_FMTo__
#define SCNo64 __UINT64_FMTo__
#else
#define SCNo64 __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINT64_FMTo__ */
#ifdef __UINT_LEAST64_FMTo__
#define SCNoLEAST64 __UINT_LEAST64_FMTo__
#else
#define SCNoLEAST64 __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINT_LEAST64_FMTo__ */
#ifdef __UINT_FAST64_FMTo__
#define SCNoFAST64 __UINT_FAST64_FMTo__
#else
#define SCNoFAST64 __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINT_FAST64_FMTo__ */
#ifdef __UINTMAX_FMTo__
#define SCNoMAX __UINTMAX_FMTo__
#else
#define SCNoMAX __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINTMAX_FMTo__ */
#ifdef __UINTPTR_FMTo__
#define SCNoPTR __UINTPTR_FMTo__
#else
#define SCNoPTR __CC_STD_PRI64_MODIFIER "o"
#endif /* __UINTPTR_FMTo__ */

#ifdef __UINT8_FMTu__
#define SCNu8 __UINT8_FMTu__
#else
#define SCNu8 "hhu"
#endif /* __UINT8_FMTu__ */
#ifdef __UINT_LEAST8_FMTu__
#define SCNuLEAST8 __UINT_LEAST8_FMTu__
#else
#define SCNuLEAST8 "hhu"
#endif /* __UINT_LEAST8_FMTu__ */
#ifdef __UINT_FAST8_FMTu__
#define SCNuFAST8 __UINT_FAST8_FMTu__
#else
#define SCNuFAST8 "hhu"
#endif /* __UINT_FAST8_FMTu__ */
#ifdef __UINT16_FMTu__
#define SCNu16 __UINT16_FMTu__
#else
#define SCNu16 "hu"
#endif /* __UINT16_FMTu__ */
#ifdef __UINT_LEAST16_FMTu__
#define SCNuLEAST16 __UINT_LEAST16_FMTu__
#else
#define SCNuLEAST16 "hu"
#endif /* __UINT_LEAST16_FMTu__ */
#ifdef __UINT_FAST16_FMTu__
#define SCNuFAST16 __UINT_FAST16_FMTu__
#else
#define SCNuFAST16 "hu"
#endif /* __UINT_FAST16_FMTu__ */
#ifdef __UINT32_FMTu__
#define SCNu32 __UINT32_FMTu__
#else
#define SCNu32 "u"
#endif /* __UINT32_FMTu__ */
#ifdef __UINT_LEAST32_FMTu__
#define SCNuLEAST32 __UINT_LEAST32_FMTu__
#else
#define SCNuLEAST32 "u"
#endif /* __UINT_LEAST32_FMTu__ */
#ifdef __UINT_FAST32_FMTu__
#define SCNuFAST32 __UINT_FAST32_FMTu__
#else
#define SCNuFAST32 "u"
#endif /* __UINT_FAST32_FMTu__ */
#ifdef __UINT64_FMTu__
#define SCNu64 __UINT64_FMTu__
#else
#define SCNu64 __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINT64_FMTu__ */
#ifdef __UINT_LEAST64_FMTu__
#define SCNuLEAST64 __UINT_LEAST64_FMTu__
#else
#define SCNuLEAST64 __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINT_LEAST64_FMTu__ */
#ifdef __UINT_FAST64_FMTu__
#define SCNuFAST64 __UINT_FAST64_FMTu__
#else
#define SCNuFAST64 __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINT_FAST64_FMTu__ */
#ifdef __UINTMAX_FMTu__
#define SCNuMAX __UINTMAX_FMTu__
#else
#define SCNuMAX __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINTMAX_FMTu__ */
#ifdef __UINTPTR_FMTu__
#define SCNuPTR __UINTPTR_FMTu__
#else
#define SCNuPTR __CC_STD_PRI64_MODIFIER "u"
#endif /* __UINTPTR_FMTu__ */

#ifdef __UINT8_FMTx__
#define SCNx8 __UINT8_FMTx__
#else
#define SCNx8 "hhx"
#endif /* __UINT8_FMTx__ */
#ifdef __UINT_LEAST8_FMTx__
#define SCNxLEAST8 __UINT_LEAST8_FMTx__
#else
#define SCNxLEAST8 "hhx"
#endif /* __UINT_LEAST8_FMTx__ */
#ifdef __UINT_FAST8_FMTx__
#define SCNxFAST8 __UINT_FAST8_FMTx__
#else
#define SCNxFAST8 "hhx"
#endif /* __UINT_FAST8_FMTx__ */
#ifdef __UINT16_FMTx__
#define SCNx16 __UINT16_FMTx__
#else
#define SCNx16 "hx"
#endif /* __UINT16_FMTx__ */
#ifdef __UINT_LEAST16_FMTx__
#define SCNxLEAST16 __UINT_LEAST16_FMTx__
#else
#define SCNxLEAST16 "hx"
#endif /* __UINT_LEAST16_FMTx__ */
#ifdef __UINT_FAST16_FMTx__
#define SCNxFAST16 __UINT_FAST16_FMTx__
#else
#define SCNxFAST16 "hx"
#endif /* __UINT_FAST16_FMTx__ */
#ifdef __UINT32_FMTx__
#define SCNx32 __UINT32_FMTx__
#else
#define SCNx32 "x"
#endif /* __UINT32_FMTx__ */
#ifdef __UINT_LEAST32_FMTx__
#define SCNxLEAST32 __UINT_LEAST32_FMTx__
#else
#define SCNxLEAST32 "x"
#endif /* __UINT_LEAST32_FMTx__ */
#ifdef __UINT_FAST32_FMTx__
#define SCNxFAST32 __UINT_FAST32_FMTx__
#else
#define SCNxFAST32 "x"
#endif /* __UINT_FAST32_FMTx__ */
#ifdef __UINT64_FMTx__
#define SCNx64 __UINT64_FMTx__
#else
#define SCNx64 __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINT64_FMTx__ */
#ifdef __UINT_LEAST64_FMTx__
#define SCNxLEAST64 __UINT_LEAST64_FMTx__
#else
#define SCNxLEAST64 __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINT_LEAST64_FMTx__ */
#ifdef __UINT_FAST64_FMTx__
#define SCNxFAST64 __UINT_FAST64_FMTx__
#else
#define SCNxFAST64 __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINT_FAST64_FMTx__ */
#ifdef __UINTMAX_FMTx__
#define SCNxMAX __UINTMAX_FMTx__
#else
#define SCNxMAX __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINTMAX_FMTx__ */
#ifdef __UINTPTR_FMTx__
#define SCNxPTR __UINTPTR_FMTx__
#else
#define SCNxPTR __CC_STD_PRI64_MODIFIER "x"
#endif /* __UINTPTR_FMTx__ */

#ifdef __UINT8_FMTX__
#define SCNX8 __UINT8_FMTX__
#else
#define SCNX8 "hhX"
#endif /* __UINT8_FMTX__ */
#ifdef __UINT_LEAST8_FMTX__
#define SCNXLEAST8 __UINT_LEAST8_FMTX__
#else
#define SCNXLEAST8 "hhX"
#endif /* __UINT_LEAST8_FMTX__ */
#ifdef __UINT_FAST8_FMTX__
#define SCNXFAST8 __UINT_FAST8_FMTX__
#else
#define SCNXFAST8 "hhX"
#endif /* __UINT_FAST8_FMTX__ */
#ifdef __UINT16_FMTX__
#define SCNX16 __UINT16_FMTX__
#else
#define SCNX16 "hX"
#endif /* __UINT16_FMTX__ */
#ifdef __UINT_LEAST16_FMTX__
#define SCNXLEAST16 __UINT_LEAST16_FMTX__
#else
#define SCNXLEAST16 "hX"
#endif /* __UINT_LEAST16_FMTX__ */
#ifdef __UINT_FAST16_FMTX__
#define SCNXFAST16 __UINT_FAST16_FMTX__
#else
#define SCNXFAST16 "hX"
#endif /* __UINT_FAST16_FMTX__ */
#ifdef __UINT32_FMTX__
#define SCNX32 __UINT32_FMTX__
#else
#define SCNX32 "X"
#endif /* __UINT32_FMTX__ */
#ifdef __UINT_LEAST32_FMTX__
#define SCNXLEAST32 __UINT_LEAST32_FMTX__
#else
#define SCNXLEAST32 "X"
#endif /* __UINT_LEAST32_FMTX__ */
#ifdef __UINT_FAST32_FMTX__
#define SCNXFAST32 __UINT_FAST32_FMTX__
#else
#define SCNXFAST32 "X"
#endif /* __UINT_FAST32_FMTX__ */
#ifdef __UINT64_FMTX__
#define SCNX64 __UINT64_FMTX__
#else
#define SCNX64 __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINT64_FMTX__ */
#ifdef __UINT_LEAST64_FMTX__
#define SCNXLEAST64 __UINT_LEAST64_FMTX__
#else
#define SCNXLEAST64 __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINT_LEAST64_FMTX__ */
#ifdef __UINT_FAST64_FMTX__
#define SCNXFAST64 __UINT_FAST64_FMTX__
#else
#define SCNXFAST64 __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINT_FAST64_FMTX__ */
#ifdef __UINTMAX_FMTX__
#define SCNXMAX __UINTMAX_FMTX__
#else
#define SCNXMAX __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINTMAX_FMTX__ */
#ifdef __UINTPTR_FMTX__
#define SCNXPTR __UINTPTR_FMTX__
#else
#define SCNXPTR __CC_STD_PRI64_MODIFIER "X"
#endif /* __UINTPTR_FMTX__ */


/* 7.8.1: the quotient and the remainder of an intmax_t division. */
typedef struct
{
    intmax_t quot;
    intmax_t rem;
} imaxdiv_t;

/* 7.8.2.1 - 7.8.2.2 */
extern intmax_t
imaxabs(intmax_t j);
extern imaxdiv_t
imaxdiv(intmax_t numer, intmax_t denom);

/* 7.8.2.3 - 7.8.2.4 */
extern intmax_t
strtoimax(const char *__restrict nptr, char **__restrict endptr, int base);
extern uintmax_t
strtoumax(const char *__restrict nptr, char **__restrict endptr, int base);
extern intmax_t
wcstoimax(const wchar_t *__restrict nptr, wchar_t **__restrict endptr,
          int base);
extern uintmax_t
wcstoumax(const wchar_t *__restrict nptr, wchar_t **__restrict endptr,
          int base);

__END_DECLS

#endif /* _CC_STD_INTTYPES_H */
