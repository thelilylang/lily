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

#include <core/lily/interpreter/package/package.h>

CONSTRUCTOR(LilyPackage__LilyInterpreterAdapter *,
            LilyPackage__LilyInterpreterAdapter,
            String *name,
            String *global_name,
            enum LilyVisibility visibility,
            char *filename,
            enum LilyPackageStatus status,
            const char *default_path,
            const char *default_package_access,
            LilyPackage__LilyInterpreterAdapter *root)
{
    char *content = read_file__File(filename);
    char *file_ext = get_extension__File(filename);

    if (strcmp(file_ext, ".lily")) {
        EMIT_ERROR("bad extension, expected `.lily`");
        exit(1);
    }

    LilyPackage__LilyInterpreterAdapter *self =
      lily_malloc(sizeof(LilyPackage__LilyInterpreterAdapter));

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
    self->count_error = 0;
    self->count_warning = 0;

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
        self->config = root->config;

        // Push default operators contains in program ressources.
        for (Usize i = 0; i < DEFAULT_OPERATORS_COUNT; ++i) {
            push__Vec(self->operator_register.operators,
                      ref__LilyCheckedOperator(
                        self->program->ressources.default_operators[i]));
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

    lily_free(file_ext);

    return self;
}

IMPL_LilyPackageAdapter(LilyInterpreterAdapter);
