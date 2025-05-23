#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
    echo "Error"
    exit 1
fi

homeDirPattern="/home"
infoFile=$(mktemp)
resultUsers=$(mktemp)

cat /etc/passwd > "${infoFile}"
cat "${infoFile}" | grep -v "${homeDirPattern}" >> "${resultUsers}"

while read -r currentLine; do
    currentUser=$(echo "${currentLine}" | awk -F ':' '{print $1}')
    currentHomeDir=$(echo "${currentLine}" | awk -F ':' '{print $6}')
    
    if [ "$(stat -c %U "${currentHomeDir}")" == "${currentUser}" ]; then
        echo "${currentUser}" >> "${resultUsers}"
    fi

    if ! sudo -u "${currentUser}" ls "${currentHomeDir}" &>/dev/null; then
        echo "${currentUser}" >> "${resultUsers}"
    fi

done < "${infoFile}"

dataFile=$(mktemp)
ps -e -o uid,pid,rss --no-headers | tr -s ' ' | sort -n -k1,1 | tail -n +2 > "${dataFile}"


prevUser=$(head -n 1 "${dataFile}" | cut -d ' ' -f 1)
counterForRSS=0
rootRSS=0

while read -r currentLine; do
    currentUser=$(echo "${currentLine}" | cut -d ' ' -f 1)
    currentRSS=$(echo "${currentLine}" | cut -d ' ' -f 3)

    if [ "${currentUser}" -eq 0 ]; then
        rootRSS=$(( rootRSS + currentRSS ))
    fi

done < "${dataFile}"  

while read -r currentLine; do
    currentUser=$(echo "${currentLine}" | cut -d ' ' -f 1)
    currentPID=$(echo "${currentLine}" | cut -d ' ' -f 2)
    currentRSS=$(echo "${currentLine}" | cut -d ' ' -f 3)
    
    if grep -q "^${currentUser}$" "${resultUsers}"; then
        if [ "${currentUser}" == "${prevUser}" ]; then
            if [ "${counterForRSS}" -gt "${rootRSS}" ]; then
                kill "${currentPID}"
            fi  
       
            counterForRSS="${currentRSS}"
            prevUser="${currentUser}"

        else
            counterForRSS=$(( counterForRSS + currentRSS ))
        fi
    fi

done < "${dataFile}"

rm -r "${dataFile}"
rm -r "${resultUsers}"
rm -r "${infoFile}"
