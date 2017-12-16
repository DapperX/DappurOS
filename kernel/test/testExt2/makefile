CC := gcc
SRC = $(wildcard *.c) $(wildcard *.asm)
OBJ = $(patsubst %,%.o,$(basename $(SRC)))
DIR_INCLUDE = $(DIR_MAIN)/include $(DIR_MAIN)/fs/ext2
CFLAG += -Werror $(DIR_INCLUDE:%=-I%)


.PHONY : build run clean

run : build
	./testExt2.test

build : testExt2.test

%.o : %.c ;$(CC) $< $(CFLAG) -c -o $@
%.o : %.asm ;$(ASM) $< $(ASMFLAG) -o $@

%.d : %.c
	@$(CC) $< $(CFLAG) -MM > $@

sinclude $(SRC:.c=.d)

testExt2.test : $(OBJ)
	$(CC) $^ $(LDFLAG) -o $@

clean :
	-rm -f *.d *.o *.a
