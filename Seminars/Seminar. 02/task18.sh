find /etc -type f -exec stat --format="%s %n" {} \; | sort -n | head -n 1
