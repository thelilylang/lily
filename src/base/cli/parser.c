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

#include <base/assert.h>
#include <base/cli/parser.h>
#include <base/cli/result/command.h>
#include <base/cli/result/option.h>
#include <base/format.h>

#include <cli/emit.h>

#include <stdio.h>
#include <stdlib.h>

CliResultCommand
run__CliParser(const Cli *self, Usize command_id, const SizedArray *args)
{
    CliCommand *command =
      get_from_id__OrderedHashMap(self->commands, command_id);
    SizedArrayIter iter = NEW(SizedArrayIter, args);
    char *current = NULL;
    CliOption *current_option = NULL;
    Usize *current_id = NULL;
    Vec *options =
      command->options ? NEW(Vec) : NULL; // Vec<CliResultOption*>*?
    Vec *values = command->has_value != CLI_VALUE_KIND_NONE
                    ? NEW(Vec)
                    : NULL; // Vec<char*>*?
    bool parse_value =
      !command->options && command->has_value != CLI_VALUE_KIND_NONE ? true
                                                                     : false;

    if (!command->options && command->has_value == CLI_VALUE_KIND_NONE &&
        args->len > 0) {
        EMIT_ERROR("no options and values are expected");
        exit(1);
    }

    while ((current = next__SizedArrayIter(&iter))) {
        if (current[0] == '-') {
            if (!options) {
                EMIT_ERROR("no options are expected for this command");
                exit(1);
            }

            if (parse_value) {
                EMIT_ERROR("expected to parse option");
                exit(1);
            }

            current_option = get__OrderedHashMap(command->options, current);

            if (current_option) {
                current_id =
                  (Usize *)get_id__OrderedHashMap(command->options, current);

                ASSERT(current_id);

                switch (current_option->has_value) {
                    case CLI_VALUE_KIND_SINGLE: {
                        char *value = next__SizedArrayIter(&iter);

                        if (value) {
                            push__Vec(
                              options,
                              NEW(CliResultOption,
                                  *current_id,
                                  NEW_VARIANT(CliValue, single, value)));
                        } else {
                            EMIT_ERROR("expected value for this option");
                            exit(1);
                        }

                        break;
                    }
                    case CLI_VALUE_KIND_MULTIPLE: {
                        char *current_value = NULL;
                        Vec *multiple = NEW(Vec);

                        while ((current_value = next__SizedArrayIter(&iter))) {
                            push__Vec(multiple, current_value);
                        }

                        if (multiple->len == 0) {
                            EMIT_ERROR("expected value for this option");
                            exit(1);
                        }

                        push__Vec(
                          options,
                          NEW(CliResultOption,
                              *current_id,
                              NEW_VARIANT(CliValue, multiple, multiple)));

                        break;
                    }
                    case CLI_VALUE_KIND_NONE:
                        push__Vec(options,
                                  NEW(CliResultOption,
                                      *current_id,
                                      NEW_VARIANT(CliValue, none)));

                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }
            } else {
                char *msg = format("unknown option `{s}`", current);

                EMIT_ERROR(msg);
                lily_free(msg);
                exit(1);
            }
        } else {
            if (!values) {
                EMIT_ERROR("no values are expected for this command");
                exit(1);
            }

            push__Vec(values, current);
        }
    }

    return NEW(CliResultCommand, command_id, options, values);
}
