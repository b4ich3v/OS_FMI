#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Error"
    exit 1
fi

if ! find . -type f -exec $1 {} ';'; then
    echo "Error"
    exit 1
fi

containerFile=$(mktemp)
resultFile=$(mktemp)
cat $1 > "${containerFile}"
currentIndex=1

while read -r currentBook; do
    newTempFile=$(mktemp)
    echo "${currentBook}" > "${newTempFile}"

    sed -i -E "s/^[1-9][0-9]* г/${currentIndex}/" "${newTempFile}"
    sed -i -E "s/-//" "${newTempFile}"
    sed -i -E "s/[[:space:]]/ /g" "${newTempFile}"
    cat "${newTempFile}" >> "${resultFile}"

    currentIndex=$(( currentIndex + 1 ))
    rm -r "${newTempFile}"
done < "${containerFile}"

cat "${resultFile}" | sort -t ' ' -k 2
rm -r "${resultFile}" 
rm -r "${containerFile}" 
