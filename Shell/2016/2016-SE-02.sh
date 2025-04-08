#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Error"
    exit 1
fi

if [ "$(id -u)" -ne  0 ]; then
    echo "Error"
    exit 1
fi

tempFile=$(mktemp)
tolerance=$1
ps -e -o uid,pid,rss | sort -n -t ' ' -k 1 > "${tempFile}"

id=$(head -n 1 "${tempFile}" | cut -d ' ' -f 1)
rss=$(head -n 1 "${tempFile}" | cut -d ' ' -f 3)
rssCounter=0
pidForMaxRss=0

while read -r currentLine; do
    currentId=$(echo "${currentLine}" | cut -d ' ' -f 1)
    currentPid=$(echo "${currentLine}" | cut -d ' ' -f 2)
    currentRss=$(echo "${currentLine}" | cut -d ' ' -f 3)

    if [ "${currentId}" -ne "${id}" ]; then
        if [ "${rssCounter}" -gt "${tolerance}" ]; then
            kill "${pidForMaxRss}"
        fi
        
        id="${currentId}"
        rss="${currentRss}"
        pidForMaxRss="${currentPid}"
        rssCounter=0

    else
        rssCounter=$(( rssCounter + currentRss ))

        if [ "${currentRss}" -gt "${rss}" ]; then
            rss="${currentRss}"
            pidForMaxRss="${currentPid}"
        fi
    fi

done < "${tempFile}"

if [ "${rssCounter}" -gt "${tolerance}" ]; then
    kill "${pidForMaxRss}"
fi

rm -r "${tempFile}"
