#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
    echo "Error"
    exit 1
fi

tempFile=$(mktemp)
ps -e -o uid,rss | tr -s ' ' ' ' | sort -t ' ' -k 1 > "${tempFile}"
read -p : argument

currentUser=""
currentSum=0

while read -r line; do
    newCurrentUser=$(echo "${line}" | cut -d ' ' -f 1)
    newCurrentSum=$(echo "${line}" | cut -d ' ' -f 2)

    if [[ "${currentUser}" != "${newCurrentUser}" ]]; then

        if [ "${currentSum}" -gt "${argument}" ]; then
                echo "User ${currentUser}: total RSS ${currentSum} > ${argument}"
            else
                echo "User ${currentUser}: total RSS ${currentSum} <= ${argument}"
            fi

        currentSum=0
        currentUser="${newCurrentUser}"

    fi

    currentSum=$(( currentSum + newCurrentSum ))

done < "${tempFile}"

if [ "${currentSum}" -gt "${argument}" ]; then
    echo "User ${currentUser}: total RSS ${currentSum} > ${argument}"
else
    echo "User ${currentUser}: total RSS ${currentSum} <= ${argument}"
fi

rm -r "${tempFile}"
