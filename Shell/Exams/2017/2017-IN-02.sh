#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Error"
    exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
    echo "Error"
    exit 1
fi

tempFile=$(mktemp)
fileA=$(mktemp)
fileB=$(mktemp)
ps -e -o user,pid,%cpu,%mem,vsz,rss,tty,stat,time,command | sort -t ' ' -k 1 > "${tempFile}"

targetUser=$1
prevUser=$(cat "${tempFile}" | head -n 2 | tail -n 1 | awk -F ' ' '{print $1}')

counterPerUser=0
counterForTargetUser=$(cat "${tempFile}" | grep -E "\b${targetUser}\b" | wc -l)

while read userLine; do
    currentUser=$(echo "${userLine}" | awk -F ' ' '{print $1}')
    currentTimePerUser=$(echo "${userLine}" | awk -F ' ' '{print $9}')
    echo "${currentTimePerUser}" >> "${fileB}"

    if [ "${currentUser}" == "${prevUser}" ]; then
        (( counterPerUser++ ))
    else
        if [ "${counterPerUser}" -gt "${counterForTargetUser}" ]; then
            echo "${prevUser}" >> "${fileA}"
        fi

        prevUser="${currentUser}"
        counterPerUser=1

    fi
done < "${tempFile}"

if [ "${counterPerUser}" -gt "${counterForTargetUser}" ]; then
    echo "${prevUser}" >> "${fileA}"
fi

totalSeconds=0

while read -r line; do
    hours=$(echo "${line}" | cut -d ':' -f 1)
    minutes=$(echo "${line}" | cut -d ':' -f 2)
    seconds=$(echo "${line}" | cut -d ':' -f 3)

    currentSeconds=$(( 10#${hours} * 3600 + 10#${minutes} * 60 + 10#${seconds} ))
    totalSeconds=$(( totalSeconds + currentSeconds ))

done < "${fileB}"

countOfAllProccesses=$(cat "${fileB}" | wc -l)
avarageSecondsBerProccess=$(( totalSeconds /  countOfAllProccesses ))

fileForTargetProccesses=$(mktemp)
cat "${tempFile}" | grep "${targetUser}" > "${fileForTargetProccesses}"

while read -r currentProccessForTargetUser; do
    time=$(echo "${currentProccessForTargetUser}" | awk -F ' ' '{print $9}')
    currentPID=$(echo "${currentProccessForTargetUser}" | awk -F ' ' '{print $2}')

    hours=$(echo "${time}" | cut -d ':' -f 1)
    minutes=$(echo "${time}" | cut -d ':' -f 2)
    seconds=$(echo "${time}" | cut -d ':' -f 3)

    currentSeconds=$(( 10#${hours} * 3600 + 10#${minutes} * 60 + 10#${seconds} ))
    currentSeconds=$(( currentSeconds * 2 ))

    if [ "${currentSeconds}" -gt "${avarageSecondsBerProccess}" ]; then
        kill "${currentPID}"
    fi

done < "${fileForTargetProccesses}"

cat "${fileA}"
echo "${avarageSecondsBerProccess}"

rm -r "${fileA}"
rm -r "${fileB}"
rm -r "${fileForTargetProccesses}"
rm -r "${tempFile}"
