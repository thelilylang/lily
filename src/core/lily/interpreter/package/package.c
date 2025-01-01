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

#include <core/lily/interpreter/package/package.h>
#include <core/lily/mir/generator.h>
#include <core/lily/package/package.h>

#include <pthread.h>

// TODO: add support for Windows.
static threadlocal pthread_t *package_threads;
static pthread_mutex_t package_thread_mutex;

/**
 *
 * @brief Run parser, analysis, mir.
 * @param self LilyPackageDependencyTree*
 */
static void *
run_threads__LilyInterpreterPackage(void *self);

DESTRUCTOR(LilyInterpreterAdapter, const LilyInterpreterAdapter *self)
{
    if (self->is_root) {
        FREE(LilyInterpreterVM, &self->vm);
    }

    if (self->lib) {
        FREE(LilyLibrary, self->lib);
    }
}

LilyPackage *
build__LilyInterpreterPackage(const LilyPackageInterpreterConfig *config,
                              const char *filename,
                              enum LilyVisibility visibility,
                              enum LilyPackageStatus status,
                              const char *default_path,
                              const LilyProgram *program,
                              LilyLibrary *lib)
{
    LilyPackage *self = NEW_VARIANT(LilyPackage,
                                    interpreter,
                                    NULL,
                                    NULL,
                                    visibility,
                                    (char *)filename,
                                    status,
                                    default_path,
                                    NULL,
                                    NULL);

    self->interpreter.config = config;

    LOG_VERBOSE(self, "running");
    LOG_VERBOSE(self, "running scanner");

    // TODO: add dump_scanner to the config.
    run__LilyScanner(&self->scanner, false);

    LOG_VERBOSE(self, "running preparser");

    run__LilyPreparser(&self->preparser, &self->preparser_info);

    SET_ROOT_PACKAGE_NAME(self);
    INTERPRETER_SET_ROOT_PACKAGE_PROGRAM(self, program);
    INTERPRETER_SET_ROOT_PACKAGE_USE_SWITCH(self);
    LOAD_ROOT_PACKAGE_RESOURCES(self, program);

    init_module__LilyAnalysis(&self->analysis);

    LOG_VERBOSE(self, "running precompiler");

    run__LilyPrecompiler(&self->precompiler, self, false);

    LOG_VERBOSE(self, "creation of threads");

    package_threads =
      lily_malloc(sizeof(pthread_t) * self->precompiler.dependency_trees->len);

    ASSERT(!pthread_mutex_init(&package_thread_mutex, NULL));

    for (Usize i = 0; i < self->precompiler.dependency_trees->len; ++i) {
        ASSERT(
          !pthread_create(&package_threads[i],
                          NULL,
                          &run_threads__LilyInterpreterPackage,
                          get__Vec(self->precompiler.dependency_trees, i)));
    }

    for (Usize i = 0; i < self->precompiler.dependency_trees->len; ++i) {
        pthread_join(package_threads[i], NULL);
    }

    lily_free(package_threads);
    pthread_mutex_destroy(&package_thread_mutex);

    // TODO: set check overflow
    self->interpreter.vm = NEW(LilyInterpreterVM,
                               config->max_heap,
                               config->max_stack,
                               &self->mir_module,
                               NEW(LilyInterpreterVMResources, config->args),
                               false);

    return self;
}

LilyLibrary *
build_lib__LilyInterpreterPackage(const LilyConfig *config,
                                  enum LilyVisibility visibility,
                                  enum LilyPackageStatus status,
                                  const char *default_path,
                                  const LilyProgram *program,
                                  String *version,
                                  String *url,
                                  String *path)
{
    return NULL;
}

static void *
run_threads__LilyInterpreterPackage(void *self)
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
                                   &run_threads__LilyInterpreterPackage,
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

void
run__LilyInterpreterPackage(const LilyConfig *config,
                            enum LilyVisibility visibility,
                            enum LilyPackageStatus status,
                            const char *default_path,
                            const LilyProgram *program)
{
    LilyPackageInterpreterConfig interpreter_config =
      from_RunConfig__LilyPackageInterpreterConfig(config);
    LilyPackage *package = build__LilyInterpreterPackage(&interpreter_config,
                                                         config->run.filename,
                                                         visibility,
                                                         status,
                                                         default_path,
                                                         program,
                                                         NULL);

    if (!package) {
        return;
    }

    // Run interpreter

    run__LilyInterpreterVM(&package->interpreter.vm);

    // Clean up

    FREE(LilyPackage, package);
}
