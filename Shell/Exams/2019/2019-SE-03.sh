#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Употреба: $0 <директория>"
    exit 1
fi

dir="$1"
hashFile="$HOME/.archive_hashes"
extractDir="/extracted"

mkdir -p "$extractDir"
touch "$hashFile"
tempHashes=$(mktemp)

find "$dir" -type f -name "*_report-*.tgz" -print | while IFS= read -r currentFileName; do
    baseFile=$(basename "$currentFileName")

    if echo "$baseFile" | grep -qE '^[^_]+_report-[0-9]+\.tgz$'; then
        name=$(echo "$baseFile" | sed -E 's/^([^_]+)_report-[0-9]+\.tgz$/\1/')
        time=$(echo "$baseFile" | sed -E 's/^[^_]+_report-([0-9]+)\.tgz$/\1/')

        currentHash=$(sha256sum "$currentFileName" | awk '{print $1}')

        if ! grep -q "$currentHash" "$hashFile"; then
            echo "Нов файл: $baseFile"
            tempDir=$(mktemp -d)

            tar -xzf "$currentFileName" -C "$tempDir"

            if [ -f "$tempDir/meow.txt" ]; then
                cp "$tempDir/meow.txt" "$extractDir/${name}_${time}.txt"
                echo "Извлечено: ${name}_${time}.txt"
            fi

            rm -r "$tempDir"
            echo "$currentHash $currentFileName" >> "$tempHashes"
        fi
    fi
done

cat "$tempHashes" >> "$hashFile"
rm -r "$tempHashes"
