#ifndef C_COMPTIME_GEN_TYPES_H
#define C_COMPTIME_GEN_TYPES_H

#include "platform.h"

#include <stdint.h>

#if __clang_major__ > 15
typedef _BitInt(8) i8;
typedef _BitInt(16) i16;
typedef _BitInt(32) i32;
typedef _BitInt(64) i64;

typedef unsigned _BitInt(8) u8;
typedef unsigned _BitInt(16) u16;
typedef unsigned _BitInt(32) u32;
typedef unsigned _BitInt(64) u64;

#if defined(PLATFORM_64)
typedef _BitInt(64) isize;
typedef unsigned _BitInt(64) usize;
#else
typedef _BitInt(32) isize;
typedef unsigned _BitInt(32) usize;
#endif

#else
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#if defined(PLATFORM_64)
typedef int64_t isize;
typedef uint64_t usize;
#else
typedef int32_t isize;
typedef uint32_t usize;
#endif

#endif

typedef float f32;
typedef double f64;

typedef uintptr_t uptr;

typedef char* str;

#endif // C_COMPTIME_GEN_TYPES_H