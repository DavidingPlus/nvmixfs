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
#include "defs.h"

#include <linux/fs.h>
#include <linux/export.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>


struct file_system_type nvmixFileSystemType = {
    .owner = THIS_MODULE,
    .name = "nvmixfs",
    .mount = nvmixMount,
    .kill_sb = nvmixKillSb,
    .fs_flags = FS_REQUIRES_DEV, // 表示本系统是基于块设备的文件系统。
};

struct super_operations nvmixSuperOps = {
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
    struct NvmixSuperBlockHelper *pNsbh = NULL;


    // s_fs_info 类似于 file 结构的 private_data，是文件系统中可被我们自己定义的私有数据信息。s_fs_info 在 fill_super 时会被初始化。这里拿到该部分数据以推进后续代码。
    pNsbh = (struct NvmixSuperBlockHelper *)(pSb->s_fs_info);

    // 标记缓冲区为脏，表示内容已被修改，需要写回磁盘。
    mark_buffer_dirty(pNsbh->m_pBh);
    // 释放缓冲区头。
    brelse(pNsbh->m_pBh);
    pNsbh->m_pBh = NULL;

    pr_info("nvmixfs: released super block resources.\n");
}

struct inode *nvmixAllocInode(struct super_block *pSb)
{
    struct NvmixInodeHelper *pNih = NULL;

    // kzalloc() 与 kmalloc() 的区别在于 kzalloc() 会把动态开辟的内存的内容置 0。
    pNih = kzalloc(sizeof(struct NvmixInodeHelper), GFP_KERNEL);
    if (!pNih)
    {
        pr_err("nvmixfs: failed to allocate inode.\n");


        return NULL;
    }

    // inode_init_once() 是内核中与 inode 对象初始化相关的函数，通常与 Slab 分配器配合使用。核心作用是为新分配的 inode 对象设置初始状态，确保其关键字段（如锁、链表、引用计数等）在首次使用时处于合法状态。
    inode_init_once(&pNih->m_vfsInode);

    pr_info("nvmixfs: allocated inode successfully.\n");


    return &pNih->m_vfsInode;
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
    struct super_block *pSb = NULL;
    struct buffer_head *pBh = NULL;
    struct NvmixInode *pNi = NULL;
    struct NvmixInodeHelper *pNih = NULL;
    int res = 0;


    pSb = pInode->i_sb;
    // 从 inode 区所在磁盘块读取数据。
    pBh = sb_bread(pSb, NVMIX_INODE_BLOCK_INDEX);
    if (!pBh)
    {
        pr_err("nvmixfs: could not read inode block.\n");

        res = -ENOMEM;
        goto ERR;
    }

    // inode 区存储的是 NvmixInode 数组，也需要偏移。获取地址的逻辑同 dir.c 中 nvmixReaddir()。
    pNi = (struct NvmixInode *)(pBh->b_data) + pInode->i_ino;

    pNi->m_mode = pInode->i_mode;
    pNi->m_uid = i_uid_read(pInode);
    pNi->m_gid = i_gid_read(pInode);
    pNi->m_size = pInode->i_size;

    pNih = NVMIX_I(pInode);
    pNi->m_dataBlockIndex = pNih->m_dataBlockIndex;

    pr_info("nvmixfs: m_mode is %05o; m_dataBlockIndex is %d.\n", pNi->m_mode, pNi->m_dataBlockIndex);

    mark_buffer_dirty(pBh);

    pr_info("nvmixfs: wrote inode %lu successfully.\n", pInode->i_ino);


ERR:
    brelse(pBh);
    pBh = NULL;


    return res;
}

// 此函数通过 super_block 结构和 vfs 中全局唯一 inode 号获得 inode。包括如下两种情况：
// 1. 从缓存中获取 inode：若 inode 已存在于内存中（缓存命中），直接返回。
// 2. 初始化新 inode：若 inode 未缓存（I_NEW 状态），从磁盘读取元数据并初始化 vfs inode。
struct inode *nvmixIget(struct super_block *pSb, unsigned long ino)
{
    // TODO

    return (struct inode *)NULL;
}
