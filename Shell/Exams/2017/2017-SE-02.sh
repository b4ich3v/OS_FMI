#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
    echo "Error"
    exit 1
fi

if [ $# -ne 3 ]; then
    echo "Error"
    exit 1
fi

if [[ ! -d $1 || ! -d $2 ]]; then
    echo "Error"
    exit 1
fi

source=$1
destination=$2
targetString=$3

allFilesInSource=$(mktemp)
find "${source}" -type f > "${allFilesInSource}"

while read -r currentFilePath; do
    if echo "${currentFilePath}" | grep -E "${targetString}"; then
        mv "${currentFilePath}" "${destination}"
    fi

done < "${allFilesInSource}"

rm -r "${allFilesInSource}"
