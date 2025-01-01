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

#include <builtin/alloc.h>

#include <core/lily/lily.h>
#include <core/lily/package/package.h>

static CONSTRUCTOR(LilyPackage *,
                   LilyPackage,
                   String *name,
                   String *global_name,
                   enum LilyVisibility visibility,
                   char *filename,
                   enum LilyPackageStatus status,
                   const char *default_path,
                   const char *default_package_access,
                   LilyPackage *root);

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

    self->count_error = 0;
    self->count_warning = 0;

    self->file = NEW(File, filename, content);
    self->scanner = NEW(LilyScanner,
                        NEW(Source, NEW(Cursor, content), &self->file),
                        &self->count_error);
#if defined(RUN_UNTIL_PREPARSER) || defined(RUN_UNTIL_PRECOMPILER)
    self->preparser = NEW(LilyPreparser,
                          &self->file,
                          self->scanner.tokens,
                          default_package_access,
                          true);
#else
    self->preparser = NEW(LilyPreparser,
                          &self->file,
                          self->scanner.tokens,
                          default_package_access,
                          false);
#endif
    self->preparser_info = NEW(LilyPreparserInfo, self->name);
    self->visibility = visibility;
    self->status = status;

#ifndef RUN_UNTIL_PREPARSER
    self->precompiler = NEW(
      LilyPrecompiler, &self->preparser_info, &self->file, self, default_path);
    self->builtin_usage = NEW(Vec);
    self->sys_usage = NEW(Vec);
    self->operator_register = NEW(LilyCheckedOperatorRegister);
    self->mir_module = LilyMirCreateModule();

    if (root) {
        self->preparser.destroy_all = root->preparser.destroy_all;
        self->program = root->program;
        self->parser = NEW(LilyParser, self, root, NULL);
        self->analysis = NEW(
          LilyAnalysis, self, root, &self->parser, root->analysis.use_switch);
        self->builtins = NULL;
        self->syss = NULL;

        // Push default operators contains in program resources.
        for (Usize i = 0; i < DEFAULT_OPERATORS_COUNT; ++i) {
            push__Vec(self->operator_register.operators,
                      ref__LilyCheckedOperator(
                        self->program->resources.default_operators[i]));
        }
    } else {
        self->parser = NEW(LilyParser, self, self, NULL);
        self->analysis = NEW(LilyAnalysis, self, self, &self->parser, false);
    }
#endif

    self->sys_is_loaded = false;
    self->std_is_loaded = false;
    self->core_is_loaded = false;
    self->builtin_is_loaded = false;
    self->main_is_found = false;
    self->is_exe = false;
    self->is_lib = false;
    self->is_static_lib = false;
    self->is_dynamic_lib = false;

    lily_free(file_ext);

    return self;
}

VARIANT_CONSTRUCTOR(LilyPackage *,
                    LilyPackage,
                    compiler,
                    String *name,
                    String *global_name,
                    enum LilyVisibility visibility,
                    char *filename,
                    enum LilyPackageStatus status,
                    const char *default_path,
                    const char *default_package_access,
                    LilyPackage *root)
{
    LilyPackage *self = NEW(LilyPackage,
                            name,
                            global_name,
                            visibility,
                            filename,
                            status,
                            default_path,
                            default_package_access,
                            root);

    self->kind = LILY_PACKAGE_KIND_COMPILER;
    self->compiler =
      NEW(LilyCompilerAdapter, root ? root->compiler.config : NULL);

    return self;
}

VARIANT_CONSTRUCTOR(LilyPackage *,
                    LilyPackage,
                    interpreter,
                    String *name,
                    String *global_name,
                    enum LilyVisibility visibility,
                    char *filename,
                    enum LilyPackageStatus status,
                    const char *default_path,
                    const char *default_package_access,
                    LilyPackage *root)
{
    LilyPackage *self = NEW(LilyPackage,
                            name,
                            global_name,
                            visibility,
                            filename,
                            status,
                            default_path,
                            default_package_access,
                            root);

    self->kind = LILY_PACKAGE_KIND_INTERPRETER;
    self->interpreter = NEW(
      LilyInterpreterAdapter, root ? root->interpreter.config : NULL, !root);

    return self;
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
        case LILY_PACKAGE_STATUS_MAIN:
        case LILY_PACKAGE_STATUS_LIB_MAIN:
            FREE(String, self->name);
            FREE(String, self->global_name);

            break;
        case LILY_PACKAGE_STATUS_NORMAL:
        case LILY_PACKAGE_STATUS_SUB_MAIN:
            lily_free(self->file.name);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    FREE(File, &self->file);

    FREE(LilyScanner, &self->scanner);
    FREE(LilyPreparserInfo, &self->preparser_info);
    FREE(LilyPrecompiler, &self->precompiler);
    FREE(LilyParser, &self->parser);
    FREE(LilyAnalysis, &self->analysis);
    LilyMirDisposeModule(&self->mir_module);

    FREE(Vec, self->builtin_usage);
    FREE(Vec, self->sys_usage);

    FREE(LilyCheckedOperatorRegister, &self->operator_register);

    switch (self->kind) {
        case LILY_PACKAGE_KIND_COMPILER:
            FREE(LilyCompilerAdapter, &self->compiler);
            break;
        case LILY_PACKAGE_KIND_INTERPRETER:
            FREE(LilyInterpreterAdapter, &self->interpreter);
            break;
        case LILY_PACKAGE_KIND_JIT:
            TODO("JIT");
        default:
            UNREACHABLE("unknown variant");
    }

    lily_free(self);
}
