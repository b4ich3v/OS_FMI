#!/bin/bash

breakStatement="end"
fileWithNumbers=$(mktemp)
maxNumber=0

while IFS= read -r currentLine; do
    if [ "$currentLine" == "$breakStatement" ]; then
        break
    fi 

    echo "$currentLine" | grep -E "^-?[0-9]+[0-9]*$" >> "$fileWithNumbers" 
    helper=$(echo "$currentLine" | grep -E "^-?[1-9]+[0-9]*$" | awk -v varTarget="$maxNumber" '{
        if($1 < 0){
            if($1 < -varTarget){
                printf("%d\n", $1);
            }
        }
        else{
            if($1 > varTarget){
                printf("%d\n", $1);
            }
        }
    }')

    if [ -n "$helper" ]; then
        maxNumber=$helper
    fi

done 

finallFile=$(mktemp)
cat "$fileWithNumbers" | uniq 1> "$finallFile"
echo " " | sed "s/ /\n/g"

while IFS= read -r currentNumber; do
    echo "$currentNumber" | awk -v varTarget="$maxNumber" '{
        if($1 < 0){
            if($1 == -varTarget){
                printf("%d\n", $1);
            }
        }
        else{
            if($1 == varTarget){
                printf("%d\n", $1);
            }
        }
    }'
done < "$finallFile"

maxSum=0
echo " " | sed "s/ /\n/g"

while IFS= read -r currentNumber; do
    currentSum=$(echo "$currentNumber" | grep -E -o "[0-9]" | awk '{sum += $1} END {print sum}')

    if [ "$currentSum" -gt "$maxSum" ]; then
        maxSum=$currentSum
    fi

done < <(cat "$finallFile" | sed "s/-//g")
    
finalFile1=$(mktemp)

while IFS= read -r currentNumber; do
    currentSum=$(echo "$currentNumber" | grep -E -o "[0-9]" | sed "s/-//g" | awk '{sum += $1} END {print sum}')

    if [ "$currentSum" -eq "$maxSum" ]; then
        echo "$currentNumber" >> "$finalFile1"
    fi
done < <(cat "$finallFile")

sort -n "$finalFile1" -o "$finalFile1"
cat "$finalFile1" | head -n 1 | tail -n 1

rm -r "$finalFile1"
rm -r "$finallFile"
rm -r "$fileWithNumbers"
