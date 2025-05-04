#!/bin/bash
cat /etc/passwd | cut -d ':' -f 5 | tr -s ',' ' ' | awk 'length($2) <= 7' | cut -d ' ' -f 1,2
