OBJ = header.o init_reg.o init_mem.o
ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += -nostdinc $(DIR_INCLUDE:%=-I%)
DEPEND += $(PATH_LIB)

$(DIR_BIN)/init : $(DEPEND) $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -s -Tlinker.ld -o $@

%.o : %.c ;$(CC) $< $(CFLAG) -c -o $@
%.o : %.asm ;$(ASM) $< $(ASMFLAG) -o $@
