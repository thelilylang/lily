#include "util.c"

#include <base/test.h>

SIMPLE(match, {
    RUN_PREPARSER(FILE_MATCH);
    FREE_PREPARSER();
});
