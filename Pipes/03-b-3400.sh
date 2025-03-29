#!/bin/bash

counter=0
target='#'

while read -ra tokens; do
  for currentToken in "${tokens[@]}"; do
    if [ "${currentToken:0:1}" == "$target" ] 
    then
      ((counter++))
    fi
  done
done < /etc/services

echo "$counter"
