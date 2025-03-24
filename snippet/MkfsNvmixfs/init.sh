# 遇到错误立即退出。
set -e


if mountpoint -q /mnt/nvmixfs/; then
    sudo umount /mnt/nvmixfs/

fi

if lsmod | grep -q "nvmixfs"; then
    sudo rmmod nvmixfs

fi

sudo insmod ./build/linux/x86_64/debug/bin/nvmixfs.ko

sudo ./build/linux/x86_64/debug/snippet/mkfs.nvmixfs /dev/sdb2

sudo mount -t nvmixfs /dev/sdb2 /mnt/nvmixfs

