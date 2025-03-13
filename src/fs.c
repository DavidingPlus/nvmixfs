/**
 * @file fs.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file_system_type 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "fs.h"

#include "inode.h"

#include <linux/fs.h>
#include <linux/export.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>


const struct file_system_type nvmixFileSystemType = {
    .owner = THIS_MODULE,
    .name = "nvmixfs",
    .mount = nvmixMount,
    .kill_sb = nvmixKillSb,
    .fs_flags = FS_REQUIRES_DEV, // 表示本系统是基于块设备的文件系统。
};

const struct super_operations nvmixSuperOps = {
    .statfs = simple_statfs,
    // put_super 的作用是在文件系统卸载或不再需要超级块时，释放与该超级块关联的资源。
    .put_super = nvmixPutSuper,
    .alloc_inode = nvmixAllocInode,
    .destroy_inode = nvmixDestroyInode,
    .write_inode = nvmixWriteInode,
};


struct dentry *nvmixMount(struct file_system_type *pFileSystemType, int flags, const char *pDevName, void *pData)
{
    struct dentry *res = NULL;


    // mount_bdev()：挂载存储在块设备上的文件系统。
    // mount_single()：挂载在所有挂载操作之间共享实例的文件系统。
    // mount_nodev()：挂载不在物理设备上的文件系统。
    // mount_pseudo()：用于伪文件系统的辅助函数（如 sockfs, pipefs 等无法被挂载的文件系统）。
    res = mount_bdev(pFileSystemType, flags, pDevName, pData, nvmixFillSuper);

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
    struct NvmixSuperBlockInfo *pNsbi = NULL;


    // s_fs_info 类似于 file 结构的 private_data，是文件系统中可被我们自己定义的私有数据信息。s_fs_info 在 fill_super 时会被初始化。这里拿到该部分数据以推进后续代码。
    pNsbi = (struct NvmixSuperBlockInfo *)(pSb->s_fs_info);

    // 标记缓冲区为脏，表示内容已被修改，需要写回磁盘。
    mark_buffer_dirty(pNsbi->m_pBh);
    // 释放缓冲区头。
    brelse(pNsbi->m_pBh);
    pNsbi->m_pBh = NULL;

    pr_info("nvmixfs: released super block resources.\n");
}

struct inode *nvmixAllocInode(struct super_block *pSb)
{
    struct NvmixInodeInfo *pNii = NULL;

    // kzalloc() 与 kmalloc() 的区别在于 kzalloc() 会把动态开辟的内存的内容置 0。
    pNii = kzalloc(sizeof(struct NvmixInodeInfo), GFP_KERNEL);
    if (!pNii)
    {
        pr_err("nvmixfs: failed to allocate inode.\n");


        return NULL;
    }

    // inode_init_once() 是内核中与 inode 对象初始化相关的函数，通常与 Slab 分配器配合使用。核心作用是为新分配的 inode 对象设置初始状态，确保其关键字段（如锁、链表、引用计数等）在首次使用时处于合法状态。
    inode_init_once(&pNii->m_vfsInode);

    pr_info("nvmixfs: allocated inode successfully.\n");


    return &pNii->m_vfsInode;
}

void nvmixDestroyInode(struct inode *pInode)
{
    // kfree() 是内核用于释放动态分配内存的函数。释放由 kmalloc()、kzalloc()、kmem_cache_alloc() 等内核内存分配函数申请的内存。
    // kzfree() 的区别是先清 0 再释放内存，避免敏感内存内容的残留。
    // 当前版本内核为 5.4，5.15 中 kzfree() 接口已废弃，转而使用 kfree_sensitive()。
    kzfree(NVMIX_I(pInode));

    pr_info("nvmixfs: destroyed inode successfully.\n");
}

int nvmixWriteInode(struct inode *pInode, struct writeback_control *pWbc)
{
    // TODO

    return 0;
}
