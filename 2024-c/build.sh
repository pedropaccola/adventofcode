#!/usr/bin/env bash

if [[ ! -f "$1" ]]; then
	echo "File \"${1}\" not found" >&2
	exit 1
fi

if [[ ! "$1" =~ .*\.c$ ]]; then
	echo "File \"${1}\" is not a .c file" >&2
	exit 1
fi

OBJECTS=(
	util.c
)

gcc -std=c23 -Wall -Werror "$1" "${OBJECTS[@]}" -o program.out

echo "Running test file"
exec ./program.out < "${1%.c}.example"
