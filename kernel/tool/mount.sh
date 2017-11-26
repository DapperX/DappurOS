#!/bin/bash
echo -e "\033[31mmounting...\033[0m"
pwd
losetup /dev/loop1 disk.img -o 1048576
mount /dev/loop1 /mnt
