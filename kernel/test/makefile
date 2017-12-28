TESTMOD = $(patsubst ./%,%,$(shell find . -mindepth 1 -maxdepth 1 -type d))

.PHONY : all

all:
	rtn=`./list.sh $(TESTMOD)`;if [ -d "$$rtn" ]; then $(MAKE) run -C $$rtn; fi