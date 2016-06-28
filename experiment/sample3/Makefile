#本Makefile由Dapper编写

filename_main = ldt_real
.PHONY:debug list clean

${filename_main}.img:${filename_main}.bin
	dd if=${filename_main}.bin of=${filename_main}.img bs=512 count=1 conv=notrunc

${filename_main}.bin:${filename_main}.asm
	nasm ${filename_main}.asm  -o ${filename_main}.bin

debug:${filename_main}.img
	bochs -q <<EOF; /
	c;/
	EOF

list:${filename_main}.asm
	nasm ${filename_main}.asm -l ${filename_main}.log -o ${filename_main}.bin
	subl ${filename_main}.log

clean:
	-rm ${filename_main}.log
	-rm ${filename_main}.bin