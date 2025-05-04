#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
    echo "Error"
    exit 1
fi

fileForPredicate1=$(mktemp)
fileForPredicate2=$(mktemp)
fileWithAllUsersAndDirs=$(mktemp)

cat /etc/passwd | grep -v "home" | cut -d ':' -f 1 | sort > "${fileForPredicate1}"
cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ' | sort -t ' ' -k 1 > "${fileWithAllUsersAndDirs}"

while read line; do
    currentUser=$(echo "${line}" | cut -d ' ' -f 1)
    currentDir=$(echo "${line}" | cut -d ' ' -f 2)

    if ! sudo -u "${currentUser}" test -w "${currentDir}"; then
        echo "${currentUser}" >> "${fileForPredicate2}"
    fi

done < "${fileWithAllUsersAndDirs}"

cat "${fileForPredicate1}" "${fileForPredicate2}" | sort -u
rm -r "${fileWithAllUsersAndDirs}"
rm -r "${fileForPredicate1}"
rm -r "${fileForPredicate2}"
