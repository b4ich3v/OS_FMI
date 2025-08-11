#!/bin/bash

if [ "$#" -ne 1 ] || [ ! -f "$1" ] ; then
    echo "Wrong number of arguemnts or invalid input"
    exit 1
fi

noCounter=$(cat "$1" | wc -l)
yesCounter=0
tempFile=$(mktemp)
target=302

while IFS= read -r currentLine; do
    currentIp=$(echo "$currentLine" | cut -d " " -f 1)
    code=$(echo "$currentLine" | awk -F ' ' '{print $9}')

    if [ "$code" -gt "$target" ]; then
        echo "$currentIp $code" >> "$tempFile"
    fi

    if echo "$currentLine" | grep -o "HTTP/2.0"; then
        yesCounter=$((yesCounter + 1))
        noCounter=$((noCounter - 1))
    fi

done < <(cat "$1")

echo "$yesCounter"
echo "$noCounter"
cat "$tempFile" | sort -t ' ' -k 1 | head -n 10

rm -rf "$tempFile"
exit 0
