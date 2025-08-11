#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Wrong number of arguments"
    exit 1
fi

targetName="$1"
targetHomeDir=$(grep "^$targetName:" /etc/passwd | cut -d ":" -f6)
inodeForTheLastMofidied=$(find "$targetHomeDir" -type f -exec stat -c "%Y %i" {} ';' | sort -t ' ' -k1 -nr | head -n 1 | cut -d " " -f2)

bestChoice=""
bestDepth=999999

while IFS= read -r currentFile; do
    currentDepth=$(echo "$currentFile" | sed "s/\//\n/g" | wc -l)
    currentDepth=$((currentDepth - 1))

    if [ "$currentDepth" -le "$bestDepth" ] && [ "$(stat -c '%i' "$currentFile")" -eq "$inodeForTheLastMofidied" ]; then
        bestChoice="$currentFile"
        bestDepth="$currentDepth"
    fi

done < <(find "$targetHomeDir" -type f)

echo "$bestChoice"
exit 0
