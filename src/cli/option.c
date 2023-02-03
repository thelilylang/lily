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

#include <cli/option.h>

#include <stdlib.h>
#include <stdio.h>

// Free Option type (OPTION_KIND_BUILD).
static VARIANT_DESTRUCTOR(Option, build, Option self);

// Free Option type (OPTION_KIND_CC).
static VARIANT_DESTRUCTOR(Option, cc, Option self);

// Free Option type (OPTION_KIND_COMPILE).
static VARIANT_DESTRUCTOR(Option, compile, Option self);

// Free Option type (OPTION_KIND_CPP).
static VARIANT_DESTRUCTOR(Option, cpp, Option self);

// Free Option type (OPTION_KIND_INIT).
static VARIANT_DESTRUCTOR(Option, init, Option self);

// Free Option type (OPTION_KIND_NEW).
static VARIANT_DESTRUCTOR(Option, new, Option self);

// Free Option type (OPTION_KIND_RUN).
static VARIANT_DESTRUCTOR(Option, run, Option self);

// Free Option type (OPTION_KIND_TEST).
static VARIANT_DESTRUCTOR(Option, test, Option self);

// Free Option type (OPTION_KIND_TO).
static VARIANT_DESTRUCTOR(Option, to, Option self);

VARIANT_DESTRUCTOR(Option, build, Option self)
{
	FREE_BUFFER_ITEMS(self.build->buffer, self.build->len, BuildOption);
	FREE(Vec, self.build);
}

VARIANT_DESTRUCTOR(Option, cc, Option self)
{
	FREE_BUFFER_ITEMS(self.cc->buffer, self.cc->len, CcOption);
	FREE(Vec, self.cc);
}

VARIANT_DESTRUCTOR(Option, compile, Option self)
{
	FREE_BUFFER_ITEMS(self.compile->buffer, self.compile->len, CompileOption);
	FREE(Vec, self.compile);
}

VARIANT_DESTRUCTOR(Option, cpp, Option self)
{
	FREE_BUFFER_ITEMS(self.cpp->buffer, self.cpp->len, CppOption);
	FREE(Vec, self.cpp);
}

VARIANT_DESTRUCTOR(Option, init, Option self)
{
	FREE_BUFFER_ITEMS(self.init->buffer, self.init->len, InitOption);
	FREE(Vec, self.init);
}

VARIANT_DESTRUCTOR(Option, new, Option self)
{
	FREE_BUFFER_ITEMS(self.new->buffer, self.new->len, NewOption);
	FREE(Vec, self.new);
}

VARIANT_DESTRUCTOR(Option, run, Option self)
{
	FREE_BUFFER_ITEMS(self.run->buffer, self.run->len, RunOption);
	FREE(Vec, self.run);
}

VARIANT_DESTRUCTOR(Option, test, Option self)
{
	FREE_BUFFER_ITEMS(self.test->buffer, self.test->len, TestOption);
	FREE(Vec, self.test);
}

VARIANT_DESTRUCTOR(Option, to, Option self)
{
	FREE_BUFFER_ITEMS(self.to->buffer, self.to->len, ToOption);
	FREE(Vec, self.to);
}

DESTRUCTOR(Option, Option self)
{
	switch (self.kind) {
		case OPTION_KIND_BUILD:
			FREE_VARIANT(Option, build, self);
			break;
		case OPTION_KIND_CC:
			FREE_VARIANT(Option, cc, self);
			break;
		case OPTION_KIND_COMPILE:
			FREE_VARIANT(Option, compile, self);
			break;
		case OPTION_KIND_CPP:
			FREE_VARIANT(Option, cpp, self);
			break;
		case OPTION_KIND_INIT:
			FREE_VARIANT(Option, init, self);
			break;
		case OPTION_KIND_NEW:
			FREE_VARIANT(Option, new, self);
			break;
		case OPTION_KIND_RUN:
			FREE_VARIANT(Option, run, self);
			break;
		case OPTION_KIND_TEST:
			FREE_VARIANT(Option, test, self);
			break;
		case OPTION_KIND_TO:
			FREE_VARIANT(Option, to, self);
			break;
		default:
			UNREACHABLE("unknown variant");
	}
}
