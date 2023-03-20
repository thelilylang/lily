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

#include <base/macros.h>

#include <core/lily/package/dependency_tree.h>
#include <core/lily/package/package.h>

#include <string.h>

/// @return Vec<LilyPackageDependencyTree* (&)>*?
static Vec *
collect_dependencies__LilyPackageDependencyTree(
  LilyPackageDependencyTree *self,
  LilyPackage *package,
  LilyPackageDependencyTree *previous);

CONSTRUCTOR(LilyPackageDependencyTree *,
            LilyPackageDependencyTree,
            LilyPackage *package,
            Vec *dependencies)
{
    LilyPackageDependencyTree *self =
      lily_malloc(sizeof(LilyPackageDependencyTree));

    self->package = package;
    self->children = NEW(Vec);
    self->dependencies = dependencies;

    return self;
}

void
add_package__LilyPackageDependencyTree(LilyPackageDependencyTree *self,
                                       LilyPackage *package,
                                       Vec *file_dependencies)
{
    // TODO: add package
}

Vec *
collect_dependencies__LilyPackageDependencyTree(
  LilyPackageDependencyTree *self,
  LilyPackage *package,
  LilyPackageDependencyTree *previous)
{
    // 1. Check if package is found.
    if (!strcmp(self->package->name->buffer, package->name->buffer)) {
        Vec *dependencies = NEW(Vec);

        append__Vec(dependencies, previous->dependencies);
        push__Vec(dependencies, previous);

        return dependencies;
    }

    // 2. Search package name.
    for (Usize i = 0; i < self->children->len; i++) {
        Vec *dependencies = collect_dependencies__LilyPackageDependencyTree(
          get__Vec(self->children, i), package, self);

        if (dependencies) {
            return dependencies;
        }
    }

    return NULL;
}

bool
is_added__LilyPackageDependencyTree(LilyPackageDependencyTree *self,
                                    LilyPackage *package)
{
    // 1. Check if package is found.
    if (!strcmp(self->package->name->buffer, package->name->buffer)) {
        return true;
    }

    // 2. Search package name.
    for (Usize i = 0; i < self->children->len; i++) {
        bool res = is_added__LilyPackageDependencyTree(
          get__Vec(self->children, i), package);

        if (res) {
            return true;
        }
    }

    return false;
}

DESTRUCTOR(LilyPackageDependencyTree, LilyPackageDependencyTree *self)
{
    FREE_BUFFER_ITEMS(
      self->children->buffer, self->children->len, LilyPackageDependencyTree);
    FREE(Vec, self->children);

    if (self->dependencies) {
        FREE(Vec, self->dependencies);
    }

    lily_free(self);
}
