df -P > file
cat file | tail -n +2 | sort -k 2 -n
