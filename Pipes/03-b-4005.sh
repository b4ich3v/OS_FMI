#!/bin/bash

tempFile=$(mktemp)

while read -r line; do
  newLine=$(echo "${line}" | tr "A-Z" "a-z")
  echo "${newLine}" >> "$tempFile" 
done < file1

mv tempFile file2
