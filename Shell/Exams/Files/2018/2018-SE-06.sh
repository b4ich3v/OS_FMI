#!/bin/bash

if [ "$#" -ne 1 ] || [ ! -f "$1" ]; then
    echo "Wrong number of arguments or invalid input data"
    exit 1
fi

dataFile=$1
tempFile=$(mktemp)

cat "$dataFile" | cut -d "," -f 2- | sort | uniq | while IFS= read -r currentLine; do
    numberVariable=$(cat "$dataFile" | grep -E -o "[0-9]*,$currentLine" | cut -d "," -f 1 | awk '
        BEGIN { result = 0 } { if($1 > result) result=$1 } END { print result }')

    echo "$numberVariable,$currentLine" >> "$tempFile"

done 

cat "$tempFile"
rm -rf "$tempFile"
exit 0
