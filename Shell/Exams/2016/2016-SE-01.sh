#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Error"
    exit 1
fi

if [ ! -d $1 ]; then
    echo "Error"
    exit 1
fi

dir=$1
find "${dir}" -type l -exec test ! -e {} ';' -print
