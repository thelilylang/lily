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
#include <core/lily/precompile.h>
#include <core/lily/preparser.h>
#include <core/lily/scanner.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CONSTRUCTOR(LilyPackage *,
            LilyPackage,
            String *name,
            enum LilyVisibility visibility,
            Vec *public_macros)
{
    LilyPackage *self = lily_malloc(sizeof(LilyPackage));

    self->name = name;
    self->public_macros = public_macros;
    self->private_macros = NEW(Vec);
    self->public_imports = NEW(Vec);
    self->private_imports = NEW(Vec);
    self->sub_packages = NEW(Vec);
    self->pacakge_dependencies = NEW(Vec);
    self->lib_dependencies = NEW(Vec);
    self->visibility = visibility;

    return self;
}

LilyPackage *
build__LilyPackage(const CompileConfig *config,
                   String *name,
                   enum LilyVisibility visibility,
                   Vec *public_macros)
{
    LilyPackage *self = public_macros
                          ? NEW(LilyPackage, NULL, visibility, NULL)
                          : NEW(LilyPackage, NULL, visibility, NEW(Vec));

    char *content = read_file__Path(config->filename);
    char *file_ext = get_extension__Path(config->filename);

    if (strcmp(file_ext, ".lily")) {
        EMIT_ERROR("bad extension, expected `.lily`");
        exit(1);
    }

    const File file = NEW(File, config->filename, content);
    LilyScanner scanner =
      NEW(LilyScanner, NEW(Source, NEW(Cursor, content), &file));

    run__LilyScanner(&scanner, config->dump_scanner);

    LilyPreparser preparser = NEW(LilyPreparser, &scanner, name);

    run__LilyPreparser(&preparser);

    if (preparser.package->name) {
        self->name = preparser.package->name;
    } else {
        self->name = from__String("main");
    }

    LilyPrecompile precompile = NEW(LilyPrecompile, &preparser, self);

    run__LilyPrecompile(&precompile);

    lily_free(file_ext);
    lily_free(content);
    FREE(LilyScanner, &scanner);

    // ===============
    FREE_BUFFER_ITEMS(preparser.public_imports->buffer,
                      preparser.public_imports->len,
                      LilyPreparserImport);
    FREE(Vec, preparser.public_imports);

    FREE_BUFFER_ITEMS(preparser.private_imports->buffer,
                      preparser.private_imports->len,
                      LilyPreparserImport);
    FREE(Vec, preparser.private_imports);

    FREE_BUFFER_ITEMS(preparser.public_macros->buffer,
                      preparser.public_macros->len,
                      LilyPreparserMacro);
    FREE(Vec, preparser.public_macros);

    FREE_BUFFER_ITEMS(preparser.private_macros->buffer,
                      preparser.private_macros->len,
                      LilyPreparserMacro);
    FREE(Vec, preparser.private_macros);

    FREE(LilyPreparserPackage, preparser.package);
    // ===============

    return self;
}

LilyPackage *
compile__LilyPackage(const CompileConfig *config,
                     String *name,
                     enum LilyVisibility visibility)
{
    return build__LilyPackage(config, name, visibility, NULL);
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
    lily_free(self);
}