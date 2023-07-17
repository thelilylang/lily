#define Rc(T)        \
    struct           \
    {                \
        T *ptr;      \
        Usize count; \
    }

#define __new__Rc(T, value)      \
    {                            \
        .ptr = value, .count = 0 \
    }

#define ref__Rc(self)   \
    ({                  \
        ++(self).count; \
        self            \
    })

#define __free__Rc(T, self)     \
    do {                        \
        if ((self).count > 0) { \
            --(self).count;     \
            break;              \
        }                       \
        FREE(T, (self).ptr);    \
    } while (0);
