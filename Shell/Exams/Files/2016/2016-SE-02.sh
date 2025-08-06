#!/bin/bash

if [ "$(id -u)" -ne 0 ] || [ "$#" -ne 1 ]; then
	echo "Inalid argument or wrong number of arguments"
	exit 1
fi

if ! echo "$1" | grep -Eq "^-?[1-9]+[0-9]*$"; then
	echo "Not a number"
	exit 1
fi

tempFile=$(mktemp)
ps -e -o uid,pid,rss | tr -s ' ' | tail -n -2 | sort -t ' ' -k1 -n 1> "$tempFile"
cat "$tempFile"

firstLine=$(head -n 1 "$tempFile")

prevId=$(echo "$firstLine" | awk -F ' ' '{ print $1 }')
maxPidPerUser=-1
maxRss=0
rssCounter=0

while IFS= read -r currentLine; do
	currentId=$(echo "$currentLine" | awk -F " " '{print $1}')
	currentPid=$(echo "$currentLine" | awk -F " " '{ print $2 }')
	currentRss=$(echo "$currentLine" | awk -F " " '{ print $3 }')

	if [ "$currentId" -ne "$prevId" ]; then
		if [ "$rssCounter" -gt "$1" ]; then
			kill -9 "$maxPidPerUser"
		fi
		
		maxPidPerUser=-1
		rssCounter=0
		prevId="$currentId"

		continue
	fi

	if [ "$maxRss" -lt "$currentRss" ]; then
		maxPidPerUser="$currentPid"
		maxRss="$currentRss"
	fi

	rssCounter=$((rssCounter+currentRss))

done < "$tempFile"


if [ "$rssCounter" -gt "$1" ]; then
	kill -9 "$maxPidPerUser"
fi

rm -r "$tempFile"
exit 0

