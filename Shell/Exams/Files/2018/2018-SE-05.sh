#!/bin/bash

if [ "$#" -ne 2 ] || [ ! -f "$1" ]; then
    echo "Wrong number of arguemnts or invalid input data"
    exit 1
fi

if [ ! -d "$2" ]; then
    mkdir -p "$2"
fi

touch "$2/dict.txt"
counter=0
dataFile=$1

while IFS= read -r currentLine; do
    currentUsername=$(echo "$currentLine" | sed -E "s/ *\([^)]*\)//g")
    if cat "$2/dict.txt" | grep -Eq "^$currentUsername:"; then
        continue
    else
        echo "$currentUsername;$counter" >> "$2/dict.txt"
    fi

    counter=$((counter + 1))
done< <(cat "$dataFile" | cut -d ":" -f 1)

while IFS= read -r currentUserAndId; do
    currentUsername=$(echo "$currentUserAndId" | cut -d ";" -f 1)
    currentId=$(echo "$currentUserAndId" | cut -d ";" -f 2)

    touch "$currentId.txt"
    cat "$dataFile" | grep -E "^$currentUsername" | cut -d ":" -f 2- > "$currentId.txt"

done< <(cat "$2/dict.txt")

exit 0
