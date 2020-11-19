#!/bin/sh

NAME_IN=asd
FILE_IN=$(pwd)/$NAME_IN
NAME_C_IN=scanner.c
FILE_C_IN=$(pwd)/$NAME_C_IN

if [ -f "$FILE_IN" ]; then
	echo "&FILE_IN exists."
	if [ -f "$FILE_C_IN" ]; then
		echo "&FILE_C_IN exists."
	else
		echo "ERROR $FILE_C_IN missing !"
		exit 0
	fi
else
	echo "ERROR $FILE_IN missing !"
	exit 0
fi
		
echo "Compiling .c"
if gcc scanner.c -o scanner; then
	echo "Success !"
else
	echo "ERROR compiling failure !"
	exit 0
fi

echo "Calling scanner"
./scanner <input.txt >output.txt
