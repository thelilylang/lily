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

#include <core/lily/ast/body/fun.h>
#include <core/lily/ast/stmt/if.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstStmtIf, const LilyAstStmtIf *self)
{
    String *res = format__String("LilyAstStmtIf{{ if_expr = {Sr}, if_body =",
                                 to_string__Debug__LilyAstExpr(self->if_expr));

    DEBUG_VEC_STRING(self->if_body, res, LilyAstBodyFunItem);
    push_str__String(res, " }");

    if (self->elif_exprs) {
        push_str__String(res, ", elif_exprs =");
        DEBUG_VEC_STRING(self->elif_exprs, res, LilyAstExpr);

        push_str__String(res, " }, elif_body =");
        DEBUG_VEC_STRING_2(self->elif_bodies, res, LilyAstBodyFunItem);
        push_str__String(res, " }");
    }

    if (self->else_body) {
        push_str__String(res, "else_body =");
        DEBUG_VEC_STRING(self->else_body, res, LilyAstBodyFunItem);
        push_str__String(res, " }");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstStmtIf, const LilyAstStmtIf *self)
{
    FREE(LilyAstExpr, self->if_expr);

    FREE_BUFFER_ITEMS(
      self->if_body->buffer, self->if_body->len, LilyAstBodyFunItem);
    FREE(Vec, self->if_body);

    if (self->elif_exprs) {
        FREE_BUFFER_ITEMS(
          self->elif_exprs->buffer, self->elif_exprs->len, LilyAstExpr);
        FREE(Vec, self->elif_exprs);

        FREE_BUFFER_ITEMS_2(self->elif_bodies->buffer,
                            self->elif_bodies->len,
                            LilyAstBodyFunItem);
        FREE(Vec, self->elif_bodies);
    }

    if (self->else_body) {
        FREE_BUFFER_ITEMS(
          self->else_body->buffer, self->else_body->len, LilyAstBodyFunItem);
        FREE(Vec, self->else_body);
    }
}