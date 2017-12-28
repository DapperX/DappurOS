SRC_C = $(wildcard *.c)
SRC_ASM = $(wildcard *.asm)
OBJ = $(SRC_C:%.c=%.o) $(SRC_ASM:%.asm=%.o)
CFLAG += $(DIR_INCLUDE:%=-I%)

%.o : %.c ;$(CC) $< $(CFLAG) -c -o $@
%.o : %.asm ;$(ASM) $< $(ASMFLAG) -o $@

%.d : %.c
	@$(CC) $< $(CFLAG) -MM > $@

sinclude $(SRC_C:%.c=%.d)
