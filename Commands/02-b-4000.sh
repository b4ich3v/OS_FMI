#!/bin/bash
cp "$(find /etc -type f -printf '%s %p\n' 2>/dev/null | sort -n | head -n 1 |cut -d ' ' -f 2)" result
