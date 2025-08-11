#!/bin/bash

targetName="s0600328"

targetFiles=$(mktemp)
allNames=$(mktemp)
find "$HOME" -type f -exec stat -c "%U %n %i %Y" {} ';' 1> "$allNames" 2> "/dev/null"

while IFS= read -r currentLine; do
	currentName=$(echo "$currentLine" | cut -d ' ' -f1)

	if [ "$targetName" == "$currentName" ]; then
		printf "%s\n" "$(echo "$currentLine" | cut -d ' ' -f2)" >> "$targetFiles"
	fi

done < "$allNames"

cat "$targetFiles"
cat "$targetFiles" | sort -t ' ' -k 4 -nr | cut -d ' '  -f 3 | head -n 1

rm -r "$allNames"
rm -r "$targetFiles"
exit 0


