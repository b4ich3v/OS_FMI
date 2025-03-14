mkdir resulDir
find /etc -type f -perm 444 -exec cp {} resultDir \;
