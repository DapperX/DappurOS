#!/bin/bash
echo -e "\033[31mumounting...\033[0m"
umount /mnt/DPOS
losetup -d /dev/loop1
