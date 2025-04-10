#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Error"
    exit 1
fi

fileName=$1
target1=$2
target2=$3

while read -r line1; do
    if echo "${line1}" | grep -E "^${target2}"; then
        values1=$(echo "${line1}" | awk -F '=' '{print $2}')

        while read -r line2; do
            if echo "${line2}" | grep -E "^${target1}"; then
                values2=$(echo "${line2}" | awk -F '=' '{print $2}')

                tempFile1=$(mktemp)
                tempFile2=$(mktemp)
                symbolsToBeRemoved=$(mktemp)

                echo "${values1}" > "${tempFile1}" 
                echo "${values2}" > "${tempFile2}"
                sed -i 's/ /\n/g' "${tempFile1}"
                sed -i "s/ /\n/g" "${tempFile2}" 
                comm  -12 "${tempFile1}" "${tempFile2}" > "${symbolsToBeRemoved}"

                while read -r currentSymbol; do
                    sed -i "/^${target2}=/ s/${currentSymbol}//g" "${fileName}"
                done < "${symbolsToBeRemoved}"

                rm -r "${symbolsToBeRemoved}"
                rm -r "${tempFile1}"
                rm -r "${tempFile2}"
            fi
        done < "${fileName}"
    fi
done < "${fileName}"
