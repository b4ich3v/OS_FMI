#!/bin/bash

maxAbsNumber=0
maxSumOfDigits=0
candidate=0
tempFile=$(mktemp)

while IFS= read -r currentLine; do
    if [ "$currentLine" == "exit" ]; then
        break
    fi

    if echo "$currentLine" | grep -Eq "^\-?[0-9]+[0-9]*$"; then
        absoluteValuedNumber=$(echo "$currentLine" | sed -E "s/\-//")
        if [ "$absoluteValuedNumber" -gt "$maxAbsNumber" ]; then
            maxAbsNumber="$absoluteValuedNumber"
        fi

        currentSumOfDigits=$(echo "$currentLine" | grep -E -o "[0-9]" | awk '
            BEGIN { result = 0 } { result += $1 } END { print result }')

        if [ "$currentSumOfDigits" -gt "$maxSumOfDigits" ]; then
            maxSumOfDigits="$currentSumOfDigits"
        fi
    fi

    echo "$currentLine" >> "$tempFile"

done

printf "\n"

while IFS= read -r currentLine; do
    if echo "$currentLine" | grep -Eq "^\-?[0-9]+[0-9]*$"; then
        absoluteValuedNumber=$(echo "$currentLine" | sed -E "s/\-//")
        if [ "$absoluteValuedNumber" -eq "$maxAbsNumber" ]; then
            echo "$currentLine"
        fi

        currentSumOfDigits=$(echo "$currentLine" | grep -E -o "[0-9]" | awk '
            BEGIN { result = 0 } { result += $1 } END { print result }')

        if [ "$currentSumOfDigits" -eq "$maxSumOfDigits" ]; then
            if [ "$currentLine" -lt "$candidate" ]; then
                candidate="$currentLine"
            fi
        fi
    fi
done < "$tempFile"

echo "$candidate"
rm -rf "$tempFile"
exit 0
