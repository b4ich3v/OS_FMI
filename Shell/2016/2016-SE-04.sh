#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error"
    exit 1
fi

if ! echo $1 | grep "^[1-9][0-9]*$"; then
    echo "Error"
    exit 1
fi

if ! echo $2 | grep "^[1-9][0-9]*$"; then
    echo "Error"
    exit 1
fi

argument1=$1
argument2=$2

tempFile=$(mktemp)
find . -mindepth 1 -maxdepth 1 -type f > "${tempFile}"

mkdir a
mkdir b
mkdir c

while read -r file; do
    countOfLines=$(cat "${file}" | wc -l)

    if [ "${countOfLines}" -lt "${argument1}" ]; then
        mv "${file}" a
    elif [[ "${countOfLines}" -gt "${argument1}" && "${countOfLines}" -lt "${argument2}" ]]; then
        mv "${file}" b
    else
        mv "${file}" c
    fi

done < "${tempFile}"

rm -r "${tempFile}"
