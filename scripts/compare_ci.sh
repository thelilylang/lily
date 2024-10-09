#!/usr/bin/env bash

# NOTE:
# This script checks whether CI (C Improved) is fully compatible
# with C features, by comparing itself with the Clang and GCC compilers.

C89=0
C99=1
C11=2
C17=3
C23=4

BASE_DIR_CI="./tests/core/cc/compare/ci"
DIR_CI=("$BASE_DIR_CI/c89" "$BASE_DIR_CI/c99" "$BASE_DIR_CI/c11" "$BASE_DIR_CI/c17" "$BASE_DIR_CI/c23")

BASE_DIR="./tests/core/cc/compare/"
DIR=("$BASE_DIR/c89" "$BASE_DIR/c99" "$BASE_DIR/c11" "$BASE_DIR/c17" "$BASE_DIR/c23")

CLANG="clang"
GCC="gcc"
CI="./bin/Debug/ci"

# C89, C99, C11, C17, C23
N_FILES=(0 0 0 0 0)

# C89, C99, C11, C17, C23
CLANG_SUCCESS=(0 0 0 0 0)
GCC_SUCCESS=(0 0 0 0 0)
CI_SUCCESS=(0 0 0 0 0)

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

# $1 standard_version_arg
# $2 C*
function process_standard() {
	local dir_c="DIR[$2]"
	local dir_ci="DIR_CI[$2]"

	while IFS= read -r -d '' FILE
	do
		check_status "clang_command -fsyntax-only -std=$1 $FILE" "inc_success CLANG_SUCCESS[$2]" $FILE $CLANG
		check_status "gcc_command -fsyntax-only -std=$1 $FILE" "inc_success GCC_SUCCESS[$2]" $FILE $GCC

		let "N_FILES[$2]+=1"
	done < <(find "${!dir_c}" -type f -print0)

	while IFS= read -r -d '' FILE
	do
		check_status "ci_command --include0 lib/cc/std -f -s $1 $FILE" "inc_success CI_SUCCESS[$2]" $FILE $CI
	done < <(find "${!dir_ci}" -type l -print0)
}

process_standard "c89" $C89
process_standard "c99" $C99
process_standard "c11" $C11
process_standard "c17" $C17
process_standard "c2x" $C23

# $1 program
function summary_program() {
	local success_c89="$1_SUCCESS[$C89]"
	local success_c99="$1_SUCCESS[$C99]"
	local success_c11="$1_SUCCESS[$C11]"
	local success_c17="$1_SUCCESS[$C17]"
	local success_c23="$1_SUCCESS[$C23]"

	echo "$1:"
	echo "C89: ${!success_c89}/${N_FILES[$C89]} = $(echo "scale=2; (${!success_c89} / ${N_FILES[$C89]}) * 100" | bc)%"
	echo "C99: ${!success_c99}/${N_FILES[$C99]} = $(echo "scale=2; (${!success_c99} / ${N_FILES[$C99]}) * 100" | bc)%"
	echo "C11: ${!success_c11}/${N_FILES[$C11]} = $(echo "scale=2; (${!success_c11} / ${N_FILES[$C11]}) * 100" | bc)%"
	echo "C17: ${!success_c17}/${N_FILES[$C17]} = $(echo "scale=2; (${!success_c17} / ${N_FILES[$C17]}) * 100" | bc)%"
	echo "C23: ${!success_c23}/${N_FILES[$C23]} = $(echo "scale=2; (${!success_c23} / ${N_FILES[$C23]}) * 100" | bc)%"
	echo "Total: $(echo "scale=2; (${!success_c89} + ${!success_c99} + ${!success_c11} + ${!success_c17} + ${!success_c23}) / (${N_FILES[$C89]} + ${N_FILES[$C99]} + ${N_FILES[$C11]} + ${N_FILES[$C17]} + ${N_FILES[$C23]}) * 100" | bc)%"
}

echo "Summary:"
summary_program "CLANG"
summary_program "GCC"
summary_program "CI"
