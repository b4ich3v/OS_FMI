#!/bin/bash

if [ "$#" -ne 1 ] || [ ! -d "$1" ]; then
    echo "Wrong number of arguments or invalid input data"
    exit 1
fi  

targetDir=$1
tempFile=$(mktemp)

while IFS= read -r currentFileFormat; do
    if ! echo "$currentFileFormat" | grep -Eq "[0-9]{4}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}\.txt"; then
        echo "Invalid data input"
        exit 1
    fi  

    currentFriend=$(echo "$currentFileFormat" | cut -d "/" -f 4)
    counterForLines=$(cat "$currentFileFormat" | wc -l)

    if ! cat "$tempFile" | grep -Eq "$currentFriend"; then
        echo "$currentFriend:$counterForLines" >> "$tempFile"
    else
        currentCout=$(cat "$tempFile" | grep -E "^$currentFriend:" | cut -d ":" -f 2)
        newCount=$((currentCout + counterForLines))
        sed -i "s/^$currentFriend:[0-9]*/$currentFriend:$newCount/" "$tempFile"
    fi

done < <(find "$targetDir" -type f)

cat "$tempFile" | sort -t ':' -k 2 -nr | head -n 10
rm -f "$tempFile"
exit 0
