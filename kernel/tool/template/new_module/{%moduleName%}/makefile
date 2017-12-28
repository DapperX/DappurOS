include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += -nostdinc
DEPEND += $(PATH_LIB)
LDFLAG += -r -s

$(DIR_BIN)/{%moduleName%}.mod : $(DEPEND) $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -Tlinker.ld -o $@