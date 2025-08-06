#!/bin/bash

tempFile=$(mktemp)

cat "info" | grep -E "SI" | cut -d ':' -f 1,3,5,6 | \
tr -s "," | tr "," ":" | sort -t ':' -k2 -nr | sed "s/:/ /" | \
sed "s/:/ /" | sed "s/:/ /" | cut -d ' ' -f 1,3,4,5 1> "$tempFile" 2> "/dev/null"

cat $tempFile""

while IFS= read -r currentLine; do
	name=$(echo "$currentLine" | cut -d ' ' -f 2,3)		
	homeDir=$(echo "$currentLine" | cut -d ' ' -f 4)

	printf "%s %s %s\n" "$name" "$homeDir" 

done < "$tempFile"

rm -r "$tempFile"
exit 0

