#!/bin/bash

counterForLines1=0
counterForLines2=0
counterForLines3=0

counterForWords1=0
counterForWords2=0
counterForWords3=0

counterForSymbols1=0
counterForSymbols2=0
counterForSymbols3=0

while read -r line; do
  counterForSymbols1=$((counterForSymbols1 + ${#line}))
done < dir5/file1

while read -r line; do
  counterForSymbols2=$((counterForSymbols2 + ${#line}))
done < dir5/file2

while read -r line; do
  counterForSymbols3=$((counterForSymbols3 + ${#line}))
done < dir5/file3

while read -ra tokens; do
  for((i=0;i<"${#tokens[@]}";i++)); do
    ((counterForWords1++))
  done
  ((counterForLines1++))
done < dir5/file1

while read -ra tokens; do
  for((i=0;i<"${#tokens[@]}";i++)); do
    ((counterForWords2++))
  done
  ((counterForLines2++))
done < dir5/file2

while read -ra tokens; do
  for((i=0;i<"${#tokens[@]}";i++)); do
    ((counterForWords3++))
  done
  ((counterForLines3++))
done < dir5/file3

echo "$counterForLines1", "$counterForLines2", "$counterForLines3" 
echo "$counterForWords1", "$counterForWords2", "$counterForWords3"
echo "$counterForSymbols1", "$counterForSymbols2", "$counterForSymbols3" 
printf "\n"

finalCounterForLines=$((counterForLines1 + counterForLines2 + counterForLines3))
finalCounterForWords=$((counterForWords1 + counterForWords2 + counterForWords3))
finalCounterForSymbols=$((counterForSymbols1 + counterForSymbols2 + counterForSymbols3))
printf "\n"

echo "$finalCounterForWords", "$finalCounterForSymbols"
echo "$finalCounterForLines"
