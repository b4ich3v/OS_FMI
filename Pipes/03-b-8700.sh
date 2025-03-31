#!/bin/bash

tempFile1=$(mktemp)
tempFile2=$(mktemp)
result=$(mktemp)

cat /etc/passwd | cut -d ':' -f 4 > "${tempFile1}" 
cat /etc/group | cut -d ':' -f 1,3 | tr -s ':' ' ' > "${tempFile2}" 

while read -r line1; do
  name=$(echo "${line1}" | cut -d ' ' -f 1)
  numberGID=$(echo "${line1}" | cut -d ' ' -f 2)
  counter=0

  while read -r currentNumber; do
    if [[ "${numberGID}" -eq "${currentNumber}" ]]; then
      ((counter++))
    fi
  done < "${tempFile1}"

  echo "${name}" "${counter}" >> "${result}"

done < "${tempFile2}"

sort -n -t ' ' -k 2 -r "${result}" | head -n 5 | cut -d ' ' -f 1
rm -r "${tempFile1}"
rm -r "${tempFile2}"
rm -r "${result}"
