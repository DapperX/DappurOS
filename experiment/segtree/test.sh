#!/bin/bash

i=0
while make test 2>/dev/null;
do
	echo "test #$i"
	i=$(($i+1))
done
