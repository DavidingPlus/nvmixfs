/**
 * @file inode.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief inode 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "inode.h"

#include "defs.h"


// 目录 inode 的操作接口。
struct inode_operations nvmixDirInodeOps = {
    .lookup = nvmixLookup,
    .create = nvmixCreate,
};

// 文件 inode 的操作接口。
struct inode_operations nvmixFileInodeOps = {
    .getattr = simple_getattr,
};

// 由进程打开的文件（用 file 结构描述）的操作接口。
extern struct file_operations nvmixFileOps;


struct dentry *nvmixLookup(struct inode *pDir, struct dentry *pDentry, unsigned int flags)
{
    // TODO

    return NULL;
}

int nvmixCreate(struct inode *pDir, struct dentry *pDentry, umode_t mode, bool excl)
{
    int res = 0;
    struct inode *pInode = NULL;
    // 创建新的 inode 时需要维护我们自己的 inode 元数据。
    struct NvmixInodeInfo *pNii = NULL;


    pInode = nvmixNewInode(pDir);
    if (!pInode)
    {
        pr_err("nvmixfs: error when allocating a new inode.\n");

        res = -ENOMEM;
        goto ERR;
    }

    pInode->i_mode = mode;
    pInode->i_op = &nvmixFileInodeOps;
    pInode->i_fop = &nvmixFileOps;

    pNii = container_of(pInode, struct NvmixInodeInfo, m_vfsInode);
    pNii->m_dataBlock = NVMIX_FIRST_DATA_BLOCK + pInode->i_ino;

    // 将新 inode 关联到父目录的目录项 dentry 中，会维护并修改父目录项的一些信息。与下面的 d_instantiate() 作用不同，注意区分。
    res = nvmixAddLink(pDentry, pInode);
    if (0 != res)
    {
        // 减少 inode 的硬链接计数。
        inode_dec_link_count(pInode);
        // 释放 inode 的引用计数。
        iput(pInode);

        goto ERR;
    }

    // dentry 作用是关联 inode 和文件名。d_instantiate() 将 dentry 与 inode 绑定，使文件名正确指向文件。
    d_instantiate(pDentry, pInode);
    // 标记 inode 为脏，表示其元数据（如权限、大小）或数据已修改，需后续同步到磁盘。
    mark_inode_dirty(pInode);

    pr_info("nvmixfs: new file inode created successfully, ino = %lu\n", pInode->i_ino);


ERR:
    return res;
}


// 此函数的语义是在传入目录下创建新 inode。参数 struct inode* pDir 代表一个目录，返回的 inode 在当前 create() 语义下是一个文件。创建目录有函数 mkdir()。
struct inode *nvmixNewInode(struct inode *pDir)
{
    // TODO

    return (struct inode *)NULL;
}

int nvmixAddLink(struct dentry *pDentry, struct inode *pInode)
{
    // TODO

    return 0;
}
