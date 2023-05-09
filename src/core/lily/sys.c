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

#include <base/alloc.h>

#include <core/lily/sys.h>

#include <string.h>

LilySysFun *
load_syss__LilySys()
{
    LilySysFun *syss = lily_malloc(sizeof(LilySysFun) * SYSS_COUNT);

    syss[0] = (LilySysFun){
        .name = "write",
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL),
        .params = init__Vec(
          3,
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_STR, NULL),
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL))
    };

    return syss;
}

bool
is_sys_function__LilySys(const char *name)
{
    if (!strcmp(name, "write")) {
        return true;
    }

    return false;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilySysFun, const LilySysFun *self)
{
    String *res = format__String(
      "LilySysFun{{ name = {s}, return_data_type = {Sr}, params =",
      self->name,
      to_string__Debug__LilyCheckedDataType(self->return_data_type));

    DEBUG_VEC_STRING(self->params, res, LilyCheckedDataType);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilySysFun, const LilySysFun *self)
{
    FREE(LilyCheckedDataType, self->return_data_type);
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyCheckedDataType);
    FREE(Vec, self->params);
}
