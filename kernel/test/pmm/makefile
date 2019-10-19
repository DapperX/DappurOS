-include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

CC := gcc
# DIR_INCLUDE = $(DIR_MAIN)/include $(DIR_MAIN)/fs/ext2

DIR_LIB_FAKE = $(DIR_MAIN)/tool/fakeLib
LIB_FAKE = DPOS_fake
PATH_LIB_FAKE = $(DIR_LIB_FAKE)/lib$(LIB_FAKE).a

LIB := $(LIB_FAKE) $(LIB)
DIR_LIB += $(DIR_LIB_FAKE)
PATH_LIB += $(PATH_LIB_FAKE)

DIR_MM = $(DIR_MAIN)/mm
DIR_INCLUDE += $(DIR_MM)
CFLAG += -Werror
DEPEND += $(PATH_LIB)
OBJ_EXTRA += $(DIR_MM)/mm.o

.PHONY : build all clean

all : build
	./test.sh

build : pmm.test

pmm.test : $(OBJ) $(OBJ_EXTRA)
	$(CC) $^ $(LDFLAG) -o $@

clean :
	-rm -f *.d *.o *.a