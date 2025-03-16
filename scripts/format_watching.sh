#!/bin/sh

set -e

INOTIFYWAIT=inotifywait
CLANG_FORMAT=clang-format
CMAKE_FORMAT=cmake-format

./scripts/check_for_missing_command.sh $INOTIFYWAIT 
./scripts/check_for_missing_command.sh $CLANG_FORMAT
./scripts/check_for_missing_command.sh $CMAKE_FORMAT

PWD=$(pwd)

function log_format() {
	echo "Format $1"
}

inotifywait -qmr -e attrib --format '%w%f%0' --no-newline "$PWD/" |\
	while IFS= read -r -d '' file
	do
		case $file in
			$PWD/include/*.h | $PWD/src/*.c | $PWD/src/*.cpp)
				$CLANG_FORMAT -i $file
				;;
			$PWD/src/*CMakeLists.txt | $PWD/CMakeLists.txt | $PWD/cmake/*.cmake)
				$CMAKE_FORMAT -i $file
				;;
			*)
				continue
				;;
		esac

		log_format $file
	done
