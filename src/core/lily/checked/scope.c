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
#include <base/new.h>

#include <core/lily/checked/scope.h>

CONSTRUCTOR(LilyCheckedScope *,
            LilyCheckedScope,
            enum LilyCheckedScopeKind kind,
            LilyCheckedScopeId id,
            Vec *access,
            Vec *children,
			enum LilyVisibility visibility)
{
    LilyCheckedScope *self = lily_malloc(sizeof(LilyCheckedScope));

    self->kind = kind;
    self->id = id;
    self->access = access;
    self->children = children;
	self->visibility = visibility;

    return self;
}

DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self)
{
    FREE_BUFFER_ITEMS(self->access->buffer, self->access->len, String);
    FREE(Vec, self->access);

    if (self->children) {
        FREE_BUFFER_ITEMS(
          self->children->buffer, self->children->len, LilyCheckedScope);
        FREE(Vec, self->children);
    }

    lily_free(self);
}
