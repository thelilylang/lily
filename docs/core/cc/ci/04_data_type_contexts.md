# Data type contexts

Data type contexts are generally used to add more context and restriction to C data types. Using these contexts will enable you to make the C language a little more secure, with very little rewriting (compared to rewriting it in a language that has nothing to do with C, like Rust).

- `!heap`
- `!non_null`
- `!stack`
- `!trace`
- `!free`

## !heap

The context data type `!heap` is used to declare that a pointer, for example, is allocated on the heap. For example, if we declare the `malloc` function from the standard C library as an `extern`, we can add the context type `!heap` to the `void*` return type.

```c
#include <stddef.h>

extern void* !heap malloc(size_t size);
```

So, what this will do is that this context type will give restrictions to the transpilator on what can be done with a type that has this context. For example, you can't pair a context type with `!stack`, as they are incompatible.

## !non_null

The `!non_null` data type context prevents a pointer from being passed a `NULL` (`(void*)0`) value. It should also be noted that this data type context cannot be used with any data type other than a pointer. Here's an example:

```c
int main() {
    int *p !non_null = NULL;
    //                 ^^^^ Error: You cannot assign a NULL value to a value with incompatible data type context
}
```

## !stack

The `!stack` type context allows you to declare that a pointer has been allocated on the stack. This prevents the pointer from being modified and only allows it to be read. Moreover, this data type context is incompatible with `!heap` and `!free`. Here's an example of code with the `!stack` context data type:

```c
#include <stddef.h>

extern void free(void* !free p);

void modify(char* !heap s, size_t len) {
    for (int i = 0; i < len; ++i) {
        s[i] = i;
    }
}

int main() {
    char* !stack s = "Hello";

    modify(s);
    //     ^ You given an incompatible param  !stack v. !heap

    free(s);
    //   ^ You given an incompatible param !stack v. !free
}
```

## !trace

The `!trace` context type ensures that a pointer cannot be freed while it is being traced.

```c
#include <stddef.h>

extern void* !heap malloc(size_t len);
extern void free(void* !free p);

void do_something(void* !trace p) {
    free(p);
    //   ^ You cannot free this pointer because it's being traced
}

int main() {
    void *p = malloc(sizeof(void*));

    do_something(p);

    free(p);
}
```

This may be a rather simple example, but in reality it's a good thing to be able to make sure that functions that are called in other functions that we call won't do weird and unexpected things like free a pointer. It also makes it easier to understand the role of each function without having to read too much documentation. 

## !free

The context type `!free` is used to declare to the transponder that this pointer will be freed in this function and that after this function call the pointer will no longer be available. Here's an example:

```c
extern void* !heap malloc(size_t len);
extern void free(void* !free p);

int main() {
    void *p = malloc(sizeof(void*));

    free(p);
    free(p);
}
```

This type of context helps to avoid "use after free", or even "double free" situations.
