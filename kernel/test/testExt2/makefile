-include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

CC := gcc
DIR_INCLUDE = $(DIR_MAIN)/include $(DIR_MAIN)/fs/ext2
CFLAG += -Werror


.PHONY : build all clean

all : build
	./testExt2.test

build : testExt2.test

testExt2.test : $(OBJ)
	$(CC) $^ -o $@

clean :
	-rm -f *.d *.o *.a testExt2.test