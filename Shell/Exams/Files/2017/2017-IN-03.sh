#!/bin/bash

if [ "$#" -ne 3 ] || [ ! -f "$1" ]; then
	echo "Wrong number of arguments or invalid input"
	exit 1
fi

key1=$2
key2=$3

terms1=""
terms2=""

while IFS= read -r currentLine; do
	if echo "$currentLine" | grep -Eq "^$key1="; then
		terms1=$(echo "$currentLine" | cut -d "=" -f 2)
	fi

	if echo "$currentLine" | grep -Eq "^$key2="; then
		terms2=$(echo "$currentLine" | cut -d "=" -f 2)
	fi

done < "$1"

combined=$(echo "$terms1 $terms2" | tr ' ' '\n' | awk '!seen[$0]++' | tr '\n' ' ')

if grep -q "^$key2=" "$1"; then
	sed -i "s|^$key2=.*|$key2=$combined|" "$1"
else
	echo "$key2=$combined" >> "$1"
fi

exit 0

