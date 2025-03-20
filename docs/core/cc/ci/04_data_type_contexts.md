# Data type contexts

Data type contexts are generally used to add more context and restriction to C data types. Using these contexts will enable you to make the C language a little more secure, with very little rewriting (compared to rewriting it in a language that has nothing to do with C, like Rust).

- `!heap`
- `!non_null`
- `!stack`
- `!trace`
- `!index`
- `!realloc`
- `!drop`
- `!dropped`
- `!static`
- `!free`
- `!freed`
- `!undefined`
- `!variant`

## !heap

The context data type `!heap` is used to declare that a pointer, for example, is allocated on the heap. For example, if we declare the `malloc` function from the standard C library as an `extern`, we can add the context type `!heap` to the `void*` return type.

```c
#include <stddef.h>

extern void* !heap malloc(size_t size);
```

So, what this will do is that this context type will give restrictions to the transpiler on what can be done with a type that has this context. For example, you can't pair a context type with `!stack`, as they are incompatible.

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

In addition, `!trace` has other rules to avoid abutting the use of this context:

- It is impossible to assign or return a `!trace` to a different region of code.

e.g.

```c
extern void* !heap malloc(size_t len);

void *do_something(void* !trace p) {
    return p;
}

int main() {
    void *p = malloc(sizeof(void*));

    // Valid, because you're returning the `!trace` to the same region
    // of code on which it was created.
    p = do_something(p);
}
```

```c
extern void* !heap malloc(size_t len);

void do_something2(void* !trace p, void * !trace *s) {
    // Invalid, because you're returning the `!trace` to not the same
    // region of code on which it was created.
    *s = p;
}

void *do_something(void* !trace p) {
    void * !trace s = NULL;

    do_something2(p, &s);
}

int main() {
    void *p = malloc(sizeof(void*));

    p = do_something(p);
}
```

## !index

The context type `!index` is used to make sure the used value is greater or equal than 0.

```c
int main() {
    int !index i = 0;
    char s[] = {'a', 'b'};
    char c = s[i];
}
```

## !realloc

The context type `!realloc` is used to allow to realloc a pointer.

```c
extern void* !heap malloc(size_t len);
extern void* !heap realloc(void * !heap !realloc ptr, size_t size);

int main() {
    void * !heap !realloc p = malloc(sizeof(int));
    p = realloc(p, sizeof(int) * 3);
//  ^ Valid, because we allow to realloc the pointer
}
```

```c
extern void* !heap malloc(size_t len);
extern void* !heap realloc(void * !heap !realloc ptr, size_t size);

int main() {
    void * !heap p = malloc(sizeof(int));
    p = realloc(p, sizeof(int) * 3);
//  ^ Invalid, because we don't allow to realloc the pointer by default
}
```

By default, it is not possible to reallocate a pointer without specifying it. The reason for this is quite simple: it guarantees as far as possible (except perhaps in the case of buffer allocation) that the pointer allocated to the base will live until it is destroyed.

## !drop

The context type `!drop` is used to declare to the transpiler that the struct or union will be dropped at the end of the function. Here's an example:

```c
extern void * !heap malloc(size_t len);

struct S {
    void * !heap p;
};

void drop(struct S !drop *s) {
    // Do something with s
}

int main() {
    struct s s = { .p = malloc(30) };

    drop(&s);
    //    ^ The variable `s` is now marked as `!dropped`
}
```

## !dropped

The context type `!dropped` is a reserved context to the transpiler to declare that the struct or union has been dropped. This context is used to avoid double free situations. Here's an example:

```c
extern void * !heap malloc(size_t len);

struct S {
    void * !heap p;
};

void drop(struct S !drop *s) {
    // Do something with s
}

int main() {
    struct s s = { .p = malloc(30) };

    drop(&s);
    //    ^ The variable `s` is now marked as `!dropped`

    free(s.p);
    //   ^ Error: The variable `s` is marked as `!dropped`
}
```

## !static

## !free

The context type `!free` is used to declare to the transpiler that this pointer will be freed in this function and that after this function call the pointer will no longer be available. Here's an example:

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

## !freed

The context type `!freed` is a reserved context to the transpiler to declare that the pointer has been freed. This context is used to avoid double free situations. Here's an example:

```c
extern void* !heap malloc(size_t len);

int main() {
    void *p = malloc(sizeof(void*));

    free(p);
    //   ^ The variable `p` is now marked as `!freed`

    free(p);
    //   ^ Error: The variable `p` is marked as `!freed`
}
```

## !undefined

The context type `!undefined` is a reserved context to the transpiler to declare a value that is undefined. This context is used to avoid "use of uninitialized value" situations. Here's an example:

```c
int main() {
    int !undefined i;
    int j = i;
    //      ^ Error: You cannot use an undefined value
}
```

## !variant

The context type `!variant` is used to associate an union field with an enum variant.

```c
enum Letter {
    A,
    B,
    C
};

union LetterVariant {
    int a !variant(A);
    int b !variant(B);
    int c !variant(C);
};

int main() {
    union LetterVariant v = { .a = 1 };
    //                         ^ A variant is active
}
```
