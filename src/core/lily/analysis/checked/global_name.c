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

#include <base/format.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/global_name.h>

void
generate_global_fun_name_with_vec__LilyCheckedGlobalName(String *global_name,
                                                         const Vec *fun)
{
    String *ser = NEW(String);

    for (Usize i = 0; i < fun->len; ++i) {
        if (is_compiler_defined__LilyCheckedDataType(get__Vec(fun, i))) {
            FREE(String, ser);
            return;
        } else {
            serialize__LilyCheckedDataType(get__Vec(fun, i), ser);
        }
    }

    APPEND_AND_FREE(global_name, ser);
}

void
generate_global_type_name_with_ordered_hash_map__LilyCheckedGlobalName(
  String *global_name,
  const OrderedHashMap *generic_params)
{
    OrderedHashMapIter iter =
      NEW(OrderedHashMapIter, (OrderedHashMap *)generic_params);
    LilyCheckedDataType *current = NULL;

    while ((current = next__OrderedHashMapIter(&iter))) {
        serialize__LilyCheckedDataType(current, global_name);
    }
}
