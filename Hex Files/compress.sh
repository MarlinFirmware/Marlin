#!/bin/bash
for file in *; do
	if [ "$file" != "*" ] ; then
		zip $file.zip $file
	fi
done

