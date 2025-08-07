#!/bin/bash

foo=$1
countOfFooProcesses=$(ps -eo user | sort | uniq -c | grep -E "$foo" | sed -E "s/[\t\r ]*//" | cut -d ' ' -f 1)

ps -eo user | sort | uniq -c | sed -E "s/[\t\r ]*//"  | awk -F ' ' -v tolerance="$countOfFooProcesses" '{ 
        if($1 > tolerance) { printf $2 } 
}'

tempFile=$(mktemp)
ps -eo user,time | sort | tail -n +2 | tr -s ' ' 1> "$tempFile" 2> "/dev/null"
printf "\n"

secondsCounter=0
prevUser=$(head -n 1 "$tempFile" | cut -d ' ' -f1)
currentUser=""
counter=0
fileWithAvarageSecondsForProcessPerUser=$(mktemp)

printf "\n\n"

while IFS= read -r currentUserTime; do
    currentUser=$(echo "$currentUserTime" | cut -d ' ' -f 1)
    currentTimeInSeconds=0

    fileWithTimeFormat=$(mktemp)
    echo "$currentUserTime" | cut -d ' ' -f 2 1> "$fileWithTimeFormat"

    while IFS=':' read -r hours minutes seconds; do
        currentTimeInSeconds=$((hours * 3600 + minutes * 60 + seconds))
    done < "$fileWithTimeFormat" 

    if [ "$currentUser" != "$prevUser" ]; then
        avg=0

        if [ "$counter" -gt 0 ]; then
            avg=$((secondsCounter / counter))
        else
            avg=0
        fi

        echo "$prevUser $avg" >> "$fileWithAvarageSecondsForProcessPerUser"
        prevUser="$currentUser"
        secondsCounter=0
        counter=0
    else

        secondsCounter=$((secondsCounter + currentTimeInSeconds))
        counter=$((counter + 1))
    fi

    rm -r "$fileWithTimeFormat"

done < "$tempFile"

if [ "$currentUser" != "$prevUser" ]; then
    echo "$prevUser $secondsCounter" >> "$fileWithAvarageSecondsForProcessPerUser"
fi

cat "$fileWithAvarageSecondsForProcessPerUser"
avarageSecondsForFoo=$(cat "$fileWithAvarageSecondsForProcessPerUser" | grep -E "$foo" | cut -d ' ' -f2)

ps -eo user,pid,time | tail -n +2 | tr -s ' ' | grep "^$foo" | while read -r user pid time; do
    IFS=':' read -r h m s <<< "$time"
    totalSeconds=$((h * 3600 + m * 60 + s))

    if [ "$totalSeconds" -gt $((2 * avarageSecondsForFoo)) ] && [ "$user" == "$foo" ]; then
        kill "$pid"
    fi
done


rm -r "$fileWithAvarageSecondsForProcessPerUser"
rm -r "$tempFile"
exit 0
