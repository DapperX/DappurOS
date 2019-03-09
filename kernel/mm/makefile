DIR_INCLUDE += $(DIR_MAIN)/init

include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += -nostdinc
DEPEND += $(PATH_LIB)
LDFLAG += -r

.PHONY : all

all : $(DIR_BIN)/mm.mod

$(DIR_BIN)/mm.mod : $(DEPEND)

$(DIR_BIN)/mm.mod : $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -Tlinker.ld -o $@