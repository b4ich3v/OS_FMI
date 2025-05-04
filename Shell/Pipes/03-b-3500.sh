#!/bin/bash

counter=0
target="#!/bin/bash"

counter=$(find . -type f -exec grep -l "$target" {} \; | wc -l)
echo "$counter"
