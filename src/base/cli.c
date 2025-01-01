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

#include <base/assert.h>
#include <base/cli.h>
#include <base/cli/diagnostic.h>
#include <base/cli/help.h>
#include <base/cli/result.h>
#include <base/format.h>
#include <base/print.h>

#include <stdio.h>
#include <stdlib.h>

static Cli *
author__Cli(Cli *self, char *author);

static Cli *
about__Cli(Cli *self, char *about);

static Cli *
version__Cli(Cli *self, char *about);

static Cli *
subcommand__Cli(Cli *self, CliCommand *subcommand);

static Cli *
option__Cli(Cli *self, CliOption *option);

static Cli *
single_value__Cli(Cli *self, char *name, bool is_required);

static Cli *
multiple_value__Cli(Cli *self, char *name, bool is_required);

static Cli *
multiple_inf_value__Cli(Cli *self, char *name, bool is_required);

/// @return Vec<CliResult*>*
static Vec *
parse__Cli(Cli *self);

/// @return Vec<CliResult*>*
static Vec *
parse_command__Cli(Cli *self, CliCommand *cmd, const Usize *cmd_id);

/// @return Vec<CliResult*>*
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

    Cli self = (Cli){ .name = name,
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
                      .$version = &version__Cli,
                      .$subcommand = &subcommand__Cli,
                      .$option = &option__Cli,
                      .$single_value = &single_value__Cli,
                      .$multiple_value = &multiple_value__Cli,
                      .$multiple_inf_value = &multiple_inf_value__Cli,
                      .$parse = &parse__Cli };

    // Add default option
    {
        CliOption *help = NEW(CliOption, "--help");
        help->$help(help, "Print the help")
          ->$default_action(
            help, NEW_VARIANT(CliDefaultAction, help, &generate_help__CliHelp))
          ->$short_name(help, "-h");

        self.$option(&self, help);
    }

    return self;
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
version__Cli(Cli *self, char *version)
{
    ASSERT(!self->version && version);

    self->version = version;

    // Add version option
    {
        CliOption *version = NEW(CliOption, "--version");
        version->$help(version, "Print the version")
          ->$default_action(
            version, NEW_VARIANT(CliDefaultAction, version, self->version))
          ->$short_name(version, "-v");

        self->$option(self, version);
    }

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

    if (option->short_name) {
        insert__OrderedHashMap(
          self->options, option->short_name, ref__CliOption(option));
    }

    insert__OrderedHashMap(self->options, (char *)option->name, option);

    return self;
}

Cli *
single_value__Cli(Cli *self, char *name, bool is_required)
{
    ASSERT(!self->value && name);

    self->value = NEW(CliValue, CLI_VALUE_KIND_SINGLE, name, is_required);
    return self;
}

Cli *
multiple_value__Cli(Cli *self, char *name, bool is_required)
{
    ASSERT(!self->value && name);

    self->value = NEW(CliValue, CLI_VALUE_KIND_MULTIPLE, name, is_required);
    return self;
}

Cli *
multiple_inf_value__Cli(Cli *self, char *name, bool is_required)
{
    ASSERT(!self->value && name);

    self->value = NEW(CliValue, CLI_VALUE_KIND_MULTIPLE_INF, name, is_required);
    return self;
}

Vec *
parse__Cli(Cli *self)
{
    if (self->args->len == 1 && self->subcommands->len > 0) {
        PRINTLN("{Sr}", generate_help__CliHelp(self, NULL));

        CliDiagnostic err = NEW(CliDiagnostic,
                                CLI_DIAGNOSTIC_KIND_ERROR,
                                "expected command",
                                self->args_iter.count,
                                self->full_command);

        emit__CliDiagnostic(&err);
    }

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
                if (!self->value) {
                    char *msg = format("command not found: `{s}`", current_arg);

                    CliDiagnostic err = NEW(CliDiagnostic,
                                            CLI_DIAGNOSTIC_KIND_ERROR,
                                            msg,
                                            self->args_iter.count,
                                            self->full_command);

                    emit__CliDiagnostic(&err);
                } else {
                    // Return to the previous arg.
                    --self->args_iter.count;
                }
            }
        } else {
            // Return to the previous arg.
            --self->args_iter.count;
        }
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
            case CLI_VALUE_KIND_MULTIPLE_INF:
                res_cmd_value = NEW_VARIANT(
                  CliResultValue, multiple_inf, cmd->value->name, NEW(Vec));
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
                if (option->default_action) {
                    print__CliDefaultAction(option->default_action, self, cmd);
                }

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
                    case CLI_RESULT_VALUE_KIND_MULTIPLE_INF:
                        push__Vec(res_cmd_value->multiple_inf, current);

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
#define PARSE_COMMAND_CHECK_MULTIPLE_VALUE(multiple)                         \
    if (multiple->len == 0) {                                                \
        char *msg = format(                                                  \
          "expected one or more values for this command: `{s}`", cmd->name); \
                                                                             \
        CliDiagnostic err = NEW(CliDiagnostic,                               \
                                CLI_DIAGNOSTIC_KIND_ERROR,                   \
                                msg,                                         \
                                self->args_iter.count - 1,                   \
                                self->full_command);                         \
                                                                             \
        emit__CliDiagnostic(&err);                                           \
    }

        switch (res_cmd_value->kind) {
            case CLI_VALUE_KIND_SINGLE:
                if (!res_cmd_value->single) {
                    char *msg = format("expected value for this command: `{s}`",
                                       cmd->name);

                    CliDiagnostic err = NEW(CliDiagnostic,
                                            CLI_DIAGNOSTIC_KIND_ERROR,
                                            msg,
                                            self->args_iter.count - 1,
                                            self->full_command);

                    emit__CliDiagnostic(&err);
                }

                break;
            case CLI_VALUE_KIND_MULTIPLE:
                PARSE_COMMAND_CHECK_MULTIPLE_VALUE(res_cmd_value->multiple);
                break;
            case CLI_VALUE_KIND_MULTIPLE_INF:
                PARSE_COMMAND_CHECK_MULTIPLE_VALUE(res_cmd_value->multiple_inf);
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
#define PARSE_VALUE_CHECK_MULTIPLE_VALUE(multiple)                        \
    if (multiple->len == 0) {                                             \
        CliDiagnostic err = NEW(CliDiagnostic,                            \
                                CLI_DIAGNOSTIC_KIND_ERROR,                \
                                "expected value after option or command", \
                                self->args_iter.count,                    \
                                self->full_command);                      \
                                                                          \
        emit__CliDiagnostic(&err);                                        \
    }

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

            PARSE_VALUE_CHECK_MULTIPLE_VALUE(multiple);

            return NEW_VARIANT(CliResultValue, multiple, value->name, multiple);
        }
        case CLI_VALUE_KIND_MULTIPLE_INF: {
            Vec *multiple_inf = NEW(Vec); // Vec<char*>*

            while (1) {
                char *current = next__VecIter(&self->args_iter);

                if (current) {
                    push__Vec(multiple_inf, current);
                } else {
                    break;
                }
            }

            PARSE_VALUE_CHECK_MULTIPLE_VALUE(multiple_inf);

            return NEW_VARIANT(
              CliResultValue, multiple_inf, value->name, multiple_inf);
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

Vec *
parse_option__Cli(Cli *self)
{
    Vec *res = NEW(Vec); // Vec<CliResult*>*
    char *current = NULL;

    while ((current = next__VecIter(&self->args_iter))) {
        if (current[0] == '-') {
            CliOption *option = get__OrderedHashMap(self->options, current);

            if (option) {
                if (option->default_action) {
                    print__CliDefaultAction(option->default_action, self, NULL);
                }

                const Usize *option_id =
                  get_id__OrderedHashMap(self->options, current);

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
            if (self->value) {
                push__Vec(res,
                          NEW_VARIANT(CliResult,
                                      value,
                                      NEW_VARIANT(CliResultValue,
                                                  single,
                                                  self->value->name,
                                                  current)));
            } else {
                CliDiagnostic err = NEW(CliDiagnostic,
                                        CLI_DIAGNOSTIC_KIND_ERROR,
                                        "no values are expected",
                                        self->args_iter.count,
                                        self->full_command);

                emit__CliDiagnostic(&err);
            }
        }
    }

    // Check whether the number of values corresponds to the expected value type
    // (SINGLE, MULTIPLE, MULTIPLE_INF).
    if (self->value) {
        VecIter iter = NEW(VecIter, res);
        CliResult *current = NULL;
        Usize count_value = 0;

        while ((current = next__VecIter(&iter))) {
            if (current->kind == CLI_RESULT_KIND_VALUE) {
                ++count_value;
            }
        }

        switch (self->value->kind) {
            case CLI_VALUE_KIND_SINGLE:
                if (self->value->is_required) {
                    if (count_value == 0) {
                        CliDiagnostic err = NEW(CliDiagnostic,
                                                CLI_DIAGNOSTIC_KIND_ERROR,
                                                "expected one value",
                                                0,
                                                self->full_command);

                        emit__CliDiagnostic(&err);
                    } else if (count_value > 1) {
                        CliDiagnostic err = NEW(CliDiagnostic,
                                                CLI_DIAGNOSTIC_KIND_ERROR,
                                                "too many values are given",
                                                0,
                                                self->full_command);

                        emit__CliDiagnostic(&err);
                    }
                }

                break;
            case CLI_VALUE_KIND_MULTIPLE:
            case CLI_VALUE_KIND_MULTIPLE_INF:
                if (self->value->is_required) {
                    if (count_value == 0) {
                        CliDiagnostic err = NEW(CliDiagnostic,
                                                CLI_DIAGNOSTIC_KIND_ERROR,
                                                "expected one or more values",
                                                0,
                                                self->full_command);

                        emit__CliDiagnostic(&err);
                    }
                }

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    return res;
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
