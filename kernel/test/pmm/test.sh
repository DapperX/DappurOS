#!/bin/bash

i=0
while ./pmm.test >stdout.log 2>/dev/null;
do
	echo "test #$i"
	i=$(($i+1))
done
