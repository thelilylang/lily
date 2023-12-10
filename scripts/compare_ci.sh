#!/bin/sh

# NOTE:
# This script checks whether CI (C Improved) is fully compatible
# with C features, by comparing itself with the Clang and GCC compilers.

DIR_CI_C89="./tests/core/cc/compare/ci/c89"
DIR_CI_C99="./tests/core/cc/compare/ci/c99"
DIR_CI_C11="./tests/core/cc/compare/ci/c11"
DIR_CI_C17="./tests/core/cc/compare/ci/c17"
DIR_CI_C23="./tests/core/cc/compare/ci/c23"

DIR_C89="./tests/core/cc/compare/c89"
DIR_C99="./tests/core/cc/compare/c99"
DIR_C11="./tests/core/cc/compare/c11"
DIR_C17="./tests/core/cc/compare/c17"
DIR_C23="./tests/core/cc/compare/c23"

CLANG="clang"
GCC="gcc"
CI="./bin/ci"

N_FILES_C89=0
N_FILES_C99=0
N_FILES_C11=0
N_FILES_C17=0
N_FILES_C23=0

CLANG_SUCCESS_C89=0
CLANG_SUCCESS_C99=0
CLANG_SUCCESS_C11=0
CLANG_SUCCESS_C17=0
CLANG_SUCCESS_C23=0

GCC_SUCCESS_C89=0
GCC_SUCCESS_C99=0
GCC_SUCCESS_C11=0
GCC_SUCCESS_C17=0
GCC_SUCCESS_C23=0

CI_SUCCESS_C89=0
CI_SUCCESS_C99=0
CI_SUCCESS_C11=0
CI_SUCCESS_C17=0
CI_SUCCESS_C23=0

# $* options
function clang_command() {
	eval "$CLANG $*"
}

# $* options
function gcc_command() {
	eval "$GCC $*"
}

# $* options
function ci_command() {
	eval "$CI $*"
}

# $1 success_variable_name
function inc_success() {
	let "${1}+=1"
}

# $1 command
# $2 inc_success
# $3 file
# $4 program
function check_status() {
	eval $1 > /dev/null

	local status=$?

	if [ $status -eq 0 ]
	then
		echo -e "\x1b[32mOK($4) $3...\x1b[0m"
		eval $2
	else
		echo -e "\x1b[31mFAILED($4) $3...\x1b[0m"
	fi
}

# $1 nb_standard_version
# $2 standard_version_arg
function process_standard() {
	local dir_c="DIR_C$1"
	local dir_ci="DIR_CI_C$1"

	while IFS= read -r -d '' FILE
	do
		check_status "clang_command -fsyntax-only -std=$2 $FILE" "inc_success CLANG_SUCCESS_C$1" $FILE $CLANG
		check_status "gcc_command -fsyntax-only -std=$2 $FILE" "inc_success GCC_SUCCESS_C$1" $FILE $GCC

		let "N_FILES_C$1+=1"
	done < <(find "${!dir_c}" -type f -print0)

	while IFS= read -r -d '' FILE
	do
		check_status "ci_command --syntax -std=c$1 $FILE" "inc_success CI_SUCCESS_C$1" $FILE $CI
	done < <(find "${!dir_ci}" -type l -print0)
}

process_standard "89" "c89"
process_standard "99" "c99"
process_standard "11" "c11"
process_standard "17" "c17"
process_standard "23" "c2x"

echo "Summary:"
echo "Clang:"
echo "C89: $CLANG_SUCCESS_C89/$N_FILES_C89 = $(($CLANG_SUCCESS_C89/$N_FILES_C89))"
echo "C99: $CLANG_SUCCESS_C99/$N_FILES_C99 = $(($CLANG_SUCCESS_C99/$N_FILES_C99))"
echo "C11: $CLANG_SUCCESS_C11/$N_FILES_C11 = $(($CLANG_SUCCESS_C11/$N_FILES_C11))"
echo "C17: $CLANG_SUCCESS_C17/$N_FILES_C17 = $(($CLANG_SUCCESS_C17/$N_FILES_C17))"
echo "C23: $CLANG_SUCCESS_C23/$N_FILES_C23 = $(($CLANG_SUCCESS_C23/$N_FILES_C23))"
echo "Total: $(($(($(($CLANG_SUCCESS_C89 + $CLANG_SUCCESS_C99 + $CLANG_SUCCESS_C11 + $CLANG_SUCCESS_C17 + $CLANG_SUCCESS_C23)) / $(($N_FILES_C89 + $N_FILES_C99 + $N_FILES_C11 + $N_FILES_C17 + $N_FILES_C23)))) * 100))%"
echo "GCC:"
echo "C89: $GCC_SUCCESS_C89/$N_FILES_C89 = $(($GCC_SUCCESS_C89/$N_FILES_C89))"
echo "C99: $GCC_SUCCESS_C99/$N_FILES_C99 = $(($GCC_SUCCESS_C99/$N_FILES_C99))"
echo "C11: $GCC_SUCCESS_C11/$N_FILES_C11 = $(($GCC_SUCCESS_C11/$N_FILES_C11))"
echo "C17: $GCC_SUCCESS_C17/$N_FILES_C17 = $(($GCC_SUCCESS_C17/$N_FILES_C17))"
echo "C23: $GCC_SUCCESS_C23/$N_FILES_C23 = $(($GCC_SUCCESS_C23/$N_FILES_C23))"
echo "Total: $(($(($(($GCC_SUCCESS_C89 + $GCC_SUCCESS_C99 + $GCC_SUCCESS_C11 + $GCC_SUCCESS_C17 + $GCC_SUCCESS_C23)) / $(($N_FILES_C89 + $N_FILES_C99 + $N_FILES_C11 + $N_FILES_C17 + $N_FILES_C23)))) * 100))%"
echo "CI:"
echo "C89: $CI_SUCCESS_C89/$N_FILES_C89 = $(($CI_SUCCESS_C89/$N_FILES_C89))"
echo "C99: $CI_SUCCESS_C99/$N_FILES_C99 = $(($CI_SUCCESS_C99/$N_FILES_C99))"
echo "C11: $CI_SUCCESS_C11/$N_FILES_C11 = $(($CI_SUCCESS_C11/$N_FILES_C11))"
echo "C17: $CI_SUCCESS_C17/$N_FILES_C17 = $(($CI_SUCCESS_C17/$N_FILES_C17))"
echo "C23: $CI_SUCCESS_C23/$N_FILES_C23 = $(($CI_SUCCESS_C23/$N_FILES_C23))"
echo "Total: $(($(($(($CI_SUCCESS_C89 + $CI_SUCCESS_C99 + $CI_SUCCESS_C11 + $CI_SUCCESS_C17 + $CI_SUCCESS_C23)) / $(($N_FILES_C89 + $N_FILES_C99 + $N_FILES_C11 + $N_FILES_C17 + $N_FILES_C23)))) * 100))%"
