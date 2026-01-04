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

#include <core/lily/analysis/checked/stmt/variable.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtVariable,
               const LilyCheckedStmtVariable *self)
{
    if (self->data_type) {
        return format__String(
          "LilyCheckedStmtVariable{{ name = {S}, data_type = {Sr}, expr = "
          "{Sr}, "
          "is_mut = {b}, is_dropped = {b}, is_moved = {b} }",
          self->name,
          to_string__Debug__LilyCheckedDataType(self->data_type),
          to_string__Debug__LilyCheckedExpr(self->expr),
          self->is_mut,
          self->is_dropped,
          self->is_moved);
    } else {
        return format__String(
          "LilyCheckedStmtVariable{{ name = {S}, data_type = NULL, expr = "
          "{Sr}, "
          "is_mut = {b}, is_dropped = {b}, is_moved = {b} }",
          self->name,
          to_string__Debug__LilyCheckedExpr(self->expr),
          self->is_mut,
          self->is_dropped,
          self->is_moved);
    }
}
#endif

DESTRUCTOR(LilyCheckedStmtVariable, const LilyCheckedStmtVariable *self)
{
    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    FREE(LilyCheckedExpr, self->expr);
}
