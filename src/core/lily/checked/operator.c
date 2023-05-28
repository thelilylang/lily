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

#include <core/lily/checked/operator.h>

#include <string.h>

CONSTRUCTOR(LilyCheckedOperator *,
            LilyCheckedOperator,
            String *name,
            Vec *signature)
{
    LilyCheckedOperator *self = lily_malloc(sizeof(LilyCheckedOperator));

    self->name = name;
    self->signature = signature;
    self->ref_count = 0;

    return self;
}

bool
valid_operator__LilyCheckedOperator(String *name)
{
    if (!strcmp(name->buffer, "+") || !strcmp(name->buffer, "and") ||
        !strcmp(name->buffer, "+=") || !strcmp(name->buffer, "&=") ||
        !strcmp(name->buffer, "<<=") || !strcmp(name->buffer, "|=") ||
        !strcmp(name->buffer, ">>=") || !strcmp(name->buffer, "/=") ||
        !strcmp(name->buffer, "**=") || !strcmp(name->buffer, "**=") ||
        !strcmp(name->buffer, "%=") || !strcmp(name->buffer, "*=") ||
        !strcmp(name->buffer, "-=") || !strcmp(name->buffer, "xor=") ||
        !strcmp(name->buffer, "=") || !strcmp(name->buffer, "&") ||
        !strcmp(name->buffer, "|") || !strcmp(name->buffer, "/") ||
        !strcmp(name->buffer, "==") || !strcmp(name->buffer, "**") ||
        !strcmp(name->buffer, ">=") || !strcmp(name->buffer, ">") ||
        !strcmp(name->buffer, "<<") || !strcmp(name->buffer, "<=") ||
        !strcmp(name->buffer, "<") || !strcmp(name->buffer, "%") ||
        !strcmp(name->buffer, "*") || !strcmp(name->buffer, "not=") ||
        !strcmp(name->buffer, "or") || !strcmp(name->buffer, ">>") ||
        !strcmp(name->buffer, ">>") || !strcmp(name->buffer, "..") ||
        !strcmp(name->buffer, "-") || !strcmp(name->buffer, "xor") ||
        !strcmp(name->buffer, "->") || !strcmp(name->buffer, "<-") ||
        !strcmp(name->buffer, "[n]") || !strcmp(name->buffer, "[n..n]") ||
        !strcmp(name->buffer, "[n..]") || !strcmp(name->buffer, "[..n]")) {
        return true;
    }

    return false;
}
