#!/usr/bin/env bash

# Usage: ./scripts/project_files.sh

set -e

FILE=$(mktemp)

EXCLUDE_LIB_DIRECTORIES='-prune -wholename "lib/local/src/llvm-project*" -prune -wholename "lib/local/src/libyaml*" -prune -wholename "lib/local/include*"'

find cmake -name "*.cmake" >> $FILE
find include -name "*.h" >> $FILE
find lib -name "*.h" $EXCLUDE_LIB_DIRECTORIES >> $FILE
find lib -name "*.c" $EXCLUDE_LIB_DIRECTORIES >> $FILE
find lib -name "CMakeLists.txt" $EXCLUDE_LIB_DIRECTORIES >> $FILE
find src -name "*.c" >> $FILE
find src -name "CMakeLists.txt" >> $FILE
find tests -name "*.c" >> $FILE
find tests -name "CMakeLists.txt" >> $FILE
echo './CMakeLists.txt' >> $FILE

cat $FILE
rm $FILE
