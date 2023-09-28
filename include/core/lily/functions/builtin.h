/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_CORE_LILY_FUNCTIONS_BUILTIN_H
#define LILY_CORE_LILY_FUNCTIONS_BUILTIN_H

#include <base/macros.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/data_type.h>

#define BUILTINS_COUNT 29

#define _max__$Int8 0
#define _max__$Int16 1
#define _max__$Int32 2
#define _max__$Int64 3
#define _max__$Isize 4
#define _max__$Uint8 5
#define _max__$Uint16 6
#define _max__$Uint32 7
#define _max__$Uint64 8
#define _max__$Usize 9
#define _max__$Float32 10
#define _max__$Float64 11
#define _min__$Int8 12
#define _min__$Int16 13
#define _min__$Int32 14
#define _min__$Int64 15
#define _min__$Isize 16
#define _min__$Uint8 17
#define _min__$Uint16 18
#define _min__$Uint32 19
#define _min__$Uint64 20
#define _min__$Usize 21
#define _min__$Float32 22
#define _min__$Float64 23
#define _len__$CStr 24
#define _align__$Alloc 25
#define _alloc__$Alloc 26
#define _resize__$Alloc 27
#define _free__$Alloc 28

typedef struct LilyBuiltinFun
{
    const char *name;
    // The `real_name` is the name that was given in the C include file at the
    // path: `lib/builtin/*.h`
    String *real_name;
    LilyCheckedDataType *return_data_type;
    Vec *params; // Vec<LilyCheckedDataType*>*
} LilyBuiltinFun;

/**
 *
 * @brief Function to load Lily builtin.
 */
LilyBuiltinFun *
load_builtins__LilyBuiltin();

/**
 *
 * @brief Return true if is a builtin function.
 */
bool
is_builtin_function__LilyBuiltin(const char *name);

/**
 *
 * @brief Get builtin function depending on the parameters of the function and
 * the type of return.
 */
const LilyBuiltinFun *
get_builtin__LilyBuiltin(LilyBuiltinFun *builtins,
                         const char *name,
                         Vec *params);

/**
 *
 * @brief Check if both builtin funs are equal.
 */
bool
eq__LilyBuiltin(const LilyBuiltinFun *self, const LilyBuiltinFun *other);

/**
 *
 * @brief Convert LilyBuiltinFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyBuiltinFun, const LilyBuiltinFun *self);
#endif

/**
 *
 * @brief Free LilyBuiltinFun type.
 */
DESTRUCTOR(LilyBuiltinFun, const LilyBuiltinFun *self);

#endif // LILY_CORE_LILY_FUNCTIONS_BUILTIN_H
