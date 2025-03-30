#!/bin/bash

tempFile1=$(mktemp)
tempFile2=$(mktemp)

ls -t | head -n 10 > "${tempFile1}"
ls -u | tail -n 10 > "${tempFile2}"

while read -ra tokens; do
  for((i=0;i<10;i++)); do
    echo "${tokens[$i]}" >> "${tempFile1}"
  done
done < "${tempFile1}" 

while read -ra tokens; do
  for((i=0;i<10;i++)); do
    echo "${tokens[$i]}" >> "${tempFile2}"
  done
done < "${tempFile2}" 

cat "${tempFile1}"
cat "${tempFile2}"

rm -r "${tempFile1}"
rm -r "${tempFile2}"
