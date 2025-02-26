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

#include <base/cli/args.h>
#include <base/new.h>
#include <base/string.h>

CliArgs
build__CliArgs(int argc, char **argv)
{
    Vec *args = NEW(Vec);

    for (Usize i = 0; i < argc; ++i) {
        push__Vec(args, argv[i]);
    }

    return args;
}

CliArgs
build_from_string_args__CliArgs(Vec *args_string)
{
    Vec *args = NEW(Vec); // Vec<char* (&)>*

    for (Usize i = 0; i < args_string->len; ++i) {
        String *arg_string = get__Vec(args_string, i);

        push__Vec(args, arg_string->buffer);
    }

    return args;
}
