#!/bin/bash
find /etc -type f -perm 444 -exec cp {} myetc \;
