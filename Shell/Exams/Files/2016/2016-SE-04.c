#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

left=$1
right=$2

tempFile=$(mktemp)
find . -type f 1> "$tempFile" 2> "/dev/null"

mkdir -p "a"
mkdir -p "b"
mkdir -p "c"

while IFS= read -r currentFileName; do
	currentCountOfLines=$(cat "$currentFileName" | wc -l)
	filename=$(basename "$currentFileName")

	if [ "$currentCountOfLines" -lt "$left" ]; then
		mv "$currentFileName" "./a/$filename" 	

	elif [[ "$currentCountOfLines" -gt "$left" && "$currentCountOfLines" -lt "$right" ]]; then
		mv "$currentFileName" "./b/$filename"
	else
		mv "$currentFileName" "./c/$filename"
	fi

done < "$tempFile"

rm -r "$tempFile"
exit 0
