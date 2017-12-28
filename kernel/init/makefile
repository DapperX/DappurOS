include $(DIR_MAIN)/tool/template/compile_with_dependence.makefile

ASMFLAG += $(DIR_INCLUDE:%=-i%/)
CFLAG += -nostdinc
DEPEND += $(PATH_LIB)
LDFLAG += -s

.PHONY : all

all : $(DIR_BIN)/init

$(DIR_BIN)/init : $(DEPEND) $(OBJ) linker.ld
	$(LD) $^ $(LDFLAG) -Tlinker.ld -o $@