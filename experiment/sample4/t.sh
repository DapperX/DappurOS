nasm gate.asm -o gate.bin && dd if=gate.bin of=gate.img bs=512 count=1 conv=notrunc && bochs -q

