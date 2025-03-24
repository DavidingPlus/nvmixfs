/**
 * @file inode.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief inode 结构头文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_INODE_H_
#define _NVMIX_INODE_H_

#include <linux/fs.h>


/**
 * @struct NvmixInodeHelper
 * @brief 将内存中的 vfs inode 和磁盘上 NvmixInode 结构（元数据）关联起来，媒介是逻辑块号 m_dataBlockIndex。
 * @details 此结构不是磁盘上的元数据，NvmixInode 才是。NvmixInode 不能存储内存中 vfs 的数据结构，但需要一个东西将 vfs 和本文件系统自己的 inode 元数据 NvmixInode 联系起来。这就是本结构的用途。
 */
struct NvmixInodeHelper
{
    /**
     * @brief 内核的 vfs inode 结构。
     */
    struct inode m_vfsInode;

    /**
     * @brief inode 对应的数据块的逻辑块号。
     * @detail 目前本文件系统最多 32 个 inode，即最多 32 个数据块，因此目前使用 unsigned short 类型。
     */
    unsigned short m_dataBlockIndex;
};


/**
 * @brief 通过有效的 vfs inode 成员获得 NvmixInodeHelper 结构指针。
 * @details container_of() 宏通过结构体的某个成员变量 member 的地址 ptr，反向找到整个结构体的起始地址。注意因为参数 pVfsInode 是成员的地址，所以 NvmixInodeHelper 中必须是 struct inode 而非指针 struct inode *，这样才能编译通过。
 * @details 此命名参考 EXT4_I、BTRFS_I、XFS_I 等知名文件系统。
 */
#define NVMIX_I(pVfsInode) container_of(pVfsInode, struct NvmixInodeHelper, m_vfsInode)


struct dentry *nvmixLookup(struct inode *, struct dentry *, unsigned int);

int nvmixCreate(struct inode *, struct dentry *, umode_t, bool);

int nvmixUnlink(struct inode *, struct dentry *);


#endif
