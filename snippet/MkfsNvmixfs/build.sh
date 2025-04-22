#!/bin/bash

# 遇到错误立即退出。
set -e


source "$(dirname "$0")/log.sh"


CURRENT_DIR="$(dirname "$(realpath "$0")")"
log "当前目录: $CURRENT_DIR"

KERNEL_HEADER_DIR="/usr/src/linux-headers-$(uname -r)/"
log "内核头文件目录: $KERNEL_HEADER_DIR"


log "执行清理脚本 clear.sh..."
$CURRENT_DIR/clear.sh

log "切换到项目根目录..."
cd "$CURRENT_DIR/../../"

log "创建构建标志文件 .buildme（若已存在则跳过）..."
touch "$CURRENT_DIR/.buildme"

log "配置 xmake 构建参数..."
xmake f -m debug --linux-headers="$KERNEL_HEADER_DIR" --with-gtest=true -y

log "开始构建项目..."
xmake build

log "创建挂载目录 /mnt/nvmixfs/（如果不存在）..."
sudo mkdir -p /mnt/nvmixfs/

NVM_PHY_ADDR=0x100000000
NVM_PHY_SIZE=0x10000000

log "加载 nvmixfs 内核模块..."
log "使用的物理地址: $NVM_PHY_ADDR, 大小: $NVM_PHY_SIZE"
# 我本地预留了一段内存，起始物理地址是 0x100000000，大小是 0x10000000，即 256 M。
sudo insmod "$CURRENT_DIR/../../build/linux/x86_64/debug/bin/nvmixfs.ko" nvmixNvmPhyAddr=$NVM_PHY_ADDR nvmixNvmPhySize=$NVM_PHY_SIZE

log "使用 mkfs.nvmixfs 格式化设备 /dev/sdb2..."
sudo "$CURRENT_DIR/../../build/linux/x86_64/debug/snippet/mkfs.nvmixfs" /dev/sdb2

log "挂载设备 /dev/sdb2 到 /mnt/nvmixfs/..."
sudo mount -t nvmixfs /dev/sdb2 /mnt/nvmixfs

log "所有操作完成 ✅"
