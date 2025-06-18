#!/bin/bash

N=10
hasOptional=0

if [ "$1" == "-n" ]; then
    N=$2
    hasOptional=1
fi

currentIteration=0
i=0

while [ $i -lt $# ]; do
    if [ "$hasOptional" -eq 1 ] && { [ "$currentIteration" -eq 0 ] || [ "$currentIteration" -eq 1 ]; }; then
        i=$((i + 1))
        currentIteration=$((currentIteration + 1))
        continue
    fi

    var=$((i + 1))
    currentFileName="${!var}"

    idf=$(echo "$currentFileName" | sed "s/\.log$//")
    dataFile=$(mktemp)

    tail -n "$N" "$currentFileName" > "$dataFile"
    sort -k1,2 "$dataFile" -o "$dataFile"

    while IFS= read -r currentLine; do
        dateTime=$(echo "$currentLine" | awk '{print $1, $2}')
        data=$(echo "$currentLine" | cut -d' ' -f3-)

        echo "$dateTime $idf $data"
    done < "$dataFile"

    currentIteration=$((currentIteration + 1))
    i=$((i + 1))
    rm "$dataFile"
done
