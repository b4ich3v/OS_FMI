#!/bin/bash

if [ "$#" -ne 2 ] || [ -f "$1" ]; then
    echo "Wrong number of arguments or already existing file"
    exit 1
fi

csv=$1
inputDir=$2

header="hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key"
touch "$csv"
echo "$header" >> "$csv"

while IFS= read -r currentFileName; do
    hostname=$(echo "$currentFileName" | sed -E "s/\.log//")
    counter=0
    row="$hostname"  # тук ще трупаме полетата за този хост

    while IFS= read -r currentLine; do
        if [ "$counter" -eq 9 ]; then
            value=$(echo "$currentLine" | tr -s " " | cut -d ":" -f 2)
            row="$row,$value"
        else
            value=$(echo "$currentLine" | tr -s " " | cut -d ':' -f 2)
            row="$row,$value"
        fi

        if [ "$counter" -eq 7 ]; then
            value=$(echo "$currentLine" | sed -E 's/.*has (an|a) (.+) license\..*/\2/')
            row="$row,$value"
        fi

        counter=$((counter + 1))
    done < "$currentFileName"

    echo "$row" >> "$csv"  
done < <(find "$inputDir" -type f -name "*.log")

exit 0
