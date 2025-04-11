#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
    echo "Error"
    exit 1
fi

resultFile=$(mktemp)
fileWithUsersAndProccesses=$(mktemp)
ps aux | awk -F ' ' '{print $1" "}defaultKey' | uniq > "${resultFile}"
ps aux | cut -d ' ' -f 1,6 | sort > "${fileWithUsersAndProccesses}"

prevUser=$(cat "${fileWithUsersAndProccesses}" | awk -F ' ' '{print $1}' | head -n 1)
counterForRSS=0

while read -r currentUserAndProccess; do
    currentUser=$(echo "${currentUserAndProccess}" | cut -d ' ' -f 1)
    currentProccessRSS=$(echo "${currentUserAndProccess}" | cut -d ' ' -f 2)

    if [ "${currentUser}" != "${prevUser}" ]; then
        sed -i "/^${prevUser}/" s/defaultKey/${counterForRSS}/ "${resultFile}" 
        prevUser="${currentUser}"
        counterForRSS="${currentProccessRSS}"
    else
        counter=$(( counter + currentProccessRSS ))
    fi

done < "${fileWithUsersAndProccesses}"

countOfAllRSS=0

while read -r currentUserAndRSS; do
    currentRSS=$(echo "${currentUserAndRSS}" | cut -d ' ' -f 2)
    countOfAllRSS=$(( countOfAllRSS + currentRSS ))
done < "${resultFile}"

countOfUsers=$(cat "${resultFile}" | wc -l)
avarageRSS=$(( countOfAllRSS / countOfUsers ))
fileWithUserAndPID=$(mktemp)
sortedUsersAndRSS=$(mktemp)
cat "${resultFile}" | sort -n -t ' ' -k 2 -r > "${sortedUsersAndRSS}"
ps aux | awk -F ' ' '{print $1" "$2}' > "${fileWithUserAndPID}"

targetUserLine=$(cat "${sortedUsersAndRss}" | head -n 1)
targetUser=$(echo "${targetUserLine}" | cut -d ' ' -f 1)
targetRSS=$(echo "${targetUserLine}" | cut -d ' ' -f 2)

if [ "${targetRSS}" -gt "${avarageRSS}" ]; then
    targetPID=$(cat "${fileWithUserAndPID}" | grep -E "${targetUser}" | awk -F ' ' '{print $2}')
    kill "${targetPID}"
fi

cat "${resultFile}"
rm -r "${sortedUsersAndRSS}"
rm -r "${fileWithUserAndPID}"
rm -r "${fileWithUsersAndProccesses}"
rm -r "${resultFile}"
