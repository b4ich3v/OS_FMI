#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Error"
    exit 1
fi

if [ ! -d $1 ]; then
    echo "Error"
    exit 1
fi

hasTwoParams=0

if [ $# -eq 2 ]; then
    (( hasTwoParams++ ))
fi

if [ "${hasTwoParams}" -eq 0 ]; then
    countOfBrokenSymlinks=$(find . -type l ! -exec test -e {} ';' -print | wc -l)
    fileWithSymlinkAndFile=$(mktemp)
    find . -type l -exec test -e {} ';' -print | awk '{print $1" "}defaultKey' > "${fileWithSymlinkAndFile}"
    
    while read -r currentSymlink; do
        toBeReplaced=$(read -f "${currentSymlink}")
        sed -i "/${currentSymlink}/ s/defaultKey/${toBeReplaced}/" "${fileWithSymlinkAndFile}"
    done < "${fileWithSymlinkAndFile}"

    cat "${fileWithSymlinkAndFile}"
    echo "${countOfBrokenSymlinks}"
    rm -r "${fileWithSymlinkAndFile}"
else
    if [ ! -f $2 ]; then
        echo "Error"
        exit 1
    fi

    fileName=$2
    countOfBrokenSymlinks=$(find . -type l ! -exec test -e {} ';' -print | wc -l)
    fileWithSymlinkAndFile=$(mktemp)
    find . -type l -exec test -e {} ';' -print | awk '{print $1" "}defaultKey' > "${fileWithSymlinkAndFile}"
    
    while read -r currentSymlink; do
        toBeReplaced=$(readlink -f "${currentSymlink}")
        sed -i "/${currentSymlink}/ s/defaultKey/${toBeReplaced}/" "${fileWithSymlinkAndFile}"
    done < "${fileWithSymlinkAndFile}"

    cat "${fileWithSymlinkAndFile}" >> "${fileName}"
    echo "${countOfBrokenSymlinks}" >> "${fileName}"
    rm -r "${fileWithSymlinkAndFile}"
fi
