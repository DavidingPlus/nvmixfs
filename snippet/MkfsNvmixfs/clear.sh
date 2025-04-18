#!/bin/bash

# 遇到错误立即退出。
set -e


if mountpoint -q /mnt/nvmixfs/; then
    sudo umount /mnt/nvmixfs/

fi

if lsmod | grep -q "nvmixfs"; then
    sudo rmmod nvmixfs

fi

if [[ -d "/mnt/nvmixfs/" ]]; then
    sudo rm -rf /mnt/nvmixfs/

fi
