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
#include <base/file.h>
#include <base/new.h>

#include <cli/config/compile.h>
#include <cli/emit.h>

#include <core/lily/package/package.h>
#include <core/lily/parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CONSTRUCTOR(LilyPackage *,
            LilyPackage,
            String *name,
            enum LilyVisibility visibility,
            Vec *public_macros,
            char *filename,
            enum LilyPackageStatus status,
            const char *default_path)
{
    char *content = read_file__Path(filename);
    char *file_ext = get_extension__Path(filename);

    if (strcmp(file_ext, ".lily")) {
        EMIT_ERROR("bad extension, expected `.lily`");
        exit(1);
    }

    LilyPackage *self = lily_malloc(sizeof(LilyPackage));

    self->name = name;
    self->public_macros = public_macros;
    self->private_macros = NEW(Vec);
    self->public_imports = NEW(Vec);
    self->private_imports = NEW(Vec);
    self->sub_packages = NEW(Vec);
    self->pacakge_dependencies = NEW(Vec);
    self->lib_dependencies = NEW(Vec);
    self->file = NEW(File, filename, content);
    self->scanner =
      NEW(LilyScanner, NEW(Source, NEW(Cursor, content), &self->file));
    self->preparser = NEW(LilyPreparser, &self->scanner, self->name);
    self->visibility = visibility;
    self->status = status;
    self->precompile =
      NEW(LilyPrecompile, &self->preparser, self, default_path);

    lily_free(file_ext);

    return self;
}

LilyPackage *
build__LilyPackage(const CompileConfig *config,
                   String *name,
                   enum LilyVisibility visibility,
                   Vec *public_macros,
                   enum LilyPackageStatus status,
                   const char *default_path)
{
    LilyPackage *self = public_macros ? NEW(LilyPackage,
                                            NULL,
                                            visibility,
                                            NULL,
                                            (char *)config->filename,
                                            status,
                                            default_path)
                                      : NEW(LilyPackage,
                                            NULL,
                                            visibility,
                                            NEW(Vec),
                                            (char *)config->filename,
                                            status,
                                            default_path);

    run__LilyScanner(&self->scanner, config->dump_scanner);
    run__LilyPreparser(&self->preparser);

    if (self->preparser.package->name) {
        self->name = self->preparser.package->name;
    } else {
        self->name = from__String("main");
    }

    LilyDumpConfig dump_config = NEW(LilyDumpConfig,
                                     config->dump_scanner,
                                     config->dump_scanner,
                                     config->dump_typecheck,
                                     config->dump_ir);

    run__LilyPrecompile(&self->precompile, &dump_config);

    return self;
}

LilyPackage *
compile__LilyPackage(const CompileConfig *config,
                     String *name,
                     enum LilyVisibility visibility,
                     enum LilyPackageStatus status,
                     const char *default_path)
{
    return build__LilyPackage(
      config, name, visibility, NULL, status, default_path);
}

DESTRUCTOR(LilyPackage, LilyPackage *self)
{
    FREE(String, self->name);

    if (self->public_macros) {
        // TODO: add free for buffer
        FREE(Vec, self->public_macros);
    }

    // TODO: add free for buffer
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
    FREE(Vec, self->pacakge_dependencies);
    FREE(Vec, self->lib_dependencies);

    if (self->status == LILY_PACKAGE_STATUS_NORMAL) {
        lily_free(self->file.name);
    }

    FREE(File, &self->file);

    FREE(LilyScanner, &self->scanner);

    FREE_BUFFER_ITEMS(self->preparser.public_imports->buffer,
                      self->preparser.public_imports->len,
                      LilyPreparserImport);
    FREE(Vec, self->preparser.public_imports);

    FREE_BUFFER_ITEMS(self->preparser.private_imports->buffer,
                      self->preparser.private_imports->len,
                      LilyPreparserImport);
    FREE(Vec, self->preparser.private_imports);

    FREE_BUFFER_ITEMS(self->preparser.public_macros->buffer,
                      self->preparser.public_macros->len,
                      LilyPreparserMacro);
    FREE(Vec, self->preparser.public_macros);

    FREE_BUFFER_ITEMS(self->preparser.private_macros->buffer,
                      self->preparser.private_macros->len,
                      LilyPreparserMacro);
    FREE(Vec, self->preparser.private_macros);

    FREE(LilyPreparserPackage, self->preparser.package);

    lily_free(self);
}