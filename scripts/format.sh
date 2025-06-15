#!/bin/sh

set -e

CLANG_FORMAT=clang-format
CMAKE_FORMAT=cmake-format

./scripts/check_for_missing_command.sh $CLANG_FORMAT
./scripts/check_for_missing_command.sh $CMAKE_FORMAT

FILE=$(mktemp)

EXCLUDE_LIB_DIRECTORIES='-prune -wholename "lib/local/src/llvm-project*" -prune -wholename "lib/local/src/libyaml*" -prune -wholename "lib/local/include*"'
EXCLUDE_TESTS_DIRECTORIES='-prune -wholename "tests/core/cc/compare*"'

find cmake -name "*.cmake" >> $FILE
find include -name "*.h" >> $FILE
find lib -name "*.h" $EXCLUDE_LIB_DIRECTORIES >> $FILE
find lib -name "*.c" $EXCLUDE_LIB_DIRECTORIES >> $FILE
find lib -name "CMakeLists.txt" $EXCLUDE_LIB_DIRECTORIES >> $FILE
find src -name "*.c" >> $FILE
find src -name "CMakeLists.txt" >> $FILE
find tests -name "*.c" $EXCLUDE_TESTS_DIRECTORIES >> $FILE
find tests -name "CMakeLists.txt" $EXCLUDE_TESTS_DIRECTORIES >> $FILE
echo './CMakeLists.txt' >> $FILE

FILE_CONTENT=$(cat $FILE)
NB_LINES=$(echo "$FILE_CONTENT" | wc -l)
FORMAT_COUNT=0

echo "$FILE_CONTENT" |\
  while IFS= read -r file
  do
	  case $file in
		  *CMakeLists.txt | *.cmake)
			  $CMAKE_FORMAT -i $file
			  ;;
		  *.c | *.h)
			  $CLANG_FORMAT -i $file
			  ;;
		  *)
			  continue
			  ;;
	  esac

	  let "FORMAT_COUNT+=1"
	  echo -ne "[$FORMAT_COUNT/$NB_LINES] $file\033[0K\r"
  done

rm $FILE

echo -e ''
echo "All $NB_LINES files are formatted."
