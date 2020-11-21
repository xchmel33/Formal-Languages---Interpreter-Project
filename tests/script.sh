#!/bin/sh

NAME_C_IN=scanner.c
FILE_C_IN=$(pwd)/$NAME_C_IN

echo "Compiling .c"
if gcc parser.c -o parser; then
	echo "Success !"
else
	echo "ERROR compiling failure !"
	exit 0
fi

echo "Calling scanner"
./parser
