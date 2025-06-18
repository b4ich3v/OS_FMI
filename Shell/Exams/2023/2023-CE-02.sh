#!/bin/bash

if [ $# -ne 3 ]; then
    echo Error
    exit 1
fi

first=$(cat "$1" | grep -E "$3:" | cut -d ':' -f 2 | sed "s/ //" | cut -d ' ' -f 1)
second=$(cat "$2" | grep -E "$3:" | cut -d ':' -f 2 | sed "s/ //" | cut -d ' ' -f 1)

isFirst=$(echo "$first < $second" | bc -l)

if [ "$isFirst" -eq 1 ]; then
    echo "$1"
else
    echo "$2"
fi
