#!/bin/bash

target="1"
tempFile=$(mktemp)

while read -r currentLine; do
  size="${#currentLine}"
  strLine=""

  for((i=0;i<size;i++)); do
    currentSymbol="${currentLine:$i:1}"

    if [ "${currentSymbol}" != "${target}" ]; 
    then
      strLine+="{$currentSymbol}"
    fi
  done
  
  echo "$strLine" >> "${tempFile}" 

done < file1

mv "$tempFile" file1
