# CImproved (CI)

## Why?

The aim of this project is to make it possible to use generics in C, with automatic generation. It's true that in C it's already possible to use pseudo-generic, but this is done manually and is not really safe for linking. For example, you can easily end up with large projects with duplicate symbols or undefined references, and this is clearly not an elegant way to write generic code.

Here's one of the current ways of doing generics in C:

```c
// ...

#define Vec(T) \
    struct Vec__##T { \
        T* buffer; \
        size_t len; \
        size_t capacity; \
    }; \
    \
    inline struct Vec__##T init__Vec__##T() { \
        return (struct Vec__##T){ .buffer = NULL, .len = 0, .capacity = 4 }; \
    }

// For example:

Vec(int);
Vec(double);
```

It's clearly not a pretty sight, and one of the problems with this way of working is that you can't use all the types (like pointers), so you're forced to alias back and forth to use these types.

The Lily project is currently written without generic, but only with `void*`, but this is clearly not the best way of doing things here either, because problems like bad casting can happen, so the use of memory is clearly not optimized. So, it was all these things that forced me to develop this transpiler in order to be able to have generic in C. 

## About the solution

For the instance, my only desire was to develop a transpiler to be able to generate generic code, automatically in a completely link-safe way, but I'll probably add things in the future to make C even better. The big advantage of CI over solutions like Zig, Rust, Go (ZRG) is that its syntax is very close to C (in fact, you can write C), which makes converting C code to CI very fast and inexpensive. Many people talk about languages like ZRG, as replacements for C (or C++, but that's not the point), but these people often forget that most maintainers of large projects like Linux, GNU, GCC, don't have the time to translate all their source code of several million lines into ZRG, because it would take too long and they need new features. CI's big advantage in this respect is that it can translate the code base of this kind of project as it goes along, not all at once, as it fully supports C syntax ((the aim is to support all standards).

In my case, the CI advantage is that I can translate Lily's source code in a very short time and with very little rewriting effort.

## Features

- Real generic support
- C syntax support
- Data type contexts
- Built-in build system

## Examples

```c
#include <stdio.h>

// Generic function
@T
get.[@T](@T value) {
    return value;
}

int main() {
    printf("%d\n", get.[int](10));
    printf("%s\n", get.[char*]("Hello, World!"));
}
```

You can find more code examples in the following [directory](/examples/core/cc/ci).

## Documentation

You can find the documentation in the following [directory](/docs/core/cc/ci).
