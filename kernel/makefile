export ASM = nasm
export LD = ld
export CC = gcc
export ASMFLAG = -f elf
export LDFLAG = $(DIR_LIB:%=-L%) $(LIB:%=-l%)
export CFLAG = -Wall -Wextra -Wconversion -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables

export DIR_MAIN = $(shell pwd)
export DIR_BIN = $(DIR_MAIN)/bin
export DIR_LIB = $(DIR_MAIN)/lib
export DIR_INCLUDE = $(DIR_MAIN)/include $(DIR_MAIN)/lib/include
export DIR_IMG = /mnt
export LIB = DPOS_main
export PATH_LIB = $(patsubst %,$(DIR_LIB)/%,$(LIB:%=lib%.a))

MOD_EXCLUDE = bin init include lib tool
MOD = $(filter-out $(MOD_EXCLUDE),$(patsubst ./%,%,$(shell find . -mindepth 1 -maxdepth 1 -type d)))
OBJ = $(MOD:%=%.o)
BIN = lib init $(MOD)

.PHONY : $(BIN) build run clean test mount umount copy bochs newmodule

run : build mount copy umount bochs

build : $(BIN)

$(BIN):
	$(MAKE) -C $@

copy : $(patsubst %,$(DIR_IMG)/%,init $(MOD:%=%.mod))

$(DIR_IMG)/init : $(DIR_BIN)/init
	sudo cp $^ $@

$(DIR_IMG)/%.mod : $(DIR_BIN)/%
	sudo cp $^ $@

mount :
	@sudo tool/mount.sh

umount :
	@sudo tool/umount.sh

bochs :
	@read -p "Press any key to start bochs" _
	bochs -q

newmodule :
	@tool/new_module.sh

clean :
	-rm $(DIR_BIN)/*

#test: build
#	python3 test/main.py