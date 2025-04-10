#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Error"
    exit 1
fi

hasTwoParams=0

if [ $# -eq 2 ]; then
    (( hasTwoParams++ ))
fi

if [ "${hasTwoParams}" -eq 1 ]; then
    allFiles=$(mktemp)
    
    targetDir=$1
    targetCountOfHardLinks=$2
    find "${targetDir}" -type f > "${allFiles}"

    while read -r currentFile; do
        currentCountOfHardLinks=$(stat -c %h "${currentFile}")

        if [[ "${currentCountOfHardLinks}" -gt  "${targetCountOfHardLinks}" ||
            "${currentCountOfHardLinks}" -eq "${targetCountOfHardLinks}" ]]; then
            echo "${currentFile}" | awk -F '/' '{print $NF}'
        fi

    done < "${allFiles}"

    rm -r "${allFiles}"
else
    targetDir=$1
    find "${targetDir}" -type l -exec test {} ';' -print
fi
