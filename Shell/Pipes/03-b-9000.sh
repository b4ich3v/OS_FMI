#!/bin/bash

tempFile1=$(mktemp)
element="defaultKey defaultValue"
find "$HOME" -type f -mmin -15
touch eternity

while read -r line; do
    value=$(stat -c %Y "${line}")
    key="${line}"
    currentElement=$(echo "${element}" | sed -e "s|defaultKey|${key}|g" -e "s|defaultValue|${value}|g")
    echo "${currentElement}" >> eternity
done < "${tempFile1}"

rm -r "${tempFile1}"
