OBJ = control.o
ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += $(DIR_INCLUDE:%=-I%)
DEPEND += $(PATH_LIB)

$(DIR_BIN)/control : $(DEPEND) $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -r -Tlinker.ld -o $@

%.o : %.c ;$(CC) $< $(CFLAG) -c -o $@
%.o : %.asm ;$(ASM) $< $(ASMFLAG) -o $@