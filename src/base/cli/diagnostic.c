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

#include <base/cli/diagnostic.h>
#include <base/color.h>
#include <base/new.h>
#include <base/print.h>

#include <stdio.h>

static void
print_detail__CliDiagnostic(const CliDiagnostic *self);

void
print_detail__CliDiagnostic(const CliDiagnostic *self)
{
    EPRINTLN("\x1b[1m{Sr}\x1b[0m",
             repeat__String("-", self->full_command->len + 2));
    EPRINTLN("{sa}", BOLD(">"));
    EPRINTLN("{sa}\x1b[1m{S}\x1b[0m", BOLD("> "), self->full_command);
    EPRINT("\x1b[1m> ");

    {
        Usize count_space = 0;
        StringIter iter = NEW(StringIter, self->full_command);

        while (count_space < self->arg_count) {
            char current = next__StringIter(&iter);

            if (current) {
                if (current == ' ') {
                    EPRINT(" ");
                    count_space += 1;
                } else if (self->arg_count == count_space + 1) {
                    EPRINT("^");
                } else {
                    EPRINT(" ");
                }
            } else {
                break;
            }
        }
    }

    EPRINTLN("\x1b[0m");
    EPRINTLN("\x1b[1m{Sr}\x1b[0m",
             repeat__String("-", self->full_command->len + 2));
}

void
emit__CliDiagnostic(const CliDiagnostic *self)
{
    switch (self->kind) {
        case CLI_DIAGNOSTIC_KIND_ERROR:
            EPRINTLN("{sa}{sa}\x1b[1m{s}\x1b[0m",
                     RED_BOLD("error"),
                     BOLD(": "),
                     self->msg);
            break;
        case CLI_DIAGNOSTIC_KIND_NOTE:
            EPRINTLN("{sa}{sa}\x1b[1m{s}\x1b[0m",
                     CYAN_BOLD("note"),
                     BOLD(": "),
                     self->msg);
            break;
        case CLI_DIAGNOSTIC_KIND_WARNING:
            EPRINTLN("{sa}{sa}\x1b[1m{s}\x1b[0m",
                     YELLOW_BOLD("warning"),
                     BOLD(": "),
                     self->msg);
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    EPRINTLN("{sa}\n", CYAN_BOLD("---> at(cli)"));
    print_detail__CliDiagnostic(self);

    switch (self->kind) {
        case CLI_DIAGNOSTIC_KIND_ERROR:
            exit(1);
        default:
            break;
    }
}
