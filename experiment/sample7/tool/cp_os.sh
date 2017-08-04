#!/bin/bash
echo 'Copying OS...'
pwd
tool/mount.sh
cp bin/init /mnt/
tool/umount.sh
