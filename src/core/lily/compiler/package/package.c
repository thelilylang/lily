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

#include <base/alloc.h>
#include <base/assert.h>
#include <base/file.h>
#include <base/new.h>

#include <cli/emit.h>
#include <cli/lilyc/config.h>

#include <core/lily/compiler/ir/llvm/compile.h>
#include <core/lily/compiler/ir/llvm/generator.h>
#include <core/lily/compiler/output/cache.h>
#include <core/lily/compiler/output/obj.h>
#include <core/lily/lily.h>
#include <core/lily/mir/generator.h>
#include <core/lily/package/default_path.h>
#include <core/lily/package/package.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: add support for Windows.
static threadlocal pthread_t *package_threads;
static pthread_mutex_t package_thread_mutex;

#define LOG_VERBOSE_SUCCESSFUL_COMPILATION(package)        \
    if (package->compiler.config->verbose) {               \
        printf("\x1b[32msuccessful compilation\x1b[0m\n"); \
    }

/**
 *
 * @brief Run parser, analysis, mir, ir and compile output object (...).
 * @param self LilyCompilerThreadWrapper*
 */
static void *
run_threads__LilyCompilerPackage(void *self);

DESTRUCTOR(LilyCompilerAdapter, const LilyCompilerAdapter *self)
{
    if (self->output_path) {
        lily_free(self->output_path);
    }

    if (self->output_exe_path) {
        lily_free(self->output_exe_path);
    }

    FREE(LilyIr, &self->ir);
}

LilyPackage *
build__LilyCompilerPackage(const LilyPackageCompilerConfig *config,
                           const char *filename,
                           enum LilyVisibility visibility,
                           enum LilyPackageStatus status,
                           const char *default_path,
                           const LilyProgram *program,
                           LilyLibrary *lib)
{
    LilyPackage *self = NEW_VARIANT(LilyPackage,
                                    compiler,
                                    NULL,
                                    NULL,
                                    visibility,
                                    (char *)filename,
                                    status,
                                    default_path,
                                    NULL,
                                    NULL);

    self->compiler.config = config;

    LOG_VERBOSE(self, "running");
    LOG_VERBOSE(self, "running scanner");

    run__LilyScanner(&self->scanner, self->compiler.config->dump_scanner);

    LOG_VERBOSE(self, "running preparser");

    run__LilyPreparser(&self->preparser, &self->preparser_info);

#ifdef RUN_UNTIL_PREPARSER
    FREE(LilyScanner, &self->scanner);
    FREE(LilyPreparserInfo, &self->preparser_info);
    FREE(File, &self->file);
    FREE_BUFFER_ITEMS(
      self->private_macros->buffer, self->private_macros->len, LilyMacro);
    FREE(Vec, self->private_macros);
    lily_free(self);

    return NULL;
#endif

    SET_ROOT_PACKAGE_NAME(self);
    COMPILER_SET_ROOT_PACKAGE_IR(self->compiler.config, self);
    COMPILER_SET_ROOT_PACKAGE_PROGRAM(self, program, lib);
    COMPILER_SET_ROOT_PACKAGE_USE_SWITCH(self);
    LOAD_ROOT_PACKAGE_RESOURCES(self, program);

    init_module__LilyAnalysis(&self->analysis);

    LOG_VERBOSE(self, "running precompiler");

    run__LilyPrecompiler(&self->precompiler, self, false);

#ifdef RUN_UNTIL_PRECOMPILER
    FREE(LilyPackage, self);
    exit(0);
#endif

    LOG_VERBOSE(self, "create cache");

    // Create `out.lily` cache
    create_cache__LilyCompilerOutputCache();

    LOG_VERBOSE(self, "creation of threads");

    package_threads =
      lily_malloc(sizeof(pthread_t) * self->precompiler.dependency_trees->len);

    ASSERT(!pthread_mutex_init(&package_thread_mutex, NULL));

    for (Usize i = 0; i < self->precompiler.dependency_trees->len; ++i) {
        ASSERT(
          !pthread_create(&package_threads[i],
                          NULL,
                          &run_threads__LilyCompilerPackage,
                          get__Vec(self->precompiler.dependency_trees, i)));
    }

    for (Usize i = 0; i < self->precompiler.dependency_trees->len; ++i) {
        pthread_join(package_threads[i], NULL);
    }

    lily_free(package_threads);
    pthread_mutex_destroy(&package_thread_mutex);

    return self;
}

LilyLibrary *
build_lib__LilyCompilerPackage(const LilycConfig *config,
                               enum LilyVisibility visibility,
                               enum LilyPackageStatus status,
                               const char *default_path,
                               const LilyProgram *program,
                               String *version,
                               String *url,
                               String *path)
{
    LilyPackageCompilerConfig compiler_config =
      from_CompileConfig__LilyPackageCompilerConfig(config);
    LilyLibrary *lib = NEW(LilyLibrary, version, url, path, NULL);

    return build__LilyCompilerPackage(&compiler_config,
                                      config->filename,
                                      visibility,
                                      status,
                                      default_path,
                                      program,
                                      lib)
      ->compiler.lib;
}

static void *
run_threads__LilyCompilerPackage(void *self)
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

    LOG_VERBOSE(tree->package, "running parser");

    run__LilyParser(&tree->package->parser, false);

    LOG_VERBOSE(tree->package, "running analysis");

    run__LilyAnalysis(&tree->package->analysis);

    LOG_VERBOSE(tree->package, "running mir");

    run__LilyMir(tree->package);

    LOG_VERBOSE(tree->package, "running ir");

    run__LilyIr(tree->package);

    LOG_VERBOSE(tree->package, "running compile output object");

    compile__LilyCompilerOutputObj(tree, &compile__LilyCompilerIrLlvm);

    LOG_VERBOSE(tree->package, "running package done");

    tree->is_done = true;

    pthread_mutex_unlock(&package_thread_mutex);

    // Run children of the tree
    if (tree->children->len > 0) {
        LOG_VERBOSE(tree->package, "running children package");

        package_threads = lily_malloc(sizeof(pthread_t) * tree->children->len);
        pthread_mutex_t children_thread_lock;

        ASSERT(!pthread_mutex_init(&children_thread_lock, NULL));
        pthread_mutex_lock(&children_thread_lock);

        for (Usize i = 0; i < tree->children->len; ++i) {
            ASSERT(!pthread_create(&package_threads[i],
                                   NULL,
                                   &run_threads__LilyCompilerPackage,
                                   get__Vec(tree->children, i)));
        }

        for (Usize i = 0; i < tree->children->len; ++i) {
            pthread_join(package_threads[i], NULL);
        }

        pthread_mutex_unlock(&children_thread_lock);

        lily_free(package_threads);
        pthread_mutex_destroy(&children_thread_lock);
    }

    return NULL;
}

LilyPackage *
compile__LilyCompilerPackage(const LilycConfig *config,
                             enum LilyVisibility visibility,
                             enum LilyPackageStatus status,
                             const char *default_path,
                             const LilyProgram *program)
{
    LilyPackageCompilerConfig compiler_config =
      from_CompileConfig__LilyPackageCompilerConfig(config);
    LilyPackage *package = build__LilyCompilerPackage(&compiler_config,
                                                      config->filename,
                                                      visibility,
                                                      status,
                                                      default_path,
                                                      program,
                                                      NULL);

    ASSERT(package->status == LILY_PACKAGE_STATUS_MAIN && package->is_exe);

    LOG_VERBOSE(package, "running compile exe");

    compile_exe__LilyLinker(package);

    LOG_VERBOSE_SUCCESSFUL_COMPILATION(package);

    return package;
}

LilyLibrary *
compile_lib__LilyCompilerPackage(const LilycConfig *config,
                                 enum LilyVisibility visibility,
                                 enum LilyPackageStatus status,
                                 const char *default_path,
                                 const LilyProgram *program)
{
    LilyLibrary *lib = build_lib__LilyCompilerPackage(
      config, visibility, status, default_path, program, NULL, NULL, NULL);

    ASSERT(lib->package->status == LILY_PACKAGE_STATUS_LIB_MAIN &&
           lib->package->is_lib);

    LOG_VERBOSE(lib->package, "running compile lib");

    compile_lib__LilyLinker(lib);

    LOG_VERBOSE_SUCCESSFUL_COMPILATION(lib->package);

    return lib;
}

void
run_scanner__LilyCompilerPackage(const LilycConfig *config)
{
    char *content = read_file__File(config->filename);
    File file = NEW(File, (char *)config->filename, content);
    Usize count_error = 0;
    LilyScanner scanner = NEW(
      LilyScanner, NEW(Source, NEW(Cursor, file.content), &file), &count_error);

    run__LilyScanner(&scanner, config->dump_scanner);

    // Clean up

    FREE(File, &file);
    FREE(LilyScanner, &scanner);
}

void
run_preparser__LilyCompilerPackage(const LilycConfig *config)
{
    char *content = read_file__File(config->filename);
    File file = NEW(File, (char *)config->filename, content);
    Usize count_error = 0;
    LilyScanner scanner = NEW(
      LilyScanner, NEW(Source, NEW(Cursor, file.content), &file), &count_error);
    LilyPreparser preparser =
      NEW(LilyPreparser, &file, scanner.tokens, NULL, true);
    LilyPreparserInfo preparser_info = NEW(LilyPreparserInfo, NULL);

    run__LilyScanner(&scanner, config->dump_scanner);
    run__LilyPreparser(&preparser, &preparser_info);

    // Clean up

    FREE(File, &file);
    FREE(LilyScanner, &scanner);
    FREE(LilyPreparserInfo, &preparser_info);
}

void
run_precompiler__LilyCompilerPackage(const LilycConfig *config)
{
    // NOTE: We don't care about the kind of program at this point.
    LilyLibrary *lib = NULL;
    LilyProgram program = NEW(LilyProgram, LILY_PROGRAM_KIND_EXE);
    char *default_path = generate_default_path((char *)config->filename);
    LilyPackage *self = NEW_VARIANT(LilyPackage,
                                    compiler,
                                    NULL,
                                    NULL,
                                    LILY_VISIBILITY_PUBLIC,
                                    (char *)config->filename,
                                    LILY_PACKAGE_STATUS_MAIN,
                                    default_path,
                                    NULL,
                                    NULL);

    LilyPackageCompilerConfig pkg_config =
      from_CompileConfig__LilyPackageCompilerConfig(config);

    self->compiler.config = &pkg_config;

    LOG_VERBOSE(self, "running");
    LOG_VERBOSE(self, "running scanner");

    run__LilyScanner(&self->scanner, self->compiler.config->dump_scanner);

    LOG_VERBOSE(self, "running preparser");

    // NOTE: Set `destroy_all` on true to free all.
    // NOTE: If we run the program after the analysis, we will set `destroy_all`
    // to false.
    self->preparser.destroy_all = true;

    run__LilyPreparser(&self->preparser, &self->preparser_info);

    SET_ROOT_PACKAGE_NAME(self);
    COMPILER_SET_ROOT_PACKAGE_IR(self->compiler.config, self);
    COMPILER_SET_ROOT_PACKAGE_PROGRAM(self, (&program), lib);
    LOAD_ROOT_PACKAGE_RESOURCES(self, (&program));

    LOG_VERBOSE(self, "running precompiler");

    run__LilyPrecompiler(&self->precompiler, self, false);

    // NOTE: This line is only used to avoid to get an unreachable result.
    init_module__LilyAnalysis(&self->analysis);

    // Clean up

    FREE(LilyPackage, self);
    lily_free(default_path);
    FREE(LilyProgram, &program);
}

void
run_parser__LilyCompilerPackage(const LilycConfig *config)
{
    TODO("--run-parser");
}

void
run_analysis__LilyCompilerPackage(const LilycConfig *config)
{
    TODO("--run-analysis");
}

void
run_mir__LilyCompilerPackage(const LilycConfig *config)
{
    TODO("--run-mir");
}

void
run_ir__LilyCompilerPackage(const LilycConfig *config)
{
    TODO("--run-ir");
}
