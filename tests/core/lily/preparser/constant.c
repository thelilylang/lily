#include "util.c"

#include <base/test.h>

SIMPLE(constant, {
    RUN_PREPARSER(FILE_CONSTANT);
    FREE_PREPARSER();
});
