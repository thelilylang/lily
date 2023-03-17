#!/bin/sh

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
			./build/Debug/lily compile $CURRENT_FILE
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
