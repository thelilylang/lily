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

#include <base/macros.h>
#include <base/print.h>

#include <core/lily/package/dependency_tree.h>
#include <core/lily/package/package.h>

#include <stdio.h>
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
    self->is_done = false;

    return self;
}

LilyPackageDependencyTree *
determine_tree__LilyPackageDependencyTree(Vec *trees, LilyPackage *package)
{
    for (Usize i = 0; i < package->package_dependencies->len; ++i) {
        LilyPackage *dependency = get__Vec(package->package_dependencies, i);

        for (Usize j = 0; j < trees->len; ++j) {
            LilyPackageDependencyTree *res =
              is_added__LilyPackageDependencyTree(get__Vec(trees, j),
                                                  dependency);

            if (res) {
                return res;
            }
        }
    }

    return NULL;
}

void
add_package__LilyPackageDependencyTree(LilyPackageDependencyTree *self,
                                       Vec *trees,
                                       LilyPackage *package)
{
    if (!is_added__LilyPackageDependencyTree(self, package)) {
        Vec *dependencies = NEW(Vec);

        for (Usize i = 0; i < package->package_dependencies->len; ++i) {
            LilyPackage *dependency_pkg =
              get__Vec(package->package_dependencies, i);
            LilyPackageDependencyTree *dependency = NULL;

        add_dependency: {
            for (Usize i = 0; i < trees->len && !dependency; ++i) {
                dependency = is_added__LilyPackageDependencyTree(
                  get__Vec(trees, i), dependency_pkg);
            }

            if (dependency) {
                push__Vec(dependencies, dependency);
            } else {
                LilyPackageDependencyTree *tree =
                  determine_tree__LilyPackageDependencyTree(trees,
                                                            dependency_pkg);

                if (tree) {
                    add_package__LilyPackageDependencyTree(
                      tree, trees, dependency_pkg);
                    goto add_dependency;
                } else {
                    UNREACHABLE("tree is NULL");
                }
            }
        }
        }

        push__Vec(self->children,
                  NEW(LilyPackageDependencyTree, package, dependencies));
    }
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

LilyPackageDependencyTree *
is_added__LilyPackageDependencyTree(LilyPackageDependencyTree *self,
                                    LilyPackage *package)
{
    // 1. Check if package is found.
    if (!strcmp(self->package->name->buffer, package->name->buffer)) {
        return self;
    }

    // 2. Search package name.
    for (Usize i = 0; i < self->children->len; i++) {
        LilyPackageDependencyTree *res = is_added__LilyPackageDependencyTree(
          get__Vec(self->children, i), package);

        if (res) {
            return res;
        }
    }

    return NULL;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPackageDependencyTree,
               const LilyPackageDependencyTree *self)
{
    String *res = format__String(
      "LilyPackageDependencyTree{{ package.global_name = {S}, children = {{ ",
      self->package->global_name);

    for (Usize i = 0; i < self->children->len; ++i) {
        char *s =
          format("package.global_name = {S}",
                 CAST(LilyPackageDependencyTree *, get__Vec(self->children, i))
                   ->package->global_name);

        PUSH_STR_AND_FREE(res, s);

        if (i + 1 != self->children->len) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, " }");

    push_str__String(res, ", dependencies =");

    if (self->dependencies) {
        push_str__String(res, " {");

        for (Usize i = 0; i < self->children->len; ++i) {
            char *s = format(
              "package.global_name = {S}",
              CAST(LilyPackageDependencyTree *, get__Vec(self->children, i))
                ->package->global_name);

            PUSH_STR_AND_FREE(res, s);

            if (i + 1 != self->children->len) {
                push_str__String(res, ", ");
            }
        }

        push_str__String(res, " }");
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}

void
IMPL_FOR_DEBUG(debug,
               LilyPackageDependencyTree,
               const LilyPackageDependencyTree *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyPackageDependencyTree(self));
}
#endif

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
