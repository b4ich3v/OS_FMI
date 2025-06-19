#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo Error
    exit 1
fi

allFiles=$(mktemp)
tempFile=$(mktemp)
firstRow="hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key"

resultFile="$1"
touch "$resultFile"
echo "$firstRow" > "$resultFile"
find "$2" -type f -name "*.log" 1> "$allFiles" 2> /dev/null

while IFS= read -r currentFileName; do
    baseName=$(basename "$currentFileName")
    hostname=$(echo "$baseName" | sed "s/.log//g")

    values=$(cat "$currentFileName" | tr -s " " | sed "s/ //g" | cut -d ':' -f 2 | grep -v 'license' | awk '
    { gsub(/^[ \t]+|[ \t]+$/, "", $0); if (NR > 1) printf ","; printf "%s", $0 } END { print "" }')

    echo "${hostname}${values}" >> "$resultFile"
done < "$allFiles"

rm -r "$allFiles"
