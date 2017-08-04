export ASM = nasm
export LD = ld
export CC = gcc
export ASMFLAG = -f elf
export LDFLAG = $(DIR_LIB:%=-L%) $(LIB:%=-l%)
export CFLAG = -g -Wall -Wextra -Wconversion -fno-builtin -fno-stack-protector

export DIR_MAIN = $(shell pwd)
export DIR_BIN = $(DIR_MAIN)/bin
export DIR_LIB = $(DIR_MAIN)/lib
export DIR_INCLUDE = $(DIR_MAIN)/include $(DIR_MAIN)/lib/include
export LIB = DPOS_main
export PATH_LIB = $(patsubst %,$(DIR_LIB)/%,$(LIB:%=lib%.a))

MOD_EXCLUDE = bin include lib tool
MOD = $(filter-out $(MOD_EXCLUDE),$(patsubst ./%,%,$(shell find . -mindepth 1 -maxdepth 1 -type d)))
OBJ = $(MOD:%=%.o)

.PHONY : lib $(MOD) build run clean test

run : build
	sudo tool/cp_os.sh
	bochs -q

build : lib $(MOD)

lib :
	$(MAKE) -C lib

$(MOD):
	$(MAKE) -C $@

clean : 
	-rm $(DIR_BIN)/*

test:
	echo $@