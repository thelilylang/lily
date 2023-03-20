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

#ifndef LILY_CORE_LILY_PACKAGE_DEPENDENCY_TREE_H
#define LILY_CORE_LILY_PACKAGE_DEPENDENCY_TREE_H

#include <base/vec.h>

typedef struct LilyPackage LilyPackage;

/*
                                Package1
                                /      \
                               /        \
                              /          \
                             /            \
                            /              \
                           /                \
                          /                  \
                         /                    \
                      Package2             Package3
                       /   \                 /   \
                      /     \               /     \
                     /       \             /       \
                Package3  Package4     Package5  Package6
*/

// NOTE: The dependency tree does not deal with the dependency of packages on
// libraries.
typedef struct LilyPackageDependencyTree
{
    LilyPackage *package; // LilyPackage* (&)
    Vec *children;        // Vec<LilyPackageDependencyTree*>*
    Vec *dependencies;    // Vec<LilyPackageDependencyTree* (&)>*?
} LilyPackageDependencyTree;

/**
 *
 * @brief Construct LilyPackageDependencyTree type.
 */
CONSTRUCTOR(LilyPackageDependencyTree *,
            LilyPackageDependencyTree,
            LilyPackage *package,
            Vec *dependencies);

/**
 *
 * @brief Add package to dependency tree.
 * @param file_dependencies Vec<LilyPackage* (&)>*
 */
void
add_package__LilyPackageDependencyTree(LilyPackageDependencyTree *self,
                                       LilyPackage *package,
                                       Vec *file_dependencies);

/**
 *
 * @brief Verify if the package is added in the tree.
 */
bool
is_added__LilyPackageDependencyTree(LilyPackageDependencyTree *self,
                                    LilyPackage *package);

/**
 *
 * @brief Free LilyPackageDependencyTree type.
 */
DESTRUCTOR(LilyPackageDependencyTree, LilyPackageDependencyTree *self);

#endif // LILY_CORE_LILY_PACKAGE_DEPENDENCY_TREE_H
