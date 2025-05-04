#!/bin/bash
cat /etc/passwd | tr -s ',' ':' | cut -d ':' -f 5,7 | tr -s ':' ' '
