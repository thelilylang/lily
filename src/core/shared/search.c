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

#include <core/shared/search.h>

#include <string.h>

Int32
get_id__Search(const String *id,
               const SizedStr ids_s[],
               const Int32 ids[],
               const Usize ids_s_len)
{
    Usize pointer = 0;
    char first_id_letter = get__String(id, 0);
    const SizedStr *current_pointer = &ids_s[pointer];
    char first_current_pointer_letter = current_pointer->buffer[0];

    // Try matching the first_id_letter with the first_current_pointer_letter.
    while (first_id_letter != first_current_pointer_letter &&
           pointer + 1 < ids_s_len) {
        current_pointer = &ids_s[++pointer];
        first_current_pointer_letter = current_pointer->buffer[0];
    }

    while (first_id_letter == first_current_pointer_letter) {
        if (!strcmp(current_pointer->buffer, id->buffer)) {
            return ids[pointer];
        } else {
            if (pointer + 1 < ids_s_len) {
                current_pointer = &ids_s[++pointer];
                first_current_pointer_letter = current_pointer->buffer[0];

                continue;
            }

            return -1;
        }
    }

    return -1;
}
