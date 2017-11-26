#!/bin/bash
echo -e "\033[31mumounting...\033[0m"
umount /mnt
losetup -d /dev/loop1
