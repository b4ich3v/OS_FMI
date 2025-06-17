#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OCC="$SCRIPT_DIR/occ"

PASSWD_FILE="${PASSWD:-/etc/passwd}"
local_users=$(awk -F: '$3 >= 1000 { print $1 }' "$PASSWD_FILE" | sort)

prevcloud_users=$( "$OCC" user:list | sed -n 's/^- \([a-zA-Z0-9_]\+\):.*$/\1/p' | sort)

comm -23 <(echo "$local_users") <(echo "$prevcloud_users") | while read -r user; do
    "$OCC" user:add "$user" >/dev/null 2>&1
done

comm -12 <(echo "$local_users") <(echo "$prevcloud_users") | while read -r user; do
    if "$OCC" user:info "$user" | grep -q 'enabled: false'; then
        "$OCC" user:enable "$user" >/dev/null 2>&1
    fi
done

comm -13 <(echo "$local_users") <(echo "$prevcloud_users") | while read -r user; do
    if "$OCC" user:info "$user" | grep -q 'enabled: true'; then
        "$OCC" user:disable "$user" >/dev/null 2>&1
    fi
done
