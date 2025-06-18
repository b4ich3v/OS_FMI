#!/bin/bash

if [ $# -ne 2 ]; then
    echo Error
    exit 1
fi

dataFile=$(mktemp)
hashTable=$(mktemp)
cat "$1" | grep -E ",$2," 1> "$dataFile"

while IFS= read -r currentLine; do
    searchingStar=$(echo "$currentLine" | cut -d ',' -f 4)

    if [ "$searchingStar" == "--" ]; then
        continue
    fi

    if cat "$hashTable" | grep -E "$searchingStar"; then
        continue
    fi

    currentCounter=$(cat "$1" | grep -E ",$searchingStar," | wc -l)
    echo "$searchingStar $currentCounter" >> "$hashTable" 

done < "$dataFile"

sort -n -r -k 2 "$hashTable" -o "$hashTable"
results=$(mktemp)
mostConstellation=$(head -n1 "$hashTable" | cut -d ' ' -f1)
cat "$dataFile" | grep -E ",$mostConstellation," >> "$results"
sort -t ',' -k 7 -n "$results" -o "$results"
cat "$results" | head -n 1 | tail -n 1 | cut -d ',' -f 1

rm -r "$results"
rm -r "$hashTable"
rm -r "$dataFile"
