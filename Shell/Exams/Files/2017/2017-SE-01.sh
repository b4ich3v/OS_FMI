#!/bin/bash

tempFile=$(mktemp)
tempFile1=$(mktemp)
find . -type f -exec stat -c "%h %n" {} ';' 1> "$tempFile" 2> "/dev/null"

while IFS= read -r currentLine; do
	number=$(echo "$currentLine" | cut -d ' ' -f1)

	if [ "$number" -gt 1 ]; then
		echo "$currentLine" 1> "$tempFile1"
	fi

done < "$tempFile"

cat "$tempFile1" | sort -t ' ' -k1 -n | cut -d ' ' -f 2

rm -r "$tempFile"
rm -r "$tempFile1"

