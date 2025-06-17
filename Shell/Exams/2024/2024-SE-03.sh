#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo Error
    exit 1
fi

if ! echo "$1" | grep -E -q "^-?[1-9]+[0-9]*$"; then
    echo Error
    exit 1
fi

if [ "$1" -lt 0 ]; then
    echo Error
    exit 1
fi

size=12
offset=$1

dataFile=$(mktemp)
actualFile=$(mktemp)

cat file.txt 1> "$actualFile"
echo "A 0_Bb 1_B 2_C 3_Db 4_D 5_Eb 6_E 7_F 8_Gb 9_G 10_Ab 11" | sed "s/_/\n/g" 1> "$dataFile"

while IFS= read -r currentLine; do
    modifiedLine=$currentLine
    
    while read -r currentTon; do
        indexForCurrentTone=$(cat "$dataFile" | grep -E "^$currentTon " | cut -d ' ' -f 2)
        indexForCurrentTone=$((indexForCurrentTone + offset))
        indexForCurrentTone=$((indexForCurrentTone % size))
        indexForCurrentTone=$((indexForCurrentTone + 1))
        replacingData=$(cat "$dataFile" | head -n "$indexForCurrentTone" | tail -n 1 | cut -d ' ' -f 1) 
        modifiedLine=$(echo "$modifiedLine" | sed "s/$currentTon/$replacingData/g") 
    done < <(echo "$currentLine" | grep -oE '\[[A-G](b|#)?[a-zA-Z0-9/]*\]' | sed -E 's/\[([A-G](b|#)?).*/\1/')

    echo "$modifiedLine"
done < "$actualFile"

rm -r "$actualFile"
rm -r "$dataFile"
