export ASM = nasm
export LD = ld
export CC = gcc
export ASMFLAG = -f elf
export LDFLAG = -static
export CFLAG = -Wall -Wextra -Wconversion -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables

export DIR_MAIN = $(shell pwd)
export DIR_BIN = $(DIR_MAIN)/bin
export DIR_LIB = $(DIR_MAIN)/lib
export DIR_INCLUDE = $(DIR_MAIN)/include $(DIR_LIB)/include
export DIR_IMG = /mnt/DPOS
export LIB = DPOS_main
export PATH_LIB = $(patsubst %,$(DIR_LIB)/%,$(LIB:%=lib%.a))

ifeq ($(MODE),nodebug)
	CFLAG += -DNDEBUG
endif


BIN_EXT = init
MOD_EXCLUDE = $(BIN_EXT) bin include lib tool test
MOD = $(filter-out $(MOD_EXCLUDE),$(patsubst ./%,%,$(shell find . -mindepth 1 -maxdepth 1 -type d)))

.PHONY : $(BIN_EXT) $(MOD) build run clean mount umount copy bochs newmodule lib test

run : build mount copy umount bochs

build : $(BIN_EXT) $(MOD)

$(BIN_EXT) $(MOD) test : lib

$(BIN_EXT) $(MOD) test lib :
	$(MAKE) all -C $@

copy : $(patsubst %,$(DIR_IMG)/%,$(BIN_EXT) $(MOD:%=%.mod))

$(DIR_IMG)/% : $(DIR_BIN)/%
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