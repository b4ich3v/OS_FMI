#!/bin/bash

if [ $# -ne  1 ]; then
    echo "Error"
    exit 1
fi

if ! find . -type d -name $1 2> /dev/null; then
    echo "Error"
    exit 1
fi

yearRegex="([0-9]{4})"
monthRegex="(0[1-9]|1[0-2])"
dayRegex="(0[1-9]|[12][0-9]|3[01])"
hoursRegex="(0[0-9]|1[0-9]|2[0-3])"
minutesRegex="(0[0-9]|[1-5][0-9])"
secondsRegex="(0[0-9]|[1-5][0-9])"
finalSearchingPattern="^${yearRegex} ${monthRegex} ${dayRegex} ${hoursRegex} ${minutesRegex} ${secondsRegex}$"

fileWithAccountsAndAllFiles=$(mktemp)
fileWithAccountsAndValidFiles=$(mktemp)
inputDir=$1

find "${inputDir}" -type f | awk -F '/' '{print $(NF-1)" "$(NF)}' > "${fileWithAccountsAndAllFiles}"

# seperate valid accounts from all accounts
while read -r currentAccAndFilePath; do
    currentAcc=$(echo "${currentAccAndFilePath}" | cut -d ' ' -f 1)
    currentFilePath=$(echo "${currentAccAndFilePath}" | cut -d ' ' -f 2)

    if echo "${currentFilePath}" | grep -E "${finalSearchingPattern}"; then
        if ! grep "${currentAcc}" "${fileWithAccountsAndValidFiles}"; then
            echo "${currentAcc} 0" >> "${fileWithAccountsAndValidFiles}"
        fi
    fi 

done < "${fileWithAccountsAndAllFiles}"

# if we find valid file and valid account, we calculate the lines in this file and increment the second field in 
# result file with this counter
while read -r currentAccAndFilePath; do
    currentAcc=$(echo "${currentAccAndFilePath}" | cut -d ' ' -f 1)
    currentFilePath=$(echo "${currentAccAndFilePath}" | cut -d ' ' -f 2)

    if grep -E "${currentAcc}" "${fileWithAccountsAndValidFiles}"; then
        if echo "${currentFilePath}" | grep -E "${finalSearchingPattern}"; then
            countOfLinesInCurrentFile=$(wc -l < "${currentFilePath}")
            currentCountOfLinesInResultFile=$(cat "${fileWithAccountsAndValidFiles}" | grep -E "${currentAcc}" | cut -d ' ' -f 2)
            newCountOfLines=$(( currentCountOfLinesInResultFile + countOfLinesInCurrentFile ))

            sed -i "s/^${currentAcc} [0-9]\+/${currentAcc} ${newCountOfLines}/" "${fileWithAccountsAndValidFiles}"

        fi
    fi

done < "${fileWithAccountsAndAllFiles}"

cat "${fileWithAccountsAndValidFiles}" | sort -n -t ' ' -f 2 | head -n 10  
rm -r "${fileWithAccountsAndValidFiles}"
rm -r "${fileWithAccountsAndAllFiles}"
