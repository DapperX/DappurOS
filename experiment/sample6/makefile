.PHONY : build run clean


header.o : header.asm
	nasm $< -f elf -o $@

init.o : init.asm
	nasm $< -f elf -o $@

lib_asm.o : lib.asm
	nasm $< -f elf -o $@

lib_c.o : lib.c
	gcc $< -fno-builtin-putchar -fno-builtin-printf -fno-stack-protector -c -o $@

kernel.o : kernel.c
	gcc $< -fno-builtin-putchar -fno-builtin-printf -fno-stack-protector -c -o $@

kernel : header.o lib_asm.o lib_c.o init.o kernel.o
	ld $^ -Ttext 0x400 -o $@

build : kernel

run : kernel
	sudo bash cp_kernel.sh
	bochs -q

clean : 
	rm *.o