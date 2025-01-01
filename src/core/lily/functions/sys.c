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

#include <base/alloc.h>

#include <core/lily/functions/sys.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LilySysFun *
load_syss__LilySys()
{
    LilySysFun *syss = lily_malloc(sizeof(LilySysFun) * SYSS_COUNT);

    syss[0] = (LilySysFun){
        .name = "read",
        .real_name = from__String("__sys__$read"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL),
        .params = init__Vec(
          3,
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
          NEW_VARIANT(
            LilyCheckedDataType,
            ptr,
            NULL,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL)),
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL)),
    };

    syss[1] = (LilySysFun){
        .name = "write",
        .real_name = from__String("__sys__$write"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL),
        .params = init__Vec(
          3,
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL),
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL))
    };

    syss[2] = (LilySysFun){
        .name = "open",
        .real_name = from__String("__sys__$open"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
        .params = init__Vec(
          3,
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL),
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL))
    };

    syss[3] = (LilySysFun){
        .name = "close",
        .real_name = from__String("__sys__$close"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL))
    };

    syss[4] = (LilySysFun){
        .name = "stat_mode",
        .real_name = from__String("__sys__$stat_mode"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[5] = (LilySysFun){
        .name = "stat_ino",
        .real_name = from__String("__sys__$stat_ino"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[6] = (LilySysFun){
        .name = "stat_dev",
        .real_name = from__String("__sys__$stat_dev"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[7] = (LilySysFun){
        .name = "stat_nlink",
        .real_name = from__String("__sys__$stat_nlink"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[8] = (LilySysFun){
        .name = "stat_uid",
        .real_name = from__String("__sys__$stat_uid"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[9] = (LilySysFun){
        .name = "stat_gid",
        .real_name = from__String("__sys__$stat_gid"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[10] = (LilySysFun){
        .name = "stat_size",
        .real_name = from__String("__sys__$stat_size"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[11] = (LilySysFun){
        .name = "stat_atime",
        .real_name = from__String("__sys__$stat_atime"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[12] = (LilySysFun){
        .name = "stat_mtime",
        .real_name = from__String("__sys__$stat_mtime"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    syss[13] = (LilySysFun){
        .name = "stat_ctime",
        .real_name = from__String("__sys__$stat_ctime"),
        .return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
        .params = init__Vec(
          1, NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSTR, NULL))
    };

    return syss;
}

bool
is_sys_function__LilySys(const char *name)
{
    if (!strcmp(name, "read") || !strcmp(name, "write") ||
        !strcmp(name, "open") || !strcmp(name, "close") ||
        !strcmp(name, "stat_mode") || !strcmp(name, "stat_ino") ||
        !strcmp(name, "stat_dev") || !strcmp(name, "stat_nlink") ||
        !strcmp(name, "stat_uid") || !strcmp(name, "stat_gid") ||
        !strcmp(name, "stat_size") || !strcmp(name, "stat_atime") ||
        !strcmp(name, "stat_mtime") || !strcmp(name, "stat_ctime")) {
        return true;
    }

    return false;
}

const LilySysFun *
get_sys__LilySys(LilySysFun *syss, const char *name)
{
    for (Usize i = 0; i < SYSS_COUNT; ++i) {
        if (!strcmp(syss[i].name, name)) {
            return &syss[i];
        }
    }

    UNREACHABLE("the sys function you are looking for does not exist");
}

bool
eq__LilySys(const LilySysFun *self, const LilySysFun *other)
{
    if (!(self->params->len == other->params->len &&
          !strcmp(self->name, other->name) &&
          !strcmp(self->real_name->buffer, other->real_name->buffer) &&
          eq__LilyCheckedDataType(self->return_data_type,
                                  other->return_data_type))) {
        return false;
    }

    for (Usize i = 0; i < self->params->len; ++i) {
        if (!eq__LilyCheckedDataType(get__Vec(self->params, i),
                                     get__Vec(other->params, i))) {
            return false;
        }
    }

    return true;
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
    FREE(String, self->real_name);
    FREE(LilyCheckedDataType, self->return_data_type);
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyCheckedDataType);
    FREE(Vec, self->params);
}
