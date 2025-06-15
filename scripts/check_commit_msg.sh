#!/usr/bin/env bash

# $1: message error
function emit_error {
	echo -e "\x1b[1m\x1b[31merror\x1b[0m: $1\x1b[0m"
	exit 1
}

# $1: message error
# $2: message note
function emit_error_with_note {
	echo -e "\x1b[1m\x1b[31merror\x1b[0m: $1\x1b[0m"
	echo -e "\x1b[1m\x1b[36mnote\x1b[0m: $2\x1b[0m"
	exit 1
}

COMMIT_MESSAGE="$1"
MAX_COMMIT_MESSAGE_LEN=72

ACTIONS="build|chore|ci|feat|fix|docs|perf|refactor|style|test"
TOPICS="base|bin/ci|bin/cic|bin/lily|bin/lilyc|cli/ci|cli/cic|cli/lily|cli/lilyc|command/ci|command/cic|command/lily|command/lilyc|cc|cc/ci|cc/runtime|cpp|docker/dev|ex|global|lib/builtin|lib/cc|lib/local|lib/std|lib/sys|lily|lily/analysis|lily/compiler|lily/diagnostic|lily/functions|lily/mir|lily/package|lily/parser|lily/precompiler|lily/preparser|lily/scanner|lily/shared|lsp|patches|scripts|shared"

COMMIT_MESSAGE_PATTERN="^([a-zA-Z]+)\(([^)]+)\): (.+)$"

if [[ $COMMIT_MESSAGE =~ $COMMIT_MESSAGE_PATTERN ]]
then
	action="${BASH_REMATCH[1]}"
	topic="${BASH_REMATCH[2]}"
	rest_of_the_message="${BASH_REMATCH[3]}"
	short_description=$(echo -e "$rest_of_the_message" | awk -v RS='\n' 'NR==1')

	if [[ ! $action =~ $ACTIONS ]]
	then
		emit_error_with_note "unknown action" "here is a list of all possible actions: $ACTIONS"
	fi

	if [[ ! $topic =~ $TOPICS ]]
	then
		emit_error_with_note "unknown topic, please add this new topic to this script" "here is a list of all possible topics: $TOPICS"
	fi

	if [[ $((${#action} + ${#topic} + ${#short_description})) -gt $MAX_COMMIT_MESSAGE_LEN ]]
	then
		emit_error "the first line of the commit message is too long (more than $MAX_COMMIT_MESSAGE_LEN characters)"
	fi
else
	emit_error "incorrect formatting of the commit message"
fi
