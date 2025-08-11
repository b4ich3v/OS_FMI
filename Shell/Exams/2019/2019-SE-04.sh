#!/bin/bash

if [ "$#" -ne 1 ] || [ ! -f "$1" ]; then
    echo "Wrong number of arguments or invalid data input"
    exit 1
fi

targetType=$(cat "$1" | tail -n +2 | sort -t ";" -k 3 -n | tail -n 1 | cut -d ";" -f 2)

while IFS= read -r currentLine; do
    currentType=$(echo "$currentLine" | cut -d ";" -f 2)

    if [ "$currentType" == "$targetType" ]; then
        echo "$currentLine" | cut -d ";" -f 1,4 | sed "s/;/ /g"
        break 
    fi

done < <(cat "$1" | tail -n +2 | sort -t ";" -k 3 -n)

exit 0
