#include "util.c"

#include <base/test.h>

SIMPLE(for, {
    RUN_PREPARSER(FILE_FOR);
    FREE_PREPARSER();
});
