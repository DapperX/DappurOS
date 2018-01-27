include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

CC = gcc

DIR_LIB_FAKE = $(DIR_MAIN)/tool/fakeLib
LIB_FAKE = DPOS_fake
PATH_LIB_FAKE = $(DIR_LIB_FAKE)/lib$(LIB_FAKE).a

LIB := $(LIB_FAKE) $(LIB)
DIR_LIB += $(DIR_LIB_FAKE)
PATH_LIB += $(PATH_LIB_FAKE)

DIR_INIT = $(DIR_MAIN)/init
DIR_INCLUDE += $(DIR_INIT)
CFLAG += -Werror
DEPEND += $(PATH_LIB)
OBJ_EXTRA += $(DIR_INIT)/init_mem.o


.PHONY : build all clean

all : build test.mod
	./relocateELF.test

build : relocateELF.test

relocateELF.test : $(OBJ) $(OBJ_EXTRA)
	$(CC) $^ $(LDFLAG) -o $@

test.mod : $(DIR_BIN)/control.mod
	cp $< $@

clean :
	-rm -f *.d *.o *.a test.mod relocateELF.test