-include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

CC = gcc

.PHONY : all clean

all : libDPOS_fake.a

libDPOS_fake.a : $(OBJ)
	$(AR) rcs $@ $^

clean :
	-rm -f *.d *.o *.a