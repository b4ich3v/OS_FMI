#!/bin/bash

if [ "$#" -ne 2 ] || [ ! -f "$1" ] || [ ! -f "$2" ]; then
	echo "Wrong number of arguments or does'n exist"
	exit 1
fi

counter1=$(cat "$1" | grep -E "$1" | wc -l)
counter2=$(cat "$2" | grep -E "$2" | wc -l)
firstHasMore=$(echo "$counter1 > $counter2" | bc)
executingFile="OK"

if [ "$firstHasMore" -eq 1 ]; then
	executingFile="$1"	
else
	executingFile="$2"
fi

resultFileName="$executingFile.songs"
touch "$resultFileName"
cat "$executingFile" | cut -d '-' -f2 | sed -E "s/[\t\r ]//" | sort 1> "$resultFileName"

exit 0
