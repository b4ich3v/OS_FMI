#!/bin/bash
cat /etc/passwd | cut -d ':' -f 5 | cut -d ',' -f 1 | cut -d ' ' -f 2 | awk 'length($0) > 6'
