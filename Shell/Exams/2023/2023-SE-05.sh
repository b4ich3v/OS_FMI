#!/bin/bash

logFile=$(mktemp)
counterForIterations=0

while true; do
    breakStatement=1
    tempFile=$(mktemp)

    ps aux | tr -s ' ' | cut -d ' ' -f 11,6 | awk '{ \
        for(i=1;i<NF;i++){
            sep=" ";
            n=split($i, array, sep);

            if(array[1] > 65536){
                printf("%s\n", array[0]);
            }
        }
    }' 1> "$tempFile"

    while IFS= read -r currentLine; do
        breakStatement=0

        if cat "$logFile" | grep -E -q "^$currentLine"; then
            currentOccur=$(cat "$logFile" | grep -E "^$currentLine" | cut -d ' ' -f 2)
            currentOccur=$((currentOccur + 1))
            sed -i -E "s/^$currentLine [0-9]+/^$currentLine $currentOccur/g"
        else
            sed -i -E "s/^$currentLine [0-9]+/^$currentLine 0/g"
        fi
    done < "$tempFile"

    rm -r "$tempFile"
    counterForIterations=$((counterForIterations + 1))

    if [ "$breakStatement" -eq 1 ]; then
        break
    fi
done 

while IFS= read -r currentLine; do
    echo "$currentLine" | awk -v tolerance="$counterForIterations" ' \
    {
        for(i=1;i<NF;i++){
            sep=" ";
            n=split($i, array, sep);

            if(array[1] + 1 > tolerance){
                printf("%s\n", array[0]);
            }
        }
    }'

done < "$logFile"

rm -r "$logFile"
