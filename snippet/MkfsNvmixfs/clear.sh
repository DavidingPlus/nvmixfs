set -e


if mountpoint -q /mnt/nvmixfs/; then
    sudo umount /mnt/nvmixfs/

fi

if lsmod | grep -q "nvmixfs"; then
    sudo rmmod nvmixfs

fi
