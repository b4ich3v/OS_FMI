#!/bin/bash

if [ ! -d "$1" ]; then
    echo "Invalid input"
    exit 1
fi

hasNumber=$(echo "$# == 2" | bc)
dir=$1

if [ "$hasNumber" -eq  1 ]; then

    if ! echo "$2" | grep -Eq "([1-9]+[0-9]*)|(0)"; then
        echo "Not a number"
        exit 1
    fi

    number=$2
    find "$dir" -type f -exec stat -c "%n %h" {} ';' 2> "/dev/null" | awk -F ' ' -v tolerance="$number" '{
        if($2 > tolerance) { print $1 }
    }'

else 
    find "$dir" -type l ! -exec test -e {} ';' -print 2> "/dev/null"  

fi

exit 0
