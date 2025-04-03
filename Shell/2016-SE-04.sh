#!/bin/bash

read -p : number1
read -p : number2

if echo "${number1}" | grep -E "[-][1-9][0-9]*"; then
    echo "Negative number"
    exit 1
fi

if echo "${number1}" | grep -E "[-][1-9][0-9]*"; then
    echo "Negative number"
    exit 1
fi

if [[ "${number1}" -gt "${number2}" ]]; then
    echo "Not logical numbers"
    exit 1
fi

mkdir a
mkdir b
mkdir c

echo "${number1}"
echo "${number2}"

tempFile=$(mktemp)

find . -type f > "${tempFile}"
cat "${tempFile}"

while read -r line; do
    currentCountOfLines=$(cat "${line}" | wc -l)

    if [[ currentCountOfLines -lt "${number1}" ]]; then
        cp "${line}" a 
    elif [[ currentCountOfLines -gt  "${number1}" && currentCountOfLines -lt "${number2}" ]] then
        cp "${line}" b 
    else
        cp "${line}" c 
    fi
done < "${tempFile}"

rm -r "${tempFile}"
