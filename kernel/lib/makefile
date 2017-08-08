CC := gcc
SRC = $(wildcard *.c) $(wildcard *.asm)
OBJ = $(patsubst %,%.o,$(basename $(SRC)))
CFLAG += $(DIR_INCLUDE:%=-I%)


.PHONY : build clean

build : libDPOS_main.a

%.o : %.c ;$(CC) $< $(CFLAG) -c -o $@
%.o : %.asm ;$(ASM) $< $(ASMFLAG) -o $@

%.d : %.c
	$(CC) $< $(DIR_INCLUDE:%=-I%) -MM > $@

sinclude $(SRC:.c=.d)

libDPOS_main.a : $(OBJ)
	$(AR) rcs $@ $^

clean :
	-rm -f *.d *.o *.a