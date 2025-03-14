cat /etc/passwd | cut -d ':' -f 5 | cut -d ',' -f 1 | awk 'length($0) > 6'
