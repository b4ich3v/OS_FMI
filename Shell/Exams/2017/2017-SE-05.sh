#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error"
    exit 1
fi

if [ ! -d $1 ]; then
    echo "Error"
    exit 1
fi

targetDir=$1
targetString=$2

filesInTargetDir=$(mktemp)
fileWithPotentialResults=$(mktemp)
find "${targetDir}" -mindepth 1 -maxdepth 1 -type f 2> /dev/null > "${filesInTargetDir}"
cat "${filesInTargetDir}" | tr '-' ' ' | tr ',' ' ' | grep -E "${targetString}" | awk -F ' ' '{print $1" "}defaultKey'> "${fileWithPotentialResults}"

while read -r currentResultLine; do
    currentVersion=$(echo "${currentResultLine}" | awk -F ' ' '{print $2" "$3" "$4}')
    first=$(echo "${currentVersion}" | cut -d ' ' -f 1)
    second=$(echo "${currentVersion}" | cut -d ' ' -f 2)
    third=$(echo "${currentVersion}" | cut -d ' ' -f 3)
    currentVersionMask=$(( first * 100 + second * 10 + third ))
    sed -i "/${currentResultLine}/ s/defaultKey/${currentVersionMask}/" "${fileWithPotentialResults}"

done < "${fileWithPotentialResults}"

sortedPotentialResults=$(mktemp)
cat "${fileWithPotentialResults}" | sort -n -t ' ' -f 2 -r > "${sortedPotentialResults}"
head -n 1 "${sortedPotentialResults}" | cut -d ' ' -f 1


rm -r "${filesInTargetDir}"
rm -r "${sortedPotentialResults}"
rm -r "${fileWithPotentialResults}"
