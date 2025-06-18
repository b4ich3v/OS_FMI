#!/bin/bash

if [ $# -ne 1 ] || [ ! -d "$1" ]; then
    echo Error
    exit 1
fi  

allFiles=$(mktemp)
duplicatedFiles=$(mktemp)
find "$1" -type f 1> "$allFiles" 2> /dev/null
counter=0

while IFS= read -r currentFileName1; do
    while IFS= read -r currentFileName2; do
        if [[ "$currentFileName1" == "$currentFileName2" ]]; then
            continue
        fi

        hash1=$(md5sum "$currentFileName1" | cut -d ' ' -f 1)
        hash2=$(md5sum "$currentFileName2" | cut -d ' ' -f 1)

        if [[ "$hash1" == "$hash2" ]]; then
            counter=$((counter + "$(stat -c %s "$currentFileName2")"))
            rm -r "$currentFileName2"
            ln -s "$currentFileName1" "$currentFileName2"
            echo "$currentFileName1 $currentFileName2" >> "$duplicatedFiles"
        fi
    done < "$allFiles"    
done < "$allFiles"

echo "Duplicated file are:"
cat "$duplicatedFiles"
echo "Freed memory is: $counter bytes"

rm -r "$duplicatedFiles"
rm -r "$allFiles"
