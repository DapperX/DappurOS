relocateELF :: $(DIR_MAIN)/tool/fakeLib $(DIR_MAIN)/init
testExt2 :: 
all_dependence = $(DIR_MAIN)/tool/fakeLib $(DIR_MAIN)/init
all_test = relocateELF testExt2
$(all_dependence) $(all_test)::
	$(MAKE) all -C $@
dependence.makefile : gen_dependence.py $(all_test:%=%/dependence.inc)
