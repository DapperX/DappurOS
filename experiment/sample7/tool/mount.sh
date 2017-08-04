#!/bin/bash
echo 'mounting...'
pwd
losetup /dev/loop1 disk.img -o 1048576
mount /dev/loop1 /mnt
