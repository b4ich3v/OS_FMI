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

cat "${resultFile}"
rm -r "${fileWithUsersAndProccesses}"
rm -r "${resultFile}"
