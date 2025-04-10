#!/bin/bash

fileWithUsernamesAndDirs=$(mktemp)
cat /etc/passwd | tr ',' ' ' | awk -F ':' '{print $1" "$6" defaultKey"}' > "${fileWithUsernamesAndDirs}"

while read -r line; do
    currentUser=$(echo "${line}" | awk -F ' ' '{print $1}')
    currentDir=$(echo "${line}" | awk -F ' ' '{print $2}')

    filesPerUser=$(mktemp)
    find "${currentDir}" -type f 2>/dev/null > "${filesPerUser}"
    bestSeconds=0

    while read -r currentFile; do
        timeInSecondsPerFile=$(stat -c %Y "${currentFile}")

        if [ "${timeInSecondsPerFile}" -gt "${bestSeconds}" ]; then
            sed -i "/^${currentUser}/ s/defaultKey/${timeInSecondsPerFile}/" "${fileWithUsernamesAndDirs}"
            bestSeconds="${timeInSecondsPerFile}"
        fi
    done < "${filesPerUser}"
    
    rm -r "${filesPerUser}"
done < "${fileWithUsernamesAndDirs}"

cat "${fileWithUsernamesAndDirs}" | sort -n -t ' ' -k 3 -r | head -n 1 | cut -d ' ' -f 1
rm -r "${fileWithUsernamesAndDirs}"
