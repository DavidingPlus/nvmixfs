/**
 * @file fs.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file_system_type 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "fs.h"

#include <linux/fs.h>
#include <linux/export.h>


struct file_system_type nvmixFileSystemType = {
    .owner = THIS_MODULE,
    .name = "nvmixfs",
    .mount = nvmixMount,
    .kill_sb = nvmixKillSb,
    .fs_flags = FS_REQUIRES_DEV, // 需要一个块设备，本系统是基于块设备的文件系统。
    .next = NULL,
};


struct dentry *nvmixMount(struct file_system_type *pFilsSystemType, int flags, const char *pDevName, void *data)
{
    // TODO

    return NULL;
}

void nvmixKillSb(struct super_block *pSuperBlock)
{
    // TODO
}
