#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "USAGE: $0 <VERSION>" >&2
	exit 1
fi

for f in */Version.txt; do
	echo $1 > $f
done

echo $1 > Version.txt

echo "Please do not forget to add the latest version to updates/parts.txt if appropriate!"
