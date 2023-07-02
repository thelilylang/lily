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
#include <base/cli.h>
#include <base/cli/diagnostic.h>
#include <base/cli/result.h>
#include <base/format.h>

#include <stdio.h>
#include <stdlib.h>

static Cli *
author__Cli(Cli *self, char *author);

static Cli *
about__Cli(Cli *self, char *about);

static Cli *
subcommand__Cli(Cli *self, CliCommand *subcommand);

static Cli *
option__Cli(Cli *self, CliOption *option);

/// @return Vec<CliResult*>*?
static Vec *
parse__Cli(Cli *self);

/// @return Vec<CliResult*>*?
static Vec *
parse_command__Cli(Cli *self, CliCommand *cmd, const Usize *cmd_id);

/// @return Vec<CliResult*>*?
static Vec *
parse_option__Cli(Cli *self);

static CliResultValue *
parse_value__Cli(Cli *self, const CliValue *value);

CONSTRUCTOR(Cli, Cli, const Vec *args, const char *name)
{
    String *full_command = NEW(String);

    {
        VecIter iter = NEW(VecIter, args);
        char *current = NULL;

        while ((current = next__VecIter(&iter))) {
            push_str__String(full_command, current);
            push__String(full_command, ' ');
        }
    }

    if (full_command->len > 0) {
        pop__String(full_command);
    }

    return (Cli){ .name = name,
                  .subcommands = NEW(OrderedHashMap),
                  .options = NEW(OrderedHashMap),
                  .author = NULL,
                  .value = NULL,
                  .about = NULL,
                  .full_command = full_command,
                  .args = args,
                  .args_iter = NEW(VecIter, args),
                  .$author = &author__Cli,
                  .$about = &about__Cli,
                  .$subcommand = &subcommand__Cli,
                  .$option = &option__Cli,
                  .$parse = &parse__Cli };
}

Cli *
author__Cli(Cli *self, char *author)
{
    ASSERT(!self->author && author);

    self->author = author;
    return self;
}

Cli *
about__Cli(Cli *self, char *about)
{
    ASSERT(!self->about && about);

    self->about = about;
    return self;
}

Cli *
subcommand__Cli(Cli *self, CliCommand *subcommand)
{
    ASSERT(subcommand);

    insert__OrderedHashMap(
      self->subcommands, (char *)subcommand->name, subcommand);
    return self;
}

Cli *
option__Cli(Cli *self, CliOption *option)
{
    ASSERT(option);

    insert__OrderedHashMap(self->options, (char *)option->name, option);
    return self;
}

Vec *
parse__Cli(Cli *self)
{
    // Skip program name
    next__VecIter(&self->args_iter);

    Usize *cmd_id = NULL;
    CliCommand *current_cmd = NULL;
    char *current_arg = next__VecIter(&self->args_iter);

    if (current_arg) {
        if (self->subcommands->len > 0 && current_arg[0] != '-') {
            current_cmd = get__OrderedHashMap(self->subcommands, current_arg);

            if (current_cmd) {
                cmd_id = (Usize *)get_id__OrderedHashMap(self->subcommands,
                                                         current_arg);
            } else {
                char *msg = format("command not found: `{s}`", current_arg);

                CliDiagnostic err = NEW(CliDiagnostic,
                                        CLI_DIAGNOSTIC_KIND_ERROR,
                                        msg,
                                        self->args_iter.count,
                                        self->full_command);

                emit__CliDiagnostic(&err);
            }
        } else {
            // Return to the previous arg.
            --self->args_iter.count;
        }
    } else {
        return NULL;
    }

    if (current_cmd) {
        return parse_command__Cli(self, current_cmd, cmd_id);
    }

    return parse_option__Cli(self);
}

Vec *
parse_command__Cli(Cli *self, CliCommand *cmd, const Usize *cmd_id)
{
    ASSERT(cmd && cmd_id);

    if (cmd->deferred) {
        cmd->deferred(cmd);
    }

    CliResultValue *res_cmd_value = NULL;

    if (cmd->value) {
        switch (cmd->value->kind) {
            case CLI_VALUE_KIND_SINGLE:
                res_cmd_value =
                  NEW_VARIANT(CliResultValue, single, cmd->value->name, NULL);
                break;
            case CLI_VALUE_KIND_MULTIPLE:
                res_cmd_value = NEW_VARIANT(
                  CliResultValue, multiple, cmd->value->name, NEW(Vec));
                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    Vec *res = init__Vec(
      1,
      NEW_VARIANT(
        CliResult, command, NEW(CliResultCommand, *cmd_id, res_cmd_value)));
    char *current = NULL;

    while ((current = next__VecIter(&self->args_iter))) {
        if (current[0] == '-') {
            CliOption *option = get__OrderedHashMap(cmd->options, current);

            if (option) {
                const Usize *option_id =
                  get_id__OrderedHashMap(cmd->options, current);

                if (option->value) {
                    push__Vec(
                      res,
                      NEW_VARIANT(CliResult,
                                  option,
                                  NEW(CliResultOption,
                                      *option_id,
                                      parse_value__Cli(self, option->value))));
                } else {
                    push__Vec(
                      res,
                      NEW_VARIANT(CliResult,
                                  option,
                                  NEW(CliResultOption, *option_id, NULL)));
                }

            } else {
                char *msg = format("unknown option: `{s}`", current);

                CliDiagnostic err = NEW(CliDiagnostic,
                                        CLI_DIAGNOSTIC_KIND_ERROR,
                                        msg,
                                        self->args_iter.count,
                                        self->full_command);

                emit__CliDiagnostic(&err);
            }
        } else {
            if (cmd->value) {
                switch (res_cmd_value->kind) {
                    case CLI_RESULT_VALUE_KIND_SINGLE:
                        if (!res_cmd_value->single) {
                            res_cmd_value->single = current;
                        } else {
                            char *msg = format(
                              "expected one value for this command: `{s}`",
                              cmd->name);

                            CliDiagnostic err = NEW(CliDiagnostic,
                                                    CLI_DIAGNOSTIC_KIND_ERROR,
                                                    msg,
                                                    self->args_iter.count,
                                                    self->full_command);

                            emit__CliDiagnostic(&err);
                        }

                        break;
                    case CLI_RESULT_VALUE_KIND_MULTIPLE:
                        push__Vec(res_cmd_value->multiple, current);

                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }
            } else {
                char *msg = format(
                  "no values are expected for this comand: `{s}`", cmd->name);

                CliDiagnostic err = NEW(CliDiagnostic,
                                        CLI_DIAGNOSTIC_KIND_ERROR,
                                        msg,
                                        self->args_iter.count,
                                        self->full_command);

                emit__CliDiagnostic(&err);
            }
        }
    }

    if (res_cmd_value) {
        switch (res_cmd_value->kind) {
            case CLI_VALUE_KIND_SINGLE:
                if (!res_cmd_value->single) {
                    char *msg = format("expected value for this command: `{s}`",
                                       cmd->name);

                    CliDiagnostic err = NEW(CliDiagnostic,
                                            CLI_DIAGNOSTIC_KIND_ERROR,
                                            msg,
                                            self->args_iter.count,
                                            self->full_command);

                    emit__CliDiagnostic(&err);
                }

                break;
            case CLI_VALUE_KIND_MULTIPLE:
                if (res_cmd_value->multiple->len == 0) {
                    char *msg = format("expected value for this command: `{s}`",
                                       cmd->name);

                    CliDiagnostic err = NEW(CliDiagnostic,
                                            CLI_DIAGNOSTIC_KIND_ERROR,
                                            msg,
                                            self->args_iter.count,
                                            self->full_command);

                    emit__CliDiagnostic(&err);
                }

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    return res;
}

CliResultValue *
parse_value__Cli(Cli *self, const CliValue *value)
{
    switch (value->kind) {
        case CLI_VALUE_KIND_SINGLE: {
            char *current = next__VecIter(&self->args_iter);

            if (current) {
                if (current[0] == '-') {
                    if (value->is_required) {
                        CliDiagnostic err =
                          NEW(CliDiagnostic,
                              CLI_DIAGNOSTIC_KIND_ERROR,
                              "expected value after option or command",
                              self->args_iter.count,
                              self->full_command);

                        emit__CliDiagnostic(&err);
                    } else {
                        --self->args_iter.count;

                        return NULL;
                    }
                } else {
                    return NEW_VARIANT(
                      CliResultValue, single, value->name, current);
                }
            } else if (value->is_required) {
                CliDiagnostic err =
                  NEW(CliDiagnostic,
                      CLI_DIAGNOSTIC_KIND_ERROR,
                      "expected value after option or command",
                      self->args_iter.count,
                      self->full_command);

                emit__CliDiagnostic(&err);
            }

            --self->args_iter.count;

            return NULL;
        }
        case CLI_VALUE_KIND_MULTIPLE: {
            Vec *multiple = NEW(Vec); // Vec<char*>*

            while (1) {
                char *current = next__VecIter(&self->args_iter);

                if (current) {
                    if (current[0] == '-') {
                        // Back to the previous element
                        --self->args_iter.count;
                        break;
                    } else {
                        push__Vec(multiple, current);
                    }
                } else {
                    break;
                }
            }

            if (multiple->len == 0) {
                CliDiagnostic err =
                  NEW(CliDiagnostic,
                      CLI_DIAGNOSTIC_KIND_ERROR,
                      "expected value after option or command",
                      self->args_iter.count,
                      self->full_command);

                emit__CliDiagnostic(&err);
            }

            return NEW_VARIANT(CliResultValue, multiple, value->name, multiple);
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

Vec *
parse_option__Cli(Cli *self)
{
    return NULL;
}

DESTRUCTOR(Cli, const Cli *self)
{
    FREE(String, self->full_command);

    FREE_ORD_HASHMAP_VALUES(self->subcommands, CliCommand);
    FREE(OrderedHashMap, self->subcommands);

    FREE_ORD_HASHMAP_VALUES(self->options, CliOption);
    FREE(OrderedHashMap, self->options);

    if (self->value) {
        FREE(CliValue, self->value);
    }
}
