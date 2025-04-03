#!/bin/bash

read -p : inputD

tempFile=$(mktemp)
find . -type d > "${tempFile}"
found=0

while read -r line; do
    if echo "${line}" | grep "${inputD}"; then
        ((found++))
        break
    fi 
done < "${tempFile}"

if [[ found -eq 1 ]]; then
    find . -type l -exect echo {} 2> /dev/null ';'
    find . -mindepth 1 -maxdepth 1 -type l -exec echo {} 2> /dev/null ";" 
fi

rm -r "${tempFile}"
