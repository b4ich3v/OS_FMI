#!/bin/bash

if [ "$#" -gt 0 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

if [ -d "$1" ]; then
	echo "Directory does not exist"
	exit 1
fi

homeDir=$(echo "$HOME")

tempFile=$(mktemp)
find "$homeDir" -type f -exec stat -c "%s %n" {} ';' 1> "$tempFile"

while IFS= read -r currentLine; do
	countOfBytes=$(echo "$currentLine" | cut -d ' ' -f1)
	fileName=$(echo "$currentLine" | cut -d ' ' -f2)

	if [ "$countOfBytes" -eq 0 ]; then
		printf "$fileName\n"
	fi

done < "$tempFile"

rm -r "$tempFile"

find "$homeDir" -type f -exec stat -c "%s %n" {} ';' | sort -t ' ' -k2 -n | cut -d ' ' -f2
printf "\n"
find "$homeDir" -type d -exec stat -c "%s %n" {} ';' | sort -t ' ' -k2 -n | cut -d ' ' -f2

exit 0
