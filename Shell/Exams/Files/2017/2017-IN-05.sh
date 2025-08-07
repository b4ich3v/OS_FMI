#!/bin/bash

fileWithAllUsers=$(mktemp)
resultFile=$(mktemp)
cat /etc/passwd | tr -s ',' | sed "s/,//" | cut -d ':' -f 1,6 | sed "s/:/ /" 1> "$fileWithAllUsers" 2> "/dev/null"

while IFS= read -r currentUserHomedir; do
    dir=$(echo "$currentUserHomedir" | cut -d ' ' -f2)
    user=$(echo "$currentUserHomedir" | cut -d ' ' -f1)
    bestFilePerUserWithSeconds=$(find "$dir" -type f -exec stat -c "%Y %n" {} ';'  2> "/dev/null" | sort -t ' ' -k1 -nr | head -n 1)

    echo "$user $bestFilePerUserWithSeconds" >> "$resultFile"

done < "$fileWithAllUsers"

cat "$resultFile" | sort -t ' ' -k 2 | head -n 1 | cut -d ' ' -f 1,3

rm -r "$resultFile"
rm -r "$fileWithAllUsers"
exit 0
