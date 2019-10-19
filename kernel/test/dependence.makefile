pmm :: $(DIR_MAIN)/tool/fakeLib $(DIR_MAIN)/mm
testExt2 :: 
relocateELF :: $(DIR_MAIN)/tool/fakeLib $(DIR_MAIN)/init
all_dependence = $(DIR_MAIN)/tool/fakeLib $(DIR_MAIN)/init $(DIR_MAIN)/mm
all_test = pmm testExt2 relocateELF
$(all_dependence) $(all_test)::
	$(MAKE) all -C $@
dependence.makefile : gen_dependence.py $(all_test:%=%/dependence.inc)
