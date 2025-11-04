#!/usr/bin/env bash

# Usage: ./scripts/format.sh [clang_format] -- [cmake_format]

set -e
set -o pipefail

CMAKE_FORMAT_OPTIONS=""
CLANG_FORMAT_OPTIONS=""
CURRENT_OPTIONS_KIND="clang"

for option in "$@"
do
	if [[ $option == "--" ]]
	then
		CURRENT_OPTIONS_KIND="cmake"
	elif [[ $CURRENT_OPTIONS_KIND == "clang" ]]
	then
		CLANG_FORMAT_OPTIONS="$CLANG_FORMAT_OPTIONS $option"
	elif [[ $CURRENT_OPTIONS_KIND == "cmake" ]]
	then
		CMAKE_FORMAT_OPTIONS="$CMAKE_FORMAT_OPTIONS $option"
	else
		echo "error: Unreachable"
		exit 1
	fi
done

DEFAULT_OPTION="-i"

if [[ $CMAKE_FORMAT_OPTIONS == "" ]]
then
	CMAKE_FORMAT_OPTIONS="$DEFAULT_OPTION"
fi

if [[ $CLANG_FORMAT_OPTIONS == "" ]]
then
	CLANG_FORMAT_OPTIONS="$DEFAULT_OPTION"
fi

CLANG_FORMAT=clang-format
CMAKE_FORMAT=cmake-format

./scripts/check_for_missing_command.sh $CLANG_FORMAT
./scripts/check_for_missing_command.sh $CMAKE_FORMAT

PROJECT_FILES=$(./scripts/project_files.sh)
NB_LINES=$(echo "$PROJECT_FILES" | wc -l)
FORMAT_COUNT=0

echo "$PROJECT_FILES" |\
  while IFS= read -r file
  do
	  case $file in
		  *CMakeLists.txt | *.cmake)
			  $CMAKE_FORMAT $CMAKE_FORMAT_OPTIONS $file
			  ;;
		  *.c | *.h)
			  $CLANG_FORMAT $CLANG_FORMAT_OPTIONS $file
			  ;;
		  *)
			  continue
			  ;;
	  esac

	  let "FORMAT_COUNT+=1"
	  echo -ne "[$FORMAT_COUNT/$NB_LINES] $file\033[0K\r"
  done

echo -e ''
echo "All $NB_LINES files are formatted."
