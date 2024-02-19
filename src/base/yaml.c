/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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
#include <base/new.h>
#include <base/types.h>
#include <base/yaml.h>

#include <local/src/libyaml/src/yaml_private.h>

DESTRUCTOR(YAMLLoadRes, const YAMLLoadRes *self)
{
    if (self->documents) {
        for (Usize i = 0; i < self->len; ++i) {
            yaml_document_delete(&self->documents[i]);
        }

        lily_free(self->documents);
    }
}

YAMLLoadRes
load__YAML(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (!file) {
        FAILED("YAML: cannot open file");
    }

    YAMLParser parser;
    YAMLDocument document;
    bool done = false;
    Usize documents_len = 0;

    if (!yaml_parser_initialize(&parser)) {
        FAILED("YAML: failed to initialize parser");
    }

    YAMLDocument *documents = NULL;

    yaml_parser_set_input_file(&parser, file);

    while (!done) {
        if (!yaml_parser_load(&parser, &document)) {
            FAILED("error occured in YAML file");
        }

        done = (!yaml_document_get_root_node(&document));

        if (!documents) {
            documents = malloc(sizeof(YAMLDocument));
        } else {
            documents =
              realloc(documents, sizeof(YAMLDocument) * (documents_len + 1));
        }

        documents[documents_len++] = document;
    }

    yaml_parser_delete(&parser);

    if (fclose(file) != 0) {
        FAILED("YAML: failed to close file");
    }

    return NEW(YAMLLoadRes, documents, documents_len);
}
