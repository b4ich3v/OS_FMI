#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error"
    exit 1
fi

if [[ ! -f $1 || ! -f $2 ]]; then
    echo "Error"
    exit 1
fi

source=$1
destination=$2

countOfElementsPerLine=$(head -n 1 "${source}" | awk -F ',' '{print NF}')
custumDict=$(mktemp)

while read -r currentLine; do
    echo "${currentLine}" | awk -F ',' -v iter="${countOfElementsPerLine}" -v dummyParam="default" '{ 
        for(i=1; i<=iter; i++){
            if($i == ""){ 
                $i = dummyParam 
            }
        } 
        
        for(i=1; i<=iter; i++){
            printf "%s ", $i
        }
        printf "\n"
    }' >> "${custumDict}"

done < "${source}"

sed -i "s/ /:/" "${custumDict}"
history=$(mktemp)

while read -r currentLine; do
    currentKey=$(echo "${currentLine}" | cut -d ':' -f 1)
    currentValue=$(echo "${currentLine}" | cut -d ':' -f 2)

    if cat "${history}" | grep "${currentValue}"; then
        keyInHistory=$(cat "${history}" | grep "${currentValue}" | cut -d ':' -f 1)

        if [ "${currentKey}" -gt  "${keyInHistory}" ]; then
            sed -i "/${currentValue}/ s/${keyInHistory}/${currentKey}/" "${history}"
        fi

    else
        echo "${currentLine}" >> "${history}"
    fi

done < "${custumDict}"

sed -i "s/:/,/" "${history}"
sed -i "s/ /,/" "${history}"
sed -i "s/ /,/" "${history}"
sed -i "s/default//g" "${history}"

cat "${history}" > "${destination}"
rm -r "${custumDict}"
rm -r "${history}"
