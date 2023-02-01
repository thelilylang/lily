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

#ifndef LILY_BASE_HASH_MAP_H
#define LILY_BASE_HASH_MAP_H

#include <base/vec.h>
#include <base/types.h>

typedef struct HashMap {
	Vec *key;
	Vec *value;
	Usize len;
} HashMap;

/**
 *
 * @brief Construct HashMap type.
 */
HashMap* __new__HashMap();

/**
 *
 * @brief Insert key-value pair into HashMap.
 * @return If the key already exists, return the value of the key, otherwise return NULL.
 */
void* insert__HashMap(HashMap *self, void *key, void *value);

/**
 *
 * @brief Free HashMap type.
 */
void __free__HashMap(HashMap *self);

#endif // LILY_BASE_HASH_MAP_H
