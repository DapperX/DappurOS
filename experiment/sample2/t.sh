nasm backup.asm -o t.bin
dd if=t.bin of=t.img bs=512 count=1 conv=notrunc
bochs -f bochs_t
