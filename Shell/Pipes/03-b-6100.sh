#!/bin/bash

for((i=28;i<=46;i++)); do
  cat /etc/passwd | head -n "$i" | tail -n 1 | cut -d ':' -f 3
done
