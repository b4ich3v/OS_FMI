#!/bin/bash

tempFile=$(mktemp)

linesOriginalArr=()
newArr=()
index=0

while read -r line; do
	linesOriginalArr+=($line)
	((index++))
done < /etc/passwd

cat /etc/passwd | cut -d ':' -f 5 | tr -s ',' ' ' | awk 'length($2) <= 7' | cut -d ' ' -f 1,2 > ${tempFile}

while read -r line; do
	newArr+=($line)
done < $tempFile


for ((i=0;i<index;i++)) do
  for((j=0;j<index;j++)) do
    if [[ "${linesOriginalArr[j]}" == *"${newArr[i]}"* ]]; then
	    echo "${linesOriginalArr[j]}"
	  fi
  done
done

rm "$tempFile"
