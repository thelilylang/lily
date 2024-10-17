# Generic type

One of CI's biggest features is the ability to create generic functions and types. The syntax for generics is quite simple: we use `@<id>` to call the generic type and `.[@<id>, ...]` to list generics when declaring functions or types. Let's start by creating our first generic function:

```ci
@T create_value.[@T](@T x) {
    return x;
}
```

Continuing with another example, we can declare a Node using a generic structure:

```ci
#include <stddef.h>

struct Node.[@T] {
    @T value;
    struct Node.[@T] *next;
};

int main() {
    struct Node.[int] first = (struct Node.[int]){ .value = 0, .next = NULL };
    struct Node.[int] second = (struct Node.[int]){ .value = 1, .next = NULL };

    first.next = &second;
}
```
