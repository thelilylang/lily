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
#include <base/assert.h>
#include <base/file.h>
#include <base/new.h>

#include <cli/emit.h>
#include <cli/lilyc/config.h>

#include <core/lily/ir/llvm/generator.h>
#include <core/lily/lily.h>
#include <core/lily/mir/generator.h>
#include <core/lily/package/package.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: add support for Windows.
static pthread_t *package_threads;
static pthread_mutex_t package_thread_mutex;

/**
 *
 * @brief Run all packages.
 * @param self LilyPackageDependencyTree*
 */
static void *
run__LilyPackage(void *self);

CONSTRUCTOR(LilyPackage *,
            LilyPackage,
            String *name,
            String *global_name,
            enum LilyVisibility visibility,
            char *filename,
            enum LilyPackageStatus status,
            const char *default_path,
            const char *default_package_access,
            LilyPackage *root)
{
    char *content = read_file__File(filename);
    char *file_ext = get_extension__File(filename);

    if (strcmp(file_ext, ".lily")) {
        EMIT_ERROR("bad extension, expected `.lily`");
        exit(1);
    }

    LilyPackage *self = lily_malloc(sizeof(LilyPackage));

    self->name = name;
    self->global_name = global_name;

    self->private_macros = NEW(Vec);

#ifndef RUN_UNTIL_PREPARSER
    if (status == LILY_PACKAGE_STATUS_MAIN) {
        self->public_macros = NEW(Vec);
    } else {
        self->public_macros = NULL;
    }

    self->public_imports = NEW(Vec);
    self->private_imports = NEW(Vec);
    self->sub_packages = NEW(Vec);
    self->package_dependencies = NEW(Vec);
    self->lib_dependencies = NEW(Vec);
#else
    self->public_imports = NULL;
    self->private_imports = NULL;
    self->sub_packages = NULL;
    self->package_dependencies = NULL;
    self->lib_dependencies = NULL;
#endif

    self->file = NEW(File, filename, content);
    self->scanner =
      NEW(LilyScanner, NEW(Source, NEW(Cursor, content), &self->file));
    self->preparser = NEW(
      LilyPreparser, &self->file, self->scanner.tokens, default_package_access);
    self->preparser_info = NEW(LilyPreparserInfo, self->name);
    self->visibility = visibility;
    self->status = status;
    self->count_error = 0;
    self->count_warning = 0;

#ifndef RUN_UNTIL_PREPARSER
    self->precompile = NEW(
      LilyPrecompile, &self->preparser_info, &self->file, self, default_path);
    self->builtin_usage = NEW(Vec);
    self->sys_usage = NEW(Vec);
    self->operator_register = NEW(LilyCheckedOperatorRegister);

    if (root) {
        self->parser = NEW(LilyParser, self, root, NULL);
        self->analysis = NEW(LilyAnalysis, self, root, &self->parser);
        self->mir_module = LilyMirCreateModule();
        self->builtins = NULL;
        self->syss = NULL;

        // Push default operators pushed in the operator register of root
        // package.
        for (Usize i = 0; i < root->operator_register.operators->len; ++i) {
            push__Vec(self->operator_register.operators,
                      ref__LilyCheckedOperator(
                        get__Vec(root->operator_register.operators, i)));
        }
    } else {
        self->parser = NEW(LilyParser, self, self, NULL);
        self->analysis = NEW(LilyAnalysis, self, self, &self->parser);
    }
#endif

    self->sys_is_loaded = false;
    self->std_is_loaded = false;
    self->core_is_loaded = false;
    self->builtin_is_loaded = false;
    self->main_is_found = false;
    self->is_exe = false;
    self->is_lib = false;

    lily_free(file_ext);

    return self;
}

LilyPackage *
build__LilyPackage(const LilycConfig *config,
                   enum LilyVisibility visibility,
                   enum LilyPackageStatus status,
                   const char *default_path,
                   const LilyProgram *program)
{
    LilyPackage *self = NEW(LilyPackage,
                            NULL,
                            NULL,
                            visibility,
                            (char *)config->filename,
                            status,
                            default_path,
                            NULL,
                            NULL);
    LilyPackageConfig pkg_config =
      from_CompileConfig__LilyPackageConfig(config);

    self->config = &pkg_config;

    run__LilyScanner(&self->scanner, pkg_config.dump_scanner);
    run__LilyPreparser(&self->preparser, &self->preparser_info);

#ifdef RUN_UNTIL_PREPARSER
    FREE(LilyScanner, &self->scanner);
    FREE(LilyPreparserInfo, &self->preparser_info);
    FREE(File, &self->file);
    lily_free(self);

    return NULL;
#endif

    // Load builtins and syss
    self->builtins = program->ressources.builtins;
    self->syss = program->ressources.syss;

    // Load default operators in operator register
    // In a normal case where we wanted to add an operator to the register, we'd
    // use `add_operator__LilyCheckedOperatorRegister`, but in this case it's
    // guaranteed that no operator repeats itself, so to increase loading speed
    // we'll add it directly to the vector without checking.
    for (Usize i = 0; i < DEFAULT_OPERATORS_COUNT; ++i) {
        push__Vec(
          self->operator_register.operators,
          ref__LilyCheckedOperator(&program->ressources.default_operators[i]));
    }

    self->program_kind = program->kind;

    switch (self->program_kind) {
        case LILY_PROGRAM_KIND_EXE:
            self->is_exe = true;
            break;
        default:
            self->is_lib = true;
    }

    if (self->preparser_info.package->name) {
        self->name = self->preparser_info.package->name;
        self->global_name = clone__String(self->name);
    } else {
        self->name = from__String("main");
        self->global_name = from__String("main");
    }

    self->mir_module = LilyMirCreateModule();

    if (config->cc_ir) {
        // TODO: add a linker for CC
        self->ir = NEW_VARIANT(LilyIr, cc, NEW(LilyIrCc));
    } else if (config->cpp_ir) {
        // TODO: add a linker for CPP
        self->ir = NEW_VARIANT(LilyIr, cpp, NEW(LilyIrCpp));
    } else if (config->js_ir) {
        self->ir = NEW_VARIANT(LilyIr, js, NEW(LilyIrJs));
    } else {
        self->ir =
          NEW_VARIANT(LilyIr, llvm, NEW(LilyIrLlvm, self->global_name->buffer));
        self->linker =
          NEW_VARIANT(LilyLinker, llvm, NEW(LilyIrLlvmLinker, &self->ir.llvm));
    }

    run__LilyPrecompile(&self->precompile, self, false);

#ifdef RUN_UNTIL_PRECOMPILE
    FREE(LilyPackage, self);
    exit(0);
#endif

    package_threads =
      lily_malloc(sizeof(pthread_t) * self->precompile.dependency_trees->len);

    ASSERT(!pthread_mutex_init(&package_thread_mutex, NULL));

    for (Usize i = 0; i < self->precompile.dependency_trees->len; ++i) {
        ASSERT(!pthread_create(&package_threads[i],
                               NULL,
                               &run__LilyPackage,
                               get__Vec(self->precompile.dependency_trees, i)));
    }

    for (Usize i = 0; i < self->precompile.dependency_trees->len; ++i) {
        pthread_join(package_threads[i], NULL);
    }

    lily_free(package_threads);
    pthread_mutex_destroy(&package_thread_mutex);

    return self;
}

static void *
run__LilyPackage(void *self)
{
    pthread_mutex_lock(&package_thread_mutex);

    LilyPackageDependencyTree *tree = self;

    if (tree->dependencies) {
        for (Usize i = 0; i < tree->dependencies->len; ++i) {
            LilyPackageDependencyTree *dep = get__Vec(tree->dependencies, i);

            while (!dep->is_done)
                ;
        }
    }

    run__LilyParser(&tree->package->parser, false);
    run__LilyAnalysis(&tree->package->analysis);
    run__LilyMir(tree->package);
    // run__LilyIr(tree->package);

    if (tree->package->status == LILY_PACKAGE_STATUS_MAIN) {
        // run__LilyLinker(tree->package);
    }

    tree->is_done = true;

    pthread_mutex_unlock(&package_thread_mutex);

    // Run children of the tree
    if (tree->children->len > 0) {
        pthread_t *children_threads =
          lily_malloc(sizeof(pthread_t) * tree->children->len);
        pthread_mutex_t children_thread_lock;

        ASSERT(!pthread_mutex_init(&children_thread_lock, NULL));
        pthread_mutex_lock(&children_thread_lock);

        for (Usize i = 0; i < tree->children->len; ++i) {
            ASSERT(!pthread_create(&children_threads[i],
                                   NULL,
                                   &run__LilyPackage,
                                   get__Vec(tree->children, i)));
        }

        for (Usize i = 0; i < tree->children->len; ++i) {
            pthread_join(children_threads[i], NULL);
        }

        pthread_mutex_unlock(&children_thread_lock);

        lily_free(children_threads);
        pthread_mutex_destroy(&children_thread_lock);
    }

    return NULL;
}

LilyPackage *
compile__LilyPackage(const LilycConfig *config,
                     enum LilyVisibility visibility,
                     enum LilyPackageStatus status,
                     const char *default_path,
                     const LilyProgram *program)
{
    return build__LilyPackage(
      config, visibility, status, default_path, program);
}

const File *
get_file_from_filename__LilyPackage(const LilyPackage *self,
                                    const char *filename)
{
    if (!strcmp(filename, self->file.name)) {
        return &self->file;
    }

    for (Usize i = 0; i < self->sub_packages->len; i++) {
        const File *f = get_file_from_filename__LilyPackage(
          get__Vec(self->sub_packages, i), filename);

        if (f) {
            return f;
        }
    }

    return NULL;
}

LilyPackage *
search_package_from_filename__LilyPackage(LilyPackage *self,
                                          const char *filename)
{
    if (!strcmp(filename, self->file.name)) {
        return self;
    }

    for (Usize i = 0; i < self->sub_packages->len; i++) {
        LilyPackage *pkg = search_package_from_filename__LilyPackage(
          get__Vec(self->sub_packages, i), filename);

        if (pkg) {
            return pkg;
        }
    }

    return NULL;
}

LilyPackage *
search_package_from_name__LilyPackage(LilyPackage *self, String *name)
{
    if (!strcmp(name->buffer, self->name->buffer)) {
        return self;
    }

    for (Usize i = 0; i < self->sub_packages->len; i++) {
        LilyPackage *pkg = search_package_from_name__LilyPackage(
          get__Vec(self->sub_packages, i), name);

        if (pkg) {
            return pkg;
        }
    }

    return NULL;
}

void
add_sys_fun_to_sys_usage__LilyPackage(LilyPackage *self, LilySysFun *fun_sys)
{
    for (Usize i = 0; i < self->sys_usage->len; ++i) {
        if (!strcmp(CAST(LilySysFun *, get__Vec(self->sys_usage, i))->name,
                    fun_sys->name)) {
            return;
        }
    }

    push__Vec(self->sys_usage, fun_sys);
}

void
add_builtin_fun_to_builtin_usage__LilyPackage(LilyPackage *self,
                                              LilyBuiltinFun *fun_builtin)
{
    for (Usize i = 0; i < self->builtin_usage->len; ++i) {
        if (!strcmp(CAST(LilySysFun *, get__Vec(self->builtin_usage, i))
                      ->real_name->buffer,
                    fun_builtin->real_name->buffer)) {
            return;
        }
    }

    push__Vec(self->builtin_usage, fun_builtin);
}

DESTRUCTOR(LilyPackage, LilyPackage *self)
{
    FREE(String, self->name);
    FREE(String, self->global_name);

    if (self->public_macros) {
        FREE_BUFFER_ITEMS(
          self->public_macros->buffer, self->public_macros->len, LilyMacro);
        FREE(Vec, self->public_macros);
    }

    FREE_BUFFER_ITEMS(
      self->private_macros->buffer, self->private_macros->len, LilyMacro);
    FREE(Vec, self->private_macros);

    FREE_BUFFER_ITEMS(
      self->public_imports->buffer, self->public_imports->len, LilyImport);
    FREE(Vec, self->public_imports);

    FREE_BUFFER_ITEMS(
      self->private_imports->buffer, self->private_imports->len, LilyImport);
    FREE(Vec, self->private_imports);

    FREE_BUFFER_ITEMS(
      self->sub_packages->buffer, self->sub_packages->len, LilyPackage);
    FREE(Vec, self->sub_packages);
    FREE(Vec, self->package_dependencies);
    FREE(Vec, self->lib_dependencies);

    switch (self->status) {
        case LILY_PACKAGE_STATUS_NORMAL:
        case LILY_PACKAGE_STATUS_SUB_MAIN:
            lily_free(self->file.name);
            break;
        default:
            break;
    }

    FREE(File, &self->file);

    FREE(LilyScanner, &self->scanner);
    FREE(LilyPreparserInfo, &self->preparser_info);
    FREE(LilyPrecompile, &self->precompile);
    FREE(LilyParser, &self->parser);
    FREE(LilyAnalysis, &self->analysis);
    LilyMirDisposeModule(&self->mir_module);
    FREE(LilyIr, &self->ir);
    FREE(LilyLinker, &self->linker);

    FREE(Vec, self->builtin_usage);
    FREE(Vec, self->sys_usage);

    FREE(LilyCheckedOperatorRegister, &self->operator_register);

    lily_free(self);
}
