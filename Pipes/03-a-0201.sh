#!/bin/bash
cat /etc/passwd | sort -n -t ':' -k 1
