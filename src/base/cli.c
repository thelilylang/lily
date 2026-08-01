/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

/// @return Vec<CliResult*>*? - NULL when an error has been emitted.
static Vec *
parse_command__Cli(Cli *self, CliCommand *cmd);

/// @return Vec<CliResult*>*? - NULL when an error has been emitted.
static Vec *
parse_option__Cli(Cli *self);

/// @return CliResultValue*? - NULL when the value is absent or on error. Check
/// `self->status` to tell both apart.
static CliResultValue *
parse_value__Cli(Cli *self, const CliValue *value);

// Emit an error diagnostic on `self` and record it in `self->status`. The `_F`
// variant takes ownership of a `format`-allocated message.
#define EMIT_ERROR(msg, count)                             \
    {                                                      \
        CliDiagnostic err = NEW(CliDiagnostic,             \
                                CLI_DIAGNOSTIC_KIND_ERROR, \
                                msg,                       \
                                count,                     \
                                self->full_command);       \
                                                           \
        emit__CliDiagnostic(&err);                         \
                                                           \
        self->status = CLI_PARSE_STATUS_ERROR;             \
    }

#define EMIT_ERROR_F(msg, count)          \
    {                                     \
        char *formatted_msg = msg;        \
                                          \
        EMIT_ERROR(formatted_msg, count); \
                                          \
        lily_free(formatted_msg);         \
    }

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
                      .version = NULL,
                      .full_command = full_command,
                      .args = args,
                      .args_iter = NEW(VecIter, args),
                      .status = CLI_PARSE_STATUS_OK };

    // Add default option
    {
        CliOption *help = NEW(CliOption, CLI_OPTION_ID_HELP, "--help");

        help__CliOption(help, "Print the help");
        default_action__CliOption(
          help, NEW_VARIANT(CliDefaultAction, help, &generate_help__CliHelp));
        short_name__CliOption(help, "-h");

        option__Cli(&self, help);
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
        CliOption *option = NEW(CliOption, CLI_OPTION_ID_VERSION, "--version");

        help__CliOption(option, "Print the version");
        default_action__CliOption(
          option, NEW_VARIANT(CliDefaultAction, version, self->version));
        short_name__CliOption(option, "-v");

        option__Cli(self, option);
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

        EMIT_ERROR("expected command", self->args_iter.count);

        return NULL;
    }

    // Skip program name
    next__VecIter(&self->args_iter);

    CliCommand *current_cmd = NULL;
    char *current_arg = next__VecIter(&self->args_iter);

    if (current_arg) {
        if (self->subcommands->len > 0 && current_arg[0] != '-') {
            current_cmd = get__OrderedHashMap(self->subcommands, current_arg);

            if (!current_cmd) {
                if (!self->value) {
                    EMIT_ERROR_F(
                      format("command not found: `{s}`", current_arg),
                      self->args_iter.count);

                    return NULL;
                }

                // Return to the previous arg.
                --self->args_iter.count;
            }
        } else {
            // Return to the previous arg.
            --self->args_iter.count;
        }
    }

    if (current_cmd) {
        return parse_command__Cli(self, current_cmd);
    }

    return parse_option__Cli(self);
}

Vec *
parse_command__Cli(Cli *self, CliCommand *cmd)
{
    ASSERT(cmd);

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
        CliResult, command, NEW(CliResultCommand, cmd->id, res_cmd_value)));
    char *current = NULL;

// Give up on the current parse, after an error has been emitted or a default
// action has been printed.
#define PARSE_COMMAND_ABORT()                                \
    {                                                        \
        FREE_BUFFER_ITEMS(res->buffer, res->len, CliResult); \
        FREE(Vec, res);                                      \
                                                             \
        return NULL;                                         \
    }

    while ((current = next__VecIter(&self->args_iter))) {
        if (current[0] == '-') {
            CliOption *option = get__OrderedHashMap(cmd->options, current);

            if (option) {
                if (option->default_action) {
                    print__CliDefaultAction(option->default_action, self, cmd);

                    self->status = CLI_PARSE_STATUS_DONE;

                    PARSE_COMMAND_ABORT();
                }

                CliResultValue *option_value = NULL;

                if (option->value) {
                    option_value = parse_value__Cli(self, option->value);

                    if (self->status != CLI_PARSE_STATUS_OK) {
                        PARSE_COMMAND_ABORT();
                    }
                }

                push__Vec(
                  res,
                  NEW_VARIANT(CliResult,
                              option,
                              NEW(CliResultOption, option->id, option_value)));
            } else {
                EMIT_ERROR_F(format("unknown option: `{s}`", current),
                             self->args_iter.count);

                PARSE_COMMAND_ABORT();
            }
        } else {
            if (cmd->value) {
                switch (res_cmd_value->kind) {
                    case CLI_RESULT_VALUE_KIND_SINGLE:
                        if (res_cmd_value->single) {
                            EMIT_ERROR_F(
                              format(
                                "expected one value for this command: `{s}`",
                                cmd->name),
                              self->args_iter.count);

                            PARSE_COMMAND_ABORT();
                        }

                        res_cmd_value->single = current;

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
                EMIT_ERROR_F(
                  format("no values are expected for this comand: `{s}`",
                         cmd->name),
                  self->args_iter.count);

                PARSE_COMMAND_ABORT();
            }
        }
    }

    if (res_cmd_value) {
#define PARSE_COMMAND_CHECK_MULTIPLE_VALUE(multiple)                    \
    if (multiple->len == 0) {                                           \
        EMIT_ERROR_F(                                                   \
          format("expected one or more values for this command: `{s}`", \
                 cmd->name),                                            \
          self->args_iter.count - 1);                                   \
                                                                        \
        PARSE_COMMAND_ABORT();                                          \
    }

        switch (res_cmd_value->kind) {
            case CLI_VALUE_KIND_SINGLE:
                if (!res_cmd_value->single) {
                    EMIT_ERROR_F(
                      format("expected value for this command: `{s}`",
                             cmd->name),
                      self->args_iter.count - 1);

                    PARSE_COMMAND_ABORT();
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

#undef PARSE_COMMAND_CHECK_MULTIPLE_VALUE
    }

#undef PARSE_COMMAND_ABORT

    return res;
}

CliResultValue *
parse_value__Cli(Cli *self, const CliValue *value)
{
#define PARSE_VALUE_CHECK_MULTIPLE_VALUE(multiple)           \
    if (multiple->len == 0) {                                \
        EMIT_ERROR("expected value after option or command", \
                   self->args_iter.count);                   \
                                                             \
        FREE(Vec, multiple);                                 \
                                                             \
        return NULL;                                         \
    }

    switch (value->kind) {
        case CLI_VALUE_KIND_SINGLE: {
            char *current = next__VecIter(&self->args_iter);

            if (current) {
                if (current[0] != '-') {
                    return NEW_VARIANT(
                      CliResultValue, single, value->name, current);
                }

                if (value->is_required) {
                    EMIT_ERROR("expected value after option or command",
                               self->args_iter.count);

                    return NULL;
                }
            } else if (value->is_required) {
                EMIT_ERROR("expected value after option or command",
                           self->args_iter.count);

                return NULL;
            }

            // Back to the previous element
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

#undef PARSE_VALUE_CHECK_MULTIPLE_VALUE
}

Vec *
parse_option__Cli(Cli *self)
{
    Vec *res = NEW(Vec); // Vec<CliResult*>*
    char *current = NULL;

// Give up on the current parse, after an error has been emitted or a default
// action has been printed.
#define PARSE_OPTION_ABORT()                                 \
    {                                                        \
        FREE_BUFFER_ITEMS(res->buffer, res->len, CliResult); \
        FREE(Vec, res);                                      \
                                                             \
        return NULL;                                         \
    }

    while ((current = next__VecIter(&self->args_iter))) {
        if (current[0] == '-') {
            CliOption *option = get__OrderedHashMap(self->options, current);

            if (option) {
                if (option->default_action) {
                    print__CliDefaultAction(option->default_action, self, NULL);

                    self->status = CLI_PARSE_STATUS_DONE;

                    PARSE_OPTION_ABORT();
                }

                CliResultValue *option_value = NULL;

                if (option->value) {
                    option_value = parse_value__Cli(self, option->value);

                    if (self->status != CLI_PARSE_STATUS_OK) {
                        PARSE_OPTION_ABORT();
                    }
                }

                push__Vec(
                  res,
                  NEW_VARIANT(CliResult,
                              option,
                              NEW(CliResultOption, option->id, option_value)));
            } else {
                EMIT_ERROR_F(format("unknown option: `{s}`", current),
                             self->args_iter.count);

                PARSE_OPTION_ABORT();
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
                EMIT_ERROR("no values are expected", self->args_iter.count);

                PARSE_OPTION_ABORT();
            }
        }
    }

    // Check whether the number of values corresponds to the expected value type
    // (SINGLE, MULTIPLE, MULTIPLE_INF).
    if (self->value && self->value->is_required) {
        VecIter iter = NEW(VecIter, res);
        CliResult *current_res = NULL;
        Usize count_value = 0;

        while ((current_res = next__VecIter(&iter))) {
            if (current_res->kind == CLI_RESULT_KIND_VALUE) {
                ++count_value;
            }
        }

        switch (self->value->kind) {
            case CLI_VALUE_KIND_SINGLE:
                if (count_value == 0) {
                    EMIT_ERROR("expected one value", 0);

                    PARSE_OPTION_ABORT();
                } else if (count_value > 1) {
                    EMIT_ERROR("too many values are given", 0);

                    PARSE_OPTION_ABORT();
                }

                break;
            case CLI_VALUE_KIND_MULTIPLE:
            case CLI_VALUE_KIND_MULTIPLE_INF:
                if (count_value == 0) {
                    EMIT_ERROR("expected one or more values", 0);

                    PARSE_OPTION_ABORT();
                }

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

#undef PARSE_OPTION_ABORT

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
