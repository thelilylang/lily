# Externs

> This directory contains the externs for the external functions that are used in the project, like inline functions. This avoids the need for multiple linkage definitions, and avoids wondering whether you've already declared the inline function you're using as an extern. So every time you define a new inline function that isn't declared with static, you need to add it to the file corresponding to the library to which the file you're modifying belongs.


## Examples

### Create a file in an existing library

If you're creating an inline function in a header file called `src/base/myheader.h` in the `lily_base` library, for example, here's the process you need to follow:

```c
// ...
// src/base/myheader.h
#ifndef LILY_BASE_MYHEADER_H
#define LILY_BASE_MYHEADER_H

inline int add(int x, int y) {
    return x + y;
}

#endif // LILY_BASE_MYHEADER_H
```

Now, to avoid linkage problems such as undefined references (and all situations involving multiple definitions), you need to add the inline function with the storage class extern to the file corresponding to the library to which the file belongs. In our case, it's `src/ex/lib/lily_base.c`.

```c
// src/ex/lib/lily_base.c
// ...
#include <base/myheader.h>

extern inline int add(int x, int y);

// ...
```

### Create a new library 

If you want to create a new library called `mylib`, for example, here's the process you need to follow:

Create a file in `src/ex/lib/mylib.c` (template):

```c
// src/ex/lib/mylib.c
#ifndef LILY_EX_LIB_MYLIB_C
#define LILY_EX_LIB_MYLIB_C
// Include libraries that are linked to this library:
// e.g. the `lily_base` library is linked to this library:
// #include "lily_base.c"
// ...
// Put inline functions with storage class extern here.
// ...
#endif // LILY_EX_LIB_MYLIB_C
```
