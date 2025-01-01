#!/usr/bin/env bash

# MIT License
#
# Copyright (c) 2022-2025 ArthurPV
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

FILE="./tests/mypkg/main.lily"
COMPILE="./bin/lily compile $FILE"

function print_commands {
	echo "Commands:"

	echo "  b: Build the project"
	echo "  compile: Compile a file"
	echo "  clear: Run the clear command"
	echo "  da: Debug analyzer"
	echo "  dd: Disable debug"
	echo "  dir: Debug IR"
	echo "  dl: Disable local"
	echo "  dp: Debug parser"
	echo "  dpc: Debug precompiler"
	echo "  dpp: Debug preparser"
	echo "  ds: Debug scanner"
	echo "  ed: Enable debug"
	echo "  el: Enable local"
	echo "  #e: Exit to the REPL"
	echo "  file: Set a new value to the file"
	echo "  git: Git command"
	echo "  git>: Run a git repl"
	echo "  #h: Print the help"
	echo "  rupc: Run until precompiler"
	echo "  rupp: Run until preparser"
	echo "  time: Run a command to calculate the time"
	echo "  valgrind: Run valgrind to check the memory leak of a command"
}

function do_nothing {
	echo "do nothing for the moment."
}

function compile {
	./bin/lily compile "$FILE"
}

echo "Welcome to the debug REPL."

print_commands

echo

while [ 1 ]
do	
	echo -n "> "

	read input
	
	case $input in
		"b")
			ninja -C build/Debug
			;;
		"clear")
			clear
			;;
		"compile")
			compile
			;;
		"da")
			do_nothing
			;;
		"dd")
		 	./scripts/patches/disable_debug.sh
			;;
		"dir")
			do_nothing
			;;
		"dl")
		 	./scripts/patches/disable_local.sh
			;;
		"dp")
			do_nothing
			;;
		"dpc")
			do_nothing
			;;
		"dpp")
			do_nothing
			;;
		"ds")
			do_nothing
			;;
		"ed")
		 	./scripts/patches/enable_debug.sh
			;;
		"el")
		 	./scripts/patches/enable_local.sh
			;;
		"#e")
			break
			;;
		"file")
			echo -n "file> "
			read file
			FILE=$file
			COMPILE="./bin/lily compile $FILE"
			echo "A new file has been successfully configured."
			;;
		"git")
			echo -n "git> "
			read args
			git "$args"
			;;
		"git>")
			while [ 1 ]
			do
				echo -n "git> "
				read

				if [ "$REPLY" == "#e" ]
				then
					break
				fi

				git "$REPLY"
			done
			;;
		"#h")
			print_commands
			;;		
		"rupc")
			do_nothing
			;;
		"rupp")
			do_nothing
			;;
		"time")
			echo -n "time> "
			read time_command 
			case $time_command in
				"compile")
					time compile
					;;
				"")
					;;
				*)
					echo "error: bad command"
					;;
			esac
			;;
		"valgrind")
			echo -n "valgrind> "
			read valgrind_command
			case $valgrind_command in
				"compile")
					valgrind --leak-check=full $COMPILE
					;;
				"")
					;;
				*)
					echo "error: bad command"
					;;
			esac
			;;
		"")
			;;
		*)
			echo "error: bad command"
			echo "please consult the command help"
			echo "to see the help, enter '#h'"
			;;
	esac
done
