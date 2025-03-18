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
#include "fs.h"

#include <linux/cred.h>


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


// 静态函数只能被当前源文件使用，类似于 C++ 类中的私有成员函数。不建议声明在头文件中，因为头文件可能被其他文件包含，可能会出现未知问题，同时可能也会降低链接速度。
static inline struct inode *nvmixNewInode(struct inode *);

static inline int nvmixAddLink(struct dentry *, struct inode *);


struct dentry *nvmixLookup(struct inode *pDir, struct dentry *pDentry, unsigned int flags)
{
    // TODO

    return NULL;
}

int nvmixCreate(struct inode *pDir, struct dentry *pDentry, umode_t mode, bool excl)
{
    int res = 0;
    struct inode *pInode = NULL;
    struct NvmixInodeHelper *pNih = NULL;


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

    pNih = NVMIX_I(pInode);
    pNih->m_dataBlockIndex = NVMIX_FIRST_DATA_BLOCK_INDEX + pInode->i_ino;

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
    struct super_block *pSb = NULL;
    struct NvmixSuperBlockHelper *pNsbh = NULL;
    struct inode *pInode = NULL;
    unsigned long index = 0;


    pSb = pDir->i_sb;
    pNsbh = (struct NvmixSuperBlockHelper *)(pSb->s_fs_info); // 含义解释见 fs.c。

    // find_first_zero_bit() 是内核提供的一个位图操作函数，其作用是从给定的位图中查找第一个值为 0 的位，即未使用的空闲资源。m_imap 是我们自己定义的管理 inode 分配状态的位图信息，类型是 unsigned long，刚好 4 个字节，32 位。每一位用于标识分配状态。
    index = find_first_zero_bit(&pNsbh->m_imap, NVMIX_MAX_INODE_NUM);
    if (NVMIX_MAX_INODE_NUM == index)
    {
        pr_err("nvmixfs: no space left in imap.\n");

        goto ERR;
    }

    // 原子性地测试并设置位图中的目标位，维护 m_imap 状态。
    test_and_set_bit(index, &pNsbh->m_imap);
    // 标记缓冲区为脏，表示内容已被修改，需要写回磁盘。
    mark_buffer_dirty(pNsbh->m_pBh);

    pInode = new_inode(pSb);                // 此函数创建新的 inode 结构，并关联到文件系统的超级块。
    pInode->i_uid = current_fsuid();        // 用户所有者 ID。
    pInode->i_gid = current_fsgid();        // 组所有者 ID。
    pInode->i_ino = index;                  // inode 编号。
    pInode->i_mtime = current_time(pInode); // 修改时间 Modification Time，作用对象是文件内容。
    pInode->i_atime = current_time(pInode); // 访问时间 Access Time，作用对象是文件内容。
    pInode->i_ctime = current_time(pInode); // 变更时间 Change Time，作用对象是 inode 元数据。
    pInode->i_blocks = 0;                   // 初始化 inode 的数据块占用数。

    // 将新创建的 inode 插入到内核维护的全局 inode 哈希表。
    insert_inode_hash(pInode);


ERR:
    return pInode;
}

int nvmixAddLink(struct dentry *pDentry, struct inode *pInode)
{
    // TODO

    return 0;
}
