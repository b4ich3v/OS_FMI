#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo Error
    exit 1
fi

target=$1

currentFile=$(cat file.txt | grep -E "^$target" | cut -d ':' -f 1)
rules=$(cat file.txt | grep -E "^$target" | cut -d ':' -f 2)
command=$(cat file.txt | grep -E "^$target" | cut -d ':' -f 3)

echo "$rules" | while IFS= read -r currentRule; do
    file.sh "$currentRule"
done

if [ ! -f "$target" ]; then
    eval "$command"
else
    echo "$rules" | tr -s ' ' '\n' | while IFS= read -r currentRule; do
        if [[ "$(stat -c %Y "$currentRule")" -lt "$(stat -c %Y "$currentFile")" ]]; then
            eval "$command"
            break;
        fi

    done

fi
