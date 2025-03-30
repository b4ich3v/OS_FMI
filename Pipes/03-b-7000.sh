#!/bin/bash

target1="\.h$"
target2="\.c$"
counter=0

while read -ra tokens; do
  size="${#tokens[@]}"

  for((i=0;i<size;i++)); do
    currentCounter1=$(echo "${tokens[$i]}" | grep -E "${target1}" | wc -l)
    currentCounter2=$(echo "${tokens[$i]}" | grep -E "${target2}" | wc -l)
    
    if [ "${currentCounter1}" -eq 1 ]; then
      ((counter++))
    elif [ "${currentCounter2}" -eq 1 ]; then
      ((counter++))
    fi

  done
done < test1.txt

echo "${counter}"
