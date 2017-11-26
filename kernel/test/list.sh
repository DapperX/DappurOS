#!/bin/bash

i=0
for test in $@
do
	i=$(($i+1))
	echo "$i : $test" >&2
done
echo '0 : *EXIT*' >&2

n=$i
while true
do
	read -p "Input test id: " testId >&2
	if [[ $testId -ge 0 && $testId -le $n ]]; then
		break
	fi
	echo 'Invalid id' >&2
done

if [[ $testId -ne 0 ]];then
	echo $@ | awk "{print \$$testId}"
fi