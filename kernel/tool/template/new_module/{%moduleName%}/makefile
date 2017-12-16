OBJ = {%moduleName%}.o
ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += -nostdinc $(DIR_INCLUDE:%=-I%)
DEPEND += $(PATH_LIB)

$(DIR_BIN)/{%moduleName%} : $(DEPEND) $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -r -Tlinker.ld -o $@

%.o : %.c ;$(CC) $< $(CFLAG) -c -o $@
%.o : %.asm ;$(ASM) $< $(ASMFLAG) -o $@
