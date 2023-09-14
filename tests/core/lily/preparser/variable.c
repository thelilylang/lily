#include "util.c"

#include <base/test.h>

SIMPLE(variable, {
    RUN_PREPARSER(FILE_VARIABLE);
    FREE_PREPARSER();
});
