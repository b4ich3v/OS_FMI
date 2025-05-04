#!/bin/bash

declare -A unorderedMap
tempFile=$(mktemp)
target="#!"

find /usr -type f | grep -E '\.sh$' > "${tempFile}"

while read -r currentDirectory; do
  currentInterpreter=$(head -n 1 "${currentDirectory}")

  if [[ "${currentInterpreter}" == "${target}"* ]]; then
    ((unorderedMap["${currentInterpreter}"]++))
  fi

done < "${tempFile}"

for key in "${!unorderedMap[@]}"; do
  echo "${key}" has "${unorderedMap[$key]}" ocuuraces
done

rm -r "${tempFile}"
