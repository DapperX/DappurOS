TESTMOD = $(patsubst ./%,%,$(shell find . -mindepth 1 -maxdepth 1 -type d))

.PHONY : all update

all : dependence.makefile
	rtn=`./list.sh $(TESTMOD)`;if [ -d "$$rtn" ]; then $(MAKE) $$rtn -f $<; fi

update : dependence.makefile

dependence.makefile :
	python gen_dependence.py > $@

sinclude dependence.makefile