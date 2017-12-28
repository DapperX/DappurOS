include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

CC := gcc

.PHONY : all clean

all : libDPOS_main.a

libDPOS_main.a : $(OBJ)
	$(AR) rcs $@ $^

clean :
	-rm -f *.d *.o *.a