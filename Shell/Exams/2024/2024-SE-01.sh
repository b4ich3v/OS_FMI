#!/bin/bash

files=$(mktemp)
hashTable=$(mktemp)

printf "%s\n" "$@" | while IFS= read -r currentArgument; do
    if echo "$currentArgument" | grepe -E -q "^-R[a-zA-Z0-9]+=[a-zA-Z0-9]+$"; then
        key=$(echo "$currentArgument" | sed -E "s/-R//" | cut -d '=' -f 1)
        value=$(echo "$currentArgument" | sed -E "s/-R//" | cut -d '=' -f 2)
        echo "$key $value" >> "$hashTable"
    else
        echo "$currentArgument" >> "$files"
    fi
done 

while IFS= read -r currentFile; do
    tempFile=$(mktemp)
    cat "$currentFile" > "$tempFile"

    while IFS= read -r keyValue; do
        key=$(echo "$keyValue" | cut -d ' ' -f 1)
        value=$(echo "$keyValue" | cut -d ' ' -f 2)

        sed -i "s/^$key$/<$key\|$value>/g" "$tempFile"
    done < "$hashTable"

    while IFS= read -r keyValue; do
        key=$(echo "$keyValue" | cut -d ' ' -f 1)
        value=$(echo "$keyValue" | cut -d ' ' -f 2)

        sed -i "s/<$key\|$value>/$value/g" "$tempFile"
    done < "$hashTable"

    cat "$tempFile" > "$currentFile"
    rm -r "$tempFile"
done < "$files"

rm -r "$files"
rm -r "$hashTable"
