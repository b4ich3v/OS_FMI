#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo Error
    exit 1
fi

command="$1"
file="$2"

value=$(eval "$command" 2>/dev/null)
status=$?

if [ "$status" -ne 0 ] || ! echo "$value" | grep -E -q '^[0-9]+(\.[0-9]+)?$'; then
    exit 3
fi

nowDate=$(date "+%Y-%m-%d")
nowHour=$(date "+%H")
nowStamp="$nowDate $nowHour:00"
nowDow=$(date "+%u")

echo "$nowStamp $value" >> "$file"

sum=0
counter=0

while IFS= read -r currentLine; do
    lineDate=$(echo "$currentLine" | cut -d ' ' -f1)
    lineHour=$(echo "$currentLine" | cut -d ' ' -f2 | cut -d ':' -f1)
    lineValue=$(echo "$currentLine" | cut -d ' ' -f3)

    lineDow=$(date -d "$lineDate" +%u 2>/dev/null)

    if [ "$lineDow" = "$nowDow" ] && [ "$lineHour" = "$nowHour" ] && [ "$lineDate" != "$nowDate" ]; then
        sum=$(echo "$sum + $lineValue" | bc -l)
        counter=$((counter + 1))
    fi

done < "$file"

if [ "$counter" -eq 0 ]; then
    exit 0
fi

avarage=$(echo "$sum / $counter" | bc -l)
low=$(echo "$avarage / 2" | bc -l)
high=$(echo "$avarage * 2" | bc -l)

less=$(echo "$value < $low" | bc -l)
more=$(echo "$value > $high" | bc -l)

if [ "$less" -eq 1 ] || [ "$more" -eq 1 ]; then
    echo "$nowStamp $value abnormal"
    exit 2
fi

exit 0
