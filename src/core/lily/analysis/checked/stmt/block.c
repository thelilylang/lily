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

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/stmt/block.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtBlock,
               const LilyCheckedStmtBlock *self)
{
    String *res = from__String("LilyCheckedStmtBlock{ body =");

    DEBUG_VEC_STRING(self->body, res, LilyCheckedBodyFunItem);

    push_str__String(res, ", scope = ");

    {
        String *s = to_string__Debug__LilyCheckedScope(self->scope);

        APPEND_AND_FREE(res, s);
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtBlock, const LilyCheckedStmtBlock *self)
{
    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body);
    FREE(LilyCheckedScope, self->scope);
}
