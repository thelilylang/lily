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

#include <base/dir.h>

#include <cli/emit.h>

#include <core/lily/lily.h>
#include <core/lily/package/default_path.h>
#include <core/lily/package/package.h>
#include <core/lily/precompiler/precompiler.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENV_DEBUG
#include <base/print.h>
#endif

// NOTE: This define will be deleted when all thread operations are safe.
#define PRECOMPILER_USE_MULTITHREAD
#undef PRECOMPILER_USE_MULTITHREAD

#ifdef PRECOMPILER_USE_MULTITHREAD
#include <pthread.h>

typedef struct LilyPrecompilerSubPackageWrapper
{
    const LilyPrecompiler *self; // const LilyPrecompiler* (&)
    const LilyPreparserSubPackage
      *sub_package;            // const LilyPreparserSubPackage* (&)
    LilyPackage *root_package; // LilyPackage* (&)
    LilyPackage *res;          // LilyPackage* (&)
    String *pkg_filename;
    char *default_path; // char*?
} LilyPrecompilerSubPackageWrapper;

// Construct LilyPrecompilerSubPackageWrapper type.
static CONSTRUCTOR(LilyPrecompilerSubPackageWrapper *,
                   LilyPrecompilerSubPackageWrapper,
                   LilyPrecompiler *self,
                   const LilyPreparserSubPackage *sub_package,
                   LilyPackage *root_package);

// Free LilyPrecompilerSubPackageWrapper type.
static DESTRUCTOR(LilyPrecompilerSubPackageWrapper,
                  LilyPrecompilerSubPackageWrapper *self);
#endif

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_ACCESS).
static VARIANT_DESTRUCTOR(LilyImportValue, access, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_FILE).
static VARIANT_DESTRUCTOR(LilyImportValue, file, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_LIBRARY).
static VARIANT_DESTRUCTOR(LilyImportValue, library, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_PACKAGE).
static VARIANT_DESTRUCTOR(LilyImportValue, package, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_SELECT).
static VARIANT_DESTRUCTOR(LilyImportValue, select, LilyImportValue *self);

// Check for some errors in the import (e.g. recursive import).
static void
check_import_to_build_dependency_tree__LilyPrecompiler(
  LilyPrecompiler *self,
  LilyPackage *package,
  LilyPackage *root_package);

/// @return Vec<LilyPackage* (&)>*
static Vec *
collect_all_packages_to_build_dependency_tree__LilyPrecompiler(
  LilyPackage *package,
  Vec *dependencies_order);

static void
check_deps__LilyPrecompiler(LilyPackage *package,
                            String *rejected_package_name);

// Check recursive import.
// e.g.:
// <Package name>: <Dependencies>
// A: [B, C]
// B: [A, C]
// C: []
// This case would find a recursive import.
static void
check_for_recursive_import_to_build_dependency_tree__LilyPrecompiler(
  Vec *dependencies_order);

// Sort dependencies_order by package->dependencies len.
static void
calculate_dependencies_order_to_build_dependency_tree__LilyPrecompiler(
  Vec *dependencies_order);

// Return true if the package is found otherwise return false.
static bool
package_is_find_in_dependencies_order__LilyPrecompiler(Vec *dependencies_order,
                                                       LilyPackage *package);

static void
build_tree__LilyPrecompiler(LilyPrecompiler *self, LilyPackage *package);

/// @param self Is the root LilyPrecompiler.
static void
build_dependency_tree__LilyPrecompiler(LilyPrecompiler *self,
                                       LilyPackage *package,
                                       LilyPackage *root_package);

// Transform the import access value (String) to LilyImportValue.
/// @return Vec<LilyImportValue>
static Vec *
precompile_import_access__LilyPrecompiler(LilyPrecompiler *self,
                                          const String *import_value,
                                          const Location *location,
                                          Usize *position);

// Check import value (syntax of import value).
static LilyImport *
precompile_import__LilyPrecompiler(LilyPrecompiler *self,
                                   const LilyPreparserImport *import);

#ifdef PRECOMPILER_USE_MULTITHREAD
static void
configure_sub_package__LilyPrecompiler(
  LilyPrecompilerSubPackageWrapper *wrapper);

/// @return NULL
static void *
precompile_sub_package__LilyPrecompiler(void *w);
#else
/// @return LilyPackage*
static LilyPackage *
precompile_sub_package__LilyPrecompiler(const LilyPrecompiler *self,
                                        const LilyPreparserSubPackage *sub_pkg,
                                        LilyPackage *root_package);
#endif

// Parse and check the parameters of the macros.
static LilyMacro *
precompile_macro__LilyPrecompiler(LilyPrecompiler *self,
                                  const LilyPreparserMacro *macro);

// Add the public macros in the root package and see if the names are not
// duplicated. Plus it does the same process for private macros.
static void
check_macros__LilyPrecompiler(LilyPrecompiler *self, LilyPackage *root_package);

#ifdef PRECOMPILER_USE_MULTITHREAD
static threadlocal pthread_mutex_t sub_package_thread_mutex;
#endif

#ifdef PRECOMPILER_USE_MULTITHREAD
CONSTRUCTOR(LilyPrecompilerSubPackageWrapper *,
            LilyPrecompilerSubPackageWrapper,
            LilyPrecompiler *self,
            const LilyPreparserSubPackage *sub_package,
            LilyPackage *root_package)
{
    LilyPrecompilerSubPackageWrapper *w =
      lily_malloc(sizeof(LilyPrecompilerSubPackageWrapper));

    w->self = self;
    w->sub_package = sub_package;
    w->root_package = root_package;
    w->pkg_filename = NULL;
    w->default_path = NULL;

    return w;
}

DESTRUCTOR(LilyPrecompilerSubPackageWrapper,
           LilyPrecompilerSubPackageWrapper *self)
{
    if (self->default_path) {
        lily_free(self->default_path);
    }

    lily_free(self->pkg_filename);
    lily_free(self);
}
#endif

CONSTRUCTOR(LilyImportValue *, LilyImportValue, enum LilyImportValueKind kind)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, access, String *access)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_ACCESS;
    self->access = access;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, file, String *file)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_FILE;
    self->file = file;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *,
                    LilyImportValue,
                    library,
                    String *library)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_LIBRARY;
    self->library = library;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *,
                    LilyImportValue,
                    package,
                    String *package)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_PACKAGE;
    self->package = package;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, select, Vec *select)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_SELECT;
    self->select = select;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyImportValueKind, enum LilyImportValueKind self)
{
    switch (self) {
        case LILY_IMPORT_VALUE_KIND_ACCESS:
            return "LILY_IMPORT_VALUE_KIND_ACCESS";
        case LILY_IMPORT_VALUE_KIND_BUILTIN:
            return "LILY_IMPORT_VALUE_KIND_BUILTIN";
        case LILY_IMPORT_VALUE_KIND_CORE:
            return "LILY_IMPORT_VALUE_KIND_CORE";
        case LILY_IMPORT_VALUE_KIND_FILE:
            return "LILY_IMPORT_VALUE_KIND_FILE";
        case LILY_IMPORT_VALUE_KIND_LIBRARY:
            return "LILY_IMPORT_VALUE_KIND_LIBRARY";
        case LILY_IMPORT_VALUE_KIND_PACKAGE:
            return "LILY_IMPORT_VALUE_KIND_PACKAGE";
        case LILY_IMPORT_VALUE_KIND_SELECT_ALL:
            return "LILY_IMPORT_VALUE_KIND_SELECT_ALL";
        case LILY_IMPORT_VALUE_KIND_SELECT:
            return "LILY_IMPORT_VALUE_KIND_SELECT";
        case LILY_IMPORT_VALUE_KIND_STD:
            return "LILY_IMPORT_VALUE_KIND_STD";
        case LILY_IMPORT_VALUE_KIND_SYS:
            return "LILY_IMPORT_VALUE_KIND_SYS";
        case LILY_IMPORT_VALUE_KIND_URL:
            return "LILY_IMPORT_VALUE_KIND_URL";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyImportValue, const LilyImportValue *self)
{
    switch (self->kind) {
        case LILY_IMPORT_VALUE_KIND_ACCESS:
            return format__String(
              "LilyImportValue{{ kind = {s}, access = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->access);
        case LILY_IMPORT_VALUE_KIND_FILE:
            return format__String(
              "LilyImportValue{{ kind = {s}, file = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->file);
        case LILY_IMPORT_VALUE_KIND_LIBRARY:
            return format__String(
              "LilyImportValue{{ kind = {s}, library = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->library);
        case LILY_IMPORT_VALUE_KIND_PACKAGE:
            return format__String(
              "LilyImportValue{{ kind = {s}, package = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->package);
        case LILY_IMPORT_VALUE_KIND_SELECT: {
            String *res =
              format__String("LilyImportValue{{ kind = {s}, select = {{ ",
                             to_string__Debug__LilyImportValueKind(self->kind));

            for (Usize i = 0; i < self->select->len; ++i) {
                Vec *item = get__Vec(self->select, i);

                push_str__String(res, " { ");

                for (Usize j = 0; j < item->len; ++j) {
                    String *s =
                      to_string__Debug__LilyImportValue(get__Vec(item, j));

                    APPEND_AND_FREE(res, s);

                    if (j != item->len - 1) {
                        push_str__String(res, ", ");
                    }
                }

                push_str__String(res, " } ");

                if (i != self->select->len - 1) {
                    push_str__String(res, ", ");
                }
            }

            push_str__String(res, " }");

            return res;
        }
        default:
            return format__String(
              "LilyImportValue{{ kind = {s} }",
              to_string__Debug__LilyImportValueKind(self->kind));
    }
}

void
IMPL_FOR_DEBUG(debug, LilyImportValue, const LilyImportValue *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyImportValue(self));
}
#endif

VARIANT_DESTRUCTOR(LilyImportValue, access, LilyImportValue *self)
{
    FREE(String, self->access);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyImportValue, file, LilyImportValue *self)
{
    FREE(String, self->file);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyImportValue, library, LilyImportValue *self)
{
    FREE(String, self->library);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyImportValue, package, LilyImportValue *self)
{
    FREE(String, self->package);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyImportValue, select, LilyImportValue *self)
{
    for (Usize i = 0; i < self->select->len; ++i) {
        Vec *item = self->select->buffer[i];

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyImportValue);
        FREE(Vec, item);
    }

    FREE(Vec, self->select);
    lily_free(self);
}

DESTRUCTOR(LilyImportValue, LilyImportValue *self)
{
    switch (self->kind) {
        case LILY_IMPORT_VALUE_KIND_ACCESS:
            FREE_VARIANT(LilyImportValue, access, self);
            break;
        case LILY_IMPORT_VALUE_KIND_FILE:
            FREE_VARIANT(LilyImportValue, file, self);
            break;
        case LILY_IMPORT_VALUE_KIND_LIBRARY:
            FREE_VARIANT(LilyImportValue, library, self);
            break;
        case LILY_IMPORT_VALUE_KIND_PACKAGE:
            FREE_VARIANT(LilyImportValue, package, self);
            break;
        case LILY_IMPORT_VALUE_KIND_SELECT:
            FREE_VARIANT(LilyImportValue, select, self);
            break;
        default:
            lily_free(self);
    }
}

CONSTRUCTOR(LilyImport *,
            LilyImport,
            Vec *values,
            Location location,
            String *as)
{
    LilyImport *self = lily_malloc(sizeof(LilyImport));

    self->values = values;
    self->location = location;
    self->as = as;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyImport, const LilyImport *self)
{
    String *res = from__String("LilyImport{{ values = {{ ");

    for (Usize i = 0; i < self->values->len; ++i) {
        String *s =
          to_string__Debug__LilyImportValue(get__Vec(self->values, i));

        APPEND_AND_FREE(res, s);

        if (i != self->values->len - 1) {
            push_str__String(res, ", ");
        }
    }

    if (self->as) {
        char *s = format("}, location = {sa}, as = {S} }",
                         to_string__Debug__Location(&self->location),
                         self->as);

        PUSH_STR_AND_FREE(res, s);
    } else {
        char *s = format("}, location = {sa} }",
                         to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}

void
IMPL_FOR_DEBUG(debug, LilyImport, const LilyImport *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyImport(self));
}
#endif

DESTRUCTOR(LilyImport, LilyImport *self)
{
    FREE_BUFFER_ITEMS(self->values->buffer, self->values->len, LilyImportValue);
    FREE(Vec, self->values);

    lily_free(self);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyMacroParamKind, enum LilyMacroParamKind self)
{
    switch (self) {
        case LILY_MACRO_PARAM_KIND_ID:
            return "LILY_MACRO_PARAM_KIND_ID";
        case LILY_MACRO_PARAM_KIND_DT:
            return "LILY_MACRO_PARAM_KIND_DT";
        case LILY_MACRO_PARAM_KIND_TK:
            return "LILY_MACRO_PARAM_KIND_TK";
        case LILY_MACRO_PARAM_KIND_TKS:
            return "LILY_MACRO_PARAM_KIND_TKS";
        case LILY_MACRO_PARAM_KIND_STMT:
            return "LILY_MACRO_PARAM_KIND_STMT";
        case LILY_MACRO_PARAM_KIND_EXPR:
            return "LILY_MACRO_PARAM_KIND_EXPR";
        case LILY_MACRO_PARAM_KIND_PATH:
            return "LILY_MACRO_PARAM_KIND_PATH";
        case LILY_MACRO_PARAM_KIND_PATT:
            return "LILY_MACRO_PARAM_KIND_PATT";
        case LILY_MACRO_PARAM_KIND_BLOCK:
            return "LILY_MACRO_PARAM_KIND_BLOCK";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(LilyMacroParam *,
            LilyMacroParam,
            enum LilyMacroParamKind kind,
            String *name,
            Location location)
{
    LilyMacroParam *self = lily_malloc(sizeof(LilyMacroParam));

    self->kind = kind;
    self->name = name;
    self->location = location;
    self->is_used = true;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyMacroParam, const LilyMacroParam *self)
{
    return format("LilyMacroParam{{ kind = {s}, name = {S}, location = {sa}, "
                  "is_used = {b} }",
                  to_string__Debug__LilyMacroParamKind(self->kind),
                  self->name,
                  to_string__Debug__Location(&self->location),
                  self->is_used);
}
#endif

DESTRUCTOR(LilyMacroParam, LilyMacroParam *self)
{
    FREE(String, self->name);
    lily_free(self);
}

CONSTRUCTOR(LilyMacro *,
            LilyMacro,
            String *name,
            Vec *params,
            Vec *tokens,
            Location location)
{
    LilyMacro *self = lily_malloc(sizeof(LilyMacro));

    self->name = name;
    self->params = params;
    self->tokens = tokens;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyMacro, const LilyMacro *self)
{
    String *res =
      format__String("LilyMacro{{ name = {S}, params =", self->name);

    if (self->params) {
        DEBUG_VEC_STR(self->params, res, LilyMacroParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", tokens =");
    DEBUG_VEC_STR(self->tokens, res, LilyToken);

    {
        char *s = format(", location = {sa} }",
                         to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}

void
IMPL_FOR_DEBUG(debug, LilyMacro, const LilyMacro *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyMacro(self));
}
#endif

DESTRUCTOR(LilyMacro, LilyMacro *self)
{
    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyMacroParam);
        FREE(Vec, self->params);
    }

    lily_free(self);
}

void
check_import_to_build_dependency_tree__LilyPrecompiler(
  LilyPrecompiler *self,
  LilyPackage *package,
  LilyPackage *root_package)
{
    for (Usize i = 0; i < package->private_imports->len; ++i) {
        LilyImport *import = get__Vec(package->private_imports, i);
        LilyImportValue *value = get__Vec(import->values, 0);

        switch (value->kind) {
            case LILY_IMPORT_VALUE_KIND_PACKAGE: {
                // Check for self import.
                if (!strcmp(value->package->buffer, package->name->buffer)) {
                    emit__Diagnostic(
                      NEW_VARIANT(Diagnostic,
                                  simple_lily_error,
                                  (&package->file),
                                  &import->location,
                                  NEW(LilyError, LILY_ERROR_KIND_SELF_IMPORT),
                                  NULL,
                                  NULL,
                                  NULL),
                      &self->count_error);

                    return;
                }

                LilyPackage *pkg = search_package_from_name__LilyPackage(
                  root_package, value->package);

                if (pkg) {
                    push__Vec(package->package_dependencies, pkg);
                } else {
                    UNREACHABLE(
                      "it is impossible that the package is not found");
                }

                break;
            }
            default:
                break;
        }
    }

    for (Usize i = 0; i < package->sub_packages->len; ++i) {
        check_import_to_build_dependency_tree__LilyPrecompiler(
          self, get__Vec(package->sub_packages, i), root_package);
    }
}

Vec *
collect_all_packages_to_build_dependency_tree__LilyPrecompiler(
  LilyPackage *package,
  Vec *dependencies_order)
{
    push__Vec(dependencies_order, package);

    for (Usize i = 0; i < package->sub_packages->len; ++i) {
        dependencies_order =
          collect_all_packages_to_build_dependency_tree__LilyPrecompiler(
            get__Vec(package->sub_packages, i), dependencies_order);
    }

    return dependencies_order;
}

void
check_deps__LilyPrecompiler(LilyPackage *package, String *rejected_package_name)
{
    if (!strcmp(package->name->buffer, rejected_package_name->buffer)) {
        Location diagnostic_location = default__Location(package->file.name);

        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      (&package->file),
                      &diagnostic_location,
                      NEW(LilyError, LILY_ERROR_KIND_RECURSIVE_IMPORT),
                      NULL,
                      NULL,
                      NULL),
          &package->count_error);

        return;
    }

    for (Usize i = 0; i < package->package_dependencies->len; ++i) {
        check_deps__LilyPrecompiler(get__Vec(package->package_dependencies, i),
                                    rejected_package_name);
    }
}

void
check_for_recursive_import_to_build_dependency_tree__LilyPrecompiler(
  Vec *dependencies_order)
{
    for (Usize i = 0; i < dependencies_order->len; ++i) {
        LilyPackage *pkg = get__Vec(dependencies_order, i);

        for (Usize j = 0; j < pkg->package_dependencies->len; ++j) {
            check_deps__LilyPrecompiler(get__Vec(pkg->package_dependencies, j),
                                        pkg->name);
        }
    }
}

void
calculate_dependencies_order_to_build_dependency_tree__LilyPrecompiler(
  Vec *dependencies_order)
{
    for (Usize i = 0; i < dependencies_order->len; ++i) {
        for (Usize j = i + 1; j < dependencies_order->len; ++j) {
            if (CAST(LilyPackage *, get__Vec(dependencies_order, i))
                  ->package_dependencies->len >
                CAST(LilyPackage *, get__Vec(dependencies_order, j))
                  ->package_dependencies->len) {
                SWAP(dependencies_order->buffer[i],
                     dependencies_order->buffer[j]);
            }
        }
    }
}

bool
package_is_find_in_dependencies_order__LilyPrecompiler(Vec *dependencies_order,
                                                       LilyPackage *package)
{
    for (Usize i = 0; i < dependencies_order->len; ++i) {
        if (!strcmp(CAST(LilyPackage *, get__Vec(dependencies_order, i))
                      ->global_name->buffer,
                    package->global_name->buffer)) {
            return true;
        }
    }

    return false;
}

void
build_tree__LilyPrecompiler(LilyPrecompiler *self, LilyPackage *package)
{
    LilyPackageDependencyTree *tree = determine_tree__LilyPackageDependencyTree(
      self->dependency_trees, package);

    add_package__LilyPackageDependencyTree(
      tree, self->dependency_trees, package);

    for (Usize i = 0; i < package->package_dependencies->len; ++i) {
        LilyPackage *p = get__Vec(package->package_dependencies, i);

        if (p->package_dependencies->len > 0) {
            build_tree__LilyPrecompiler(self, p);
        }
    }
}

// 1. Check import (only with `@package`).
// 2. Collect all packages dependencies.
// 3. Check for recursive import.
// 4. Calculate dependencies.
// 5. Add package with fewer dependencies.
void
build_dependency_tree__LilyPrecompiler(LilyPrecompiler *self,
                                       LilyPackage *package,
                                       LilyPackage *root_package)
{
    // 1. Check import.
    check_import_to_build_dependency_tree__LilyPrecompiler(
      self, package, root_package);

    // 2. Collect all packages dependencies.
    Vec *dependencies_order =
      collect_all_packages_to_build_dependency_tree__LilyPrecompiler(
        package,
        NEW(Vec)); // Vec<LilyPackage* (&)>*

    // 3. Check for recursive import.
    check_for_recursive_import_to_build_dependency_tree__LilyPrecompiler(
      dependencies_order);

#ifdef ENV_DEBUG
    PRINTLN("\n====Precompiler dependencies order before sort({S})====\n",
            root_package->name);

    for (Usize i = 0; i < dependencies_order->len; ++i) {
        LilyPackage *pkg = get__Vec(dependencies_order, i);

        PRINTLN("package name: {S}, global_name: {S}, dependencies len: {d}",
                pkg->name,
                pkg->global_name,
                pkg->package_dependencies->len);
    }
#endif

    // 4. Calculate dependencies.
    calculate_dependencies_order_to_build_dependency_tree__LilyPrecompiler(
      dependencies_order);

#ifdef ENV_DEBUG
    PRINTLN("\n====Precompiler dependencies order({S})====\n",
            root_package->name);

    for (Usize i = 0; i < dependencies_order->len; ++i) {
        LilyPackage *pkg = get__Vec(dependencies_order, i);

        PRINTLN("package name: {S}, global_name: {S}, dependencies len: {d}",
                pkg->name,
                pkg->global_name,
                pkg->package_dependencies->len);
    }
#endif

    // 4. Add package with less dependencies.

    // 4.1 Push all packages with no package dependencies.
    for (Usize i = 0; i < dependencies_order->len;) {
        if (CAST(LilyPackage *, get__Vec(dependencies_order, i))
              ->package_dependencies->len == 0) {
            push__Vec(self->dependency_trees,
                      NEW(LilyPackageDependencyTree,
                          get__Vec(dependencies_order, i),
                          NULL));
            remove__Vec(dependencies_order, i);
        } else {
            ++i;
        }
    }

    // 4.2 Push all other packages.
    for (Usize i = 0; i < dependencies_order->len; ++i) {
        build_tree__LilyPrecompiler(self, get__Vec(dependencies_order, i));
    }

    FREE(Vec, dependencies_order);
}

Vec *
precompile_import_access__LilyPrecompiler(LilyPrecompiler *self,
                                          const String *import_value,
                                          const Location *location,
                                          Usize *position)
{
    Vec *values = NEW(Vec);

    while (*position < import_value->len) {
        if (isalpha(import_value->buffer[*position]) ||
            import_value->buffer[*position] == '_') {
            String *access = NEW(String);

            while ((isalpha(import_value->buffer[*position]) ||
                    import_value->buffer[*position] == '_') &&
                   import_value->buffer[*position] != '.' &&
                   import_value->buffer[*position]) {
                push__String(access, import_value->buffer[*position]);
                *position += 1;
            }

            push__Vec(values, NEW_VARIANT(LilyImportValue, access, access));
        } else if (import_value->buffer[*position] == '{') {
            *position += 1;

            Vec *selector_res = NEW(Vec); // Vec<Vec<LilyImportValue*>*>*
            Usize selector_position = 0;

            while (import_value->buffer[*position] != '}' &&
                   import_value->buffer[*position]) {
                String *selector_value = NEW(String);

                while ((import_value->buffer[*position] != '}' &&
                        import_value->buffer[*position] != ',') &&
                       import_value->buffer[*position]) {
                    push__String(selector_value,
                                 import_value->buffer[*position]);
                    *position += 1;
                }

                if (!import_value->buffer[*position]) {
                    goto expected_r_brace;
                }

                if (import_value->buffer[*position] != '}') {
                    *position += 1;
                }

                Vec *selector_item = precompile_import_access__LilyPrecompiler(
                  self, selector_value, location, &selector_position);

                if (selector_item && selector_item->len > 0) {
                    push__Vec(selector_res, selector_item);
                    FREE(String, selector_value);
                } else if (selector_item) {
                    FREE(Vec, selector_item);
                    FREE(String, selector_value);
                } else {
                    FREE(String, selector_value);

                    return NULL;
                }
            }

            if (!import_value->buffer[*position]) {
                goto expected_r_brace;
            } else {
                *position += 1;
                goto select_success_way;
            }

        expected_r_brace: {
            if (!import_value->buffer[*position]) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                    NULL,
                    NULL,
                    from__String("expected `}` to close the selector")),
                  &self->count_error);

                return NULL;
            }
        }

        select_success_way: {
            push__Vec(values,
                      NEW_VARIANT(LilyImportValue, select, selector_res));
        }

        } else if (import_value->buffer[*position] == '.' &&
                   import_value->buffer[*position + 1] == '*') {
            *position += 2;
            push__Vec(values,
                      NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_SELECT_ALL));
        } else if (import_value->buffer[*position] == '.') {
            *position += 1;
        } else if (isspace(import_value->buffer[*position])) {
            while (isspace(import_value->buffer[*position])) {
                *position += 1;
            }

            continue;
        } else {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                location,
                NEW(LilyError,
                    LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                NULL,
                NULL,
                NULL),
              &self->count_error);

            FREE_BUFFER_ITEMS(values->buffer, values->len, LilyImportValue);
            FREE(Vec, values);

            return NULL;
        }
    }

    return values;
}

LilyImport *
precompile_import__LilyPrecompiler(LilyPrecompiler *self,
                                   const LilyPreparserImport *import)
{
    Vec *values = NEW(Vec);
    Usize position = 0;

    switch (get__String(import->value, position++)) {
        case '@':
            break;
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &import->location,
                          NEW(LilyError, LILY_ERROR_KIND_BAD_IMPORT_VALUE),
                          NULL,
                          NULL,
                          from__String("import value must be start by `@`")),
              &self->count_error);

            return NULL;
        }
    }

    {
        String *name = NEW(String);

        while (import->value->buffer[position] != '(' &&
               import->value->buffer[position] != '.' &&
               import->value->buffer[position]) {
            push__String(name, import->value->buffer[position++]);
        }

        if (!strcmp(name->buffer, "builtin")) {
            self->package->builtin_is_loaded = true;

            push__Vec(values,
                      NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_BUILTIN));
        } else if (!strcmp(name->buffer, "core")) {
            self->package->core_is_loaded = true;

            push__Vec(values,
                      NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_CORE));
        } else if (!strcmp(name->buffer, "std")) {
            self->package->std_is_loaded = true;

            push__Vec(values, NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_STD));
        } else if (!strcmp(name->buffer, "sys")) {
            self->package->sys_is_loaded = true;

            if (position != import->value->len) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &import->location,
                    NEW(
                      LilyError,
                      LILY_ERROR_KIND_PATH_IS_NOT_EXPECTED_AFTER_SYS_IMPORT_FLAG),
                    NULL,
                    NULL,
                    NULL),
                  &self->count_error);
            }

            push__Vec(values, NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_SYS));
        } else if (strcmp(name->buffer, "file") != 0 &&
                   strcmp(name->buffer, "library") != 0 &&
                   strcmp(name->buffer, "package") != 0) {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &import->location,
                NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_IMPORT_AT_FLAG),
                init__Vec(
                  1,
                  from__String("expected `@builtin`, `@core`, `@file`, "
                               "`@library`, `@package`, `@sys`, `@std`")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, name);
            FREE(Vec, values);

            return NULL;
        } else {
            if (!import->value->buffer[position++]) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &import->location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                    NULL,
                    NULL,
                    from__String(
                      "expected `(` after `@file`, `@library` or `@package`")),
                  &self->count_error);

                FREE(Vec, values);
                FREE(String, name);

                return NULL;
            }

            String *flag_value = NEW(String);

            while (import->value->buffer[position] != ')' &&
                   import->value->buffer[position]) {
                push__String(flag_value, import->value->buffer[position++]);
            }

            if (!import->value->buffer[position++]) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &import->location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                    NULL,
                    NULL,
                    from__String(
                      "expected `)` after @file, @library, @package value")),
                  &self->count_error);

                FREE(String, flag_value);
                FREE(String, name);
                FREE(Vec, values);

                return NULL;
            }

            if (!strcmp(name->buffer, "file")) {
                push__Vec(values,
                          NEW_VARIANT(LilyImportValue, file, flag_value));
            } else if (!strcmp(name->buffer, "library")) {
                push__Vec(values,
                          NEW_VARIANT(LilyImportValue, library, flag_value));
            } else {
                push__Vec(values,
                          NEW_VARIANT(LilyImportValue, package, flag_value));
            }
        }

        FREE(String, name);
    }

    if (position < import->value->len) {
        String *rest_import_value = take_slice__String(import->value, position);
        Usize position_import_access = 0;

        Vec *access = precompile_import_access__LilyPrecompiler(
          self, rest_import_value, &import->location, &position_import_access);

        if (access) {
            append__Vec(values, access);

            FREE(Vec, access);
        } else {
            FREE(String, rest_import_value);

            return NULL;
        }

        FREE(String, rest_import_value);
    }

    return NEW(LilyImport, values, import->location, import->as);
}

#ifdef PRECOMPILER_USE_MULTITHREAD
void
configure_sub_package__LilyPrecompiler(
  LilyPrecompilerSubPackageWrapper *wrapper)
{
#define INIT_IR()                                                        \
    switch (wrapper->root_package->compiler.ir.kind) {                   \
        case LILY_IR_KIND_CC:                                            \
            /* TODO: add a linker for CC */                              \
            res->compiler.ir = NEW_VARIANT(LilyIr, cc, NEW(LilyIrCc));   \
            res->compiler.linker = LILY_LINKER_KIND_CC;                  \
            break;                                                       \
        case LILY_IR_KIND_CPP:                                           \
            /* TODO: add a linker for CPP */                             \
            res->compiler.ir = NEW_VARIANT(LilyIr, cpp, NEW(LilyIrCpp)); \
            res->compiler.linker = LILY_LINKER_KIND_CPP;                 \
            break;                                                       \
        case LILY_IR_KIND_JS:                                            \
            res->compiler.ir = NEW_VARIANT(LilyIr, js, NEW(LilyIrJs));   \
            break;                                                       \
        case LILY_IR_KIND_LLVM:                                          \
            res->compiler.ir = NEW_VARIANT(                              \
              LilyIr, llvm, NEW(LilyIrLlvm, res->global_name->buffer));  \
            res->compiler.linker = LILY_LINKER_KIND_LLVM;                \
            break;                                                       \
        default:                                                         \
            UNREACHABLE("unknown variant");                              \
    }

    Vec *split_pkg_name = split__String(wrapper->sub_package->name, '.');

#ifdef LILY_WINDOWS_OS
    String *pkg_filename_join = join__Vec(split_pkg_name, '\\');
#else
    String *pkg_filename_join = join__Vec(split_pkg_name, '/');
#endif

    String *pkg_filename = NEW(String);

    push_str__String(pkg_filename, (char *)wrapper->self->default_path);

#ifdef LILY_WINDOWS_OS
    push_str__String(pkg_filename, "\\");
#else
    push_str__String(pkg_filename, "/");
#endif

    APPEND_AND_FREE(pkg_filename, pkg_filename_join);

    LilyPackage *res = NULL;

    if (is__Dir(pkg_filename->buffer)) {
#ifdef LILY_WINDOWS_OS
        push_str__String(pkg_filename, "\\pkg.lily");
#else
        push_str__String(pkg_filename, "/pkg.lily");
#endif

        char *default_path = generate_default_path(pkg_filename->buffer);

        switch (wrapper->root_package->kind) {
            case LILY_PACKAGE_KIND_COMPILER:
                res = NEW_VARIANT(LilyPackage,
                                  compiler,
                                  wrapper->sub_package->name,
                                  wrapper->sub_package->global_name,
                                  wrapper->sub_package->visibility,
                                  pkg_filename->buffer,
                                  LILY_PACKAGE_STATUS_SUB_MAIN,
                                  default_path,
                                  wrapper->sub_package->global_name->buffer,
                                  wrapper->root_package);

                INIT_IR();

                break;
            case LILY_PACKAGE_KIND_INTERPRETER:
                res = NEW_VARIANT(LilyPackage,
                                  interpreter,
                                  wrapper->sub_package->name,
                                  wrapper->sub_package->global_name,
                                  wrapper->sub_package->visibility,
                                  pkg_filename->buffer,
                                  LILY_PACKAGE_STATUS_SUB_MAIN,
                                  default_path,
                                  wrapper->sub_package->global_name->buffer,
                                  wrapper->root_package);
                break;
            case LILY_PACKAGE_KIND_JIT:
                TODO("JIT");
            default:
                UNREACHABLE("unknown variant");
        }

        LOG_VERBOSE(res, "running");

        wrapper->default_path = default_path;
    } else {
        push_str__String(pkg_filename, ".lily");

        char *default_path = generate_default_path(pkg_filename->buffer);

        switch (wrapper->root_package->kind) {
            case LILY_PACKAGE_KIND_COMPILER:
                res = NEW_VARIANT(LilyPackage,
                                  compiler,
                                  wrapper->sub_package->name,
                                  wrapper->sub_package->global_name,
                                  wrapper->sub_package->visibility,
                                  pkg_filename->buffer,
                                  LILY_PACKAGE_STATUS_NORMAL,
                                  default_path,
                                  wrapper->sub_package->global_name->buffer,
                                  wrapper->root_package);

                INIT_IR();

                break;
            case LILY_PACKAGE_KIND_INTERPRETER:
                res = NEW_VARIANT(LilyPackage,
                                  interpreter,
                                  wrapper->sub_package->name,
                                  wrapper->sub_package->global_name,
                                  wrapper->sub_package->visibility,
                                  pkg_filename->buffer,
                                  LILY_PACKAGE_STATUS_NORMAL,
                                  default_path,
                                  wrapper->sub_package->global_name->buffer,
                                  wrapper->root_package);

                break;
            case LILY_PACKAGE_KIND_JIT:
                TODO("JIT");
            default:
                UNREACHABLE("unknown variant");
        }

        LOG_VERBOSE(res, "running");
    }

    wrapper->res = res;
    wrapper->pkg_filename = pkg_filename;

    // Clean up

    FREE_BUFFER_ITEMS(split_pkg_name->buffer, split_pkg_name->len, String);
    FREE(Vec, split_pkg_name);
}

void *
precompile_sub_package__LilyPrecompiler(void *w)
{
#define RUN_PRECOMPILER()                                                  \
    pthread_mutex_lock(&sub_package_thread_mutex);                         \
                                                                           \
    LOG_VERBOSE(wrapper->res, "running scanner");                          \
                                                                           \
    switch (wrapper->res->kind) {                                          \
        case LILY_PACKAGE_KIND_COMPILER:                                   \
            run__LilyScanner(&wrapper->res->scanner,                       \
                             wrapper->res->compiler.config->dump_scanner); \
            break;                                                         \
        case LILY_PACKAGE_KIND_INTERPRETER:                                \
            /* TODO: maybe set the dump scanner */                         \
            run__LilyScanner(&wrapper->res->scanner, false);               \
            break;                                                         \
        case LILY_PACKAGE_KIND_JIT:                                        \
            TODO("JIT: run scanner");                                      \
        default:                                                           \
            UNREACHABLE("unknown variant");                                \
    }                                                                      \
                                                                           \
    LOG_VERBOSE(wrapper->res, "running preparser");                        \
                                                                           \
    run__LilyPreparser(&wrapper->res->preparser,                           \
                       &wrapper->res->preparser_info);                     \
                                                                           \
    LOG_VERBOSE(wrapper->res, "running precompiler");                      \
                                                                           \
    run__LilyPrecompiler(                                                  \
      &wrapper->res->precompiler, wrapper->root_package, false);

    LilyPrecompilerSubPackageWrapper *wrapper = w;

    RUN_PRECOMPILER();

    init_module__LilyAnalysis(&wrapper->res->analysis);

    return NULL;
}
#else
LilyPackage *
precompile_sub_package__LilyPrecompiler(const LilyPrecompiler *self,
                                        const LilyPreparserSubPackage *sub_pkg,
                                        LilyPackage *root_package)
{
#define INIT_IR()                                                        \
    switch (root_package->compiler.ir.kind) {                            \
        case LILY_IR_KIND_CC:                                            \
            /* TODO: add a linker for CC */                              \
            res->compiler.ir = NEW_VARIANT(LilyIr, cc, NEW(LilyIrCc));   \
            res->compiler.linker = LILY_LINKER_KIND_CC;                  \
            break;                                                       \
        case LILY_IR_KIND_CPP:                                           \
            /* TODO: add a linker for CPP */                             \
            res->compiler.ir = NEW_VARIANT(LilyIr, cpp, NEW(LilyIrCpp)); \
            res->compiler.linker = LILY_LINKER_KIND_CPP;                 \
            break;                                                       \
        case LILY_IR_KIND_JS:                                            \
            res->compiler.ir = NEW_VARIANT(LilyIr, js, NEW(LilyIrJs));   \
            break;                                                       \
        case LILY_IR_KIND_LLVM:                                          \
            res->compiler.ir = NEW_VARIANT(                              \
              LilyIr, llvm, NEW(LilyIrLlvm, res->global_name->buffer));  \
            res->compiler.linker = LILY_LINKER_KIND_LLVM;                \
            break;                                                       \
        default:                                                         \
            UNREACHABLE("unknown variant");                              \
    }

    Vec *split_pkg_name = split__String(sub_pkg->name, '.');

#ifdef LILY_WINDOWS_OS
    String *pkg_filename_join = join__Vec(split_pkg_name, '\\');
#else
    String *pkg_filename_join = join__Vec(split_pkg_name, '/');
#endif

    String *pkg_filename = NEW(String);

    push_str__String(pkg_filename, (char *)self->default_path);

#ifdef LILY_WINDOWS_OS
    push_str__String(pkg_filename, "\\");
#else
    push_str__String(pkg_filename, "/");
#endif

    APPEND_AND_FREE(pkg_filename, pkg_filename_join);

    // If it's a directory. Check if there is `pkg.lily` in the directory.
    if (is__Dir(pkg_filename->buffer)) {
#ifdef LILY_WINDOWS_OS
        push_str__String(pkg_filename, "\\pkg.lily");
#else
        push_str__String(pkg_filename, "/pkg.lily");
#endif

        char *default_path = generate_default_path(pkg_filename->buffer);
        LilyPackage *res = NULL;

        switch (root_package->kind) {
            case LILY_PACKAGE_KIND_COMPILER:
                NEW_VARIANT(LilyPackage,
                            compiler,
                            sub_pkg->name,
                            sub_pkg->global_name,
                            sub_pkg->visibility,
                            pkg_filename->buffer,
                            LILY_PACKAGE_STATUS_SUB_MAIN,
                            default_path,
                            sub_pkg->global_name->buffer,
                            root_package);

                INIT_IR();

                break;
            case LILY_PACKAGE_KIND_INTERPRETER:
                NEW_VARIANT(LilyPackage,
                            interpreter,
                            sub_pkg->name,
                            sub_pkg->global_name,
                            sub_pkg->visibility,
                            pkg_filename->buffer,
                            LILY_PACKAGE_STATUS_SUB_MAIN,
                            default_path,
                            sub_pkg->global_name->buffer,
                            root_package);

                break;
            case LILY_PACKAGE_KIND_JIT:
                TODO("JIT");
            default:
                UNREACHABLE("unknown variant");
        }

        res->compiler.config = root_package->compiler.config;

        run__LilyScanner(&res->scanner, res->compiler.config->dump_scanner);
        run__LilyPreparser(&res->preparser, &res->preparser_info);
        run__LilyPrecompiler(&res->precompiler, root_package, false);

        init_module__LilyAnalysis(&res->analysis);

        FREE_BUFFER_ITEMS(split_pkg_name->buffer, split_pkg_name->len, String);
        FREE(Vec, split_pkg_name);
        lily_free(pkg_filename);
        lily_free(default_path);

        return res;
    } else {
        push_str__String(pkg_filename, ".lily");

        LilyPackage *res = NULL;

        switch (root_package->kind) {
            case LILY_PACKAGE_KIND_COMPILER:
                res = NEW_VARIANT(LilyPackage,
                                  compiler,
                                  sub_pkg->name,
                                  sub_pkg->global_name,
                                  sub_pkg->visibility,
                                  pkg_filename->buffer,
                                  LILY_PACKAGE_STATUS_NORMAL,
                                  self->default_path,
                                  self->package->global_name->buffer,
                                  root_package);

                INIT_IR();

                break;
            case LILY_PACKAGE_KIND_INTERPRETER:
                res = NEW_VARIANT(LilyPackage,
                                  interpreter,
                                  sub_pkg->name,
                                  sub_pkg->global_name,
                                  sub_pkg->visibility,
                                  pkg_filename->buffer,
                                  LILY_PACKAGE_STATUS_NORMAL,
                                  self->default_path,
                                  self->package->global_name->buffer,
                                  root_package);

                break;
            case LILY_PACKAGE_KIND_JIT:
                TODO("JIT");
            default:
                UNREACHABLE("unknown variant");
        }

        res->compiler.config = root_package->compiler.config;

        switch (res->kind) {
            case LILY_PACKAGE_KIND_COMPILER:
                run__LilyScanner(&res->scanner,
                                 res->compiler.config->dump_scanner);
                break;
            case LILY_PACKAGE_KIND_INTERPRETER:
                // TODO: maybe set the dump scanner
                run__LilyScanner(&res->scanner, false);
                break;
            case LILY_PACKAGE_KIND_JIT:
                TODO("JIT: run scanner");
                break;
            default:
                UNREACHABLE("unknown variant");
        }

        run__LilyPreparser(&res->preparser, &res->preparser_info);
        run__LilyPrecompiler(&res->precompiler, root_package, false);

        init_module__LilyAnalysis(&res->analysis);

        FREE_BUFFER_ITEMS(split_pkg_name->buffer, split_pkg_name->len, String);
        FREE(Vec, split_pkg_name);
        lily_free(pkg_filename);

        return res;
    }
}
#endif

LilyMacro *
precompile_macro__LilyPrecompiler(LilyPrecompiler *self,
                                  const LilyPreparserMacro *macro)
{
    // 1. Parse params of macro.
    Vec *params = NULL; // Vec<LilyMacroParam*>*?

    if (macro->params) {
        params = NEW(Vec);

        for (Usize i = 0; i < macro->params->len; ++i) {
            Vec *param = get__Vec(macro->params, i);
            Usize position = 0;
            LilyToken *current = param->len > 0 ? get__Vec(param, 0) : NULL;
            String *name = NULL;
            enum LilyMacroParamKind kind = -1;
            Location location;

#define NEXT_TOKEN() \
    current = ++position < param->len ? get__Vec(param, position) : NULL;
#define HAS_REACHED_THE_END() (!current)

            if (HAS_REACHED_THE_END()) {
                String *current_s = from__String(",");

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &macro->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    NULL),
                  &self->count_error);

                FREE(String, current_s);

                continue;
            } else {
                location = clone__Location(&current->location);

                switch (current->kind) {
                    case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
                        name = clone__String(current->identifier_dollar);
                        NEXT_TOKEN();

                        break;
                    default:
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &current->location,
                            NEW(LilyError,
                                LILY_ERROR_KIND_EXPECTED_IDENTIFIER_DOLLAR),
                            NULL,
                            NULL,
                            NULL),
                          &self->count_error);

                        continue;
                }
            }

            if (HAS_REACHED_THE_END()) {
                String *current_s = from__String(",");

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &macro->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    NULL),
                  &self->count_error);

                FREE(String, current_s);
                FREE(String, name);

                continue;
            } else {
                // Check the macro data type of the parameter.
                switch (current->kind) {
                    case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                        if (!strcmp(current->identifier_normal->buffer, "id")) {
                            kind = LILY_MACRO_PARAM_KIND_ID;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "dt")) {
                            kind = LILY_MACRO_PARAM_KIND_DT;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "tk")) {
                            kind = LILY_MACRO_PARAM_KIND_TK;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "tks")) {
                            kind = LILY_MACRO_PARAM_KIND_TKS;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "stmt")) {
                            kind = LILY_MACRO_PARAM_KIND_STMT;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "expr")) {
                            kind = LILY_MACRO_PARAM_KIND_EXPR;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "path")) {
                            kind = LILY_MACRO_PARAM_KIND_PATH;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "patt")) {
                            kind = LILY_MACRO_PARAM_KIND_PATT;
                        } else if (!strcmp(current->identifier_normal->buffer,
                                           "block")) {
                            kind = LILY_MACRO_PARAM_KIND_BLOCK;
                        } else {
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &current->location,
                                NEW(LilyError,
                                    LILY_ERROR_KIND_UNKNOWN_MACRO_DATA_TYPE),
                                NULL,
                                NULL,
                                NULL),
                              &self->count_error);

                            FREE(String, name);

                            continue;
                        }

                        break;
                    default:
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &current->location,
                            NEW(LilyError,
                                LILY_ERROR_KIND_EXPECTED_MACRO_DATA_TYPE),
                            NULL,
                            NULL,
                            NULL),
                          &self->count_error);

                        FREE(String, name);

                        continue;
                }
            }

            if (position + 1 != param->len) {
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &current->location,
                              NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                              NULL,
                              NULL,
                              from__String("expected `,`")),
                  &self->count_error);
            }

            END_LOCATION(&location, current->location);
            push__Vec(params, NEW(LilyMacroParam, kind, name, location));
        }
    }

    if (params) {
        // 2. Check if the parameter names are not duplicated.
        for (Usize i = 0; i < params->len; ++i) {
            LilyMacroParam *param_i = get__Vec(params, i);

            for (Usize j = i + 1; j < params->len; ++j) {
                if (!strcmp(param_i->name->buffer,
                            CAST(LilyMacroParam *, get__Vec(params, j))
                              ->name->buffer)) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &param_i->location,
                        NEW(LilyError, LILY_ERROR_KIND_MACRO_DUPLICATE_PARAM),
                        NULL,
                        NULL,
                        format__String(
                          "duplicate at parameter number {d} and {d}",
                          i + 1,
                          j + 1)),
                      &self->count_error);
                }
            }
        }

        // 3. See if the parameters of the macro are used.
        for (Usize i = 0; i < params->len; ++i) {
            LilyMacroParam *param = get__Vec(params, i);
            bool is_used = false;

            for (Usize j = 0; j < macro->tokens->len && !is_used; ++j) {
                LilyToken *token = get__Vec(macro->tokens, j);

                if (token->kind == LILY_TOKEN_KIND_IDENTIFIER_MACRO) {
                    if (!strcmp(param->name->buffer,
                                token->identifier_macro->buffer)) {
                        is_used = true;
                        param->is_used = is_used;
                    }
                }
            }

            if (!is_used) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_warning,
                    self->file,
                    &param->location,
                    NEW(LilyWarning, LILY_WARNING_KIND_UNUSED_MACRO_PARAM),
                    NULL,
                    NULL,
                    format__String("unused `{S}`", param->name)),
                  &self->count_error);
            }
        }
    }

    return NEW(LilyMacro, macro->name, params, macro->tokens, macro->location);
}

void
check_macros__LilyPrecompiler(LilyPrecompiler *self, LilyPackage *root_package)
{
    // 1. Add the public macros obtained by the preparer to the public macros of
    // root_package.
    for (Usize i = 0; i < self->info->public_macros->len; ++i) {
        push__Vec(root_package->public_macros,
                  precompile_macro__LilyPrecompiler(
                    self, get__Vec(self->info->public_macros, i)));
    }

    // 2. Add the private macros obtained by the preparser to private macro of
    // current package.
    for (Usize i = 0; i < self->info->private_macros->len; ++i) {
        push__Vec(self->package->private_macros,
                  precompile_macro__LilyPrecompiler(
                    self, get__Vec(self->info->private_macros, i)));
    }

    // 3. Check name conflict for macros (public macros).
    for (Usize i = 0; i < root_package->public_macros->len; ++i) {
        for (Usize j = i + 1; j < root_package->public_macros->len; ++j) {
            if (!strcmp(
                  CAST(LilyMacro *, get__Vec(root_package->public_macros, i))
                    ->name->buffer,
                  CAST(LilyMacro *, get__Vec(root_package->public_macros, j))
                    ->name->buffer)) {
                const Location *location_i =
                  &CAST(LilyMacro *, get__Vec(root_package->public_macros, i))
                     ->location;
                const Location *location_j =
                  &CAST(LilyMacro *, get__Vec(root_package->public_macros, j))
                     ->location;

                const File *file_j = get_file_from_filename__LilyPackage(
                  root_package, location_j->filename);

                if (file_j) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        file_j,
                        location_j,
                        NEW(LilyError, LILY_ERROR_KIND_NAME_CONFLICT),
                        NULL,
                        init__Vec(1,
                                  format__String("a macro with the same name "
                                                 "is defined at {s}:{d}:{d}",
                                                 location_i->filename,
                                                 location_i->start_line,
                                                 location_i->start_column)),
                        NULL),
                      &self->count_error);
                }
            }
        }
    }

    // 4. Check name conflict for macros (private macros).
    for (Usize i = 0; i < self->package->private_macros->len; ++i) {
        for (Usize j = i + 1; j < self->package->private_macros->len; ++j) {
            if (!strcmp(
                  CAST(LilyMacro *, get__Vec(self->package->private_macros, i))
                    ->name->buffer,
                  CAST(LilyMacro *, get__Vec(self->package->private_macros, j))
                    ->name->buffer)) {
                const Location *location_i =
                  &CAST(LilyMacro *, get__Vec(self->package->private_macros, i))
                     ->location;
                const Location *location_j =
                  &CAST(LilyMacro *, get__Vec(self->package->private_macros, j))
                     ->location;

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    &self->package->file,
                    location_j,
                    NEW(LilyError, LILY_ERROR_KIND_NAME_CONFLICT),
                    NULL,
                    init__Vec(
                      1,
                      format__String(
                        "a macro with the same name is defined at {s}:{d}:{d}",
                        location_i->filename,
                        location_i->start_line,
                        location_i->start_column)),
                    NULL),
                  &self->count_error);
            }
        }
    }

    // 5. Check name conflict for macros (all macros).
    for (Usize i = 0; i < self->package->private_macros->len; ++i) {
        for (Usize j = 0; j < root_package->public_macros->len; ++j) {
            if (!strcmp(
                  CAST(LilyMacro *, get__Vec(self->package->private_macros, i))
                    ->name->buffer,
                  CAST(LilyMacro *, get__Vec(root_package->public_macros, j))
                    ->name->buffer) &&
                i <= j) {
                const Location *location_i =
                  &CAST(LilyMacro *, get__Vec(self->package->private_macros, i))
                     ->location;
                const Location *location_j =
                  &CAST(LilyMacro *, get__Vec(root_package->public_macros, j))
                     ->location;
                const File *file_j = get_file_from_filename__LilyPackage(
                  root_package, location_j->filename);

                if (file_j) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        file_j,
                        location_j,
                        NEW(LilyError, LILY_ERROR_KIND_NAME_CONFLICT),
                        NULL,
                        init__Vec(1,
                                  format__String("a macro with the same name "
                                                 "is defined at {s}:{d}:{d}",
                                                 location_i->filename,
                                                 location_i->start_line,
                                                 location_i->start_column)),
                        NULL),
                      &self->count_error);
                }
            }
        }
    }
}

void
run__LilyPrecompiler(LilyPrecompiler *self,
                     LilyPackage *root_package,
                     bool precompile_macro_expand)
{
    // 1. Precompiler all imports
    for (Usize i = 0; i < self->info->public_imports->len; ++i) {
        LilyImport *import = precompile_import__LilyPrecompiler(
          self, get__Vec(self->info->public_imports, i));

        if (import) {
            push__Vec(self->package->public_imports, import);
        }
    }

    for (Usize i = 0; i < self->info->private_imports->len; ++i) {
        LilyImport *import = precompile_import__LilyPrecompiler(
          self, get__Vec(self->info->private_imports, i));

        if (import) {
            push__Vec(self->package->private_imports, import);
        }
    }

    // 2. Check macros
    check_macros__LilyPrecompiler(self, root_package);

    // 4. Precompiler all sub packages
#ifdef PRECOMPILER_USE_MULTITHREAD
    {
        Vec *wrappers = NEW(Vec); // Vec<LilyPrecompilerSubPackageWrapper*>*
        pthread_t *sub_package_threads = lily_malloc(
          sizeof(pthread_t) * self->info->package->sub_packages->len);

        ASSERT(!pthread_mutex_init(&sub_package_thread_mutex, NULL));

        for (Usize i = 0; i < self->info->package->sub_packages->len; ++i) {
            LilyPrecompilerSubPackageWrapper *wrapper =
              NEW(LilyPrecompilerSubPackageWrapper,
                  self,
                  get__Vec(self->info->package->sub_packages, i),
                  root_package);

            configure_sub_package__LilyPrecompiler(wrapper);
            push__Vec(wrappers, wrapper);

            ASSERT(wrapper->res);

            push__Vec(self->package->sub_packages, wrapper->res);
        }

        for (Usize i = 0; i < self->info->package->sub_packages->len; ++i) {
            ASSERT(!pthread_create(&sub_package_threads[i],
                                   NULL,
                                   &precompile_sub_package__LilyPrecompiler,
                                   get__Vec(wrappers, i)));
        }

        for (Usize i = 0; i < self->info->package->sub_packages->len; ++i) {
            pthread_join(sub_package_threads[i], NULL);
            FREE(LilyPrecompilerSubPackageWrapper, get__Vec(wrappers, i));
        }

        FREE(Vec, wrappers);
        lily_free(sub_package_threads);
        pthread_mutex_destroy(&sub_package_thread_mutex);
    }
#else
    for (Usize i = 0; i < self->info->package->sub_packages->len; ++i) {
        push__Vec(self->package->sub_packages,
                  precompile_sub_package__LilyPrecompiler(
                    self,
                    get__Vec(self->info->package->sub_packages, i),
                    root_package));
    }
#endif

    // 5. Init dependency tree.
    if (!strcmp(self->package->name->buffer, root_package->name->buffer) &&
        !precompile_macro_expand) {
        self->dependency_trees = NEW(Vec);

        build_dependency_tree__LilyPrecompiler(
          self, self->package, root_package);
    } else {
        // TODO: reload dependency tree
    }

#ifdef DEBUG_PRECOMPILE
    printf("\n====Precompiler(%s)====\n", self->file->name);

    if (self->dependency_trees) {
        printf("\n====Precompiler dependency trees(%s)====\n",
               self->file->name);

        for (Usize i = 0; i < self->dependency_trees->len; ++i) {
            CALL_DEBUG(LilyPackageDependencyTree,
                       get__Vec(self->dependency_trees, i));
        }
    }

    printf("\n====Precompiler public imports(%s)====\n", self->file->name);

    for (Usize i = 0; i < self->package->public_imports->len; ++i) {
        CALL_DEBUG(LilyImport, get__Vec(self->package->public_imports, i));
    }

    printf("\n====Precompiler private imports(%s)====\n", self->file->name);

    for (Usize i = 0; i < self->package->private_imports->len; ++i) {
        CALL_DEBUG(LilyImport, get__Vec(self->package->private_imports, i));
    }

    if (self->package->public_macros) {
        printf("\n====Precompiler public macros(%s)====\n", self->file->name);

        for (Usize i = 0; i < self->package->public_macros->len; ++i) {
            CALL_DEBUG(LilyMacro, get__Vec(self->package->public_macros, i));
        }
    }

    printf("\n====Precompiler private macros(%s)====\n", self->file->name);

    for (Usize i = 0; i < self->package->private_macros->len; ++i) {
        CALL_DEBUG(LilyMacro, get__Vec(self->package->private_macros, i));
    }

    for (Usize i = 0; i < self->package->sub_packages->len; ++i) {
    }
#endif
}

DESTRUCTOR(LilyPrecompiler, const LilyPrecompiler *self)
{
    if (self->dependency_trees) {
        FREE_BUFFER_ITEMS(self->dependency_trees->buffer,
                          self->dependency_trees->len,
                          LilyPackageDependencyTree);
        FREE(Vec, self->dependency_trees);
    }
}
