TESTMOD = $(patsubst ./%,%,$(shell find . -mindepth 1 -maxdepth 1 -type d))

.PHONY : viewList

viewList:
	rtn=`./list.sh $(TESTMOD)`;if [ -d "$$rtn" ]; then $(MAKE) -C $$rtn; fi