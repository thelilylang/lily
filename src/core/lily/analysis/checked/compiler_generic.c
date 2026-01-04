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

#include <core/lily/analysis/checked/compiler_generic.h>
#include <core/lily/analysis/checked/data_type.h>

#include <string.h>

#define a 0x61

CONSTRUCTOR(LilyCheckedCompilerGenericValue *,
            LilyCheckedCompilerGenericValue,
            const String *name,
            LilyCheckedDataType *data_type)
{
    LilyCheckedCompilerGenericValue *self =
      lily_malloc(sizeof(LilyCheckedCompilerGenericValue));

    self->name = name;
    self->data_type = data_type;

    return self;
}

int
add_value__LilyCheckedCompilerGenericValue(
  LilyCheckedCompilerGenericValue *self,
  Vec *buffer)
{
    for (Usize i = 0; i < buffer->len; ++i) {
        if (!strcmp(CAST(LilyCheckedCompilerGenericValue *, get__Vec(buffer, i))
                      ->name->buffer,
                    self->name->buffer)) {
            return 1;
        }
    }

    push__Vec(buffer, self);

    return 0;
}

LilyCheckedCompilerGenericValue *
get_value__LilyCheckedCompilerGenericValue(Vec *buffer, const String *name)
{
    for (Usize i = 0; i < buffer->len; ++i) {
        LilyCheckedCompilerGenericValue *value = get__Vec(buffer, i);

        if (!strcmp(value->name->buffer, name->buffer)) {
            return value;
        }
    }

    return NULL;
}

String *
generate_compiler_generic__LilyCheckedCompilerGeneric(
  const Vec *used_compiler_generic)
{
    if (used_compiler_generic->len > 26) {
        if (used_compiler_generic->len % 26 == 0) {
            return format__String("{S}a", last__Vec(used_compiler_generic));
        } else {
            String *last = last__Vec(used_compiler_generic);

            return format__String("{sa}{c}",
                                  get_slice__String(last, 0, last->len - 1),
                                  used_compiler_generic->len % 26 + a);
        }
    } else {
        return format__String("{c}", a + used_compiler_generic->len);
    }
}
