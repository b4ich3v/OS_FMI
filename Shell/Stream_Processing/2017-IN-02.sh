find . -type f -size 0 -exec rm {} \;
find $HOME -type f -exec du -h {} + | sort -rh | head -n 5 | cut -f 2 | xargs rm
