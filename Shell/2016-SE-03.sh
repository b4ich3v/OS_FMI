#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
  echo "Error"
  exit 1
fi

while read -r line; do
    user=$(echo "${line}" | cut -d ':' -f 1)
    dir=$(echo "${line}" | cut -d ':' -f 6)

    if [ -z "${dir}" ] || [ ! -d "${dir}" ]; then
        echo "${line}"
    fi

    if sudo -u "${user}" -r "${dir}"; then
        echo "${line}"
    fi

done < /etc/passwd
