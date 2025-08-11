#!/bin/bash

if [ "$(id -u)" -ne 0 ] ; then
	echo "Inalid argument"
	exit 1
fi

cat /etc/passwd | awk -F ':' '{
	found=0

	for(i=1;i<=NF;i++)
	{

		if($i ~ /home/)
		{

			found=1
			break

		}
		
	}

	if(!found) { print $0 }
}'

exit 0
