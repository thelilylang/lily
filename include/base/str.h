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

#ifndef LILY_BASE_STR_H
#define LILY_BASE_STR_H

#include <base/vec.h>

/**
 *
 * @brief Split str.
 * @return Vec<char*>*.
 * @note The result and its items can be free.
 */
Vec *
split__Str(const char *self, char separator);

/**
 *
 * @brief Get slice of Str.
 */
char *
get_slice__Str(const char *self, Usize start, Usize end);

/**
 *
 * @brief Replace target by replace character.
 */
void
replace__Str(char *self, char target, char replace);

/**
 *
 * @brief Count the number of repetitions of this character in this Str.
 */
Usize
count_c__Str(const char *self, char c);

/**
 *
 * @brief Try to find the first occurence over a list of substring.
 * @param matches char* (&)* (&)
 * @param match_at Int32*? (&) - If an occurrence is found in the list, it
 * returns the index of the list where an occurrence was found with the
 * corresponding substring, otherwise it returns -1.
 * @return char*? (&)
 */
char *
strstr_list__Str(char *self, char **matches, Int32 matches_n, Int32 *match_at);

#endif // LILY_BASE_STR_H
