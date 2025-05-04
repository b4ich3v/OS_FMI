#!/bin/bash

touch resultFile

for file in file1 file2 file3; do
  while read -r line; do
    echo "${line}" >> resultFile
  done < "$file"
done
