# C Improved (CI) 

This tool will allow you to write all the things that are redundant in C, like for example a function to debug a structure, or also a destructor for a structure. Also, in the future, this tool will allow you to type `void*` as a generic type. More details to come.

## Generic

```c
struct Vec<T> {
    T *buffer;
    size_t len;
};
```
