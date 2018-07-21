DIR_INCLUDE += $(DIR_MAIN)/init

include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += -nostdinc
DEPEND += $(PATH_LIB)
LDFLAG += -r

.PHONY : all

all : $(DIR_BIN)/control.mod

$(DIR_BIN)/control.mod : $(DEPEND) $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -Tlinker.ld -o $@