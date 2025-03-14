df -P 1> file
cat file | tail -n +2 | tr -s ' ' | sort -n -t ' ' -k 2
