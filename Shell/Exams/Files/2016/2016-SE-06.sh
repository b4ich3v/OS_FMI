#!/bin/bash

if [ ! -f "$1" ] || [ "$#" -ne 1 ]; then
	echo "Wrong number of arguments or not a file"
	exit 1
fi

tempFile=$(mktemp)
resultFile=$(mktemp)
customSeparator="|"

cat "$1" | sed -E "s/-/$customSeparator/" | cut -d "$customSeparator" -f2 | sed -E "s/[\t\r ]//" 1> "$tempFile" 2> "/dev/null"

counter=1

while IFS= read -r currentLine; do
	printf "$counter $currentLine\n" >> "$resultFile"
	counter=$((counter+1))

done < "$tempFile"

cat "$resultFile" | sort -t ' ' -k2

rm -r "$tempFile"
rm -r "$resultFile"
exit 0
