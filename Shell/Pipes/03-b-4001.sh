#!/bin/bash

arrOfLines=()
tempFile=$(mktemp)

while read -r currentLine; do
  newLineToBeReplaced=$(echo "$currentLine" | tr "a-z" "A-Z")
  arrOfLines+=("$newLineToBeReplaced")
done < file1

for((i=0;i<"${#arrOfLines[@]}";i++)); do
  echo "${arrOfLines[i]}" >> "$tempFile" 
done

mv "$tempFile" file1
