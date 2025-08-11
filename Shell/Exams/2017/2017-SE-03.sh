#!/bin/bash

if [ "$(id -u)" -ne 0 ] || [ "$#" -ne 3 ] || [ ! -d "$1" ]; then
    echo "Wrong number of arguments or "
    exit 1
fi

created=0

if [ ! -d "$2" ]; then
    mkdir -p "$HOME/$2"
    created=1
fi

sourceDir=$1
destinationDir=$2
targetStr=$3
tempFile=$(mktemp)

find "$sourceDir" -type f | awk -v target="$targetStr" '{
    if($1 ~ target) { print $1 }
}' 1> "$tempFile"

while IFS= read -r currentFileName; do
    relpath="${currentFileName#$sourceDir/}"
    targetDir="$destinationDir/$(dirname "$relpath")"

    mkdir -p "$targetDir"

    if [ "$created" -eq 1 ]; then
        mv "$currentFileName" "$HOME/$2/$relpath"
    else
        mv "$currentFileName" "$targetDir/"
    fi
    
done < "$tempFile"

rm -r "$tempFile"
exit 0
