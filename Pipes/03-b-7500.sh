#!/bin/bash

tempFile1=$(mktemp)
tempFile2=$(mktemp)
cat /etc/services | tr "A-Z" "a-z" | grep -oE '[a-z]+' > "${tempFile1}"

declare -A unorderedMap

while read -ra tokens; do
  size="${#tokens[@]}"

  for((i=0;i<size;i++)); do
    ((unorderedMap["${tokens[$i]}"]++))
  done

done < "${tempFile1}"

for key in "${!unorderedMap[@]}"; do
  echo "${key}" has "${unorderedMap[$key]}" occurrences >> "${tempFile2}";
done

sort -n -t ' ' -k 3 "${tempFile2}" -r | head -n 10 | cut -d ' ' -f 1
rm -r "${tempFile1}"
rm -r "${tempFile2}"
