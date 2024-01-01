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

#include <base/alloc.h>

#include <core/lily/analysis/checked/inherit_param.h>

CONSTRUCTOR(LilyCheckedInheritParam *,
            LilyCheckedInheritParam,
            LilyCheckedDataType *data_type)
{
    LilyCheckedInheritParam *self =
      lily_malloc(sizeof(LilyCheckedInheritParam));

    self->data_type = data_type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedInheritParam,
               const LilyCheckedInheritParam *self)
{
    return format__String(
      "LilyCheckedInheritParam{{ data_type = {Sr} }",
      to_string__Debug__LilyCheckedDataType(self->data_type));
}
#endif

DESTRUCTOR(LilyCheckedInheritParam, LilyCheckedInheritParam *self)
{
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}
