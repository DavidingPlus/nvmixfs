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
#include <linux/buffer_head.h>


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


struct dentry *nvmixLookup(struct inode *pParentDirInode, struct dentry *pDentry, unsigned int flags)
{
    // TODO

    return NULL;
}

// 这个接口的参数逆天。pParentDirInode 是父目录的 inode 节点，在函数里我需要手动创建新的 vfs inode。而 pDentry 却是新 inode 节点对应的 dentry 对象，内核帮我创建好了。很容易误解为父目录的 dentry，我们需要自己手动创建 dentry，但是内核似乎并没有这种函数。
int nvmixCreate(struct inode *pParentDirInode, struct dentry *pDentry, umode_t mode, bool excl)
{
    int res = 0;
    struct inode *pInode = NULL;
    struct NvmixInodeHelper *pNih = NULL;


    pInode = nvmixNewInode(pParentDirInode);
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
    // 注意此 pDentry 是 pInode 对应的 pDentry，而非父目录的 dentry，前面提到过。
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


// 此函数的语义是在父目录下创建新 inode。参数 struct inode* pParentDirInode 代表父目录。在 create() 语义下返回的 inode 是一个文件。创建目录有函数 mkdir()。
struct inode *nvmixNewInode(struct inode *pParentDirInode)
{
    struct super_block *pSb = NULL;
    struct NvmixSuperBlockHelper *pNsbh = NULL;
    struct inode *pInode = NULL;
    unsigned long index = 0;


    pSb = pParentDirInode->i_sb;
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

// 在 nvmixCreate() 函数中使用，将新 inode 关联到父目录的目录项 dentry 中，会维护并修改父目录项的一些信息。
int nvmixAddLink(struct dentry *pDentry, struct inode *pInode)
{
    struct inode *pParentDirInode = NULL;
    struct NvmixInodeHelper *pNih = NULL;
    struct super_block *pSb = NULL;
    struct buffer_head *pBh = NULL;
    struct NvmixDentry *pNd = NULL;
    int i = 0;
    int res = 0;


    pParentDirInode = pDentry->d_parent->d_inode;
    pNih = NVMIX_I(pParentDirInode);
    pSb = pParentDirInode->i_sb;

    pBh = sb_bread(pSb, pNih->m_dataBlockIndex);
    if (!pBh)
    {
        pr_err("nvmixfs: could not read data block.\n");

        res = -ENOMEM;
        goto ERR;
    }

    // 找到第一个空闲的 NvmixDentry，即 inode 号为 0。
    for (i = 0; i < NVMIX_MAX_ENTRY_NUM; ++i)
    {
        pNd = (struct NvmixDentry *)(pBh->b_data) + i;

        if (0 == pNd->m_ino) break;
    }

    // 判断父目录是否已经满了。
    if (NVMIX_MAX_ENTRY_NUM == i)
    {
        pr_err("nvmixfs: failed to add link, because parent directory is full.\n");

        res = -ENOSPC;
        goto ERR;
    }

    // 填充磁盘上新目录项的元数据。
    pNd->m_ino = pInode->i_ino;
    strncpy(pNd->m_name, pDentry->d_name.name, NVMIX_MAX_NAME_LENGTH);

    // 修改父目录的 Modified Time 和 Changed Time，维护 vfs 的数据结构。
    pParentDirInode->i_mtime = current_time(pInode);
    pParentDirInode->i_ctime = current_time(pInode);

    mark_buffer_dirty(pBh);


ERR:
    brelse(pBh);
    pBh = NULL;


    return res;
}
