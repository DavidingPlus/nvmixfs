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
    .fs_flags = FS_REQUIRES_DEV, // 表示本系统是基于块设备的文件系统。
};

struct super_operations nvmixSuperOps = {
    .statfs = simple_statfs,
    .put_super = nvmixPutSuper,
    .alloc_inode = nvmixAllocInode,
    .destroy_inode = nvmixDestroyInode,
    .write_inode = nvmixWriteInode,
};


struct dentry *nvmixMount(struct file_system_type *pFileSystemType, int flags, const char *pDevName, void *pData)
{
    // mount_bdev()：挂载存储在块设备上的文件系统。
    // mount_single()：挂载在所有挂载操作之间共享实例的文件系统。
    // mount_nodev()：挂载不在物理设备上的文件系统。
    // mount_pseudo()：用于伪文件系统的辅助函数（如 sockfs, pipefs 等无法被挂载的文件系统）。
    struct dentry *res = mount_bdev(pFileSystemType, flags, pDevName, pData, nvmixFillSuper);

    // 使用 IS_ERR() 函数检测是否为错误指针。
    if (IS_ERR(res))
    {
        pr_err("nvmixfs: failed to mount %s.\n", pDevName);
    }
    else
    {
        pr_info("nvmixfs: mounted %s successfully.\n", pDevName);
    }


    return res;
}

void nvmixKillSb(struct super_block *pSuperBlock)
{
    // kill_block_super()：卸载块设备上的文件系统。
    // kill_anon_super()：卸载虚拟文件系统（当请求时生成信息）。
    // kill_litter_super()：卸载不在物理设备上的文件系统（信息保存在内存中）。
    kill_block_super(pSuperBlock);

    pr_info("nvmixfs: unmounted disk successfully.\n");
}

int nvmixFillSuper(struct super_block *pSuperBlock, void *pData, int silent)
{
    // TODO

    return 0;
}

void nvmixPutSuper(struct super_block *pSb)
{
    // TODO

    return;
}

struct inode *nvmixAllocInode(struct super_block *pSb)
{
    // TODO

    return (struct inode *)NULL;
}

void nvmixDestroyInode(struct inode *pInode)
{
    // TODO

    return;
}

int nvmixWriteInode(struct inode *pInode, struct writeback_control *pWbc)
{
    // TODO

    return 0;
}
