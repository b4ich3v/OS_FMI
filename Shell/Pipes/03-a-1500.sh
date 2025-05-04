#!/bin/bash
cat /etc/passwd | wc -c -m -l | tr -s ' '
