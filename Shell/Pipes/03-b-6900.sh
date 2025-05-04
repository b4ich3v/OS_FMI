#!/bin/bash

tempFile1=$(mktemp)
tempFile2=$(mktemp)

ls -t | head -n 10 > "${tempFile1}"
ls -u | head -n 10 > "${tempFile2}"

while read -r line; do
  echo "${line}"
done < "${tempFile1}" 

echo ' '

while read -r line; do
  echo "${line}"
done < "${tempFile2}"

rm -r "${tempFile1}"
rm -r "${tempFile2}"
