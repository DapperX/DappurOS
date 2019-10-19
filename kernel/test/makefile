TESTMOD = $(patsubst ./%,%,$(shell find . -mindepth 1 -maxdepth 1 -type d))
ifneq ($(MODE), nodebug)
	CFLAG += -g
endif
CFLAG += -DEXTEST

.PHONY : all update

all : dependence.makefile
	rtn=`./list.sh $(TESTMOD)`;if [ -d "$$rtn" ]; then $(MAKE) $$rtn -f $<; fi

update : dependence.makefile

dependence.makefile : $(TESTMOD:%=%/dependence.inc)
	python gen_dependence.py > $@

sinclude dependence.makefile