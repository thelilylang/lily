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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, PATTERNESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_LITERAL_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_LITERAL_H

#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <base/types.h>

enum LilyCheckedPatternLiteralKind
{
    LILY_CHECKED_PATTERN_LITERAL_KIND_BOOL,
    LILY_CHECKED_PATTERN_LITERAL_KIND_BYTE,
    LILY_CHECKED_PATTERN_LITERAL_KIND_BYTES,
    LILY_CHECKED_PATTERN_LITERAL_KIND_CHAR,
    LILY_CHECKED_PATTERN_LITERAL_KIND_CSTR,
    LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT32,
    LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT64,
    LILY_CHECKED_PATTERN_LITERAL_KIND_INT32,
    LILY_CHECKED_PATTERN_LITERAL_KIND_INT64,
    LILY_CHECKED_PATTERN_LITERAL_KIND_NIL,
    LILY_CHECKED_PATTERN_LITERAL_KIND_NONE,
    LILY_CHECKED_PATTERN_LITERAL_KIND_STR,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT32,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT64,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT8,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT16,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT32,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT64,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_ISIZE,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT8,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT16,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT32,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT64,
    LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_USIZE,
    LILY_CHECKED_PATTERN_LITERAL_KIND_UINT32,
    LILY_CHECKED_PATTERN_LITERAL_KIND_UINT64,
    LILY_CHECKED_PATTERN_LITERAL_KIND_UNDEF,
    LILY_CHECKED_PATTERN_LITERAL_KIND_UNIT
};

/**
 *
 * @brief Convert LilyCheckedPatternLiteralKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternLiteralKind,
               enum LilyCheckedPatternLiteralKind self);
#endif

typedef struct LilyCheckedPatternLiteral
{
    enum LilyCheckedPatternLiteralKind kind;
    union
    {
        bool bool_;
        Uint8 byte;
        Uint8 *bytes; // Uint8* (&)
        char char_;
        char *cstr; // char* (&)
        Float32 float32;
        Float64 float64;
        Int32 int32;
        Int64 int64;
        String *str; // String* (&)
        Float32 suffix_float32;
        Float64 suffix_float64;
        Int8 suffix_int8;
        Int16 suffix_int16;
        Int32 suffix_int32;
        Int64 suffix_int64;
        Isize suffix_isize;
        Uint8 suffix_uint8;
        Uint16 suffix_uint16;
        Uint32 suffix_uint32;
        Uint64 suffix_uint64;
        Usize suffix_usize;
        Uint32 uint32;
        Uint64 uint64;
    };
} LilyCheckedPatternLiteral;

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_BOOL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           bool_,
                           bool bool_)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_BOOL, .bool_ = bool_
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_BYTE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           byte,
                           Uint8 byte)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_BYTE, .byte = byte
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_BYTES).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           bytes,
                           Uint8 *bytes)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_BYTES, .bytes = bytes
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_CHAR).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           char,
                           char char_)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_CHAR, .char_ = char_
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_CSTR).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           cstr,
                           char *cstr)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_CSTR, .cstr = cstr
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT32).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           float32,
                           Float32 float32)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT32, .float32 = float32
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT64).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           float64,
                           Float64 float64)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT64, .float64 = float64
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_INT32).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           int32,
                           Int32 int32)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_INT32, .int32 = int32
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_INT64).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           int64,
                           Int64 int64)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_INT64, .int64 = int64
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_STR).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           str,
                           String *str)
{
    return (LilyCheckedPatternLiteral){ .kind =
                                          LILY_CHECKED_PATTERN_LITERAL_KIND_STR,
                                        .str = str };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT32).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_float32,
                           Float32 suffix_float32)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT32,
        .suffix_float32 = suffix_float32
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT64).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_float64,
                           Float64 suffix_float64)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT64,
        .suffix_float64 = suffix_float64
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT8).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_int8,
                           Int8 suffix_int8)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT8,
        .suffix_int8 = suffix_int8
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT16).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_int16,
                           Int16 suffix_int16)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT16,
        .suffix_int16 = suffix_int16
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT32).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_int32,
                           Int32 suffix_int32)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT32,
        .suffix_int32 = suffix_int32
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT64).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_int64,
                           Int64 suffix_int64)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT64,
        .suffix_int64 = suffix_int64
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_ISIZE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_isize,
                           Isize suffix_isize)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_ISIZE,
        .suffix_isize = suffix_isize
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT8).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_uint8,
                           Uint8 suffix_uint8)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT8,
        .suffix_uint8 = suffix_uint8
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT16).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_uint16,
                           Uint16 suffix_uint16)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT16,
        .suffix_uint16 = suffix_uint16
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT32).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_uint32,
                           Uint32 suffix_uint32)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT32,
        .suffix_uint32 = suffix_uint32
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT64).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_uint64,
                           Uint64 suffix_uint64)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT64,
        .suffix_uint64 = suffix_uint64
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_USIZE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           suffix_usize,
                           Usize suffix_usize)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_USIZE,
        .suffix_usize = suffix_usize
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT32).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           uint32,
                           Uint32 uint32)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_UINT32, .uint32 = uint32
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type
 * (LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT64).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                           LilyCheckedPatternLiteral,
                           uint64,
                           Uint64 uint64)
{
    return (LilyCheckedPatternLiteral){
        .kind = LILY_CHECKED_PATTERN_LITERAL_KIND_UINT64, .uint64 = uint64
    };
}

/**
 *
 * @brief Construct LilyCheckedPatternLiteral type.
 */
inline CONSTRUCTOR(LilyCheckedPatternLiteral,
                   LilyCheckedPatternLiteral,
                   enum LilyCheckedPatternLiteralKind kind)
{
    return (LilyCheckedPatternLiteral){ .kind = kind };
}

/**
 *
 * @brief Check if the both pattern literal are equal.
 */
bool
eq__LilyCheckedPatternLiteral(const LilyCheckedPatternLiteral *self,
                              const LilyCheckedPatternLiteral *other);

/**
 *
 * @brief Convert LilyCheckedPatternLiteral in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternLiteral,
               const LilyCheckedPatternLiteral *self);
#endif

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_LITERAL_H
