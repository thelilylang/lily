#include "util.c"

#include <base/test.h>

SIMPLE(enum_object, {
    RUN_PREPARSER(FILE_ENUM_OBJECT);
    FREE_PREPARSER();
});
