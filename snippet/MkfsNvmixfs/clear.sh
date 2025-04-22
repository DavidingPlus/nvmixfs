#!/bin/bash

set -e


# 绿色 INFO 日志输出
log() {
    echo -e "[\033[1;32mINFO\033[0m] $*"
}


log "开始清理挂载点和内核模块..."

if mountpoint -q /mnt/nvmixfs/; then
    log "发现 /mnt/nvmixfs/ 已挂载，正在卸载..."
    sudo umount /mnt/nvmixfs/

else
    log "/mnt/nvmixfs/ 未挂载，跳过卸载步骤。"

fi

if lsmod | grep -q "nvmixfs"; then
    log "发现 nvmixfs 模块已加载，正在卸载..."
    sudo rmmod nvmixfs

else
    log "nvmixfs 模块未加载，跳过 rmmod。"

fi

if [[ -d "/mnt/nvmixfs/" ]]; then
    log "删除目录 /mnt/nvmixfs/..."
    sudo rm -rf /mnt/nvmixfs/

else
    log "目录 /mnt/nvmixfs/ 不存在，跳过删除。"

fi


log "清理完成 ✅"
