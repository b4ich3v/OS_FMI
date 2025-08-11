#!/bin/bash

userId=$(id -u)
tempFile=$(mktemp)

find ~ -type f -exec stat -c "%u %n" {} ';' 1> "$tempFile" 2> "/dev/null"

while IFS= read -r currentLine; do
	id=$(echo "$currentLine" | cut -d ' ' -f1)
	fileName=$(echo "$currentLine" | cut -d ' ' -f2)
	
	if [ "$id" -ne "$userId" ]; then
		continue
	fi

	printf "%s\n" "$fileName"

done < "$tempFile"


rm -r "$tempFile"
exit 0

