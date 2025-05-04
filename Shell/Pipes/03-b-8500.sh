#!/bin/bash

myGroup=$(id -gn)
tempFile=$(mktemp)
cat test1.txt | cut -d ':' -f 1 > "${tempFile}"

while read -r group; do
  if [ "${group}" ==  "${myGroup}" ]; then
    echo Hello, "${group}" - I am here! 
  else
    echo Hello, "${group}"
  fi
done < "${tempFile}"

rm -r "${tempFile}"
