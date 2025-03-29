#!/bin/bash
df -P > file
cat file | tail -n +2 | tr -s ' ' | sort -n -t ' ' -k 2
