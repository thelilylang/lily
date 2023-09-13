#include "util.c"

#include <base/test.h>

SIMPLE(break, {
    RUN_PREPARSER(FILE_BREAK);
    FREE_PREPARSER();
});
