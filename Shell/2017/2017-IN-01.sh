#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Error"
    exit 1
fi

if ! find . -type f -exec grep -q $1 {} ';'; then
    echo "Error"
    exit 1
fi

file=$1
target1=$2
target2=$3

tempFile1=$(mktemp)
tempFile2=$(mktemp)
tempFile3=$(mktemp)

sortedTempFile1=$(mktemp)
sortedTempFile2=$(mktemp)

cat "${file}" | grep -E "${target1}=" | cut -d '=' -f 2 > "${tempFile1}"
cat "${file}" | grep -E "${target2}=" | cut -d '=' -f 2 > "${tempFile2}"
sed -i "s# #\\n#g" "${tempFile1}"
sed -i "s# #\\n#g" "${tempFile2}"

sort "${tempFile1}" > "${sortedTempFile1}"
sort "${tempFile2}" > "${sortedTempFile2}"

comm -13 "${sortedTempFile1}" "${sortedTempFile2}" > "${tempFile3}"
sed -i "s#\\n# #g" "${tempFile3}"
value=$(cat "${tempFile3}")
sed -i -E "s#${target2}=.*#${target2}=${value}#" "${file}"

rm -r "${tempFile3}"
rm -r "${sortedTempFile1}"
rm -r "${sortedTempFile2}"
rm -r "${tempFile2}"
rm -r "${tempFile1}"
