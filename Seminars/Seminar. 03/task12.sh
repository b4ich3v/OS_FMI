cat /etc/passwd | grep $(whoami)
cat /etc/passwd | grep $(whoami) -B 2
cat /etc/passwd | grep $(whoami) -B 2 -A 3
cat /etc/passwd | grep $(whoami) -B 2 -A 3 | head -n 1
