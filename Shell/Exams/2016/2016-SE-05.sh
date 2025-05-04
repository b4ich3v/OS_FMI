#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error"
    exit 1
fi

if ! find . -type f -exec grep "$1" {} ';'; then
    echo "Error"
    exit 1
fi

if ! find . -type f -exec grep "$2" {} ';'; then
    echo "Error"
    exit 1
fi

file1=$1
file2=$2

winnerFlag=1
countOfLines1=$(cat "${file1}" | grep "${file1}" | wc -l)
countOfLines2=$(cat "${file2}" | grep "${file2}" | wc -l)

if [ "${countOfLines1}" -lt "${countOfLines2}" ]; then
    winnerFlag=$(( winnerFlag + 1 ))
fi

if [ "${winnerFlag}" -eq 1 ]; then
    touch "${file1}".songs
    cat "${file1}" | sed -E "s/^[1-9][0-9]*г\.$//" | sed -E "s/^[[:space:]]*//" | sort > "${file1}".songs
else
    touch "${file2}".songs
    cat "${file2}" | sed -E "s/^[1-9][0-9]*г\.$//" | sed -E "s/^[[:space:]]*//" | sort > "${file2}".songs
fi
