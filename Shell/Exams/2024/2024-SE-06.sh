#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo Error
    exit 1
fi

while IFS= read -r line; do
    if [ -z "$line" ] || [[ "$line" == \#* ]]; then
        continue
    fi

    name=$(echo "$line" | cut -d ' ' -f 1)
    type=$(echo "$line" | cut -d ' ' -f 2)
    perms=""
    usergroup=""

    if [ "$type" = "file" ] || [ "$type" = "dir" ]; then
        if echo "$(echo "$line" | cut -d ' ' -f 3)" | grep -q ':'; then
            usergroup=$(echo "$line" | cut -d ' ' -f 3)
            perms=$(echo "$line" | cut -d ' ' -f 4)
        else
            usergroup=""
            perms=$(echo "$line" | cut -d ' ' -f 3)
        fi
    elif [ "$type" = "symlink" ]; then
        target=$(echo "$line" | cut -d ' ' -f 3)
        usergroup=""
        perms=""
    elif [ "$type" = "nonexistant" ]; then
        usergroup=""
        perms=""
    fi

    if [ "$type" != "nonexistant" ]; then
        parentdir=$(dirname "$name")
        if [ ! -d "$parentdir" ]; then
            mkdir -p "$parentdir" 
        fi
    fi

    if [ -e "$name" ]; then
        if [ "$type" = "file" ]; then
            if [ ! -f "$name" ]; then
                rm -r "$name"
                touch "$name"
            fi
        elif [ "$type" = "dir" ]; then
            if [ ! -d "$name" ]; then
                rm -r "$name"
                mkdir -p "$name"
            fi
        elif [ "$type" = "symlink" ]; then
            if [ ! -L "$name" ]; then
                rm -r "$name"
                ln -s "$target" "$name"
            else
                currentTarget=$(readlink "$name")
                if [ "$currentTarget" != "$target" ]; then
                    rm "$name"
                    ln -s "$target" "$name"
                fi
            fi
        elif [ "$type" = "nonexistant" ]; then
            rm -r "$name"
            continue
        fi
    else
        if [ "$type" = "file" ]; then
            touch "$name"
        elif [ "$type" = "dir" ]; then
            mkdir -p "$name"
        elif [ "$type" = "symlink" ]; then
            ln -s "$target" "$name"
        fi
    fi

    if [ "$type" = "nonexistant" ]; then
        continue
    fi

    if [ -n "$usergroup" ]; then
        chown "$usergroup" "$name" 2>/dev/null 
    fi

    if [ -n "$perms" ]; then
        chmod "$perms" "$name" 2>/dev/null 
    fi

done < "$1"
