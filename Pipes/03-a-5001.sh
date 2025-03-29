#!/bin/bash
cat /etc/passwd | cut -d ':' -f 7 | grep '/bin/bash' -v | wc -l
