#!/bin/bash
echo 'Copying OS...'
pwd
tool/mount.sh
cp bin/init /mnt/
cp bin/mm /mnt/mm.mod
tool/umount.sh
