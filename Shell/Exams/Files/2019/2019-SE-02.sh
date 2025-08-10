#!/bin/bash

i=0
N=10
tempFile=$(mktemp)

if [ "$1" == "-n" ]; then
    if ! echo "$2" | grep -Eq "[0-9]*"; then
        echo "Invalid input data"
        exit 1
    fi

    N=$2
    i=3
fi

while [ "$i" -le "$#" ]; do
    while IFS= read -r currentLine; do
        idf=$(basename "${!i}" | sed "s/\.log//")
        timestamp=$(echo "$currentLine" | cut -d " " -f 1,2)
        data=$(echo "$currentLine" | cut -d " " -f 3-)
    
        if ! echo "$timestamp" | grep -Eq "^[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}$"; then
            echo "Invalid data input"
            exit 1
        fi

        echo "$timestamp $idf $data" >> "$tempFile"

    done < "${!i}"

    i=$((i + 1))
done

cat "$tempFile" | sort -t ' ' -k 1,2 | tail -n "$N"
rm -rf "$tempFile"
exit 0
