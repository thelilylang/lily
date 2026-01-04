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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_COMPILER_GENERIC_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_COMPILER_GENERIC_H

#include <base/alloc.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct LilyCheckedDataType LilyCheckedDataType;

typedef struct LilyCheckedCompilerGenericValue
{
    const String *name;             // String* (&)
    LilyCheckedDataType *data_type; // LilyCheckedDataType* (&)
} LilyCheckedCompilerGenericValue;

/**
 *
 * @brief Construct LilyCheckedCompilerGenericValue type.
 */
CONSTRUCTOR(LilyCheckedCompilerGenericValue *,
            LilyCheckedCompilerGenericValue,
            const String *name,
            LilyCheckedDataType *data_type);

/**
 *
 * @brief Add value and check if the name is not already pushed.
 * @param buffer Vec<LilyCheckedCompilerGenericValue*>*
 */
int
add_value__LilyCheckedCompilerGenericValue(
  LilyCheckedCompilerGenericValue *self,
  Vec *buffer);

/**
 *
 * @brief Get value from compiler generic name.
 * @param buffer Vec<LilyCheckedCompilerGenericValue*>*
 * @return LilyCheckedCompilerGenericValue* (&)?
 */
LilyCheckedCompilerGenericValue *
get_value__LilyCheckedCompilerGenericValue(Vec *buffer, const String *name);

/**
 *
 * @brief Free LilyCheckedCompilerGenericValue type.
 */
inline DESTRUCTOR(LilyCheckedCompilerGenericValue,
                  LilyCheckedCompilerGenericValue *self)
{
    lily_free(self);
}

/**
 *
 * @brief Generate a name a not used compiler generic name.
 * @param used_compiler_generic Vec<String*>*
 */
String *
generate_compiler_generic__LilyCheckedCompilerGeneric(
  const Vec *used_compiler_generic);

/**
 *
 * @brief Add compiler generic to `used_compiler_generic`.
 */
inline void
add_compiler_generic__LilyCheckedCompilerGeneric(Vec *used_compiler_generic)
{
    push__Vec(used_compiler_generic,
              generate_compiler_generic__LilyCheckedCompilerGeneric(
                used_compiler_generic));
}

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_COMPILER_GENERIC_H
