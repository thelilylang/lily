#!/bin/sh

# MIT License
#
# Copyright (c) 2022-2023 ArthurPV
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

function print_commands {
	echo "Commands:"

	echo "  b: Build the project"
	echo "  compile: Compile a file"
	echo "  da: Debug analyzer"
	echo "  dd: Disable debug"
	echo "  dir: Debug IR"
	echo "  dp: Debug parser"
	echo "  dpc: Debug precompiler"
	echo "  dpp: Debug preparser"
	echo "  ds: Debug scanner"
	echo "  ed: Enable debug"
	echo "  exit: Exit to the REPL"
	echo "  file: Set a new value to the file"
	echo "  git: Git command"
	echo "  git>: Run a git repl"
	echo "  help: Print the help"
	echo "  rupc: Run until precompiler"
	echo "  rupp: Run until preparser"
}

function do_nothing {
	echo "do nothing for the moment."
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
		"compile")
			./build/Debug/lily compile $FILE
			;;
		"da")
			do_nothing
			;;
		"dd")
			cd patches && patch ../include/base/macros.h disable_debug.patch && cd ..
			;;
		"dir")
			do_nothing
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
			cd patches && patch ../include/base/macros.h enable_debug.patch && cd ..
			;;
		"exit")
			break
			;;
		"file")
			echo -n "file> "
			read file
			FILE=$file
			echo "A new file has been successfully configured."
			;;
		"git")
			echo -n "git> "
			read args
			git $args
			;;
		"git>")
			while [ 1 ]
			do
				echo -n "git> "
				read args

				if [ $args == "#e" ]
				then
					break
				fi

				git $args
			done
			;;
		"help")
			print_commands
			;;		
		"rupc")
			do_nothing
			;;
		"rupp")
			do_nothing
			;;
		*)
			echo "error: bad command"
			echo "please consult the command help"
			;;
	esac
done
