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

#ifndef LILY_CORE_LILY_INTERPRETER_MEMORY_H
#define LILY_CORE_LILY_INTERPRETER_MEMORY_H

#include <base/macros.h>
#include <base/types.h>

#include <builtin/alloc.h>

#include <core/lily/interpreter/vm/value.h>

typedef struct LilyInterpreterMemory
{
    Usize capacity;   // custom capacity or max capacity in bytes of the device
    Usize total_size; // total size allocated in bytes
    Usize total_size_free; // total size free in bytes
} LilyInterpreterMemory;

/**
 *
 * @brief Construct LilyInterpreterMemory type with custom capacity.
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterMemory,
                           LilyInterpreterMemory,
                           capacity,
                           Usize capacity)
{
    return (LilyInterpreterMemory){ .capacity = capacity == 0
                                                  ? __max_capacity__$Alloc()
                                                  : capacity,
                                    .total_size = 0,
                                    .total_size_free = 0 };
}

/**
 *
 * @brief Construct LilyInterpreterMemory type.
 */
inline CONSTRUCTOR(LilyInterpreterMemory, LilyInterpreterMemory)
{
    return (LilyInterpreterMemory){
        .capacity = __max_capacity__$Alloc(),
        .total_size = 0,
        .total_size_free = 0,
    };
}

/**
 *
 * @brief Alloc memory from VM memory controller.
 */
LilyInterpreterValue
alloc__LilyInterpreterMemory(LilyInterpreterMemory *self, Usize size);

/**
 *
 * @brief Resize memory allocated from VM memory controller.
 */
LilyInterpreterValue
resize__LilyInterpreterMemory(LilyInterpreterMemory *self,
                              LilyInterpreterValue *value,
                              Usize new_size);

/**
 *
 * @brief Free memory allocated from VM memory controller.
 */
void
free__LilyInterpreterMemory(LilyInterpreterMemory *self,
                            LilyInterpreterValue *value);

#endif // LILY_CORE_LILY_INTERPRETER_MEMORY_H
