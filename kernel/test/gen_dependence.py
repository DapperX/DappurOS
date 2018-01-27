#-*- coding: UTF-8 -*-

import os


def get_dependence(name):
	with open(name+'/dependence.inc') as file:
		content=file.readlines()
	return map(str.strip,filter(lambda x:""!=x.strip(),content))


all_dependence=set()
all_test=[]
for file in os.listdir('.'):
	if os.path.isdir(file):
		dependence=get_dependence(file)
		all_dependence.update(dependence)
		all_test.append(file)
		print(file+' :: '+" ".join(dependence))

print("all_dependence = "+" ".join(all_dependence))
print("all_test = "+" ".join(all_test))
print("$(all_dependence) $(all_test)::\n\t$(MAKE) all -C $@")
print("dependence.makefile : gen_dependence.py $(all_test:%=%/dependence.inc)")