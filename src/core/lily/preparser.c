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

#include <core/lily/preparser.h>

// Advance to the one position and update the current.
static void
next_token__LilyPreparser(LilyPreparser *self);

// Remove an item at the current position and update the current.
static void
eat_token__LilyPreparser(LilyPreparser *self);

void
next_token__LilyPreparser(LilyPreparser *self)
{
    if (self->position + 1 < self->scanner->tokens->len) {
        ++self->position;
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
}

void
eat_token__LilyPreparser(LilyPreparser *self)
{
    FREE(LilyToken, remove__Vec(self->scanner->tokens, self->position));

    if (self->position >= self->scanner->tokens->len) {
        self->current = last__Vec(self->scanner->tokens);
    } else {
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
}

void
run__LilyPreparser(LilyPreparser *self)
{
    while (self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_IMPORT:
                break;
            case LILY_TOKEN_KIND_KEYWORD_MACRO:
                break;
            case LILY_TOKEN_KIND_KEYWORD_PUB:
                break;
            default:
                break;
        }

        next_token__LilyPreparser(self);
    }
}