#!/bin/bash
cat /etc/passwd | cut -d ':' -f 1 > users
