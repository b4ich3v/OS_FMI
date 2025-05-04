#!/bin/bash
cat /etc/passwd | tr -s ',' ':' | cut -d ':' -f 1,5
