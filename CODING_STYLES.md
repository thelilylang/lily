# Coding Styles

## C

We using [C23](https://en.cppreference.com/w/c/23) standard. An other thing to know, we use `lily_base` library that's include some data structures, such as File, Vec, Stack, etc.

### Header Guard

When we name a header guard, we use the file path to name the guard. In addition, headers must be written in upper case. Additionally, the guard name must start with `LILY` and must end with the file extension, so most often `H` or `HPP`. Plus, the last two rules are that for example the name of the folder and the file is repeated with the same name without taking into account the extension of the file, we avoid repeating the same name. Finally, the header guard of the project file must be unique (exception in certain cases, like in `tests/core/lily/scanner/util.c`).

```c
// path: include/core/lily/analysis/analysis.h
// guard name: CORE_LILY_ANALYSIS_H (bad)
// guard name: LILY_CORE_LILY_ANALYSIS_ANALYSIS_H (bad)
// guard name: LILY_CORE_LILY_ANALYSIS_H (ok)
```

### Function

When you want to name a function, you must add a suffix `__` + type name. Plus, the base of the function name must be write in snake-case.

```c
typedef struct Person {
    const char *name;
    Uint8 age;
} Person;

const char *
get_name(const Person *self); // bad

const char *
get_name__Person(const Person *self); // ok
```

However, if the function is not associated to a type (enum or struct), add a suffix `__` + name access.

```c
// path: src/str.c
Usize
len__Str(const char *s);
```

NOTE: The prefix must be unique, to avoid any conflicting function name.

#### Parametter

If the function is associated to a type, the first parametter, must be named `self`.

### Global variable

### Macro

### Include

### Enum

### Struct

## C++

We using [C++20](https://en.cppreference.com/w/cpp/20) standard.

## Lily

## Abreviations
