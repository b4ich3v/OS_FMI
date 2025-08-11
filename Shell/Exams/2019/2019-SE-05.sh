#!/bin/bash

tempFile=$(mktemp)
lowerBond=1551168000
upperBond=1551176100

cat "/etc/passwd" | cut -d ":" -f6 1> "$tempFile" 2> "/dev/null"

while IFS= read -r currentDir; do
    currentSeconds=$(stat -c "%Z %n" "$currentDir" | cut -d " " -f1)
    isFromSi=0

    if echo "$currentDir" | grep -E "/home/SI"; then
        isFromSi=1
    fi

    if [ "$currentSeconds" -ge "$lowerBond" ] && [ "$currentSeconds" -le "$upperBond" ] && [ "$isFromSi" -eq 1 ]; then
        username=$(cat "/etc/passwd" | grep -E "$currentDir" | cut -d ":" -f5 | cut -d " " -f1)
        echo "$currentSeconds $username"
    fi

done < "$tempFile"

rm -rf "$tempFile"
exit 0
