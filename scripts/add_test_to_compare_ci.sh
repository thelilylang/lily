#!/bin/sh

case $# in
	3)
		STD=$1
		DIR=$2
		TEST_NAME=$3
		;;
	*)
		echo "$0 [std] [dir] [test_name]" 1>&2
		echo -e "\nDetails:"
		echo "  std: c89 | c99 | c11 | c17 | c23"
		exit 1
		;;
esac

BASE_CWD=$(pwd)
BASE_STD_DIR="./tests/core/cc/compare/$STD"
BASE_CI_STD_DIR="./tests/core/cc/compare/ci/$STD"

cd $BASE_STD_DIR

OLD_IFS=$IFS
IFS="/"
CURRENT_PATH="."

read -ra SPLIT_DIR <<< $DIR

for SINGLE_DIR in "${SPLIT_DIR[@]}"
do
	mkdir -p $CURRENT_PATH/$SINGLE_DIR
	CURRENT_PATH="$CURRENT_PATH/$SINGLE_DIR"
done

IFS=$OLD_IFS

cd $BASE_CWD

FULL_BASE_STD_DIR="$BASE_STD_DIR/$DIR"
FULL_BASE_CI_STD_DIR="$BASE_CI_STD_DIR/$DIR"
BACK_COUNT=$(echo -n $BASE_CI_STD_DIR/$DIR | grep -o "/" | wc -l)
TO_LINK_PATH=".."

mkdir -p $FULL_BASE_STD_DIR && touch $FULL_BASE_STD_DIR/$TEST_NAME.c && mkdir -p $FULL_BASE_CI_STD_DIR && cd $FULL_BASE_CI_STD_DIR

for i in $(seq 0 $(($BACK_COUNT - 2)))
do
	TO_LINK_PATH="$TO_LINK_PATH/.."
done

ln -s "$TO_LINK_PATH/$FULL_BASE_STD_DIR/$TEST_NAME.c" "./$TEST_NAME.ci"
