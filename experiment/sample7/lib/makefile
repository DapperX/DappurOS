CC := gcc
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
CFLAG += $(DIR_INCLUDE:%=-I%)


.PHONY : build clean

build : libDPOS_main.a

%.d : %.c
	$(CC) $< $(DIR_INCLUDE:%=-I%) -MM | sed 's/\(.*\)[^\]$$/\0\n\t$$(CC) $$< $$(CFLAG) -c -o $$@/g' > $@

sinclude $(SRC:.c=.d)

libDPOS_main.a : $(OBJ)
	$(AR) rcs $@ $^

clean :
	-rm -f *.d *.o *.a