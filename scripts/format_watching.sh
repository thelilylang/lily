#!/bin/sh

INOTIFYWAIT=inotifywait
CLANG_FORMAT=clang-format
CMAKE_FORMAT=cmake-format

function log_missing_command() {
	if [ $? -ne 0 ]
	then
		echo "You need to install $1."
		exit 1
	fi
}

function check_for_missing_command() {
	which $1 > /dev/null 2>&1
	log_missing_command $1 
}

check_for_missing_command $INOTIFYWAIT 
check_for_missing_command $CLANG_FORMAT
check_for_missing_command $CMAKE_FORMAT

set -e

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
