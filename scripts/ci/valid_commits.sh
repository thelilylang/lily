#!/usr/bin/env bash

set -e

ORIGIN_HEAD_HASH=$(git rev-parse origin/main)
HEAD_HASH=$(git rev-parse HEAD)
COMMITS=$(git log --format=%s $HEAD_HASH --not $ORIGIN_HEAD_HASH)
COMMITS_LEN=$(echo -n "$COMMITS" | wc -c)

if [ $COMMITS_LEN -gt 0 ]
then
	printf "%s\n" "$COMMITS" | while read -r commit
	do
		echo "${commit:0:72} ... OK"
		./scripts/check_commit_msg.sh "$commit"
	done

	echo "success: all commits seem well formatted."
fi
