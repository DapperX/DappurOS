#!/bin/bash

function replace_string()
{
	echo "$1" | sed "s/{%moduleName%}/$moduleName/g" | sed "s/{%moduleName_upperCase%}/$moduleName_upperCase/g"
}

function create_module()
{
	dir_search=$1
	dir_dist=$2

	for path in $dir_search/*
	do
		base_dist=$(replace_string `basename $path`)
		path_dist_next=$dir_dist/$base_dist
		if [ -d $path ]; then
			mkdir $path_dist_next
			create_module $path $path_dist_next
		else
			echo "$(replace_string "$(cat $path)")" > $path_dist_next
		fi
	done
}


read -p "input moduleName: " moduleName
echo "create new module '$moduleName'"
moduleName_upperCase=`echo $moduleName | tr '[a-z]' '[A-Z]'`
create_module ./tool/template/new_module .