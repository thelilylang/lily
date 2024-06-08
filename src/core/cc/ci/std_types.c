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

#include <base/assert.h>
#include <base/hash_map.h>

#include <core/cc/ci/parser.h>
#include <core/cc/ci/std_types.h>

CIDataType *
load_std_type__StdTypes(char *define_name)
{
    CIResultFile *builtin = get_ref__CIPreDefined();
    const CIResultDefine *define =
      get_define_from_str__CIResultFile(builtin, define_name);

    ASSERT(define);
    ASSERT(is_empty__CITokens(&define->define->tokens));

    CIParser data_type_parser =
      from_tokens__CIParser((CIResultFile *)builtin, &define->define->tokens);
    CIDataType *data_type = parse_data_type__CIParser(&data_type_parser);

    free_from_tokens_case__CIParser(&data_type_parser);

    return data_type;
}
