# 遇到错误立即退出。
set -e

./clear.sh

sudo insmod ./build/linux/x86_64/debug/bin/nvmixfs.ko

sudo ./build/linux/x86_64/debug/snippet/mkfs.nvmixfs /dev/sdb2

sudo mount -t nvmixfs /dev/sdb2 /mnt/nvmixfs

