#!/bin/bash

tempFile=$(mktemp)
cat test | grep -E "SI" | sort -t ':' -k 1 | cut -d ':' -f 1 > "${tempFile}"

cat "${tempFile}"
rm -r "${tempFile}"
