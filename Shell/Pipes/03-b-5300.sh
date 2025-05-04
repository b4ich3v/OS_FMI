#!/bin/bash

tempFile=$(mktemp)
cat /etc/passwd | cut -d ':' -f 5 | tr -s ',' ' ' | cut -d ' ' -f 1,2 > "${tempFile}"

while read -r line; do
  size="${#line}"
  declare -A unorderedMap

  for((i=0;i<size;i++)); do
    currentSymbol="${line:$i:1}"
    if [ "${currentSymbol}" != " " ]; then
      (( unorderedMap["${currentSymbol}"]++ ))
    fi
  done

  for key in "${!unorderedMap[@]}"; do
    if [ "${unorderedMap["${key}"]}" -eq 1 ]; then
      echo "$line has '$key' as unique symbol"
    fi
  done
done < "${tempFile}"

rm -r tempFile
