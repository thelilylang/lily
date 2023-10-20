# C comptime generator

This tool will allow you to write all the things that are redundant in C, like for example a function to debug a structure, or also a destructor for a structure. Also, in the future, this tool will allow you to type `void*` as a generic type. More details to come.

## Eq

```c
// #derive: Eq
typedef struct Person {
    u8 age;
} Person;

int main() {
    Person p = {
        .age = 34
    };

    Person p2 = {   
        .age = 34
    };

    // now you can call EQ(type_name, x, y)
    printf("%d", EQ(Person, p, p2));
}
```