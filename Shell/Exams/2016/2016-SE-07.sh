#!/bin/bash

if [ ! -d "$1" ] || [ "$#" -ne 1 ]; then
	echo "Inalid argument or wrong number of arguments"
	exit 1
fi

find "$1" -type l ! -exec test -e {} ';' -print
exit 0

