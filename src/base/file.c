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

#define _GNU_SOURCE

#include <base/assert.h>
#include <base/file.h>
#include <base/macros.h>
#include <base/platform.h>
#include <base/types.h>

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(LILY_WINDOWS_OS)
#include <windows.h>
#endif

bool is_directory__Path(const char *path) {
#if defined(LILY_WINDOWS_OS)
  DWORD dwAttrib = GetFileAttributes(path);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
          (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
  DIR *dir = opendir(path);

  if (ENOENT == errno || dir == NULL) {
    return false;
  }

  closedir(dir);
#endif

  return true;
}

char *get_extension__Path(const char *path) {
  bool extension_started = false;
  Usize size = 1;
  char *extension = malloc(1);
  Usize len = strlen(path);

  for (Usize i = 0; i < len; i++) {
    if (path[i] == '.' && path[i + 1 < len ? i + 1 : i] != '.' &&
        path[i + 1 < len ? i + 1 : i] != '/' &&
        path[i + 1 < len ? i + 1 : i] != '\\') {
      extension_started = true;
    }

    if (extension_started) {
      extension = realloc(extension, size + 1);
      extension[size - 1] = path[i];
      extension[size++] = '\0';
    }
  }

  return extension;
}

char *read_file__Path(const char *path) {
  ASSERT(is_directory__Path(path) == false);

  FILE *file = fopen(path, "r");
  char *content = malloc(1);

  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\"", path);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, file) != -1)) {
    content = realloc(content, strlen(content) + strlen(line) + 1);
    strcat(content, line);
  }

  fclose(file);
  free(line);

  content[strlen(content)] = '\0';

  return content;
}
