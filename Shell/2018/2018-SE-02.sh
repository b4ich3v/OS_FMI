#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error"
    exit 1
fi

if [ ! -f $1 ]; then
    echo "Error"
    exit 1
fi

if [ ! -d $2 ]; then
    echo "Error"
    exit 1
fi

inputFile=$1
inputDir=$2
resultDictFile="dict.txt"

cd "${inputDir}"
touch "${resultDictFile}"

fileWithUserFields=$(mktemp)
cat "${inputFile}" | tr -s ' ' | cut -d ' ' -f 1,2 | sort > "${fileWithUserFields}"

prevUser=$(cat "${fileWithUserFields}" | head -n 1)
staticVariableMask=0
echo "${prevUser};${staticVariableMask}" >> "${resultDictFile}"
touch "${staticVariableMask}.txt"

while read -r currentUserLine; do
    currentUser=$(echo "${currentUserLine}")

    if [ "${currentUser}" != "${prevUser}" ]; then
        echo "${currentUser};${staticVariableMask}" >> "${resultDictFile}"
        touch "${staticVariableMask}.txt"

        staticVariableMask=$(( staticVariableMask + 1 ))
        prevUser="${currentUser}"
    fi

done < "${fileWithUserFields}"

while read -r currentLine; do
    currentUser=$(echo "${currentLine}" | tr -s ' ' | cut -d ' ' -f 1,2)
    currentSentance=$(echo "${currentLine}" | tr -s ' ' | tr ': ' ':' | tr ' :' ':' | cut -d ':' -f 2)

    if cat "${resultDictFile}" | grep -E "${currentUser}"; then
        idNumber=$(grep -E "^${currentUser};" "${resultDictFile}" | cut -d ';' -f 2)
        echo "${currentSentance}" >> "${idNumber}.txt"
    fi

done < "${inputFile}"

rm -r "${fileWithUserFields}"
