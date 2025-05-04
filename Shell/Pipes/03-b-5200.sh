#!/bin/bash

counter=0
target="a"

while read -r line; do
  size=${#line}

  for((i=0;i<size;i++)); do
    currentSymb="${line:$i:1}"

    if [ "${currentSymb}" != "${target}" ]; then
      ((counter++))
    fi
  done
done < /etc/passwd

echo "${counter}"
