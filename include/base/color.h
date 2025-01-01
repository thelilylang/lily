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

#ifndef LILY_BASE_COLOR_H
#define LILY_BASE_COLOR_H

#include <base/types.h>

#define RED(s) red__Color(s)
#define GREEN(s) green__Color(s)
#define YELLOW(s) yellow__Color(s)
#define BLUE(s) blue__Color(s)
#define MAGENTA(s) magenta__Color(s)
#define CYAN(s) cyan__Color(s)
#define BOLD(s) bold__Style(s)

#define RED_BOLD(s) red_bold__Color(s)
#define GREEN_BOLD(s) green_bold__Color(s)
#define YELLOW_BOLD(s) yellow_bold__Color(s)
#define BLUE_BOLD(s) blue_bold__Color(s)
#define MAGENTA_BOLD(s) magenta_bold__Color(s)
#define CYAN_BOLD(s) cyan_bold__Color(s)

/**
 *
 * @return the formatted char* in red color.
 */
char *
red__Color(char *s);

/**
 *
 * @return the formatted char* in green color.
 */
char *
green__Color(char *s);

/**
 *
 * @return the formatted char* in yellow color.
 */
char *
yellow__Color(char *s);

/**
 * @return the formatted char* in blue color.
 */
char *
blue__Color(char *s);

/**
 *
 * @return the formatted char* in magenta color.
 */
char *
magenta__Color(char *s);

/**
 *
 * @return the formatted char* in cyan color.
 */
char *
cyan__Color(char *s);

/**
 *
 * @return the formatted char* in bold style.
 */
char *
bold__Style(char *s);

/**
 *
 * @return the formatted char* in red color with bold style.
 */
char *
red_bold__Color(char *s);

/**
 *
 * @return the formatted char* in green color with bold style.
 */
char *
green_bold__Color(char *s);

/**
 *
 * @return the formatted char* in yellow color with bold style.
 */
char *
yellow_bold__Color(char *s);

/**
 *
 * @return the formatted char* in blue color with bold style.
 */
char *
blue_bold__Color(char *s);

/**
 *
 * @return the formatted char* in magenta color with bold style.
 */
char *
magenta_bold__Color(char *s);

/**
 *
 * @return the formatted char* in cyan color with bold style.
 */
char *
cyan_bold__Color(char *s);

#endif // LILY_BASE_COLOR_H
