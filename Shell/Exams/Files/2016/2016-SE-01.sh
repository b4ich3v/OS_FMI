#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Wrong number of arguments"
	exit 1
fi	

tempFile=$(mktemp)
cat "$1" 1> "$tempFile" 2> "/dev/null"

while IFS= read -r currentLine; do
	if echo "$currentLine" | grep -q '[02468]' && ! echo "$currentLine" | grep -q '[a-z]'; then
        printf "%s\n" "$currentLine"
    fi
done < "$tempFile"

rm -r "$tempFile"
exit 0



