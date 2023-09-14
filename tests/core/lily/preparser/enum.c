#include "util.c"

#include <base/test.h>

SIMPLE(enum, {
    RUN_PREPARSER(FILE_ENUM);
    FREE_PREPARSER();
});
