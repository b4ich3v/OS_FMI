#!/bin/bash

declare -a statisticsArray

for((i=0;i<256;i++)); do
  statisticsArray[$i]=0
done

for file in file1 file2 file3; do
  while read -r line; do
    size="${#line}"

    for((i=0;i<size;i++)); do
      currentSymbol="${line:$i:1}"
      ascii=$(printf "%d" "'$currentSymbol")
      statisticsArray["${ascii}"]=$(( statisticsArray["${ascii}"] + 1 ))
    done

  done < "$file"
done

for((i=0;i<256;i++)); do
  currentSymbol=$(printf "\\$(printf '%03o' "$i")")
  echo "${currentSymbol}" : "${statisticsArray[$i]}"
done
