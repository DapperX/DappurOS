OBJ = header.o init_asm.o init_c.o
ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += $(DIR_INCLUDE:%=-I%)
DEPEND += $(PATH_LIB)

$(DIR_BIN)/init : $(DEPEND) $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -s -Tlinker.ld -o $@

header.o : header.asm
	$(ASM) $< $(ASMFLAG) -o $@

init_asm.o : init.asm
	$(ASM) $< $(ASMFLAG) -o $@

init_c.o : init.c
	$(CC) $< $(CFLAG) -c -o $@