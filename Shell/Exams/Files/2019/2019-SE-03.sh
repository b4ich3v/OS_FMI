#!/bin/bash

if [ "$#" -ne 1 ] || [ ! -d "$1" ]; then
    echo "Wrong number of arguments or invalid input data"
    exit 1
fi

searchingPattern="^[^_]+_report-[0-9]+\.tgz"
separator="_"
mapTable="hashes.db"

touch "$mapTable"

while IFS= read -r currentFile; do
    currentHash=$(sha256sum "$currentFile" | cut -d ' ' -f1)
    oldHash=$(grep -F "^$currentFile:" "$mapTable" | cut -d ':' -f2)

    if [ "$currentHash" == "$oldHash" ]; then
        continue
    fi

    tempDir=$(mktemp -d)
    tar -xzf "$currentFile" -C "$tempDir"

    name=$(basename "$currentFile" | grep -E -o "^[^_]+")
    timestamp=$(basename "$currentFile" | grep -E -o "[0-9]+")

    targetFile=$(find "$tempDir" -type f -name "meow.txt" | head -n1)
    if [ -f "$targetFile" ]; then
        mv "$targetFile" "$(dirname "$targetFile")/${name}${separator}${timestamp}.txt"
    fi

    tar -czf "$currentFile" -C "$tempDir" .
    rm -rf "$tempDir"

    sed -i "\|^$currentFile:|d" "$mapTable"
    echo "$currentFile:$currentHash" >> "$mapTable"

done < <(find "$1" -type f | grep -E "$searchingPattern")

exit 0
