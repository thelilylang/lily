#include "util.c"

#include <base/test.h>

SIMPLE(if, {
    RUN_PREPARSER(FILE_IF);
    FREE_PREPARSER();
});
