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

#include <base/new.h>

#include <cli/command.h>
#include <cli/parse_command.h>

#include <string.h>

CONSTRUCTOR(ParseCommand, ParseCommand, char *command, char **options)
{
  enum Command command_kind;
  
  if (!(strcmp(command, "build")))
    command_kind = COMMAND_BUILD;
  else if (!strcmp(command, "cc"))
    command_kind = COMMAND_CC;
  else if (!strcmp(command, "compile"))
    command_kind = COMMAND_COMPILE;
  else if (!strcmp(command, "cpp"))
    command_kind = COMMAND_CPP;
  else if (!strcmp(command, "help") || !strcmp(command, "--help") || !strcmp(command, "-h"))
    command_kind = COMMAND_HELP;
  else if (!strcmp(command, "init"))
    command_kind = COMMAND_INIT;
  else if (!strcmp(command, "new"))
    command_kind = COMMAND_NEW;
  else if (!strcmp(command, "run"))
    command_kind = COMMAND_RUN;
  else if (!strcmp(command, "to"))
    command_kind = COMMAND_TO;
  else if (!strcmp(command, "version") || !strcmp(command, "--version") || !strcmp(command, "-v"))
    command_kind = COMMAND_VERSION;
  else
    command_kind = COMMAND_ERROR;
  
  return (ParseCommand){
    .command = command,
    .options = options,
    .command_kind = command_kind,
  };
}
