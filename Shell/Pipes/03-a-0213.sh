#!/bin/bash
cat /etc/passwd | cut -d ':' -f 6 | cut -d '/' -f 2
